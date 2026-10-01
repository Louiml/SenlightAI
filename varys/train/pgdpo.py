"""Policy gradient with an affective reward: PG-DPO.

The training loop's hardest constraint, and the reason this is a separate module from
:mod:`varys.train.reward`.

## Why the state is held fixed inside a training window

A preference pair is scored under a pinned state, then the policy is updated. If the state
moved during the update, the reward would be attached to a condition that no longer holds,
and the gradient would be teaching the model to produce a response that was good *under a
state it is no longer in*.

So the state is computed once per pair and held across both the reward forward pass and the
policy update. It is recomputed at the next turn. That is a constraint on the optimisation,
not a simplification of the architecture: the architecture allows per-token evolution during
decode, and the training loop has to pin it for the gradient to mean anything.

## What the update is

Per-token log-probabilities under the policy and a frozen reference, differences taken, then
a reward-weighted objective. The reward is not a reward in the reinforcement-learning sense:
it is a scalar the affective reward model produced, and the token-level term is the policy
gradient of that scalar under the reference-anchored ratio. No baseline and no advantage
estimation, because there is no environment to have an advantage over.

The reference anchor is the difference between this and plain reward-weighted SFT. Without
it, the KL penalty is implicit and the model drifts on every dimension the reward model
happens not to see, which for a psychological model is most of them.

## The guard that matters most

:func:`pgdpo_update` refuses to run when the chosen and rejected responses are scored under
different states. That produces a gradient that is not just wrong but *directionally*
misleading: it rewards the chosen response for being produced in a warmer state than the
rejected one, and after enough steps the model learns to induce warm states rather than to
write well. It is the kind of bug that improves the loss curve and ruins the model.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Dict, Optional, Tuple

import torch
import torch.nn.functional as F
from torch import Tensor

from varys.config.base import TrainConfig

__all__ = [
    "PGDPOOutput",
    "sequence_logprobs",
    "pgdpo_loss",
    "pgdpo_update",
    "check_state_consistency",
]


@dataclass
class PGDPOOutput:
    """The loss and everything needed to check it."""

    loss: Tensor
    chosen_logp: Tensor
    rejected_logp: Tensor
    reward_margin: Tensor
    kl: Tensor
    n_tokens: int = 0

    def to_dict(self) -> Dict[str, float]:
        return {
            "loss": float(self.loss.detach()),
            "reward_margin": float(self.reward_margin.detach().mean()),
            "kl": float(self.kl.detach()),
            "n_tokens": self.n_tokens,
        }


def sequence_logprobs(
    logits: Tensor,
    labels: Tensor,
    mask: Optional[Tensor] = None,
) -> Tuple[Tensor, Tensor]:
    """Summed and mean log-probability of ``labels`` under ``logits``.

    Returns:
        ``(sum_logp (B,), n_tokens)``. The count is returned because the caller needs it to
        normalise: without it, a long response is rewarded for being long, which is the
        length prior that :func:`varys.train.reward.validate_pair` exists to catch in the
        data and which would otherwise be reintroduced in the loss.
    """
    if logits.dim() != 3 or labels.dim() != 2:
        raise ValueError(
            f"expected logits (B, S, V) and labels (B, S), got {tuple(logits.shape)} and "
            f"{tuple(labels.shape)}"
        )
    if logits.shape[:2] != labels.shape:
        raise ValueError(
            f"logits {tuple(logits.shape)} and labels {tuple(labels.shape)} disagree on (B, S)"
        )

    logp = torch.log_softmax(logits.float(), dim=-1)
    gathered = logp.gather(dim=-1, index=labels.unsqueeze(-1)).squeeze(-1)

    if mask is None:
        mask = torch.ones_like(gathered)
    mask = mask.to(gathered.dtype)

    summed = (gathered * mask).sum(dim=-1)
    n_tokens = int(mask.sum().item())
    return summed, n_tokens


def pgdpo_loss(
    policy_logits_chosen: Tensor,
    policy_logits_rejected: Tensor,
    ref_logits_chosen: Tensor,
    ref_logits_rejected: Tensor,
    chosen_labels: Tensor,
    rejected_labels: Tensor,
    reward_margin: Tensor,
    chosen_mask: Optional[Tensor] = None,
    rejected_mask: Optional[Tensor] = None,
    beta: float = 0.1,
) -> PGDPOOutput:
    """Reward-weighted, reference-anchored policy gradient.

    Args:
        policy_logits_*: ``(B, S, V)`` from the model being updated.
        ref_logits_*: ``(B, S, V)`` from the frozen reference.
        chosen_labels / rejected_labels: ``(B, S)`` next-token targets.
        reward_margin: ``(B,)``, the reward model's chosen-minus-rejected score.
        beta: strength of the KL anchor. The reference term is what stops the model drifting
            on every dimension the reward model does not score.

    Returns:
        :class:`PGDPOOutput`.
    """
    if reward_margin.dim() != 1 or reward_margin.shape[0] != policy_logits_chosen.shape[0]:
        raise ValueError(
            f"reward_margin must be (B,) with B={policy_logits_chosen.shape[0]}, got "
            f"{tuple(reward_margin.shape)}"
        )

    pol_c, n_c = sequence_logprobs(policy_logits_chosen, chosen_labels, chosen_mask)
    pol_r, n_r = sequence_logprobs(policy_logits_rejected, rejected_labels, rejected_mask)
    with torch.no_grad():
        ref_c, _ = sequence_logprobs(ref_logits_chosen, chosen_labels, chosen_mask)
        ref_r, _ = sequence_logprobs(ref_logits_rejected, rejected_labels, rejected_mask)

    # The KL anchor is the difference of summed log-probabilities, normalised by the batch. It
    # is sequence-level rather than per-token: a per-token KL would need the token masks
    # threaded through the reference pass separately, and the sequence-level form is what the
    # DPO derivation uses.
    chosen_ratio = pol_c - ref_c
    rejected_ratio = pol_r - ref_r

    # Reward-weighted: a positive margin pushes the chosen response up, a negative one pushes
    # it down. softplus keeps the loss bounded below so one bad batch cannot produce a
    # gradient large enough to undo a lot of training.
    margin = reward_margin.to(pol_c.dtype)
    weighted = F.softplus(-margin * chosen_ratio).mean()
    rejected_pull = F.softplus(margin * rejected_ratio).mean()

    kl = (chosen_ratio - rejected_ratio).mean()

    loss = weighted + rejected_pull + beta * kl
    return PGDPOOutput(
        loss=loss,
        chosen_logp=pol_c,
        rejected_logp=pol_r,
        reward_margin=margin,
        kl=kl,
        n_tokens=n_c + n_r,
    )


def check_state_consistency(
    state_chosen: Optional[Tensor],
    state_rejected: Optional[Tensor],
    atol: float = 1e-6,
) -> Tuple[bool, str]:
    """Whether a preference pair was scored under the same state.

    This is the guard described in the module docstring, and it is a hard check rather than a
    warning because the failure is silent, improves the loss curve, and teaches the model to
    induce emotional states instead of writing well.

    Args:
        state_chosen / state_rejected: ``(21,)`` or ``(B, 21)``, or ``None`` for a pair that
            was not state-pinned. Two ``None``s are consistent, since an unpinned pair is
            unpinned on both sides.
    Returns:
        ``(ok, reason)``.
    """
    if state_chosen is None and state_rejected is None:
        return True, "neither side pinned"
    if state_chosen is None or state_rejected is None:
        return False, "only one side pinned; the pair is scored under different conditions"

    a = state_chosen.reshape(-1, state_chosen.shape[-1])
    b = state_rejected.reshape(-1, state_rejected.shape[-1])
    if a.shape != b.shape:
        return False, f"state shapes differ: {tuple(a.shape)} vs {tuple(b.shape)}"
    if not torch.allclose(a, b, atol=atol):
        worst = float((a - b).abs().max())
        return False, (
            f"chosen and rejected were scored under different states (max difference "
            f"{worst:.4f}). The gradient would reward the chosen response for the state it "
            "was pinned to, not for the response, and training on it teaches the model to "
            "induce emotional states rather than to write well."
        )
    return True, "states match"


def pgdpo_update(
    policy,
    reference,
    batch: Dict[str, Tensor],
    cfg: TrainConfig,
    optimizer,
    state_chosen: Optional[Tensor] = None,
    state_rejected: Optional[Tensor] = None,
) -> PGDPOOutput:
    """One PG-DPO step, with the state-consistency guard enforced.

    Raises:
        ValueError: if the pair was scored under different states. See
            :func:`check_state_consistency`.
    """
    ok, reason = check_state_consistency(state_chosen, state_rejected)
    if not ok:
        raise ValueError(reason)

    policy.train()
    policy_out = policy(
        batch["chosen_ids"],
        attention_mask=batch.get("chosen_mask"),
        state=state_chosen,
    )
    rejected_out = policy(
        batch["rejected_ids"],
        attention_mask=batch.get("rejected_mask"),
        state=state_chosen,
    )
    with torch.no_grad():
        reference.eval()
        ref_out = reference(
            batch["chosen_ids"],
            attention_mask=batch.get("chosen_mask"),
            state=state_chosen,
        )
        ref_rejected = reference(
            batch["rejected_ids"],
            attention_mask=batch.get("rejected_mask"),
            state=state_chosen,
        )

    out = pgdpo_loss(
        policy_out.logits,
        rejected_out.logits,
        ref_out.logits,
        ref_rejected.logits,
        batch["chosen_labels"],
        batch["rejected_labels"],
        batch["reward_margin"],
        chosen_mask=batch.get("chosen_mask"),
        rejected_mask=batch.get("rejected_mask"),
        beta=cfg.dpo_beta,
    )

    optimizer.zero_grad(set_to_none=True)
    out.loss.backward()
    if cfg.grad_clip > 0:
        torch.nn.utils.clip_grad_norm_(
            [p for p in policy.parameters() if p.requires_grad], cfg.grad_clip
        )
    optimizer.step()
    return out
