"""Grouped-query attention with multi-axis affective bias.

This is the file Varys is actually about.

## The equation, and the shape problem

Standard attention is

    Attention(Q, K, V) = softmax(QK^T / sqrt(d_k)) V

and the affective architecture adds a bias derived from the internal state

    B = f(S_t)
    Affective Attention(Q, K, V) = softmax(QK^T / sqrt(d_k) + B) V

Taken at face value, ``QK^T`` is ``(seq, seq)``, so ``B`` wants to be ``(seq, seq)`` too. It
cannot be. ``B`` is a function of a state vector, so a positional ``(seq, seq)`` bias would
need a weight of shape ``state_dim x seq x seq``: position-dependent, quadratic in context
length, and unbounded as the vocabulary grows.

The reading that works treats the bias the way BERT does, binding value directions to key
directions rather than binding positions:

    q @ (I + B) @ k^T  ==  q @ k^T  +  q @ B @ k^T

so ``B`` is ``(head_dim, head_dim)`` per head, and the whole thing is two matmuls:

    scores = ((q @ B) @ k^T) / sqrt(d_k)

Two properties matter more than literal shape agreement. Cost is ``O(d)`` per head rather
than ``O(seq)``, so folding the bias into ``q`` before the score matmul is ``d`` times
cheaper than the product it modifies. And the KV cache problem disappears: ``B`` acts in
feature space, not position space, so prefill and single-token decode compute identical
arithmetic by construction, with nothing to cache beside the keys and no way for the two
paths to disagree.

## What Varys changes, and why it is not just a wider input

Elafry's bias is one projection from 3 state dimensions. The obvious Varys change is one
projection from 21, which is a one-line edit and is what a first pass produces. It is wrong
for three reasons.

**The blocks have different safety properties.** Affect shaping attention is the mechanism
the architecture is for. Clinical state shaping attention is a model attending differently
to someone because it inferred a spectrum from their text, which is a discrimination the
model has no standing to make. Moral state shaping attention is a persuasion surface, and it
is discussed at length in :class:`~varys.config.base.AffectConfig`. One wide projection
cannot hold those three at different levels of trust.

**Ablation needs to address them separately.** "Does the affective mechanism do anything" is
answered by zeroing all of it. "Does clinical conditioning help or hurt" needs the clinical
block to be removable on its own. A single shared projection cannot be partially removed;
the two terms are entangled from step zero.

**The parameter cost is per block, and that should be visible.** A block of ``k`` state
dimensions costs ``k * n_bias * head_dim^2`` per layer, which at 72B is 1.05M parameters per
dimension per block. At three blocks that is a real line in the budget, and a line item
nobody can see is a line item nobody argues about.

So each block gets its own projection and its own learnable scalar scale, and the scales are
initialised to encode the position above: VAD at 1.0, spectra at 0.1, moral absent by
default. They are initialisation values, not caps. The model can raise any of them, which is
exactly why the moral case is handled by absence rather than by a small number.

## The zero-init contract

Every block projection is zero-initialised, so at step zero ``B`` is exactly zero and the
model is arithmetically identical to vanilla attention. Gradients still flow: ``dL/dW``
depends on ``q`` and ``k``, which are non-zero, even though the layer's own output is zero.
Varys scales are zero, which means a non-zero scale multiplied by a zero projection is still
zero, so the initialisation contract holds independently of the scale.
"""

from __future__ import annotations

import math
from typing import Dict, Optional, Tuple

import torch
from torch import Tensor, nn

__all__ = ["MultiAxisBias", "GroupedQueryAttention"]


class MultiAxisBias(nn.Module):
    """Maps a multi-block affective state to a per-head attention bias.

    Args:
        block_widths: ``{block_name: width}`` in the order they appear in the state vector.
            Only blocks listed here reach the bias, which is how the moral block is excluded
            without the state having to change shape.
        n_heads: query heads, for ``granularity="head"``.
        n_kv_heads: KV groups, for ``granularity="kv"``.
        head_dim: per-head width.
        block_scales: initial value of each block's learnable scale. See the module docstring.
        granularity: ``"head"`` gives every query head its own bias matrix. ``"kv"`` gives one
            per KV group broadcast across the ``n_rep`` query heads, at ``n_rep`` times less
            memory. Default ``"head"``, because per-head selectivity is the point.
    """

    def __init__(
        self,
        block_widths: Dict[str, int],
        n_heads: int,
        n_kv_heads: int,
        head_dim: int,
        block_scales: Optional[Dict[str, float]] = None,
        granularity: str = "head",
    ):
        super().__init__()
        if granularity not in ("head", "kv"):
            raise ValueError(f"granularity must be 'head' or 'kv', got {granularity!r}")
        if n_heads % n_kv_heads != 0:
            raise ValueError(f"n_heads {n_heads} not a multiple of n_kv_heads {n_kv_heads}")
        if not block_widths:
            raise ValueError("at least one state block must reach the bias")

        self.block_widths = dict(block_widths)
        self.block_names = tuple(block_widths)
        self.head_dim = head_dim
        self.n_heads = n_heads
        self.n_kv_heads = n_kv_heads
        self.n_rep = n_heads // n_kv_heads
        self.granularity = granularity
        self.n_bias = n_heads if granularity == "head" else n_kv_heads
        self.state_dim = sum(block_widths.values())

        scales = block_scales or {}
        n_bias_dd = self.n_bias * head_dim * head_dim
        for name, width in self.block_widths.items():
            proj = nn.Linear(width, n_bias_dd, bias=False)
            nn.init.zeros_(proj.weight)
            setattr(self, f"proj_{name}", proj)
            self.register_parameter(
                f"scale_{name}", nn.Parameter(torch.tensor(float(scales.get(name, 1.0))))
            )

    def forward(self, state: Tensor) -> Tensor:
        """``state: (state_dim,) or (B, state_dim) -> (B, n_bias, head_dim, head_dim)``.

        The blocks are summed after their own projections, so each block's contribution is a
        separate term rather than a mixture. That is what makes the ablation honest: zeroing
        one block's scale removes exactly that block and leaves the others untouched.
        """
        if state.dim() == 1:
            state = state.unsqueeze(0)
        if state.dim() != 2:
            raise ValueError(f"state must be (state_dim,) or (B, state_dim), got {state.shape}")
        if state.shape[-1] != self.state_dim:
            raise ValueError(
                f"state width {state.shape[-1]} does not match state_dim {self.state_dim} "
                f"(blocks: {self.block_widths})"
            )

        bsz = state.shape[0]
        out: Optional[Tensor] = None
        offset = 0
        for name, width in self.block_widths.items():
            chunk = state[:, offset : offset + width]
            offset += width
            term = getattr(self, f"proj_{name}")(chunk) * getattr(self, f"scale_{name}")
            out = term if out is None else out + term

        assert out is not None  # guaranteed by the constructor's check
        return out.view(bsz, self.n_bias, self.head_dim, self.head_dim)

    def bias_for_heads(self, state: Tensor) -> Tensor:
        """Bias expanded to one matrix per query head. ``(B, n_heads, d, d)``."""
        bias = self.forward(state)
        if self.granularity == "head":
            return bias
        # "kv": repeat each group's bias across the n_rep query heads in that group. The
        # grouping has to match the one GroupedQueryAttention._expand_kv applies to K and V.
        expanded = bias.unsqueeze(2).expand(-1, -1, self.n_rep, -1, -1)
        return expanded.reshape(bias.shape[0], self.n_heads, self.head_dim, self.head_dim)

    def block_influence(self) -> Dict[str, float]:
        """Each block's current scale, for logging and for the ablation report.

        Read at runtime rather than read from the config, because these are learnable and
        their values after training are the actual answer to "how much is this state
        influencing attention".
        """
        return {
            name: float(getattr(self, f"scale_{name}").detach().abs())
            for name in self.block_widths
        }

    def extra_repr(self) -> str:
        return (
            f"blocks={self.block_widths}, n_bias={self.n_bias}, "
            f"head_dim={self.head_dim}, granularity={self.granularity}"
        )


class GroupedQueryAttention(nn.Module):
    """Multi-head attention with grouped KV heads and an optional multi-axis bias.

    The KV cache holds ``(k, v)`` shaped ``(B, n_kv_heads, seq, head_dim)``. KV heads are
    stored *unexpanded*; expansion happens inside ``forward`` so the cache stays ``n_rep``
    times smaller than the equivalent MHA cache.
    """

    def __init__(
        self,
        dim: int,
        n_heads: int,
        n_kv_heads: int,
        head_dim: int,
        block_widths: Optional[Dict[str, int]] = None,
        block_scales: Optional[Dict[str, float]] = None,
        affect_granularity: Optional[str] = "head",
    ):
        super().__init__()
        self.dim = dim
        self.n_heads = n_heads
        self.n_kv_heads = n_kv_heads
        self.head_dim = head_dim
        self.n_rep = n_heads // n_kv_heads
        self.scale = 1.0 / math.sqrt(head_dim)

        self.q_proj = nn.Linear(dim, n_heads * head_dim, bias=False)
        self.k_proj = nn.Linear(dim, n_kv_heads * head_dim, bias=False)
        self.v_proj = nn.Linear(dim, n_kv_heads * head_dim, bias=False)
        self.o_proj = nn.Linear(n_heads * head_dim, dim, bias=False)

        self.affect = (
            MultiAxisBias(
                block_widths=block_widths or {},
                n_heads=n_heads,
                n_kv_heads=n_kv_heads,
                head_dim=head_dim,
                block_scales=block_scales,
                granularity=affect_granularity,
            )
            if (block_widths and affect_granularity is not None)
            else None
        )

    def _split_heads(self, x: Tensor, n_heads: int) -> Tensor:
        """``(B, S, n_heads*d) -> (B, n_heads, S, d)``"""
        bsz, seq, _ = x.shape
        return x.view(bsz, seq, n_heads, self.head_dim).transpose(1, 2)

    def _expand_kv(self, x: Tensor) -> Tensor:
        """``(B, n_kv, S, d) -> (B, n_heads, S, d)`` by repeating within each group."""
        if self.n_rep == 1:
            return x
        bsz, _, seq, _ = x.shape
        # Expand the head axis then reshape, so each KV head's *contiguous block* of n_rep
        # query heads receives the same keys. That grouping must match training.
        return (
            x[:, :, None, :, :]
            .expand(bsz, self.n_kv_heads, self.n_rep, seq, self.head_dim)
            .reshape(bsz, self.n_heads, seq, self.head_dim)
        )

    def forward(
        self,
        x: Tensor,
        cos: Tensor,
        sin: Tensor,
        past_kv: Optional[Tuple[Tensor, Tensor]] = None,
        use_cache: bool = False,
        state: Optional[Tensor] = None,
    ) -> Tuple[Tensor, Optional[Tuple[Tensor, Tensor]]]:
        """Args:
            x: (B, S, dim)
            cos, sin: (max_seq, head_dim) rotary tables, already gathered for this step
            past_kv: (k, v) each (B, n_kv_heads, past_seq, head_dim)
            state: (B, state_dim). ``None`` means vanilla attention, which is what the
                ablation uses as its control condition.
        Returns:
            (output (B, S, dim), present_kv or None)
        """
        from varys.models.rope import apply_rope

        bsz, seq, _ = x.shape

        q = self._split_heads(self.q_proj(x), self.n_heads)
        k = self._split_heads(self.k_proj(x), self.n_kv_heads)
        v = self._split_heads(self.v_proj(x), self.n_kv_heads)

        # cos/sin arrive already gathered for this step's positions, so they are used as-is.
        # Only the causal mask needs to know where we are, which is what past_len is for.
        past_len = past_kv[0].shape[2] if past_kv is not None else 0
        q = apply_rope(q, cos, sin)
        k = apply_rope(k, cos, sin)

        present_kv: Optional[Tuple[Tensor, Tensor]] = (k, v)
        if past_kv is not None:
            k = torch.cat([past_kv[0], k], dim=2)
            v = torch.cat([past_kv[1], v], dim=2)
            present_kv = (k, v)
        elif not use_cache:
            present_kv = None

        total = k.shape[2]
        k_exp = self._expand_kv(k)
        v_exp = self._expand_kv(v)

        # Fold the affective bias into the queries. Zero-initialised projections make this a
        # no-op at initialisation, which is why it is safe to always run.
        if self.affect is not None and state is not None:
            q = q + torch.matmul(q, self.affect.bias_for_heads(state))

        scores = torch.matmul(q, k_exp.transpose(2, 3)) * self.scale  # (B, H, S, total)

        # Causal mask. Only needed when new queries can see cached or future positions, i.e.
        # when seq > 1. Single-token decode against a full cache is already causal.
        if seq > 1:
            device = scores.device
            q_pos = torch.arange(past_len, past_len + seq, device=device).unsqueeze(1)
            k_pos = torch.arange(total, device=device).unsqueeze(0)
            scores = scores.masked_fill(~(k_pos <= q_pos), torch.finfo(scores.dtype).min)

        attn = torch.softmax(scores.float(), dim=-1).to(scores.dtype)
        out = torch.matmul(attn, v_exp)  # (B, H, S, d)
        out = out.transpose(1, 2).contiguous().view(bsz, seq, self.n_heads * self.head_dim)
        return self.o_proj(out), present_kv

    def extra_repr(self) -> str:
        return (
            f"dim={self.dim}, n_heads={self.n_heads}, n_kv_heads={self.n_kv_heads}, "
            f"head_dim={self.head_dim}, n_rep={self.n_rep}, "
            f"affect={'on' if self.affect is not None else 'off'}"
        )
