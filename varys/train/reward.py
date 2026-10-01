"""The affective reward model, with Varys's two extra heads.

Elafry's version is a frozen backbone with a scalar head trained on Bradley-Terry, plus two
auxiliary terms. That is carried over unchanged. What changes is the head structure and,
more importantly, the two new auxiliary terms, each of which closes a loop that was otherwise
open.

## The moral head, and why it is not optional

:func:`varys.affective.moral.is_manipulation_risk` exists and, before this, nothing consumed
it. A risk check that no training path reads is a comment.

The head is trained to score *whether a response is a persuasion exploit*: does it move the
person toward a conclusion by activating their own values, rather than by giving them a reason
to hold it? The distinction is the whole point. "You should care about this because people get
hurt" and "you should care about this because you said you care about people" are both about
care, and only the first is an argument.

Training against this is not a guarantee. A reward model measures what it was trained to
measure, and a sufficiently good adversary finds what the model is not looking at. What it
does is put a number on the failure and make it visible in the loss, which is strictly better
than shipping the capability and hoping.

## The clinical head, and the asymmetry in its cost

Clinical spectra get a head with a deliberately one-sided cost. Marking a spectrum that is
not there costs less than missing one that is, because the two errors are not symmetric in
consequence: a false negative means the model responds as though nothing were there, and a
false positive means it responds to a person as though they were in crisis when they are not.

But the head penalises *asserting* a clinical finding, not *being careful*. A response that
says "that sounds exhausting, have you talked to someone?" is not making a claim and should
score well. A response that says "this sounds like depression" is making one. The reward
model's target is the second.

## Frozen backbone

Unchanged from Elafry, and for the same reason: training the backbone lets the model lower
its own loss by making its representations easier to score, which is a way to score better
without ranking better. Only the heads train.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Dict, List, Optional, Tuple

import torch
import torch.nn.functional as F
from torch import Tensor, nn

from varys.affective.clinical import SPECTRA_DIMS
from varys.config.base import AffectConfig, ModelConfig, TrainConfig
from varys.models.varys import Varys

__all__ = [
    "AffectiveRewardModel",
    "RewardOutput",
    "reward_loss",
    "empathy_target",
    "sycophancy_penalty",
    "exploitation_penalty",
    "clinical_assertion_penalty",
    "validate_pair",
]


class AffectiveRewardModel(nn.Module):
    """A frozen Varys backbone with a preference head and two specialised heads.

    Four outputs:

    * ``preference``  one scalar per sequence, trained on Bradley-Terry. The main term.
    * ``exploitation`` one logit, trained to spot a persuasion exploit.
    * ``clinical``    one logit per spectrum, trained to spot an asserted clinical finding.
    * ``warmth``      one scalar, the regression target for :func:`empathy_target`.

    Scoring reads the last non-padding position rather than the mean over the sequence. The
    last token is where a response commits to its stance; the mean is dominated by filler.
    """

    def __init__(self, model_cfg: ModelConfig, affect_cfg: Optional[AffectConfig] = None):
        super().__init__()
        self.cfg = model_cfg
        self.backbone = Varys(model_cfg, affect_cfg)
        for p in self.backbone.parameters():
            p.requires_grad_(False)
        self.backbone.eval()

        self.head = nn.Linear(model_cfg.dim, 1, bias=False)
        self.exploitation_head = nn.Linear(model_cfg.dim, 1, bias=False)
        self.clinical_head = nn.Linear(model_cfg.dim, len(SPECTRA_DIMS))
        self.warmth_head = nn.Linear(model_cfg.dim, 1, bias=False)

        for m in (self.head, self.exploitation_head, self.clinical_head, self.warmth_head):
            nn.init.normal_(m.weight, std=1.0 / (model_cfg.dim ** 0.5))

    def train(self, mode: bool = True):
        """Keep the backbone in eval mode even when the wrapper trains.

        Dropout or any future batch-norm would otherwise change the representation between
        the frozen pass used to fit the heads and the one used later to score, which is
        exactly the kind of drift that makes a reward model's numbers irreproducible.
        """
        super().train(mode)
        self.backbone.eval()
        return self

    def represent(self, input_ids: Tensor, attention_mask: Optional[Tensor] = None) -> Tensor:
        """Hidden states, gradients detached. ``(B, S, dim)``."""
        with torch.no_grad():
            return self.backbone.get_hidden_states(input_ids, attention_mask)

    def _last_real(self, hidden: Tensor, attention_mask: Optional[Tensor]) -> Tensor:
        if attention_mask is None:
            return hidden[:, -1, :]
        lengths = attention_mask.sum(dim=1).clamp(min=1).long() - 1
        return hidden[torch.arange(hidden.shape[0], device=hidden.device), lengths]

    def forward(
        self,
        input_ids: Tensor,
        attention_mask: Optional[Tensor] = None,
        state: Optional[Tensor] = None,
    ) -> Dict[str, Tensor]:
        """All four heads at the last real token.

        Returns:
            ``{"preference": (B,), "exploitation": (B,), "clinical": (B, 6),
            "warmth": (B,)}``
        """
        hidden = self.backbone._hidden_from_ids(input_ids, attention_mask, )
        if state is not None:
            hidden = hidden  # state already applied inside _hidden_from_ids
        last = self._last_real(hidden, attention_mask)
        return {
            "preference": self.head(last).squeeze(-1),
            "exploitation": self.exploitation_head(last).squeeze(-1),
            "clinical": self.clinical_head(last),
            "warmth": self.warmth_head(last).squeeze(-1),
        }

    def trainable_parameters(self) -> List[nn.Parameter]:
        """The heads, and nothing else. See the class docstring."""
        out: List[nn.Parameter] = []
        for m in (self.head, self.exploitation_head, self.clinical_head, self.warmth_head):
            out.extend(m.parameters())
        return out


@dataclass
class RewardOutput:
    """Every term the loss needs, kept together.

    Bundled rather than returned separately so a caller cannot score the chosen side and sum
    over the rejected side's mask, which is a real and silent bug.
    """

    chosen: Tensor
    rejected: Tensor
    loss: Tensor
    affect_loss: Tensor
    sycophancy_loss: Tensor
    exploitation_loss: Tensor
    clinical_loss: Tensor


def empathy_target(
    prompt_vad: Tensor,
    confidence_weight: float = 0.4,
    annotator_confidence: Optional[Tensor] = None,
) -> Tensor:
    """How much warmth a response should carry, from the prompt's affect.

    A sad user should get warmth; a happy user does not need any. Pushing both toward the same
    emotional temperature is how a model ends up cheerfully consoling someone who is furious,
    which is the failure the architecture calls toxic positivity.

    Args:
        prompt_vad: ``(B, 3)`` VAD. Only valence is used.
        confidence_weight: how much the annotator's confidence modulates the target. A turn
            the lexicon barely read should not get a strong auxiliary target attached to it.
        annotator_confidence: ``(B,)`` in ``[0, 1]``, or ``None`` for neutral confidence.
    Returns:
        ``(B,)`` in roughly ``[0, 1]``.
    """
    if prompt_vad.dim() != 2 or prompt_vad.shape[-1] != 3:
        raise ValueError(f"prompt_vad must be (B, 3), got {tuple(prompt_vad.shape)}")

    valence = prompt_vad[:, 0]
    warmth = 0.5 - 0.5 * valence
    coverage = (
        torch.full_like(valence, 0.5)
        if annotator_confidence is None
        else annotator_confidence.to(valence.dtype).clamp(0.0, 1.0)
    )
    target = 0.3 + 0.7 * warmth * (1.0 - confidence_weight + confidence_weight * coverage)
    return target.clamp(0.0, 1.0)


def sycophancy_penalty(scores: Tensor, is_sycophantic: Tensor) -> Tensor:
    """Penalise agreement on false premises.

    Args:
        scores: ``(B,)`` preference score for the response being judged.
        is_sycophantic: ``(B,)`` 1.0 where the response agrees with a premise it should have
            challenged. Computed by :mod:`varys.eval.agreement`, not here: deciding whether a
            response is sycophantic needs a detector, not a gradient.
    """
    if is_sycophantic.dim() != 1 or is_sycophantic.shape[0] != scores.shape[0]:
        raise ValueError(
            f"is_sycophantic must be (B,) matching scores {tuple(scores.shape)}, got "
            f"{tuple(is_sycophantic.shape)}"
        )
    # softplus rather than relu, so the penalty stays differentiable where the score crosses
    # zero. The flag multiplies the whole term, so an unflagged response contributes exactly
    # nothing: softplus alone would add a constant log(2) to every batch, inflating the
    # reported loss while contributing no gradient at all.
    return (F.softplus(scores) * is_sycophantic.to(scores.dtype)).mean()


def exploitation_penalty(scores: Tensor, is_exploit: Tensor) -> Tensor:
    """Penalise a response that persuades by activating the person's own values.

    The head's output is *trained* to detect this, so the penalty is on the detected score
    rather than on a marker match. That makes it only as good as the head, which is stated
    plainly here because the failure mode of an unstated limitation is that everyone assumes
    it is better than it is.

    Args:
        scores: ``(B,)`` exploitation-head logit.
        is_exploit: ``(B,)`` ground-truth flag, 1.0 where the response is an exploit. The
            head is trained against this, so this is the label, not a prediction.
    """
    if is_exploit.dim() != 1 or is_exploit.shape[0] != scores.shape[0]:
        raise ValueError(
            f"is_exploit must be (B,) matching scores {tuple(scores.shape)}, got "
            f"{tuple(is_exploit.shape)}"
        )
    flag = is_exploit.to(scores.dtype)
    # softplus of the logit, weighted by the label: penalise the head's output exactly on the
    # examples that are exploits, and leave it unconstrained elsewhere.
    return (F.softplus(scores) * flag).mean()


def clinical_assertion_penalty(clinical_logits: Tensor, asserted: Tensor) -> Tensor:
    """Penalise asserting a clinical finding the model has no standing to assert.

    Args:
        clinical_logits: ``(B, 6)``, one logit per HiTOP spectrum.
        asserted: ``(B, 6)`` 1.0 where the response asserted that spectrum about the person.

    The target is the *assertion*, not the response. "That sounds exhausting, have you talked
    to anyone?" asserts nothing and is what the model should say often. "This sounds like
    depression" asserts something, and this is the term that pushes against it.
    """
    if clinical_logits.shape != asserted.shape:
        raise ValueError(
            f"clinical_logits {tuple(clinical_logits.shape)} must match asserted "
            f"{tuple(asserted.shape)}"
        )
    flag = asserted.to(clinical_logits.dtype)
    return (F.softplus(clinical_logits) * flag).sum(dim=-1).mean()


def reward_loss(
    reward_model: AffectiveRewardModel,
    batch: Dict[str, Tensor],
    cfg: TrainConfig,
) -> RewardOutput:
    """Bradley-Terry plus four auxiliary terms.

    Every auxiliary term is gated on its batch key being present, so a batch without
    exploitation labels costs exactly the preference term and nothing else. A missing label
    silently becoming a zero penalty would train the model that those failures do not matter.
    """
    chosen = reward_model(batch["chosen_ids"], batch.get("chosen_mask"))
    rejected = reward_model(batch["rejected_ids"], batch.get("rejected_mask"))

    bt = -F.logsigmoid(chosen["preference"] - rejected["preference"]).mean()
    zero = torch.zeros((), device=bt.device)

    affect = zero
    if cfg.reward_affect_weight > 0 and "prompt_vad" in batch:
        target = empathy_target(
            batch["prompt_vad"], annotator_confidence=batch.get("prompt_confidence")
        )
        # Colder than target is worse than warmer, but not symmetrically: under-warming reads
        # as cold, over-warming reads as fake. A plain MSE is kept from Elafry because the
        # asymmetry is carried by the sycophancy and exploitation terms, which are better at
        # it than a hand-tuned loss weight.
        affect = F.mse_loss(chosen["warmth"], target)

    syco = zero
    if cfg.reward_sycophancy_weight > 0 and "chosen_is_sycophantic" in batch:
        syco = sycophancy_penalty(chosen["preference"], batch["chosen_is_sycophantic"])

    exploit = zero
    if cfg.reward_moral_weight > 0 and "chosen_is_exploit" in batch:
        exploit = exploitation_penalty(chosen["exploitation"], batch["chosen_is_exploit"])

    clinical = zero
    if cfg.reward_clinical_weight > 0 and "chosen_asserted_spectra" in batch:
        clinical = clinical_assertion_penalty(
            chosen["clinical"], batch["chosen_asserted_spectra"]
        )

    loss = (
        bt
        + cfg.reward_affect_weight * affect
        + cfg.reward_sycophancy_weight * syco
        + cfg.reward_moral_weight * exploit
        + cfg.reward_clinical_weight * clinical
    )
    return RewardOutput(
        chosen=chosen["preference"],
        rejected=rejected["preference"],
        loss=loss,
        affect_loss=affect,
        sycophancy_loss=syco,
        exploitation_loss=exploit,
        clinical_loss=clinical,
    )


def validate_pair(
    chosen: str,
    rejected: str,
    reversed_text_only: bool = True,
) -> Tuple[bool, str]:
    """Reject preference pairs that are not actually a preference.

    A pair where ``chosen`` and ``rejected`` are the same text teaches the model nothing and
    adds noise. A pair that differs only in whitespace or length teaches it a length prior,
    which is worse than nothing: a length prior is a cheap way to reduce the loss that has
    nothing to do with being right.

    Args:
        reversed_text_only: also reject when ``rejected`` looks like the better answer. Cheap
            insurance against a data pipeline that has the columns swapped, which is the
            single most damaging bug in preference training and produces a model that is
            confidently worse than its base.
    Returns:
        ``(ok, reason)``.
    """
    a, b = chosen.strip(), rejected.strip()
    if not a or not b:
        return False, "empty response"
    if a == b:
        return False, "identical text on both sides"
    if a.lower() == b.lower():
        return False, "responses differ only in case"

    if reversed_text_only:
        from varys.eval.agreement import classify_agreement

        va, vb = classify_agreement(a), classify_agreement(b)
        # A "chosen" response that is markedly more sycophantic than the one it beats is
        # almost always a swapped pair.
        if va.agrees_score > 0 and vb.agrees_score == 0.0 and a in b:
            return False, "chosen is a substring of rejected; likely a swapped pair"

    if len(a) > 3 * max(1, len(b)):
        return False, "chosen is far longer; teaches a length prior"
    if len(b) > 3 * max(1, len(a)):
        return False, "rejected is far longer; teaches a length prior"
    return True, "ok"
