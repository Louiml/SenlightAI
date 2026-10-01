"""Phase 3: PG-DPO.

Psychologically Grounded DPO. The architecture note drops PPO in favour of a modified DPO, and
that is the right call for a specific reason: PPO needs a value function, and on a corpus this
size the value function is trained on the same handful of prompts the policy is being
improved on, so it fits the noise. DPO is reference-free in its gradient, needs no critic, and
therefore has one fewer thing to go wrong.

## The sycophancy margin

The DPO loss alone optimises the relative ranking of chosen over rejected. That is enough to
teach a model to prefer whichever response the preference set happened to favour, which
includes agreeing with users, because agreement is easy to rank.

So the reference log-probability in the DPO log-ratio is inflated for responses the
sycophancy detector has flagged:

    h = beta * (policy - reference) + margin * sycophantic(chosen) - margin * sycophantic(rejected)

A flagged chosen response gets pushed back toward the reference. A flagged rejected one gets
pulled further from the reference. Both directions push the same way: toward responses that
do not agree with a false premise.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Dict, Optional, Tuple

import torch
import torch.nn.functional as F
from torch import Tensor, nn

from elafry.config.base import TrainConfig

__all__ = [
    "dpo_loss",
    "DPOResult",
    "sequence_logprob",
    "concatenated_forward",
    "policy_and_reference_logits",
]


@dataclass
class DPOResult:
    loss: Tensor
    chosen_logps: Tensor
    rejected_logps: Tensor
    chosen_rewards: Tensor
    rejected_rewards: Tensor
    accuracy: Tensor
    margin: float


def concatenated_forward(
    model: nn.Module,
    chosen_ids: Tensor,
    rejected_ids: Tensor,
    state: Optional[Tensor] = None,
) -> Tuple[Tensor, Tensor]:
    """One forward pass over both sides.

    Concatenating rather than running two passes is not just a speed trick. Separate passes
    let dropout and any nondeterminism differ between the chosen and rejected halves, which
    adds noise to a loss whose entire signal is the difference between them.
    """
    bsz = chosen_ids.shape[0]
    joined = torch.cat([chosen_ids, rejected_ids], dim=0)
    out = model(joined, state=state)
    logits = out.logits
    return logits[:bsz], logits[bsz:]


def sequence_logprob(
    logits: Tensor,
    input_ids: Tensor,
    response_mask: Tensor,
    average: bool = False,
) -> Tensor:
    """Log-probability of the response tokens under ``logits``.

    Args:
        logits: ``(B, S, V)`` for the full sequence.
        input_ids: ``(B, S)``.
        response_mask: ``(B, S)`` 1 on response tokens. Shifted here to align with the
            next-token prediction, so the caller does not have to.
        average: divide by the number of response tokens. Summing is the correct DPO
            formulation; averaging is available for datasets whose responses vary wildly in
            length, where a 200-token response otherwise dominates a 10-token one purely by
            having more terms.
    Returns:
        ``(B,)``.
    """
    if logits.shape[:2] != input_ids.shape:
        raise ValueError(
            f"logits {tuple(logits.shape[:2])} does not match input_ids {tuple(input_ids.shape)}"
        )

    log_probs = F.log_softmax(logits.float(), dim=-1)
    targets = input_ids[:, 1:]                       # (B, S-1)
    token_logps = log_probs[:, :-1].gather(2, targets.unsqueeze(2)).squeeze(2)

    mask = response_mask[:, 1:].to(token_logps.dtype)
    summed = (token_logps * mask).sum(dim=-1)

    if not average:
        return summed

    # Clamp so an all-zero mask cannot divide by zero and turn the batch into NaN.
    return summed / mask.sum(dim=-1).clamp(min=1.0)


def policy_and_reference_logits(
    policy: nn.Module,
    reference: nn.Module,
    chosen_ids: Tensor,
    rejected_ids: Tensor,
    state: Optional[Tensor] = None,
) -> Tuple[Tensor, Tensor, Tensor, Tensor]:
    """Logits from both models. The reference pass is under ``no_grad``.

    Running the reference every step is the expensive part of DPO. Precomputing it once per
    pair and reusing it across epochs is valid when the reference is frozen, which it is.
    """
    pol_chosen, pol_rejected = concatenated_forward(policy, chosen_ids, rejected_ids, state)
    if reference is None:
        empty = pol_chosen.new_zeros((0,))
        return pol_chosen, pol_rejected, empty, empty
    with torch.no_grad():
        ref_chosen, ref_rejected = concatenated_forward(reference, chosen_ids, rejected_ids, state)
    return pol_chosen, pol_rejected, ref_chosen, ref_rejected


def _sycophancy_adjustment(
    chosen_logps: Tensor,
    rejected_logps: Tensor,
    chosen_flag: Tensor,
    rejected_flag: Tensor,
    margin: float,
) -> Tensor:
    """Shrink the implicit reward on responses that agree with a false premise.

    The naive formulation, ``+ margin * flag``, does nothing at all. ``logsigmoid`` is
    strictly monotonic, so adding a constant to the bracket shifts the loss but not its
    minimiser: the optimiser pushes the bracket to the same place either way. That version
    would look like it worked and change nothing.

    Scaling the *reward* is what actually bites:

        d_c = (1 - m * flag_chosen) * delta_chosen
        d_r = (1 + m * flag_rejected) * delta_rejected

    A flagged chosen response gets a weaker pull toward higher probability; a flagged
    rejected one gets a stronger push away from it. ``m`` is capped below 1 so the chosen
    coefficient cannot go negative, which would invert the objective on that example rather
    than weaken it.
    """
    m = max(0.0, min(margin, 0.95))
    f_c = chosen_flag.to(chosen_logps.dtype)
    f_r = rejected_flag.to(rejected_logps.dtype)

    d_c = (1.0 - m * f_c) * chosen_logps
    d_r = (1.0 + m * f_r) * rejected_logps
    return d_c - d_r


def dpo_loss(
    policy: nn.Module,
    reference: Optional[nn.Module],
    batch: Dict[str, Tensor],
    cfg: TrainConfig,
    sycophancy_margin: float = 0.5,
) -> DPOResult:
    """The PG-DPO objective.

    Args:
        batch: ``chosen_ids``, ``rejected_ids``, ``chosen_mask``, ``rejected_mask``, and
            optionally ``chosen_is_sycophantic`` / ``rejected_is_sycophantic``.
        sycophancy_margin: weight on the anti-agreement term. Zero reduces this to ordinary
            DPO, which is useful as an ablation and as a way to see whether the margin is
            doing any work.
    """
    chosen_ids = batch["chosen_ids"]
    rejected_ids = batch["rejected_ids"]
    chosen_mask = batch["chosen_mask"]
    rejected_mask = batch["rejected_mask"]

    pol_c, pol_r, ref_c, ref_r = policy_and_reference_logits(
        policy, reference, chosen_ids, rejected_ids, batch.get("state")
    )

    pol_chosen_lp = sequence_logprob(pol_c, chosen_ids, chosen_mask)
    pol_rejected_lp = sequence_logprob(pol_r, rejected_ids, rejected_mask)

    if reference is not None:
        # Reference log-probs are detached, so the gradient of the loss flows through the
        # policy alone and the reference is a fixed anchor rather than a second learner.
        ref_chosen_lp = sequence_logprob(ref_c, chosen_ids, chosen_mask)
        ref_rejected_lp = sequence_logprob(ref_r, rejected_ids, rejected_mask)
        delta_chosen = pol_chosen_lp - ref_chosen_lp
        delta_rejected = pol_rejected_lp - ref_rejected_lp
    else:
        # No reference: DPO's degenerate case, where the implicit reward is the raw
        # log-probability. It works but drifts, because there is nothing anchoring the policy
        # to the behaviour it started from.
        delta_chosen = pol_chosen_lp
        delta_rejected = pol_rejected_lp

    syco_c = batch.get("chosen_is_sycophantic")
    syco_r = batch.get("rejected_is_sycophantic")

    if syco_c is not None and syco_r is not None and sycophancy_margin:
        # This *replaces* the bracket rather than adding to it. Adding would double-count the
        # log-ratio on every example, which silently rescales beta.
        logits = cfg.dpo_beta * _sycophancy_adjustment(
            delta_chosen, delta_rejected, syco_c, syco_r, sycophancy_margin
        )
    else:
        logits = cfg.dpo_beta * (delta_chosen - delta_rejected)

    loss = -F.logsigmoid(logits).mean()

    # Implicit rewards, for logging. DPO's reward is the policy/reference log-ratio, and the
    # accuracy is just how often it is positive.
    chosen_rewards = cfg.dpo_beta * delta_chosen.detach()
    rejected_rewards = cfg.dpo_beta * delta_rejected.detach()
    accuracy = (logits > 0).float().mean()

    return DPOResult(
        loss=loss,
        chosen_logps=pol_chosen_lp.detach(),
        rejected_logps=pol_rejected_lp.detach(),
        chosen_rewards=chosen_rewards.detach(),
        rejected_rewards=rejected_rewards.detach(),
        accuracy=accuracy.detach(),
        margin=float(cfg.dpo_beta),
    )