"""Config validation and preset parameter counts.

The preset names carry parameter counts. If the analytic breakdown and the real module ever
disagree, one of them has a bug, so this file asserts they agree and that the name is honest
about what it builds.
"""

from __future__ import annotations

import re

import pytest

from elafry.config import (
    AffectConfig,
    ModelConfig,
get_preset,
    list_presets,
    param_count,
)

from elafry.models.elafry import Elafry, param_breakdown


def _label_in_millions(name: str):
    """Pull the parameter count out of the preset name, in millions.

    ``elafry-8b`` -> 8000.0, ``elafry-316m`` -> 316.0. Returns None for names that carry no
    count, such as ``elafry-tiny``.
    """
    m = re.search(r"(\d+(?:\.\d+)?)([mb])$", name)
    if not m:
        return None
    value, unit = float(m.group(1)), m.group(2)
    return value * 1000 if unit == "b" else value


def _total_millions(name: str) -> float:
    preset = get_preset(name)
    return sum(param_breakdown(preset["model"], preset["affect"]).values()) / 1e6


def test_analytic_breakdown_matches_real_module():
    """The analytic count must equal ``sum(p.numel())``.

    This is the whole reason for keeping an analytic count: it can be checked without
    allocating the model, so the check scales to 8B.
    """
    checked = 0
    for name in list_presets():
        preset = get_preset(name)
        cfg: ModelConfig = preset["model"]
        affect: AffectConfig = preset["affect"]
        analytic = sum(param_breakdown(cfg, affect).values())

        # Skip anything that would need gigabytes just to count its parameters.
        if analytic > 400e6:
            continue

        model = Elafry(cfg, affect)
        real = sum(p.numel() for p in model.parameters())
        assert analytic == real, (
            f"{name}: analytic {analytic:,} != real {real:,}; "
            "param_breakdown has drifted from the module construction"
        )
        checked += 1

    assert checked >= 3, f"only checked {checked} presets against the real module"


def test_preset_names_match_actual_sizes():
    """A preset called 8b should be about 8b.

    The breakdown includes the affective subsystem, which every preset switches on, so the
    real number sits a little above the bare transformer count. That is the honest number.
    """
    for name in list_presets():
        expected = _label_in_millions(name)
        if expected is None:
            continue
        actual = _total_millions(name)
        assert abs(actual - expected) / expected < 0.07, (
            f"{name} builds {actual:.1f}M but the name says {expected:.1f}M"
        )


def test_every_preset_validates():
    for name in list_presets():
        cfg: ModelConfig = get_preset(name)["model"]
        assert cfg.dim % cfg.n_heads == 0
        assert cfg.n_heads % cfg.n_kv_heads == 0
        assert cfg.rope_theta > 0
        assert cfg.max_seq_len > 0


def test_dim_must_divide_by_heads():
    with pytest.raises(ValueError, match="not divisible"):
        ModelConfig(dim=100, n_heads=12, n_kv_heads=4)


def test_heads_must_be_multiple_of_kv_heads():
    with pytest.raises(ValueError, match="not a multiple"):
        ModelConfig(dim=96, n_heads=12, n_kv_heads=5)


def test_config_roundtrips_through_dict():
    cfg = ModelConfig(dim=64, n_layers=2, n_heads=4, n_kv_heads=2, head_dim=16, ff_dim=128)
    assert ModelConfig.from_dict(cfg.to_dict()) == cfg


def test_inject_layers_resolves_negative_indices():
    assert AffectConfig(inject_layers=(-1,)).layers_to_inject(12) == (11,)
    assert AffectConfig(inject_layers=(-2, -1)).layers_to_inject(12) == (10, 11)
    assert AffectConfig(inject_layers=(0, 3)).layers_to_inject(12) == (0, 3)


def test_inject_layers_defaults_to_all():
    assert AffectConfig().layers_to_inject(6) == tuple(range(6))


def test_inject_layers_rejects_out_of_range():
    with pytest.raises(ValueError, match="out of range"):
        AffectConfig(inject_layers=(99,)).layers_to_inject(12)


def test_param_count_helper_agrees_with_module():
    preset = get_preset("elafry-tiny")
    assert param_count("elafry-tiny") == sum(
        p.numel() for p in Elafry(preset["model"], preset["affect"]).parameters()
    )


def test_8b_is_untied_and_lands_on_8b():
    """The 128k-vocab presets drop weight tying, which is the only reason they reach their
    stated size. Tying would give 7.50B."""
    preset = get_preset("elafry-8b")
    assert preset["model"].tie_embeddings is False
    total = sum(param_breakdown(preset["model"], preset["affect"]).values())
    assert 8.0e9 < total < 8.1e9


def test_8b_would_miss_the_mark_if_tied():
    """Documents why the untied choice exists, so nobody 'simplifies' it back."""
    preset = get_preset("elafry-8b")
    tied = ModelConfig.from_dict({**preset["model"].to_dict(), "tie_embeddings": True})
    tied_total = sum(param_breakdown(tied, preset["affect"]).values())
    assert tied_total < 7.6e9


def test_100m_geometry_matches_the_existing_checkpoint():
    """Elafry's 100M preset is deliberately the 12-layer/768-dim shape of the checkpoint
    already sitting in the parent repo, so the two are directly comparable."""
    cfg = get_preset("elafry-100m")["model"]
    assert (cfg.n_layers, cfg.dim, cfg.n_heads, cfg.n_kv_heads, cfg.ff_dim) == (
        12,
        768,
        12,
        4,
        2048,
    )


def test_state_feature_width_defaults_to_bare_vad():
    assert AffectConfig().state_feature_dim == 3
    assert AffectConfig(use_intent=True).state_feature_dim == 4
    assert AffectConfig(use_intent=True, use_velocity=True, use_crisis=True).state_feature_dim == 6


def test_unknown_preset_raises_with_a_helpful_message():
    with pytest.raises(KeyError, match="unknown preset"):
        get_preset("elafry-69b")