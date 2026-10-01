"""Supervised fine-tuning.

The simplest part of the training stack and the one with the most ways to be subtly wrong, so
the non-obvious choices are written down rather than left to whoever reads it next.

## The state is pinned per example, not per batch

Each example is stepped through the state machine under its own state. A batch that shares
one state across all of it means the model is trained on ``batch_size`` copies of one
emotional condition, and the affective mechanism never sees the variety it will be tested on.

## The affective parameters are not dead in SFT

The obvious move is to train only the language-model parameters in SFT and let PG-DPO handle
the affective side. That is wrong for a specific reason: the state predictor, the drive
encoder and the per-block bias scales are the *only* path by which affective state reaches the
output, and if they are frozen here they stay at their zero initialisation for a whole
supervised phase. The bias projections in particular are zero-initialised, so frozen they are
exactly zero, and the model spends SFT learning a task it can only solve by ignoring its own
state.

## Loss masking

The loss is computed on response tokens only. Including prompt tokens teaches the model to
model the user, which at best wastes capacity and at worst optimises for producing plausible
user turns, since those are often easier to predict than good advice.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Dict, List, Optional, Tuple

import torch
import torch.nn.functional as F
from torch import Tensor

from varys.config.base import AffectConfig, TrainConfig
from varys.models.varys import Varys

__all__ = [
    "SFTOutput",
    "response_mask",
    "sft_loss",
    "sft_step",
    "build_affect_optimizer_groups",
]


@dataclass
class SFTOutput:
    """Loss, and the state the batch produced, for logging."""

    loss: Tensor
    mean_state: Tensor
    n_response_tokens: int = 0

    def to_dict(self) -> Dict[str, float]:
        return {
            "loss": float(self.loss.detach()),
            "n_response_tokens": self.n_response_tokens,
        }


def response_mask(
    prompt_len: int,
    total_len: int,
    device=None,
) -> Tensor:
    """A mask that is 1 on response tokens and 0 on prompt tokens.

    Args:
        prompt_len: length of the prompt, tokens not to be trained on.
        total_len: full sequence length.
    """
    if prompt_len >= total_len:
        raise ValueError(
            f"prompt_len {prompt_len} must be less than total_len {total_len}; a sequence "
            "with no response tokens has no loss and would silently train on nothing"
        )
    m = torch.zeros(total_len, device=device)
    m[prompt_len:] = 1.0
    return m


def sft_loss(
    logits: Tensor,
    labels: Tensor,
    mask: Optional[Tensor] = None,
) -> Tuple[Tensor, int]:
    """Cross-entropy over masked positions.

    ``labels`` are already shifted by the caller, which is the caller's job because the shift
    has to be consistent with how the KV cache advanced during generation, and getting that
    wrong is invisible except as a slightly worse model.
    """
    if logits.shape[:2] != labels.shape:
        raise ValueError(
            f"logits {tuple(logits.shape)} and labels {tuple(labels.shape)} disagree on (B, S)"
        )
    per_token = F.cross_entropy(
        logits.float().reshape(-1, logits.shape[-1]),
        labels.reshape(-1),
        reduction="none",
    ).view(labels.shape)

    if mask is None:
        mask = torch.ones_like(per_token)
    mask = mask.to(per_token.dtype)

    denom = mask.sum().clamp(min=1.0)
    return (per_token * mask).sum() / denom, int(mask.sum().item())


def build_affect_optimizer_groups(
    model: Varys,
    lr: float,
    affect_lr_mult: float = 1.0,
) -> List[Dict[str, object]]:
    """Parameter groups, with the affective subsystem addressable on its own.

    Returned rather than built inside the training step so a caller can see the grouping and
    set a different learning rate on the affective parameters without reading the loop.

    The affective group is not automatically excluded. It is separated so it *can* be, and so
    an ablation can zero its influence on the optimiser without touching the model.
    """
    affective, rest = [], []
    for name, p in model.named_parameters():
        if not p.requires_grad:
            continue
        if name.startswith(("internal_state", "state_encoder", "router")) or ".affect." in name:
            affective.append(p)
        else:
            rest.append(p)
    return [
        {"params": rest, "lr": lr, "name": "trunk"},
        {"params": affective, "lr": lr * affect_lr_mult, "name": "affect"},
    ]


def sft_step(
    model: Varys,
    batch: Dict[str, Tensor],
    optimizer,
    cfg: TrainConfig,
    affect_cfg: Optional[AffectConfig] = None,
    affect_lr_mult: float = 1.0,
) -> SFTOutput:
    """One supervised step, with per-example state stepping.

    Args:
        batch: ``input_ids (B, S)``, ``labels (B, S)``, ``prompt_lens (B,)``,
            optional ``attention_mask``.
    """
    if "prompt_lens" not in batch:
        raise KeyError(
            "batch must carry 'prompt_lens'; without it the loss cannot be masked to "
            "response tokens and the model is trained to model the user"
        )

    model.train()
    ids = batch["input_ids"]
    bsz, seq = ids.shape

    # Per-example state, each stepped through the machine under its own mask. See the module
    # docstring: a batch sharing one state trains on batch_size copies of one condition.
    states: List[Tensor] = []
    for i in range(bsz):
        plen = int(batch["prompt_lens"][i])
        window = min(seq, max(plen, 1))
        sub_ids = ids[i : i + 1, :window]
        sub_mask = None
        if "attention_mask" in batch:
            sub_mask = batch["attention_mask"][i : i + 1, :window]
        s, _ = model.step_state(sub_ids, sub_mask)
        states.append(s)
    state = torch.cat(states, dim=0)

    out = model(ids, attention_mask=batch.get("attention_mask"), state=state)

    mask = torch.zeros(bsz, seq, device=ids.device)
    for i in range(bsz):
        plen = int(batch["prompt_lens"][i])
        if plen < seq:
            mask[i, plen:] = 1.0

    loss, n = sft_loss(out.logits[:, :-1], batch["labels"][:, 1:], mask[:, 1:])

    optimizer.zero_grad(set_to_none=True)
    loss.backward()
    if cfg.grad_clip > 0:
        torch.nn.utils.clip_grad_norm_(
            [p for p in model.parameters() if p.requires_grad], cfg.grad_clip
        )
    optimizer.step()
    return SFTOutput(loss=loss, mean_state=state.mean(dim=0).detach(), n_response_tokens=n)
