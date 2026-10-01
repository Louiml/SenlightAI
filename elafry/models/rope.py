"""Rotary position embeddings.

The "rotate_half" convention: split the head dimension in two, negate the second half, and
concatenate. This is the Llama convention and differs from the interleaved "rotate_half"
some implementations use, so getting it wrong produces a model that trains but never
coheres. ``tests/test_rope.py`` pins it against a closed-form reference.
"""

from __future__ import annotations

import torch
from torch import Tensor

__all__ = ["precompute_rope", "rotate_half", "apply_rope", "rope_from_config"]


def precompute_rope(head_dim: int, max_seq_len: int, theta: float, device=None, dtype=torch.float32):
    """Build the cos/sin tables.

    Returns ``(inv_freq, cos, sin)`` with cos and sin shaped ``(max_seq_len, head_dim)``.
    The frequency index wraps with ``i % half`` rather than tiling the table, so the two
    halves of each head rotate by the same angle.
    """
    if head_dim % 2 != 0:
        raise ValueError(f"head_dim must be even for RoPE, got {head_dim}")

    half = head_dim // 2
    # Exponent ladder: high frequencies first, decaying to 1/theta at the last index.
    exponent = torch.arange(half, dtype=torch.float32, device=device)
    inv_freq = 1.0 / (theta ** (exponent / half))

    positions = torch.arange(max_seq_len, dtype=torch.float32, device=device)
    freqs = torch.outer(positions, inv_freq)  # (max_seq_len, half)

    # Duplicate to head_dim so a single cos/sin pair covers both halves.
    freqs = torch.cat([freqs, freqs], dim=-1)  # (max_seq_len, head_dim)
    return inv_freq, freqs.cos().to(dtype), freqs.sin().to(dtype)


def rope_from_config(cfg, device=None, dtype=torch.float32):
    return precompute_rope(cfg.head_dim, cfg.max_seq_len, cfg.rope_theta, device=device, dtype=dtype)


def rotate_half(x: Tensor) -> Tensor:
    """``(..., head_dim) -> (..., head_dim)``. Negate the back half, keep the front half."""
    half = x.shape[-1] // 2
    front = x[..., :half]
    back = x[..., half:]
    return torch.cat((-back, front), dim=-1)


def apply_rope(x: Tensor, cos: Tensor, sin: Tensor) -> Tensor:
    """Apply rotary embeddings to ``x`` of shape ``(batch, heads, seq, head_dim)``.

    ``cos`` and ``sin`` are ``(seq, head_dim)`` for the common case where every row in the
    batch shares the same positions, or ``(batch, seq, head_dim)`` when they do not. Both
    broadcast up to a 4D shape without an explicit expand, which keeps this cheaper than
    materialising full-rank tables.
    """
    if x.shape[-1] != cos.shape[-1]:
        raise ValueError(
            f"head_dim mismatch: x has {x.shape[-1]}, rope tables have {cos.shape[-1]}"
        )
    if cos.dim() == 2:
        c = cos.unsqueeze(0).unsqueeze(0)
        s = sin.unsqueeze(0).unsqueeze(0)
    elif cos.dim() == 3:
        c = cos.unsqueeze(1)
        s = sin.unsqueeze(1)
    else:
        raise ValueError(f"cos/sin must be (S, d) or (B, S, d), got {tuple(cos.shape)}")
    if c.shape[-2] != x.shape[-2]:
        raise ValueError(
            f"position count {c.shape[-2]} does not match sequence length {x.shape[-2]}"
        )
    return x * c + rotate_half(x) * s