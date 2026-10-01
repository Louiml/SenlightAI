"""The Varys state: one vector, four subsystems, three timescales.

## Layout

Varys does not replace Elafry's VAD cube, it grows past it. The continuous state is 21
dimensions in three blocks:

    block 1   valence, arousal, dominance                      3   signed  ``[-1, 1]``
    block 2   HiTOP spectra                                     6   unsigned ``[0, 1]``
    block 3   Moral Foundations, both poles per foundation     12   unsigned ``[0, 1]``

Plutchik is deliberately *not* a block. Every node on the wheel is derived from a VAD
midpoint, so a Plutchik block would be a second, redundant encoding of information already
in block 1. It is kept as a symbolic annotation instead: a label, a depth on the wheel, and
a similarity, recomputed from VAD on demand. That keeps the wheel navigable without paying for
32 dimensions that carry 3 dimensions of information.

## The three-slot moral block, corrected

The original plan gave Moral Foundations Theory six slots, one signed scalar per foundation.
That cannot represent a strong position on *both* poles of one foundation at once, which is
the configuration Moral Licensing Theory identifies: a person who is genuinely non-prejudiced
in intent but has still done something harmful, and whose "I am a good person" belief is
doing the work. A signed scalar reads as neutral there. Twelve slots read it correctly, and
the vector length is not a real cost next to 72B parameters.

## Why the axes decay on different clocks

This is the part worth getting right, and it is not in the spec.

Affect, clinical spectrum, and moral grounding do not move at the same rate, and a state
update that treats them identically is a category error rather than a simplification:

* **VAD is fast and mean-reverting.** Affect is responsive and it passes. A neutral origin
  is the right fixed point because an emotional state with no driver decays to nothing.
* **Spectra are slow and floored.** A psychopathology spectrum is not "currently active". It
  is a property of a person that expresses in some blocks and not others. Decaying it to zero
  between mentions would turn a person into a series of unrelated episodes, which is exactly
  the error the longitudinal profile exists to prevent. Each spectrum therefore decays toward
  a *baseline*, not toward zero.
* **Moral grounding is the slowest and the stickiest.** Values do not flicker with context the
  way affect does. Aggressive persuasion of someone's values is one of the most reliable
  attacks on a model, and a fast-moving moral state is one that can be pushed.

So :func:`transition` takes per-block decay rates, and the defaults encode exactly that
ordering. The clinical and moral blocks are also given a slow *rise* rate, meaning evidence
has to accumulate before a spectrum is treated as present. A single utterance mentioning
death is not suicidal ideation, and the rise rate is what stops the model from saying it is.
"""

from __future__ import annotations

import math
from dataclasses import dataclass, field
from typing import Any, Dict, List, Optional, Sequence, Tuple

from varys.affective.clinical import SPECTRA_DIMS, SpectrumVector, clamp_spectra
from varys.affective.moral import MORAL_DIMS, MoralVector, clamp_moral
from varys.affective.plutchik import classify_affect
from varys.affective.vad import (
    VAD,
    VAD_AXES,
    clamp_vad,
    neutral_vad,
    vad_distance,
)

__all__ = [
    "VAD_SLICE",
    "SPECTRA_SLICE",
    "MORAL_SLICE",
    "STATE_DIMS",
    "STATE_BLOCKS",
    "VAD_DECAY",
    "SPECTRA_DECAY",
    "MORAL_DECAY",
    "SPECTRA_BASELINE",
    "SPECTRA_RISE",
    "MORAL_RISE",
    "VarysState",
    "neutral_state",
    "state_to_vector",
    "vector_to_state",
    "transition",
    "EvidenceGate",
    "describe",
    "VARYS_FEATURE_NAMES",
    "DEFAULT_FEATURE_BLOCKS",
    "state_feature_dim",
    "feature_names",
]


# ------------------------------------------------------------------- layout

VAD_SLICE = slice(0, 3)
SPECTRA_SLICE = slice(3, 9)
MORAL_SLICE = slice(9, 21)

STATE_DIMS: Tuple[str, ...] = (
    VAD_AXES + SPECTRA_DIMS + MORAL_DIMS
)

STATE_BLOCKS: Dict[str, slice] = {
    "vad": VAD_SLICE,
    "spectra": SPECTRA_SLICE,
    "moral": MORAL_SLICE,
}


# ----------------------------------------------------------------- dynamics

# Ordered by timescale, fastest to slowest. These are the defaults; they are exposed as
# module constants rather than buried as literals so a training run can log and sweep them.
VAD_DECAY = 0.70
SPECTRA_DECAY = 0.94
MORAL_DECAY = 0.98

# Clinical spectra decay toward this rather than toward zero. It represents a trait that is
# present whether or not the current text expresses it.
SPECTRA_BASELINE = 0.05

# Rise rates. Slower than the decay rates, which is the point: evidence has to accumulate
# before a spectrum counts as present, so one passing mention of death does not register.
SPECTRA_RISE = 0.60
MORAL_RISE = 0.35


# -------------------------------------------------------------------- state

@dataclass
class VarysState:
    """Varys's full internal state.

    Holds the three continuous blocks plus the symbolic Plutchik annotation, which is derived
    from ``vad`` rather than stored, so it can never disagree with the cube it came from.
    """

    vad: VAD = field(default_factory=VAD)
    spectra: SpectrumVector = field(default_factory=SpectrumVector)
    moral: MoralVector = field(default_factory=MoralVector)

    # ------------------------------------------------------------ blocks

    def to_vector(self) -> Tuple[float, ...]:
        return state_to_vector(self)

    @classmethod
    def from_vector(cls, v: Sequence[float]) -> "VarysState":
        return vector_to_state(v)

    def to_dict(self) -> Dict[str, Any]:
        return {
            "vad": self.vad.to_dict(),
            "spectra": self.spectra.to_dict(),
            "moral": self.moral.to_dict(),
            "plutchik": self.plutchik.to_dict(),
        }

    # --------------------------------------------------------- annotation

    @property
    def plutchik(self):
        """Where on the wheel this state's affect sits. Recomputed, never stored."""
        return classify_affect(self.vad.vector)

    # --------------------------------------------------------- operations

    def distance(self, other: "VarysState") -> float:
        """Euclidean distance over all 21 dimensions.

        Divided by ``sqrt(STATE_DIMS)`` so the result is a mean per-axis deviation in
        roughly ``[0, 1]``. Without that, the number grows with the state size and cannot be
        compared across the two models, which is the only reason to compute it.
        """
        return vad_distance(self.to_vector(), other.to_vector()) / math.sqrt(len(STATE_DIMS))

    def similarity(self, other: "VarysState") -> float:
        a, b = self.to_vector(), other.to_vector()
        na = math.sqrt(sum(x * x for x in a))
        nb = math.sqrt(sum(x * x for x in b))
        if na == 0.0 or nb == 0.0:
            return 0.0
        return sum(p * q for p, q in zip(a, b)) / (na * nb)

    def is_neutral(self, eps: float = 1e-6) -> bool:
        return all(abs(x) <= eps for x in self.to_vector())


def neutral_state() -> VarysState:
    """No affect, no marked spectrum, no active foundation.

    Note that a neutral *state* is not a neutral *person*: a real profile with all-zero
    clinical baselines is a person with nothing to report, and a profile whose baseline sits
    above zero is not "non-neutral", it has a floor. See :data:`SPECTRA_BASELINE`.
    """
    return VarysState(
        vad=VAD(*neutral_vad()),
        spectra=SpectrumVector(),
        moral=MoralVector(),
    )


def state_to_vector(state: VarysState) -> Tuple[float, ...]:
    return (
        tuple(clamp_vad(state.vad.vector))
        + tuple(clamp_spectra(state.spectra.vector))
        + tuple(clamp_moral(state.moral.vector))
    )


def vector_to_state(v: Sequence[float]) -> VarysState:
    v = tuple(float(x) for x in v)
    if len(v) != len(STATE_DIMS):
        raise ValueError(f"Varys state needs {len(STATE_DIMS)} dims {STATE_DIMS}, got {len(v)}")
    return VarysState(
        vad=VAD(*v[VAD_SLICE]),
        spectra=SpectrumVector(v[SPECTRA_SLICE]),
        moral=MoralVector(v[MORAL_SLICE]),
    )


# ---------------------------------------------------------------- transition

def transition(
    state: VarysState,
    drive: Optional[Sequence[float]] = None,
    vad_decay: float = VAD_DECAY,
    spectra_decay: float = SPECTRA_DECAY,
    moral_decay: float = MORAL_DECAY,
    baseline: float = SPECTRA_BASELINE,
    spectra_rise: float = SPECTRA_RISE,
    moral_rise: float = MORAL_RISE,
    drive_gain: float = 1.0,
) -> VarysState:
    """Advance one block.

    The three blocks get different recurrences on purpose, per the module docstring:

    * VAD     ``v <- clamp(decay * v + drive)``, mean-reverting toward the origin
    * spectra ``s <- clamp(decay * s + rise * drive)`` floored at ``baseline``
    * moral   ``m <- clamp(decay * m + rise * drive)`` floored at 0

    Args:
        state: current state.
        drive: per-dimension evidence for this block, same length as the state vector.
            ``None`` means no new evidence, so the state only relaxes.
        baseline: floor for the clinical block, the trait that persists between mentions.
        drive_gain: global scale on the drive, for temperature control at inference.

    ## The recurrence does not decide what counts as evidence

    An earlier version of this function tried to stop a single mention from registering a
    spectrum by making the rise rate small, and that does not work. A small rise rate and
    high stickiness are the same knob, so slowing one slows the other and the state stops
    responding to sustained evidence too. The limit of a pure leaky integrator is that it
    cannot distinguish one strong mention from many weak ones.

    So the gate is explicit and lives in :class:`EvidenceGate`: the recurrence integrates,
    the gate decides whether an observation is supported often enough to be worth driving
    with. Keep them separate.
    """
    current = state.to_vector()
    if drive is None:
        d = (0.0,) * len(current)
    else:
        d = tuple(float(x) * drive_gain for x in drive)
        if len(d) != len(current):
            raise ValueError(f"drive needs {len(current)} dims, got {len(d)}")

    out = list(current)

    # VAD: fast, mean-reverting to the origin.
    for i in range(VAD_SLICE.start, VAD_SLICE.stop):
        out[i] = vad_decay * current[i] + d[i]

    # Spectra: slow, slow to rise, and floored once raised. An untouched spectrum stays
    # exactly 0, so `marked` reports nothing rather than reporting a floor value as present.
    # Once a spectrum has been raised, it relaxes toward `baseline` instead of to 0, which is
    # what makes a trait persist between the blocks that express it.
    #
    # The latch is `current > 0`, not `current > baseline`. Comparing against the baseline
    # fails exactly at the boundary: a spectrum relaxing down to the baseline has
    # `current == baseline`, which is not `> baseline`, so on the next step it drops through
    # the floor and decays to nothing. Testing for any positive value latches correctly, and
    # a spectrum driven hard negative returns to 0 and becomes unraised again, which is the
    # right reading of strong counter-evidence.
    for i in range(SPECTRA_SLICE.start, SPECTRA_SLICE.stop):
        relaxed = spectra_decay * current[i]
        nxt = relaxed + spectra_rise * d[i]
        if current[i] > 0.0:
            nxt = max(baseline, nxt)
        out[i] = nxt

    # Moral: slowest, sticky.
    for i in range(MORAL_SLICE.start, MORAL_SLICE.stop):
        relaxed = moral_decay * current[i]
        out[i] = max(0.0, relaxed + moral_rise * d[i])

    return vector_to_state(out)


class EvidenceGate:
    """Decides whether an observation is sustained enough to drive the state with.

    The clinical and moral blocks must not move on a single mention. "My grandmother died
    last year" is not evidence of a thought disorder, and a model that reports one is worse
    than a model that reports nothing.

    The gate tracks a per-dimension *confidence*, an exponential moving average of observed
    strength. Two properties follow, and both are the point:

    * A single mention cannot open it. Confidence starts at 0 and only approaches the
      observation strength, so one block of 0.4 gives 0.07, not 0.4.
    * Evidence that stops arriving decays it, and it decays through the same EMA rather than
      a separate decay pass. Absence is an observation of zero, which is the honest reading
      of "the model did not notice it this time" and stops the state ratcheting upward.

    The EMA is normalized, which is why it is an EMA and not a leaky sum. A counter
    ``s <- decay*s + strength`` has a fixed point of ``strength/(1-decay)``, so any evidence
    weaker than ``threshold*(1-decay)`` can never accumulate no matter how often it repeats:
    a real, repeated, weak signal would be silently dropped forever. An EMA has fixed point
    exactly equal to mean strength, so confidence means "how consistently present is this",
    which is what the gate is supposed to measure. The consequence is that a dimension which
    is only ever weakly present never opens the gate, and that is correct rather than a bug.

    Example::

        gate = EvidenceGate(dims=6, min_confidence=0.5, decay=0.5)
        gate.observe(None, 1.0)                     # 0.25, no output
        gate.observe("internalizing", 0.8)          # 0.525, fires
    """

    __slots__ = ("_conf", "_decay", "_min_confidence", "_dims")

    def __init__(self, dims: int, min_confidence: float = 0.5, decay: float = 0.5):
        if not 0.0 <= min_confidence <= 1.0:
            raise ValueError(f"min_confidence must be in [0, 1], got {min_confidence}")
        if not 0.0 < decay < 1.0:
            raise ValueError(f"decay must be in (0, 1), got {decay}")
        self._conf: List[float] = [0.0] * dims
        self._decay = decay
        self._min_confidence = min_confidence
        self._dims = dims

    def observe(self, hit: Optional[str], strength: float = 1.0, names: Sequence[str] = ()) -> float:
        """Record one block's observation for every dimension, returning this block's drive.

        Dimensions not named as ``hit`` are updated with strength 0, so not-being-observed
        is itself evidence and the gate cannot ratchet upward on silence.
        """
        s = max(0.0, min(1.0, float(strength)))
        if not names:
            raise ValueError("observe needs the dimension names it is scoring")

        if hit and hit not in names:
            return 0.0
        idx = names.index(hit) if hit else -1

        drive = 0.0
        for i in range(self._dims):
            observed = s if i == idx else 0.0
            self._conf[i] = (1.0 - self._decay) * self._conf[i] + self._decay * observed
            if i == idx and self._conf[i] >= self._min_confidence:
                drive = self._conf[i]
        return drive

    def confidence(self, names: Sequence[str] = ()) -> Dict[str, float]:
        if not names:
            return {str(i): c for i, c in enumerate(self._conf)}
        return {n: self._conf[i] for i, n in enumerate(names) if i < self._dims}

    @property
    def open_dimensions(self) -> List[int]:
        return [i for i, c in enumerate(self._conf) if c >= self._min_confidence]

    def reset(self) -> None:
        self._conf = [0.0] * self._dims

# ------------------------------------------------------- feature contract
#
# Torch-free on purpose. ``varys.config`` needs the feature width, and if the width lived in a
# module that imported torch then reading a config would pull in the whole tensor stack and
# the import graph would stop being a tree. Same reasoning as Elafry's
# ``STATE_FEATURE_NAMES``, extended for Varys's blocks.

VARYS_FEATURE_NAMES: Tuple[str, ...] = (
    ("valence", "arousal", "dominance")
    + SPECTRA_DIMS
    + MORAL_DIMS
)

# The moral block is excluded by default, and that default is a safety decision rather than a
# capacity one. 12 extra dimensions into a bias projection is nothing next to 72B parameters.
#
# Conditioning attention on someone's moral foundations is the exact mechanism the
# manipulation risk check exists to catch: give the model a live read on which foundations a
# person appeals to, and the cheapest way to move them is to activate those foundations rather
# than to give them an argument. The care axis is close to harmless, and liberty and sanctity
# are where persuasion lives, so the axis is not "moral reasoning" but "persuasion surface".
#
# VAD and the clinical spectra are different. Responding to distress with more care is the
# behaviour the whole project is for, and clinical state is not a persuasion surface. So the
# default feature set is 9: the cube plus the spectra.

DEFAULT_FEATURE_BLOCKS: Tuple[str, ...] = ("vad", "spectra")


def state_feature_dim(
    include_moral: bool = False,
    include_velocity: bool = False,
    include_intent: bool = False,
    include_peak: bool = False,
) -> int:
    """Width of the state feature vector for a given set of channels.

    Defaults to 9: bare VAD plus the HiTOP spectra. The moral block is opt-in, per the note
    above; the extra channels are opt-in because a caller has to genuinely have that signal.
    """
    n = 3 + len(SPECTRA_DIMS)
    if include_moral:
        n += len(MORAL_DIMS)
    if include_velocity:
        n += 4        # dVAD (3) plus d(peak spectrum) (1)
    if include_intent:
        n += 1
    if include_peak:
        n += 1
    return n


def feature_names(
    include_moral: bool = False,
    include_velocity: bool = False,
    include_intent: bool = False,
    include_peak: bool = False,
) -> Tuple[str, ...]:
    """The feature vector's channel names, in order. Width matches :func:`state_feature_dim`."""
    names: List[str] = ["valence", "arousal", "dominance"] + list(SPECTRA_DIMS)
    if include_moral:
        names += list(MORAL_DIMS)
    if include_velocity:
        names += ["d_valence", "d_arousal", "d_dominance", "d_peak_spectrum"]
    if include_intent:
        names += ["intent"]
    if include_peak:
        names += ["peak_spectrum"]
    return tuple(names)


def describe(state: VarysState) -> Dict[str, Any]:
    """A readable summary. For logs and diagnostics, not for showing to a person.

    The keys are deliberately reportable and the clinical block is reported as marked
    spectra rather than as a score, because "internalizing: 0.7" reads as a measurement and
    it is not one.
    """
    pl = state.plutchik
    return {
        "affect": pl.label,
        "affect_depth": pl.level,
        "vad": {k: round(v, 3) for k, v in state.vad.to_dict().items()},
        "spectra_marked": {k: round(v, 3) for k, v in state.spectra.marked.items()},
        "foundations_active": {k: round(v, 3) for k, v in state.moral.dominant().items()},
        "moral_polarity": {k: round(v, 3) for k, v in state.moral.polarity().items()},
    }
