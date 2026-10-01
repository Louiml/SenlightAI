"""Model presets.

Four geometries. ``tiny`` exists so the test suite runs on a 12GB card; ``72b`` is the one the
project is actually about and is never allocated on this machine.

## The 72B numbers, honestly

Elafry's 8B is a real trained model. Varys's 72B is a *specification*, and the difference
matters for how the parameter budget should be read.

``varys-72b`` lands at roughly 71.4B parameters with the affective subsystem's per-block bias
enabled, untied embeddings, and no moral block. The round "72B+" in the spec is a scale
description, not a claim of an exact count. Two things would change it materially:

* Untying a 131k vocab at ``dim=8192`` is 2.15B parameters, 3% of the model, spent entirely
  on output projection. ``tie_embeddings=True`` recovers all of it and costs a little
  quality on large-vocab models. It is left untied because the spec says 72B+ and the
  untying is what gets there honestly.
* The affective bias is the one part of the budget that is *new* relative to Elafry. Per
  block, per layer, it costs ``state_block_dim * n_bias * head_dim^2``. At 80 layers with
  64 query heads and ``head_dim=128`` that is 1.05M parameters per dimension of state per
  block, so the 3 VAD dimensions cost 251M and the 6 spectra cost 503M. Enabling the moral
  block would add 1.0B, which is 1.4% of the model and the reason it is opt-in on cost
  grounds as well as the safety grounds.

Neither preset here has been trained. Nothing in Varys has seen data. See ``SCALING.md`` for
what a real 72B campaign needs and ``README.md`` for why no dataset ships with the code.
"""

from __future__ import annotations

from typing import Dict, List

from varys.config.base import AffectConfig, ModelConfig

__all__ = ["get_preset", "list_presets", "affect_for", "PRESET_NOTES"]


PRESET_NOTES: Dict[str, str] = {
    "tiny": "Test preset. Fits in a few hundred MB, runs the whole suite on CPU or a 3060.",
    "small": "Development preset. A real training run at this size will overfit on purpose "
             "and is used to prove the loop works, not to produce a usable model.",
    "8b": "Elafry's geometry. Not a Varys target, but the cheapest way to check that Varys's "
          "additions are the only difference from a known-good model.",
    "72b": "The Varys target. Specification only. Never allocated outside a server cluster.",
}


def _tiny() -> ModelConfig:
    return ModelConfig(
        vocab_size=1024,
        dim=128,
        n_layers=4,
        n_heads=4,
        n_kv_heads=2,
        head_dim=32,
        ff_dim=352,
        rope_theta=10_000.0,
        max_seq_len=512,
        tie_embeddings=True,
    )


def _small() -> ModelConfig:
    return ModelConfig(
        vocab_size=8192,
        dim=512,
        n_layers=8,
        n_heads=8,
        n_kv_heads=4,
        head_dim=64,
        ff_dim=1408,
        rope_theta=100_000.0,
        max_seq_len=2048,
        tie_embeddings=True,
    )


def _8b() -> ModelConfig:
    """Elafry's geometry, for the comparison run.

    Kept deliberately identical to Elafry's 8B preset so that any behavioural difference
    between the two projects is attributable to the affective additions rather than to a
    different backbone.
    """
    return ModelConfig(
        vocab_size=128_000,
        dim=4096,
        n_layers=32,
        n_heads=32,
        n_kv_heads=8,
        head_dim=128,
        ff_dim=14336,
        rope_theta=500_000.0,
        max_seq_len=8192,
        tie_embeddings=False,
    )


def _72b() -> ModelConfig:
    return ModelConfig(
        vocab_size=131_072,
        dim=8192,
        n_layers=80,
        n_heads=64,
        n_kv_heads=8,
        head_dim=128,
        ff_dim=28672,
        rope_theta=500_000.0,
        max_seq_len=131_072,
        tie_embeddings=False,
        # 8k was the context the base theta was chosen for; 128k is 16x that, so the
        # angles get YaRN rather than being left to extrapolate into noise.
        rope_scaling="yarn",
        rope_factor=16.0,
        yarn_beta_fast=32.0,
        yarn_beta_slow=1.0,
        yarn_orig_ctx=8192,
    )


_BUILDERS = {
    "tiny": _tiny,
    "small": _small,
    "8b": _8b,
    "72b": _72b,
}


def get_preset(name: str) -> ModelConfig:
    """Geometry for a named preset.

    Args:
        name: one of :func:`list_presets`. Case-insensitive, and ``"72b"``/``"72B"`` and
            ``"8b"``/``"8B"`` both work because typing the wrong case should not be an error.
    """
    key = name.strip().lower()
    if key not in _BUILDERS:
        raise KeyError(f"unknown preset {name!r}; available: {list_presets()}")
    return _BUILDERS[key]()


def list_presets() -> List[str]:
    return list(_BUILDERS)


def affect_for(preset: str, **overrides) -> AffectConfig:
    """Affect config appropriate to a preset.

    Every preset gets the same affective config, because the affective block is not what
    scales with model size: it is three state blocks either way. Overriding ``use_moral``
    here is the deliberate act described in :class:`AffectConfig`.
    """
    get_preset(preset)  # validate the name
    return AffectConfig(**overrides)
