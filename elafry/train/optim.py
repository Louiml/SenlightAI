"""Optimiser, learning-rate schedule, and gradient accumulation.

The schedule is linear warmup into a cosine decay. Two details that are easy to get wrong:

* Warmup is not optional at 8B. The first few hundred steps of a large model run in bf16 with
  activations that have not yet settled into a stable range, and skipping warmup is how a
  run dies at step 40 with a NaN.
* The decay floor is ``min_lr`` rather than zero. Decaying all the way to zero leaves the
  final weights sitting in a flat basin where nothing moves them, which matters for the
  preference stages: a model still taking large steps while it learns to disagree with users
  will undo that learning between checkpoints.
"""

from __future__ import annotations

import math
from typing import Iterable

import torch
from torch import Tensor
from torch.optim import AdamW
from torch.optim.lr_scheduler import LambdaLR

from elafry.config.base import TrainConfig

__all__ = [
    "build_optimizer",
    "build_scheduler",
    "lr_at_step",
    "AmpContext",
    "GradientAccumulator",
    "count_tokens",
]


def build_optimizer(params: Iterable[torch.nn.Parameter], cfg: TrainConfig) -> AdamW:
    """AdamW with weight decay on matrices only.

    Biases, RMSNorm gains and the embedding matrix are excluded from decay. Decaying a
    LayerNorm gain pulls it toward zero, which shrinks the normaliser and eventually makes
    the layer a no-op; it is a well-known way to lose a model quietly rather than
    dramatically.
    """
    decay, no_decay = [], []
    seen: set = set()

    for p in params:
        if not p.requires_grad:
            continue
        # Tied weights appear once under two names; decaying twice would double the rate.
        if id(p) in seen:
            continue
        seen.add(id(p))

        if p.ndim >= 2 and not _is_embedding_like(p):
            decay.append(p)
        else:
            no_decay.append(p)

    groups = [
        {"params": decay, "weight_decay": cfg.weight_decay},
        {"params": no_decay, "weight_decay": 0.0},
    ]
    return AdamW(groups, lr=cfg.lr, betas=cfg.betas, eps=1e-8)


def _is_embedding_like(p: torch.nn.Parameter) -> bool:
    return p.ndim == 2 and p.shape[0] > 10_000


def lr_at_step(step: int, cfg: TrainConfig) -> float:
    """The learning rate at ``step``, as a fraction of ``cfg.lr``.

    Warmup covers steps ``[0, warmup_steps)``. Past that the cosine decays from ``lr`` to
    ``min_lr`` across the remaining steps. A warmup shorter than the total is respected;
    if ``max_steps`` is inside the warmup the schedule never reaches the decay.
    """
    warmup = max(cfg.warmup_steps, 0)
    if warmup > 0 and step < warmup:
        return (step + 1) / warmup

    decay_span = max(cfg.max_steps - warmup, 1)
    progress = min(max((step - warmup) / decay_span, 0.0), 1.0)
    # Smoothstep-free cosine: (1 + cos(pi * p)) / 2 maps p=0 to 1 and p=1 to 0.
    cosine = 0.5 * (1.0 + math.cos(math.pi * progress))
    floor = cfg.min_lr / cfg.lr if cfg.lr > 0 else 0.0
    return floor + (1.0 - floor) * cosine


def build_scheduler(optimizer: AdamW, cfg: TrainConfig) -> LambdaLR:
    return LambdaLR(optimizer, lr_lambda=lambda step: lr_at_step(step, cfg))


class AmpContext:
    """bf16 autocast where it is available, fp32 everywhere else.

    bf16 rather than fp16 because the 3060 and every current accelerator support it natively,
    and it does not need a loss scaler. The exponent range is the same as fp32, so a run that
    diverges diverges for a real reason rather than because a gradient underflowed to zero.
    """

    def __init__(self, enabled: bool = True, device_type: str = "cuda", dtype: str = "bf16"):
        self.enabled = enabled and device_type == "cuda"
        self.device_type = device_type
        self.dtype = torch.bfloat16 if dtype == "bf16" else torch.float16

    def __enter__(self):
        if self.enabled:
            self._ctx = torch.autocast(device_type=self.device_type, dtype=self.dtype)
            return self._ctx.__enter__()
        return _NullCtx().__enter__()

    def __exit__(self, *exc):
        if self.enabled:
            return self._ctx.__exit__(*exc)
        return False


class _NullCtx:
    def __enter__(self):
        return self

    def __exit__(self, *exc):
        return False


class GradientAccumulator:
    """Accumulate over micro-batches, step on ``every``.

    The scale factor matters. When micro-batches have different token counts, averaging
    per-micro-batch losses weights short batches the same as long ones. Normalising by total
    tokens instead weights each *token* equally, which is what the loss is meant to measure.
    """

    def __init__(self, every: int):
        if every < 1:
            raise ValueError(f"grad_accum must be at least 1, got {every}")
        self.every = every
        self.micro_step = 0

    def reset(self) -> None:
        self.micro_step = 0

    @property
    def should_step(self) -> bool:
        return self.micro_step % self.every == 0

    def step(self, optimizer: AdamW, grad_clip: float = 0.0) -> float:
        """Clip, step, zero. Returns the gradient norm before clipping."""
        if grad_clip and grad_clip > 0:
            norm = torch.nn.utils.clip_grad_norm_(
                [p for p in optimizer.param_groups[0]["params"]]
                + [p for g in optimizer.param_groups[1:] for p in g["params"]],
                grad_clip,
            )
        else:
            norm = torch.zeros(())

        optimizer.step()
        optimizer.zero_grad(set_to_none=True)
        return float(norm)

    def backward(self, loss: Tensor, scale: float = 1.0) -> None:
        """Scale then backward. The scale is applied here rather than at the call site so no
        caller can forget it."""
        (loss * scale).backward()
        self.micro_step += 1


def count_tokens(batch: dict) -> int:
    """Number of supervised tokens in a packed batch, used for token-budget accounting."""
    mask = batch.get("mask")
    if mask is None:
        labels = batch.get("labels")
        if labels is None:
            return 0
        return int((labels != -100).sum())
    return int((mask != 0).sum())


def tokens_per_step(cfg: TrainConfig, block_size: int) -> int:
    return cfg.batch_size * cfg.grad_accum * block_size