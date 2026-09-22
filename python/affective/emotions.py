"""Affective AI core: VAD emotional space + Plutchik wheel.

This module is the Python analogue of the "3D VAD model" from the research
visuals. It turns free text into a point in Valence–Arousal–Dominance (VAD)
space and classifies it into:
  * 9 reference emotional centroids (exactly the ones defined in
    ``research/3dVisualOfEmotions.html``: Triumph, Anger, Fear, Sadness,
    Relaxation, Boredom, Surprise, Contentment, Neutral), and
  * the 8 Plutchik primary emotions (Joy, Sadness, Anger, Fear, Trust,
    Disgust, Surprise, Anticipation).

The scoring is lexicon-based: every token is projected through a curated
word→(V, A, D) table (external affective model), and the sentence vector is the
length-weighted mean of its word vectors. This is deterministic, dependency-free
and offline — a practical stand-in for a heavier neural sentiment model.

VAD conventions (from the research visual):
  Valence    X : pleasantness       (-1 despair … +1 joy)
  Arousal    Y : activation         (-1 low energy … +1 high energy)
  Dominance  Z : agency/control     (-1 submissive … +1 in control)
"""

from __future__ import annotations

import math
import re
from dataclasses import dataclass, field

# ---------------------------------------------------------------------------
# Reference emotional centroids — copied from the user's 3D visual
# ---------------------------------------------------------------------------
@dataclass
class Emotion:
    name: str
    vad: tuple[float, float, float]  # (valence, arousal, dominance)
    plutchik: str                    # Plutchik primary / derived label
    desc: str = ""


REFERENCE_EMOTIONS: list[Emotion] = [
    Emotion("Triumph/Excited", (1.00, 1.00, 1.00), "Joy"),
    Emotion("Anger/Rage", (-1.00, 1.00, 1.00), "Anger"),
    Emotion("Fear/Panic", (-1.00, 1.00, -1.00), "Fear"),
    Emotion("Sadness/Despair", (-1.00, -1.00, -1.00), "Sadness"),
    Emotion("Relaxation/Calm", (0.80, -0.80, 0.50), "Trust"),
    Emotion("Boredom/Apathy", (-0.50, -0.80, -0.50), "Disgust"),
    Emotion("Surprise", (0.20, 0.90, -0.50), "Surprise"),
    Emotion("Contentment", (0.90, -0.40, 0.60), "Trust"),
    Emotion("Neutral", (0.00, 0.00, 0.00), "Anticipation"),
]

# Plutchik wheel: 8 primaries with VAD anchors (literature-standard-ish).
PLUTCHIK_VAD: dict[str, tuple[float, float, float]] = {
    "Joy": (0.9, 0.6, 0.7),
    "Trust": (0.6, 0.3, 0.4),
    "Fear": (-0.4, 0.7, -0.7),
    "Surprise": (0.1, 0.8, -0.3),
    "Sadness": (-0.8, -0.5, -0.5),
    "Disgust": (-0.6, -0.1, -0.3),
    "Anger": (-0.7, 0.8, 0.6),
    "Anticipation": (0.2, 0.5, 0.3),
}


def _vad_nearest(va: float, ad: float, dn: float) -> str:
    """Ref + Plutchik label of the closest reference centroid to (V,A,D)."""
    best = min(
        REFERENCE_EMOTIONS,
        key=lambda e: (e.vad[0] - va) ** 2 + (e.vad[1] - ad) ** 2 + (e.vad[2] - dn) ** 2,
    )
    return best.plutchik


# ---------------------------------------------------------------------------
# Lexicon: word -> (V, A, D). A curated, coverable subset of NRC-VAD.
# ---------------------------------------------------------------------------
_LEX: list[tuple[str, float, float, float]] = [
    # high-positive
    ("joy", .9, .5, .6), ("happy", .9, .6, .6), ("excited", .8, .9, .6),
    ("thrilled", .9, .9, .7), ("delighted", .9, .6, .6), ("love", .9, .6, .5),
    ("glad", .8, .4, .4), ("great", .8, .4, .5), ("wonderful", .9, .5, .6),
    ("amazing", .8, .7, .5), ("awesome", .8, .7, .5), ("proud", .7, .5, .7),
    ("hopeful", .7, .4, .5), ("optimistic", .8, .4, .6), ("content", .7, -.3, .5),
    ("relaxed", .7, -.8, .4), ("calm", .6, -.8, .4), ("peaceful", .7, -.8, .5),
    ("grateful", .8, -.3, .4), ("safe", .5, -.5, .6), ("secure", .5, -.5, .6),
    ("confident", .6, .5, .9), ("strong", .5, .4, .9), ("triumphant", .9, .9, 1.0),
    # anger / frustration
    ("angry", -.7, .8, .6), ("furious", -.9, .9, .6), ("rage", -.9, .9, .6),
    ("frustrated", -.6, .6, -.2), ("irritated", -.5, .5, -.1), ("annoyed", -.5, .4, -.1),
    ("mad", -.7, .7, .5), ("upset", -.6, .5, -.3), ("resentful", -.6, .3, -.2),
    ("fuming", -.8, .8, .6), ("seething", -.8, .7, .5), ("infuriated", -.8, .8, .6),
    ("enraged", -.9, .9, .6), ("livid", -.8, .8, .5), ("incensed", -.8, .8, .5),
    # fear / anxiety
    ("afraid", -.5, .6, -.6), ("scared", -.6, .7, -.7), ("fearful", -.5, .6, -.6),
    ("terrified", -.8, .9, -.8), ("anxious", -.4, .7, -.3), ("worried", -.4, .5, -.3),
    ("nervous", -.3, .6, -.3), ("panicked", -.8, .9, -.8), ("afraid", -.5, .6, -.6),
    ("uneasy", -.4, .4, -.2), ("insecure", -.5, .2, -.8), ("vulnerable", -.4, .3, -.7),
    # sadness
    ("sad", -.8, -.5, -.5), ("unhappy", -.7, -.3, -.4), ("depressed", -.8, -.8, -.7),
    ("hopeless", -.9, -.6, -.8), ("miserable", -.9, -.4, -.6), ("lonely", -.7, -.6, -.4),
    ("heartbroken", -.9, -.4, -.6), ("grief", -.9, -.5, -.6), ("despair", -1.0, -.6, -.8),
    ("disappointed", -.6, -.3, -.3), ("down", -.6, -.5, -.4), ("gloomy", -.7, -.5, -.4),
    # disgust
    ("disgusted", -.6, -.1, -.3), ("revolted", -.6, -.2, -.3), ("grossed", -.5, -.2, -.2),
    # surprise
    ("surprised", .2, .9, -.5), ("astonished", .3, .8, -.3), ("shocked", -.3, .8, -.4),
    ("amazed", .7, .8, .2), ("stunned", .1, .6, -.4),
    # anticipation / mixed
    ("expectant", .4, .5, .4), ("anticipating", .3, .5, .3), ("curious", .5, .5, .3),
    ("interested", .5, .4, .4), ("engaged", .5, .4, .5), ("eager", .5, .6, .4),
    # low-arousal negative
    ("tired", -.2, -.7, -.3), ("exhausted", -.4, -.8, -.4), ("fatigued", -.3, -.7, -.3),
    ("bored", -.5, -.8, -.3), ("apathetic", -.4, -.7, -.4), ("numb", -.4, -.7, -.5),
    ("drained", -.5, -.7, -.5), ("weary", -.4, -.6, -.4), ("empty", -.8, -.7, -.6),
    # positive/low-arousal / neutral support
    ("okay", .3, -.1, .2), ("fine", .2, -.2, .2), ("normal", .1, .0, .1),
    ("alright", .2, -.1, .2), ("neutral", .0, .0, .0), ("meh", -.2, -.5, -.2),
    # interpersonal
    ("valued", .7, .2, .6), ("appreciated", .7, .2, .5), ("heard", .6, .0, .4),
    ("understood", .6, .0, .4), ("supported", .6, .1, .4), ("loved", .9, .4, .3),
    ("rejected", -.6, -.3, -.5), ("abandoned", -.8, -.4, -.7), ("ignored", -.6, -.4, -.5),
    ("overwhelmed", -.5, .6, -.6), ("trapped", -.7, .4, -.8), ("stressed", -.5, .6, -.4),
    # --- extra affective coverage for psychological prose ---
    ("worthless", -.8, -.4, -.8), ("devastated", -.9, -.4, -.6),
    ("heartbroken", -.9, -.4, -.6), ("grief", -.9, -.5, -.6),
    ("lonely", -.7, -.6, -.4), ("alone", -.5, -.5, -.3), ("isolated", -.6, -.5, -.5),
    ("guilty", -.6, .4, -.4), ("ashamed", -.6, .4, -.4), ("embarrassed", -.5, .3, -.3),
    ("regret", -.5, .2, -.3), ("sorry", -.4, .1, -.2), ("apologetic", -.3, .0, -.2),
    ("resentful", -.6, .3, -.2), ("bitter", -.6, .3, -.1), ("jealous", -.4, .5, -.1),
    ("envious", -.4, .4, -.1), ("moody", -.3, .4, -.2), ("grumpy", -.4, .3, -.1),
    ("cranky", -.4, .3, -.1), ("pensive", .0, .0, .0), ("thoughtful", .3, -.1, .2),
    ("mindful", .5, -.2, .5), ("reflective", .3, -.2, .3), ("introspective", .2, -.2, .2),
    ("focused", .4, .3, .7), ("determined", .4, .5, .8), ("driven", .4, .6, .8),
    ("motivated", .6, .6, .7), ("inspired", .7, .5, .4), ("creative", .6, .5, .6),
    ("excited", .8, .9, .6), ("energetic", .7, .9, .6), ("vibrant", .8, .8, .6),
    ("drained", -.5, -.7, -.5), ("burned", -.5, -.4, -.4), ("burnt", -.5, -.4, -.4),
    ("exhausted", -.4, -.8, -.4), ("sleepy", .1, -.8, -.1), ("drowsy", .0, -.7, -.1),
    ("restless", -.2, .6, -.2), ("agitated", -.4, .7, -.3), ("frantic", -.6, .9, -.5),
    ("jittery", -.3, .6, -.2), ("on-edge", -.4, .6, -.3), ("keyed", -.2, .6, -.1),
    ("gloomy", -.7, -.5, -.4), ("dreary", -.5, -.6, -.3), ("melancholy", -.7, -.6, -.4),
    ("wistful", -.3, -.4, -.1), ("nostalgic", .1, -.4, -.1), ("sentimental", .3, -.3, .1),
    ("hopeful", .7, .4, .5), ("optimistic", .8, .4, .6), ("encouraged", .6, .3, .5),
    ("reassured", .5, -.3, .4), ("comforted", .5, -.4, .4), ("soothed", .5, -.5, .4),
    ("relieved", .6, -.4, .4), ("at-ease", .6, -.6, .5), ("tranquil", .6, -.8, .5),
    ("serene", .6, -.8, .5), ("blissful", .9, -.3, .6), ("euphoric", .9, .9, .7),
    ("ecstatic", .9, .9, .7), ("elated", .8, .7, .6), ("cheerful", .8, .5, .5),
    ("jovial", .8, .6, .6), ("upbeat", .7, .6, .5), ("positive", .7, .3, .4),
    ("hopeful", .7, .4, .5), ("confident", .6, .5, .9), ("assured", .5, .2, .8),
    ("terrified", -.8, .9, -.8), ("panicked", -.8, .9, -.8), ("horrified", -.8, .8, -.6),
    ("dread", -.7, .5, -.6), ("dreading", -.6, .5, -.5), ("apprehensive", -.4, .5, -.4),
    ("wary", -.4, .4, -.4), ("suspicious", -.4, .5, -.3), ("distrustful", -.4, .4, -.4),
    ("content", .7, -.3, .5), ("satisfied", .6, -.2, .5), ("pleased", .7, .1, .5),
    ("fulfilled", .8, .1, .6), ("accomplished", .7, .3, .8), ("successful", .7, .3, .7),
    ("grieving", -.9, -.5, -.6), ("mourning", -.8, -.5, -.5), ("yearning", -.2, .2, -.1),
    ("longing", -.1, .1, -.1), ("craving", .1, .6, .1), ("desiring", .2, .5, .2),
    ("frustrated", -.6, .6, -.2), ("irritated", -.5, .5, -.1), ("miffed", -.4, .3, -.1),
    ("peeved", -.4, .3, -.1), ("annoyed", -.5, .4, -.1), ("spiteful", -.7, .4, .2),
    ("vindictive", -.7, .4, .3), ("hostile", -.7, .7, .5), ("aggressive", -.6, .8, .7),
    ("menacing", -.5, .7, .6), ("intimidated", -.5, .5, -.7), ("unwelcome", -.4, .2, -.4),
    ("shunned", -.5, .2, -.4), ("ostracized", -.6, .1, -.5), ("dismissed", -.5, .2, -.4),
    ("undervalued", -.5, .1, -.5), ("underappreciated", -.5, .1, -.4),
    ("misunderstood", -.5, .2, -.4), ("doubtful", -.4, .2, -.4),
    ("uncertain", -.3, .2, -.3), ("confused", -.3, .4, -.3), ("bewildered", -.2, .5, -.3),
    ("perplexed", -.2, .4, -.2), ("unsure", -.3, .2, -.3), ("hesitant", -.2, .2, -.2),
    ("reluctant", -.3, .1, -.2), ("resistant", -.3, .4, .3), ("defiant", -.4, .6, .6),
    ("rebellious", -.3, .7, .6), ("stubborn", -.3, .3, .7), ("obstinate", -.3, .2, .7),
]
# Build token -> VAD dict.
VAD_LEXICON: dict[str, tuple[float, float, float]] = {w: (v, a, d) for (w, v, a, d) in _LEX}

# ---------------------------------------------------------------------------
# Affective classifier
# ---------------------------------------------------------------------------
_WORD_RE = re.compile(r"[A-Za-z']+")


@dataclass
class AffectiveState:
    """The emotional state vector + label for a piece of text."""

    text: str = ""
    vad: tuple[float, float, float] = (0.0, 0.0, 0.0)
    confidence: float = 0.0
    emotion: str = "Neutral"
    plutchik: str = "Anticipation"
    matched_tokens: int = 0
    total_tokens: int = 0

    def label(self) -> str:
        return self.emotion

    def to_dict(self) -> dict:
        return {
            "text": self.text,
            "vad": [round(v, 3) for v in self.vad],
            "valence": round(self.vad[0], 3),
            "arousal": round(self.vad[1], 3),
            "dominance": round(self.vad[2], 3),
            "confidence": round(self.confidence, 3),
            "emotion": self.emotion,
            "plutchik": self.plutchik,
            "matched_tokens": self.matched_tokens,
            "total_tokens": self.total_tokens,
        }


def analyze(text: str | None) -> AffectiveState:
    """Compute the VAD vector and nearest emotional centroid for ``text``."""
    if not text:
        return AffectiveState(text="", emotion="Neutral", plutchik="Anticipation")

    tokens = [t.lower() for t in _WORD_RE.findall(text)]
    total = len(tokens)
    weights: list[tuple[float, float, float]] = []
    matched = 0
    for tok in tokens:
        hit = VAD_LEXICON.get(tok)
        if hit is not None:
            weights.append(hit)
            matched += 1

    if not weights:
        return AffectiveState(
            text=text, vad=(0.0, 0.0, 0.0), confidence=0.0,
            emotion="Neutral", plutchik="Anticipation",
            matched_tokens=0, total_tokens=total,
        )

    # Length-weighted mean of matched word vectors.
    v = sum(w[0] for w in weights) / len(weights)
    a = sum(w[1] for w in weights) / len(weights)
    d = sum(w[2] for w in weights) / len(weights)
    v, a, d = max(-1.0, min(1.0, v)), max(-1.0, min(1.0, a)), max(-1.0, min(1.0, d))

    conf = matched / max(1, total)
    # Nearest Plutchik anchor for a finer-grained label.
    plutchik_label = min(
        PLUTCHIK_VAD,
        key=lambda k: (PLUTCHIK_VAD[k][0] - v) ** 2
        + (PLUTCHIK_VAD[k][1] - a) ** 2
        + (PLUTCHIK_VAD[k][2] - d) ** 2,
    )
    # Nearest reference centroid for the visual's coarse emotion name.
    nearest = min(
        REFERENCE_EMOTIONS,
        key=lambda e: (e.vad[0] - v) ** 2 + (e.vad[1] - a) ** 2 + (e.vad[2] - d) ** 2,
    )
    return AffectiveState(
        text=text, vad=(v, a, d), confidence=conf,
        emotion=nearest.name, plutchik=plutchik_label,
        matched_tokens=matched, total_tokens=total,
    )


def plutchik_to_vad(label: str) -> tuple[float, float, float]:
    """Map a Plutchik emotion name to its VAD anchor."""
    return PLUTCHIK_VAD.get(label.title(), (0.0, 0.0, 0.0))


# Sanity helper used by tests / CLI.
if __name__ == "__main__":
    samples = [
        "I am feeling so anxious and worried lately",
        "I am absolutely thrilled with this great news",
        "I am angry and frustrated that you ignored me",
        "I feel sad and lonely and everything seems hopeless",
        "Everything is fine and calm today",
    ]
    for s in samples:
        st = analyze(s)
        v, a, d = st.vad
        print(
            f"{s[:38]!r:42} -> V={v:+.2f} A={a:+.2f} D={d:+.2f}  "
            f"[{st.emotion}]  conf={st.confidence:.2f}"
        )