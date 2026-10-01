"""Configuration dataclasses for Elafry.

Everything the model, the affective subsystem and the training loop need to be described
by. These are plain dataclasses with ``to_dict`` / ``from_dict`` so a config can ride
alongside a safetensors file as a ``config.json`` sidecar. The inference engine in
``elafry_rs`` reads that sidecar, which is the whole point: no hardcoded geometry.
"""

from __future__ import annotations

import json
from dataclasses import asdict, dataclass, field, fields
from pathlib import Path
from typing import Any, Dict, Literal, Optional, Tuple

from elafry.affective.state import build_state_features, state_feature_dim

StateSource = Literal["internal", "external", "fixed"]
BiasGranularity = Literal["head", "kv"]


@dataclass
class ModelConfig:
    """Decoder geometry. Mirrors ``elafry_rs`` exactly, so keep the field names stable."""

    vocab_size: int = 32000
    dim: int = 1024
    n_layers: int = 24
    n_heads: int = 16
    n_kv_heads: int = 8
    head_dim: int = 64
    ff_dim: int = 2816
    rope_theta: float = 500_000.0
    max_seq_len: int = 4096
    rms_eps: float = 1e-5
    tie_embeddings: bool = True

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

    def to_dict(self) -> Dict[str, Any]:
        return asdict(self)

    @classmethod
    def from_dict(cls, d: Dict[str, Any]) -> "ModelConfig":
        known = {f.name for f in fields(cls)}
        return cls(**{k: v for k, v in d.items() if k in known})

    @classmethod
    def from_json(cls, path: Path) -> "ModelConfig":
        return cls.from_dict(json.loads(Path(path).read_text(encoding="utf-8")))


@dataclass
class AffectConfig:
    """The affective subsystem.

    The two knobs that actually change model behaviour are ``inertia`` (alpha in the
    transition equation) and ``bias_granularity``. Everything else is bookkeeping.

    ``bias_granularity="head"`` gives every query head its own bias row, which is the
    faithful reading of the architecture note: emotion reweights what each head attends
    to. ``"kv"`` gives one bias row per KV group, broadcast across the group, and costs
    ``n_rep`` times less memory. Default is ``"head"`` because the point of the mechanism
    is per-head selectivity, and 12GB of VRAM is not the binding constraint yet.
    """

    enabled: bool = True
    state_source: StateSource = "internal"
    bias_granularity: BiasGranularity = "head"

    # S_t = alpha * S_{t-1} + (1 - alpha) * MLP_affect(U_t)
    inertia: float = 0.4

    # Plutchik cosine-similarity floor for accepting a discrete label.
    plutchik_threshold: float = 0.72

    # Where S_t comes from when state_source is "external": hidden size of the encoder.
    external_encoder_dim: int = 256

    # Injection points, as fractions of n_layers. Late-layer-only injection lets early
    # layers stay emotional-neutral, which trains faster; "all" is the faithful reading.
    inject_layers: Tuple[int, ...] = field(default_factory=lambda: ())

    # Clamp on S_t after every update. [-1, 1] is the VAD convention.
    clamp: float = 1.0

    # Which channels the state feature vector carries, beyond bare VAD. The vector's width
    # is fixed by these three flags and has to match on the model side and the bias side.
    # Changing one without the other is the shape mismatch described in
    # ``elafry.models.affect.build_state_features``.
    #
    # All three default off, so the state is exactly the S_t in [-1,1]^3 that the
    # architecture note specifies. Intent reaches generation through the router's mode
    # selection and through ``IntentRouter.valence_pull``, which is where it belongs: an
    # intent channel that is zero on every step is a dead channel that only costs width.
    use_intent: bool = False
    use_velocity: bool = False
    use_crisis: bool = False

    @property
    def state_feature_dim(self) -> int:
        """Width of the state vector fed to the attention bias."""
        return state_feature_dim(
            include_intent=self.use_intent,
            include_velocity=self.use_velocity,
            include_crisis=self.use_crisis,
        )

    def build_features(
        self,
        state,
        intent=None,
        arousal_velocity=None,
        crisis=None,
    ):
        """Assemble the state feature vector, sized by this config."""
        return build_state_features(
            state,
            intent=intent,
            arousal_velocity=arousal_velocity,
            crisis=crisis,
            include_intent=self.use_intent,
            include_velocity=self.use_velocity,
            include_crisis=self.use_crisis,
        )

    def layers_to_inject(self, n_layers: int) -> Tuple[int, ...]:
        """Resolve ``inject_layers`` against a concrete depth.

        Empty means every layer. Negative indices count back from the end, so ``(-1,)``
        is just the final layer.
        """
        if not self.inject_layers:
            return tuple(range(n_layers))
        resolved = []
        for idx in self.inject_layers:
            resolved.append(idx if idx >= 0 else n_layers + idx)
        bad = [i for i in resolved if not 0 <= i < n_layers]
        if bad:
            raise ValueError(f"inject_layers {self.inject_layers} out of range for {n_layers} layers")
        return tuple(sorted(set(resolved)))

    def to_dict(self) -> Dict[str, Any]:
        d = asdict(self)
        d["inject_layers"] = list(self.inject_layers)
        return d

    @classmethod
    def from_dict(cls, d: Dict[str, Any]) -> "AffectConfig":
        known = {f.name for f in fields(cls)}
        kwargs = {k: v for k, v in d.items() if k in known}
        if "inject_layers" in kwargs:
            kwargs["inject_layers"] = tuple(kwargs["inject_layers"])
        return cls(**kwargs)


@dataclass
class TrainConfig:
    """Optimisation. Defaults are sized for the 350M preset on one RTX 3060 12GB."""

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

    # PG-RL. The reward model scores on Bradley-Terry plus two auxiliary heads; the
    # affective weight is the auxiliary term and the sycophancy weight is what makes
    # the model stop agreeing with users.
    reward_affect_weight: float = 0.3
    reward_sycophancy_weight: float = 0.5
    dpo_beta: float = 0.1

    def to_dict(self) -> Dict[str, Any]:
        d = asdict(self)
        d["betas"] = list(self.betas)
        return d

    @classmethod
    def from_dict(cls, d: Dict[str, Any]) -> "TrainConfig":
        known = {f.name for f in fields(cls)}
        kwargs = {k: v for k, v in d.items() if k in known}
        if "betas" in kwargs:
            kwargs["betas"] = tuple(kwargs["betas"])
        return cls(**kwargs)