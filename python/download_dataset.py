"""Download a slice of the OpenBMB/UltraData-Code (cpp) dataset for training.

Pulls page-by-page through the HF datasets-server rows API and writes each
row's primary training text (``full_content`` = task + solution, typically in
solution-first order) as a ``.cpp`` file under ``<repo>/data/cpp/``.

Usage:
    python download_dataset.py --rows 5000 --offset 0 --out-dir ../data/cpp
"""

from __future__ import annotations

import argparse
import json
import time
import urllib.request
from pathlib import Path

import config as C

DATASET = "openbmb/UltraData-Code"
CONFIG = "UltraData-Code-L3"
SPLIT = "cpp"
BASE = (
    "https://datasets-server.huggingface.co/rows"
    f"?dataset={DATASET.replace('/', '%2F')}"
    f"&config={CONFIG}&split={SPLIT}"
)


def fetch_page(offset: int, length: int) -> list[dict]:
    url = f"{BASE}&offset={offset}&length={length}"
    for attempt in range(8):
        try:
            with urllib.request.urlopen(url, timeout=60) as resp:
                payload = json.loads(resp.read().decode("utf-8"))
            return payload.get("rows", [])
        except Exception as exc:  # noqa: BLE001
            # HTTP 429 = rate limited; back off much harder before retrying.
            wait = 5.0 * (2 ** attempt)
            print(f"[download] retry {attempt + 1}/8 for offset {offset} "
                  f"({exc}); waiting {wait:.0f}s...", flush=True)
            time.sleep(wait)
    raise RuntimeError(f"failed fetching offset {offset} after 8 attempts")


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--rows", type=int, default=5000)
    ap.add_argument("--offset", type=int, default=0)
    ap.add_argument("--per-page", type=int, default=100)
    ap.add_argument("--out-dir", default=str(C.DATA_DIR / "cpp"))
    ap.add_argument("--field", default="full_content")
    args = ap.parse_args()

    out = Path(args.out_dir)
    out.mkdir(parents=True, exist_ok=True)

    # Resume-friendly: keep an explicit pointer file so we never re-download
    # rows we've already consumed, and never duplicate files.
    marker = out / ".progress"
    initial = int(marker.read_text().strip()) if marker.exists() else args.offset
    offset = initial

    written = 0
    while written < args.rows:
        want = min(args.per_page, args.rows - written)
        rows = fetch_page(offset, want)
        if not rows:
            print(f"[download] no more rows at offset {offset}; stopping.")
            break
        for r in rows:
            row = r.get("row", {})
            text = (row.get(args.field) or "").strip()
            if len(text) < 32:
                continue
            uuid = (row.get("uuid") or f"row_{offset}").replace("-", "")
            target = out / f"{uuid}.cpp"
            if not target.exists():
                target.write_text(text + "\n", encoding="utf-8")
            written += 1
            if written >= args.rows:
                break
        offset += len(rows)
        marker.write_text(str(offset))  # persist position for resume
        print(f"[download] ...{written}/{args.rows} written (dataset offset {offset})",
              flush=True)

    total = len(list(out.glob("*.cpp")))
    print(f"[download] done: {total} .cpp files in {out}.")


if __name__ == "__main__":
    main()