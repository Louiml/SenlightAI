"""Phase 3 — Training loop for Senlight Coder AI.

Responsibilities
----------------
1. Load the DataLoader (Phase 1b) and the model (Phase 2).
2. Train with:
     * `AdamW`            (+ decoupled weight decay, Llama betas)
     * Cosine-annealed LR with linear warmup
     * Global gradient clipping
     * Mixed precision via `torch.amp` (autocast + GradScaler), usable on CUDA
       with graceful CPU fallback.
3. Periodically checkpoint `safetensors` weights (HF-style key names) and a
   full PyTorch state dict (for resuming), plus a JSON of training stats.

Usage:
    python train.py                 # uses config.py defaults
    python train.py --steps 1000    # override schedule length
"""

from __future__ import annotations

import argparse
import json
import math
import time
import warnings
from pathlib import Path

import torch
from torch import nn
from torch.optim import AdamW
from torch.optim.lr_scheduler import LambdaLR

import config as C
from dataset import load_tokenizer, make_dataloader
from model import SenlightCoder

# `scheduler.step()` after `optimizer.step()` is the correct order, but when a
# `GradScaler` *skips* an optimizer step (inf/nan grads) PyTorch emits a
# misleading warning. We step the scheduler unconditionally, and the warmup
# coefficient already handles step 0, so suppress the false positive.
warnings.filterwarnings(
    "ignore",
    message="Detected call of `lr_scheduler.step()` before `optimizer.step()`.",
    category=UserWarning,
)

# ---------------------------------------------------------------------------
# Learning-rate schedule: linear warmup -> cosine decay
# ---------------------------------------------------------------------------
def build_lr_lambda(warmup_steps: int, total_steps: int) -> callable:
    """Return a callable mapping step -> LR multiplier for Cosmetic warmup.

    * For ``step < warmup_steps``: linear ramp ``step / warmup_steps``.
    * Afterwards: half-cosine anneal from 1.0 down to ``MIN_LR/LR``.
    """

    def lr_lambda(step: int) -> float:
        if step < warmup_steps:
            return float(step + 1) / max(1, warmup_steps)
        progress = (step - warmup_steps) / max(1, total_steps - warmup_steps)
        progress = min(1.0, progress)
        cosine = 0.5 * (1.0 + math.cos(math.pi * progress))  # 1 -> 0
        return 1.0 * cosine + (C.MIN_LR / C.LR) * (1.0 - cosine)

    return lr_lambda


# ---------------------------------------------------------------------------
# Mixed-precision context manager helper
# ---------------------------------------------------------------------------
class AMPController:
    """Wraps `torch.amp` so the rest of the loop is device-agnostic.

    On CUDA devices we run fp16 autocast plus a gradient scaler; otherwise we
    fall back to a no-op fp32 path. This keeps the same code working on a
    CUDA-capable box (RTX 3060) and a CPU-only dev machine.
    """

    def __init__(self, device: torch.device, enabled: bool = True):
        self.enabled = enabled and device.type == "cuda"
        self.device = device
        self.scaler = (
            torch.amp.GradScaler("cuda", enabled=self.enabled)
            if self.enabled
            else None
        )

    def autocast(self):
        if self.enabled:
            return torch.amp.autocast("cuda", dtype=torch.float16)
        return torch.enable_grad()

    def backward(self, loss: torch.Tensor) -> None:
        if self.scaler is not None:
            self.scaler.scale(loss).backward()
        else:
            loss.backward()

    def step(self, optimizer: torch.optim.Optimizer) -> None:
        if self.scaler is not None:
            self.scaler.step(optimizer)
            self.scaler.update()
        else:
            optimizer.step()


# ---------------------------------------------------------------------------
# Weight export to safetensors (HF-friendly key names for Candle)
# ---------------------------------------------------------------------------
def export_to_safetensors(
    model: nn.Module,
    out_path: Path = C.MODEL_OUT,
    dtype: torch.dtype = torch.float32,
) -> str:
    """Write the model weights as a HuggingFace-style ``safetensors`` tensor.

    Keys follow the LLaMA naming that the Candle Rust loader (Phase 4) expects:
      * ``model.embed_tokens.weight``   (and no separate lm_head if tied)
      * ``model.layers.N.input_layernorm.weight``, ``.self_attn.q_proj.weight``,
        ``.self_attn.k_proj.weight``, ``.self_attn.v_proj.weight``,
        ``.self_attn.o_proj.weight``,
        ``.post_attention_layernorm.weight``, ``.mlp.gate_proj.weight``,
        ``.mlp.up_proj.weight``, ``.mlp.down_proj.weight``
      * ``model.norm.weight``

    Our PyTorch SwiGLU parameters are named ``mlp.gate/_up/_down``; Llama/Candle
    use ``gate_proj/up_proj/down_proj`` — this function renames accordingly, and
    maps the tied lm_head to ``model.embed_tokens.weight`` (single tensor).
    """
    from safetensors.torch import save_file

    # Drop non-weights (RoPE buffers are recomputed at load time in Candle)
    # and the tied lm_head (its tensor == embed_tokens.weight).
    skip = {"cos", "sin", "inv_freq", "lm_head.weight"}

    state: dict[str, torch.Tensor] = {}
    for name, param in model.state_dict().items():
        if name in skip:
            continue

        # Translate SwiGLU projection names to HF/Llama convention.
        name = name.replace(".mlp.gate.", ".mlp.gate_proj.")
        name = name.replace(".mlp.up.", ".mlp.up_proj.")
        name = name.replace(".mlp.down.", ".mlp.down_proj.")

        # Prepend the standard `model.` namespace Candle's Llama loader expects.
        state[f"model.{name}"] = param.to(dtype).detach().cpu()

    out_path.parent.mkdir(parents=True, exist_ok=True)
    save_file(state, str(out_path))
    print(f"[export] wrote {len(state)} tensors -> {out_path}")
    return str(out_path)


def save_checkpoint(
    model: nn.Module,
    optimizer: torch.optim.Optimizer,
    scheduler,
    amp: AMPController,
    step: int,
    loss: float,
    out_dir: Path = C.ARTIFACTS_DIR,
) -> None:
    """Persist a full PyTorch checkpoint for resuming + a safetensors snapshot."""
    out_dir.mkdir(parents=True, exist_ok=True)
    torch.save(
        {
            "model": model.state_dict(),
            "optimizer": optimizer.state_dict(),
            "scheduler": scheduler.state_dict(),
            "scaler": amp.scaler.state_dict() if amp.scaler else {},
            "step": step,
            "loss": loss,
        },
        out_dir / f"checkpoint_step{step}.pt",
    )
    export_to_safetensors(model, C.ARTIFACTS_DIR / f"senlight_step{step}.safetensors")


# ---------------------------------------------------------------------------
# Training loop
# ---------------------------------------------------------------------------
def train(args) -> None:
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(f"[train] device = {device} ({torch.cuda.get_device_name(0) if device.type=='cuda' else 'CPU'})")

    # Data + model.
    tokenizer = load_tokenizer()
    dataloader = make_dataloader(
        tokenizer,
        batch_size=args.batch_size,
        shuffle=True,
        num_workers=args.num_workers,
    )
    model = SenlightCoder().to(device)
    n_params = sum(p.numel() for p in model.parameters())
    print(f"[train] model params = {n_params:,}")

    optimizer = AdamW(
        model.parameters(),
        lr=args.lr,
        betas=C.BETAS,
        weight_decay=C.WEIGHT_DECAY,
    )

    total_steps = args.steps
    scheduler = LambdaLR(
        optimizer, lr_lambda=build_lr_lambda(args.warmup_steps, total_steps)
    )

    amp = AMPController(device, enabled=args.amp)

    global_step = 0
    # Continue-from support: resume the exact pretrained weights so we keep
    # training the SAME model (accumulating tokens seen across generations).
    # Accepts either a plain state-dict export (.pt, e.g. senlight_300m_he2) or a
    # wrapped step-checkpoint ({"model", "optimizer", "scheduler", "scaler", ...}).
    if args.continue_from:
        ckpt = C.ARTIFACTS_DIR / f"{args.continue_from}.pt"
        if not ckpt.exists():
            raise FileNotFoundError(f"--continue-from checkpoint missing: {ckpt}")
        raw = torch.load(ckpt, map_location=device)
        if isinstance(raw, dict) and "model" in raw and "optimizer" in raw:
            model.load_state_dict(raw["model"], strict=False)
            try:
                optimizer.load_state_dict(raw["optimizer"])
                scheduler.load_state_dict(raw["scheduler"])
                if amp.scaler and "scaler" in raw and raw["scaler"]:
                    amp.scaler.load_state_dict(raw["scaler"])
            except Exception as exc:  # noqa: BLE001
                print(f"[train] note: optimizer resync skipped ({exc}); weights resumed.")
            global_step = int(raw.get("step", 0))
            print(f"[train] resumed wrapped checkpoint {ckpt.name} at step {global_step}")
        else:
            model.load_state_dict(raw, strict=False)
            print(f"[train] continuing from weights {ckpt.name}")

    running_loss = 0.0
    start = time.time()

    # Tokens per optimisation step = batch_size * block_size (each window predicts
    # block_size next tokens).
    tokens_per_step = args.batch_size * C.BLOCK_SIZE
    tokens_seen = 0
    token_budget = args.token_budget  # stop once this many tokens have been seen

    print(f"[train] training for {total_steps} steps (lr={args.lr}, "
          f"warmup={args.warmup_steps}, amp={'fp16' if amp.enabled else 'fp32'})")
    if token_budget:
        print(f"[train] token budget = {token_budget:,} ({tokens_per_step:,} tokens/step "
              f"=> ~{token_budget // max(1, tokens_per_step):,} steps)")
    batches_per_epoch = max(1, len(dataloader))

    while global_step < total_steps:
        if token_budget and tokens_seen >= token_budget:
            break
        model.train()
        for input_ids, labels in dataloader:
            if global_step >= total_steps:
                break
            if token_budget and tokens_seen >= token_budget:
                break
            input_ids = input_ids.to(device)
            labels = labels.to(device)

            optimizer.zero_grad(set_to_none=True)
            with amp.autocast():
                loss, _ = model.compute_loss(input_ids, labels)
            amp.backward(loss)

            # Global gradient clipping (applied on the *scaled* grads under AMP
            # is handled automatically by GradScaler in `step`; clip first on
            # the unscaled grads here for stability).
            if amp.scaler is None:
                torch.nn.utils.clip_grad_norm_(model.parameters(), C.GRAD_CLIP_NORM)
            else:
                # Clip the scaled gradients inside the scaler scope.
                amp.scaler.unscale_(optimizer)
                torch.nn.utils.clip_grad_norm_(model.parameters(), C.GRAD_CLIP_NORM)

            amp.step(optimizer)
            scheduler.step()

            running_loss += loss.detach().item()
            global_step += 1
            tokens_seen += tokens_per_step

            if global_step % args.log_every == 0:
                avg = running_loss / args.log_every
                lr_now = optimizer.param_groups[0]["lr"]
                tok = time.time()
                print(
                    f"[train] step {global_step:>6}/{total_steps}  "
                    f"tokens {tokens_seen:>12,}  "
                    f"loss {avg:8.4f}  lr {lr_now:.2e}  "
                    f"{args.log_every/(tok-start):.1f} steps/s"
                )
                running_loss = 0.0
                start = time.time()

            if global_step % args.save_every == 0:
                save_checkpoint(
                    model, optimizer, scheduler, amp, global_step, loss.detach().item()
                )

    # Final export (honours a custom output name for continued runs).
    final_stem = args.out or (
        f"senlight_c{token_budget // 1_000_000}M" if token_budget else "senlight"
    )
    final_sft = C.ARTIFACTS_DIR / f"{final_stem}.safetensors"
    export_to_safetensors(model, final_sft)
    torch.save(model.state_dict(), C.ARTIFACTS_DIR / f"{final_stem}.pt")
    print(f"[train] finished after {global_step:,} steps / {tokens_seen:,} tokens. "
          f"Saved -> {final_sft}")


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------
def parse_args():
    p = argparse.ArgumentParser(description="Train Senlight Coder AI")
    p.add_argument("--steps", type=int, default=C.MAX_STEPS)
    p.add_argument("--batch-size", type=int, default=C.BATCH_SIZE)
    p.add_argument("--lr", type=float, default=C.LR)
    p.add_argument("--warmup-steps", type=int, default=C.WARMUP_STEPS)
    p.add_argument("--num-workers", type=int, default=C.NUM_WORKERS)
    p.add_argument("--log-every", type=int, default=C.LOG_EVERY)
    p.add_argument("--save-every", type=int, default=C.SAVE_EVERY)
    p.add_argument("--continue-from", default="",
                   help="stem of a .pt checkpoint to continue training from (same model, accrue tokens)")
    p.add_argument("--token-budget", type=int, default=0,
                   help="stop once this many tokens have been seen (0 = run all --steps)")
    p.add_argument("--out", default="", help="output stem for the final safetensors/pt")
    fp = p.add_mutually_exclusive_group()
    fp.add_argument("--amp", action="store_true", help="enable fp16 mixed precision (default on CUDA)")
    fp.add_argument("--no-amp", action="store_true", help="disable mixed precision")
    p.set_defaults(amp=None)
    return p.parse_args()


if __name__ == "__main__":
    args = parse_args()
    if args.amp is None:
        args.amp = True  # default ON
    train(args)