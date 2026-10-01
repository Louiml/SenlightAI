"""Control tokens: the affective vocabulary.

## Why not ``<|vad_-0.6_0.5_0.2|>``

The architecture note shows a single token carrying a whole VAD triple. That does not scale.
VAD is continuous, so a per-turn token has unbounded cardinality, and the vocabulary would
have to be open-ended to cover it. A vocabulary is fixed at training time; you cannot add a
token later without retraining the embedding matrix.

The fix is the same one that makes ``SiLU`` work instead of a lookup table: quantise the
value and enumerate the levels. Nine levels per axis over three axes is 27 tokens that cover
the cube at 0.25 resolution, which is finer than the distinction Plutchik's own primary
anchors make.

## What the tokens are for

The model's *own* state reaches it through the attention bias, not through the context
window, so it does not need to read its own state off the tokens. These tokens annotate the
*user's* turn: they carry a parse of what was said, so downstream layers can attend to the
subtext without re-deriving it. On the response side the relevant markers are the boundary
and stance tokens, which tell the model what posture it is currently holding.

That split is why the vocabulary is under a hundred entries against a 32k to 128k base
vocabulary. It is signal, not padding.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Dict, List, Optional, Tuple

from elafry.affective.state import PLUTCHIK_DYADS, PLUTCHIK_PRIMARIES
from elafry.models.router import INTENTS

__all__ = [
    "CONTROL_TOKENS",
    "ControlToken",
    "control_token",
    "pad_id",
    "bos_id",
    "eos_id",
    "unk_id",
    "affect_reset_id",
    "boundary_ids",
    "vad_token",
    "vad_token_id",
    "VAD_LEVELS",
    "VOCAB_BUDGET",
    "describe",
    "assert_budget",
    "is_control",
    "token_id",
    "token_string",
    "control_token",
    "vad_token_id",
]


# --------------------------------------------------------------------- VAD levels

# Nine levels per axis: -1.0 to 1.0 in steps of 0.25. Finer than the resolution at which
# Plutchik's primary anchors differ, so quantisation is not the limiting factor anywhere.
VAD_LEVELS: Tuple[float, ...] = (-1.0, -0.75, -0.5, -0.25, 0.0, 0.25, 0.5, 0.75, 1.0)

# Intensity rungs for the emotion markers. Three is enough to express "this is drifting":
# the escalation ladder in the research notes (annoyance -> anger -> rage) is the same idea.
INTENSITIES: Tuple[str, ...] = ("low", "mid", "high")

# Quantisation step, for snapping a float to the nearest level.
_VAD_STEP = 0.25


def _vad_level_token(axis: str, value: float) -> str:
    """Nearest VAD level for ``value`` on ``axis``, as a control token string."""
    clamped = max(-1.0, min(1.0, float(value)))
    steps = round(clamped / _VAD_STEP)
    level = VAD_LEVELS[int(steps) + len(VAD_LEVELS) // 2]
    # Sign is part of the token so the model never has to infer polarity from magnitude.
    if level > 0:
        return f"<|vad_{axis}_p{abs(level):.2f}|>"
    if level < 0:
        return f"<|vad_{axis}_m{abs(level):.2f}|>"
    return f"<|vad_{axis}_z|>"


def vad_token(valence: float, arousal: float, dominance: float) -> List[str]:
    """Three tokens describing a VAD triple, one per axis."""
    return [
        _vad_level_token("v", valence),
        _vad_level_token("a", arousal),
        _vad_level_token("d", dominance),
    ]


def _build_vocabulary() -> Tuple[str, ...]:
    tokens: List[str] = []

    # Structural specials first, so their ids are stable and low.
    tokens += ["<|pad|>", "<|bos|>", "<|eos|>", "<|unk|>"]

    # Plutchik primaries at three intensities: 24
    for primary in PLUTCHIK_PRIMARIES:
        for intensity in INTENSITIES:
            tokens.append(f"<|emo_{primary}_{intensity}|>")

    # Plutchik dyads, which is where an escalation lands: 8
    for dyad in PLUTCHIK_DYADS:
        tokens.append(f"<|emo2_{dyad}|>")

    # Intents: 7
    for intent in INTENTS:
        tokens.append(f"<|intent_{intent}|>")

    # User-side VAD, three axes at nine levels: 27
    for axis in ("v", "a", "d"):
        for level in VAD_LEVELS:
            tokens.append(_vad_level_token(axis, level))

    # Boundary and stance. This is what the response side carries, and it is the vocabulary
    # that has to exist for "the model can refuse" to be more than a aspiration: a boundary
    # the model can see the shape of is a boundary it can hold.
    tokens += [
        "<|boundary_set|>",
        "<|boundary_hold|>",
        "<|boundary_release|>",
        "<|stance_warm|>",
        "<|stance_firm|>",
        "<|stance_detached|>",
        "<|stance_deescalating|>",
        "<|stance_refusing|>",
    ]

    # Framing controls.
    tokens += [
        "<|affect_begin|>",
        "<|affect_end|>",
        "<|affect_reset|>",
        "<|user|>",
        "<|assistant|>",
    ]

    if len(tokens) != len(set(tokens)):
        dupes = sorted({t for t in tokens if tokens.count(t) > 1})
        raise AssertionError(f"duplicate control tokens: {dupes}")

    return tuple(tokens)


CONTROL_TOKENS: Tuple[str, ...] = _build_vocabulary()

# The base vocabulary the control tokens are prepended to.
VOCAB_BUDGET: int = 32_000


@dataclass(frozen=True)
class ControlToken:
    """One entry in the control vocabulary, with the id it will occupy."""

    token: str
    index: int

    @property
    def id(self) -> int:
        """Final id in the combined vocabulary, after the base BPE tokens."""
        return VOCAB_BUDGET + self.index


_TOKEN_INDEX: Dict[str, int] = {t: i for i, t in enumerate(CONTROL_TOKENS)}
_ID_INDEX: Dict[int, str] = {i: t for i, t in enumerate(CONTROL_TOKENS)}


def control_token(token: str) -> ControlToken:
    if token not in _TOKEN_INDEX:
        raise KeyError(f"unknown control token {token!r}")
    return ControlToken(token=token, index=_TOKEN_INDEX[token])


def token_id(token: str) -> int:
    """Id in the combined vocabulary."""
    return control_token(token).id


def token_string(token_id_: int) -> str:
    """Inverse lookup, including the base range."""
    if token_id_ >= VOCAB_BUDGET:
        return _ID_INDEX[token_id_ - VOCAB_BUDGET]
    return f"<base_{token_id_}>"


def pad_id() -> int:
    return token_id("<|pad|>")


def bos_id() -> int:
    return token_id("<|bos|>")


def eos_id() -> int:
    return token_id("<|eos|>")


def unk_id() -> int:
    return token_id("<|unk|>")


def affect_reset_id() -> int:
    return token_id("<|affect_reset|>")


def boundary_ids() -> Dict[str, int]:
    return {
        "set": token_id("<|boundary_set|>"),
        "hold": token_id("<|boundary_hold|>"),
        "release": token_id("<|boundary_release|>"),
    }


def vad_token_id(valence: float, arousal: float, dominance: float) -> List[int]:
    """Ids for a VAD triple. Snapshots to the nearest level on each axis."""
    return [token_id(t) for t in vad_token(valence, arousal, dominance)]


def is_control(token: str) -> bool:
    return token in _TOKEN_INDEX


def describe() -> Dict[str, int]:
    """Counts per group, for the README and for spotting a vocabulary that grew unnoticed."""
    groups = {
        "special": 4,
        "emotion": len(PLUTCHIK_PRIMARIES) * len(INTENSITIES),
        "dyad": len(PLUTCHIK_DYADS),
        "intent": len(INTENTS),
        "vad": 3 * len(VAD_LEVELS),
        "boundary_stance": 8,
        "framing": 5,
    }
    groups["total"] = sum(groups.values())
    return groups


def assert_budget(vocab_size: Optional[int] = None) -> None:
    """The control vocabulary must fit in the base vocabulary's tail.

    Silently overrunning means control ids collide with BPE ids, and a boundary marker turns
    into a random word with no error anywhere. Cheap to check, so it is checked.
    """
    base = vocab_size or VOCAB_BUDGET
    if len(CONTROL_TOKENS) > base:
        raise ValueError(
            f"{len(CONTROL_TOKENS)} control tokens do not fit in a {base}-token base "
            "vocabulary; widen it or trim the control set"
        )