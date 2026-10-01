"""Tests for the Varys affective state.

Every test pins a property that was decided in the source comments, not just a number that
happened to come out. The calibration tests in particular exist because the first
implementation of the wheel classifier and the evidence gate were both wrong in ways that
only showed up under adversarial inputs, and those inputs are what is worth keeping.
"""

from __future__ import annotations

import pytest
import torch

from varys.affective import (
    DYADS,
    MORAL_DIMS,
    MORAL_SLICE,
    PRIMARIES,
    SPECTRA,
    SPECTRA_BASELINE,
    SPECTRA_DIMS,
    SPECTRA_SLICE,
    STATE_DIMS,
    TERTIARIES,
    VAD,
    VAD_DECAY,
    VAD_SLICE,
    WHEEL,
    AffectEncoder,
    EvidenceGate,
    InternalStatePredictor,
    MORAL_DECAY,
    MultiAxisVelocity,
    MoralVector,
    SPECTRA_DECAY,
    SpectrumVector,
    VarysState,
    VarysStateBank,
    build_state_features,
    classify_affect,
    cooccurring,
    derive_anchor,
    feature_names,
    gated_transition,
    is_manipulation_risk,
    neutral_state,
    neutral_vad,
    screening_flags,
    state_feature_dim,
    transition,
    vad_similarity,
)


# ----------------------------------------------------------------- layout

def test_state_is_21_dims_in_three_blocks():
    assert len(STATE_DIMS) == 21
    assert (VAD_SLICE.start, VAD_SLICE.stop) == (0, 3)
    assert (SPECTRA_SLICE.start, SPECTRA_SLICE.stop) == (3, 9)
    assert (MORAL_SLICE.start, MORAL_SLICE.stop) == (9, 21)


def test_moral_block_is_twelve_not_six():
    """One signed scalar per foundation cannot express a strong position on both poles,
    which is the moral-licensing configuration the block exists to detect."""
    assert len(MORAL_DIMS) == 12
    assert len(SPECTRA_DIMS) == 6


def test_feature_width_and_names_always_agree():
    for moral in (False, True):
        for vel in (False, True):
            for intent in (False, True):
                for peak in (False, True):
                    n = state_feature_dim(moral, vel, intent, peak)
                    assert len(feature_names(moral, vel, intent, peak)) == n


def test_moral_block_excluded_from_default_features():
    """The default is a safety decision, not a capacity one. Conditioning attention on
    someone's moral foundations is a persuasion surface."""
    assert state_feature_dim() == 9
    assert "care_care" not in feature_names()
    assert state_feature_dim(include_moral=True) == 21


# -------------------------------------------------------------------- VAD

def test_neutral_state_is_all_zero_and_self_identical():
    s = neutral_state()
    assert s.is_neutral()
    assert s.distance(s) == pytest.approx(0.0, abs=1e-9)
    assert s.similarity(s) == pytest.approx(0.0, abs=1e-9)


def test_vad_similarity_of_zero_vector_is_zero_not_nan():
    """A neutral state is common and must not poison an average."""
    assert vad_similarity(neutral_vad(), neutral_vad()) == 0.0
    assert vad_similarity((1.0, 0.0, 0.0), neutral_vad()) == 0.0


def test_vad_clamps_to_cube():
    assert VAD(5.0, -5.0, 0.0).vector == (1.0, -1.0, 0.0)


def test_state_vector_roundtrip_is_lossless():
    s = VarysState(
        vad=VAD(0.3, -0.2, 0.7),
        spectra=SpectrumVector([0.1, 0.2, 0.3, 0.4, 0.5, 0.6]),
        moral=MoralVector.from_dict({"care_care": 0.8, "care_harm": 0.2}),
    )
    assert VarysState.from_vector(s.to_vector()).to_vector() == pytest.approx(s.to_vector())


def test_state_rejects_wrong_width():
    with pytest.raises(ValueError, match="needs 21"):
        VarysState.from_vector([0.0] * 20)


# --------------------------------------------------------------- Plutchik

def test_wheel_is_thirty_two_nodes():
    assert len(PRIMARIES) == 8
    assert len(DYADS) == 8
    assert len(TERTIARIES) == 16
    assert len(WHEEL) == 32


def test_tertiary_count_is_twice_the_dyad_count():
    """Each dyad blends with its own two primaries."""
    assert len(TERTIARIES) == 2 * len(DYADS)


def test_every_wheel_node_derives_its_anchor_from_its_parents():
    """The three levels must not be able to drift apart."""
    for name in WHEEL:
        if WHEEL[name].level > 0:
            assert WHEEL[name].vad.vector == pytest.approx(derive_anchor(name).vector)


def test_dyad_anchor_is_the_midpoint_of_its_primaries():
    joy = derive_anchor("joy").vector
    trust = derive_anchor("trust").vector
    assert derive_anchor("serenity").vector == pytest.approx(
        tuple((a + b) / 2 for a, b in zip(joy, trust))
    )


def test_classification_prefers_a_primary_over_its_own_tertiary():
    """A tertiary is a midpoint of its parents, so it always matches well enough to shadow
    the primary it came from. Without the tie tolerance, a state sitting exactly on joy comes
    back as serenity_joy."""
    r = classify_affect(derive_anchor("joy").vector)
    assert r.label == "joy"
    assert r.level == "primary"


def test_classification_resolves_a_genuine_blend_to_a_dyad():
    """The counterweight to the test above: tightening the tolerance until nothing ever
    shadows a primary also stops genuine blends from resolving."""
    assert classify_affect(derive_anchor("serenity").vector).label == "serenity"
    assert classify_affect(derive_anchor("loathing").vector).label == "loathing"


def test_classification_returns_none_for_neutral():
    r = classify_affect(neutral_vad())
    assert r.label is None
    assert r.level == "none"


def test_deepest_zero_reduces_to_the_eight_primary_classifier():
    for name in PRIMARIES:
        r = classify_affect(derive_anchor(name).vector, deepest=0)
        assert r.level == "primary"
        assert r.label == name


# --------------------------------------------------------------- clinical

def test_spectra_are_unsigned_and_clamped():
    assert SpectrumVector([-1.0, 0.5, 2.0, 0.0, 0.0, 0.0]).vector == (0.0, 0.5, 1.0, 0.0, 0.0, 0.0)


def test_spectrum_rejects_wrong_width():
    with pytest.raises(ValueError, match="expected 6"):
        SpectrumVector([0.1, 0.2])


def test_all_zero_spectra_report_nothing_marked():
    """Absent is not the same as marked-at-zero."""
    assert SpectrumVector().marked == {}
    assert SpectrumVector().peak == (None, 0.0)


def test_every_spectrum_has_facets():
    for name, spec in SPECTRA.items():
        assert spec.n_facets > 0, name


def test_cooccurrence_finds_a_known_pair():
    """Internalizing and detachment is the most common pairing in the taxonomy."""
    assert ("detachment", "internalizing") in cooccurring({"internalizing": 0.7, "detachment": 0.6})


def test_cooccurrence_ignores_a_non_adjacent_pair():
    """Thought and externalizing share no neighbour relation, so marking both is not
    evidence of the same underlying process."""
    assert cooccurring({"thought": 0.7, "externalizing": 0.6}) == []


def test_screening_flags_carry_a_disclaimer():
    f = screening_flags(SpectrumVector([0.7, 0.0, 0.0, 0.0, 0.0, 0.0]))
    assert "internalizing" in f["present"]
    assert "diagnosis" in f["disclaimer"].lower()


# ------------------------------------------------------------------ moral

def test_moral_vector_rejects_wrong_width():
    with pytest.raises(ValueError, match="expected 12"):
        MoralVector([0.1] * 6)


def test_polarity_leans_toward_the_care_pole():
    v = MoralVector.from_dict({"care_care": 0.8, "care_harm": 0.2})
    assert v.polarity()["care"] == pytest.approx(0.6)


def test_moral_block_expresses_strong_on_both_poles():
    """The configuration that made this block 12 wide. A signed scalar would read neutral,
    which is exactly the moral-licensing case that goes unnoticed."""
    v = MoralVector.from_dict({"care_care": 0.9, "care_harm": 0.8})
    assert v["care_care"] == 0.9
    assert v["care_harm"] == 0.8
    assert v.polarity()["care"] == pytest.approx(0.1)


def test_manipulation_risk_fires_on_strong_alignment():
    prof = MoralVector.from_dict(
        {"care_care": 1.0, "care_harm": 0.0, "sanctity_care": 0.95, "sanctity_harm": 0.0}
    )
    agenda = MoralVector.from_dict(
        {"care_care": 0.95, "care_harm": 0.0, "sanctity_care": 0.9, "sanctity_harm": 0.0}
    )
    r = is_manipulation_risk(prof.vector, agenda.vector)
    assert r["risk"] is True
    assert "care" in r["foundations"]


def test_manipulation_risk_does_not_fire_on_opposition():
    prof = MoralVector.from_dict({"care_care": 1.0, "care_harm": 0.0})
    agenda = MoralVector.from_dict({"care_care": 0.0, "care_harm": 0.95})
    assert is_manipulation_risk(prof.vector, agenda.vector)["risk"] is False


def test_manipulation_risk_does_not_fire_on_ordinary_agreement():
    """A loose threshold would flag value alignment generally and train the model to
    distrust it."""
    prof = MoralVector.from_dict({"care_care": 1.0, "care_harm": 0.0})
    agenda = MoralVector.from_dict({"care_care": 0.6, "care_harm": 0.0})
    assert is_manipulation_risk(prof.vector, agenda.vector)["risk"] is False


# --------------------------------------------------------------- dynamics

def test_decay_rates_are_ordered_fast_to_slow():
    """Affect, clinical spectrum and moral grounding do not move on the same clock."""
    assert VAD_DECAY < SPECTRA_DECAY < MORAL_DECAY


def test_vad_mean_reverts_to_the_origin():
    s = VarysState(vad=VAD(0.8, 0.8, 0.8))
    for _ in range(500):
        s = transition(s)
    assert s.vad.magnitude == pytest.approx(0.0, abs=1e-6)


def test_spectra_floor_at_the_baseline_once_raised():
    """A spectrum is a trait, not a momentary mood, so it relaxes to a floor rather than to
    nothing. The latch has to survive resting exactly on the floor, which is the case a
    `current > baseline` test gets wrong."""
    s = VarysState(spectra=SpectrumVector([0.6, 0, 0, 0, 0, 0]))
    for _ in range(2000):
        s = transition(s)
    assert s.spectra["internalizing"] == pytest.approx(SPECTRA_BASELINE, abs=1e-6)


def test_untouched_spectra_stay_at_zero_and_report_unmarked():
    """The floor applies to a spectrum that was raised, not to one that never was. Otherwise
    every spectrum reports a baseline value as present."""
    s = VarysState()
    for _ in range(200):
        s = transition(s)
    assert s.spectra.marked == {}


def test_evidence_gate_does_not_open_on_a_single_mention():
    """'My grandmother died last year' is not evidence of a thought disorder."""
    g = EvidenceGate(dims=6, min_confidence=0.5, decay=0.5)
    assert g.observe("internalizing", 0.8, names=SPECTRA_DIMS) == 0.0


def test_evidence_gate_opens_on_sustained_evidence():
    g = EvidenceGate(dims=6, min_confidence=0.5, decay=0.5)
    opened = None
    for i in range(10):
        if g.observe("internalizing", 1.0, names=SPECTRA_DIMS) > 0:
            opened = i + 1
            break
    assert opened is not None


def test_evidence_gate_confidence_decays_during_silence():
    """Absence is an observation of zero, so the state cannot ratchet upward."""
    g = EvidenceGate(dims=6, min_confidence=0.5, decay=0.5)
    for _ in range(4):
        g.observe("internalizing", 1.0, names=SPECTRA_DIMS)
    before = g.confidence(SPECTRA_DIMS)["internalizing"]
    for _ in range(10):
        g.observe(None, 1.0, names=SPECTRA_DIMS)
    assert g.confidence(SPECTRA_DIMS)["internalizing"] < before


def test_weak_evidence_never_registers_however_often_it_repeats():
    """A leaky counter has fixed point strength/(1-decay), so evidence below
    threshold*(1-decay) could be silently dropped forever. The EMA's fixed point is mean
    strength, so a dimension that is only ever weakly present is correctly never reported as
    present."""
    g = EvidenceGate(dims=6, min_confidence=0.5, decay=0.5)
    for _ in range(50):
        assert g.observe("internalizing", 0.2, names=SPECTRA_DIMS) == 0.0
    assert g.confidence(SPECTRA_DIMS)["internalizing"] == pytest.approx(0.2, abs=1e-6)


def test_gate_does_not_delay_response_to_distress():
    """VAD always passes. Requiring repetition before reacting to a disclosure of self-harm
    would be a failure, not a safety feature."""
    g = EvidenceGate(dims=6, min_confidence=0.99, decay=0.99)
    drive = [0.0] * 21
    drive[VAD_SLICE.start + 1] = 0.9   # arousal
    assert gated_transition(neutral_state(), drive, gate=g).vad.arousal > 0.0


# --------------------------------------------------------------- encoders

def test_affect_encoder_emits_a_bounded_21_drive():
    enc = AffectEncoder(dim=16, hidden=8, max_drive=0.7)
    d = enc(torch.randn(4, 16))
    assert d.shape == (4, 21)
    assert float(d.abs().max()) <= 0.7 + 1e-6


def test_affect_encoder_starts_near_zero_drive():
    """A model should not open a conversation convinced the user is in crisis."""
    assert float(AffectEncoder(dim=16, hidden=8)(torch.randn(64, 16)).abs().max()) < 0.2


def test_affect_encoder_rejects_wrong_turn_width():
    with pytest.raises(ValueError, match="expected turn width 16"):
        AffectEncoder(dim=16, hidden=8)(torch.randn(2, 32))


def test_internal_state_predictor_ignores_padding():
    """Without the mask the state becomes a function of batch composition, which is not a
    state at all."""
    pred = InternalStatePredictor(dim=8, hidden=4)
    hidden = torch.zeros(2, 6, 8)
    hidden[0, :4] = 1.0
    mask = torch.tensor([[1.0] * 4 + [0.0] * 2, [1.0] * 6])
    a = pred(hidden, mask)[0]
    hidden2 = hidden.clone()
    hidden2[0, 4:] = 99.0   # pad tokens, should be ignored
    assert torch.allclose(a, pred(hidden2, mask)[0], atol=1e-6)


def test_internal_state_predictor_emits_21():
    pred = InternalStatePredictor(dim=8, hidden=4)
    assert pred(torch.randn(2, 5, 8), torch.ones(2, 5)).shape == (2, 21)


def test_velocity_seeds_on_first_call():
    """A cold EMA reports a full-magnitude step that never happened."""
    vel = MultiAxisVelocity(0.8)
    assert float(vel(torch.randn(2, 21)).abs().max()) == 0.0


def test_velocity_is_zero_for_a_constant_state_and_tracks_a_jump():
    vel = MultiAxisVelocity(0.8)
    a = torch.zeros(1, 21)
    a[0, 1] = 0.5
    vel(a)
    assert float(vel(a).abs().max()) == pytest.approx(0.0, abs=1e-6)
    b = torch.zeros(1, 21)
    b[0, 1] = 0.9
    v = vel(b)
    assert v.shape == (1, 4)
    assert v[0, 1] > 0.0        # d_arousal
    assert v[0, 0] == pytest.approx(0.0, abs=1e-6)


def test_build_state_features_width_matches_contract():
    z = torch.zeros(3, 21)
    assert build_state_features(z).shape == (3, state_feature_dim())
    assert build_state_features(z, include_moral=True).shape[-1] == state_feature_dim(
        include_moral=True
    )


def test_build_state_features_keeps_velocity_four_channels():
    """Reducing velocity to a column would average dVAD into one meaningless number."""
    z = torch.zeros(2, 21)
    f = build_state_features(z, include_velocity=True, velocity=torch.randn(2, 4))
    assert f.shape[-1] == state_feature_dim(include_velocity=True)


def test_build_state_features_zero_fills_absent_channels():
    z = torch.zeros(2, 21)
    f = build_state_features(z, include_moral=True, include_velocity=True, include_intent=True)
    assert f.shape[-1] == state_feature_dim(include_moral=True, include_velocity=True, include_intent=True)
    assert torch.allclose(f[:, 21:25], torch.zeros(2, 4))


def test_build_state_features_rejects_wrong_state_width():
    with pytest.raises(ValueError, match=r"\(B, 21\)"):
        build_state_features(torch.zeros(2, 3))


def test_build_state_features_rejects_wrong_velocity_width():
    with pytest.raises(ValueError, match=r"\(B, 4\)"):
        build_state_features(torch.zeros(2, 21), include_velocity=True, velocity=torch.randn(2, 3))


def test_state_bank_rolls_and_resets():
    bank = VarysStateBank(batch_size=1, max_seq=4, dim=21)
    for i in range(6):
        bank.append(torch.full((1, 21), float(i)))
    assert bank.history().shape == (1, 4, 21)
    assert float(bank.history()[0, -1, 0]) == 5.0
    bank.reset()
    assert float(bank.latest().abs().max()) == 0.0


def test_gated_transition_holds_the_clinical_block_shut():
    g = EvidenceGate(dims=6, min_confidence=0.5, decay=0.5)
    state = neutral_state()
    drive = [0.0] * 21
    drive[SPECTRA_SLICE.start] = 0.9
    for _ in range(5):
        state = gated_transition(state, drive, gate=g)
    assert state.spectra.marked == {}


def test_gated_transition_admits_sustained_evidence():
    g = EvidenceGate(dims=6, min_confidence=0.5, decay=0.5)
    state = neutral_state()
    drive = [0.0] * 21
    drive[SPECTRA_SLICE.start] = 0.9
    for _ in range(6):
        g.observe("internalizing", 0.9, names=SPECTRA_DIMS)
        state = gated_transition(state, drive, gate=g)
    assert state.spectra["internalizing"] > 0.0
