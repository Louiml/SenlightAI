"""Step 1 (finalize) — merge affective/HF intake into the SFT corpus + intent tags.

Reads the existing ``psy_sft.jsonl`` (from ``prep_psy.py``) and the HF intake
``affective_intake.jsonl`` (from ``hf_datasets.py``), back-fills / re-computes the
**intent** tag on every row (via ``affective.intent``), deduplicates, and writes
a single merged corpus ``psy_sft_intent.jsonl``.

The merged file has the canonical annotation columns used downstream by the SFT
(``train_psy.py``), the reward model (``reward_model.py``) and PPO (``ppo.py``):

    instruction, input, output, valence, arousal, dominance,
    intent, emotion, plutchik, source

Usage:
    python intent_prep.py [--pure]
"""

from __future__ import annotations

import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import config as C  # noqa: E402
from affective.emotions import analyze  # noqa: E402
from affective.intent import classify_intent  # noqa: E402

ART = C.ARTIFACTS_DIR
EXISTING_SFT = ART / "psy_sft.jsonl"
EXISTING_PURE = ART / "psy_sft_pure.jsonl"
INTAKE = ART / "affective_intake.jsonl"
OUT = ART / "psy_sft_intent.jsonl"
OUT_PURE = ART / "psy_sft_intent_pure.jsonl"


def _load_jsonl(path: Path) -> list[dict]:
    if not path.exists():
        print(f"[intent_prep] skip missing {path.name}")
        return []
    with open(path, encoding="utf-8") as fh:
        return [json.loads(line) for line in fh]


def _annotate(row: dict) -> dict:
    probe = (row.get("input") or "").strip() or row.get("text") or ""
    if not probe:
        probe = (row.get("instruction") or "").strip()
    st = analyze(probe)
    it = classify_intent(st)
    v, a, d = st.vad
    out = dict(row)
    out["valence"] = round(v, 3)
    out["arousal"] = round(a, 3)
    out["dominance"] = round(d, 3)
    out["intent"] = it.intent
    out["emotion"] = st.emotion
    out["plutchik"] = st.plutchik
    out.pop("text", None)
    return out


def main() -> None:
    pure = "--pure" in sys.argv

    existing = _load_jsonl(EXISTING_PURE if pure else EXISTING_SFT)
    intake = _load_jsonl(INTAKE)
    print(f"[intent_prep] existing={len(existing)} intake={len(intake)}")

    merged: list[dict] = []
    seen: set[str] = set()
    for row in existing + intake:
        out = (row.get("output") or "").strip()
        if not out:  # SFT rows need a gold response to fine-tune on.
            continue
        key = ((row.get("input") or "") + "|" + out).lower().strip()
        if not key or key in seen:
            continue
        seen.add(key)
        merged.append(_annotate(row))

    ART.mkdir(parents=True, exist_ok=True)
    path = OUT_PURE if pure else OUT
    with open(path, "w", encoding="utf-8") as fh:
        for r in merged:
            fh.write(json.dumps(r, ensure_ascii=False) + "\n")

    from collections import Counter
    dist = Counter(r["intent"] for r in merged)
    print(f"[intent_prep] wrote {len(merged)} rows -> {path.name}")
    print("[intent_prep] intent distribution:", dict(dist.most_common()))


if __name__ == "__main__":
    main()