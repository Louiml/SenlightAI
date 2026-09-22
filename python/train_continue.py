"""Continued pretraining campaign toward high token counts (resumable).

Extends the existing ``train.py`` engine into a *campaign* that keeps training
the SAME model (``--base``) until a cumulative **token milestone** is reached,
saving a resumable checkpoint at a fixed cadence so progress survives across
sessions and crashes.

Key behaviours
--------------
* Auto-resume: picks the latest ``checkpoint_step*.pt`` (or ``--base``) and the
  step/loss/optimizer state from it, so the token count is cumulative.
* Configurable budget: ``PY_TRAIN_TOKEN_TARGET`` or ``--milestone-tokens``
  (default 1_000_000_000) and a maximum wall-clock ``--max-minutes`` per run
  (so you can run it in hourly slices).
* Continued-training LR: uses a low LR (default 1e-4) so learned features are
  refined, not destroyed.
* Emits ``artifacts/senlight_campaign.status.json`` with live progress and a
  ``generate`` self-check after each save to show the model learns and "keeps
  responsive".

The engine itself (forward/backward/AMP/scheduler) lives in ``train.py``; this
script only wraps it with resume + milestone + status reporting.

Usage
-----
    python train_continue.py --milestone-tokens 500000000 --max-minutes 120 \
        --base senlight_300m_he2 --out senlight_500m
Run repeatedly (same args) to resume until the milestone is reached.
"""

from __future__ import annotations

import argparse
import json
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import config as C  # noqa: E402
from dataset import load_tokenizer  # noqa: E402
from model import SenlightCoder  # noqa: E402

ART = C.ARTIFACTS_DIR
STATUS_FILE = ART / "senlight_campaign.status.json"
TOKENS_PER_STEP = C.BATCH_SIZE * C.BLOCK_SIZE  # 8 * 512 = 4096


def _jsonlines(path: Path) -> list[dict]:
    if not path.exists():
        return []
    with open(path, encoding="utf-8") as fh:
        return [json.loads(l) for l in fh]


def latest_checkpoint() -> Path | None:
    cks = sorted(ART.glob("checkpoint_step*.pt"),
                 key=lambda p: int(p.stem.split("_")[1].replace("step", "")),
                 reverse=True)
    return cks[0] if cks else None


def _status() -> dict:
    if not STATUS_FILE.exists():
        return {"tokens_seen": 0, "step": 0}
    with open(STATUS_FILE, encoding="utf-8") as fh:
        return json.load(fh)


def generation_check(prompt: str, max_new: int = 32) -> str:
    """Quick responsiveness probe on the current weights (also logged to status)."""
    import torch
    import torch.nn.functional as F
    try:
        tok = load_tokenizer(); dev = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        m = SenlightCoder().to(dev).eval()
        m.load_state_dict(torch.load(_last_pt(), map_location=dev), strict=False)
        ids = tok.encode(prompt, add_special_tokens=False).ids
        out = list(ids)
        with torch.no_grad():
            for _ in range(max_new):
                inp = torch.tensor(out[-512:], device=dev).unsqueeze(0)
                logits, _ = m(inp)
                p = F.softmax(logits[0, -1], dim=-1)
                n = int(torch.multinomial(p, 1).item())
                if n == C.EOS_ID:
                    break
                out.append(n)
        return tok.decode(out[len(ids):], True)
    except Exception as exc:  # noqa: BLE001
        return f"<gen-check failed: {exc}>"


def _last_pt() -> Path:
    lt = latest_checkpoint()
    if lt is not None:
        return lt
    # fall back to a normal full-model export name if no step ckpt exists
    cands = sorted(ART.glob("*.pt"),
                   key=lambda p: p.stat().st_mtime, reverse=True)
    return cands[0] if cands else ART / "senlight.pt"


def main() -> None:
    ap = argparse.ArgumentParser(description="Resumable continued-pretraining campaign")
    ap.add_argument("--base", default="senlight_300m_he2",
                    help="stem of the base .pt checkpoint to continue from")
    ap.add_argument("--milestone-tokens", type=int,
                    default=int(__import__("os").environ.get(
                        "PY_TRAIN_TOKEN_TARGET", "500000000")),
                    help="cumulative tokens to reach (default 500M)")
    ap.add_argument("--max-minutes", type=int, default=120,
                    help="hard wall-clock cap for this invocation")
    ap.add_argument("--out", default="senlight_500m",
                    help="output stem for the final safetensors/pt")
    ap.add_argument("--save-every", type=int, default=5000,
                    help="checkpoint + gen-check cadence (steps)")
    ap.add_argument("--log-every", type=int, default=200)
    ap.add_argument("--lr", type=float, default=1e-4)
    ap.add_argument("--batch-size", type=int, default=C.BATCH_SIZE)
    args = ap.parse_args()

    status = _status()
    tokens_done = int(status.get("tokens_seen", 0))
    # If previous runs already passed the milestone we're done.
    if tokens_done >= args.milestone_tokens:
        print(f"[campaign] already at {tokens_done:,} >= {args.milestone_tokens:,} "
              f"target; nothing to do.")
        return

    # Steps remaining to the milestone at the current batch size. The engine
    # trains toward the *additional* tokens needed, so cumulative progress is exact.
    additional_steps = (args.milestone_tokens - tokens_done) // TOKENS_PER_STEP + 1
    # Cap by wall-clock so a single invocation can be run in a limited window.
    est_steps_per_min = 26  # ~0.44 step/s * 60 = 26.4 ; tune to measured rate
    wall_cap = min(additional_steps, max(1, args.max_minutes * est_steps_per_min))
    additional_tokens = wall_cap * TOKENS_PER_STEP
    print(f"[campaign] tokens done {tokens_done:,}/{args.milestone_tokens:,}"
          f" -> need {additional_steps:,} steps; capping this run at {wall_cap:,} steps "
          f"(~{args.max_minutes} min).")

    # Delegate the actual training to the (now resumable) train engine.
    from train import train as engine
    from types import SimpleNamespace

    # Prefer the latest step-checkpoint (weights + optimizer resumed); otherwise
    # fall back to the base full-model export.
    lt = latest_checkpoint()
    continue_from = lt.stem if lt is not None else args.base

    eng_args = SimpleNamespace(
        steps=wall_cap, batch_size=args.batch_size, lr=args.lr,
        warmup_steps=min(200, wall_cap // 10), num_workers=0,
        log_every=args.log_every, save_every=args.save_every,
        amp=True, continue_from=continue_from,
        token_budget=additional_tokens, out=args.out,
    )
    t0 = time.time()
    engine(eng_args)

    # Persist campaign status so the next invocation resumes from where we left.
    tokens_now = tokens_done + additional_tokens
    status.update({
        "tokens_seen": tokens_now,
        "last_step": latest_checkpoint().stem if latest_checkpoint() else "",
        "milestone": args.milestone_tokens,
        "updated": time.strftime("%Y-%m-%d %H:%M:%S"),
    })
    with open(STATUS_FILE, "w", encoding="utf-8") as fh:
        json.dump(status, fh, indent=2)

    # Responsiveness probe at the end of this slice.
    probe_prompt = "The user says: I am feeling really anxious and hopeless today.\n\nAssistant:"
    gen = generation_check(probe_prompt)
    status["last_gen"] = gen
    with open(STATUS_FILE, "w", encoding="utf-8") as fh:
        json.dump(status, fh, indent=2)
    print(f"[campaign] slice done in {(time.time()-t0)/60:.1f} min; ~{tokens_now:,} tokens total.")
    print(f"[campaign] gen-check: {gen}")


if __name__ == "__main__":
    main()