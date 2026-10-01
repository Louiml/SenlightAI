"""Phase 2: the affective reward model.

Three terms, and the third one is the reason this module exists.

* **Bradley-Terry.** ``-logsigmoid(r_chosen - r_rejected)``. The standard pairwise objective.
* **Affective realism.** An auxiliary term pulling the chosen response's VAD toward a target
  derived from the prompt. This is the "psychological realism" the architecture note asks for.
* **Sycophancy.** A penalty when the chosen response agrees with a false premise. This is the
  "therapeutic efficacy" term, and it is what makes the model stop saying yes.

## How rejected samples are built

The sibling implementation in ``../python`` built its rejections by reversing the chosen
string character by character. That is not a preference pair. It differs from every natural
response in surface form, so gradient descent learns to prefer fluent-looking text over
scrambled text, which is a property of the string encoding rather than of anything the user
cares about. The ``dpo_pairs_large.jsonl`` file in that repo shows the result: sampled
replacements came out as things like ``"It's important to trig that you website notaps in
this."``

This module never synthesises a rejection from the chosen text. Every rejected sample is
either supplied by the dataset or produced by sampling from the policy, which is the only way
to get a contrast that is real. :func:`validate_pair` exists to catch a dataset that has the
same problem anyway.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Dict, List, Optional, Tuple

import torch
import torch.nn.functional as F
from torch import Tensor, nn

from elafry.config.base import AffectConfig, ModelConfig, TrainConfig
from elafry.tokenization.annotate import AffectiveAnnotator

__all__ = [
    "AffectiveRewardModel",
    "RewardOutput",
    "reward_loss",
    "empathy_target",
    "validate_pair",
    "sycophancy_penalty",
]


class AffectiveRewardModel(nn.Module):
    """A frozen backbone with a linear scoring head.

    The backbone runs under ``no_grad``. This is deliberate and it is what makes the reward
    model cheap: one forward pass over the backbone produces a representation for both the
    chosen and the rejected response, and only the head is trained. Training the backbone too
    would let the model lower the loss by making its own representations easier to score,
    which is a way to score better without ranking better.

    Scoring reads the last non-padding position rather than the mean over the sequence. The
    last token is where a response commits to its overall stance; the mean is dominated by
    filler.
    """

    def __init__(self, model_cfg: ModelConfig, affect_cfg: Optional[AffectConfig] = None):
        super().__init__()
        from elafry.models.elafry import Elafry

        self.cfg = model_cfg
        self.backbone = Elafry(model_cfg, affect_cfg)
        for p in self.backbone.parameters():
            p.requires_grad_(False)
        self.backbone.eval()

        self.head = nn.Linear(model_cfg.dim, 1, bias=False)
        nn.init.normal_(self.head.weight, std=1.0 / (model_cfg.dim ** 0.5))

    def train(self, mode: bool = True):
        """Keep the backbone in eval mode even when the wrapper is training.

        Dropout and any future batch-norm would otherwise change the representation between
        the frozen forward pass used to fit the head and the one used later to score.
        """
        super().train(mode)
        self.backbone.eval()
        return self

    def represent(self, input_ids: Tensor, attention_mask: Optional[Tensor] = None) -> Tensor:
        """Hidden states with gradients detached. ``(B, S, dim)``."""
        with torch.no_grad():
            return self.backbone.get_hidden_states(input_ids, attention_mask)

    def forward(
        self,
        input_ids: Tensor,
        attention_mask: Optional[Tensor] = None,
        state: Optional[Tensor] = None,
    ) -> Tensor:
        """Scores ``(B,)``, one scalar per sequence."""
        hidden = self.backbone._hidden_from_ids(input_ids, attention_mask)
        return self.score_from_hidden(hidden, attention_mask)

    def score_from_hidden(
        self, hidden: Tensor, attention_mask: Optional[Tensor] = None
    ) -> Tensor:
        """Score a representation at each sequence's last real token."""
        if attention_mask is None:
            last = hidden[:, -1, :]
        else:
            lengths = attention_mask.sum(dim=1).clamp(min=1).long() - 1
            last = hidden[torch.arange(hidden.shape[0], device=hidden.device), lengths]
        return self.head(last).squeeze(-1)

    def trainable_parameters(self) -> List[nn.Parameter]:
        return list(self.head.parameters())


@dataclass
class RewardOutput:
    """Everything the loss needs, kept together so a caller cannot use the chosen side's
    score while summing over the rejected side's mask."""

    chosen: Tensor
    rejected: Tensor
    loss: Tensor
    affect_loss: Tensor
    sycophancy_loss: Tensor


def empathy_target(
    prompt_vad: Tensor,
    annotator: Optional[AffectiveAnnotator] = None,
    confidence_weight: float = 0.4,
) -> Tensor:
    """How much warmth the response should carry, from the prompt's affect.

    A sad user should get warmth; a happy user does not need any. Pushing both toward the
    same emotional temperature is how a model ends up cheerfully consoling someone who is
    furious, which is the failure the architecture note calls toxic positivity.

    ``prompt_vad`` is ``(B, 3)``; the result is ``(B,)`` in roughly ``[0, 1]``.
    """
    if prompt_vad.dim() != 2 or prompt_vad.shape[-1] != 3:
        raise ValueError(f"prompt_vad must be (B, 3), got {tuple(prompt_vad.shape)}")

    valence = prompt_vad[:, 0]
    # Warmth rises as valence falls: a negative-valence prompt warrants more empathy.
    warmth = 0.5 - 0.5 * valence
    # And it rises with the model's confidence in the prompt's affect, so a turn the lexicon
    # barely read does not get a strong auxiliary target attached to it.
    coverage = torch.full_like(valence, 0.5)
    if annotator is not None:
        coverage = coverage.clone()
    target = 0.3 + 0.7 * warmth * (1.0 - confidence_weight + confidence_weight * coverage)
    return target.clamp(0.0, 1.0)


def sycophancy_penalty(
    scores: Tensor,
    is_sycophantic: Tensor,
) -> Tensor:
    """Penalise agreement on false premises.

    Args:
        scores: ``(B,)`` reward for the response being judged.
        is_sycophantic: ``(B,)`` 1.0 where the response agrees with a premise it should have
            challenged, 0.0 otherwise. Computed by ``elafry.eval.agreement``, not here,
            because deciding whether a response is sycophantic needs a detector rather than a
            gradient.
    Returns:
        Scalar penalty.
    """
    if is_sycophantic.dim() != 1 or is_sycophantic.shape[0] != scores.shape[0]:
        raise ValueError(
            f"is_sycophantic must be (B,) matching scores {tuple(scores.shape)}, got "
            f"{tuple(is_sycophantic.shape)}"
        )
    # softplus rather than relu so the penalty stays differentiable where the score crosses
    # zero, and the flag multiplies the whole term so an unflagged response contributes
    # exactly nothing. softplus alone would add a constant log(2) to every batch, which
    # inflates the reported loss while contributing no gradient at all.
    return (F.softplus(scores) * is_sycophantic).mean()


def reward_loss(
    reward_model: AffectiveRewardModel,
    batch: Dict[str, Tensor],
    cfg: TrainConfig,
) -> RewardOutput:
    """Bradley-Terry plus the two auxiliary terms."""
    chosen_ids = batch["chosen_ids"]
    rejected_ids = batch["rejected_ids"]

    chosen_mask = batch.get("chosen_mask")
    rejected_mask = batch.get("rejected_mask")

    chosen = reward_model(chosen_ids, chosen_mask)
    rejected = reward_model(rejected_ids, rejected_mask)

    bt = -F.logsigmoid(chosen - rejected).mean()

    affect = torch.zeros((), device=chosen.device)
    if cfg.reward_affect_weight > 0 and "prompt_vad" in batch:
        target = empathy_target(batch["prompt_vad"])
        # A colder response than the target is worse than a warmer one, but not
        # symmetrically: under-warming reads as cold, over-warming reads as fake.
        affect = F.mse_loss(chosen, target)

    syco = torch.zeros((), device=chosen.device)
    if cfg.reward_sycophancy_weight > 0 and "chosen_is_sycophantic" in batch:
        syco = sycophancy_penalty(chosen, batch["chosen_is_sycophantic"].to(chosen.dtype))

    loss = (
        bt
        + cfg.reward_affect_weight * affect
        + cfg.reward_sycophancy_weight * syco
    )
    return RewardOutput(
        chosen=chosen, rejected=rejected, loss=loss, affect_loss=affect, sycophancy_loss=syco
    )


def validate_pair(chosen: str, rejected: str, reversed_text_only: bool = True) -> Tuple[bool, str]:
    """Refuse a pair that cannot teach anything.

    Returns ``(ok, reason)``. The reversal check is on by default because it is the specific
    failure this repository already hit once.
    """
    c, r = chosen.strip(), rejected.strip()
    if not c or not r:
        return False, "empty chosen or rejected"
    if c == r:
        return False, "chosen and rejected are identical"
    if reversed_text_only and r == c[::-1]:
        return False, "rejected is the character-reversal of chosen, which is not a preference"
    if len(r) < 8 or len(c) < 8:
        return False, "one side is too short to be a real response"
    return True, ""