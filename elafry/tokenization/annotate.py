"""Turning a turn into annotated tokens.

This is where the control vocabulary gets used. The pipeline is deliberately modular: a
sentence gets classified for intent, measured for VAD, matched to a Plutchik primary, and
handed back as a token prefix. Every stage is optional and independently testable, because
a lexicon this size will be wrong sometimes and it should be possible to swap any single
stage out without touching the rest.

What this module does *not* do is compute the model's affective state. That happens inside
the model, through the attention bias. These tokens annotate the *input*; the state is
internal. Keeping the two separate is what stops the model from learning to read its own
emotion off the context window and skip the mechanism the architecture is built around.
"""

from __future__ import annotations

import math
import re
from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple

from elafry.affective.state import PLUTCHIK_PRIMARIES, PLUTCHIK_VAD
from elafry.tokenization.control_tokens import (
    INTENSITIES,
token_id,
    vad_token,
)

__all__ = [
    "TurnAnnotation",
    "AffectiveAnnotator",
    "annotate_tokens",
]

_WORD_RE = re.compile(r"[a-z']+")

# A small hand-authored lexicon: word -> (valence, arousal, dominance).
#
# Deliberately small. A 200k-word lexicon looks more rigorous and is worse here: it cannot be
# eyeballed, its coverage on conversational text is unknown, and an unseen word silently
# contributes nothing, so a turn of mostly out-of-vocabulary words averages to zero valence
# and the model reads the user as neutral. A short list you have actually checked beats a
# long one you have not. The averaging below reports how much of the turn it covered, so a
# low-coverage parse is visible rather than silent.
_LEXICON: Dict[str, Tuple[float, float, float]] = {
    # negative valence, high arousal
    "angry": (-0.7, 0.8, 0.6),
    "furious": (-0.85, 0.95, 0.75),
    "rage": (-0.85, 0.95, 0.8),
    "annoyed": (-0.4, 0.4, 0.1),
    "irritated": (-0.45, 0.45, 0.15),
    "frustrated": (-0.55, 0.55, 0.25),
    "hate": (-0.8, 0.6, 0.5),
    "resent": (-0.6, 0.45, 0.35),
    "disgusted": (-0.7, 0.4, 0.3),
    "disgusting": (-0.7, 0.4, 0.3),
    # negative valence, low arousal
    "sad": (-0.7, -0.4, -0.5),
    "sadness": (-0.7, -0.4, -0.5),
    "depressed": (-0.8, -0.6, -0.55),
    "hopeless": (-0.85, -0.5, -0.7),
    "exhausted": (-0.5, -0.75, -0.4),
    "tired": (-0.3, -0.6, -0.2),
    "lonely": (-0.65, -0.35, -0.55),
    "alone": (-0.5, -0.25, -0.45),
    "lost": (-0.55, 0.1, -0.5),
    "empty": (-0.6, -0.5, -0.4),
    "hurt": (-0.6, 0.35, -0.4),
    "pain": (-0.65, 0.35, -0.4),
    "afraid": (-0.7, 0.7, -0.7),
    "scared": (-0.7, 0.7, -0.75),
    "terrified": (-0.85, 0.9, -0.85),
    "anxious": (-0.6, 0.65, -0.6),
    "worried": (-0.5, 0.55, -0.4),
    "panic": (-0.8, 0.95, -0.75),
    "nervous": (-0.45, 0.6, -0.5),
    # positive valence
    "happy": (0.8, 0.5, 0.55),
    "joy": (0.85, 0.65, 0.6),
    "glad": (0.65, 0.4, 0.4),
    "grateful": (0.7, 0.35, 0.4),
    "thankful": (0.7, 0.35, 0.4),
    "love": (0.8, 0.5, 0.5),
    "calm": (0.5, -0.6, 0.45),
    "relaxed": (0.6, -0.65, 0.5),
    "peaceful": (0.65, -0.6, 0.55),
    "content": (0.6, -0.35, 0.45),
    "relieved": (0.55, -0.4, 0.4),
    "hopeful": (0.55, 0.3, 0.4),
    "proud": (0.65, 0.5, 0.65),
    "excited": (0.7, 0.8, 0.5),
    "surprised": (0.05, 0.85, -0.15),
    # crisis
    "suicide": (-0.95, 0.7, -0.6),
    "kill": (-0.8, 0.7, 0.4),
    "harm": (-0.8, 0.65, 0.3),
    "end": (-0.4, 0.3, 0.0),
    "overdose": (-0.85, 0.6, -0.3),
    # process
    "feel": (0.0, 0.05, 0.0),
    "feeling": (0.0, 0.05, 0.0),
    "think": (0.0, 0.05, 0.1),
    "know": (0.0, 0.0, 0.15),
    "want": (0.1, 0.25, 0.3),
    "need": (-0.1, 0.2, -0.15),
}

# Phrases that mean a crisis regardless of word-level valence. Checked before the lexicon,
# because "ending it" is two innocuous words that mean something else entirely together.
_CRISIS_PATTERNS = (
    r"end(ing)? (it all|my life|everything)",
    r"kill myself",
    r"suicid\w*",
    r"want to die",
    r"better off dead",
    r"self[- ]harm",
    r"overdose",
    r"no reason to live",
    r"can'?t go on",
)

_CRISIS_RE = re.compile("|".join(_CRISIS_PATTERNS), re.IGNORECASE)
_VALIDATION_RE = re.compile(
    r"\b(do you (agree|think)|am i (wrong|right|being)|you (agree|understand)|"
    r"tell me i'?m|validate|right to feel)\b",
    re.IGNORECASE,
)
_ADVICE_RE = re.compile(
    r"\b(what should i|how (do|can|should) i|any (advice|ideas|suggestions)|help me (figure|decide))\b",
    re.IGNORECASE,
)
# Third-party emotion is the case the architecture note singles out: a query about someone
# else's affect must not drag the model into mirroring it.
_THIRD_PARTY_RE = re.compile(
    r"\b(why (do|does|would) (serial killers|psychopaths|abusers|people|he|she|they))\b|"
    r"\b(psychopathy|sociopath|he kills|homicide)\b",
    re.IGNORECASE,
)


@dataclass
class TurnAnnotation:
    """A turn's affective parse. Every field is optional evidence, not a verdict."""

    vad: Tuple[float, float, float] = (0.0, 0.0, 0.0)
    intent: str = "casual"
    plutchik: Optional[str] = None
    plutchik_similarity: float = 0.0
    coverage: float = 0.0
    is_crisis: bool = False
    low_confidence: bool = False
    tokens: List[str] = field(default_factory=list)

    def control_prefix(self) -> List[str]:
        """The control tokens for this annotation, in the order a model should read them.

        Order matters and is not arbitrary: framing, then affect, then intent. The model
        sees which turn it is looking at before it sees what that turn feels like, and what
        it feels like before it decides what to do about it.
        """
        out = ["<|user|>", "<|affect_begin|>"]
        out += vad_token(*self.vad)
        if self.plutchik is not None:
            intensity = INTENSITIES[min(2, int(abs(self.vad[1]) * 3))]
            out.append(f"<|emo_{self.plutchik}_{intensity}|>")
        out.append(f"<|intent_{self.intent}|>")
        out.append("<|affect_end|>")
        return out

    def control_ids(self) -> List[int]:
        return [token_id(t) for t in self.tokens]


class AffectiveAnnotator:
    """Lexicon and regex based annotation of a single turn.

    Args:
        lexicon: word -> VAD overrides. Merged over the default table, so a caller can patch
            known failures without redefining the whole thing.
        min_coverage: below this fraction of words matched, the annotation is reported as
            low-confidence. It is still produced, because zero valence and "no signal" are
            different claims.
    """

    def __init__(
        self,
        lexicon: Optional[Dict[str, Tuple[float, float, float]]] = None,
        min_coverage: float = 0.02,
    ):
        self.lexicon = dict(_LEXICON)
        if lexicon:
            self.lexicon.update(lexicon)
        self.min_coverage = min_coverage

    # ------------------------------------------------------------------ VAD

    def vad(self, text: str) -> Tuple[Tuple[float, float, float], float]:
        """Mean of the matched lexicon vectors, plus the fraction of words matched."""
        words = _WORD_RE.findall(text.lower())
        if not words:
            return (0.0, 0.0, 0.0), 0.0

        matched = [self.lexicon[w] for w in words if w in self.lexicon]
        if not matched:
            return (0.0, 0.0, 0.0), 0.0

        n = len(matched)
        mean = tuple(sum(v[i] for v in matched) / n for i in range(3))
        clamped = tuple(max(-1.0, min(1.0, c)) for c in mean)
        return clamped, len(matched) / len(words)

    # ------------------------------------------------------------------ intent

    def intent(self, text: str, vad: Tuple[float, float, float]) -> str:
        """Priority-ordered classification. Crisis first, because every other label is the
        wrong answer for someone in acute distress."""
        if _CRISIS_RE.search(text):
            return "crisis"
        if _THIRD_PARTY_RE.search(text):
            return "third_party_emotion"
        if _ADVICE_RE.search(text):
            return "advice_seeking"
        if _VALIDATION_RE.search(text):
            return "seeking_validation"

        valence, arousal, _ = vad
        if valence >= 0.15 and arousal < 0.3:
            return "casual"
        if valence < 0 and arousal > 0.5:
            return "venting"
        return "disclosure"

    # ------------------------------------------------------------------ plutchik

    @staticmethod
    def plutchik(vad: Tuple[float, float, float]) -> Tuple[Optional[str], float]:
        """Nearest primary by cosine similarity, or ``None`` below threshold.

        A neutral turn gets no label. Forcing one would give the model a mood the user did
        not express.
        """
        norm = math.sqrt(sum(c * c for c in vad))
        if norm < 1e-6:
            return None, 0.0

        best_name, best_sim = None, -1.0
        for name in PLUTCHIK_PRIMARIES:
            anchor = PLUTCHIK_VAD[name]
            dot = sum(vad[i] * anchor[i] for i in range(3))
            sim = dot / (norm * math.sqrt(sum(a * a for a in anchor)))
            if sim > best_sim:
                best_name, best_sim = name, sim
        if best_sim < 0.72:
            return None, best_sim
        return best_name, best_sim

    # ------------------------------------------------------------------ full pass

    def annotate(self, text: str, with_tokens: bool = True) -> TurnAnnotation:
        if not text or not text.strip():
            return TurnAnnotation(tokens=["<|user|>", "<|affect_begin|>",
                                          "<|vad_v_z|>", "<|vad_a_z|>", "<|vad_d_z|>",
                                          "<|intent_casual|>", "<|affect_end|>"])

        vad, coverage = self.vad(text)
        is_crisis = _CRISIS_RE.search(text) is not None
        intent = self.intent(text, vad)
        label, sim = self.plutchik(vad)

        # Below the coverage floor the parse is not evidence. A Plutchik label derived from
        # one matched word in a long turn is a coin flip dressed up as a measurement, and
        # nothing downstream can tell the difference. Dropping the label is the honest
        # answer. The intent classification still runs, because it leans on phrase patterns
        # that match when word-level valence does not.
        low_confidence = coverage < self.min_coverage
        if low_confidence:
            label, sim = None, 0.0

        ann = TurnAnnotation(
            vad=vad,
            intent=intent,
            plutchik=label,
            plutchik_similarity=sim,
            coverage=coverage,
            is_crisis=is_crisis,
            low_confidence=low_confidence,
        )
        if with_tokens:
            ann.tokens = ann.control_prefix()
        return ann


def annotate_tokens(text: str, annotator: Optional[AffectiveAnnotator] = None) -> List[int]:
    """Convenience: annotate a turn and return control token ids."""
    return (annotator or AffectiveAnnotator()).annotate(text).control_ids()