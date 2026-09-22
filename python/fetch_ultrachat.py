"""Download a slice of HuggingFaceH4/ultrachat_200k for fluency grounding.

Converts each multi-turn conversation into instruction/input/output rows
(assistant reply for the latest user turn), matching the SFT pipeline format.
Writes ``research/ultrachat.jsonl``.

Usage: python fetch_ultrachat.py --rows 25000
"""

from __future__ import annotations

import argparse
import json
import time
import urllib.request
from pathlib import Path

BASE = "https://datasets-server.huggingface.co/rows?dataset=HuggingFaceH4%2Fultrachat_200k&config=default&split=train_sft&offset={o}&length=100"
OUT = Path(__file__).resolve().parent.parent / "research" / "ultrachat.jsonl"


def to_sft(messages) -> dict | None:
    """Turn a chat message list into a (prompt_hint, output) row."""
    if isinstance(messages, str):
        return None  # already-ish; skip
    turns = []
    for m in messages:
        if isinstance(m, dict):
            turns.append((m.get("role") or "user", m.get("content") or ""))
        else:
            return None
    # find the last user->assistant pair we can use
    for i in range(len(turns) - 1, -1, -1):
        if turns[i][0] == "assistant":
            # prompt = concatenation up to this assistant turn
            prompt = "\n".join(f"{r}: {c}" for r, c in turns[: i + 1])
            return {"instruction": "Continue this conversation helpfully.",
                    "input": prompt}
    return None


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--rows", type=int, default=25000)
    args = ap.parse_args()

    OUT.parent.mkdir(parents=True, exist_ok=True)
    # Resume if partially present (count existing).
    n0 = sum(1 for _ in open(OUT, encoding="utf-8")) if OUT.exists() else 0
    out = open(OUT, "a", encoding="utf-8") if n0 else open(OUT, "w", encoding="utf-8")
    offset = n0
    written = n0
    while written < args.rows and offset < 300_000:
        try:
            d = json.loads(urllib.request.urlopen(BASE.format(o=offset), timeout=60).read())
        except Exception:
            time.sleep(4)
            continue
        rows = d.get("rows", [])
        if not rows:
            break
        for r in rows:
            row = r.get("row", {})
            msgs = row.get("messages") or []
            if not isinstance(msgs, list):
                continue
            convo = to_sft(msgs)
            if convo is None:
                continue
            user_turns = [m for m in msgs if m.get("role") == "user" or m.get("role") == "human"]
            asst_turns = [m for m in msgs if m.get("role") == "assistant" or m.get("role") == "gpt"]
            if not user_turns or not asst_turns:
                continue
            last_user = user_turns[-1]["content"] or ""
            last_asst = asst_turns[-1]["content"] or ""
            if not last_user or not last_asst:
                continue
            rec = {
                "instruction": convo["instruction"],
                "input": last_user,
                "output": last_asst.strip(),
            }
            out.write(json.dumps(rec, ensure_ascii=False) + "\n")
            written += 1
            if written >= args.rows:
                break
        offset += len(rows)
        if offset % 5000 == 0:
            print(f"[ultrachat] offset {offset} rows written {written}", flush=True)
    out.close()
    print(f"[ultrachat] DONE: {sum(1 for _ in open(OUT, encoding='utf-8'))} rows -> {OUT}")


if __name__ == "__main__":
    main()