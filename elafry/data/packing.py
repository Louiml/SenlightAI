"""Sequence packing.

Turns a list of documents into fixed-length blocks with loss masks, using the arrangement
the sibling implementation already established: a flat token stream plus per-document ranges,
sliding windows with a stride of half the block, and windows that never straddle a document
boundary.

That last property is the one worth defending. A window that runs off the end of one document
and into the start of the next trains the model on a transition that does not exist, and
with only a few hundred documents in a small corpus those phantom transitions are a
meaningful fraction of the training signal. Dropping the boundary window costs a little data
and removes the artefact.
"""

from __future__ import annotations

import json
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, List, Optional, Sequence, Tuple

import numpy as np

from elafry.data.contracts import PackedBlock

__all__ = [
    "TokenStream",
    "encode_document",
    "encode_sft_document",
    "pack_blocks",
    "save_stream",
    "load_stream",
]


@dataclass
class TokenStream:
    """A flat token array plus the ranges that say where each document starts and ends."""

    tokens: np.ndarray
    ranges: List[Tuple[int, int]]

    def __post_init__(self) -> None:
        if self.tokens.dtype != np.int32:
            self.tokens = self.tokens.astype(np.int32)
        self.ranges = [(int(a), int(b)) for a, b in self.ranges]

    def __len__(self) -> int:
        return int(self.tokens.shape[0])

    @property
    def n_documents(self) -> int:
        return len(self.ranges)

    def document(self, index: int) -> np.ndarray:
        lo, hi = self.ranges[index]
        return self.tokens[lo:hi]

    def blocks(self, block_size: int, stride: Optional[int] = None) -> Iterable[np.ndarray]:
        """Yield in-range windows. Windows crossing a boundary are skipped."""
        stride = stride or block_size // 2
        for lo, hi in self.ranges:
            length = hi - lo
            if length < 2:
                continue
            start = lo
            while start + block_size <= hi:
                yield self.tokens[start : start + block_size]
                start += stride
            # A trailing partial window is dropped rather than padded. Padding teaches the
            # model to predict pad tokens, which is why the mask exists in the first place.
            if length > block_size and (start - lo) % stride != 0:
                remainder = hi - start
                if remainder >= 2:
                    pass  # intentionally not emitted: short windows waste signal


def encode_document(tokenizer, text: str, bos_id: int, eos_id: int) -> List[int]:
    """Encode one document, wrapping it in BOS and EOS.

    Both are included here rather than left to the caller, because the sibling implementation
    documented that it wrapped each file and then encoded without them, so every document in
    the 14.5M-token stream began mid-utterance. Documents that start without BOS give the
    model no idea that it is at the start.
    """
    ids = tokenizer.encode(text, add_special_tokens=False).ids
    return [bos_id] + ids + [eos_id]


def encode_sft_document(
    tokenizer,
    prompt: str,
    response: str,
    bos_id: int,
    eos_id: int,
    prompt_prefix: Sequence[int] = (),
) -> Tuple[List[int], int]:
    """Encode a prompt/response pair. Returns ``(ids, response_start)``.

    ``response_start`` is the index of the first response token, which is what the loss mask
    keys off. Computing it here means the mask and the tokens cannot disagree.
    """
    prompt_ids = list(prompt_prefix) + tokenizer.encode(prompt, add_special_tokens=False).ids
    response_ids = tokenizer.encode(response, add_special_tokens=False).ids

    ids = [bos_id] + prompt_ids + response_ids + [eos_id]
    response_start = 1 + len(prompt_ids)
    return ids, response_start


def pack_blocks(
    ids: Sequence[int],
    response_start: int,
    block_size: int,
    pad_id: int,
    document_id: int = 0,
    stride: Optional[int] = None,
) -> List[PackedBlock]:
    """Cut one document into blocks, masking everything outside the response.

    A window that contains no response tokens is still emitted, with an all-zero mask, so the
    document is not silently dropped. :meth:`PackedBlock.supervised_loss` returns a detached
    zero for those, which is the right behaviour: the prompt-only window carries no
    supervised signal, and dropping it would bias the corpus toward longer responses.

    A trailing window shorter than ``block_size`` is padded rather than dropped, because it
    usually contains the end of the response and the EOS token, which is exactly the
    supervision that teaches the model to stop.
    """
    stride = stride or block_size // 2
    ids = list(ids)
    if block_size < 2:
        raise ValueError(f"block_size must be at least 2, got {block_size}")

    blocks: List[PackedBlock] = []
    start = 0

    while start < len(ids):
        window = ids[start : start + block_size]
        is_last = len(window) < block_size

        if is_last:
            # Pad the final window so shapes stay rectangular. Padding is masked out of the
            # loss, which is why masking has to be carried explicitly.
            window = window + [pad_id] * (block_size - len(window))

        tokens = window[:-1]
        labels = window[1:]

        # labels[i] is the target for document position ``start + i + 1``, so position i is
        # supervised when that absolute position falls inside the response and is not
        # padding. Adding the window offset matters: comparing ``i + 1`` to response_start
        # directly compares a window-local index against a document-level one, which
        # supervises nothing on any window after the first.
        offset = start
        mask = [
            1 if (offset + i + 1 >= response_start and labels[i] != pad_id) else 0
            for i in range(len(tokens))
        ]

        blocks.append(
            PackedBlock(tokens=tokens, labels=labels, mask=mask, document_id=document_id)
        )

        if is_last:
            break
        start += stride

    return blocks


def save_stream(stream: TokenStream, tokens_path: Path, ranges_path: Path) -> None:
    """Write the flat stream and its sidecar.

    Two files rather than one: ``tokens.bin`` is a fixed-width int32 array a memory-mapped
    loader can consume without parsing, and ``ranges.json`` is small and human-readable so
    the document boundaries can be checked by eye.
    """
    tokens_path = Path(tokens_path)
    ranges_path = Path(ranges_path)
    tokens_path.parent.mkdir(parents=True, exist_ok=True)
    ranges_path.parent.mkdir(parents=True, exist_ok=True)

    stream.tokens.tofile(tokens_path)
    ranges_path.write_text(
        json.dumps({"ranges": [list(r) for r in stream.ranges]}), encoding="utf-8"
    )


def load_stream(tokens_path: Path, ranges_path: Path) -> TokenStream:
    tokens = np.fromfile(str(tokens_path), dtype=np.int32)
    payload = json.loads(Path(ranges_path).read_text(encoding="utf-8"))
    ranges = [tuple(int(v) for v in pair) for pair in payload["ranges"]]
    return TokenStream(tokens=tokens, ranges=ranges)