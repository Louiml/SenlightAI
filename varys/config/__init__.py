"""Configuration for Varys. Torch-free by design.

    base.py      ModelConfig, AffectConfig, TrainConfig
    presets.py   tiny, small, 8b, 72b geometries

``base`` imports ``varys.affective.state`` for the width contract, which is why that module
has no torch import. The dependency graph is a tree, not a cycle.
"""

from varys.config.base import AffectConfig, ModelConfig, TrainConfig
from varys.config.presets import PRESET_NOTES, affect_for, get_preset, list_presets

__all__ = [
    "ModelConfig",
    "AffectConfig",
    "TrainConfig",
    "get_preset",
    "list_presets",
    "affect_for",
    "PRESET_NOTES",
]
