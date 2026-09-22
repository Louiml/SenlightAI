"""Combine + prepare the psychological/empathetic corpora for Senlight training.

Merges `research/dataset.csv` (800 rows) and `research/Psychology-10K.json`
(9,846 rows) into one deduplicated instruction-tuning corpus. Each sample is
annotated with an affective state (VAD vector + Plutchik emotion + reference
emotion) computed by the embedded lexicon-based affective model.

Outputs:
  * ``<PROJECT>/artifacts/psy_sft.jsonl``  — SFT rows, one JSON per line:
        {instruction, input, output, emotion, plutchik, vad:[V,A,D], ref}
  * Writes synthesized conversation text into ``<PROJECT>/data/psy/`` so the
    existing tokenizer/dataset pipeline can be retrained on conversational text.
"""

from __future__ import annotations

import csv
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import config as C  # noqa: E402
from affective.emotions import analyze  # noqa: E402

RESEARCH = C.ROOT / "research"
OUT_JSONL = C.ARTIFACTS_DIR / "psy_sft.jsonl"
OUT_PURE_JSONL = C.ARTIFACTS_DIR / "psy_sft_pure.jsonl"
OUT_TXT_DIR = C.DATA_DIR / "psy"


def load_dataset_csv() -> list[dict]:
    rows = []
    with open(RESEARCH / "dataset.csv", encoding="utf-8", errors="ignore") as fh:
        for r in csv.DictReader(fh):
            if r.get("output") and r.get("instruction"):
                rows.append(r)
    return rows


def load_psych_10k() -> list[dict]:
    with open(RESEARCH / "Psychology-10K.json", encoding="utf-8") as fh:
        data = json.load(fh)
    out = []
    for r in data:
        if r.get("output") and (r.get("instruction") or r.get("input")):
            out.append(r)
    return out


def load_counsel_chat() -> list[dict]:
    """Convert nbertagnolli/counsel-chat Q/A rows into SFT instruction rows."""
    path = RESEARCH / "counsel_chat.jsonl"
    if not path.exists():
        print("[prep] counsel_chat.jsonl not found; skipping.")
        return []
    out = []
    with open(path, encoding="utf-8") as fh:
        for r in (json.loads(l) for l in fh):
            q = (r.get("questionText") or "").strip()
            a = (r.get("answerText") or "").strip()
            if not q or not a:
                continue
            out.append({
                "instruction": "As a compassionate therapist, respond warmly and "
                               "helpfully to this person reaching out for support.",
                "input": q,
                "output": a,
                "topic": r.get("topic", ""),
            })
    print(f"[prep] loaded {len(out)} counsel-chat Q/A pairs")
    return out


def load_alpaca() -> list[dict]:
    """Ground the model in fluent English with general instruction data.

    Alpaca is 52k high-quality, fluent instructive responses. Including a slice
    gives the 100M model a broad language base (fixes shaky prose) that the
    psychological SFT then styles as empathetic.
    """
    path = RESEARCH / "alpaca.jsonl"
    if not path.exists():
        print("[prep] alpaca.jsonl not found; skipping.")
        return []
    out = []
    with open(path, encoding="utf-8") as fh:
        for r in (json.loads(l) for l in fh):
            inst = (r.get("instruction") or "").strip()
            inp = (r.get("input") or "").strip()
            out_ = (r.get("output") or "").strip()
            if not inst or not out_:
                continue
            out.append({"instruction": inst, "input": inp, "output": out_})
    print(f"[prep] loaded {len(out)} alpaca instruction rows")
    return out


def load_ultrachat() -> list[dict]:
    """Add realistic human conversational data (HuggingFaceH4/ultrachat_200k) to
    boost language fluency. Excluded from the 'pure' psychological set."""
    path = RESEARCH / "ultrachat.jsonl"
    if not path.exists():
        print("[prep] ultrachat.jsonl not found; skipping.")
        return []
    out = []
    with open(path, encoding="utf-8") as fh:
        for r in (json.loads(l) for l in fh):
            inst = (r.get("instruction") or "").strip()
            inp = (r.get("input") or "").strip()
            out_ = (r.get("output") or "").strip()
            if not inst or not out_:
                continue
            out.append({"instruction": inst, "input": inp, "output": out_})
    print(f"[prep] loaded {len(out)} ultrachat conversation rows")
    return out


def load_greetings() -> list[dict]:
    """Basic greetings / small-talk so the model warms up politely and warmly."""
    path = RESEARCH / "greetings.jsonl"
    if not path.exists():
        print("[prep] greetings.jsonl not found; skipping.")
        return []
    out = []
    with open(path, encoding="utf-8") as fh:
        for r in (json.loads(l) for l in fh):
            inst = (r.get("instruction") or "").strip()
            inp = (r.get("input") or "").strip()
            out_ = (r.get("output") or "").strip()
            if not inst or not out_:
                continue
            out.append({"instruction": inst, "input": inp, "output": out_})
    print(f"[prep] loaded {len(out)} greeting/small-talk rows")
    return out


def load_hebrew() -> list[dict]:
    """Basic Hebrew greetings + basic knowledge so the model speaks and
    understands fluent Hebrew (greetings, polite phrases, numbers, days,
    colors, Israeli culture)."""
    path = RESEARCH / "hebrew.jsonl"
    if not path.exists():
        print("[prep] hebrew.jsonl not found; skipping.")
        return []
    out = []
    with open(path, encoding="utf-8") as fh:
        for r in (json.loads(l) for l in fh):
            inst = (r.get("instruction") or "").strip()
            inp = (r.get("input") or "").strip()
            out_ = (r.get("output") or "").strip()
            if not inst or not out_:
                continue
            out.append({"instruction": inst, "input": inp, "output": out_})
    print(f"[prep] loaded {len(out)} hebrew rows")
    return out


def make_prompt(instruction: str, user_input: str) -> str:
    """Render the instruction-tuning prompt (classic Alpaca-style)."""
    if user_input and user_input.strip():
        return f"{instruction}\n\nUser: {user_input}\n\nAssistant:"
    return f"{instruction}\n\nAssistant:"


def _tag(rows, source):
    for r in rows:
        r["source"] = source
    return rows


def annotate_and_write(rows: list[dict]) -> int:
    out_txt_dir = Path(OUT_TXT_DIR)
    out_txt_dir.mkdir(parents=True, exist_ok=True)
    (C.ARTIFACTS_DIR).mkdir(parents=True, exist_ok=True)

    seen: set[str] = set()
    n = 0
    pure = open(OUT_PURE_JSONL, "w", encoding="utf-8")
    try:
        with open(OUT_JSONL, "w", encoding="utf-8") as fh:
            for r in rows:
                instruction = (r.get("instruction") or "").strip()
                user_input = (r.get("input") or "").strip()
                output = (r.get("output") or "").strip()
                if not instruction or not output:
                    continue
                key = (instruction + "|" + output).lower()
                if key in seen:
                    continue
                seen.add(key)

                state = analyze(user_input or instruction)
                rec = {
                    "instruction": instruction,
                    "input": user_input,
                    "output": output,
                    "emotion": state.emotion,
                    "plutchik": state.plutchik,
                    "vad": list(state.vad),
                    "ref": state.plutchik,
                    "source": r.get("source", "unknown"),
                }
                fh.write(json.dumps(rec, ensure_ascii=False) + "\n")
                # pure file = psychological sources only (exclude fluency grounding)
                if rec.get("source") not in ("alpaca", "ultrachat"):
                    pure.write(json.dumps(rec, ensure_ascii=False) + "\n")
                n += 1
    finally:
        pure.close()
    return n


def write_conversation_texts(jsonl: Path) -> int:
    """Dump plain "prompt\\nresponse" text for tokenizer/pretrain retraining."""
    (C.DATA_DIR / "psy").mkdir(parents=True, exist_ok=True)
    chunks = []
    with open(jsonl, encoding="utf-8") as fh:
        for i, line in enumerate(fh):
            r = json.loads(line)
            chunks.append(make_prompt(r["instruction"], r["input"]) + "\n" + r["output"] + "\n")
            if len(chunks) >= 500:
                (Path(OUT_TXT_DIR) / f"psy_{i // 500}.txt").write_text(
                    "\n".join(chunks), encoding="utf-8")
                chunks = []
    if chunks:
        (Path(OUT_TXT_DIR) / "psy_tail.txt").write_text("\n".join(chunks), encoding="utf-8")
    return len(list(Path(OUT_TXT_DIR).glob("*.txt")))


def main() -> None:
    csv_rows = _tag(load_dataset_csv(), "dataset_csv")
    psy_rows = _tag(load_psych_10k(), "psych_10k")
    counsel_rows = _tag(load_counsel_chat(), "counsel_chat")
    alpaca_rows = _tag(load_alpaca(), "alpaca")
    ultra_rows = _tag(load_ultrachat(), "ultrachat")
    greet_rows = _tag(load_greetings(), "greetings")
    hebrew_rows = _tag(load_hebrew(), "hebrew")
    print(f"[prep] loaded {len(csv_rows)} csv + {len(psy_rows)} psych-10k + "
          f"{len(counsel_rows)} counsel-chat + {len(alpaca_rows)} alpaca + "
          f"{len(ultra_rows)} ultrachat + {len(greet_rows)} greetings + "
          f"{len(hebrew_rows)} hebrew rows")

    n = annotate_and_write(
        csv_rows + psy_rows + counsel_rows + alpaca_rows + ultra_rows
        + greet_rows + hebrew_rows
    )
    print(f"[prep] wrote {n} deduped SFT rows -> {OUT_JSONL}")

    # A quick emotion distribution summary.
    dist: dict[str, int] = {}
    with open(OUT_JSONL, encoding="utf-8") as fh:
        for line in fh:
            r = json.loads(line)
            dist[r["plutchik"]] = dist.get(r["plutchik"], 0) + 1
    print("[prep] Plutchik emotion distribution:")
    for k, c in sorted(dist.items(), key=lambda x: -x[1]):
        print(f"    {k:<14} {c}")

    nfiles = write_conversation_texts(OUT_JSONL)
    print(f"[prep] wrote {nfiles} conversation .txt files -> {OUT_TXT_DIR}")


if __name__ == "__main__":
    main()