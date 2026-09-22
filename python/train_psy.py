"""PSY-SFT: empathetic instruction fine-tuning for Senlight.

Loads the combined psychological corpus (``artifacts/psy_sft.jsonl``), builds
instruction-tuning batches, and fine-tunes the decoder-only model with
**response-masked cross-entropy** — loss is computed only over the assistant
``output`` tokens (proper SFT), not the prompt.

This produces the "Supervised Fine-Tuning: Empathetic Baseline" stage from the
Affective AI training pipeline.

Usage:
    python train_psy.py --steps 2000 --batch-size 4 --amp --out psy_sft
"""

from __future__ import annotations

import argparse
import json
import math
import os
import time
import warnings
from pathlib import Path

import torch
from torch import nn
from torch.optim import AdamW
from torch.optim.lr_scheduler import LambdaLR

import config as C
from dataset import load_tokenizer
from model import SenlightCoder

warnings.filterwarnings(
    "ignore",
    message="Detected call of `lr_scheduler.step()` before `optimizer.step()`.",
    category=UserWarning,
)

RESEARCH = C.ROOT / "research"
SFT_JSONL = C.ARTIFACTS_DIR / "psy_sft.jsonl"
SFT_PURE_JSONL = C.ARTIFACTS_DIR / "psy_sft_pure.jsonl"

EOS_ID = C.EOS_ID
PAD_ID = C.PAD_ID
MAX_LEN = 384  # budget per sample; therapist answers are long (512 model limit)

RNG = torch.Generator().manual_seed(C.SEED)


# ---------------------------------------------------------------------------
# Data
# ---------------------------------------------------------------------------
def load_sft_rows(pure: bool = False) -> list[dict]:
    path = SFT_PURE_JSONL if pure else SFT_JSONL
    if not Path(path).exists():
        raise FileNotFoundError(f"missing {path} — run prep_psy.py first")
    with open(path, encoding="utf-8") as fh:
        return [json.loads(line) for line in fh]


def prompt_from(row: dict) -> str:
    inst = row["instruction"].strip()
    inp = (row.get("input") or "").strip()
    if inp:
        return f"{inst}\n\nUser: {inp}\n\nAssistant:"
    return f"{inst}\n\nAssistant:"


def collate(row: dict, tok, device: torch.device, max_len: int = MAX_LEN):
    """Tokenize prompt and output; return (input_ids, labels) with output-mask.

    Loss mask: prompt tokens -> PAD (-100 via loss mask), output tokens -> the
    target ids shifted for next-token prediction.
    """
    prompt = prompt_from(row)
    output = row["output"].strip() + "\n"

    p_ids = tok.encode(prompt).ids
    o_ids = tok.encode(output).ids
    # Budget: leave at least 1 token for the assistant start; fits under max_len.
    total = len(p_ids) + len(o_ids)
    if total > max_len:
        over = total - max_len
        if over < len(o_ids):
            o_ids = o_ids[: len(o_ids) - over]
        else:
            # Very short output still matters; keep prompt head.
            p_ids = p_ids[: max_len - 2]
            o_ids = o_ids[:2]
    ids = p_ids + o_ids  # full sequence

    # input_ids = ids[:-1], labels = ids[1:] ; mask prompt region from loss.
    input_ids = torch.tensor(ids[:-1], dtype=torch.long)
    labels = torch.tensor(ids[1:], dtype=torch.long)

    # Positions after prompt belong to the output; those are the only targets.
    prompt_len = len(p_ids)
    loss_mask = torch.ones_like(labels)
    # labels index == ids index+1; prompt positions (just before output start)
    # get -100 so they don't contribute.
    mask = torch.zeros_like(labels)
    if len(labels) > prompt_len:
        mask[- (len(labels) - prompt_len):] = 1.0
    loss_mask = mask

    return input_ids.to(device), labels.to(device), loss_mask.to(device), len(p_ids)


# ---------------------------------------------------------------------------
# LR schedule (linear warmup + cosine)
# ---------------------------------------------------------------------------
def make_scheduler(opt, warmup: int, total: int):
    def lr_fn(step):
        if step < warmup:
            return float(step + 1) / max(1, warmup)
        prog = (step - warmup) / max(1, total - warmup)
        prog = min(1.0, prog)
        return 0.5 * (1.0 + math.cos(math.pi * prog))

    return LambdaLR(opt, lr_lambda=lr_fn)


# ---------------------------------------------------------------------------
# Masked SFT loss
# ---------------------------------------------------------------------------
def masked_sft_loss(logits: torch.Tensor, labels: torch.Tensor, mask: torch.Tensor) -> torch.Tensor:
    """Cross-entropy only over masked (output) positions."""
    # logits: (B, S, V), labels: (B, S), mask: (B, S)
    logits = logits.reshape(-1, logits.size(-1))
    labels = labels.reshape(-1)
    ce = torch.nn.functional.cross_entropy(logits, labels, reduction="none")  # (B*S,)
    ce = ce.reshape(-1, mask.size(1))
    masked = ce * mask  # zero out prompt positions
    n = mask.sum().clamp(min=1)
    return masked.sum() / n


# ---------------------------------------------------------------------------
# Training
# ---------------------------------------------------------------------------
def train(args) -> None:
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(f"[psy-sft] device = {device}")

    tok = load_tokenizer()
    rows = load_sft_rows(pure=args.pure)
    if args.only_source:
        keep = [r for r in rows if r.get("source") == args.only_source]
        rows = keep
        print(f"[psy-sft] filtered to source='{args.only_source}': {len(rows)} rows")
    # Upsample basic greetings so the model reliably warms up politely.
    if args.upsample_greets > 0:
        gathers = [r for r in rows if r.get("source") == "greetings" or
                   (r.get("instruction", "") or "").lower().startswith(("respond warmly", "reply warmly"))]
        if gathers:
            rows = rows + gathers * max(1, args.upsample_greets - 1)
        print(f"[psy-sft] upsample greetings x{args.upsample_greets}: "
              f"total rows now {len(rows)}")
    # Upsample Hebrew rows so the model learns to respond in fluent Hebrew.
    if args.upsample_hebrew > 0:
        heb = [r for r in rows if r.get("source") == "hebrew"]
        if heb:
            rows = rows + heb * max(1, args.upsample_hebrew - 1)
        print(f"[psy-sft] upsample hebrew x{args.upsample_hebrew}: "
              f"total rows now {len(rows)}")
    print(f"[psy-sft] {len(rows)} SFT rows ({'pure-psy' if args.pure else 'combined'})")

    model = SenlightCoder().to(device)
    if args.continue_from:
        ckpt = C.ARTIFACTS_DIR / f"{args.continue_from}.pt"
        model.load_state_dict(torch.load(ckpt, map_location=device))
        print(f"[psy-sft] continued from {ckpt}")
    print(f"[psy-sft] params = {sum(p.numel() for p in model.parameters()):,}")

    opt = AdamW(model.parameters(), lr=args.lr, betas=C.BETAS, weight_decay=C.WEIGHT_DECAY)
    sched = make_scheduler(opt, args.warmup, args.steps)
    scaler = torch.amp.GradScaler("cuda", enabled=(args.amp and device.type == "cuda"))
    autocast = (
        torch.amp.autocast("cuda", dtype=torch.float16)
        if (args.amp and device.type == "cuda")
        else torch.enable_grad()
    )

    model.train()
    start = time.time()
    for step in range(1, args.steps + 1):
        # Sample a mini-batch of rows (exhaust-resample: pick random indices).
        idxs = torch.randint(0, len(rows), (args.batch_size,), generator=RNG).tolist()
        batch = [collate(rows[i], tok, device) for i in idxs]

        # Pad batch to equal length.
        T = max(b[0].size(0) for b in batch)
        input_ids = torch.full((args.batch_size, T), PAD_ID, device=device, dtype=torch.long)
        labels = torch.full((args.batch_size, T), PAD_ID, device=device, dtype=torch.long)
        loss_mask = torch.zeros((args.batch_size, T), device=device, dtype=torch.float)
        for b_i, (x, y, m, _plen) in enumerate(batch):
            L = x.size(0)
            input_ids[b_i, :L] = x
            labels[b_i, :L] = y
            loss_mask[b_i, :L] = m

        opt.zero_grad(set_to_none=True)
        with autocast:
            logits, _ = model(input_ids)  # (B, T, V)
            loss = masked_sft_loss(logits, labels, loss_mask)
        if scaler is not None:
            scaler.scale(loss).backward()
        else:
            loss.backward()

        if scaler is not None:
            scaler.unscale_(opt)
            torch.nn.utils.clip_grad_norm_(model.parameters(), C.GRAD_CLIP_NORM)
            scaler.step(opt)
            scaler.update()
        else:
            torch.nn.utils.clip_grad_norm_(model.parameters(), C.GRAD_CLIP_NORM)
            opt.step()
        sched.step()

        if step % args.log_every == 0:
            dt = time.time() - start
            print(
                f"[psy-sft] step {step:>5}/{args.steps} loss {loss.item():.4f} "
                f"lr {opt.param_groups[0]['lr']:.2e} {args.log_every/dt:.1f} step/s"
            )
            start = time.time()

    # Save.
    out_safetensors = C.ARTIFACTS_DIR / f"{args.out}.safetensors"
    from train import export_to_safetensors
    export_to_safetensors(model, out_safetensors)
    torch.save(model.state_dict(), C.ARTIFACTS_DIR / f"{args.out}.pt")
    print(f"[psy-sft] saved -> {out_safetensors}")


def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument("--steps", type=int, default=2500)
    p.add_argument("--batch-size", type=int, default=4)
    p.add_argument("--lr", type=float, default=3e-4)
    p.add_argument("--warmup", type=int, default=150)
    p.add_argument("--log-every", type=int, default=100)
    p.add_argument("--out", default="senlight_psy_sft")
    p.add_argument("--pure", action="store_true", help="train on psychological rows only (no alpaca grounding)")
    p.add_argument("--upsample-greets", type=int, default=0,
                   help="repeat greeting/small-talk rows N times in the pool")
    p.add_argument("--upsample-hebrew", type=int, default=0,
                   help="repeat Hebrew rows N times so the model learns fluent Hebrew")
    p.add_argument("--only-source", default="",
                   help="train only on rows from this source tag (e.g. greetings)")
    p.add_argument("--continue-from", default="", help="continue SFT from this weights stem (e.g. senlight_psy_sft)")
    p.add_argument("--amp", action="store_true", default=True)
    p.add_argument("--no-amp", action="store_true")
    return p.parse_args()


if __name__ == "__main__":
    a = parse_args()
    if a.no_amp:
        a.amp = False
    os.environ.setdefault("SENLIGHT_DATA_DIR", str(C.DATA_DIR / "psy"))
    train(a)