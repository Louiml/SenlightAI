"""Phase 1: supervised fine-tuning.

Response-masked. The loss is computed only over the assistant's tokens, because training a
conversational model on the whole sequence teaches it to generate the user's side of the
conversation, which is both wasteful and actively confusing at inference.

The affective state is threaded through automatically. That is not optional: if
``state_source`` is ``"internal"`` and the caller forgets to run the model, the state never
updates and the bias stays at whatever the last turn left it at.
"""

from __future__ import annotations

import time
from dataclasses import dataclass
from typing import Callable, Dict, Iterable, List, Optional

import torch
import torch.nn.functional as F
from torch import Tensor, nn

from elafry.config.base import AffectConfig, ModelConfig, TrainConfig
from elafry.train.checkpoint import TrainState, save_checkpoint
from elafry.train.optim import AmpContext, GradientAccumulator, build_optimizer, build_scheduler

__all__ = ["SFTResult", "sft_step", "train_sft"]


@dataclass
class SFTResult:
    steps: int
    tokens_seen: int
    final_loss: float
    stopped_reason: str
    state: TrainState


def sft_step(
    model: nn.Module,
    batch: Dict[str, Tensor],
    loss_mask: Optional[Tensor] = None,
    label_smoothing: float = 0.0,
) -> Tensor:
    """One supervised step's loss.

    Args:
        batch: ``{"input_ids", optionally "labels", optionally "attention_mask", "state"}``.
        loss_mask: ``(B, S)`` 1 where the label should count. ``None`` uses the labels with
            ``-100`` marking ignored positions.
        label_smoothing: passed to cross-entropy. Small values (0.0 to 0.1) help on a corpus
            this size, where every transcript is memorised eventually.
    """
    input_ids = batch["input_ids"]
    attention_mask = batch.get("attention_mask")
    state = batch.get("state")

    out = model(input_ids, attention_mask=attention_mask, state=state)
    logits = out.logits[:, :-1]

    # Already shifted: labels[:, i] is the target for logits[:, i]. Shifting a second time
    # here would drop a token and silently misalign every target by one.
    labels = batch.get("labels")
    if labels is None:
        labels = input_ids[:, 1:]
    if labels.shape[1] != logits.shape[1]:
        raise ValueError(
            f"labels have {labels.shape[1]} positions but logits have {logits.shape[1]}; "
            "labels should already be shifted against the predictions"
        )

    if loss_mask is not None:
        mask = loss_mask[:, :-1].to(torch.bool)
        if mask.shape != logits.shape[:2]:
            raise ValueError(
                f"loss_mask {tuple(mask.shape)} does not match logits {tuple(logits.shape[:2])}"
            )
        selected = logits[mask]
        targets = labels[mask]
    else:
        selected = logits.reshape(-1, logits.shape[-1])
        targets = labels.reshape(-1)

    if selected.numel() == 0:
        # A batch with nothing supervised must contribute nothing, not a NaN. Returning a
        # zero that requires grad keeps the backward pass well-formed.
        return out.logits.sum() * 0.0

    return F.cross_entropy(
        selected.float(), targets, ignore_index=-100, label_smoothing=label_smoothing
    )


def train_sft(
    model: nn.Module,
    data: Iterable[Dict[str, Tensor]],
    cfg: TrainConfig,
    model_cfg: ModelConfig,
    affect_cfg: AffectConfig,
    output_dir,
    device: str = "cpu",
    grad_clip: Optional[float] = None,
    log_every: Optional[int] = None,
    sample_fn: Optional[Callable[[nn.Module], str]] = None,
) -> SFTResult:
    """Run supervised fine-tuning until whichever bound bites first.

    Stopping conditions are deliberately a set rather than a single step count. A token budget
    and a wall-clock cap are both needed: token budget is the honest measure of progress, and
    the clock is what stops a laptop run from quietly running for nine hours.

    Args:
        data: an iterable of batches. Iterated once; a resume needs a fresh iterable.
        grad_clip: overrides ``cfg.grad_clip``.
    """
    model.to(device)
    model.train()

    optimizer = build_optimizer(model.parameters(), cfg)
    scheduler = build_scheduler(optimizer, cfg)
    accumulator = GradientAccumulator(cfg.grad_accum)
    clip = cfg.grad_clip if grad_clip is None else grad_clip
    log_every = log_every or cfg.log_every

    state = TrainState(started_at=time.time())
    history: List[float] = []
    amp = AmpContext(enabled=True, device_type=torch.device(device).type, dtype=cfg.amp_dtype)

    from pathlib import Path

    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    stop_reason = "max_steps"
    step = 0

    while step < cfg.max_steps:
        for batch in data:
            batch = {k: (v.to(device) if torch.is_tensor(v) else v) for k, v in batch.items()}
            supervised = int(batch.get("mask", torch.ones(1)).sum()) or batch["input_ids"].numel()

            with amp:
                loss = sft_step(model, batch, batch.get("mask"))

            # Normalise by supervised tokens so every token counts equally regardless of how
            # many response tokens a particular micro-batch happened to contain.
            accumulator.backward(loss, scale=1.0 / max(supervised, 1))

            if not accumulator.should_step:
                continue

            grad_norm = accumulator.step(optimizer, grad_clip=clip)
            scheduler.step()

            state.step += 1
            state.tokens_seen += supervised
            history.append(float(loss))

            if state.step % log_every == 0:
                print(
                    f"step {state.step:>6d}  loss {float(loss):.4f}  "
                    f"lr {scheduler.get_last_lr()[0]:.2e}  gnorm {grad_norm:.2f}  "
                    f"tokens {state.tokens_seen:,}"
                )

            if state.step % cfg.save_every == 0:
                save_checkpoint(
                    output_dir / f"step{state.step}",
                    model,
                    model_cfg,
                    affect_cfg,
                    cfg,
                    state,
                    optimizer,
                )

            if cfg.token_budget is not None and state.tokens_seen >= cfg.token_budget:
                stop_reason = "token_budget"
                step = state.step
                break
            if cfg.max_minutes is not None and state.elapsed_minutes >= cfg.max_minutes:
                stop_reason = "wall_clock"
                step = state.step
                break
            if state.step >= cfg.max_steps:
                step = state.step
                break

        else:
            # The iterable is exhausted. Restarting it would repeat data, which for a small
            # corpus means the model memorises the same rows over and over; stop instead.
            if stop_reason == "max_steps":
                stop_reason = "data_exhausted"
            step = state.step
            break

        if stop_reason != "max_steps":
            break

    save_checkpoint(
        output_dir / f"step{state.step}", model, model_cfg, affect_cfg, cfg, state, optimizer
    )

    if sample_fn is not None:
        model.eval()
        print("sample:", sample_fn(model))

    return SFTResult(
        steps=state.step,
        tokens_seen=state.tokens_seen,
        final_loss=history[-1] if history else float("nan"),
        stopped_reason=stop_reason,
        state=state,
    )