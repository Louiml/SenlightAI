"""Affective Reward Model (RLEF stage).

Trains a scalar "empathy reward" scorer: given ``(prompt, response)`` it outputs
a score that the RL stage will maximise. Architecture mirrors Llama reward
models — the base `SenlightCoder` transformer (initialised from the SFT model)
plus a linear reward head over the final hidden state.

Two signal sources are combined into a training target:
  1. **Preference pairs (RLEF)**: real empathetic responses are "chosen"
     (reward target high) against synthetic "rejected" alternatives
     (low reward). Loss = Bradley–Terry pairwise ranking.
  2. **Affective consistency**: a soft MSE term nudges the reward toward the
     affective (VAD/empathy) model's assessment of how supportive the response
     is, tying the reward to the emotional framework in the guide.

The trained head is exported as ``affective_reward.safetensors`` so the RL
stage and the GUI can score candidate responses live.
"""

from __future__ import annotations

import argparse
import json
import math
import time
from pathlib import Path

import torch
from torch import nn
from torch.nn import functional as F
from torch.optim import AdamW

import config as C
from affective.emotions import analyze, PLUTCHIK_VAD
from dataset import load_tokenizer
from model import SenlightCoder

SFT_JSONL = C.ARTIFACTS_DIR / "psy_sft.jsonl"
RNG = torch.Generator().manual_seed(0)


class AffectiveRewardModel(nn.Module):
    """Base transformer + scalar reward head."""

    def __init__(self, backbone: SenlightCoder):
        super().__init__()
        self.backbone = backbone
        # Reward head: hidden (after final norm / before lm_head) -> scalar.
        self.reward_head = nn.Linear(C.MODEL_DIM, 1, bias=False)
        self.reward_head.weight.data.normal_(mean=0.0, std=0.02)

    def reward(self, input_ids: torch.Tensor, pad_id: int = C.PAD_ID) -> torch.Tensor:
        """Scalar reward per row: hidden state at the last non-pad position."""
        with torch.no_grad():
            src = self.backbone
            batch = input_ids.size(0)
            h = src.get_hidden_states(input_ids)  # (B, S, dim)
        lengths = (input_ids != pad_id).sum(dim=1) - 1
        idx = lengths.clamp(min=0)  # (B,)
        hid = h[torch.arange(batch, device=h.device), idx]  # (B, dim)
        return self.reward_head(hid).squeeze(-1)


def prompt_from(row: dict) -> str:
    inst = row["instruction"].strip()
    inp = (row.get("input") or "").strip()
    if inp:
        return f"{inst}\n\nUser: {inp}\n\nAssistant:"
    return f"{inst}\n\nAssistant:"


def tokenize(tok, text: str, max_len: int = 256) -> list[int]:
    ids = tok.encode(text, add_special_tokens=False).ids
    return ids[: max_len - 1] + [C.EOS_ID]


def ctx_ids(tok, row: dict, max_len: int = 256) -> torch.Tensor:
    """Full (prompt + response) token row padded to max_len."""
    ids = tokenize(tok, prompt_from(row)) + tokenize(tok, row["output"])
    ids = ids[:max_len]
    return torch.tensor(ids + [C.PAD_ID] * (max_len - len(ids)), dtype=torch.long)


def empathy_target(response: str) -> float:
    """Affective consistency label (0..1): how supportive is the response.

    Uses the VAD model: empathy/support correlates with moderate-high valence,
    moderated arousal, and positive dominance — scaled into a soft target.
    """
    st = analyze(response)
    v, a, d = st.vad
    val = 0.5 + 0.5 * v
    # de-escalation: prefer not elevating arousal when user is distressed; here
    # we reward calm, controlled support.
    calm = 1.0 - min(1.0, abs(a))
    agency = 0.5 + 0.4 * d
    conf = min(1.0, st.confidence * 2.0)
    score = 0.5 * val + 0.25 * calm + 0.25 * agency
    return 0.3 + 0.7 * score * (0.6 + 0.4 * conf)


def make_batch(rows, tok, device):
    """A preference batch: (chosen_ctx, rejected_ctx, chosen_empathy_tgt)."""
    chos, rej, tgts = [], [], []
    for row in rows:
        chos.append(ctx_ids(tok, row))
        out = row["output"]
        # Rejected = reversed (non-empathetic) output string.
        rej_text = out[::-1]
        rej_row = dict(row)
        rej_row["output"] = rej_text
        rej.append(ctx_ids(tok, rej_row))
        tgts.append(empathy_target(out))
    return (
        torch.stack(chos).to(device),
        torch.stack(rej).to(device),
        torch.tensor(tgts, dtype=torch.float).to(device),
    )


def train(args) -> None:
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(f"[reward] device = {device}")
    tok = load_tokenizer()
    rows = [json.loads(l) for l in open(SFT_JSONL, encoding="utf-8")]
    print(f"[reward] {len(rows)} rows")

    # Initialise backbone from the trained SFT model (native .pt uses the model's
    # own state_dict keys, so it loads directly).
    backbone = SenlightCoder().to(device)
    sft_pt = C.ARTIFACTS_DIR / f"{args.sft}.pt"
    if sft_pt.exists():
        backbone.load_state_dict(torch.load(sft_pt, map_location=device))
        print(f"[reward] loaded SFT backbone from {sft_pt}")
    else:
        print("[reward] WARNING: no SFT backbone found; using random init.")

    model = AffectiveRewardModel(backbone).to(device)
    n_params = sum(p.numel() for p in model.parameters() if p.requires_grad)
    print(f"[reward] params (incl. backbone) = {n_params:,}")

    opt = AdamW(model.parameters(), lr=args.lr, weight_decay=0.05)
    scaler = torch.amp.GradScaler("cuda", enabled=args.amp)
    autocast = torch.amp.autocast("cuda", dtype=torch.float16) if args.amp else torch.enable_grad()

    model.train()
    for step in range(1, args.steps + 1):
        idxs = torch.randint(0, len(rows), (args.batch_size,), generator=RNG).tolist()
        c, r, tgt = make_batch([rows[i] for i in idxs], tok, device)
        opt.zero_grad(set_to_none=True)
        with autocast:
            rc = model.reward(c)
            rr = model.reward(r)
            # Bradley–Terry ranking: chosen should beat rejected.
            diff = rc - rr
            btl = -F.logsigmoid(diff).mean()
            # Affective MSE: chosen reward should track empathy target.
            mse = F.mse_loss(rc, tgt)
            loss = btl + args.aff_w * mse
        if scaler:
            scaler.scale(loss).backward()
            scaler.unscale_(opt)
            torch.nn.utils.clip_grad_norm_(model.parameters(), 1.0)
            scaler.step(opt); scaler.update()
        else:
            loss.backward()
            torch.nn.utils.clip_grad_norm_(model.parameters(), 1.0)
            opt.step()

        if step % args.log_every == 0:
            print(f"[reward] step {step:>5}/{args.steps} loss {loss.item():.4f} "
                  f"btl {btl.item():.4f} mse {mse.item():.4f}")

    # Save reward head + a convenience full model.
    out = C.ARTIFACTS_DIR / f"{args.out}.pt"
    torch.save(model.state_dict(), out)
    from safetensors.torch import save_file
    save_file(
        {"reward_head.weight": model.reward_head.weight.detach().cpu()},
        str(C.ARTIFACTS_DIR / f"{args.out}_head.safetensors"),
    )
    print(f"[reward] saved -> {out} and {args.out}_head.safetensors")


def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument("--steps", type=int, default=1500)
    p.add_argument("--batch-size", type=int, default=4)
    p.add_argument("--lr", type=float, default=3e-5)
    p.add_argument("--aff-w", type=float, default=0.5)
    p.add_argument("--log-every", type=int, default=150)
    p.add_argument("--sft", default="senlight_psy_sft")
    p.add_argument("--sft-weights", default="")
    p.add_argument("--out", default="affective_reward")
    p.add_argument("--amp", action="store_true", default=True)
    p.add_argument("--no-amp", action="store_true")
    return p.parse_args()


if __name__ == "__main__":
    a = parse_args()
    if a.no_amp:
        a.amp = False
    train(a)