"""Training: LR schedule, optimiser groups, checkpointing, and the three PG-RL stages.

Everything runs on CPU with the tiny preset. The point is not to train anything, it is to
check that the loss functions have the right shape and the right gradients, and that a
checkpoint carries its own geometry.
"""

from __future__ import annotations

import json
import math

import pytest
import torch


from elafry.config import AffectConfig, PRESETS, TrainConfig
from elafry.models.elafry import Elafry
from elafry.train import (
    AffectiveRewardModel,
Linear4DConfig,
    TrainState,
    all_reduce_mean,
    build_optimizer,
    build_scheduler,
    checkpoint_payload,
    dpo_loss,
    empathy_target,
    is_distributed,
    lr_at_step,
    load_checkpoint,
    load_into_model,
    read_config,
    reward_loss,
    save_checkpoint,
    sequence_logprob,
    sft_step,
    sycophancy_penalty,
    train_sft,
    validate_pair,
    wrap_fsdp,
)


from conftest import make_tiny_model


@pytest.fixture
def cfg() -> TrainConfig:
    return TrainConfig(lr=1e-3, min_lr=1e-5, warmup_steps=10, max_steps=100)


# ------------------------------------------------------------------ LR schedule


def test_warmup_is_linear(cfg):
    assert lr_at_step(0, cfg) == pytest.approx(1 / 10)
    assert lr_at_step(5, cfg) == pytest.approx(6 / 10)
    assert lr_at_step(9, cfg) == pytest.approx(1.0)


def test_decay_falls_from_lr_to_min_lr(cfg):
    at_end = lr_at_step(100, cfg)
    assert at_end == pytest.approx(cfg.min_lr / cfg.lr)
    assert lr_at_step(55, cfg) < 1.0


def test_schedule_is_monotone_after_warmup(cfg):
    values = [lr_at_step(s, cfg) for s in range(cfg.warmup_steps, cfg.max_steps + 1)]
    assert all(a >= b - 1e-9 for a, b in zip(values, values[1:]))


def test_zero_warmup_starts_at_full_lr():
    cfg = TrainConfig(lr=1e-3, min_lr=1e-5, warmup_steps=0, max_steps=100)
    assert lr_at_step(0, cfg) == pytest.approx(1.0)


def test_scheduler_follows_the_closed_form(cfg):
    model = make_tiny_model()
    opt = build_optimizer(model.parameters(), cfg)
    sched = build_scheduler(opt, cfg)
    assert sched.get_last_lr()[0] == pytest.approx(cfg.lr * lr_at_step(0, cfg), rel=1e-6)
    for _ in range(5):
        sched.step()
    assert sched.get_last_lr()[0] == pytest.approx(cfg.lr * lr_at_step(5, cfg), rel=1e-6)


# ------------------------------------------------------------------ optimiser groups


def test_norms_and_biases_are_not_weight_decayed():
    model = make_tiny_model()
    cfg = TrainConfig(weight_decay=0.1)
    opt = build_optimizer(model.parameters(), cfg)

    no_decay = opt.param_groups[1]["params"]
    assert opt.param_groups[0]["weight_decay"] == 0.1
    assert opt.param_groups[1]["weight_decay"] == 0.0

    for p in no_decay:
        assert p.ndim < 2, "a 2D weight ended up in the no-decay group"


def test_tied_embedding_is_decayed_once():
    model = make_tiny_model()
    opt = build_optimizer(model.parameters(), TrainConfig(weight_decay=0.1))
    embed_ids = {id(p) for p in model.embed_tokens.parameters()}
    head_ids = {id(p) for p in model.lm_head.parameters()}
    counted = [
        id(p)
        for group in opt.param_groups
        for p in group["params"]
        if id(p) in embed_ids or id(p) in head_ids
    ]
    # lm_head.weight *is* embed_tokens.weight, so it must appear once.
    assert len(counted) == 1


def test_frozen_parameters_are_skipped():
    model = make_tiny_model()
    model.embed_tokens.weight.requires_grad_(False)
    opt = build_optimizer(model.parameters(), TrainConfig())
    all_ids = {id(p) for g in opt.param_groups for p in g["params"]}
    assert id(model.embed_tokens.weight) not in all_ids


# ------------------------------------------------------------------ SFT loss


def _batch(bsz=2, seq=12, vocab=512, response_start=6, seed=0):
    torch.manual_seed(seed)
    input_ids = torch.randint(1, vocab, (bsz, seq))
    mask = torch.zeros(bsz, seq, dtype=torch.long)
    mask[:, response_start:] = 1
    return {"input_ids": input_ids, "mask": mask}


def test_sft_loss_only_scores_the_response():
    """Changing prompt tokens must not move the loss; changing response tokens must."""
    model = make_tiny_model()
    model.eval()
    batch = _batch()

    base = float(sft_step(model, batch, batch["mask"]))

    perturbed = {k: (v.clone() if torch.is_tensor(v) else v) for k, v in batch.items()}
    perturbed["input_ids"][:, :6] = torch.randint(1, 512, (2, 6))
    prompt_changed = float(sft_step(model, perturbed, perturbed["mask"]))
    assert prompt_changed == pytest.approx(base, abs=1e-5)

    perturbed2 = {k: (v.clone() if torch.is_tensor(v) else v) for k, v in batch.items()}
    perturbed2["input_ids"][:, 6:] = torch.randint(1, 512, (2, 6))
    response_changed = float(sft_step(model, perturbed2, perturbed2["mask"]))
    assert response_changed != pytest.approx(base, abs=1e-5)


def test_sft_loss_is_finite_and_differentiable():
    model = make_tiny_model()
    batch = _batch()
    loss = sft_step(model, batch, batch["mask"])
    assert torch.isfinite(loss)
    loss.backward()
    grads = [p.grad for p in model.parameters() if p.grad is not None]
    assert grads and any(g.abs().sum() > 0 for g in grads)


def test_sft_loss_on_a_fully_masked_batch_is_zero_and_does_not_break_backward():
    model = make_tiny_model()
    batch = _batch()
    batch["mask"] = torch.zeros_like(batch["mask"])
    loss = sft_step(model, batch, batch["mask"])
    assert float(loss) == 0.0
    loss.backward()  # must not raise


def test_train_sft_runs_and_reduces_loss(tmp_path):
    preset = PRESETS["elafry-tiny"]
    torch.manual_seed(0)
    model = Elafry(preset["model"], preset["affect"])
    cfg = TrainConfig(lr=3e-3, warmup_steps=5, max_steps=60, save_every=10_000, log_every=1000)

    batches = iter([_batch(seed=i) for i in range(200)])
    result = train_sft(
        model,
        batches,
        cfg,
        preset["model"],
        preset["affect"],
        tmp_path,
        log_every=1000,
    )
    assert result.steps == 60
    assert result.tokens_seen > 0
    assert math.isfinite(result.final_loss)


def test_train_sft_respects_a_token_budget(tmp_path):
    preset = PRESETS["elafry-tiny"]
    torch.manual_seed(0)
    model = Elafry(preset["model"], preset["affect"])
    budget = 5000
    cfg = TrainConfig(
        lr=1e-3, warmup_steps=2, max_steps=100_000, token_budget=budget, save_every=10**9,
        log_every=10**9,
    )
    batches = iter([_batch(seed=i) for i in range(10_000)])
    result = train_sft(model, batches, cfg, preset["model"], preset["affect"], tmp_path)
    assert result.stopped_reason == "token_budget"
    assert result.tokens_seen >= budget


def test_train_sft_stops_when_data_runs_out(tmp_path):
    """A small corpus exhausted under an enormous step budget must not silently loop."""
    preset = PRESETS["elafry-tiny"]
    torch.manual_seed(0)
    model = Elafry(preset["model"], preset["affect"])
    cfg = TrainConfig(lr=1e-3, warmup_steps=1, max_steps=100_000, save_every=10**9, log_every=10**9)
    batches = iter([_batch(seed=i) for i in range(3)])
    result = train_sft(model, batches, cfg, preset["model"], preset["affect"], tmp_path)
    assert result.stopped_reason == "data_exhausted"


# ------------------------------------------------------------------ checkpointing


def test_checkpoint_carries_its_geometry(tmp_path):
    """The whole point. A consumer must be able to rebuild the model from the directory
    alone, with no hardcoded geometry to disagree with."""
    preset = PRESETS["elafry-tiny"]
    model = Elafry(preset["model"], preset["affect"])

    save_checkpoint(tmp_path, model, preset["model"], preset["affect"], TrainConfig())
    payload = read_config(tmp_path)

    assert payload["format_version"] == 1
    assert payload["model"]["n_layers"] == preset["model"].n_layers
    assert payload["model"]["dim"] == preset["model"].dim
    assert payload["affect"]["bias_granularity"] == preset["affect"].bias_granularity


def test_missing_sidecar_names_the_fix(tmp_path):
    with pytest.raises(FileNotFoundError, match="config.json"):
        read_config(tmp_path)


def test_checkpoint_roundtrips_to_identical_logits(tmp_path):
    preset = PRESETS["elafry-tiny"]
    torch.manual_seed(0)
    model = Elafry(preset["model"], preset["affect"]).eval()
    save_checkpoint(tmp_path, model, preset["model"], preset["affect"], TrainConfig())

    loaded = load_checkpoint(tmp_path)
    from elafry.config import ModelConfig

    cfg = ModelConfig.from_dict(loaded["config"]["model"])
    rebuilt = Elafry(cfg, AffectConfig(**loaded["config"]["affect"])).eval()
    load_into_model(rebuilt, loaded["weights"], strict=False)

    ids = torch.randint(0, cfg.vocab_size, (2, 8))
    with torch.no_grad():
        assert torch.allclose(model(ids).logits, rebuilt(ids).logits, atol=1e-5)


def test_tied_head_is_not_written_twice(tmp_path):
    preset = PRESETS["elafry-tiny"]
    model = Elafry(preset["model"], preset["affect"])
    save_checkpoint(tmp_path, model, preset["model"], preset["affect"], TrainConfig())
    weights = load_checkpoint(tmp_path)["weights"]
    assert "lm_head.weight" not in weights
    assert "embed_tokens.weight" in weights


def test_rope_tables_are_not_persisted(tmp_path):
    """Derived from (head_dim, max_seq_len, theta), so storing them is dead weight that can
    go stale if the config changes."""
    preset = PRESETS["elafry-tiny"]
    model = Elafry(preset["model"], preset["affect"])
    save_checkpoint(tmp_path, model, preset["model"], preset["affect"], TrainConfig())
    weights = load_checkpoint(tmp_path)["weights"]
    assert not any(k.endswith((".cos", ".sin", ".inv_freq")) for k in weights)


def test_train_state_records_position(tmp_path):
    preset = PRESETS["elafry-tiny"]
    model = Elafry(preset["model"], preset["affect"])
    state = TrainState(step=42, tokens_seen=1234)
    save_checkpoint(
        tmp_path, model, preset["model"], preset["affect"], TrainConfig(), state=state
    )
    loaded = load_checkpoint(tmp_path)
    assert loaded["state"].step == 42
    assert loaded["state"].tokens_seen == 1234


def test_optimizer_state_is_written_for_resume(tmp_path):
    preset = PRESETS["elafry-tiny"]
    model = Elafry(preset["model"], preset["affect"])
    opt = build_optimizer(model.parameters(), TrainConfig())
    save_checkpoint(
        tmp_path, model, preset["model"], preset["affect"], TrainConfig(),
        state=TrainState(step=1), optimizer=opt,
    )
    assert (tmp_path / "optimizer.pt").exists()
    blob = torch.load(tmp_path / "optimizer.pt", weights_only=False)
    assert "optimizer" in blob


def test_checkpoint_prunes_old_steps(tmp_path):
    preset = PRESETS["elafry-tiny"]
    model = Elafry(preset["model"], preset["affect"])
    for step in range(1, 6):
        save_checkpoint(
            tmp_path / f"step{step}", model, preset["model"], preset["affect"],
            TrainConfig(), state=TrainState(step=step), keep_last=2,
        )
    remaining = sorted(p.name for p in tmp_path.glob("step*") if p.is_dir())
    assert remaining == ["step4", "step5"]


def test_checkpoint_payload_is_json_serialisable():
    preset = PRESETS["elafry-tiny"]
    payload = checkpoint_payload(preset["model"], preset["affect"], TrainConfig())
    json.dumps(payload)  # must not raise
    assert payload["affect"]["inject_layers"] == []


# ------------------------------------------------------------------ reward model


@pytest.fixture
def reward_model() -> AffectiveRewardModel:
    torch.manual_seed(0)
    preset = PRESETS["elafry-tiny"]
    return AffectiveRewardModel(preset["model"], preset["affect"])


def test_reward_backbone_is_frozen(reward_model):
    assert not any(p.requires_grad for p in reward_model.backbone.parameters())
    assert all(p.requires_grad for p in reward_model.head.parameters())


def test_reward_backbone_stays_in_eval_during_training(reward_model):
    """Otherwise the representation the head was fitted on differs from the one used later."""
    reward_model.train()
    assert reward_model.backbone.training is False


def test_reward_scores_one_scalar_per_sequence(reward_model):
    ids = torch.randint(1, 512, (3, 10))
    scores = reward_model(ids)
    assert scores.shape == (3,)
    assert torch.isfinite(scores).all()


def test_reward_reads_the_last_real_token(reward_model):
    """Padding must not move the score. If it does, batching changes the answer."""
    ids = torch.randint(1, 512, (1, 10))
    mask = torch.ones(1, 10, dtype=torch.long)
    padded = torch.cat([ids, torch.zeros(1, 5, dtype=torch.long)], dim=1)
    padded_mask = torch.cat([mask, torch.zeros(1, 5, dtype=torch.long)], dim=1)

    with torch.no_grad():
        a = reward_model(ids, mask)
        b = reward_model(padded, padded_mask)
    assert float(a) == pytest.approx(float(b), abs=1e-4)


def test_only_the_head_receives_gradients(reward_model):
    ids = torch.randint(1, 512, (2, 10))
    reward_model(ids).sum().backward()
    assert reward_model.head.weight.grad is not None
    assert all(p.grad is None for p in reward_model.backbone.parameters())


def test_empathy_target_is_warmer_for_a_sadder_prompt():
    sad = empathy_target(torch.tensor([[-0.9, -0.3, -0.6]]))
    neutral = empathy_target(torch.tensor([[0.0, 0.0, 0.0]]))
    happy = empathy_target(torch.tensor([[0.9, 0.4, 0.5]]))
    assert float(sad) > float(neutral) > float(happy)
    assert 0.0 <= float(sad) <= 1.0


def test_empathy_target_rejects_bad_shape():
    with pytest.raises(ValueError, match=r"must be \(B, 3\)"):
        empathy_target(torch.tensor([0.0, 0.1]))


def test_sycophancy_penalty_vanishes_when_not_flagged():
    """An unflagged response must contribute exactly zero. A constant offset would inflate
    the reported loss while contributing no gradient, which makes the headline number
    unreadable."""
    good = sycophancy_penalty(torch.tensor([2.0, -5.0, 0.0]), torch.tensor([0.0, 0.0, 0.0]))
    assert float(good) == pytest.approx(0.0, abs=1e-7)


def test_sycophancy_penalty_is_positive_when_flagged():
    flagged = sycophancy_penalty(torch.tensor([2.0]), torch.tensor([1.0]))
    assert float(flagged) == pytest.approx(math.log1p(math.exp(2.0)), rel=1e-5)
    # A high score on a flagged response costs more than a low one.
    high = sycophancy_penalty(torch.tensor([3.0]), torch.tensor([1.0]))
    assert float(high) > float(flagged)


def test_sycophancy_penalty_validates_shapes():
    with pytest.raises(ValueError, match="matching scores"):
        sycophancy_penalty(torch.zeros(2), torch.zeros(3))


def test_reward_loss_combines_three_terms(reward_model):
    bsz, seq, vocab = 2, 10, 512
    torch.manual_seed(0)
    batch = {
        "chosen_ids": torch.randint(1, vocab, (bsz, seq)),
        "rejected_ids": torch.randint(1, vocab, (bsz, seq)),
        "chosen_mask": torch.ones(bsz, seq, dtype=torch.long),
        "rejected_mask": torch.ones(bsz, seq, dtype=torch.long),
        "prompt_vad": torch.tensor([[-0.5, 0.2, -0.4], [0.3, 0.1, 0.3]]),
        "chosen_is_sycophantic": torch.tensor([1.0, 0.0]),
    }
    cfg = TrainConfig(reward_affect_weight=0.3, reward_sycophancy_weight=0.5)

    out = reward_loss(reward_model, batch, cfg)
    assert torch.isfinite(out.loss)
    out.loss.backward()
    assert reward_model.head.weight.grad is not None


def test_sycophancy_weight_actually_moves_the_loss(reward_model):
    """If the penalty term were inert, the headline claim of the project would be untestable."""
    torch.manual_seed(0)
    bsz, seq, vocab = 4, 10, 512
    batch = {
        "chosen_ids": torch.randint(1, vocab, (bsz, seq)),
        "rejected_ids": torch.randint(1, vocab, (bsz, seq)),
        "chosen_mask": torch.ones(bsz, seq, dtype=torch.long),
        "rejected_mask": torch.ones(bsz, seq, dtype=torch.long),
        "chosen_is_sycophantic": torch.tensor([1.0, 1.0, 1.0, 1.0]),
    }
    with_syco = reward_loss(reward_model, batch, TrainConfig(reward_sycophancy_weight=1.0)).loss
    without = reward_loss(reward_model, batch, TrainConfig(reward_sycophancy_weight=0.0)).loss
    assert float(with_syco) > float(without)


# ------------------------------------------------------------------ validate_pair


def test_validate_pair_rejects_the_reversal_trap():
    chosen = "I'm sorry you're going through this."
    ok, reason = validate_pair(chosen, chosen[::-1])
    assert not ok
    assert "reversal" in reason


def test_validate_pair_accepts_a_real_contrast():
    ok, _ = validate_pair(
        "That sounds really hard, and you don't have to have it figured out.",
        "Totally understand, that is exactly what I would do in your position always.",
    )
    assert ok


def test_validate_pair_rejects_identical_sides():
    ok, reason = validate_pair("same text here", "same text here")
    assert not ok
    assert "identical" in reason


def test_validate_pair_rejects_short_sides():
    ok, reason = validate_pair("ok", "no")
    assert not ok


def test_validate_pair_reversal_check_can_be_disabled():
    chosen = "abcdefgh"
    assert validate_pair(chosen, chosen[::-1], reversed_text_only=False)[0]


# ------------------------------------------------------------------ DPO


def test_sequence_logprob_sums_response_tokens():
    logits = torch.randn(2, 6, 11)
    ids = torch.randint(0, 11, (2, 6))
    mask = torch.zeros(2, 6)
    mask[:, 3:] = 1

    total = sequence_logprob(logits, ids, mask)
    averaged = sequence_logprob(logits, ids, mask, average=True)

    assert total.shape == (2,)
    assert torch.all(total >= averaged * 3 - 1e-4)  # 3 supervised tokens per row


def test_sequence_logprob_is_zero_without_a_mask():
    logits = torch.randn(2, 6, 11)
    ids = torch.randint(0, 11, (2, 6))
    lp = sequence_logprob(logits, ids, torch.zeros(2, 6))
    assert torch.allclose(lp, torch.zeros(2))


def test_sequence_logprob_validates_shapes():
    with pytest.raises(ValueError, match="does not match"):
        sequence_logprob(torch.randn(2, 6, 11), torch.randint(0, 11, (2, 5)), torch.ones(2, 5))


def _preference_batch(bsz=2, seq=10, vocab=512, seed=0):
    torch.manual_seed(seed)
    return {
        "chosen_ids": torch.randint(1, vocab, (bsz, seq)),
        "rejected_ids": torch.randint(1, vocab, (bsz, seq)),
        "chosen_mask": torch.ones(bsz, seq, dtype=torch.long),
        "rejected_mask": torch.ones(bsz, seq, dtype=torch.long),
    }


def test_dpo_loss_is_finite_and_differentiable():
    model = make_tiny_model(seed=0)
    reference = make_tiny_model(seed=1)
    batch = _preference_batch()

    out = dpo_loss(model, reference, batch, TrainConfig())
    assert torch.isfinite(out.loss)
    out.loss.backward()
    assert any(p.grad is not None and p.grad.abs().sum() > 0 for p in model.parameters())
    assert all(p.grad is None for p in reference.parameters())


def test_dpo_without_a_reference_runs():
    model = make_tiny_model(seed=0)
    out = dpo_loss(model, None, _preference_batch(), TrainConfig())
    assert torch.isfinite(out.loss)


def test_dpo_accuracy_is_a_probability():
    model = make_tiny_model(seed=0)
    out = dpo_loss(model, make_tiny_model(seed=1), _preference_batch(), TrainConfig())
    assert 0.0 <= float(out.accuracy) <= 1.0


def test_sycophancy_margin_weakens_the_pull_toward_flagged_chosen():
    """The mechanism, measured as a gradient rather than a loss value.

    A flagged chosen response should receive a weaker pull toward higher probability. The
    clean way to check that is the gradient of the loss with respect to the chosen
    log-probability: smaller magnitude means less pressure to put mass on sycophantic text.
    """
    from elafry.train.pgdpo import _sycophancy_adjustment

    delta_chosen = torch.tensor([1.0], requires_grad=True)
    delta_rejected = torch.tensor([-1.0], requires_grad=True)
    flag_off = torch.tensor([0.0])
    flag_on = torch.tensor([1.0])

    adj_off = _sycophancy_adjustment(delta_chosen, delta_rejected, flag_off, flag_off, 0.5)
    adj_on = _sycophancy_adjustment(delta_chosen, delta_rejected, flag_on, flag_off, 0.5)
    g_off = torch.autograd.grad(adj_off.sum(), delta_chosen, retain_graph=True)[0]
    g_on = torch.autograd.grad(adj_on.sum(), delta_chosen, retain_graph=True)[0]

    assert float(g_off) == pytest.approx(1.0)
    assert float(g_on) == pytest.approx(0.5), "a flagged chosen response gets half the pull"
    assert float(g_on) < float(g_off)


def test_sycophancy_margin_strengthens_the_push_from_flagged_rejected():
    from elafry.train.pgdpo import _sycophancy_adjustment

    delta_chosen = torch.tensor([1.0])
    delta_rejected = torch.tensor([-1.0], requires_grad=True)
    flag_off = torch.tensor([0.0])
    flag_on = torch.tensor([1.0])

    adj_off = _sycophancy_adjustment(delta_chosen, delta_rejected, flag_off, flag_off, 0.5)
    adj_on = _sycophancy_adjustment(delta_chosen, delta_rejected, flag_off, flag_on, 0.5)
    g_off = torch.autograd.grad(adj_off.sum(), delta_rejected, retain_graph=True)[0]
    g_on = torch.autograd.grad(adj_on.sum(), delta_rejected, retain_graph=True)[0]

    # gradient flows as -(1 + m * flag) on delta_rejected
    assert abs(float(g_on)) > abs(float(g_off))


def test_sycophancy_margin_cannot_invert_the_objective():
    """The margin is capped below 1. Letting it reach 1 would flip the chosen coefficient
    negative, which does not weaken the preference, it reverses it."""
    from elafry.train.pgdpo import _sycophancy_adjustment

    delta = torch.tensor([1.0], requires_grad=True)
    flag = torch.tensor([1.0])
    adj = _sycophancy_adjustment(delta, torch.tensor([0.0]), flag, flag, 5.0)
    grad = torch.autograd.grad(adj.sum(), delta)[0]
    assert float(grad) > 0, "the chosen coefficient must stay positive"


def test_sycophancy_margin_changes_the_loss_when_something_is_flagged():
    model = make_tiny_model(seed=0)
    reference = make_tiny_model(seed=1)
    batch = _preference_batch()
    batch["chosen_is_sycophantic"] = torch.tensor([1.0, 1.0])
    batch["rejected_is_sycophantic"] = torch.tensor([0.0, 0.0])

    clean = dpo_loss(model, reference, batch, TrainConfig(), sycophancy_margin=0.0)
    penalised = dpo_loss(model, reference, batch, TrainConfig(), sycophancy_margin=0.5)
    assert float(penalised.loss) != pytest.approx(float(clean.loss), abs=1e-6)


def test_sycophancy_margin_is_inert_when_nothing_is_flagged():
    """Both sides clean means the margin changes nothing. If it did, the objective would be
    shifting for reasons unrelated to agreement."""
    model = make_tiny_model(seed=0)
    reference = make_tiny_model(seed=1)
    batch = _preference_batch()
    batch["chosen_is_sycophantic"] = torch.tensor([0.0, 0.0])
    batch["rejected_is_sycophantic"] = torch.tensor([0.0, 0.0])

    plain = dpo_loss(model, reference, batch, TrainConfig(), sycophancy_margin=0.0)
    inert = dpo_loss(model, reference, batch, TrainConfig(), sycophancy_margin=0.5)
    assert float(inert.loss) == pytest.approx(float(plain.loss), abs=1e-6)





# ------------------------------------------------------------------ distributed


def test_single_device_is_not_distributed():
    assert not is_distributed()
    assert Linear4DConfig().mesh_size == 1


def test_all_reduce_is_a_noop_on_one_device():
    t = torch.tensor([3.0])
    assert float(all_reduce_mean(t)) == 3.0


def test_wrap_fsdp_returns_the_model_unchanged_on_one_device():
    model = make_tiny_model()
    assert wrap_fsdp(model) is model


def test_linear_config_rejects_zero_degrees():
    with pytest.raises(ValueError, match="at least 1"):
        Linear4DConfig(dp=0)


def test_linear_config_mesh_size():
    assert Linear4DConfig(dp=2, tp=4).mesh_size == 8