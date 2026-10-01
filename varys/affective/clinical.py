"""Clinical dimensions, on the HiTOP spectra.

Varys is specified as "capable of dissecting complex psychopathology". That needs a
representation of psychopathology that is not a re-skin of Plutchik.

## Why HiTOP and not the DSM categories

The DSM is a categorical nosology organised by what clinicians decided to bill. Its
categories overlap heavily and are not orthogonal: a person can meet criteria for several at
once, and "a mood disorder" plus "an anxiety disorder" describes most presentations. Encoding
that as one-hot labels gives the model a target it cannot be right about.

HiTOP (Kotov et al., 2017) proposes dimensions instead, on the empirical observation that
psychopathology is organised along a small number of continuous spectra rather than discrete
boxes. Six spectra cover the space, they are near-orthogonal, and every DSM category sits in
some region of them. That makes them a usable training target.

## What these numbers are and are not

Each spectrum is a single value in ``[0, 1]``: 0 is absent, 1 is marked. They are *not*
symptom counts, not a diagnostic instrument, and not a severity scale across different
people.

The hard limit worth stating plainly: a symptom checklist conducted by a language model is
not a clinical assessment. The scales here are for organising what the model notices and for
giving the reward model something to score against. They must not be surfaced as a
diagnosis. :func:`screening_flags` is named for what it is, a set of things worth a human's
attention, and the docstring says so.

The taxonomy table is deliberately *thin*. Each spectrum carries its canonical sub-facets so
the structure is available, but no lexicon or anchor values are authored per facet: fabricating
plausible-looking coordinates for 70 clinical constructs would be inventing numbers and
giving them the authority of measurements.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Dict, FrozenSet, List, Optional, Sequence, Tuple

__all__ = [
    "SPECTRA",
    "SPECTRA_DIMS",
    "SpectrumVector",
    "neutral_spectra",
    "clamp_spectra",
    "SPECTRA_NEIGHBOURS",
    "screening_flags",
    "cooccurring",
    "HI_TOP_CITATION",
]

HI_TOP_CITATION = (
    "Kotov, R., Watson, D. J., First, M. B., et al. (2017). The Hierarchical Taxonomy of "
    "Psychopathology (HiTOP): a dimensional approach to psychopathology. Journal of Abnormal "
    "and Social Psychology, 113(6), 1437."
)


@dataclass(frozen=True)
class Spectrum:
    """One HiTOP spectrum with its canonical facets."""

    name: str
    description: str
    facets: Tuple[str, ...] = ()

    @property
    def n_facets(self) -> int:
        return len(self.facets)


SPECTRA: Dict[str, Spectrum] = {
    "internalizing": Spectrum(
        name="internalizing",
        description=(
            "Distress experienced in the self or interpersonal realm: depressed mood, "
            "anxiety, guilt, worthlessness."
        ),
        facets=("depressed_mood", "anxiety", "social_anxiety", "guilt", "worthlessness"),
    ),
    "detachment": Spectrum(
        name="detachment",
        description=(
            "Withdrawal from inner and outer experience: loss of motivation, anhedonia, "
            "social withdrawal."
        ),
        facets=("anhedonia", "low_interest", "social_withdrawal", "indifference"),
    ),
    "dissociation": Spectrum(
        name="dissociation",
        description=(
            "Disruption in the coordination of memory, identity, affect and perception: "
            "depersonalisation, amnesia, identity disturbance."
        ),
        facets=("depersonalisation", "derealisation", "amnesia", "identity_disturbance"),
    ),
    "thought": Spectrum(
        name="thought",
        description=(
            "Disruption in thought form and content: obsessions, compulsions, unusual beliefs."
        ),
        facets=("obsessions", "compulsions", "unusual_beliefs", "thought_disorder"),
    ),
    "psychoticism": Spectrum(
        name="psychoticism",
        description=(
            "Dissociation from reality expressed as unusual perceptual experiences and "
            "distorted beliefs."
        ),
        facets=("hallucinations", "delusions", "perceptual_disturbance"),
    ),
    "externalizing": Spectrum(
        name="externalizing",
        description=(
            "Disruption in behaviour involving loss of control: impulsive aggression, "
            "rule-breaking, deceitfulness."
        ),
        facets=("impulsive_aggression", "rule_violation", "deceitfulness", "irritability"),
    ),
}

SPECTRA_DIMS: Tuple[str, ...] = tuple(SPECTRA)

# Co-occurrence is the clinically important part and the reason dimensions beat categories.
# Every pair below is a frequent real co-occurrence. A representation that treats them as
# independent will happily diagnose a patient with "pure" dissociation and nothing else,
# which is the error dimensional structure exists to prevent.
SPECTRA_NEIGHBOURS: Dict[str, FrozenSet[str]] = {
    "internalizing": frozenset({"detachment", "psychoticism"}),
    "detachment": frozenset({"internalizing", "dissociation"}),
    "dissociation": frozenset({"detachment", "psychoticism", "thought"}),
    "thought": frozenset({"dissociation", "psychoticism"}),
    "psychoticism": frozenset({"internalizing", "thought", "dissociation", "externalizing"}),
    "externalizing": frozenset({"psychoticism", "internalizing"}),
}


def neutral_spectra() -> Tuple[float, ...]:
    """All-zero, meaning nothing marked. The correct starting point."""
    return (0.0,) * SPECTRA_DIMS.__len__()


def clamp_spectra(values: Sequence[float]) -> Tuple[float, ...]:
    """Hold every spectrum inside ``[0, 1]``.

    Values here are *intensities of a construct*, not signed coordinates, so the range is not
    symmetric and there is no meaningful zero-crossing.
    """
    return tuple(max(0.0, min(1.0, float(v))) for v in values)


class SpectrumVector:
    """The six spectra, with named access and a few operations the training loop needs."""

    __slots__ = ("_v",)

    def __init__(self, values: Optional[Sequence[float]] = None):
        if values is None:
            self._v: Tuple[float, ...] = neutral_spectra()
        else:
            if len(values) != len(SPECTRA_DIMS):
                raise ValueError(
                    f"expected {len(SPECTRA_DIMS)} spectra {SPECTRA_DIMS}, got {len(values)}"
                )
            self._v = clamp_spectra(values)

    def __getitem__(self, name: str) -> float:
        return self._v[SPECTRA_DIMS.index(name)]

    def __setitem__(self, name: str, value: float) -> None:
        idx = SPECTRA_DIMS.index(name)
        v = list(self._v)
        v[idx] = max(0.0, min(1.0, float(value)))
        self._v = tuple(v)

    def __iter__(self):
        return iter(self._v)

    def __len__(self) -> int:
        return len(self._v)

    def __eq__(self, other: object) -> bool:
        if isinstance(other, SpectrumVector):
            return self._v == other._v
        if isinstance(other, (tuple, list)):
            return self._v == tuple(clamp_spectra(other))
        return NotImplemented

    @property
    def vector(self) -> Tuple[float, ...]:
        return self._v

    @property
    def marked(self) -> Dict[str, float]:
        """Only the spectra above ``epsilon``. An all-zero vector reports nothing marked,
        which is different from reporting everything as absent."""
        eps = 1e-6
        return {n: v for n, v in zip(SPECTRA_DIMS, self._v) if v > eps}

    @property
    def peak(self) -> Tuple[Optional[str], float]:
        """The most marked spectrum. ``(None, 0.0)`` when nothing is marked."""
        if not self.marked:
            return None, 0.0
        name = max(self.marked, key=lambda n: self.marked[n])
        return name, self.marked[name]

    def to_dict(self) -> Dict[str, float]:
        return dict(zip(SPECTRA_DIMS, self._v))

    @classmethod
    def from_dict(cls, d: Dict[str, float]) -> "SpectrumVector":
        return cls([d.get(n, 0.0) for n in SPECTRA_DIMS])


def cooccurring(marked: Dict[str, float]) -> List[Tuple[str, str]]:
    """Pairs within ``marked`` that commonly co-occur clinically.

    Useful as a consistency check on the reward model: a response that flags
    ``internalizing`` while being silent about ``detachment`` may simply be wrong, and this
    is where that shows up.
    """
    names = set(marked)
    pairs: List[Tuple[str, str]] = []
    for a in names:
        for b in SPECTRA_NEIGHBOURS.get(a, ()):  # type: ignore[arg-type]
            if b in names and tuple(sorted((a, b))) not in pairs:
                pairs.append(tuple(sorted((a, b))))  # type: ignore[arg-type]
    return sorted(pairs)


def screening_flags(
    spectra: SpectrumVector,
    threshold: float = 0.6,
    strong: float = 0.85,
) -> Dict[str, object]:
    """What a clinician would want to know, and nothing more.

    This is a *screening* aid, not a diagnosis. A language model reading text cannot conduct
    a clinical assessment, and a model that reports one is worse than a model that reports
    nothing. The distinction that matters:

    * what was noticed in the text
    * whether the pattern warrants a human professional looking at it

    Args:
        threshold: mark a spectrum as present at or above this.
        strong: flag it as prominent at or above this.
    """
    marked = spectra.marked
    present = {n: v for n, v in marked.items() if v >= threshold}

    return {
        "present": present,
        "prominent": sorted(n for n, v in marked.items() if v >= strong),
        "pairs": cooccurring(present),
        "disclaimer": (
            "Generated from text. Not a diagnosis and not a clinical assessment. A licensed "
            "professional would need a proper assessment to say anything about a person."
        ),
    }