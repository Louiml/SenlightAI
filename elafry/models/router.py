"""Intent routing: subjective empathy mode vs objective analytical mode.

The architecture note's worked example is the sharpest statement of why this exists. Given
*"I feel completely lost and alone"*, the model should drop its own valence and meet the
user there. Given *"why do serial killers feel calm after their crimes"*, it should
**not**, because adopting a serial killer's affective state would be grotesque, and a model
that cannot tell those two apart is not a psychological interlocutor, it is a mirror.

Routing is a small classifier over the turn representation. It is deliberately shallow: the
hard part is deciding which mode applies, not learning that modes exist.

## Why crisis has its own head

Crisis detection does not compete in the argmax. It sits on a separate binary head with its
own threshold, so a turn whose most likely intent is "venting" can still be routed to
de-escalation if the crisis head fires.

An earlier version folded crisis into the same softmax and gated it on
``P(crisis) >= threshold``. That gate could never do anything: a probability at or above the
threshold implies crisis is the argmax, and a probability below it implies the gate does not
fire. Both branches land on the same answer the argmax already gave. High recall has to come
from a detector that is not competing for the same softmax, which is what a separate head is.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Dict, Tuple

import torch

from torch import Tensor, nn

__all__ = [
    "INTENTS",
    "ROUTING_MODES",
    "IntentRouter",
    "IntentDecision",
]

# Ordered by descending urgency. Crisis is checked first and overrides everything else,
# because every other mode is the wrong response to someone in acute distress.
INTENTS: Tuple[str, ...] = (
    "crisis",
    "advice_seeking",
    "seeking_validation",
    "venting",
    "disclosure",
    "casual",
    "third_party_emotion",
)

# What each intent does to generation.
ROUTING_MODES: Dict[str, str] = {
    "crisis": "de_escalate",
    "advice_seeking": "analytical",
    "seeking_validation": "reciprocity",
    "venting": "reciprocity",
    "disclosure": "reciprocity",
    "casual": "conversational",
    "third_party_emotion": "analytical",
}

# Intents that compete in the 7-way softmax. Crisis is excluded because it has its own head.
COMPETING_INTENTS: Tuple[str, ...] = tuple(i for i in INTENTS if i != "crisis")


@dataclass
class IntentDecision:
    """What the router decided, and why."""

    intent: str
    mode: str
    confidence: float
    logits: Tuple[float, ...]
    crisis_score: float = 0.0

    def to_dict(self) -> Dict[str, object]:
        return {
            "intent": self.intent,
            "mode": self.mode,
            "confidence": self.confidence,
            "crisis_score": self.crisis_score,
            "logits": list(self.logits),
        }


class IntentRouter(nn.Module):
    """Classifies a turn's intent and maps it to a generation mode.

    Two heads:

    * ``crisis_head`` is a single logit scored with a sigmoid. Fires above
      ``crisis_threshold``, independent of the argmax. Set low, since a false negative is far
      more expensive than a false positive here.
    * ``classifier`` covers the remaining six intents and is only consulted when the crisis
      head stays quiet.

    Args:
        dim: hidden width of the turn representation.
        crisis_threshold: sigmoid score above which the crisis head fires. Default 0.25 is
            deliberately permissive.
        headroom: how much room reciprocal mode has to shift the model's own valence toward
            the user's. Zero means "do not move your own state at all".
    """

    def __init__(
        self,
        dim: int,
        crisis_threshold: float = 0.25,
        headroom: float = 0.4,
    ):
        super().__init__()
        self.dim = dim
        self.crisis_threshold = crisis_threshold
        self.headroom = headroom
        self.classifier = nn.Linear(dim, len(COMPETING_INTENTS))
        self.crisis_head = nn.Linear(dim, 1)

    def forward(self, turn: Tensor) -> Tuple[Tensor, Tensor]:
        """``turn: (B, dim) -> (intent_logits (B, 6), crisis_logit (B,))``"""
        if turn.shape[-1] != self.dim:
            raise ValueError(f"expected width {self.dim}, got {turn.shape[-1]}")
        return self.classifier(turn), self.crisis_head(turn).squeeze(-1)

    @torch.no_grad()
    def decide(self, turn: Tensor) -> IntentDecision:
        """Route a single turn. Inference only; see the module docstring."""
        if turn.dim() == 1:
            turn = turn.unsqueeze(0)

        logits, crisis_logit = self.forward(turn)
        probs = torch.softmax(logits[0], dim=-1)
        crisis_score = float(torch.sigmoid(crisis_logit[0]))

        if crisis_score >= self.crisis_threshold:
            return IntentDecision(
                intent="crisis",
                mode=ROUTING_MODES["crisis"],
                confidence=crisis_score,
                logits=tuple(float(x) for x in logits[0]),
                crisis_score=crisis_score,
            )

        idx = int(torch.argmax(logits[0]))
        intent = COMPETING_INTENTS[idx]
        return IntentDecision(
            intent=intent,
            mode=ROUTING_MODES.get(intent, "conversational"),
            confidence=float(probs[idx]),
            logits=tuple(float(x) for x in logits[0]),
            crisis_score=crisis_score,
        )

    def valence_pull(self, intent: str, user_valence: Tensor) -> Tensor:
        """How far to drag the model's own valence toward the user's.

        Reciprocal intents (disclosure, venting, validation) pull. Analytical intents do
        not, which is the whole serial-killer case: the model explains the affect without
        acquiring it. Crisis suppresses the pull entirely, because a model that mirrors a
        suicide attempt's valence is making it worse.

        Args:
            intent: one of :data:`INTENTS`.
            user_valence: ``(B,)`` or ``(B, 1)``.
        Returns:
            ``(B,)`` target valence for the model's own state.
        """
        if user_valence.dim() == 2 and user_valence.shape[-1] == 1:
            user_valence = user_valence.squeeze(-1)

        if intent == "crisis":
            return torch.zeros_like(user_valence)
        if ROUTING_MODES.get(intent) == "reciprocity":
            return user_valence * self.headroom
        return torch.zeros_like(user_valence)