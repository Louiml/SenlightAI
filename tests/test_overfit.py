"""The overfit gate.

This is the acceptance test for a from-scratch transformer. With no corpus, the question is
not "is the model good", it is "is the implementation correct". The standard answer is to
make the model memorise one small batch: if it cannot drive the loss to near zero on a
handful of examples, something in the wiring is wrong, and no amount of data will fix it.

It catches the failure modes that produce a model which runs, trains, reports a falling
loss, and never learns anything:

* a residual connection wired backwards
* an attention mask that leaks the future
* a RoPE convention that scrambles position
* a KV cache that disagrees with prefill
* zero-initialised projections that zero the gradient as well as the output

That last one matters most for Elafry specifically. ``o_proj``, ``mlp.down`` and
``AffectiveBias.state_proj`` all start at zero so the model begins as the identity. That is
only safe because the gradient does not go to zero with them. If it did, the overfit would
stall immediately and the whole architecture would be a very expensive no-op.

Run on CPU in float32. Takes a few seconds.
"""

from __future__ import annotations


import torch

from elafry.config import AffectConfig, PRESETS
from elafry.models.elafry import Elafry


def _batch(bsz: int = 4, seq: int = 16, vocab: int = 512, seed: int = 7):
    torch.manual_seed(seed)
    ids = torch.randint(1, vocab, (bsz, seq))
    labels = ids.clone()
    return ids, labels


def _train(model, ids, labels, steps: int, lr: float = 3e-3, state=None):
    """Plain next-token training. No masking, no scheduler, nothing clever."""
    optimizer = torch.optim.AdamW(model.parameters(), lr=lr, weight_decay=0.0)
    losses = []
    for _ in range(steps):
        out = model(ids, state=state)
        loss = torch.nn.functional.cross_entropy(
            out.logits[:, :-1].reshape(-1, out.logits.shape[-1]).float(),
            labels[:, 1:].reshape(-1),
        )
        optimizer.zero_grad(set_to_none=True)
        loss.backward()
        optimizer.step()
        losses.append(float(loss))
    return losses


def test_tiny_model_overfits_one_batch():
    """The core gate. 300 steps of a 110k-parameter model on 64 tokens has to be enough."""
    preset = PRESETS["elafry-tiny"]
    torch.manual_seed(0)
    model = Elafry(preset["model"], preset["affect"])
    model.train()

    ids, labels = _batch(vocab=preset["model"].vocab_size)

    initial = torch.nn.functional.cross_entropy(
        model(ids).logits[:, :-1].reshape(-1, preset["model"].vocab_size).float(),
        labels[:, 1:].reshape(-1),
    )
    assert float(initial) > 4.0, (
        f"initial loss {float(initial):.2f} is suspiciously low for an untrained model; "
        "the test would not prove anything"
    )

    losses = _train(model, ids, labels, steps=300)

    assert losses[-1] < 0.05, (
        f"loss plateaued at {losses[-1]:.4f} after 300 steps on a single batch; "
        "the model cannot fit 64 tokens, so the implementation is wrong somewhere"
    )
    # Monotone in the tail. A model that is still bouncing around has not settled.
    tail = losses[-20:]
    assert tail[-1] <= min(tail) + 1e-6, f"loss never settled: last 20 = {tail}"


def test_overfit_works_with_the_affective_subsystem_disabled():
    """The control condition has to be trainable too, or it is not a usable baseline."""
    preset = PRESETS["elafry-tiny"]
    torch.manual_seed(0)
    model = Elafry(preset["model"], AffectConfig(enabled=False))
    model.train()

    ids, labels = _batch(vocab=preset["model"].vocab_size)
    losses = _train(model, ids, labels, steps=300)
    assert losses[-1] < 0.05


def test_overfit_works_with_a_pinned_external_state():
    """Injecting a fixed state must not break training. If the bias were somehow applied to
    the loss, a constant state would act as a constant offset and still fit, so this is
    really checking that the state path stays finite and connected."""
    preset = PRESETS["elafry-tiny"]
    torch.manual_seed(0)
    model = Elafry(preset["model"], preset["affect"])
    model.train()

    ids, labels = _batch(vocab=preset["model"].vocab_size)
    pinned = torch.tensor([[-0.6, 0.5, 0.2]])
    losses = _train(model, ids, labels, steps=300, state=pinned)
    assert losses[-1] < 0.05
    assert all(l == l for l in losses), "loss went NaN"


def test_zero_init_still_receives_gradient():
    """The load-bearing property behind every zero-initialised projection in this codebase.

    A zeroed weight contributes nothing to the forward pass. It is tempting to assume the
    gradient is therefore zero too, and to stop training those layers. It is not: the
    upstream activations are non-zero, so dL/dW is non-zero. This test is the reason that
    holds rather than being an assumption.
    """
    preset = PRESETS["elafry-tiny"]
    torch.manual_seed(0)
    model = Elafry(preset["model"], preset["affect"])

    # Confirm the starting point is genuinely zeroed, so this test cannot pass vacuously.
    for layer in model.layers:
        assert torch.count_nonzero(layer.self_attn.o_proj.weight) == 0
        assert torch.count_nonzero(layer.mlp.down.weight) == 0

    ids, _ = _batch(vocab=preset["model"].vocab_size)
    model(ids).logits.float().mean().backward()

    for i, layer in enumerate(model.layers):
        assert layer.self_attn.o_proj.weight.grad is not None
        assert layer.self_attn.o_proj.weight.grad.abs().sum().item() > 0, (
            f"layer {i} o_proj is zero-initialised and its gradient is also zero; "
            "this layer will never train"
        )
        assert layer.mlp.down.weight.grad is not None
        assert layer.mlp.down.weight.grad.abs().sum().item() > 0, f"layer {i} mlp.down is dead"


def test_affective_bias_learns_a_nonzero_projection():
    """The same question for the mechanism the whole project is about. After fitting, the
    affective projection should have moved off zero. If it has not, emotion is decorative."""
    preset = PRESETS["elafry-tiny"]
    torch.manual_seed(0)
    model = Elafry(preset["model"], preset["affect"])
    model.train()

    before = [
        layer.self_attn.affect.state_proj.weight.detach().clone()
        for layer in model.layers
    ]

    ids, labels = _batch(vocab=preset["model"].vocab_size)
    _train(model, ids, labels, steps=300)

    moved = [
        not torch.equal(before[i], layer.self_attn.affect.state_proj.weight.detach())
        for i, layer in enumerate(model.layers)
    ]
    assert any(moved), (
        "the affective bias projection never moved; the state reaches attention but gradient "
        "descent has no reason to use it"
    )


def test_internal_state_predictor_learns():
    """The state should move off the origin during training. A state predictor frozen at zero
    means the model is emotionally inert no matter what it reads."""
    preset = PRESETS["elafry-tiny"]
    torch.manual_seed(0)
    model = Elafry(preset["model"], preset["affect"])
    model.train()

    ids, labels = _batch(vocab=preset["model"].vocab_size)
    _train(model, ids, labels, steps=300)

    out = model(ids)
    assert out.state.abs().max().item() > 1e-5, (
        "the internal state stayed at the origin after training; the state predictor has no "
        "gradient reaching it"
    )
    assert out.state.abs().max().item() <= preset["affect"].clamp + 1e-6


def test_100m_preset_starts_its_loss_near_uniform():
    """A larger model should not have a pathological starting loss. Anything far from
    log(vocab) means the initialisation is broken for that geometry."""
    preset = PRESETS["elafry-100m"]
    torch.manual_seed(0)
    model = Elafry(preset["model"], preset["affect"])
    model.eval()

    ids, labels = _batch(bsz=2, seq=32, vocab=preset["model"].vocab_size)
    with torch.no_grad():
        logits = model(ids).logits[:, :-1]
    loss = torch.nn.functional.cross_entropy(
        logits.reshape(-1, preset["model"].vocab_size).float(), labels[:, 1:].reshape(-1)
    )

    uniform = torch.log(torch.tensor(float(preset["model"].vocab_size)))
    assert abs(float(loss) - float(uniform)) < 1.0, (
        f"initial loss {float(loss):.2f} is far from uniform {float(uniform):.2f}"
    )


def test_generation_is_coherent_after_overfitting():
    """The end-to-end proof. Memorise one sequence, then check greedy decoding reproduces it.

    This is the only generation-quality assertion that makes sense without a corpus. A model
    that can fit a batch but cannot reproduce it autoregressively has a bug in the decode
    path that a loss curve cannot show.
    """
    preset = PRESETS["elafry-tiny"]
    torch.manual_seed(0)
    model = Elafry(preset["model"], preset["affect"])
    model.train()

    ids, labels = _batch(bsz=1, seq=24, vocab=preset["model"].vocab_size)
    _train(model, ids, labels, steps=400)
    model.eval()

    prompt_len = 12
    prompt, expected = ids[:, :prompt_len], ids[:, prompt_len:]

    with torch.no_grad():
        # Greedy: top_k=1 collapses sampling to argmax.
        produced, _ = model.generate(
            prompt, max_new_tokens=12, temperature=1.0, top_k=1, top_p=1.0, eos_id=None
        )

    matches = int((produced == expected).sum().item())
    assert matches >= 10, (
        f"only {matches}/12 tokens reproduced after fitting; the decode path disagrees with "
        "the training path"
    )