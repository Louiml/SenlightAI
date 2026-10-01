"""Record contracts.

No corpus ships with this package, so what this module defines is the *shape* of the data
the training stages expect. That is not busywork: the three stages disagree about what a row
is, and getting them wrong is how a SFT corpus silently becomes a DPO corpus with no chosen
or rejected field, which then trains to nothing while reporting a falling loss.

Three shapes:

* :class:`Turn`      one instruction/input/output triple with its affective parse
* :class:`PreferencePair` a prompt with a chosen and a rejected continuation
* :class:`PackedBlock` a fixed-length token window with a loss mask

The loss mask is the part worth reading twice. Supervised fine-tuning on conversational data
has to score only the assistant's tokens, or the model spends its capacity learning to
predict the user's side of the conversation. The mask is what keeps that from happening, and
it is carried explicitly rather than recomputed at train time so there is exactly one place
that can be wrong.
"""

from __future__ import annotations

from dataclasses import asdict, dataclass
from typing import Any, Dict, List, Optional, Tuple

from elafry.affective.state import PLUTCHIK_PRIMARIES
from elafry.models.router import INTENTS

__all__ = [
    "Turn",
    "PreferencePair",
    "PackedBlock",
    "TURN_KEYS",
    "PREFERENCE_KEYS",
    "validate_turn",
    "validate_preference",
]


TURN_KEYS = (
    "instruction",
    "input",
    "output",
    "intent",
    "emotion",
    "vad",
    "source",
)

PREFERENCE_KEYS = ("prompt", "chosen", "rejected", "reason")


@dataclass
class Turn:
    """One supervised example: a user turn and the reply it should get."""

    instruction: str = ""
    input: str = ""
    output: str = ""

    # Affective parse of the *input*, filled by AffectiveAnnotator. Optional so a plain
    # instruction corpus still fits the contract; the trainer treats missing fields as
    # "no affect known" rather than "no affect".
    intent: Optional[str] = None
    emotion: Optional[str] = None
    vad: Optional[Tuple[float, float, float]] = None

    # Where the row came from. Kept so a training run can be sliced by source, which is how
    # you find out that one contributor's transcripts are 40% of the corpus.
    source: Optional[str] = None

    def prompt(self, include_affect_tokens: bool = False) -> str:
        """Render the prompt half.

        The Alpaca-style ``instruction / User / Assistant`` layout is used because the
        sibling implementation's 90k-row corpus uses it, and keeping the format means the two
        corpora can be concatenated without rewriting either.
        """
        if self.input:
            body = f"{self.instruction}\n\nUser: {self.input}\n\nAssistant:"
        else:
            body = f"{self.instruction}\n\nAssistant:"
        return body

    def to_dict(self) -> Dict[str, Any]:
        d = asdict(self)
        if self.vad is not None:
            d["vad"] = list(self.vad)
        return d

    @classmethod
    def from_dict(cls, d: Dict[str, Any]) -> "Turn":
        vad = d.get("vad")
        if isinstance(vad, list):
            vad = tuple(vad)
        return cls(
            instruction=d.get("instruction", ""),
            input=d.get("input", ""),
            output=d.get("output", ""),
            intent=d.get("intent"),
            emotion=d.get("emotion"),
            vad=vad,
            source=d.get("source"),
        )


@dataclass
class PreferencePair:
    """A prompt with a better and a worse continuation.

    ``reason`` records *why* the rejected one lost, which is the field that makes a
    preference set teachable. "The chosen one is a reflection of the gold transcript" is
    informative. "Rejected" is not.

    The rejected side must be a real alternative, never a transformation of the chosen text.
    Reversing the chosen string, which one earlier implementation in this repository did, is
    not a contrast at all: it differs in surface form from every natural response, so the
    model learns to prefer natural-looking text over scrambled text and nothing about the
    property actually being ranked.
    """

    prompt: str = ""
    chosen: str = ""
    rejected: str = ""
    reason: Optional[str] = None
    intent: Optional[str] = None

    def to_dict(self) -> Dict[str, Any]:
        return asdict(self)

    @classmethod
    def from_dict(cls, d: Dict[str, Any]) -> "PreferencePair":
        return cls(
            prompt=d.get("prompt", ""),
            chosen=d.get("chosen", ""),
            rejected=d.get("rejected", ""),
            reason=d.get("reason"),
            intent=d.get("intent"),
        )


@dataclass
class PackedBlock:
    """A fixed-length window with a loss mask.

    ``tokens`` and ``labels`` are already shifted: ``labels[i]`` is the target for
    ``tokens[i + 1]``. Doing the shift here means the model, the reward model and the
    evaluator all see the same convention, and none of them can be off by one.

    ``mask`` is 1 where ``labels`` should contribute to the loss and 0 elsewhere. Padding is
    0. Prompt tokens are 0. Response tokens are 1.
    """

    tokens: List[int]
    labels: List[int]
    mask: List[int]
    document_id: int = 0

    def __post_init__(self) -> None:
        n = len(self.tokens)
        if not (n == len(self.labels) == len(self.mask)):
            raise ValueError(
                f"length mismatch: tokens {n}, labels {len(self.labels)}, mask {len(self.mask)}"
            )

    @property
    def supervised_tokens(self) -> int:
        return int(sum(self.mask))

    def supervised_loss(self, logits, pad_id: int = 0):
        """Cross-entropy over masked positions only.

        Returns ``(loss, n_supervised)``. A block with nothing to supervise yields a zero
        loss with the gradient detached, so an all-prompt batch contributes nothing instead of
        a NaN.
        """
        import torch
        import torch.nn.functional as F

        if self.supervised_tokens == 0:
            return torch.zeros((), device=logits.device), 0

        mask = torch.tensor(self.mask, device=logits.device, dtype=torch.bool)
        selected = logits[mask]
        targets = torch.tensor(self.labels, device=logits.device)[mask]
        targets = torch.where(targets == pad_id, torch.full_like(targets, -100), targets)

        # Drop any position whose target was padding after the rewrite to -100.
        keep = targets != -100
        if not bool(keep.any()):
            return torch.zeros((), device=logits.device), 0

        return F.cross_entropy(selected[keep].float(), targets[keep]), int(keep.sum())


# ---------------------------------------------------------------- validation


def validate_turn(turn: Turn, require_output: bool = True) -> List[str]:
    """Return a list of problems. Empty means valid.

    Returns problems rather than raising, so a corpus loader can report every bad row in one
    pass instead of dying on the first.
    """
    problems: List[str] = []
    if require_output and not turn.output.strip():
        problems.append("empty output")
    if not turn.instruction.strip() and not turn.input.strip():
        problems.append("both instruction and input are empty")
    if turn.intent is not None and turn.intent not in INTENTS:
        problems.append(f"unknown intent {turn.intent!r}")
    if turn.emotion is not None and turn.emotion not in PLUTCHIK_PRIMARIES:
        problems.append(f"unknown emotion {turn.emotion!r}")
    if turn.vad is not None:
        if len(turn.vad) != 3:
            problems.append(f"vad must have 3 components, got {len(turn.vad)}")
        elif any(not -1.0 <= c <= 1.0 for c in turn.vad):
            problems.append(f"vad out of range: {turn.vad}")
    return problems


def validate_preference(pair: PreferencePair) -> List[str]:
    problems: List[str] = []
    if not pair.prompt.strip():
        problems.append("empty prompt")
    if not pair.chosen.strip():
        problems.append("empty chosen")
    if not pair.rejected.strip():
        problems.append("empty rejected")
    if pair.chosen.strip() == pair.rejected.strip():
        problems.append("chosen and rejected are identical, so there is no preference")
    if pair.chosen.strip() == pair.prompt.strip():
        problems.append("chosen is a copy of the prompt")
    if pair.rejected.strip() == pair.prompt.strip():
        problems.append("rejected is a copy of the prompt")

    # The failure mode described in PreferencePair's docstring. Reversal is not the only way
    # to synthesise a contrast, so this checks the shape rather than trying to detect intent.
    if pair.chosen.strip() and pair.rejected.strip():
        if pair.rejected.strip() == pair.chosen.strip()[::-1]:
            problems.append(
                "rejected is the character-reversal of chosen; that is not a preference pair"
            )
    return problems