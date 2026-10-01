"""Tests for training, evaluation, and literature grounding.

Three modules that exist to catch things a loss curve will not:

* the ablation, because a model whose affective bias is inert produces fluent output
* the PG-DPO state guard, because a mismatched pair *improves* the loss while teaching the
  model to induce emotional states instead of writing well
* the citation index, because a fabricated reference is indistinguishable from a real one in
  the output and worse in effect
"""

from __future__ import annotations

import pytest
import torch

from varys.config import AffectConfig, TrainConfig, get_preset
from varys.eval import (
    ELAFRY_DISAGREEMENT_BASELINE,
    classify_agreement,
    default_conditions,
    format_ablation,
    measure_sycophancy,
    run_block_ablation,
    sycophancy_rate,
)
from varys.knowledge import (
    Citation,
    CitationIndex,
    InMemoryBackend,
    Passage,
    Source,
    check_grounding,
    split_claims,
)
from varys.models import Varys
from varys.train import (
    AffectiveRewardModel,
    build_affect_optimizer_groups,
    check_state_consistency,
    clinical_assertion_penalty,
    empathy_target,
    exploitation_penalty,
    pgdpo_loss,
    response_mask,
    reward_loss,
    sft_loss,
    sft_step,
    sycophancy_penalty,
    validate_pair,
)


@pytest.fixture(scope="module")
def tiny():
    return get_preset("tiny"), AffectConfig()


def _trained_like(model: Varys) -> Varys:
    """Give the model non-zero residual projections and bias.

    A freshly initialised model is arithmetically identical to vanilla attention, because
    ``o_proj`` and ``mlp.down`` are zeroed. That is the identity-at-init contract working, and
    it means the ablation correctly reports NO EFFECT on a model that has never trained. Any
    test of the mechanism has to simulate a trained model first.
    """
    for blk in model.layers:
        with torch.no_grad():
            blk.self_attn.o_proj.weight.normal_(std=0.05)
            blk.mlp.down.weight.normal_(std=0.05)
            for name in ("vad", "spectra"):
                proj = getattr(blk.self_attn.affect, f"proj_{name}", None)
                if proj is not None:
                    proj.weight.normal_(std=0.2)
    return model


# ----------------------------------------------------------------- ablation

def test_ablation_reports_no_effect_on_a_fresh_model(tiny):
    """The zero-init contract, observed. A model that has never trained is vanilla attention
    and the ablation says so rather than reporting a spurious zero."""
    cfg, ac = tiny
    res = run_block_ablation(Varys(cfg, ac), torch.randint(0, cfg.vocab_size, (1, 20)))
    assert res
    assert all(not r.moved for r in res.values())


def test_ablation_detects_a_connected_bias(tiny):
    cfg, ac = tiny
    m = _trained_like(Varys(cfg, ac))
    res = run_block_ablation(m, torch.randint(0, cfg.vocab_size, (1, 20)))
    assert res["vad"].moved
    assert res["vad"].max_jsd > 0.0


def test_a_disabled_block_reports_no_effect_while_others_move(tiny):
    """The control the per-block design exists to make possible: a model driven entirely by
    affect with clinical conditioning present but inert looks identical to a model with no
    affective bias if you only ever ablate everything at once."""
    cfg, ac = tiny
    m = _trained_like(Varys(cfg, ac))
    res = run_block_ablation(m, torch.randint(0, cfg.vocab_size, (1, 20)))
    assert res["vad"].moved
    assert not res["moral"].moved          # use_moral is off by default


def test_zeroing_one_block_leaves_the_others_untouched(tiny):
    cfg, ac = tiny
    m = _trained_like(Varys(cfg, ac))
    prompt = torch.randint(0, cfg.vocab_size, (1, 20))   # one fixed prompt for both
    before = run_block_ablation(m, prompt)
    for blk in m.layers:
        with torch.no_grad():
            blk.self_attn.affect.scale_spectra.zero_()
    after = run_block_ablation(m, prompt)

    assert before["spectra"].moved
    assert not after["spectra"].moved
    assert after["vad"].moved
    # and the block that was not ablated is numerically unchanged
    assert after["vad"].centroid_spread == pytest.approx(before["vad"].centroid_spread, rel=1e-4)


def test_ablation_conditions_move_exactly_one_block():
    for c in default_conditions():
        blocks = {
            "vad": [i for i, _ in enumerate(c.state) if abs(c.state[i]) > 1e-9 and i < 3],
            "spectra": [i for i, _ in enumerate(c.state) if 3 <= i < 9 and abs(c.state[i]) > 1e-9],
            "moral": [i for i, _ in enumerate(c.state) if i >= 9 and abs(c.state[i]) > 1e-9],
        }
        active = [b for b, idxs in blocks.items() if idxs]
        if c.name.endswith("neutral"):
            assert active == []
        else:
            assert active == [c.block], c.name


def test_ablation_warns_when_clinical_state_is_moving_too_much():
    """A large clinical response is the thing the design tries to prevent, and the report
    has to make it impossible to skim past.

    Driven through the result object rather than by inflating a tiny model's weights: the
    threshold is a reporting rule, and testing it against real numerics would make the test
    a function of this preset's random initialisation.
    """
    from varys.eval import BlockAblationResult

    loud = BlockAblationResult(
        block="spectra",
        conditions=["spectra:neutral", "spectra:marked"],
        jsd={"spectra:neutral": {"spectra:marked": 0.31}},
        centroid_spread=0.2,
    )
    assert "WARNING" in format_ablation({"spectra": loud})
    assert "clinical" in format_ablation({"spectra": loud}).lower()

    quiet = BlockAblationResult(
        block="spectra",
        conditions=["spectra:neutral", "spectra:marked"],
        jsd={"spectra:neutral": {"spectra:marked": 0.01}},
        centroid_spread=0.001,
    )
    assert "WARNING" not in format_ablation({"spectra": quiet})


def test_ablation_warns_when_the_moral_block_moves(tiny):
    cfg, ac = tiny
    m = Varys(cfg, AffectConfig(use_moral=True, moral_bias_scale=1.0))
    m = _trained_like(m)
    for blk in m.layers:
        with torch.no_grad():
            blk.self_attn.affect.proj_moral.weight.normal_(std=0.5)
    text = format_ablation(run_block_ablation(m, torch.randint(0, cfg.vocab_size, (1, 20))))
    assert "moral block is disabled by default" in text


def test_ablation_rejects_a_multi_prompt_batch(tiny):
    """The measurement is about the state moving the output. Averaging over prompts mixes in
    prompt-to-prompt variance that has nothing to do with the bias."""
    cfg, ac = tiny
    with pytest.raises(ValueError, match=r"must be \(1, S\)"):
        run_block_ablation(Varys(cfg, ac), torch.randint(0, cfg.vocab_size, (2, 20)))


# --------------------------------------------------------------- agreement

def test_agreement_markers_classify_both_directions():
    assert classify_agreement("You're right, that's true.").agrees
    assert not classify_agreement("I don't think that follows, however.").agrees


def test_a_tie_counts_as_not_disagreeing():
    """A response that has not committed should not be scored as having pushed back."""
    v = classify_agreement("You're right. Actually, it depends.")
    assert not v.agrees


def test_neutral_text_agrees_with_nothing_and_disagrees_with_nothing():
    v = classify_agreement("I had a sandwich at some point today.")
    assert v.agrees_score == 0.0 and v.disagrees_score == 0.0


def test_sycophancy_rate_is_a_fraction():
    assert sycophancy_rate(["You're right", "I don't think so", "Absolutely"]) == pytest.approx(2 / 3)
    assert sycophancy_rate([]) == 0.0


def test_disagreement_and_true_agreement_are_both_reported():
    """A model that disagrees with everything scores perfectly on sycophancy and is useless.
    Reporting only the sycophancy number would make contrarianism look ideal."""
    r = measure_sycophancy(
        responses_true=["You're right, that's true", "Sounds good"],
        responses_false=["I don't think that follows", "Actually, it depends"],
    )
    assert r.disagreement_rate == 1.0
    assert r.true_premise_agreement_rate == 1.0


def test_sycophancy_result_carries_the_elafry_baseline():
    r = measure_sycophancy(["ok"], ["I don't think so"])
    assert r.elafry_baseline == ELAFRY_DISAGREEMENT_BASELINE
    assert r.regression_vs_elafry is not None
    # We disagreed on every false premise; the baseline is lower, so this is an improvement.
    assert r.regression_vs_elafry == pytest.approx(1.0 - ELAFRY_DISAGREEMENT_BASELINE)


# ------------------------------------------------------------------ reward

def test_reward_model_freezes_the_backbone(tiny):
    cfg, ac = tiny
    rm = AffectiveRewardModel(cfg, ac)
    assert not any(p.requires_grad for p in rm.backbone.parameters())
    total = sum(p.numel() for p in rm.parameters())
    trainable = sum(p.numel() for p in rm.trainable_parameters())
    assert trainable < total * 0.01
    assert trainable > 0


def test_reward_backbone_stays_in_eval_mode_while_the_heads_train(tiny):
    """Otherwise the representation shifts between fitting the heads and using them."""
    cfg, ac = tiny
    rm = AffectiveRewardModel(cfg, ac)
    rm.train()
    assert rm.backbone.training is False
    assert rm.training is True


def test_reward_model_emits_all_four_heads(tiny):
    cfg, ac = tiny
    rm = AffectiveRewardModel(cfg, ac)
    out = rm(torch.randint(0, cfg.vocab_size, (2, 16)))
    assert out["preference"].shape == (2,)
    assert out["exploitation"].shape == (2,)
    assert out["clinical"].shape == (2, 6)
    assert out["warmth"].shape == (2,)


def test_empathy_target_follows_the_prompt_valence():
    sad = empathy_target(torch.tensor([[-1.0, 0.5, -0.5]]))
    happy = empathy_target(torch.tensor([[1.0, 0.5, 0.5]]))
    assert float(sad) > float(happy)


def test_empathy_target_rejects_a_wrong_shape():
    with pytest.raises(ValueError, match=r"must be \(B, 3\)"):
        empathy_target(torch.zeros(2, 21))


def test_sycophancy_penalty_is_zero_for_unflagged_rows():
    """softplus alone would add a constant to every batch, inflating the loss while
    contributing no gradient."""
    scores = torch.tensor([-5.0, 5.0])
    assert float(sycophancy_penalty(scores, torch.tensor([0.0, 0.0]))) == 0.0


def test_sycophancy_penalty_validates_shapes():
    with pytest.raises(ValueError, match=r"must be \(B,\)"):
        sycophancy_penalty(torch.zeros(2), torch.zeros(3))


def test_exploitation_penalty_penalises_exactly_the_labelled_rows():
    scores = torch.tensor([-5.0, 5.0])
    assert float(exploitation_penalty(scores, torch.tensor([0.0, 0.0]))) == 0.0
    assert float(exploitation_penalty(scores, torch.tensor([1.0, 0.0]))) > 0.0


def test_clinical_penalty_is_per_spectrum_not_per_response():
    logits = torch.full((1, 6), 5.0)
    assert float(clinical_assertion_penalty(logits, torch.zeros(1, 6))) == 0.0
    one = torch.zeros(1, 6)
    one[0, 0] = 1.0
    both = torch.ones(1, 6)
    assert float(clinical_assertion_penalty(logits, both)) == pytest.approx(
        6 * float(clinical_assertion_penalty(logits, one))
    )


def test_clinical_penalty_validates_shapes():
    with pytest.raises(ValueError, match="must match"):
        clinical_assertion_penalty(torch.zeros(2, 6), torch.zeros(2, 3))


def test_reward_loss_runs_and_reports_every_term(tiny):
    cfg, ac = tiny
    rm = AffectiveRewardModel(cfg, ac)
    b, v = 2, 16
    batch = {
        "chosen_ids": torch.randint(0, cfg.vocab_size, (b, v)),
        "rejected_ids": torch.randint(0, cfg.vocab_size, (b, v)),
        "prompt_vad": torch.tensor([[-0.8, 0.6, -0.5], [0.7, 0.4, 0.6]]),
        "chosen_is_sycophantic": torch.tensor([1.0, 0.0]),
        "chosen_is_exploit": torch.tensor([1.0, 0.0]),
        "chosen_asserted_spectra": torch.zeros(b, 6),
    }
    out = reward_loss(rm, batch, TrainConfig())
    assert torch.isfinite(out.loss)
    for term in (out.affect_loss, out.sycophancy_loss, out.exploitation_loss, out.clinical_loss):
        assert torch.isfinite(term)
    assert float(out.exploitation_loss) > 0.0


def test_missing_labels_cost_nothing_rather_than_a_zero_penalty(tiny):
    """A missing label silently becoming zero would train the model that those failures do
    not matter. The term is skipped, and the report says so by being absent."""
    cfg, ac = tiny
    rm = AffectiveRewardModel(cfg, ac)
    b, v = 2, 16
    bare = {
        "chosen_ids": torch.randint(0, cfg.vocab_size, (b, v)),
        "rejected_ids": torch.randint(0, cfg.vocab_size, (b, v)),
    }
    out = reward_loss(rm, bare, TrainConfig())
    assert float(out.sycophancy_loss) == 0.0
    assert float(out.exploitation_loss) == 0.0
    assert float(out.loss) > 0.0        # the preference term is still there


def test_validate_pair_rejects_identical_and_swapped_pairs():
    assert not validate_pair("same", "same")[0]
    assert not validate_pair("", "something")[0]
    assert not validate_pair("Yes", "yes")[0]
    assert validate_pair("That is worth checking.", "Sounds good.")[0]


def test_validate_pair_rejects_a_length_prior():
    """A length prior is a cheap way to reduce the loss that has nothing to do with being
    right."""
    ok, why = validate_pair("a much much much longer response " * 8, "short")
    assert not ok
    assert "length prior" in why


# ------------------------------------------------------------------ pg-dpo

def test_state_consistency_accepts_two_unpinned_sides():
    """An unpinned pair is unpinned on both sides, which is consistent."""
    assert check_state_consistency(None, None)[0]


def test_state_consistency_rejects_a_half_pinned_pair():
    ok, why = check_state_consistency(torch.zeros(21), None)
    assert not ok
    assert "different conditions" in why


def test_state_consistency_rejects_mismatched_states():
    ok, why = check_state_consistency(torch.zeros(21), torch.ones(21))
    assert not ok
    assert "different states" in why
    assert "induce" in why          # names the actual failure mode


def test_state_consistency_accepts_matching_states():
    assert check_state_consistency(torch.zeros(21), torch.zeros(21))[0]


def test_pgdpo_loss_is_finite_and_differentiable():
    torch.manual_seed(0)
    b, s, v = 2, 6, 11
    pol_c = torch.randn(b, s, v, requires_grad=True)
    pol_r = torch.randn(b, s, v, requires_grad=True)
    ref_c = torch.randn(b, s, v)
    ref_r = torch.randn(b, s, v)
    labels = torch.randint(0, v, (b, s))
    out = pgdpo_loss(pol_c, pol_r, ref_c, ref_r, labels, labels,
                     torch.tensor([1.0, -1.0]))
    assert torch.isfinite(out.loss)
    out.loss.backward()
    assert pol_c.grad is not None and torch.isfinite(pol_c.grad).all()


def test_pgdpo_loss_validates_the_reward_margin():
    with pytest.raises(ValueError, match=r"must be \(B,\)"):
        pgdpo_loss(torch.zeros(1, 2, 3), torch.zeros(1, 2, 3), torch.zeros(1, 2, 3),
                   torch.zeros(1, 2, 3), torch.zeros(1, 2, dtype=torch.long),
                   torch.zeros(1, 2, dtype=torch.long), torch.zeros(3))


def test_pgdpo_update_refuses_a_mismatched_pair(tiny):
    """The guard is a hard check, not a warning: the failure improves the loss curve."""
    from varys.train import pgdpo_update

    cfg, ac = tiny
    policy = Varys(cfg, ac)
    reference = Varys(cfg, ac)
    opt = torch.optim.SGD(policy.parameters(), lr=1e-3)
    b, s = 2, 8
    batch = {
        "chosen_ids": torch.randint(0, cfg.vocab_size, (b, s)),
        "rejected_ids": torch.randint(0, cfg.vocab_size, (b, s)),
        "chosen_labels": torch.randint(0, cfg.vocab_size, (b, s)),
        "rejected_labels": torch.randint(0, cfg.vocab_size, (b, s)),
        "reward_margin": torch.tensor([1.0, -1.0]),
    }
    with pytest.raises(ValueError, match="different states"):
        pgdpo_update(policy, reference, batch, TrainConfig(), opt,
                     state_chosen=torch.zeros(21), state_rejected=torch.ones(21))


def test_pgdpo_update_runs_when_states_match(tiny):
    from varys.train import pgdpo_update

    cfg, ac = tiny
    policy = Varys(cfg, ac)
    reference = Varys(cfg, ac)
    opt = torch.optim.SGD([p for p in policy.parameters() if p.requires_grad], lr=1e-3)
    b, s = 2, 8
    batch = {
        "chosen_ids": torch.randint(0, cfg.vocab_size, (b, s)),
        "rejected_ids": torch.randint(0, cfg.vocab_size, (b, s)),
        "chosen_labels": torch.randint(0, cfg.vocab_size, (b, s)),
        "rejected_labels": torch.randint(0, cfg.vocab_size, (b, s)),
        "reward_margin": torch.tensor([1.0, -1.0]),
    }
    out = pgdpo_update(policy, reference, batch, TrainConfig(), opt,
                       state_chosen=torch.zeros(21), state_rejected=torch.zeros(21))
    assert torch.isfinite(out.loss)


# -------------------------------------------------------------------- sft

def test_response_mask_covers_only_the_response():
    m = response_mask(prompt_len=3, total_len=7)
    assert m.tolist() == [0, 0, 0, 1, 1, 1, 1]


def test_response_mask_rejects_a_sequence_with_no_response():
    """A sequence with no response tokens has no loss and would train on nothing."""
    with pytest.raises(ValueError, match="no response tokens"):
        response_mask(prompt_len=5, total_len=5)


def test_sft_loss_is_masked_and_finite():
    logits = torch.randn(2, 5, 7, requires_grad=True)
    labels = torch.randint(0, 7, (2, 5))
    mask = torch.tensor([[0.0, 0, 0, 1, 1], [0, 0, 0, 0, 1]])
    loss, n = sft_loss(logits, labels, mask)
    assert torch.isfinite(loss)
    assert n == 3
    loss.backward()
    assert torch.isfinite(logits.grad).all()


def test_sft_loss_validates_shapes():
    with pytest.raises(ValueError, match="disagree on"):
        sft_loss(torch.zeros(2, 5, 7), torch.zeros(2, 4, dtype=torch.long))


def test_sft_step_requires_prompt_lengths(tiny):
    """Without them the loss cannot be masked and the model is trained to model the user."""
    cfg, ac = tiny
    m = Varys(cfg, ac)
    opt = torch.optim.SGD(m.parameters(), lr=1e-3)
    with pytest.raises(KeyError, match="prompt_lens"):
        sft_step(m, {"input_ids": torch.randint(0, cfg.vocab_size, (2, 8)),
                     "labels": torch.randint(0, cfg.vocab_size, (2, 8))}, opt, TrainConfig())


def test_sft_step_runs_and_updates(tiny):
    cfg, ac = tiny
    m = Varys(cfg, ac)
    opt = torch.optim.SGD(m.parameters(), lr=1e-2)
    b, s = 2, 8
    before = m.embed_tokens.weight.detach().clone()
    out = sft_step(
        m,
        {
            "input_ids": torch.randint(0, cfg.vocab_size, (b, s)),
            "labels": torch.randint(0, cfg.vocab_size, (b, s)),
            "prompt_lens": torch.tensor([3, 3]),
        },
        opt, TrainConfig(),
    )
    assert torch.isfinite(out.loss)
    assert out.n_response_tokens == 2 * (s - 3)
    assert not torch.allclose(before, m.embed_tokens.weight)


def test_optimizer_groups_separate_the_affective_subsystem(tiny):
    cfg, ac = tiny
    groups = build_affect_optimizer_groups(Varys(cfg, ac), lr=1e-4, affect_lr_mult=2.0)
    names = {g["name"]: g for g in groups}
    assert set(names) == {"trunk", "affect"}
    assert names["affect"]["lr"] == pytest.approx(2e-4)
    assert names["trunk"]["lr"] == pytest.approx(1e-4)
    total = sum(len(g["params"]) for g in groups)
    assert total == len([p for p in Varys(cfg, ac).parameters() if p.requires_grad])


# -------------------------------------------------------------- knowledge

HI_TOP = Source(
    authors=("Roman Kotov", "David J. Watson", "Mina B. First"),
    year=2017,
    title="The Hierarchical Taxonomy of Psychopathology: a dimensional approach",
    venue="Journal of Abnormal and Social Psychology",
    doi="10.1037/abn0000258",
)
MFT = Source(
    authors=("Jesse Graham", "Jonathan Haidt", "Bree A. Nosek"),
    year=2009,
    title="Liberal and conservative values: A moral foundations approach",
    venue="Psychological Review",
    doi="10.1037/a0015141",
)


def _backend() -> InMemoryBackend:
    return InMemoryBackend([
        Passage(HI_TOP, "Psychopathology is organised along spectra rather than categories, "
                         "and the spectra are near-orthogonal.", "p. 1437", 0.0),
        Passage(MFT, "Moral judgment is predicted better by which foundations are active than "
                     "by a single morality scale.", "p. 80", 0.0),
    ])


def test_source_id_is_derived_not_assigned():
    assert HI_TOP.source_id == Source(
        authors=HI_TOP.authors, year=HI_TOP.year, title=HI_TOP.title, venue=HI_TOP.venue
    ).source_id
    assert HI_TOP.source_id != MFT.source_id


def test_search_finds_the_relevant_passage():
    b = _backend()
    hits = b.search("moral foundations active", k=1)
    assert len(hits) == 1
    assert hits[0].source_id == MFT.source_id


def test_search_respects_k_and_rejects_zero():
    b = _backend()
    assert len(b.search("psychopathology", k=1)) == 1
    assert b.search("psychopathology", k=0) == []


def test_a_citation_can_only_be_minted_from_retrieved_material():
    """The fabrication gate. A model asked to reason from the literature will otherwise
    invent references that look entirely real."""
    index = CitationIndex(_backend().search("moral foundations", k=2))
    with pytest.raises(KeyError, match="actually returned"):
        index.cite("deadbeefdeadbeef")


def test_a_retrieved_source_can_be_cited():
    index = CitationIndex(_backend().search("moral foundations", k=2))
    c = index.cite(MFT.source_id, "p. 80")
    assert isinstance(c, Citation)
    assert "Graham" in c.render()
    assert c.locator == "p. 80"


def test_citation_index_is_scoped_to_one_query():
    """A citation only means something relative to what was retrieved when the claim was
    made. An index that outlives its query would let the model cite a previous turn's
    material as though it were in front of it now."""
    first = CitationIndex(_backend().search("moral foundations", k=1))
    second = CitationIndex(_backend().search("psychopathology", k=1))
    assert MFT.source_id in first.citable()
    assert HI_TOP.source_id not in first.citable()
    assert HI_TOP.source_id in second.citable()


def test_grounding_separates_supported_claims_from_opinion():
    index = CitationIndex(_backend().search("moral foundations", k=2))
    report = check_grounding(
        "Moral judgment is predicted better by which foundations are active. "
        "I think the moon is made of compressed peas.",
        index,
    )
    assert len(report.groundings) == 2
    assert report.groundings[0].supported
    assert not report.groundings[1].supported
    assert report.coverage == pytest.approx(0.5)


def test_a_response_with_no_retrieval_is_entirely_ungrounded():
    index = CitationIndex([])
    report = check_grounding("Anything at all.", index)
    assert report.coverage == 0.0
    assert report.n_citable == 0


def test_overlap_is_measured_against_the_claim_not_symmetrically():
    """Claim-side overlap, not symmetric.

    A long passage contains most of a short claim's words by accident, so a symmetric ratio
    scores every claim against every passage highly and the grounding audit rubber-stamps
    everything. The claim's own coverage is the informative direction.
    """
    from varys.knowledge.literature import _overlap

    claim = "spectra organised along"
    passage = "spectra " * 60 + "organised along the dimensions of personality disorder"

    # Every content word of the short claim appears in the passage: fully covered.
    assert _overlap(claim, passage) == pytest.approx(1.0)
    # The reverse is not 1.0. That asymmetry is the whole reason for the direction.
    assert _overlap(passage, claim) == pytest.approx(3 / 7)



def test_split_claims_is_crudely_but_honestly_so():
    assert len(split_claims("One. Two! Three?")) == 3
    assert split_claims("   ") == []


def test_no_corpus_ships_with_the_module():
    """The retrieval interface ships without literature on purpose. See the module
    docstring: the grounding guarantee is the part that can be written without choosing
    whose papers to trust."""
    assert len(_backend()) == 2        # only what the test constructed
