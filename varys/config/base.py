"""Configuration for Varys.

Same shape as Elafry's: plain dataclasses with ``to_dict`` / ``from_dict`` so a config can
ride beside a safetensors file as a ``config.json`` sidecar. That sidecar is the point. The
inference engine reads it rather than hardcoding geometry, which is the only reason a
72B model can be served by a Rust process that never imports torch.

## Why this module imports no torch

``AffectConfig.state_feature_dim`` calls into ``varys.affective.state``, which is torch-free
precisely so this file can be. ``varys.models`` needs the config, and the config needs the
width contract. If the width lived next to the encoders, importing a config would drag in the
tensor stack and the import graph would stop being a tree.

## The one place Varys's config diverges from Elafry's

Elafry's ``AffectConfig`` has three channel flags and a state that is exactly ``S_t in
[-1,1]^3``. Varys's state is 21 dimensions in three blocks with different ranges, different
decay rates and different safety properties, so the flags are per-block and each block gets
its own bias scale. See :class:`AffectConfig` for why the moral block is off by default.
"""

from __future__ import annotations

import json
from dataclasses import asdict, dataclass, field
from pathlib import Path
from typing import Any, Dict, Literal, Optional, Tuple

from varys.affective.state import (
    MORAL_DIMS,
    SPECTRA_DIMS,
    VAD_AXES,
    state_feature_dim,
)

StateSource = Literal["internal", "external", "fixed"]
BiasGranularity = Literal["head", "kv"]
RopeScaling = Literal["none", "linear", "ntk", "yarn"]

__all__ = [
    "ModelConfig",
    "AffectConfig",
    "TrainConfig",
    "StateSource",
    "BiasGranularity",
    "RopeScaling",
]


@dataclass
class ModelConfig:
    """Decoder geometry.

    Field names are load-bearing for the ``varys_rs`` inference engine, which reads them out
    of the sidecar. Renaming one is a breaking change to every published checkpoint, so treat
    them as a wire format.

    Defaults are the ``tiny`` preset's, sized to run its tests on a 12GB card. The 72B
    geometry lives in :mod:`varys.config.presets`.
    """

    vocab_size: int = 32000
    dim: int = 256
    n_layers: int = 4
    n_heads: int = 4
    n_kv_heads: int = 2
    head_dim: int = 64
    ff_dim: int = 704
    rope_theta: float = 500_000.0
    max_seq_len: int = 4096
    rms_eps: float = 1e-5
    tie_embeddings: bool = True

    # --- long context -------------------------------------------------------
    # Varys is specified for longitudinal work, and longitudinal material does not fit in
    # 4k. These four fields extend the context without retraining from scratch, and
    # ``rope_scaling != "none"`` is a real change to the geometry a model was trained under,
    # so it is opt-in and recorded in the sidecar. See ``varys.models.rope``.
    rope_scaling: RopeScaling = "none"
    rope_factor: float = 1.0
    yarn_beta_fast: float = 32.0
    yarn_beta_slow: float = 1.0
    yarn_orig_ctx: int = 4096

    def __post_init__(self) -> None:
        if self.dim % self.n_heads != 0:
            raise ValueError(
                f"dim {self.dim} is not divisible by n_heads {self.n_heads}; "
                "pick a head_dim that divides dim"
            )
        if self.n_heads % self.n_kv_heads != 0:
            raise ValueError(
                f"n_heads {self.n_heads} is not a multiple of n_kv_heads {self.n_kv_heads}; "
                "grouped-query attention needs whole groups"
            )
        if self.rope_theta <= 0:
            raise ValueError("rope_theta must be positive")
        if self.rope_scaling not in ("none", "linear", "ntk", "yarn"):
            raise ValueError(f"unknown rope_scaling {self.rope_scaling!r}")
        if self.rope_scaling != "none" and self.rope_factor < 1.0:
            raise ValueError(
                f"rope_factor must be >= 1.0, got {self.rope_factor}; a factor below 1 "
                "compresses the context rather than extending it"
            )
        if self.yarn_orig_ctx <= 0:
            raise ValueError("yarn_orig_ctx must be positive")

    @property
    def n_rep(self) -> int:
        """Query heads per KV head. ``n_rep`` is what turns GQA into plain MHA."""
        return self.n_heads // self.n_kv_heads

    @property
    def q_dim(self) -> int:
        return self.n_heads * self.head_dim

    @property
    def kv_dim(self) -> int:
        return self.n_kv_heads * self.head_dim

    @property
    def effective_theta(self) -> float:
        """RoPE base after NTK-aware adjustment.

        NTK scaling is not a post-hoc interpolation of the rotation angles. It raises the
        base so the high-frequency dimensions, which are never trained on long-range structure
        anyway, are nudged toward lower frequency and the model can express distance it never
        saw. That is why it is folded into the base rather than applied to the angles.
        """
        if self.rope_scaling == "ntk":
            return self.rope_theta * (self.rope_factor ** (self.head_dim / (self.head_dim - 2)))
        return self.rope_theta

    def to_dict(self) -> Dict[str, Any]:
        return asdict(self)

    @classmethod
    def from_dict(cls, d: Dict[str, Any]) -> "ModelConfig":
        known = {f for f in cls.__dataclass_fields__}  # type: ignore[attr-defined]
        return cls(**{k: v for k, v in d.items() if k in known})

    @classmethod
    def from_json(cls, path: Path) -> "ModelConfig":
        return cls.from_dict(json.loads(Path(path).read_text(encoding="utf-8")))


@dataclass
class AffectConfig:
    """The affective subsystem.

    The state is 21 dimensions in three blocks, and the central decision here is that the
    blocks do not all get to influence attention equally.

    ## Per-block bias scales

    ``AffectiveBias`` keeps one learnable scalar per block and multiplies each block's
    contribution by it. A single wide projection over all 21 dimensions would be simpler and
    would not let anyone answer "how much is clinical state actually changing attention",
    which is a question the eval suite has to ask and the ablation has to measure. Separate
    projections plus separate scales make each block's influence independently readable and
    independently ablatable, at the cost of a per-layer ``d*d`` matrix per block instead of
    one shared.

    The initial values encode a position, not a tuning result:

    * ``vad`` starts at 1.0. Affect shaping attention is the mechanism the architecture is
      for, and Elafry trains it.
    * ``spectra`` starts at 0.1. Clinical state should change *how* the model responds, not
      *which tokens it attends to*. Attending differently to someone because the model
      inferred a spectrum from their text is a discrimination the model has no standing to
      make. Starting low and letting training raise it means the scale has to earn the
      influence.
    * ``moral`` starts absent. See below.

    These are initialisation values, not caps. Nothing stops training from raising a scale,
    which is why the moral case is handled by absence rather than by a small number.

    ## Why the moral block is off

    Conditioning attention on a person's moral foundations is a persuasion surface. The
    attack is not telling someone what they want, it is manufacturing the affect that makes
    their own values push toward a desired outcome, and a model with a live read on
    ``liberty_care`` and ``sanctity_care`` can do that more cheaply than it can argue.

    The cost of the block is 12 dimensions, which is 0.7% of a 72B model. It is not a
    capacity question, so it is not argued on capacity. It is off because enabling it is a
    decision somebody has to make deliberately, having read
    :func:`varys.affective.moral.is_manipulation_risk`, rather than a default somebody
    inherits.

    ``moral_bias_scale`` exists so that if it *is* enabled, the reward model has a scale to
    hold it down with.
    """

    enabled: bool = True
    state_source: StateSource = "internal"
    bias_granularity: BiasGranularity = "head"

    # Which blocks reach the attention bias at all.
    use_spectra: bool = True
    use_moral: bool = False
    use_velocity: bool = False
    use_intent: bool = False

    # Initial per-block bias scales, as described above. Learnable, not fixed.
    vad_bias_scale: float = 1.0
    spectra_bias_scale: float = 0.1
    moral_bias_scale: float = 0.0

    # Decay rates. Duplicated from varys.affective.state on purpose: a checkpoint has to
    # carry the dynamics it was trained with, and reading them from a module constant would
    # mean changing that constant silently changes the behaviour of every old checkpoint.
    vad_decay: float = 0.70
    spectra_decay: float = 0.94
    moral_decay: float = 0.98
    spectra_baseline: float = 0.05
    drive_gain: float = 1.0

    # Evidence-gate thresholds. The clinical block does not move on a single mention.
    gate_min_confidence: float = 0.5
    gate_decay: float = 0.5
    gate_clinical: bool = True

    # Width of the external affect encoder, when state_source is "external".
    external_encoder_dim: int = 256

    # Injection points as fractions of depth. Empty means every layer. Negative indices
    # count back from the end, so (-1,) is the final layer.
    inject_layers: Tuple[int, ...] = field(default_factory=tuple)

    # Clamp on the signed VAD block. The other blocks are unsigned and floored instead.
    clamp: float = 1.0

    @property
    def state_feature_dim(self) -> int:
        """Width of the state vector fed to the attention bias."""
        return state_feature_dim(
            include_moral=self.use_moral,
            include_velocity=self.use_velocity,
            include_intent=self.use_intent,
        )

    @property
    def state_dim(self) -> int:
        """Width of the full internal state, always 21 regardless of the bias flags.

        The bias flags change what reaches *attention*, not what the state *is*. A model with
        the moral block off from the bias still tracks moral state, so turning it on later
        does not require having recorded it, and turning it off does not leave a hole.
        """
        return len(VAD_AXES) + len(SPECTRA_DIMS) + len(MORAL_DIMS)

    def block_widths(self) -> Dict[str, int]:
        """Width of each state block, for slicing the 21-dim state."""
        return {
            "vad": len(VAD_AXES),
            "spectra": len(SPECTRA_DIMS),
            "moral": len(MORAL_DIMS),
        }

    def build_features(
        self,
        state,
        velocity=None,
        intent=None,
    ):
        """Assemble the state feature vector, sized by this config.

        Imports here rather than at module scope to keep the module torch-free: the caller
        necessarily has torch, but importing this file should not require it.
        """
        from varys.affective.encoders import build_state_features

        return build_state_features(
            state,
            include_moral=self.use_moral,
            velocity=velocity,
            intent=intent,
            include_velocity=self.use_velocity,
            include_intent=self.use_intent,
        )

    def layers_to_inject(self, n_layers: int) -> Tuple[int, ...]:
        """Resolve ``inject_layers`` against a concrete depth. Empty means every layer."""
        if not self.inject_layers:
            return tuple(range(n_layers))
        resolved = [idx if idx >= 0 else n_layers + idx for idx in self.inject_layers]
        bad = [i for i in resolved if not 0 <= i < n_layers]
        if bad:
            raise ValueError(
                f"inject_layers {self.inject_layers} out of range for {n_layers} layers"
            )
        return tuple(sorted(set(resolved)))

    def to_dict(self) -> Dict[str, Any]:
        d = asdict(self)
        d["inject_layers"] = list(self.inject_layers)
        return d

    @classmethod
    def from_dict(cls, d: Dict[str, Any]) -> "AffectConfig":
        known = {f for f in cls.__dataclass_fields__}  # type: ignore[attr-defined]
        kwargs = {k: v for k, v in d.items() if k in known}
        if "inject_layers" in kwargs:
            kwargs["inject_layers"] = tuple(kwargs["inject_layers"])
        return cls(**kwargs)


@dataclass
class TrainConfig:
    """Optimisation.

    Defaults are sized for the ``tiny`` preset on one RTX 3060 12GB. The 72B campaign does
    not use this file's defaults and is documented in ``SCALING.md``.
    """

    lr: float = 3e-4
    min_lr: float = 3e-5
    weight_decay: float = 0.1
    betas: Tuple[float, float] = (0.9, 0.95)
    grad_clip: float = 1.0
    warmup_steps: int = 200
    max_steps: int = 20_000
    batch_size: int = 8
    grad_accum: int = 1
    log_every: int = 50
    save_every: int = 1000
    seed: int = 42
    amp_dtype: str = "bf16"
    grad_checkpointing: bool = True

    # Campaign-style training: stop on whichever bound bites first.
    token_budget: Optional[int] = None
    max_minutes: Optional[float] = None

    # PG-RL. The reward model scores Bradley-Terry plus auxiliary heads.
    # ``reward_moral_weight`` is the one Varys adds: it penalises reasoning that moves toward
    # a persuasion exploit rather than toward an argument.
    reward_affect_weight: float = 0.3
    reward_sycophancy_weight: float = 0.5
    reward_clinical_weight: float = 0.2
    reward_moral_weight: float = 0.2
    dpo_beta: float = 0.1

    def to_dict(self) -> Dict[str, Any]:
        d = asdict(self)
        d["betas"] = list(self.betas)
        return d

    @classmethod
    def from_dict(cls, d: Dict[str, Any]) -> "TrainConfig":
        known = {f for f in cls.__dataclass_fields__}  # type: ignore[attr-defined]
        kwargs = {k: v for k, v in d.items() if k in known}
        if "betas" in kwargs:
            kwargs["betas"] = tuple(kwargs["betas"])
        return cls(**kwargs)
