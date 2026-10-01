"""VAD: the three-dimensional affective space.

Carried over from Elafry unchanged, because it is the right primitive and there was no
reason to improve on it. Valence, arousal and dominance in ``[-1, 1]^3``.

Varys does not replace this, it adds axes alongside it. The VAD cube remains the substrate
that Plutchik's wheel is defined against, and it remains what the affective bias is
conditioned on. What changes is that a 72B model is asked to do things Elafry was not: hold
a longitudinal profile of a person, dissect psychopathology, and reason about moral
dilemmas. None of that fits in three numbers, so the state vector grows and VAD stays as
its first three components.

See :mod:`varys.affective.state` for how the axes are composed into one vector.
"""

from __future__ import annotations

import math
from dataclasses import dataclass
from typing import Dict, Iterable, Optional, Tuple

__all__ = [
    "VAD",
    "VAD_DIMS",
    "VAD_AXES",
    "VAD_SPACE",
    "neutral_vad",
    "clamp_vad",
    "vad_distance",
    "vad_similarity",
    "classify_vad",
]

VAD_DIMS = 3
VAD_AXES: Tuple[str, ...] = ("valence", "arousal", "dominance")

# The conventional poles. Kept as data rather than prose because the axes are referenced
# numerically in the Plutchik anchors and in the training targets.
VAD_SPACE: Dict[str, Tuple[float, float]] = {
    "valence": (-1.0, 1.0),   # unpleasant -> pleasant
    "arousal": (-1.0, 1.0),   # calm -> excited
    "dominance": (-1.0, 1.0), # submissive -> in control
}


def neutral_vad() -> Tuple[float, float, float]:
    """The origin. A model starting here has no emotional prior, which is the correct default
    and also the reason a freshly constructed model is completely insensitive to state."""
    return (0.0, 0.0, 0.0)


def clamp_vad(vad: Iterable[float], bound: float = 1.0) -> Tuple[float, float, float]:
    return tuple(max(-bound, min(bound, float(v))) for v in vad)  # type: ignore[return-value]


def _norm(v: Tuple[float, float, float]) -> float:
    return math.sqrt(sum(c * c for c in v))


def vad_similarity(a: Iterable[float], b: Iterable[float]) -> float:
    """Cosine similarity in the VAD cube. Zero-magnitude vectors score 0 rather than NaN,
    which is the one case that matters here: a neutral state is common and must not
    poison an average."""
    x, y = clamp_vad(a), clamp_vad(b)
    nx, ny = _norm(x), _norm(y)
    if nx == 0.0 or ny == 0.0:
        return 0.0
    return sum(p * q for p, q in zip(x, y)) / (nx * ny)


def vad_distance(a: Iterable[float], b: Iterable[float]) -> float:
    """Euclidean distance in the cube. In [0, 2*sqrt(3)]."""
    x, y = clamp_vad(a), clamp_vad(b)
    return math.sqrt(sum((p - q) ** 2 for p, q in zip(x, y)))


@dataclass(frozen=True)
class VAD:
    """A named VAD triple, for readability at call sites."""

    valence: float = 0.0
    arousal: float = 0.0
    dominance: float = 0.0

    def __post_init__(self) -> None:
        object.__setattr__(self, "valence", max(-1.0, min(1.0, float(self.valence))))
        object.__setattr__(self, "arousal", max(-1.0, min(1.0, float(self.arousal))))
        object.__setattr__(self, "dominance", max(-1.0, min(1.0, float(self.dominance))))

    @property
    def vector(self) -> Tuple[float, float, float]:
        return (self.valence, self.arousal, self.dominance)

    @property
    def magnitude(self) -> float:
        return _norm(self.vector)

    def similarity(self, other: "VAD") -> float:
        return vad_similarity(self.vector, other.vector)

    def distance(self, other: "VAD") -> float:
        return vad_distance(self.vector, other.vector)

    def to_dict(self) -> Dict[str, float]:
        return {
            "valence": self.valence,
            "arousal": self.arousal,
            "dominance": self.dominance,
        }

    @classmethod
    def from_vector(cls, v: Iterable[float]) -> "VAD":
        x = list(v)
        if len(x) != VAD_DIMS:
            raise ValueError(f"VAD needs {VAD_DIMS} components, got {len(x)}")
        return cls(*x)


def classify_vad(vad: Iterable[float], anchors: Dict[str, Iterable[float]], threshold: float = 0.72):
    """Nearest named anchor by cosine similarity, or ``None`` below ``threshold``.

    Returns ``(label, similarity)``. ``None`` means genuinely ambiguous, and the honest
    response to that is to hold the previous disposition rather than snap to the nearest
    centroid. A neutral state has no label at all.
    """
    v = clamp_vad(vad)
    if _norm(v) == 0.0:
        return None, 0.0

    best_name: Optional[str] = None
    best_sim = -1.0
    for name, anchor in anchors.items():
        sim = vad_similarity(v, anchor)
        if sim > best_sim:
            best_name, best_sim = name, sim
    if best_sim < threshold:
        return None, best_sim
    return best_name, best_sim