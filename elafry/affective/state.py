"""The affective state: VAD vector, Markovian transition, Plutchik mapping.

The state is a 3D vector in ``[-1, 1]^3`` (valence, arousal, dominance) and it is a *prior*
on generation, not a metric logged after it. That distinction is the whole argument of the
architecture, so it is worth being literal about what "prior" requires:

* The transition runs **before** the next token is produced, never after.
* The state carries across turns with inertia, so the model has a disposition when the user
  says something new rather than reacting to each turn in isolation.
* The state is differentiable and lives inside the autograd graph, so gradient descent can
  shape how it moves. A state tracked in Python floats outside the model cannot be trained.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Dict, List, Optional, Tuple

import torch
from torch import Tensor

__all__ = [
    "PLUTCHIK_PRIMARIES",
    "PLUTCHIK_DYADS",
    "PLUTCHIK_VAD",
    "STATE_FEATURE_NAMES",
    "AffectiveState",
    "state_feature_dim",
    "build_state_features",
    "plutchik_tensor",
    "classify_plutchik",
    "clamp_state",
    "transition",
    "initial_state",
    "all_primary_states",
    "intensity_ladder",
]

# Ordered so a feature vector built by ``build_state_features`` lines up with a model built
# for ``state_feature_dim()`` given the same channel flags. The first three are always
# present; the rest are opt-in and zero-filled when a caller supplies nothing.
#
# These live here rather than in ``affect.py`` because ``config.base`` needs them too, and
# ``affect`` already depends on ``config``. Putting them in a module that depends on neither
# keeps the import graph a tree.
STATE_FEATURE_NAMES: Tuple[str, ...] = (
    "valence",
    "arousal",
    "dominance",
    "intent",
    "arousal_velocity",
    "crisis",
)


def state_feature_dim(
    include_intent: bool = False,
    include_velocity: bool = False,
    include_crisis: bool = False,
) -> int:
    """Width of the state feature vector for a given set of channels.

    Defaults to 3: bare VAD, which is exactly the ``S_t in [-1,1]^3`` the architecture note
    specifies. Optional channels widen it when a caller genuinely has that signal.
    """
    n = 3
    if include_intent:
        n += 1
    if include_velocity:
        n += 1
    if include_crisis:
        n += 1
    return n


def _as_column(x: Tensor, bsz: int) -> Tensor:
    """Reduce to ``(B, 1)`` by mean over trailing dims."""
    if x.dim() == 0:
        return x.reshape(1, 1).expand(bsz, 1)
    if x.dim() == 1:
        return x.unsqueeze(-1)
    return x.reshape(bsz, -1).mean(dim=-1, keepdim=True)


def build_state_features(
    state: Tensor,
    intent: Optional[Tensor] = None,
    arousal_velocity: Optional[Tensor] = None,
    crisis: Optional[Tensor] = None,
    include_intent: bool = False,
    include_velocity: bool = False,
    include_crisis: bool = False,
) -> Tensor:
    """Concatenate VAD with optional channels into one feature vector per batch item.

    The output width is always exactly
    :func:`state_feature_dim`(include_intent, include_velocity, include_crisis), zero-filling
    any channel the caller did not supply. Sizing the vector by what happened to be passed
    in rather than by what the model was built for is how a state tensor ends up three wide
    going into a bias that expects four, with the error surfacing several layers later.
    """
    if state.dim() != 2 or state.shape[-1] != 3:
        raise ValueError(f"state must be (B, 3), got {tuple(state.shape)}")
    bsz = state.shape[0]

    parts: List[Tensor] = [state]
    for value, enabled in (
        (intent, include_intent),
        (arousal_velocity, include_velocity),
        (crisis, include_crisis),
    ):
        if not enabled:
            continue
        parts.append(state.new_zeros(bsz, 1) if value is None else _as_column(value, bsz))

    out = torch.cat(parts, dim=-1)
    expected = state_feature_dim(include_intent, include_velocity, include_crisis)
    if out.shape[-1] != expected:
        raise RuntimeError(
            f"built {out.shape[-1]} state features but state_feature_dim says {expected}"
        )
    return out

# Plutchik's eight primaries, with VAD anchors. Values are authored here rather than
# inherited from anywhere else in the repository, because the two HTML visualisations that
# exist disagree with each other (one lists 9 reference centroids, the other 19 finer
# states). This table is the single source of truth for Elafry.
PLUTCHIK_PRIMARIES: Tuple[str, ...] = (
    "joy",
    "trust",
    "fear",
    "surprise",
    "sadness",
    "disgust",
    "anger",
    "anticipation",
)

PLUTCHIK_DYADS: Tuple[str, ...] = (
    "lovey",
    "optimism",
    "submission",
    "amazement",
    "pessimism",
    "disappointment",
    "rage",
    "vigilance",
)

# Anchors in [-1, 1]^3: (valence, arousal, dominance).
#
# Order matches PLUTCHIK_PRIMARIES. Dominance is the axis that separates anger from fear
# (both high-arousal negative-valence, but anger dominates and fear submits), so it is what
# carries the distinction between pushing back and caving.
PLUTCHIK_VAD: Dict[str, Tuple[float, float, float]] = {
    "joy": (0.85, 0.55, 0.60),
    "trust": (0.70, -0.20, 0.35),
    "fear": (-0.70, 0.65, -0.65),
    "surprise": (0.10, 0.90, -0.10),
    "sadness": (-0.80, -0.45, -0.60),
    "disgust": (-0.70, 0.20, 0.15),
    "anger": (-0.65, 0.75, 0.60),
    "anticipation": (0.20, 0.35, 0.45),
}


def plutchik_tensor(device=None, dtype=torch.float32) -> Tensor:
    """``(8, 3)`` tensor of the primary anchors, in ``PLUTCHIK_PRIMARIES`` order."""
    rows = [PLUTCHIK_VAD[name] for name in PLUTCHIK_PRIMARIES]
    return torch.tensor(rows, device=device, dtype=dtype)


def initial_state(batch_size: int = 1, device=None, dtype=torch.float32) -> Tensor:
    """The neutral origin. Starting at exactly zero means no emotional prior."""
    return torch.zeros(batch_size, 3, device=device, dtype=dtype)


def clamp_state(state: Tensor, bound: float = 1.0) -> Tensor:
    """Hold VAD inside [-1, 1]^3. Cheap, and keeps cosine similarity well-conditioned."""
    return state.clamp(-bound, bound)


@dataclass
class AffectiveState:
    """A VAD vector plus whatever the caller wants to track alongside it.

    The dataclass is for inspection and logging. The model path uses raw tensors, because
    a Python object per token per step is a lot of allocation for a value the backward pass
    never sees.
    """

    valence: float = 0.0
    arousal: float = 0.0
    dominance: float = 0.0

    def __post_init__(self) -> None:
        self.valence = max(-1.0, min(1.0, float(self.valence)))
        self.arousal = max(-1.0, min(1.0, float(self.arousal)))
        self.dominance = max(-1.0, min(1.0, float(self.dominance)))

    @property
    def vector(self) -> Tuple[float, float, float]:
        return (self.valence, self.arousal, self.dominance)

    @classmethod
    def from_tensor(cls, t: Tensor) -> "AffectiveState":
        flat = t.detach().flatten().tolist()
        if len(flat) != 3:
            raise ValueError(f"expected 3 components, got {len(flat)}")
        return cls(*flat)

    def similarity(self, other: "AffectiveState") -> float:
        """Cosine similarity to another state. Zero-magnitude vectors score 0."""
        import math

        a, b = self.vector, other.vector
        dot = sum(x * y for x, y in zip(a, b))
        na = math.sqrt(sum(x * x for x in a))
        nb = math.sqrt(sum(x * x for x in b))
        if na == 0.0 or nb == 0.0:
            return 0.0
        return dot / (na * nb)

    def to_dict(self) -> Dict[str, float]:
        return {
            "valence": self.valence,
            "arousal": self.arousal,
            "dominance": self.dominance,
        }


def classify_plutchik(state: Tensor, threshold: float = 0.72) -> Tuple[Optional[str], float]:
    """Nearest primary by cosine similarity.

    Args:
        state: ``(3,)`` or ``(1, 3)``.
        threshold: minimum cosine similarity to accept a label. Below it the state is
            genuinely ambiguous and the honest answer is ``None``, which the caller should
            treat as "hold the previous disposition" rather than snapping to the nearest
            centroid. Forcing a label here is how a neutral state turns into a mood.
    Returns:
        ``(label, similarity)``; ``label`` is ``None`` when below threshold.
    """
    if state.dim() == 2:
        if state.shape[0] != 1:
            raise ValueError("classify_plutchik expects a single state vector")
        state = state[0]
    if state.shape[-1] != 3:
        raise ValueError(f"state must have 3 components, got {state.shape[-1]}")

    anchors = plutchik_tensor(device=state.device, dtype=state.dtype)
    norm = torch.linalg.norm(state)
    if float(norm) == 0.0:
        return None, 0.0
    sims = torch.nn.functional.cosine_similarity(
        state.unsqueeze(0), anchors, dim=-1
    )  # (8,)
    best_idx = int(torch.argmax(sims))
    best_sim = float(sims[best_idx])
    if best_sim < threshold:
        return None, best_sim
    return PLUTCHIK_PRIMARIES[best_idx], best_sim


def transition(
    prev: Tensor,
    drive: Tensor,
    inertia: float = 0.4,
    clamp: float = 1.0,
) -> Tensor:
    """The state transition, ``S_t = alpha * S_{t-1} + (1 - alpha) * MLP_affect(U_t)``.

    Args:
        prev: ``(B, 3)`` previous state. On the first turn pass zeros.
        drive: ``(B, 3)`` what the current user turn pushes the state toward, already
            computed by an encoder. Any magnitude; the convex combination plus the clamp
            keeps the result in range.
        inertia: alpha. High values make the model slow to change mood, which reads as
            composure. ``1.0`` freezes the state entirely.
    Returns:
        ``(B, 3)`` clamped next state.
    """
    if not 0.0 <= inertia <= 1.0:
        raise ValueError(f"inertia must be in [0, 1], got {inertia}")
    if prev.shape != drive.shape:
        raise ValueError(f"shape mismatch: prev {tuple(prev.shape)} vs drive {tuple(drive.shape)}")
    return clamp_state(inertia * prev + (1.0 - inertia) * drive, bound=clamp)


def all_primary_states() -> Dict[str, Tensor]:
    """Every primary as a ``(1, 3)`` tensor. The ablation harness pokes these in by hand."""
    return {name: torch.tensor([PLUTCHIK_VAD[name]]) for name in PLUTCHIK_PRIMARIES}


def intensity_ladder(base: str, levels: Optional[List[str]] = None) -> Dict[str, Tensor]:
    """Scale a primary toward its dyad partner to build an intensity ladder.

    The guide notes this as an escalation guard: moving from *annoyance* to *rage* is the
    same direction as *anger* to *disgust*, one step further out. Returns
    ``{label: (1,3) state}`` for three rungs.
    """
    if base not in PLUTCHIK_VAD:
        raise KeyError(f"unknown primary {base!r}; expected one of {PLUTCHIK_PRIMARIES}")
    ladder_labels = levels or [f"{base}_low", f"{base}_mid", f"{base}_high"]
    base_v = torch.tensor([PLUTCHIK_VAD[base]])
    # Push outward from the origin for higher rungs, which increases arousal and magnitude
    # without changing which primary it is nearest to.
    out: Dict[str, Tensor] = {}
    for i, label in enumerate(ladder_labels):
        scale = 0.6 + 0.2 * i
        out[label] = clamp_state(base_v * scale)
    return out