"""Affective AI core: utterance intent classification for mental-health support.

Extends ``affective/emotions.py`` (VAD space + Plutchik labels) with a
fine-grained *communicative intent* label. The pipeline (Step 1 affective
annotation) tags every user turn with one of:

    * ``Crisis``          — active self-harm / immediate danger (highest priority)
    * ``Seeking_Validation`` — wanting reassurance about self-worth / being heard
    * ``Venting``         — high-arousal negative release (anger/frustration/upset)
    * ``Advice_Seeking``  — asking what to do; wants guidance/options
    * ``Disclosure``      — sharing an experience/story (wants reflection/empathy)
    * ``Casual``          — everyday chat, no affective need

The classifier combines two signals:

1. A **lexicon + rules** layer over the VAD scores from ``emotions.analyze``.
2. A set of **surface-form detectors** (keyword / regex) for crisis signals and
   question-style intent that pure VAD misses (e.g. "what should I do",
   "I want to end it").

The classifier is deterministic, offline and dependency-free — consistent with
the rest of the embedded affective stack — so the annotation step never needs a
network or a heavy model.
"""

from __future__ import annotations

import re
import sys
from dataclasses import dataclass, field
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from affective.emotions import analyze, AffectiveState  # noqa: E402

# ---------------------------------------------------------------------------
# Intent taxonomy (stable machine-friendly keys + human labels)
# ---------------------------------------------------------------------------
INTENTS: tuple[str, ...] = (
    "Crisis",
    "Seeking_Validation",
    "Venting",
    "Advice_Seeking",
    "Disclosure",
    "Casual",
)

INTENT_LABELS: dict[str, str] = {
    "Crisis": "Crisis / immediate risk",
    "Seeking_Validation": "Seeking validation & reassurance",
    "Venting": "Venting / emotional release",
    "Advice_Seeking": "Seeking advice & options",
    "Disclosure": "Sharing an experience",
    "Casual": "Casual / neutral chat",
}

# ---------------------------------------------------------------------------
# Crisis keywords (self-harm / danger / "end it"). Fuzzy-ish word list; the
# containment regex below handles the most dangerous forms.
# ---------------------------------------------------------------------------
_CRISIS_WORDS: tuple[str, ...] = (
    "kill myself", "killing myself", "end my life", "end it all",
    "take my own life", "suicide", "suicidal", "self harm", "self-harm",
    "hurt myself", "harming myself", "don't want to live", "do not want to live",
    "no reason to live", "want to die", "wish i was dead", "better off dead",
    "overdose", "jump off", "cutting myself", "hang myself",
    "end it", "ending it", "end everything", "ending everything",
    "can't go on", "cannot go on", "give up on life", "want to disappear",
    "disappear forever", "stop existing",
)

_CRISIS_RE = re.compile(r"|".join(re.escape(w) for w in _CRISIS_WORDS), re.IGNORECASE)

# ---------------------------------------------------------------------------
# Advice-seeking surface forms (questions that ask for direction/options)
# ---------------------------------------------------------------------------
_ADVICE_RE = re.compile(
    r"\b(what (do|should|to|can) i\b|how (do|should|can) i\b|anyone (know|else)"
    r"\b|do you (think|have)\b|what would you\b|should i\b|could you (help|tell)\b"
    r"|i don'?t know what to do\b|i need (help|advice|options|guidance)\b)",
    re.IGNORECASE,
)

# Validation-seeking: self-worth / "am I" / needing to be okay
_VALIDATION_RE = re.compile(
    r"\b(am i (a |the |so |too )?(bad|loser|failure|worthless|broken|wrong)"
    r"|do you (think|feel)|is it (ok|okay|normal|my fault)"
    r"|i feeling (better|ok|okay)|it'?s (so|really|just) hard\b|no one (cares|listens)"
    r"|does anyone (care|even)\b)\b",
    re.IGNORECASE,
)

# ---------------------------------------------------------------------------
# Intent record
# ---------------------------------------------------------------------------
@dataclass
class IntentState:
    """The intent label + the affective state that informed it."""

    intent: str = "Casual"
    text: str = ""
    vad: tuple[float, float, float] = (0.0, 0.0, 0.0)
    valence: float = 0.0
    arousal: float = 0.0
    dominance: float = 0.0
    plutchik: str = "Anticipation"
    emotion: str = "Neutral"
    confidence: float = 0.0
    signals: list[str] = field(default_factory=list)
    is_crisis: bool = False

    def to_dict(self) -> dict:
        return {
            "intent": self.intent,
            "text": self.text,
            "vad": [round(v, 3) for v in self.vad],
            "valence": round(self.valence, 3),
            "arousal": round(self.arousal, 3),
            "dominance": round(self.dominance, 3),
            "plutchik": self.plutchik,
            "emotion": self.emotion,
            "confidence": round(self.confidence, 3),
            "is_crisis": self.is_crisis,
            "signals": self.signals,
        }


def _classify(v: float, a: float, d: float, text: str) -> tuple[str, list[str]]:
    """Return ``(intent, signals)`` from VAD + surface form, priority-ordered."""

    # 1) Crisis overrides everything — immediate risk takes precedence.
    if _CRISIS_RE.search(text):
        return "Crisis", ["crisis:keyword"]

    signals: list[str] = []

    # 2) Advice-seeking: an explicit ask for direction or help.
    if _ADVICE_RE.search(text):
        signals.append("advice:question")

    # 3) Validation-seeking: negative self-view, seeking reassurance/being heard.
    if _VALIDATION_RE.search(text):
        signals.append("validation:keyword")
    low_dom = d < -0.3
    neg_val = v < -0.15
    if neg_val and low_dom:
        signals.append("validation:low-valence+low-dominance")

    # 4) Distinguish positive vs negative case.
    if v >= -0.05:
        # Neutral-to-positive content: casual unless a clear ask is present.
        if any(s.startswith("advice:") for s in signals):
            return "Advice_Seeking", signals
        if any(s.startswith("validation:") for s in signals):
            return "Seeking_Validation", signals
        return "Casual", signals

    # Negative valence region: choose among venting / advice / validation / disclosure.
    if any(s.startswith("advice:") for s in signals):
        return "Advice_Seeking", signals
    if any(s.startswith("validation:") for s in signals):
        return "Seeking_Validation", signals
    if a >= 0.35:
        # High-arousal negative -> venting (anger / frustration / upset).
        signals.append("venting:high-arousal-negative")
        return "Venting", signals
    # Low-arousal negative, not a crisis and not validation-focused:
    # sharing a sad/exhausting experience -> disclosure (wants reflection).
    return "Disclosure", signals


def classify_intent(state: AffectiveState | None = None, text: str = "") -> IntentState:
    """Map free text (or an existing ``AffectiveState``) to an ``IntentState``."""
    if state is None:
        state = analyze(text)
    v, a, d = state.vad
    intent, signals = _classify(v, a, d, state.text)
    return IntentState(
        intent=intent,
        text=state.text,
        vad=state.vad,
        valence=v,
        arousal=a,
        dominance=d,
        plutchik=state.plutchik,
        emotion=state.emotion,
        confidence=state.confidence,
        signals=signals,
        is_crisis=(intent == "Crisis"),
    )


# Convenience one-liner reused by the data-prep / annotation scripts.
def annotate(text: str) -> dict:
    """Full annotation envelope (VAD + emotion + intent) for a user turn."""
    state = analyze(text)
    it = classify_intent(state)
    return it.to_dict()


# --- Self-test ---------------------------------------------------------------
if __name__ == "__main__":
    samples = [
        "I feel incredibly anxious and worried, my heart is racing",
        "I just got fired and I am so angry, I cannot believe they did this",
        "Everyone I know hates me and I feel like such a worthless failure",
        "I don't know what to do, should I go back to school or not",
        "I have been lonely and sad for weeks, everything feels empty",
        "What should I do about my relationship stress?",
        "I honestly keep thinking about ending it, I want to be okay",
        "Had a pretty normal day today",
    ]
    print(f"{'text':48} -> {'intent':18} V A D")
    for s in samples:
        r = annotate(s)
        v, a, d = r["valence"], r["arousal"], r["dominance"]
        print(f"{s[:46]:48} -> {r['intent']:18} {v:+.2f} {a:+.2f} {d:+.2f}")