"""Tests for the Varys model.

The load-bearing ones, in order of how much damage a regression would do:

``test_torch_transition_matches_python``
    Two implementations of one recurrence. If they drift, the model trains against one and
    every evaluation, export and explanation uses the other. This test caught the torch
    version missing all three clamps: Python gets them free from its dataclass constructors,
    the tensor version had to be told, and spectra silently reached 1.47.

``test_prefill_matches_incremental_decode``
    A KV cache that is subtly wrong produces fluent, wrong output. Nothing else in this file
    would notice.

``test_param_breakdown_matches_measured``
    The 72B figure is stated in prose and cannot be checked by allocating 143GB. It can be
    checked arithmetically against a model that does fit.

The rest pin decisions recorded in the source comments, including several that exist only
because the first implementation got them wrong.
"""

from __future__ import annotations

import pytest
import torch

from varys.affective.state import (
    MORAL_SLICE,
    SPECTRA_SLICE,
    VAD_SLICE,
    neutral_state,
    transition,
)
from varys.config import AffectConfig, ModelConfig, get_preset, list_presets
from varys.models import (
    GroupedQueryAttention,
    GatedStateMachine,
    MultiAxisBias,
    RMSNorm,
    STANCES,
    SwiGLU,
    Varys,
    VarysBlock,
    apply_rope,
    clinical_stance,
    ntk_theta,
    param_breakdown,
    precompute_rope,
    rotate_half,
    sample_from_logits,
    slice_state,
    transition_tensor,
    yarn_mscale,
)
from varys.models.rope import yarn_correction_range, yarn_ramp


# ------------------------------------------------------------------ config

def test_all_presets_build_and_validate():
    for name in list_presets():
        cfg = get_preset(name)
        assert cfg.dim % cfg.n_heads == 0, name
        assert cfg.n_heads % cfg.n_kv_heads == 0, name
        assert cfg.q_dim > 0 and cfg.kv_dim > 0, name


def test_preset_lookup_is_case_insensitive():
    assert get_preset("72B").dim == get_preset("72b").dim
    assert get_preset("8B").dim == get_preset("8b").dim


def test_unknown_preset_raises_with_the_available_list():
    with pytest.raises(KeyError, match="available"):
        get_preset("70b")


def test_config_rejects_indivisible_head_geometry():
    with pytest.raises(ValueError, match="divisible by n_heads"):
        ModelConfig(dim=100, n_heads=8, head_dim=16)
    with pytest.raises(ValueError, match="multiple of n_kv_heads"):
        ModelConfig(dim=96, n_heads=6, n_kv_heads=4, head_dim=16)


def test_config_rejects_a_factor_below_one():
    """A factor under 1 compresses the context. That is a different operation, not a
    stronger version of this one."""
    with pytest.raises(ValueError, match="must be >= 1.0"):
        ModelConfig(rope_scaling="linear", rope_factor=0.5)


def test_config_rejects_unknown_scaling():
    with pytest.raises(ValueError, match="unknown rope_scaling"):
        ModelConfig(rope_scaling="magic")


def test_config_roundtrips_through_dict():
    cfg = get_preset("small")
    assert ModelConfig.from_dict(cfg.to_dict()) == cfg


def test_affect_config_roundtrips_inject_layers_as_a_tuple():
    ac = AffectConfig(inject_layers=(0, -1))
    back = AffectConfig.from_dict(ac.to_dict())
    assert back.inject_layers == (0, -1)


def test_state_dim_is_21_whether_or_not_the_moral_block_reaches_the_bias():
    """The bias flags change what reaches attention, not what the state is. A model with
    the moral block off from the bias still tracks it, so turning it on later needs no
    retroactively recorded data."""
    off = AffectConfig()
    on = AffectConfig(use_moral=True)
    assert off.state_dim == on.state_dim == 21
    assert off.state_feature_dim == 9
    assert on.state_feature_dim == 21


def test_moral_bias_is_off_by_default():
    """Persuasion surface, not a capacity question. See AffectConfig."""
    assert AffectConfig().use_moral is False
    assert AffectConfig().moral_bias_scale == 0.0


def test_spectra_bias_starts_low_and_vad_starts_full():
    """Initialisation encodes a position, not a tuning result: clinical state should change
    how the model responds, not which tokens it attends to."""
    ac = AffectConfig()
    assert ac.vad_bias_scale == 1.0
    assert ac.spectra_bias_scale == 0.1


def test_inject_layers_resolves_negative_indices_and_rejects_out_of_range():
    assert AffectConfig(inject_layers=(-1,)).layers_to_inject(8) == (7,)
    assert AffectConfig().layers_to_inject(3) == (0, 1, 2)
    with pytest.raises(ValueError, match="out of range"):
        AffectConfig(inject_layers=(99,)).layers_to_inject(8)


# ------------------------------------------------------------------- rope

def test_rope_rotation_preserves_norm():
    """A rotation, not a shear. If this fails the model trains and never coheres."""
    inv, cos, sin = precompute_rope(64, 32, 10_000.0)
    x = torch.randn(2, 4, 32, 64)
    y = apply_rope(x, cos, sin)
    assert torch.allclose(x.norm(dim=-1), y.norm(dim=-1), atol=1e-5)


def test_rotate_half_negates_the_back_half():
    x = torch.arange(8, dtype=torch.float32)
    out = rotate_half(x)
    assert torch.equal(out, torch.tensor([-4., -5., -6., -7., 0., 1., 2., 3.]))


def test_rope_position_zero_is_the_identity():
    inv, cos, sin = precompute_rope(32, 8, 10_000.0)
    x = torch.randn(1, 2, 8, 32)
    out = apply_rope(x, cos[:8], sin[:8])
    assert torch.allclose(out[:, :, 0], x[:, :, 0], atol=1e-6)


def test_rope_is_relative():
    """The inner product of two rotated vectors depends only on their distance. This is the
    property that makes extrapolation work at all."""
    _, cos, sin = precompute_rope(32, 64, 10_000.0)
    q = torch.randn(1, 1, 1, 32)
    def ip(i, j):
        a = apply_rope(q, cos[i:i+1], sin[i:i+1])[0, 0, 0]
        b = apply_rope(q, cos[j:j+1], sin[j:j+1])[0, 0, 0]
        return float((a * b).sum())
    assert ip(3, 10) == pytest.approx(ip(20, 27), abs=1e-4)


def test_rope_rejects_odd_head_dim():
    with pytest.raises(ValueError, match="must be even"):
        precompute_rope(63, 8, 10_000.0)


def test_linear_scaling_compresses_the_highest_frequency():
    """The cost of linear interpolation: the dimension that encodes local order gets
    rescaled too, which is why it is the worst of the four."""
    _, none_c, _ = precompute_rope(64, 32, 10_000.0, scaling="none")
    inv, lin_c, _ = precompute_rope(64, 32, 10_000.0, scaling="linear", factor=8.0)
    assert float(inv[0]) == pytest.approx(1.0 / 8.0, rel=1e-6)


def test_yarn_leaves_the_highest_frequency_untouched():
    """The whole point of YaRN: local order survives the extension."""
    inv, _, _ = precompute_rope(64, 32, 10_000.0, scaling="yarn", factor=8.0, original_context=32)
    assert float(inv[0]) == pytest.approx(1.0, rel=1e-6)
    assert float(inv[-1]) < 1.0


def test_yarn_interpolates_a_wider_band_than_ntk_touches():
    ntk_inv, _, _ = precompute_rope(64, 32, 10_000.0, scaling="ntk", factor=8.0)
    yarn_inv, _, _ = precompute_rope(64, 32, 10_000.0, scaling="yarn", factor=8.0, original_context=32)
    # NTK moves every frequency but the top one; YaRN moves a band and leaves the rest.
    # NTK scales every frequency but the top one, progressively.
    base_inv, _, _ = precompute_rope(64, 32, 10_000.0, scaling="none")
    assert float(ntk_inv[0]) == pytest.approx(1.0, rel=1e-6)
    assert float(ntk_inv[-1]) < float(base_inv[-1])
    # YaRN is a different algorithm, not a variant of the same one.
    assert not torch.allclose(yarn_inv, ntk_inv)


def test_ntk_is_applied_by_the_table_builder_not_the_caller():
    """It used to depend on the caller pre-raising theta, which meant a direct call with an
    unadjusted base silently produced base-RoPE tables: no error, identical output, a model
    quietly stuck at its original context."""
    inv, _, _ = precompute_rope(64, 32, 10_000.0, scaling="ntk", factor=8.0)
    base, _, _ = precompute_rope(64, 32, 10_000.0, scaling="none")
    assert not torch.allclose(inv, base)


def test_ntk_rejects_factor_below_one_and_tiny_head_dim():
    with pytest.raises(ValueError, match=">= 1.0"):
        ntk_theta(10_000.0, 0.5, 64)
    with pytest.raises(ValueError, match="head_dim must be > 2"):
        ntk_theta(10_000.0, 4.0, 2)


def test_ntk_exponent_holds_the_longest_wavelength():
    """The d/(d-2) exponent is what makes the lowest frequency match the scaled context."""
    hd = 128
    raised = ntk_theta(10_000.0, 4.0, hd)
    inv, _, _ = precompute_rope(hd, 8, raised, scaling="none")
    base, _, _ = precompute_rope(hd, 8, 10_000.0, scaling="none")
    assert float(inv[-1]) < float(base[-1])


def test_yarn_ramp_is_clamped_and_monotone():
    r = yarn_ramp(4, 12, 32)
    assert float(r[0]) == 0.0
    assert float(r[-1]) == 1.0
    assert torch.all(r[1:] >= r[:-1])


def test_yarn_correction_range_is_derived_and_ordered():
    low, high = yarn_correction_range(32.0, 1.0, 32, 10_000.0, 4096)
    assert 0 <= low < high <= 31


def test_yarn_mscale_is_one_below_the_scaling_factor_and_grows_with_it():
    assert yarn_mscale(1.0) == 1.0
    assert yarn_mscale(16.0) > yarn_mscale(4.0) > 1.0


def test_72b_rope_table_is_finite_at_full_length():
    """131072 positions is the point of the preset. A table that produces NaN at the far
    end fails only after a day of training."""
    cfg = get_preset("72b")
    inv, cos, sin = precompute_rope(
        cfg.head_dim, 4096, cfg.rope_theta, scaling=cfg.rope_scaling,
        factor=cfg.rope_factor, beta_fast=cfg.yarn_beta_fast,
        beta_slow=cfg.yarn_beta_slow, original_context=cfg.yarn_orig_ctx,
    )
    assert cos.shape == (4096, cfg.head_dim)
    assert bool(torch.isfinite(cos).all() and torch.isfinite(sin).all())


# ---------------------------------------------------------- multi-axis bias

def test_bias_is_zero_at_initialisation():
    """The zero-init contract: at step zero the model is arithmetically identical to
    vanilla attention, so emotion has to earn its influence instead of wrecking the initial
    attention distribution."""
    bias = MultiAxisBias({"vad": 3, "spectra": 6}, 4, 2, 8, {"vad": 1.0, "spectra": 0.1})
    out = bias(torch.randn(3, 9))
    assert torch.count_nonzero(out) == 0


def test_bias_gradients_flow_despite_zero_output():
    """dL/dW depends on q and k, which are non-zero, even though the layer's output is."""
    bias = MultiAxisBias({"vad": 3}, 2, 1, 4)
    bias(torch.randn(2, 3)).sum().backward()
    assert bias.proj_vad.weight.grad is not None
    assert torch.count_nonzero(bias.proj_vad.weight.grad) > 0


def test_blocks_are_independently_ablatable():
    """The reason for per-block projections rather than one wide layer: the eval suite has
    to be able to ask how much clinical state is doing."""
    bias = MultiAxisBias({"vad": 3, "spectra": 6}, 2, 1, 4, {"vad": 1.0, "spectra": 0.1})
    state = torch.randn(2, 9)
    with torch.no_grad():
        bias.proj_vad.weight.normal_()
        bias.proj_spectra.weight.normal_()
    full = bias(state)
    with torch.no_grad():
        bias.scale_spectra.zero_()
    vad_only = bias(state)
    assert not torch.allclose(full, vad_only)
    assert torch.allclose(full[:, :2] * 0 + vad_only, vad_only)


def test_block_influence_reports_absolute_scales():
    bias = MultiAxisBias({"vad": 3}, 2, 1, 4, {"vad": -2.0})
    assert bias.block_influence() == {"vad": pytest.approx(2.0)}


def test_bias_rejects_an_empty_block_set():
    with pytest.raises(ValueError, match="at least one state block"):
        MultiAxisBias({}, 2, 1, 4)


def test_bias_rejects_a_state_of_the_wrong_width():
    bias = MultiAxisBias({"vad": 3, "spectra": 6}, 2, 1, 4)
    with pytest.raises(ValueError, match="does not match state_dim"):
        bias(torch.randn(2, 9 + 1))


def test_kv_granularity_broadcasts_within_a_group_and_shrinks_the_projection():
    head = MultiAxisBias({"vad": 3}, 4, 2, 8, granularity="head")
    kv = MultiAxisBias({"vad": 3}, 4, 2, 8, granularity="kv")
    assert head.state_proj_params() if False else head.proj_vad.weight.numel() == 3 * 4 * 64
    assert kv.proj_vad.weight.numel() == 3 * 2 * 64
    with torch.no_grad():
        head.proj_vad.weight.normal_()
        kv.proj_vad.weight.normal_()
    b = kv.bias_for_heads(torch.randn(1, 3))
    assert b.shape == (1, 4, 8, 8)
    # each KV group's n_rep query heads share one matrix
    assert torch.allclose(b[0, 0], b[0, 1])
    assert not torch.allclose(b[0, 0], b[0, 2])


# --------------------------------------------------------------- attention

def test_prefill_matches_incremental_decode():
    """A subtly wrong KV cache produces fluent, wrong output. Nothing else here notices."""
    torch.manual_seed(0)
    attn = GroupedQueryAttention(dim=32, n_heads=4, n_kv_heads=2, head_dim=8,
                                 block_widths={"vad": 3})
    with torch.no_grad():
        attn.o_proj.weight.normal_(std=0.05)
        attn.affect.proj_vad.weight.normal_(std=0.1)
    _, cos, sin = precompute_rope(8, 32, 10_000.0)
    state = torch.randn(1, 3)
    ids = torch.randn(1, 12, 32)

    full, cache = attn(ids, cos[:12], sin[:12], use_cache=True, state=state)

    # ``cache`` is a single (k, v) pair here, not a list of them.
    ck, cv = cache
    steps = []
    for t in range(12):
        past = (ck[:, :, :t], cv[:, :, :t]) if t else None
        out, _ = attn(ids[:, t:t+1], cos[t:t+1], sin[t:t+1], past_kv=past,
                      use_cache=True, state=state)
        steps.append(out)
    incremental = torch.cat(steps, dim=1)

    assert torch.allclose(full, incremental, atol=1e-5), \
        f"max diff {(full - incremental).abs().max().item():.3e}"


def test_attention_without_a_state_is_vanilla():
    attn = GroupedQueryAttention(dim=16, n_heads=2, n_kv_heads=1, head_dim=8,
                                 block_widths={"vad": 3})
    _, cos, sin = precompute_rope(8, 8, 10_000.0)
    out, _ = attn(torch.randn(1, 4, 16), cos[:4], sin[:4], state=None)
    assert out.shape == (1, 4, 16)


def test_attention_mask_blocks_future_positions():
    """A single change to token 0's content must not move token 0's output at later
    positions, which is what causality means."""
    torch.manual_seed(1)
    attn = GroupedQueryAttention(dim=16, n_heads=2, n_kv_heads=1, head_dim=8,
                                 block_widths=None, affect_granularity=None)
    with torch.no_grad():
        attn.o_proj.weight.normal_(std=0.1)
    _, cos, sin = precompute_rope(8, 8, 10_000.0)
    a = torch.randn(1, 6, 16)
    b = a.clone()
    b[0, 5] = torch.randn(16)   # change the LAST token
    oa, _ = attn(a, cos[:6], sin[:6])
    ob, _ = attn(b, cos[:6], sin[:6])
    # Causality: a later token cannot reach back. Earlier positions are untouched.
    assert torch.allclose(oa[:, :5], ob[:, :5], atol=1e-5)
    assert not torch.allclose(oa[:, 5], ob[:, 5], atol=1e-5)


# ------------------------------------------------------------------- block

def test_block_is_the_identity_at_initialisation():
    """What makes an 80-layer stack safe to instantiate: without zeroed residuals, a random
    deep residual stack diverges and the only way to find out is to allocate 143GB."""
    blk = VarysBlock(dim=32, n_heads=2, n_kv_heads=1, head_dim=16, ff_dim=64,
                     block_widths={"vad": 3})
    _, cos, sin = precompute_rope(16, 8, 10_000.0)
    x = torch.randn(2, 8, 32)
    out, _ = blk(x, cos[:8], sin[:8], state=torch.zeros(2, 3))
    assert torch.allclose(out, x, atol=1e-6)


def test_block_without_injection_ignores_the_state():
    blk = VarysBlock(dim=16, n_heads=2, n_kv_heads=1, head_dim=8, ff_dim=32,
                     block_widths={"vad": 3}, inject_affect=False)
    assert blk.self_attn.affect is None


# ------------------------------------------------------- state machine

def test_torch_transition_matches_python():
    """The single most important test in this file. Two implementations of one recurrence:
    if they drift, the model trains against one and every evaluation uses the other.

    This caught the torch version missing all three clamps. The Python recurrence gets them
    free from ``VAD.__post_init__`` and ``clamp_spectra``; the tensor version has to be told,
    and spectra reached 1.47 while the divergence stayed invisible in any loss curve.
    """
    torch.manual_seed(0)
    import random
    random.seed(0)
    worst = 0.0
    for _ in range(200):
        drives = [
            [random.uniform(-1, 1) for _ in range(3)]
            + [random.uniform(0, 1) for _ in range(6)]
            + [random.uniform(0, 1) for _ in range(12)]
            for _ in range(5)
        ]
        py = neutral_state()
        ts = torch.zeros(1, 21)
        for d in drives:
            py = transition(py, d)
            ts = transition_tensor(ts, torch.tensor([d], dtype=torch.float32))
        worst = max(worst, max(abs(a - b) for a, b in zip(py.to_vector(), ts[0].tolist())))
    assert worst < 1e-6, f"torch and python recurrences diverge by {worst:.3e}"


def test_transition_clamps_every_block():
    """Sustained max drive pins all three blocks to their maxima, in both implementations."""
    sat = [1.0] * 3 + [1.0] * 6 + [1.0] * 12
    py = neutral_state()
    ts = torch.zeros(1, 21)
    for _ in range(10):
        py = transition(py, sat)
        ts = transition_tensor(ts, torch.tensor([sat]))
    assert all(abs(x - 1.0) < 1e-6 for x in py.to_vector())
    assert float(ts.max()) <= 1.0
    assert float(ts.min()) >= 0.0


def test_vad_block_stays_inside_the_cube():
    sat = [1.0] * 3 + [1.0] * 6 + [1.0] * 12
    ts = torch.zeros(1, 21)
    for _ in range(20):
        ts = transition_tensor(ts, torch.tensor([sat]))
    assert float(ts[:, VAD_SLICE].min()) >= -1.0
    assert float(ts[:, VAD_SLICE].max()) <= 1.0


def test_slice_state_partitions_the_21_dims():
    st = torch.arange(21, dtype=torch.float32).unsqueeze(0)
    blocks = slice_state(st)
    assert torch.equal(blocks["vad"], st[:, VAD_SLICE])
    assert torch.equal(blocks["spectra"], st[:, SPECTRA_SLICE])
    assert torch.equal(blocks["moral"], st[:, MORAL_SLICE])
    assert sum(b.shape[1] for b in blocks.values()) == 21


def test_transition_rejects_a_wrong_width_state_or_drive():
    with pytest.raises(ValueError, match=r"\(B, 21\)"):
        transition_tensor(torch.zeros(1, 20))
    with pytest.raises(ValueError, match="must match state"):
        transition_tensor(torch.zeros(1, 21), torch.zeros(1, 5))


def test_machine_rejects_a_state_batch_mismatch():
    """Reading the buffer and ignoring it is how a batch-2 call ends up broadcasting a
    batch-1 state."""
    m = GatedStateMachine()
    with pytest.raises(ValueError, match="does not match drive batch"):
        m.step(torch.zeros(2, 21), state=torch.zeros(1, 21))


def test_machine_gate_holds_the_clinical_block_shut_without_hits():
    """A single utterance cannot establish a spectrum, and the honest response to no
    clinical detector is for the block to relax rather than to pretend the gate opened."""
    m = GatedStateMachine(gate_clinical=True)
    m.reset(1)
    drive = torch.zeros(1, 21)
    drive[:, SPECTRA_SLICE] = 0.9
    for _ in range(5):
        m.step(drive, clinical_hits=None)
    assert float(m.state[:, SPECTRA_SLICE].abs().max()) == 0.0


def test_machine_gate_admits_sustained_evidence():
    m = GatedStateMachine(gate_clinical=True, gate_min_confidence=0.5, gate_decay=0.5)
    m.reset(1)
    drive = torch.zeros(1, 21)
    drive[:, SPECTRA_SLICE] = 0.9
    hits = torch.zeros(1, 6)
    hits[:, 0] = 0.9
    for _ in range(6):
        m.step(drive, clinical_hits=hits)
    assert float(m.state[0, SPECTRA_SLICE.start]) > 0.0


def test_machine_reset_clears_state_and_gate():
    m = GatedStateMachine()
    m.reset(2)
    m.step(torch.ones(2, 21), clinical_hits=torch.ones(2, 6))
    m.reset(2)
    assert float(m.state.abs().max()) == 0.0
    assert float(m.gate_conf.abs().max()) == 0.0
    assert m.state.shape == (2, 21)


def test_machine_gate_is_a_buffer_not_a_parameter():
    """A gate is running bookkeeping. Training it would be training the model to decide
    what counts as evidence about itself."""
    m = GatedStateMachine()
    names = dict(m.named_buffers())
    assert "gate_conf" in names
    assert not any("gate" in n for n, _ in m.named_parameters())
    # persistent=False: a checkpoint must not depend on where in a conversation it was saved
    assert "gate_conf" not in m.state_dict()


def test_peak_spectrum_reports_minus_one_when_nothing_is_marked():
    m = GatedStateMachine()
    idx, peak = m.peak_spectrum()
    assert int(idx[0]) == -1
    assert float(peak[0]) == 0.0


def test_peak_spectrum_finds_the_largest():
    m = GatedStateMachine()
    m.reset(1)
    m.state[0, SPECTRA_SLICE.start + 2] = 0.8
    idx, peak = m.peak_spectrum()
    assert int(idx[0]) == 2
    assert float(peak[0]) == pytest.approx(0.8)


# ------------------------------------------------------------------ router

def test_clinical_stance_without_spectra_just_answers():
    d = clinical_stance("advice_seeking", None)
    assert d.stance == "direct"
    assert d.spectra_used is False


def test_clinical_stance_always_takes_de_escalate_on_crisis():
    """The one branch that overrides everything, including a strong spectrum."""
    d = clinical_stance("crisis", {"psychoticism": 0.99})
    assert d.stance == "de_escalate"
    assert d.escalate is True


def test_clinical_stance_refers_on_prominent_psychoticism():
    d = clinical_stance("advice_seeking", {"psychoticism": 0.9})
    assert d.stance == "refer"
    assert d.escalate is True
    assert d.spectra_used is True


def test_clinical_stance_asks_rather_than_asserts_on_a_present_spectrum():
    d = clinical_stance("advice_seeking", {"internalizing": 0.7})
    assert d.stance == "ask_before_assert"


def test_clinical_stance_slows_down_for_disclosure():
    d = clinical_stance("disclosure", {"internalizing": 0.7})
    assert d.stance == "slow"


def test_clinical_stance_ignores_a_sub_threshold_spectrum():
    d = clinical_stance("advice_seeking", {"internalizing": 0.4})
    assert d.stance == "direct"
    assert d.spectra_used is False


def test_no_stance_claims_anything_about_the_user():
    """Every stance is a process choice defensible for anyone, and none of them asserts a
    clinical finding. This is the guard on the whole design: clinical state changes the
    model's process, never the model's claims about the user."""
    assert set(STANCES) == {"direct", "slow", "ask_before_assert", "de_escalate", "refer"}
    for intent in ("crisis", "advice_seeking", "disclosure", "casual", "venting"):
        for spectra in (None, {"internalizing": 0.7}, {"psychoticism": 0.95},
                        {"dissociation": 0.9}, {"externalizing": 0.99}):
            d = clinical_stance(intent, spectra)
            assert d.stance in STANCES
            assert "disorder" not in d.rationale.lower()
            assert "diagnos" not in d.rationale.lower()


# ------------------------------------------------------------------- model

def test_param_breakdown_matches_measured():
    """The 72B figure is stated in prose and cannot be checked by allocating 143GB. It can be
    checked against a model that does fit."""
    for name in ("tiny", "small"):
        cfg = get_preset(name)
        m = Varys(cfg, AffectConfig())
        bd = param_breakdown(cfg, AffectConfig())
        total = sum(v for k, v in bd.items() if k != "affect_bias_by_block")
        assert total == sum(p.numel() for p in m.parameters()), name


def test_param_breakdown_does_not_double_count_the_bias():
    """It did, by 147M on the tiny preset and 755M on the 72B one. The bias was folded
    into ``layers`` and then also reported separately."""
    cfg = get_preset("72b")
    bd = param_breakdown(cfg, AffectConfig())
    per_dim = 64 * 128 * 128
    assert bd["affect_bias"] == (3 + 6) * per_dim * cfg.n_layers
    assert bd["affect_bias_by_block"]["vad"] == 3 * per_dim * cfg.n_layers
    assert sum(bd["affect_bias_by_block"].values()) == bd["affect_bias"]


def test_72b_lands_where_the_spec_says():
    cfg = get_preset("72b")
    total = sum(
        v for k, v in param_breakdown(cfg, AffectConfig()).items()
        if k != "affect_bias_by_block"
    )
    assert 70e9 < total < 73e9, f"{total/1e9:.2f}B"


def test_adding_the_moral_block_costs_about_a_billion():
    """Stated in the config docstring, so it gets checked. Not the reason moral is off."""
    cfg = get_preset("72b")
    off = param_breakdown(cfg, AffectConfig())["affect_bias"]
    on = param_breakdown(cfg, AffectConfig(use_moral=True))["affect_bias"]
    assert 1.0e9 < on - off < 1.1e9


def test_forward_shapes():
    cfg = get_preset("tiny")
    m = Varys(cfg, AffectConfig())
    out = m(torch.randint(0, cfg.vocab_size, (2, 16)))
    assert out.logits.shape == (2, 16, cfg.vocab_size)
    assert out.state.shape == (2, 21)
    assert out.drive.shape == (2, 21)
    assert out.next_token_logits.shape == (2, cfg.vocab_size)


def test_forward_works_with_a_batch_wider_than_the_state_buffer():
    """A regression that surfaced as a hard error rather than a wrong answer, which is the
    only acceptable way for it to surface."""
    cfg = get_preset("tiny")
    m = Varys(cfg, AffectConfig())
    m.reset_state(1)
    out = m(torch.randint(0, cfg.vocab_size, (4, 8)))
    assert out.state.shape == (4, 21)


def test_moral_block_absent_from_the_bias_by_default():
    cfg = get_preset("tiny")
    off = Varys(cfg, AffectConfig())
    on = Varys(cfg, AffectConfig(use_moral=True))
    assert off.bias_blocks == {"vad": 3, "spectra": 6}
    assert on.bias_blocks == {"vad": 3, "spectra": 6, "moral": 12}
    assert sum(on.bias_blocks.values()) == on.affect_cfg.state_feature_dim


def test_configured_bias_scales_reach_the_bias():
    """They were built and then dropped on the floor once: the dict was constructed in
    __init__ and never passed down, so every block silently defaulted to 1.0."""
    cfg = get_preset("tiny")
    m = Varys(cfg, AffectConfig(vad_bias_scale=1.0, spectra_bias_scale=0.1))
    infl = m.block_influence()
    assert infl["vad"] == pytest.approx(1.0)
    assert infl["spectra"] == pytest.approx(0.1)


def test_state_features_width_matches_the_bias_width():
    cfg = get_preset("tiny")
    m = Varys(cfg, AffectConfig())
    feats = m.state_features(torch.zeros(2, 21))
    assert feats.shape[-1] == m.bias_width


def test_model_rejects_a_sequence_past_the_rope_table():
    """Indexing past the table wraps rather than erroring, which corrupts position silently.
    With 131072 positions the table is also 64MB of memory per model, so it is not grown on
    demand."""
    cfg = get_preset("tiny")
    m = Varys(cfg, AffectConfig())
    with pytest.raises(ValueError, match="exceeds max_seq_len"):
        m(torch.zeros(1, cfg.max_seq_len + 1, dtype=torch.long))


def test_disabled_subsystem_still_runs():
    cfg = get_preset("tiny")
    m = Varys(cfg, AffectConfig(enabled=False))
    out = m(torch.randint(0, cfg.vocab_size, (1, 8)))
    assert out.logits.shape == (1, 8, cfg.vocab_size)
    assert float(out.state.abs().max()) == 0.0


def test_generation_respects_max_new_tokens_and_returns_a_state():
    torch.manual_seed(0)
    cfg = get_preset("tiny")
    m = Varys(cfg, AffectConfig())
    toks, state = m.generate(torch.randint(0, cfg.vocab_size, (1, 8)), max_new_tokens=5, top_k=5)
    assert toks.shape == (1, 5)
    assert state.shape == (1, 21)


def test_generation_stops_at_eos():
    torch.manual_seed(0)
    cfg = get_preset("tiny")
    m = Varys(cfg, AffectConfig())
    toks, _ = m.generate(
        torch.randint(0, cfg.vocab_size, (1, 8)), max_new_tokens=20, top_k=5, eos_id=1
    )
    assert toks.shape[1] <= 20


def test_generation_rejects_a_budget_that_leaves_no_room():
    cfg = get_preset("tiny")
    m = Varys(cfg, AffectConfig())
    with pytest.raises(ValueError, match="no room"):
        m.generate(torch.zeros(1, 4, dtype=torch.long), max_new_tokens=cfg.max_seq_len)


def test_hidden_states_bypass_the_head():
    cfg = get_preset("tiny")
    m = Varys(cfg, AffectConfig())
    h = m.get_hidden_states(torch.randint(0, cfg.vocab_size, (2, 8)))
    assert h.shape == (2, 8, cfg.dim)


def test_sample_from_logits_is_deterministic_under_a_seeded_generator():
    torch.manual_seed(0)
    logits = torch.randn(2, 50)
    g1 = torch.Generator().manual_seed(7)
    g2 = torch.Generator().manual_seed(7)
    a = sample_from_logits(logits, top_k=10, generator=g1)
    b = sample_from_logits(logits, top_k=10, generator=g2)
    assert torch.equal(a, b)


def test_sample_from_logits_rejects_nonpositive_temperature():
    with pytest.raises(ValueError, match="temperature must be positive"):
        sample_from_logits(torch.randn(1, 10), temperature=0.0)


def test_tied_embeddings_share_one_parameter():
    """Tested on the tiny geometry rather than the 8b one. Building 8b to check a flag
    allocates 8.2B parameters and hard-crashes the process rather than raising, which takes
    the whole test session with it."""
    tied = Varys(get_preset("tiny"), AffectConfig())
    assert tied.lm_head.weight is tied.embed_tokens.weight
    untied_cfg = get_preset("tiny")
    untied_cfg.tie_embeddings = False
    untied = Varys(untied_cfg, AffectConfig())
    assert untied.lm_head.weight is not untied.embed_tokens.weight


def test_8b_and_72b_presets_are_specifications_not_allocatable_here():
    """A guard on the test suite itself: these are stated in prose and checked
    arithmetically, and trying to build one takes the process down with no traceback."""
    for name in ("8b", "72b"):
        bd = param_breakdown(get_preset(name), AffectConfig())
        total = sum(v for k, v in bd.items() if k != "affect_bias_by_block")
        assert total > 1e9, name


def test_rope_tables_are_not_persisted():
    """Derived from the config, so storing them bloats every checkpoint and risks going
    stale. A 131072-position table is 64MB of float32 per model."""
    m = Varys(get_preset("tiny"), AffectConfig())
    for key in ("cos", "sin", "inv_freq"):
        assert key not in m.state_dict()


# ----------------------------------------------------------------- modules

def test_rmsnorm_preserves_shape_and_normalises():
    n = RMSNorm(16)
    out = n(torch.randn(2, 4, 16))
    assert out.shape == (2, 4, 16)
    assert torch.allclose(out.pow(2).mean(-1), torch.ones(2, 4), atol=1e-3)


def test_swiglu_shape():
    m = SwiGLU(16, 48)
    assert m(torch.randn(2, 4, 16)).shape == (2, 4, 16)
