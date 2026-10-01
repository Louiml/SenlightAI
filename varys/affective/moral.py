"""Moral reasoning axes, on Moral Foundations Theory.

Varys is specified for "complex moral reasoning", and Elafry has no representation of that at
all. VAD carries affect; it cannot carry the fact that one person finds a duty sacred and
another finds it merely optional.

## Why Moral Foundations Theory

The obvious alternative is a single "morality" scalar, and it is useless: it cannot tell a
harm judgment from a fairness judgment, so it cannot represent two people who agree that
something is wrong for entirely different reasons, which is the interesting case.

Graham et al. (2009/2013) propose five foundations, later six, that cut across cultures and
predict moral judgment better than a single scale. The six are used here because the
liberty/oppression and sanctity/degradation pairs were added on comparative evidence and
they matter for exactly the cases a psychological model encounters: bodily autonomy, purity,
and the moral licensing that gets attached to being good.

Each axis is a *tradeoff*, not a virtue. High care and low fairness is a real and common
configuration, and the pair encodes it as something the model can represent rather than
something it has to flatten.

## What these numbers are not

They are not a claim about how moral a person is. A person scoring high on every axis is not
good, they are someone whose judgments are strongly foundation-driven, which is not the same
thing and is sometimes less desirable. The axes describe *which* considerations are active,
not how much anyone should defer to them.

This is the one place in Varys where a model could most easily be weaponised: a profile that
records which foundations someone appeals to is a map of how to manipulate them. The privacy
module treats that as a first-class concern rather than an afterthought.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Dict, Optional, Sequence, Tuple

__all__ = [
    "FOUNDATIONS",
    "FOUNDATION_DIMS",
    "MORAL_DIMS",
    "Foundation",
    "MoralVector",
    "neutral_moral",
    "clamp_moral",
    "polarity",
    "MFT_CITATION",
    "is_manipulation_risk",
]

MFT_CITATION = (
    "Graham, J., Haidt, J., & Nosek, B. A. (2009). Liberal and conservative values: A "
    "moral foundations approach. Psychological Review, 116(1), 77. "
    "Haidt, J., & Graham, J. (2012). Moral foundations theory. In The Social Psychology of "
    "Morality (2nd ed.)."
)


@dataclass(frozen=True)
class Foundation:
    """One pole pair. Both poles are recorded because both are morally real positions."""

    name: str
    care: str
    harm: str
    description: str

    @property
    def poles(self) -> Tuple[str, str]:
        return (self.care, self.harm)


FOUNDATIONS: Dict[str, Foundation] = {
    "care": Foundation(
        name="care",
        care="care",
        harm="harm",
        description=(
            "Suffering and well-being. Warding off harm to people and animals. Drives most "
            "ordinary moral reaction to visible distress."
        ),
    ),
    "fairness": Foundation(
        name="fairness",
        care="fairness",
        harm="cheating",
        description=(
            "Reciprocation and equal treatment. Cheating, free-riding, unequal burdens."
        ),
    ),
    "loyalty": Foundation(
        name="loyalty",
        care="loyalty",
        harm="betrayal",
        description=(
            "Fidelity to in-group and authority. Betrayal, disloyalty, abandonment of one's "
            "own people."
        ),
    ),
    "authority": Foundation(
        name="authority",
        care="authority",
        harm="subversion",
        description=(
            "Legitimate hierarchy and role. Rebellion against authority, disorder, "
            "disrespect for structure."
        ),
    ),
    "liberty": Foundation(
        name="liberty",
        care="liberty",
        harm="oppression",
        description=(
            "Freedom of self-determination. Coercion, restriction of autonomy, being told "
            "what to do. Added after the first five because it predicts independently."
        ),
    ),
    "sanctity": Foundation(
        name="sanctity",
        care="sanctity",
        harm="degradation",
        description=(
            "Purity, dignity, sacredness. Contamination, degradation, bodily and spiritual "
            "disrespect. Also the seat of most disgust-based moral reasoning."
        ),
    ),
}

FOUNDATION_DIMS: Tuple[str, ...] = tuple(FOUNDATIONS)

# The state vector carries both poles of each foundation, so it needs two slots per
# foundation. One signed scalar would work arithmetically but loses the model's ability to
# represent a strong position on *both*, which is precisely the moral licensing case that
# Moral Licensing Theory says is worth detecting.
MORAL_DIMS: Tuple[str, ...] = tuple(
    f"{name}_{pole}" for name, f in FOUNDATIONS.items() for pole in ("care", "harm")
)


def neutral_moral() -> Tuple[float, ...]:
    """All-zero. No foundation is active, which means the model is reasoning from something
    else entirely. Rare, and worth knowing about rather than defaulting silently."""
    return (0.0,) * len(MORAL_DIMS)


def clamp_moral(values: Sequence[float]) -> Tuple[float, ...]:
    return tuple(max(0.0, min(1.0, float(v))) for v in values)


def polarity(vector: Sequence[float]) -> Dict[str, float]:
    """Collapse each pole pair into a signed ``[care, harm]`` reading in ``[-1, 1]``.

    Positive leans toward the care pole, negative toward the harm pole. This is a view for
    logging and for the token vocabulary; the model consumes the raw two-slot form.
    """
    v = clamp_moral(vector)
    out: Dict[str, float] = {}
    for i, name in enumerate(FOUNDATION_DIMS):
        care, harm = v[2 * i], v[2 * i + 1]
        out[name] = care - harm
    return out


class MoralVector:
    """The twelve moral slots, with named access."""

    __slots__ = ("_v",)

    def __init__(self, values: Optional[Sequence[float]] = None):
        if values is None:
            self._v: Tuple[float, ...] = neutral_moral()
        else:
            if len(values) != len(MORAL_DIMS):
                raise ValueError(
                    f"expected {len(MORAL_DIMS)} moral slots {MORAL_DIMS}, got {len(values)}"
                )
            self._v = clamp_moral(values)

    def __getitem__(self, name: str) -> float:
        if name not in MORAL_DIMS:
            raise KeyError(f"unknown moral dimension {name!r}; expected one of {MORAL_DIMS}")
        return self._v[MORAL_DIMS.index(name)]

    def __setitem__(self, name: str, value: float) -> None:
        idx = MORAL_DIMS.index(name)
        v = list(self._v)
        v[idx] = max(0.0, min(1.0, float(value)))
        self._v = tuple(v)

    def __iter__(self):
        return iter(self._v)

    def __len__(self) -> int:
        return len(self._v)

    def __eq__(self, other: object) -> bool:
        if isinstance(other, MoralVector):
            return self._v == other._v
        if isinstance(other, (tuple, list)):
            return self._v == tuple(clamp_moral(other))
        return NotImplemented

    @property
    def vector(self) -> Tuple[float, ...]:
        return self._v

    def polarity(self) -> Dict[str, float]:
        return polarity(self._v)

    def dominant(self, eps: float = 1e-6) -> Dict[str, float]:
        """The strongest pole per foundation, filtered to active ones."""
        out: Dict[str, float] = {}
        for i, name in enumerate(FOUNDATION_DIMS):
            care, harm = self._v[2 * i], self._v[2 * i + 1]
            if max(care, harm) > eps:
                out[name] = max(care, harm)
        return out

    def to_dict(self) -> Dict[str, float]:
        return dict(zip(MORAL_DIMS, self._v))

    @classmethod
    def from_dict(cls, d: Dict[str, float]) -> "MoralVector":
        return cls([d.get(n, 0.0) for n in MORAL_DIMS])


def is_manipulation_risk(
    profile_moral: Sequence[float],
    target_moral: Sequence[float],
    similarity_threshold: float = 0.85,
) -> Dict[str, object]:
    """Flag when a profile's foundations line up closely enough to be exploitable.

    A model that knows which foundations someone appeals to can learn to activate them. That
    is the attack: not telling the person what they want, but manufacturing the affect that
    makes their own values push toward the desired outcome.

    This returns a flag and a rationale for the system to refuse on, not a score to optimise.
    ``similarity_threshold`` is high deliberately. A loose match flags ordinary agreement and
    would train the model to distrust any value alignment at all.
    """
    profile = polarity(profile_moral)
    target = polarity(target_moral)
    overlap: Dict[str, float] = {}
    for name, t in target.items():
        p = profile.get(name, 0.0)
        if t == 0.0:
            continue
        # Both poles leaning the same way is the exploitable configuration.
        if (p > 0) == (t > 0):
            overlap[name] = min(abs(p), abs(t))

    if not overlap:
        return {"risk": False, "foundations": {}, "rationale": "no aligned active foundations"}

    strongest = max(overlap.values())
    flagged = strongest >= similarity_threshold
    rationale = (
        f"foundations {sorted(overlap)} align at {strongest:.2f}; "
        + (
            "close enough that activating the profile's own values would serve an agenda"
            if flagged
            else "below the flag threshold, ordinary value agreement"
        )
    )
    return {"risk": flagged, "foundations": overlap, "rationale": rationale}