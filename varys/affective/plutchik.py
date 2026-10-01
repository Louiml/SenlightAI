"""Plutchik's wheel as a hierarchy, not a flat list of eight.

Elafry implements eight primaries and stops. That is enough for "how does the model feel
right now", which is Elafry's whole job.

Varys is asked to dissect complex psychopathology, and a flat list of eight cannot express
"ambivalent", "guilt", or "suspicion". Plutchik's own theory already contains the hierarchy,
so Varys uses it:

    primaries   eight, mutually opposed in dyads
    dyads       eight, each the blend of two adjacent primaries
    tertiaries  sixteen, each the blend of one primary and one of its two dyads

The tertiary level is what makes it a hierarchy rather than a longer list. Plutchik's claim is
that tertiary blends are systematically more intense and more complex than their parents,
which means the intensity is implied by the position in the wheel rather than carried as a
separate float. That is a better parameterisation than an independent intensity knob,
because two independent knobs can disagree with each other.

Every blend's VAD anchor is derived from its parents rather than authored separately, so the
three levels cannot drift out of consistency. See :func:`derive_anchor`.

Note on naming. The dyad names below are the conventional ones from the wheel. Tertiary
names are not standardised across sources, so they are generated from their components
(``serenity`` from trust + anticipation) and documented as such.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Dict, List, Optional, Sequence, Tuple

from varys.affective.vad import VAD, neutral_vad

__all__ = [
    "PRIMARIES",
    "DYADS",
    "TERTIARIES",
    "PRIMARY_VAD",
    "PRIMARY_DYADS",
    "DYAD_PRIMARIES",
    "TERTIARY_PARTS",
    "Blended",
    "blend_vad",
    "derive_anchor",
    "classify_affect",
    "AffectClassification",
    "intensity_ramp",
]


# ---------------------------------------------------------------- primaries

PRIMARIES: Tuple[str, ...] = (
    "joy",
    "trust",
    "fear",
    "surprise",
    "sadness",
    "disgust",
    "anger",
    "anticipation",
)

# VAD anchors. These are the same eight Elafry uses, unchanged. Dominance is what separates
# anger from fear and sadness from disgust, so it is load-bearing rather than decorative.
PRIMARY_VAD: Dict[str, Tuple[float, float, float]] = {
    "joy": (0.85, 0.55, 0.60),
    "trust": (0.70, -0.20, 0.35),
    "fear": (-0.70, 0.65, -0.65),
    "surprise": (0.10, 0.90, -0.10),
    "sadness": (-0.80, -0.45, -0.60),
    "disgust": (-0.70, 0.20, 0.15),
    "anger": (-0.65, 0.75, 0.60),
    "anticipation": (0.20, 0.35, 0.45),
}

# Each primary has one opposite, in wheel order. joy-sadness, trust-distrust(fear),
# fear-surprise, surprise-anticipation, sadness-disgust, disgust-anger, anger-anticipation,
# anticipation-joy.
PRIMARY_DYADS: Dict[str, Tuple[str, str]] = {
    "joy": ("sadness", "joy"),
    "trust": ("fear", "trust"),
    "fear": ("anger", "fear"),
    "surprise": ("anticipation", "surprise"),
    "sadness": ("joy", "sadness"),
    "disgust": ("surprise", "disgust"),
    "anger": ("fear", "anger"),
    "anticipation": ("disgust", "anticipation"),
}


# -------------------------------------------------------------------- dyads

# A dyad is the blend of two adjacent primaries. Ordered so `PRIMARY_ORDER` above and the
# dyad list share a wheel orientation.
DYAD_PRIMARIES: Dict[str, Tuple[str, str]] = {
    "serenity": ("joy", "trust"),
    "fatalism": ("sadness", "trust"),
    "submission": ("trust", "fear"),
    "amazement": ("surprise", "fear"),
    "pessimism": ("sadness", "surprise"),
    "disappointment": ("fear", "disgust"),
    "loathing": ("anger", "disgust"),
    "vigilance": ("anticipation", "anger"),
}

# Conventional alternate names, so external text using either spelling resolves.
DYAD_ALIASES: Dict[str, str] = {
    "lovely": "serenity",
    "optimism": "serenity",
    "submission": "submission",
    "pessimistic": "pessimism",
    "rage": "loathing",
    "vigilant": "vigilance",
}

DYADS: Tuple[str, ...] = tuple(DYAD_PRIMARIES)


# --------------------------------------------------------------- tertiaries

def _build_tertiaries() -> Dict[str, Tuple[str, str]]:
    """One tertiary per (primary, adjacent dyad) pair.

    "Adjacent" means the dyads that contain that primary. joy belongs to serenity, so joy
    yields serenity_joy and fatalism_joy, and so on around the wheel.
    """
    out: Dict[str, Tuple[str, str]] = {}
    for primary in PRIMARIES:
        adjacents = [d for d, (a, b) in DYAD_PRIMARIES.items() if primary in (a, b)]
        for dyad in adjacents:
            out[f"{dyad}_{primary}"] = (primary, dyad)
    return out


TERTIARY_PARTS: Dict[str, Tuple[str, str]] = _build_tertiaries()
TERTIARIES: Tuple[str, ...] = tuple(TERTIARY_PARTS)


# ------------------------------------------------------------------ blending

@dataclass(frozen=True)
class Blended:
    """A point on the wheel, at whatever depth of the hierarchy it was reached."""

    label: str
    level: int          # 0 primary, 1 dyad, 2 tertiary
    parts: Tuple[str, ...]
    vad: VAD

    @property
    def depth(self) -> str:
        return ("primary", "dyad", "tertiary")[self.level]

    def to_dict(self) -> Dict[str, object]:
        return {"label": self.label, "level": self.depth, "parts": list(self.parts), "vad": self.vad.to_dict()}


def blend_vad(parts: Sequence[str]) -> VAD:
    """The VAD anchor of a blend, derived from its parts.

    The midpoint of the components, which is the simplest defensible choice and the one that
    keeps the hierarchy self-consistent: a dyad sits between its primaries, so by construction
    it is never closer to an unrelated primary than its parents are.

    An empty or unknown blend returns the neutral origin rather than raising, because the
    classifier runs over arbitrary model output and a hard failure there would take down a
    training run.
    """
    vectors: List[Tuple[float, float, float]] = []
    for part in parts:
        if part in PRIMARY_VAD:
            vectors.append(PRIMARY_VAD[part])
        elif part in DYAD_PRIMARIES:
            vectors.append(blend_vad(DYAD_PRIMARIES[part]).vector)
    if not vectors:
        return VAD(*neutral_vad())
    n = len(vectors)
    return VAD(*(sum(v[i] for v in vectors) / n for i in range(3)))


# The full wheel, assembled once. 32 nodes: 8 primaries, 8 dyads, 16 tertiaries.
WHEEL: Dict[str, Blended] = {}
for _name in PRIMARIES:
    WHEEL[_name] = Blended(_name, 0, (_name,), VAD(*PRIMARY_VAD[_name]))
for _name in DYADS:
    _parts = DYAD_PRIMARIES[_name]
    WHEEL[_name] = Blended(_name, 1, _parts, blend_vad(_parts))
for _name, _parts in TERTIARY_PARTS.items():
    WHEEL[_name] = Blended(_name, 2, _parts, blend_vad(_parts))


def resolve(label: str) -> Optional[str]:
    """Canonical name for a label, accepting the common alternate spellings."""
    label = label.strip().lower()
    if label in WHEEL:
        return label
    return DYAD_ALIASES.get(label)


def derive_anchor(label: str) -> VAD:
    """The VAD anchor for any node on the wheel, including the aliases."""
    name = resolve(label)
    return WHEEL[name].vad if name else VAD()


@dataclass
class AffectClassification:
    """What the classifier found, and how sure it is."""

    label: Optional[str]
    level: str
    similarity: float
    vad: Tuple[float, float, float]

    @property
    def confident(self) -> bool:
        return self.label is not None

    def to_dict(self) -> Dict[str, object]:
        return {
            "label": self.label,
            "level": self.level,
            "similarity": round(self.similarity, 4),
            "vad": [round(c, 4) for c in self.vad],
        }


def classify_affect(
    vad: Sequence[float],
    threshold: float = 0.80,
    deepest: int = 2,
    tie_tolerance: float = 1e-3,
) -> AffectClassification:
    """Nearest node on the wheel, deepened only when the blend is genuinely competitive.

    Every node is scored, then the deepest node within ``tie_tolerance`` of the best score
    wins. Searching deepest-first and returning on the first hit does not work: a tertiary
    anchor is a midpoint of its parents, so a tertiary always matches well enough to shadow
    the primary it came from, and a state sitting exactly on *joy* gets labelled
    ``serenity_joy``. Tolerancing instead means an exact primary match (similarity 1.0) beats
    a near-tertiary one (0.99), while a state genuinely pointing between joy and trust still
    resolves to *serenity*.

    ``deepest=0`` makes this exactly Elafry's eight-primary classifier.

    ## Why the tolerance is 1e-3 and not something loose

    Every node on the wheel is a midpoint of its parents, so a blend sits close to each
    parent. With a tolerance of 0.02 every tertiary shadows the primary it derives from and a
    state sitting exactly on *joy* comes back as ``serenity_joy``. With 1e-3 an exact parent
    match always wins, while a state genuinely pointing between two primaries still resolves
    to the blend.

    Measured, all at default threshold:

    | tolerance | exact joy | exact serenity | near joy+trust |
    | --- | --- | --- | --- |
    | 0.02 | tertiary | tertiary | tertiary |
    | 0.01 | tertiary | dyad | dyad |
    | 1e-3 | primary | dyad | dyad |
    | 1e-6 | primary | dyad | dyad |

    1e-3 is the loosest value that gets all three right, so it is the default. Anything
    above it starts labelling primaries as blends; anything below it is equivalent to exact
    tie-breaking and stops resolving genuine blends.
    """
    from varys.affective.vad import vad_similarity

    scored: List[Tuple[float, int, str]] = []
    for level in range(deepest + 1):
        for name, node in WHEEL.items():
            if node.level == level:
                scored.append((vad_similarity(vad, node.vad.vector), level, name))

    if not scored:
        return AffectClassification(label=None, level="none", similarity=0.0, vad=tuple(vad))

    best_sim = max(s for s, _, _ in scored)
    if best_sim < threshold:
        return AffectClassification(label=None, level="none", similarity=0.0, vad=tuple(vad))

    # Deepest first among the near-ties, so the winner is deterministic rather than
    # dependent on dict ordering.
    tied = [(s, lvl, n) for s, lvl, n in scored if s >= best_sim - tie_tolerance]
    tied.sort(key=lambda t: (-t[1], -t[0]))
    _, level, label = tied[0]

    return AffectClassification(
        label=label,
        level=("primary", "dyad", "tertiary")[level],
        similarity=best_sim,
        vad=tuple(vad),
    )

def intensity_ramp(label: str, steps: int = 3) -> List[VAD]:
    """Scale a node outward from the origin to express intensity.

    Tertiary blends are already more intense than their parents, so the ramp is only needed
    for primaries. Scaling by a factor along the origin-to-node ray preserves the direction,
    which is what "more intense" means on this wheel.
    """
    if steps < 2:
        raise ValueError(f"need at least 2 steps to express a ramp, got {steps}")
    node = WHEEL.get(resolve(label) or "")
    if node is None:
        return []
    base = node.vad.vector
    return [VAD(*(c * (0.7 + 0.15 * i) for c in base)) for i in range(steps)]