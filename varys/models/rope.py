"""Rotary position embeddings, with long-context extension.

The base implementation is Elafry's, unchanged: the "rotate_half" convention where the head
dimension is split in two, the back half negated, and the two concatenated. That is the
Llama convention and it differs from the interleaved variant some implementations use, so
getting it wrong produces a model that trains and never coheres.

## Why Varys needs more than 4k

Longitudinal psychological work does not fit in a short window. A person's history, the
literature a clinician would consult, the reasoning chain behind a complicated case: all of
it is long, and truncating it silently produces a model that answers confidently from the
last 4k tokens of a life. Four extensions are provided, and they are not equivalent.

**``none``** is base RoPE. Correct and nothing more.

**``linear``** divides the position index by the factor. One line, and it is the worst of
these: it compresses *every* distance, including the short ones the model is actually good
at, so local structure is degraded to buy long-range reach. It is here because it is what
most implementations reach for by default, and having it named makes the choice explicit.

**``ntk``** raises the base: ``theta' = theta * factor^(d/(d-2))``. This is the "base scaling"
trick. The intuition is that the high-frequency dimensions, which only ever encode local
order, can be pushed to lower frequency and freed to express distance. The `d/(d-2)`
exponent is the correction that makes the *lowest* frequency match what base RoPE at the
scaled context would have given, so the long-range structure lands where the model expects it
rather than in a novel region.

**``yarn``** interpolates only the dimensions that need it, per-dimension, using an
attention-scaling factor derived from a ramp between ``beta_fast`` and ``beta_slow``.

YaRN is the right default for Varys and the reason is worth stating, because it is a
compromise rather than a pure win. It leaves the highest-frequency dimensions untouched.
Those dimensions are what encode local order, and they are exactly the ones that make
fluent text possible. Linear interpolation compresses them along with everything else and
loses local coherence. NTK avoids the loss but moves the whole spectrum, so behaviour at
short range shifts too. YaRN accepts a worse perplexity in exchange for behaving like the
model it was trained from at short range, and for a model that has to be *trustworthy* in
ordinary conversation, "still normal in a normal conversation" is worth more than a better
number at 128k.

## The trap this module exists to avoid

Applying any of these to a model that was trained without them produces a model that runs,
generates fluent text, and is quietly wrong about position. There is no error. The failure
is silent, which is the dangerous kind.

So the scaling is recorded in ``ModelConfig`` and carried in the checkpoint sidecar, and
:func:`rope_from_config` is the only supported way to build the tables. A checkpoint cannot
be loaded with the wrong geometry without the sidecar disagreeing with itself.
"""

from __future__ import annotations

import math
from typing import Tuple

import torch
from torch import Tensor

__all__ = [
    "precompute_rope",
    "rope_from_config",
    "rotate_half",
    "apply_rope",
    "yarn_ramp",
    "yarn_correction_range",
    "yarn_mscale",
    "ntk_theta",
]


def ntk_theta(base_theta: float, factor: float, head_dim: int) -> float:
    """Base-scaled RoPE base. See the module docstring.

    The exponent is ``d / (d - 2)``. That form is not cosmetic: it is the correction that
    holds the lowest frequency fixed against the ideal scaled context, so the model sees
    long-range structure in the region it was shaped to expect. A plain ``theta * factor``
    shifts every frequency, including the ones the model relies on most.
    """
    if head_dim <= 2:
        raise ValueError(f"head_dim must be > 2 for NTK scaling, got {head_dim}")
    if factor < 1.0:
        raise ValueError(f"NTK factor must be >= 1.0, got {factor}")
    return base_theta * (factor ** (head_dim / (head_dim - 2)))


def yarn_correction_range(
    low_rot: float,
    high_rot: float,
    dim: int,
    base: float,
    original_context: int,
) -> Tuple[int, int]:
    """The band of frequency indices YaRN interpolates, in index units.

    Derived from the wavelengths, not chosen. A dimension rotates ``2*pi/lambda`` times per
    token, and the wavelengths that complete between ``low_rot`` and ``high_rot`` rotations
    across the *original* context are the ones whose behaviour extrapolation would break.
    Those, and only those, get interpolated.

    Returns:
        ``(low, high)`` indices into the half-dimension, clamped to the table.
    """
    def _idx(rotations: float) -> float:
        return (dim * math.log(original_context / (rotations * 2 * math.pi))) / (2 * math.log(base))

    low = math.floor(_idx(low_rot))
    high = math.ceil(_idx(high_rot))
    return max(low, 0), min(high, dim - 1)


def yarn_ramp(low: int, high: int, dim: int, device=None, dtype=torch.float32) -> Tensor:
    """Linear 0-to-1 ramp over frequency indices, from ``low`` to ``high``.

    The ramp is what makes YaRN selective rather than uniform: it decides per dimension how
    much interpolation that dimension receives.
    """
    if high <= low:
        high = low + 1
    ramp = torch.arange(dim, dtype=dtype, device=device)
    return ((ramp - low) / (high - low)).clamp(0.0, 1.0)


def yarn_mscale(scaling_factor: float) -> float:
    """YaRN's global attention correction, ``0.1 * ln(s) + 1``.

    Interpolating a frequency by ``1/s`` divides its period by ``s``, which is equivalent to
    reducing how much attention that dimension carries. Attention is scale-invariant across
    heads only when every head is scaled the same way, so the correction is global.

    It is folded into the cos/sin tables rather than applied to attention after the fact, so
    prefill and decode cannot drift onto different code paths. Forgetting it costs about half
    a bit and is easy to miss, because the model still trains.
    """
    if scaling_factor <= 1.0:
        return 1.0
    return 0.1 * math.log(scaling_factor) + 1.0

def precompute_rope(
    head_dim: int,
    max_seq_len: int,
    theta: float,
    device=None,
    dtype=torch.float32,
    scaling: str = "none",
    factor: float = 1.0,
    beta_fast: float = 32.0,
    beta_slow: float = 1.0,
    original_context: int = 4096,
) -> Tuple[Tensor, Tensor, Tensor]:
    """Build the cos/sin tables.

    Returns ``(inv_freq, cos, sin)`` with cos and sin shaped ``(max_seq_len, head_dim)``.

    The frequency index wraps with ``i % half`` rather than tiling the table, so both halves
    of each head rotate by the same angle. That is what makes the resulting rotation an
    actual rotation rather than a shear, and getting it wrong is invisible in the output
    shape and catastrophic in the output text.
    """
    if head_dim % 2 != 0:
        raise ValueError(f"head_dim must be even for RoPE, got {head_dim}")
    if scaling not in ("none", "linear", "ntk", "yarn"):
        raise ValueError(f"unknown rope scaling {scaling!r}")
    if factor < 1.0:
        raise ValueError(f"rope factor must be >= 1.0, got {factor}")

    # NTK is applied *here*, from the raw base, rather than being left to the caller to
    # pre-raise theta via ModelConfig.effective_theta. It was originally done that way, and
    # the result was a function that silently did nothing when called with an unadjusted base:
    # no error, identical tables, a model quietly limited to its original context. One place
    # computes it, so no caller can get it wrong. ``effective_theta`` stays as a read-only
    # view for verification.
    if scaling == "ntk":
        theta = ntk_theta(theta, factor, head_dim)

    half = head_dim // 2
    exponent = torch.arange(half, dtype=torch.float32, device=device)
    inv_freq = 1.0 / (theta ** (exponent / half))

    positions = torch.arange(max_seq_len, dtype=torch.float32, device=device)

    if scaling == "linear":
        # Compresses every distance, including the short ones the model reads best.
        inv_freq = inv_freq / factor
    elif scaling == "yarn":
        # Interpolate only the band of dimensions whose wavelengths would not survive
        # extrapolation, and leave the rest at their trained frequencies.
        low, high = yarn_correction_range(
            beta_fast, beta_slow, half, theta, original_context
        )
        mask = 1.0 - yarn_ramp(low, high, half, device=device)  # 1 = keep trained freq
        inv_freq_interp = inv_freq / factor
        inv_freq = inv_freq_interp * (1.0 - mask) + inv_freq * mask

    freqs = torch.outer(positions, inv_freq)  # (max_seq_len, half)

    if scaling == "yarn":
        freqs = freqs * yarn_mscale(factor)

    # Duplicate to head_dim so one cos/sin pair covers both halves.
    freqs = torch.cat([freqs, freqs], dim=-1)  # (max_seq_len, head_dim)
    return inv_freq, freqs.cos().to(dtype), freqs.sin().to(dtype)


def rope_from_config(cfg, device=None, dtype=torch.float32):
    """Build the tables from a :class:`~varys.config.base.ModelConfig`.

    The only supported path. Going through the config is what guarantees that a checkpoint's
    tables match the scaling it was trained under, since the config travels in the sidecar.
    """
    return precompute_rope(
        cfg.head_dim,
        cfg.max_seq_len,
        cfg.rope_theta,
        device=device,
        dtype=dtype,
        scaling=cfg.rope_scaling,
        factor=cfg.rope_factor,
        beta_fast=cfg.yarn_beta_fast,
        beta_slow=cfg.yarn_beta_slow,
        original_context=cfg.yarn_orig_ctx,
    )


def rotate_half(x: Tensor) -> Tensor:
    """``(..., head_dim) -> (..., head_dim)``. Negate the back half, keep the front half."""
    half = x.shape[-1] // 2
    front = x[..., :half]
    back = x[..., half:]
    return torch.cat((-back, front), dim=-1)


def apply_rope(x: Tensor, cos: Tensor, sin: Tensor) -> Tensor:
    """Apply rotary embeddings to ``x`` of shape ``(batch, heads, seq, head_dim)``.

    ``cos``/``sin`` are ``(seq, head_dim)`` when every row of the batch shares positions, or
    ``(batch, seq, head_dim)`` when they do not. Both broadcast to 4D without an explicit
    expand, which is cheaper than materialising full-rank tables.
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
