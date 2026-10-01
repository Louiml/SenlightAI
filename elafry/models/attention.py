"""Grouped-query attention with affective bias injection.

This is the file the architecture note is actually about. Standard attention is

    Attention(Q, K, V) = softmax(QK^T / sqrt(d_k)) V

and Elafry adds a bias derived from the internal affective state

    B_vad = Linear(S_t) * W_affect
    Affective Attention(Q, K, V) = softmax(QK^T / sqrt(d_k) + B_vad) V

## Why the bias is bilinear rather than literally (S, S)

Taken at face value, ``QK^T`` is ``(seq, seq)``, so adding ``B_vad`` to it wants ``B_vad``
to be ``(seq, seq)`` too. It cannot be. ``B_vad`` is a function of a 3D state vector, so a
positional ``(seq, seq)`` bias would need ``W_affect`` of shape ``3 x seq x seq``: position-
dependent, quadratic in context length, and unbounded as the vocabulary grows. No serious
implementation does this.

The reading that works treats the bias as an attention bias in the BERT sense, where it
binds value directions to key directions rather than binding positions:

    q @ (I + B_vad) @ k^T  ==  q @ k^T  +  q @ B_vad @ k^T

so ``B_vad`` has shape ``(head_dim, head_dim)`` per head, and the whole thing is
implemented as two matmuls:

    scores = ((q @ B_vad) @ k^T) / sqrt(d_k)

This is faithful to the equation (the bias term lands on the pre-softmax logits) and it has
two properties that matter more than literal shape agreement:

1. **Cost is O(d) per head, not O(seq).** Folding the bias into ``q`` before the score
   matmul makes it ``d`` times cheaper than the ``(seq, seq)`` product it modifies. A
   literal additive ``(seq, seq)`` bias would cost the same again as computing all the
   scores.
2. **The KV cache problem disappears.** ``B_vad`` acts in feature space, not position space,
   so prefill and single-token decode compute identical arithmetic by construction. There is
   nothing to cache alongside the keys, and no chance of the two paths disagreeing. The
   state still re-reads its own history through a different feature lens on every step,
   which is the behaviour we want; it just happens for free.

The consequence for the mechanism's power: pin the state to "anger" and the model attends
along genuinely different query-key feature pairings, not merely along a shifted constant.
``tests/test_affect_injection.py`` measures this rather than asserting it.
"""

from __future__ import annotations

import math
from typing import Optional, Tuple

import torch
from torch import Tensor, nn

__all__ = ["AffectiveBias", "GroupedQueryAttention"]


class AffectiveBias(nn.Module):
    """Maps an affective state to a per-head attention bias.

    The state projection is zero-initialised, so at step zero ``B_vad`` is exactly zero and
    the model is arithmetically identical to vanilla attention. Emotion has to earn its
    influence through training rather than wrecking the initial attention distribution.
    Gradients still flow: ``dL/dW`` depends on ``q`` and ``k``, which are non-zero, even
    though the layer's own output is zero.

    Args:
        state_dim: width of the state feature vector. 3 for raw VAD; wider if intent,
            arousal velocity or crisis flags are concatenated in.
        n_heads: number of query heads, for ``granularity="head"``.
        n_kv_heads: number of KV groups, for ``granularity="kv"``.
        head_dim: per-head width.
        granularity: ``"head"`` gives every query head its own bias matrix. ``"kv"`` gives
            one per KV group broadcast across the ``n_rep`` query heads in that group, at
            ``n_rep`` times less memory. Default ``"head"``, because per-head selectivity
            is the point of the mechanism.
    """

    def __init__(
        self,
        state_dim: int,
        n_heads: int,
        n_kv_heads: int,
        head_dim: int,
        granularity: str = "head",
    ):
        super().__init__()
        if granularity not in ("head", "kv"):
            raise ValueError(f"granularity must be 'head' or 'kv', got {granularity!r}")
        if n_heads % n_kv_heads != 0:
            raise ValueError(f"n_heads {n_heads} not a multiple of n_kv_heads {n_kv_heads}")
        self.state_dim = state_dim
        self.head_dim = head_dim
        self.n_heads = n_heads
        self.n_kv_heads = n_kv_heads
        self.n_rep = n_heads // n_kv_heads
        self.granularity = granularity
        self.n_bias = n_heads if granularity == "head" else n_kv_heads

        self.state_proj = nn.Linear(state_dim, self.n_bias * head_dim * head_dim, bias=False)
        nn.init.zeros_(self.state_proj.weight)

    def forward(self, state: Tensor) -> Tensor:
        """``state: (state_dim,) or (B, state_dim) -> (B, n_bias, head_dim, head_dim)``."""
        if state.dim() == 1:
            state = state.unsqueeze(0)
        if state.dim() != 2:
            raise ValueError(f"state must be (state_dim,) or (B, state_dim), got {state.shape}")
        if state.shape[-1] != self.state_dim:
            raise ValueError(
                f"state width {state.shape[-1]} does not match state_dim {self.state_dim}"
            )

        bsz = state.shape[0]
        out = self.state_proj(state)  # (B, n_bias * d * d)
        return out.view(bsz, self.n_bias, self.head_dim, self.head_dim)

    def bias_for_heads(self, state: Tensor) -> Tensor:
        """Bias expanded to one matrix per query head. Shape ``(B, n_heads, d, d)``."""
        bias = self.forward(state)
        if self.granularity == "head":
            return bias
        # "kv": repeat each group's bias across the n_rep query heads in that group.
        # (B, n_kv, 1, d, d) -> expand -> reshape. The grouping has to match the one
        # GroupedQueryAttention._expand_kv applies to keys and values.
        expanded = bias.unsqueeze(2).expand(-1, -1, self.n_rep, -1, -1)
        return expanded.reshape(bias.shape[0], self.n_heads, self.head_dim, self.head_dim)

    def extra_repr(self) -> str:
        return (
            f"state_dim={self.state_dim}, n_bias={self.n_bias}, "
            f"head_dim={self.head_dim}, granularity={self.granularity}"
        )


class GroupedQueryAttention(nn.Module):
    """Multi-head attention with grouped KV heads and an optional affective bias.

    The KV cache is a list of ``(k, v)`` pairs shaped ``(B, n_kv_heads, seq, head_dim)``.
    KV heads are stored *unexpanded*; expansion happens inside ``forward`` so the cache stays
    ``n_rep`` times smaller than the equivalent MHA cache.
    """

    def __init__(
        self,
        dim: int,
        n_heads: int,
        n_kv_heads: int,
        head_dim: int,
        state_dim: int = 3,
        affect_granularity: str = "head",
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

        if affect_granularity is not None:
            self.affect = AffectiveBias(
                state_dim=state_dim,
                n_heads=n_heads,
                n_kv_heads=n_kv_heads,
                head_dim=head_dim,
                granularity=affect_granularity,
            )
        else:
            self.affect = None

    def _split_heads(self, x: Tensor, n_heads: int) -> Tensor:
        """``(B, S, n_heads*d) -> (B, n_heads, S, d)``"""
        bsz, seq, _ = x.shape
        return x.view(bsz, seq, n_heads, self.head_dim).transpose(1, 2)

    def _expand_kv(self, x: Tensor) -> Tensor:
        """``(B, n_kv, S, d) -> (B, n_heads, S, d)`` by repeating within each group."""
        if self.n_rep == 1:
            return x
        bsz, _, seq, _ = x.shape
        # Expand the head axis then reshape, so each KV head's *contiguous block* of
        # n_rep query heads receives the same keys. That grouping must match training.
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
            cos, sin: (max_seq, head_dim) rotary tables
            past_kv: (k, v) each (B, n_kv_heads, past_seq, head_dim)
            state: (B, state_dim) or (state_dim,). ``None`` means vanilla attention, which
                is what the ablation uses as its control condition.
        Returns:
            (output (B, S, dim), present_kv or None)
        """
        from elafry.models.rope import apply_rope

        bsz, seq, _ = x.shape

        q = self._split_heads(self.q_proj(x), self.n_heads)
        k = self._split_heads(self.k_proj(x), self.n_kv_heads)
        v = self._split_heads(self.v_proj(x), self.n_kv_heads)

        # ``cos``/``sin`` arrive already gathered for this step's positions by the caller,
        # so they are used as-is. Only the causal mask needs to know where we are in the
        # sequence, which is what past_len is for.
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

        # Fold the affective bias into the queries. Zero-initialised state_proj makes this
        # a no-op at initialisation, which is why it is safe to always run.
        if self.affect is not None and state is not None:
            bias = self.affect.bias_for_heads(state)  # (B, n_heads, d, d)
            q = q + torch.matmul(q, bias)

        scores = torch.matmul(q, k_exp.transpose(2, 3)) * self.scale  # (B, H, S, total)

        # Causal mask. Only needed when new queries can see cached or future positions,
        # i.e. when seq > 1. Single-token decode against a full cache is already causal.
        if seq > 1:
            device = scores.device
            q_pos = torch.arange(past_len, past_len + seq, device=device).unsqueeze(1)
            k_pos = torch.arange(total, device=device).unsqueeze(0)
            mask = k_pos <= q_pos
            scores = scores.masked_fill(~mask, torch.finfo(scores.dtype).min)

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