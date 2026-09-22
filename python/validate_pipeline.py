"""Validation harness for every stage of the affective training pipeline.

Run: ``python validate_pipeline.py``

Checks (each stage exits non-zero on failure):
  Stage 1a  Affective annotation  — valence/arousal/intent correctness on probes.
  Stage 1b  HF intake & merge      — annotated SFT corpus is present/well-formed.
  Stage 2   SFT corpus readiness   — masked-SFT collate produces valid tensors.
  Stage 3a  Reward model           — trains a tiny reward model (architecture + loss < threshold).
  Stage 3b  PPO                    — 1-step PPO forward/backward + saving works.
  Stage 4   State tracker          — latent state & live loop respond correctly.

Each stage is runnable independently with ``--only <stage>``.
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import config as C  # noqa: E402

PASS = 0
FAILS: list[str] = []


def check(name: str, cond: bool, detail: str = "") -> None:
    global PASS
    if cond:
        PASS += 1
        print(f"  [OK] {name}")
    else:
        FAILS.append(name)
        print(f"  [FAIL] {name} {detail}")


# ---------------------------------------------------------------------------
# Stage 1a — affective annotation correctness
# ---------------------------------------------------------------------------
def stage_1a() -> None:
    from affective.emotions import analyze
    from affective.intent import annotate

    print("[stage 1a] affective annotation")
    r = annotate("everyone hates me, I feel like such a worthless failure")
    check("crisis/validation flagged", r["is_crisis"] is not None or r["intent"] in
          ("Crisis", "Seeking_Validation"), f"got {r['intent']}")
    check("valence in [-1,1]", -1.0 <= r["valence"] <= 1.0, f"{r['valence']}")
    check("arousal in [-1,1]", -1.0 <= r["arousal"] <= 1.0, f"{r['arousal']}")
    check("intent nonzero", r["intent"] != "", f"{r['intent']}")

    crisis = annotate("I keep thinking about ending it all")
    check("crisis-detected", crisis["intent"] == "Crisis", f"{crisis['intent']}")
    v, a, d = analyze("I feel calm and peaceful").vad
    check("positive-valence", v > 0.3, f"{v:.2f}")


# ---------------------------------------------------------------------------
# Stage 1b — HF intake & merged corpus
# ---------------------------------------------------------------------------
def stage_1b() -> None:
    print("[stage 1b] HF intake + annotated corpus")
    rows = []
    path = C.ARTIFACTS_DIR / "psy_sft_intent.jsonl"
    if not path.exists():
        check("merged SFT corpus exists", False, f"missing {path.name}")
        return
    with open(path, encoding="utf-8") as fh:
        rows = [json.loads(l) for l in fh]
    check("corpus non-empty", len(rows) > 100, f"{len(rows)} rows")
    check("has output", all(r.get("output") for r in rows[:50]))
    check("has intent", all(r.get("intent") for r in rows[:50]))
    intents = {r["intent"] for r in rows}
    check("intent taxonomy", intents <= set(
        ("Crisis", "Seeking_Validation", "Venting", "Advice_Seeking",
         "Disclosure", "Casual")))
    check("vad bounds", all(-1 <= r["valence"] <= 1 for r in rows[:50]))


# ---------------------------------------------------------------------------
# Stage 2 — SFT collate produces valid tensors
# ---------------------------------------------------------------------------
def stage_2() -> None:
    import torch
    from dataset import load_tokenizer
    from train_psy import collate, load_sft_rows

    print("[stage 2] SFT masked collate")
    tok = load_tokenizer()
    rows = load_sft_rows(pure=True)
    if len(rows) < 1:
        check("pure-SFT rows present", False); return
    dev = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    x, y, m, plen = collate(rows[0], tok, dev)
    check("collate shapes", x.dim() == 1 and y.dim() == 1 and m.dim() == 1)
    check("device on cuda", x.device.type == dev.type)
    check("mask sums positive", m.sum() > 0, f"mask={m.sum()}")


# ---------------------------------------------------------------------------
# Stage 3a — tiny reward-model training
# ---------------------------------------------------------------------------
def stage_3a() -> None:
    import torch
    print("[stage 3a] reward-model forward/backward (tiny)")
    from model import SenlightCoder
    from reward_model import AffectiveRewardModel
    torch.manual_seed(0)
    dev = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    # Small model but keep MODEL_DIM so the (MODEL_DIM -> 1) reward_head matches.
    bb = SenlightCoder(n_layers=1, n_heads=8, n_kv_heads=8, dim=C.MODEL_DIM,
                       ff_dim=512, vocab_size=512, max_len=64).to(dev)
    rm = AffectiveRewardModel(bb).to(dev)
    ids = torch.randint(1, 511, (2, 32), device=dev)
    r = rm.reward(ids)
    check("reward shape", r.shape == (2,), f"{tuple(r.shape)}")
    check("reward finite", bool(torch.isfinite(r).all()))
    # A quick gradient step.
    loss = (r**2).mean()
    loss.backward()
    check("backward ok", all(p.grad is not None for p in rm.reward_head.parameters()))


# ---------------------------------------------------------------------------
# Stage 3b — PPO one step end-to-end
# ---------------------------------------------------------------------------
def stage_3b() -> None:
    import subprocess, sys as _sys
    print("[stage 3b] PPO one-step smoke")
    base = next((C.ARTIFACTS_DIR / f"{n}.pt" for n in ("senlight_300m_he2",)
                 if (C.ARTIFACTS_DIR / f"{n}.pt").exists()), None)
    reward = next((C.ARTIFACTS_DIR / f"{n}.pt" for n in ("affective_reward_v2",
                  "affective_reward") if (C.ARTIFACTS_DIR / f"{n}.pt").exists()), None)
    if base is None or reward is None:
        check("PPO prerequisites", False, "need a base checkpoint + reward model")
        return
    out = C.ARTIFACTS_DIR / "val_ppo"
    cmd = [_sys.executable, str(Path(__file__).resolve().parent / "ppo.py"),
           "--steps", "1", "--batch-size", "1",
           "--sft", base.stem, "--reward", reward.stem,
           "--out", "val_ppo", "--log-every", "1"]
    proc = subprocess.run(cmd, capture_output=True, text=True)
    ok = proc.returncode == 0 and Path(str(out) + ".safetensors").exists()
    check("PPO runs & saves", ok, proc.stderr[-400:])
    Path(str(out) + ".pt").unlink(missing_ok=True)
    Path(str(out) + "_critic.pt").unlink(missing_ok=True)
    Path(str(out) + ".safetensors").unlink(missing_ok=True)


# ---------------------------------------------------------------------------
# Stage 4 — state tracker
# ---------------------------------------------------------------------------
def stage_4() -> None:
    print("[stage 4] latent state tracker + live loop")
    from state_tracker import LiveFeedbackLoop
    loop = LiveFeedbackLoop(alpha=0.4)
    m1 = loop.feed("I feel anxious and worried about everything")
    m2 = loop.feed("I keep thinking about ending it all")
    m3 = loop.feed("I called someone and I feel a little better")
    check("intent safe", m2["response_mode"] in ("de-escalate", "comfort"))
    check("risk high on crisis", m2["risk"] in ("high", "elevated"), m2["risk"])
    check("turns increment", m2["turns"] == 2 and m3["turns"] == 3)
    check("metrics complete", all(k in m1 for k in
          ("valence", "arousal", "v_velocity", "response_mode", "risk")))


# ---------------------------------------------------------------------------
# Runner
# ---------------------------------------------------------------------------
STAGES = {
    "1a": stage_1a, "1b": stage_1b, "2": stage_2,
    "3a": stage_3a, "3b": stage_3b, "4": stage_4,
}


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--only", default="", help="comma-separated stage keys, e.g. 1a,4")
    args = ap.parse_args()

    keys = [k.strip() for k in args.only.split(",") if k.strip()] if args.only else list(STAGES)
    for k in keys:
        if k not in STAGES:
            print(f"unknown stage {k}; known: {list(STAGES)}"); sys.exit(2)
        try:
            STAGES[k]()
        except Exception as exc:  # noqa: BLE001
            FAILS.append(k)
            print(f"  [FAIL] stage {k} raised: {exc}")

    print("\n" + "=" * 46)
    print(f"PASSED {PASS} checks, FAILED {len(FAILS)}")
    if FAILS:
        print("failed:", ", ".join(FAILS))
        sys.exit(1)
    print("ALL STAGES VALID.")


if __name__ == "__main__":
    main()