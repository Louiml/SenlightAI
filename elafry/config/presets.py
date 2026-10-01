"""The named preset ladder.

One code path covers everything from a 100M model that trains on a laptop to the 8B
target. Presets are data, not code, so adding a rung means adding a dataclass instance.

The 125m and 350m geometries deliberately match the two configurations that already exist
elsewhere in the repository, which makes them directly comparable against those
checkpoints. The 8b geometry is the Llama-3-8B shape, a proven 8B, so the parameter count
lands where the label says it should.
"""

from __future__ import annotations

from typing import Dict, List

from elafry.config.base import AffectConfig, ModelConfig, TrainConfig

__all__ = [
    "PRESETS",
    "get_preset",
    "list_presets",
    "param_count",
]


def _p(
    vocab_size: int,
    dim: int,
    n_layers: int,
    n_heads: int,
    n_kv_heads: int,
    head_dim: int,
    ff_dim: int,
    max_seq_len: int,
    tie_embeddings: bool = True,
) -> ModelConfig:
    return ModelConfig(
        vocab_size=vocab_size,
        dim=dim,
        n_layers=n_layers,
        n_heads=n_heads,
        n_kv_heads=n_kv_heads,
        head_dim=head_dim,
        ff_dim=ff_dim,
        max_seq_len=max_seq_len,
        tie_embeddings=tie_embeddings,
    )


# Preset names carry the real parameter count, not a round marketing number. ``8b`` would
# have been 7.50B with tied embeddings; untying them lands on 8.03B, which is why the
# 128k-vocabulary rungs drop weight tying. Every number here is asserted against
# ``sum(p.numel() for p in model.parameters())`` in tests/test_config.py.
PRESETS: Dict[str, Dict[str, object]] = {
    # 100M. Matches the geometry of the 12-layer/768-dim checkpoint that exists in the
    # parent repo's artifacts/, so results are directly comparable to it.
    "elafry-100m": {
        "model": _p(32000, 768, 12, 12, 4, 64, 2048, 2048),
        "affect": AffectConfig(),
        "train": TrainConfig(lr=6e-4, warmup_steps=200, max_steps=30_000, batch_size=16),
    },
    # 316M. Matches the 24-layer/1024-dim geometry the older code hardcoded, which is
    # what made that engine unable to load its own checkpoint.
    "elafry-316m": {
        "model": _p(32000, 1024, 24, 16, 8, 64, 2816, 4096),
        "affect": AffectConfig(),
        "train": TrainConfig(lr=3e-4, warmup_steps=200, max_steps=20_000, batch_size=8),
    },
    # 1.15B. Full fine-tuning wants more than 12GB; use LoRA or an 8-bit optimizer.
    "elafry-1.1b": {
        "model": _p(32000, 2048, 24, 32, 8, 64, 5632, 4096),
        "affect": AffectConfig(),
        "train": TrainConfig(lr=2e-4, warmup_steps=500, max_steps=40_000, batch_size=4, grad_accum=4),
    },
    # 3.52B. Past single-consumer-GPU full fine-tuning.
    "elafry-3.5b": {
        "model": _p(32000, 3072, 32, 32, 8, 128, 8192, 8192),
        "affect": AffectConfig(),
        "train": TrainConfig(lr=1.5e-4, warmup_steps=1000, max_steps=60_000, batch_size=2, grad_accum=8),
    },
    # The target. 8.03B, Llama-3-8B geometry with untied embeddings.
    "elafry-8b": {
        "model": _p(128256, 4096, 32, 32, 8, 128, 14336, 8192, tie_embeddings=False),
        "affect": AffectConfig(),
        "train": TrainConfig(
            lr=1.5e-4,
            warmup_steps=2000,
            max_steps=120_000,
            batch_size=1,
            grad_accum=32,
            grad_checkpointing=True,
        ),
    },
    # Not a research target. Exists so tests can run the real forward pass in under a
    # second, and so the overfit gate has something cheap to chew on.
    "elafry-tiny": {
        "model": _p(512, 64, 2, 4, 2, 16, 128, 128),
        "affect": AffectConfig(),
        "train": TrainConfig(lr=1e-3, warmup_steps=5, max_steps=200, batch_size=4),
    },
}


def list_presets() -> List[str]:
    return sorted(PRESETS)


def get_preset(name: str) -> Dict[str, object]:
    if name not in PRESETS:
        raise KeyError(f"unknown preset {name!r}; available: {list_presets()}")
    return PRESETS[name]


def param_count(name: str) -> int:
    """Total parameters for a named preset.

    Delegates to the analytic breakdown in ``elafry.models.elafry``, which is derived from
    the same field names the module construction reads. ``tests/test_config.py`` asserts
    this analytic number equals ``sum(p.numel() for p in model.parameters())``, so the
    two cannot silently drift apart.
    """
    from elafry.models.elafry import param_breakdown

    preset = get_preset(name)
    return sum(param_breakdown(preset["model"], preset["affect"]).values())  # type: ignore[arg-type]