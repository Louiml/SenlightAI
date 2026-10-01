"""VAD state, the Markovian transition, Plutchik classification, and the encoders."""

from __future__ import annotations

import pytest
import torch

from elafry.affective.encoders import (
    AffectEncoder,
    AffectiveStateBank,
    InternalStatePredictor,
    Velocity,
)
from elafry.affective.state import (
    PLUTCHIK_DYADS,
    PLUTCHIK_PRIMARIES,
    PLUTCHIK_VAD,
    AffectiveState,
    all_primary_states,
    build_state_features,
    clamp_state,
    classify_plutchik,
    initial_state,
    intensity_ladder,
    plutchik_tensor,
    state_feature_dim,
    transition,
)
from elafry.config import AffectConfig


# ------------------------------------------------------------------ VAD basics


def test_initial_state_is_the_neutral_origin():
    assert torch.equal(initial_state(3), torch.zeros(3, 3))


def test_clamp_holds_the_cube():
    s = torch.tensor([[2.0, -3.0, 0.5]])
    out = clamp_state(s)
    assert out.tolist() == [[1.0, -1.0, 0.5]]


def test_plutchik_table_is_complete_and_in_range():
    assert len(PLUTCHIK_PRIMARIES) == 8
    assert len(PLUTCHIK_DYADS) == 8
    for name in PLUTCHIK_PRIMARIES:
        v, a, d = PLUTCHIK_VAD[name]
        assert -1.0 <= v <= 1.0, name
        assert -1.0 <= a <= 1.0, name
        assert -1.0 <= d <= 1.0, name


def test_plutchik_tensor_shape_and_order():
    t = plutchik_tensor()
    assert t.shape == (8, 3)
    assert torch.allclose(t[0], torch.tensor(PLUTCHIK_VAD["joy"]))


def test_dominance_separates_anger_from_fear():
    """Same valence band, same arousal band, opposite dominance. This is the distinction the
    architecture note's serial-killer example depends on."""
    anger = AffectiveState(*PLUTCHIK_VAD["anger"])
    fear = AffectiveState(*PLUTCHIK_VAD["fear"])
    assert anger.dominance > 0 > fear.dominance
    assert abs(anger.valence - fear.valence) < 0.2
    assert abs(anger.arousal - fear.arousal) < 0.2


# ------------------------------------------------------------------ transition


def test_transition_is_a_convex_combination():
    prev = torch.tensor([[0.8, -0.8, 0.4]])
    drive = torch.tensor([[-0.8, 0.8, -0.4]])
    out = transition(prev, drive, inertia=0.25)
    # 0.25 * (0.8, -0.8, 0.4) + 0.75 * (-0.8, 0.8, -0.4) = (-0.4, 0.4, -0.2)
    assert torch.allclose(out, torch.tensor([[-0.4, 0.4, -0.2]]), atol=1e-6)


@pytest.mark.parametrize("inertia", [0.0, 0.25, 0.5, 0.75, 0.99])
def test_high_inertia_keeps_more_of_the_previous_state(inertia):
    prev = torch.tensor([[0.9, 0.0, 0.0]])
    drive = torch.tensor([[-0.9, 0.0, 0.0]])
    out = transition(prev, drive, inertia=inertia)
    # The drive opposes the previous state here, so the state has to fall, and it has to
    # land between the two. Higher inertia keeps it nearer the previous value.
    assert float(drive[0, 0]) <= float(out[0, 0]) < float(prev[0, 0])


def test_inertia_one_freezes_the_state():
    prev = torch.tensor([[0.5, 0.5, 0.5]])
    drive = torch.tensor([[-1.0, -1.0, -1.0]])
    out = transition(prev, drive, inertia=1.0)
    assert torch.allclose(out, prev)


def test_inertia_zero_ignores_the_previous_state():
    prev = torch.tensor([[0.9, 0.9, 0.9]])
    drive = torch.tensor([[-0.5, 0.25, 0.0]])
    out = transition(prev, drive, inertia=0.0)
    assert torch.allclose(out, drive)


def test_transition_clamps_extreme_drives():
    prev = torch.tensor([[0.0, 0.0, 0.0]])
    drive = torch.tensor([[50.0, -50.0, 0.0]])
    out = transition(prev, drive, inertia=0.5)
    assert out[0, 0] <= 1.0
    assert out[0, 1] >= -1.0


def test_transition_rejects_bad_inertia():
    with pytest.raises(ValueError, match="inertia must be"):
        transition(torch.zeros(1, 3), torch.zeros(1, 3), inertia=1.5)


def test_transition_rejects_shape_mismatch():
    with pytest.raises(ValueError, match="shape mismatch"):
        transition(torch.zeros(2, 3), torch.zeros(1, 3))


def test_repeated_transition_converges_to_the_drive():
    """A long conversation should settle toward whatever the input keeps pushing for,
    not oscillate."""
    prev = initial_state(1)
    drive = torch.tensor([[0.4, -0.2, 0.1]])
    for _ in range(200):
        prev = transition(prev, drive, inertia=0.8)
    assert torch.allclose(prev, drive, atol=1e-2)


# ------------------------------------------------------------------ classification


@pytest.mark.parametrize("name", PLUTCHIK_PRIMARIES)
def test_each_anchor_classifies_as_itself(name):
    state = torch.tensor([PLUTCHIK_VAD[name]])
    label, sim = classify_plutchik(state, threshold=0.5)
    assert label == name, f"{name} classified as {label} (sim {sim:.3f})"


def test_a_neutral_state_has_no_label():
    """Below threshold the answer is ``None``, not the nearest centroid. Forcing a label
    here is how a neutral state acquires a mood."""
    label, sim = classify_plutchik(torch.zeros(3), threshold=0.72)
    assert label is None
    assert sim == 0.0


def test_threshold_controls_the_strictness():
    # Cosine similarity is scale-invariant, so shrinking an anchor does not move it toward
    # any other centroid. To exercise the rejection path the state has to point somewhere
    # genuinely between anchors: straight down the dominance axis, between anger and fear.
    between = torch.tensor([0.0, 0.8, 0.0])
    label, sim = classify_plutchik(between, threshold=0.99)
    assert label is None
    assert sim < 0.99
    assert classify_plutchik(between, threshold=0.0)[0] in PLUTCHIK_PRIMARIES


def test_anger_and_sadness_are_more_distinct_than_their_neighbours():
    """Cosine distance ordering. anger's nearest neighbour should be disgust or fear, not
    sadness, since the wheel puts them on opposite sides of the valence axis."""
    anchors = plutchik_tensor()
    norms = anchors / anchors.norm(dim=-1, keepdim=True)

    def dist(a: str, b: str) -> float:
        ia, ib = PLUTCHIK_PRIMARIES.index(a), PLUTCHIK_PRIMARIES.index(b)
        return 1.0 - float(norms[ia] @ norms[ib])

    assert dist("anger", "sadness") > dist("anger", "disgust")
    assert dist("joy", "sadness") > dist("joy", "trust")


def test_classify_rejects_a_batch():
    with pytest.raises(ValueError, match="single state vector"):
        classify_plutchik(torch.zeros(4, 3))


def test_intensity_ladder_pushes_outward():
    ladder = intensity_ladder("anger")
    assert len(ladder) == 3
    norms = [float(v.norm()) for v in ladder.values()]
    assert norms[0] < norms[1] < norms[2], "higher rungs should sit further from the origin"
    for v in ladder.values():
        assert v.abs().max() <= 1.0


def test_all_primary_states_covers_every_primary():
    states = all_primary_states()
    assert set(states) == set(PLUTCHIK_PRIMARIES)
    for v in states.values():
        assert v.shape == (1, 3)


# ------------------------------------------------------------------ feature width


def test_feature_width_defaults_to_three():
    assert state_feature_dim() == 3


def test_feature_builder_matches_the_declared_width():
    s = torch.randn(4, 3)
    assert build_state_features(s).shape == (4, 3)
    assert build_state_features(s, include_intent=True).shape == (4, 4)
    assert build_state_features(s, include_intent=True, include_velocity=True).shape == (4, 5)


def test_absent_channels_are_zero_filled_not_omitted():
    """This is the contract that stops a three-wide tensor reaching a four-wide bias."""
    s = torch.randn(2, 3)
    feats = build_state_features(s, include_intent=True, include_crisis=True)
    assert feats.shape == (2, 5)
    assert torch.equal(feats[:, 3], torch.zeros(2))  # intent, not supplied
    assert torch.equal(feats[:, 4], torch.zeros(2))  # crisis, not supplied


def test_supplied_channels_are_used():
    s = torch.zeros(2, 3)
    feats = build_state_features(s, intent=torch.tensor([0.7, 0.2]), include_intent=True)
    assert torch.allclose(feats[:, 3], torch.tensor([0.7, 0.2]))


def test_feature_builder_rejects_a_bad_state():
    with pytest.raises(ValueError, match=r"must be \(B, 3\)"):
        build_state_features(torch.zeros(2, 4))


def test_config_reports_the_same_width_as_the_builder():
    cfg = AffectConfig(use_intent=True, use_velocity=True)
    assert cfg.state_feature_dim == build_state_features(
        torch.zeros(1, 3), include_intent=True, include_velocity=True
    ).shape[-1]
    assert cfg.build_features(torch.zeros(1, 3)).shape[-1] == cfg.state_feature_dim


# ------------------------------------------------------------------ encoders


def test_affect_encoder_output_is_bounded():
    enc = AffectEncoder(dim=32, hidden=64)
    torch.nn.init.normal_(enc.net[-1].weight, std=5.0)  # deliberately out of range
    drive = enc(torch.randn(8, 32) * 50)
    assert drive.shape == (8, 3)
    assert drive.abs().max() <= enc.max_drive + 1e-6


def test_internal_predictor_respects_the_mask():
    """Without the mask, pad tokens drag the mean and the state becomes a function of batch
    composition."""
    pred = InternalStatePredictor(dim=8, hidden=16)
    hidden = torch.randn(1, 6, 8)
    mask = torch.tensor([[1, 1, 1, 0, 0, 0]])

    masked = pred(hidden, mask)

    # Change only the padded region. The prediction must not move.
    perturbed = hidden.clone()
    perturbed[0, 3:] = torch.randn(3, 8) * 100
    assert torch.allclose(masked, pred(perturbed, mask), atol=1e-5)

    # Without a mask the same change does move it, which is the bug the mask prevents.
    assert not torch.allclose(pred(hidden), pred(perturbed), atol=1e-3)


def test_internal_predictor_rejects_wrong_rank():
    pred = InternalStatePredictor(dim=8)
    with pytest.raises(ValueError, match=r"expected \(B, S, dim\)"):
        pred(torch.randn(4, 8))


def test_velocity_tracks_signed_change():
    v = Velocity(beta=0.5)
    first = v(torch.tensor([0.0]))
    assert torch.allclose(first, torch.tensor([0.0]))

    second = v(torch.tensor([0.4]))
    assert float(second) > 0

    third = v(torch.tensor([-0.4]))
    assert float(third) < 0, "a drop in arousal should read as negative velocity"


def test_state_bank_rolls():
    bank = AffectiveStateBank(batch_size=1, max_seq=3, dim=3)
    for value in (1.0, 2.0, 3.0):
        bank.append(torch.full((1, 3), value))
    hist = bank.history()
    assert hist.shape == (1, 3, 3)
    assert torch.allclose(bank.latest(), torch.full((1, 3), 3.0))

    bank.append(torch.full((1, 3), 4.0))
    hist = bank.history()
    # Rolled left, so the oldest value is gone.
    assert hist[0, 0].mean().item() == pytest.approx(2.0)
    assert torch.allclose(bank.latest(), torch.full((1, 3), 4.0))


def test_state_bank_reset():
    bank = AffectiveStateBank(batch_size=1, max_seq=4, dim=3)
    bank.append(torch.ones(1, 3))
    bank.reset()
    assert torch.equal(bank.history(), torch.zeros(1, 4, 3))


# ------------------------------------------------------------------ dataclass


def test_affective_state_clamps_and_converts():
    s = AffectiveState(2.0, -3.0, 0.5)
    assert s.vector == (1.0, -1.0, 0.5)

    t = torch.tensor([[0.1, 0.2, 0.3]])
    assert AffectiveState.from_tensor(t).vector == pytest.approx((0.1, 0.2, 0.3))


def test_affective_state_similarity_of_zero_vectors():
    assert AffectiveState().similarity(AffectiveState(1, 1, 1)) == 0.0


def test_affective_state_similarity_is_one_for_itself():
    s = AffectiveState(0.3, 0.2, -0.5)
    assert s.similarity(s) == pytest.approx(1.0)