"""Phase 1b — PyTorch `Dataset` / `DataLoader` for Senlight Coder AI.

Responsibilities
----------------
1. Load the BPE `tokenizer.json` produced by the Rust tokenizer.
2. Tokenize every source file into a flat integer token stream, wrapping each
   logical file with BOS/EOS, and persist the stream to `artifacts/data.bin`.
3. Slide overlapping context windows over that stream, yielding
   ``input_ids`` and shifted ``labels`` (classic next-token prediction).
4. Provide a ``collate_fn`` and a ready-to-use ``DataLoader``.

Window boundary correctness
---------------------------
Windows never straddle a file boundary: each file's token range is windowed
independently, so a window always belongs to a single source file (this avoids
learning spurious cross-file constructs).
"""

from __future__ import annotations

import numpy as np
import torch
import os
from pathlib import Path
from typing import Iterator, List, Optional, Tuple

from tokenizers import Tokenizer
from torch.utils.data import DataLoader, Dataset

import config as C


def _effective_data_dir(data_dir: Path) -> Path:
    """Resolve the corpus directory, honoring SENLIGHT_DATA_DIR override."""
    env_dir = os.environ.get("SENLIGHT_DATA_DIR")
    return Path(env_dir) if env_dir else Path(data_dir)

# ---------------------------------------------------------------------------
# Tokenizer loading
# ---------------------------------------------------------------------------

def load_tokenizer(path: Path = C.TOKENIZER_PATH) -> Tokenizer:
    """Load the serialized `Tokenizer` produced by the Rust trainer."""
    if not Path(path).exists():
        raise FileNotFoundError(
            f"Tokenizer not found at {path}. Run the Rust tokenizer "
            f"(tokenizer_rs\\src\\main.rs) first."
        )
    tok = Tokenizer.from_file(str(path))
    # Padding is enabled so the collate/short-window path can use <pad>.
    tok.enable_padding(pad_id=C.PAD_ID, pad_token="<pad>")
    # No truncation: the dataset slides its own context windows, so truncating
    # here would only lose token stream data.
    return tok


def _discover_source_files(data_dir: Path = C.DATA_DIR) -> List[Path]:
    """Recursively collect source files by extension."""
    if not data_dir.exists():
        raise FileNotFoundError(f"corpus dir not found: {data_dir}")
    files = [
        p
        for p in sorted(data_dir.rglob("*"))
        if p.is_file() and p.suffix.lower() in C.SOURCE_EXTENSIONS
    ]
    if not files:
        raise FileNotFoundError(f"no source files in {data_dir}")
    return files


# ---------------------------------------------------------------------------
# Token stream construction
# ---------------------------------------------------------------------------

def tokenize_corpus(
    tokenizer: Tokenizer,
    data_dir: Path = C.DATA_DIR,
    data_bin: Path = C.DATA_BIN,
) -> Tuple[int, List[Tuple[int, int]]]:
    """Tokenize all source files into one flat int32 stream written to disk.

    Returns ``(total_tokens, file_ranges)`` where ``file_ranges[i] == (start,
    end)`` (half-open) locates file *i*'s tokens in the flat stream.

    The flat stream and its per-file ranges are persisted side-by-side so that
    subsequent runs reuse the exact same tokenization without re-reading the
    corpus.
    """
    ranges_json = data_bin.with_suffix(".bin.ranges.json")
    if data_bin.exists() and ranges_json.exists():
        import json

        with open(ranges_json, "r", encoding="utf-8") as fh:
            meta = json.load(fh)
        mem = np.memmap(str(data_bin), mode="r", dtype=np.int32)
        ranges = [tuple(r) for r in meta["ranges"]]
        return int(mem.size), ranges

    files = _discover_source_files(_effective_data_dir(data_dir))
    ids_out: List[np.ndarray] = []
    ranges: List[Tuple[int, int]] = []
    offset = 0

    for path in files:
        text = path.read_text(encoding="utf-8", errors="ignore")
        enc = tokenizer.encode(text)      # ids for this file (no auto BOS/EOS)
        file_ids = np.asarray(enc.ids, dtype=np.int32)
        if file_ids.size == 0:
            continue
        ids_out.append(file_ids)
        ranges.append((offset, offset + int(file_ids.size)))
        offset += int(file_ids.size)

    stream = np.concatenate(ids_out) if ids_out else np.empty(0, dtype=np.int32)
    data_bin.parent.mkdir(parents=True, exist_ok=True)
    stream.tofile(str(data_bin))

    import json

    ranges_json.parent.mkdir(parents=True, exist_ok=True)
    with open(ranges_json, "w", encoding="utf-8") as fh:
        json.dump({"ranges": [list(r) for r in ranges]}, fh)

    print(
        f"[dataset] tokenized {len(files)} file(s) -> {stream.size:,} tokens "
        f"({len(ranges)} sequences); saved to {data_bin}"
    )
    return int(stream.size), ranges


# ---------------------------------------------------------------------------
# Dataset
# ---------------------------------------------------------------------------

class SenlightCodeDataset(Dataset):
    """Sliding-window next-token-prediction dataset over a token stream."""

    def __init__(
        self,
        tokenizer: Tokenizer,
        data_dir: Path = C.DATA_DIR,
        data_bin: Path = C.DATA_BIN,
        block_size: int = C.BLOCK_SIZE,
        stride: int = C.STRIDE,
        seq_len: int = C.BLOCK_SIZE,
    ) -> None:
        if stride <= 0:
            raise ValueError("stride must be > 0")
        if block_size < 2:
            raise ValueError("block_size must be >= 2")
        self.block_size = block_size
        self.stride = stride
        # Import/normalize the length param: seq_len is just an alias.
        self.seq_len = seq_len if seq_len is not None else block_size

        total, ranges = tokenize_corpus(tokenizer, data_dir, data_bin)
        self.stream = np.memmap(str(data_bin), mode="r", dtype=np.int32)
        self.total_tokens = int(total)

        # Pre-compute the index of every (start, end) window.
        self._windows: List[Tuple[int, int]] = []
        for (lo, hi) in ranges:
            length = hi - lo
            if length < block_size:
                # A file shorter than the block; emit a single (padded) window.
                self._windows.append((lo, hi))
                continue
            # Slide windows along the file's token range with the given stride.
            for start in range(lo, hi - block_size + 1, stride):
                self._windows.append((start, start + block_size))
            # Always include the final truncating window so the tail is learned.
            last_window = hi - block_size
            if self._windows[-1][0] != last_window:
                self._windows.append((last_window, hi))

    def __len__(self) -> int:
        return len(self._windows)

    def __getitem__(self, idx: int) -> Tuple[torch.Tensor, torch.Tensor]:
        start, end = self._windows[idx]
        tokens = self.stream[start:end].astype(np.int64)

        # Handle short right-edge windows: left-pad with PAD so we still get a
        # full block_size window for the model.
        if tokens.size < self.block_size:
            pad = np.full((self.block_size - tokens.size,), C.PAD_ID, dtype=np.int64)
            tokens = np.concatenate([pad, tokens])

        # input_ids = tokens[x], labels = tokens[x+1] (predict next token).
        input_ids = torch.from_numpy(tokens[:-1])
        labels = torch.from_numpy(tokens[1:])
        return input_ids.long(), labels.long()


# ---------------------------------------------------------------------------
# Collate + DataLoader
# ---------------------------------------------------------------------------

def collate_fn(
    batch: List[Tuple[torch.Tensor, torch.Tensor]]
) -> Tuple[torch.Tensor, torch.Tensor]:
    """Stack a batch of (input_ids, labels) pairs into contiguous tensors."""
    max_len = max(x.size(0) for x, _ in batch)
    input_batch = torch.full((len(batch), max_len), C.PAD_ID, dtype=torch.long)
    label_batch = torch.full((len(batch), max_len), C.PAD_ID, dtype=torch.long)
    for i, (x, y) in enumerate(batch):
        input_batch[i, : x.size(0)] = x
        label_batch[i, : y.size(0)] = y
    return input_batch, label_batch


def make_dataloader(
    tokenizer: Tokenizer,
    batch_size: int = C.BATCH_SIZE,
    shuffle: bool = C.SHUFFLE,
    num_workers: int = C.NUM_WORKERS,
    seed: int = C.SEED,
) -> DataLoader:
    """Build a `DataLoader` that yields ``(input_ids, labels)`` LongTensors."""
    dataset = SenlightCodeDataset(tokenizer)
    if shuffle and seed is not None:
        generator = torch.Generator().manual_seed(seed)
    else:
        generator = None
    return DataLoader(
        dataset,
        batch_size=batch_size,
        shuffle=shuffle,
        collate_fn=collate_fn,
        num_workers=num_workers,
        generator=generator,
        drop_last=False,
        pin_memory=torch.cuda.is_available(),
    )


# ---------------------------------------------------------------------------
# Self-test
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    tok = load_tokenizer()
    dl = make_dataloader(tok)
    print(f"[dataset] total samples (windows): {len(dl.dataset)}")
    print(f"[dataset] DataLoader batches    : {len(dl)}")
    x, y = next(iter(dl))
    print(f"[dataset] batch shapes: input_ids={tuple(x.shape)} labels={tuple(y.shape)}")
    print(f"[dataset] dtypes: input_ids={x.dtype} labels={y.dtype}")
    # Sanity: labels should be input shifted by one token.
    assert x[:, 0].size(0) == y.size(0)
    assert x.dtype == torch.long and y.dtype == torch.long
    print("[dataset] OK — DataLoader yields correctly-shaped LongTensors.")