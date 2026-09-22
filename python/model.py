"""Phase 2 — Core model architecture: Senlight Coder AI (Llama 3.1-style).

Config: layered, bias-free decoder-only transformer with:
  * Pre-RMSNorm                    (stabilized, no learned bias)
  * Rotary Position Embeddings    (RoPE, high base freq 500k)
  * Grouped-Query Attention       (GQA: 12 query heads, 4 KV heads)
  * SwiGLU feed-forward           (gate path, no standard activation)
  * Shared embedding/lm_head      (weight tying to cut ~24M params)

All `nn.Linear` layers are bias-free and initialized exactly as Meta's LLaMA:
  * hidden/proj/attn/embedding/FFN-down: ~Normal(0, 1/sqrt(dim))
  * FFN gate & up (2x the inputs channel count handled inside the scale):
    ~Normal(0, 1/sqrt(ff_dim)) -- same 1/sqrt(fan_in) rule.
  * Residual FFN-down and attention output projections are zero-initialized
    (the "residual stream with no extra signal at init" trick) so each block
    starts as an identity mapping.

Weight key naming matches HuggingFace Llama (``model.layers.N.*``) so the
safetensors export in Phase 3 loads directly into the Candle Rust engine.
"""

from __future__ import annotations

import math
from typing import Tuple

import torch
import torch.nn as nn
import torch.nn.functional as F

import config as C


# ---------------------------------------------------------------------------
# Layer 0: RMSNorm — normalize by root-mean-square, then scale by gamma
# ---------------------------------------------------------------------------
class RMSNorm(nn.Module):
    """Root-Mean-Square LayerNormalization (arXiv:1910.07467).

    ``y = gamma * x / sqrt(mean(x^2) + eps)`` — a bias-free normalization used
    in LLaMA. Applied *before* sublayers (Pre-Norm). ``gamma`` starts at one.
    """

    def __init__(self, dim: int, eps: float = C.RMS_EPS):
        super().__init__()
        self.eps = eps
        self.weight = nn.Parameter(torch.ones(dim))

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        # x: (..., dim)
        rms = x.pow(2).mean(-1, keepdim=True).add(self.eps).rsqrt()
        return x * rms * self.weight


# ---------------------------------------------------------------------------
# Rotary Position Embeddings (RoPE) — https://arxiv.org/abs/2104.09864
# ---------------------------------------------------------------------------
def precompute_rope_frequencies(
    head_dim: int, max_len: int, theta: float
) -> Tuple[torch.Tensor, torch.Tensor, torch.Tensor]:
    """Precompute RoPE inverse frequencies and the (cos, sin) rotation tables.

    Following Llama 3.1, rotation frequencies are ``theta^(-2i/head_dim)`` for
    ``i`` in ``[0, head_dim/2)`` with a single, group-agnostic ``theta``. The
    returned ``cos``/``sin`` are full ``head_dim`` wide (the two frequency
    halves are duplicated), so they broadcast directly against ``(B, H, S,
    head_dim)`` Q/K tensors when combined with `rotate_half`.

    Returns ``(inv_freq, cos, sin)`` where ``cos``/``sin`` have shape
    ``(max_len, head_dim)``.
    """
    # inv_freq: (head_dim // 2,)
    inv_freq = 1.0 / (theta ** (torch.arange(0, head_dim, 2, dtype=torch.float32) / head_dim))
    # t: (max_len,)
    t = torch.arange(max_len, dtype=torch.float32)
    # freqs: (max_len, head_dim // 2)
    freqs = torch.outer(t, inv_freq)
    # Duplicate the halves -> (max_len, head_dim), matching HF Llama's cos/sin.
    emb = torch.cat((freqs, freqs), dim=-1)
    cos = emb.cos()
    sin = emb.sin()
    return inv_freq, cos, sin


def rotate_half(x: torch.Tensor) -> torch.Tensor:
    """Split the head into two halves and swap them with a sign flip (RoPE)."""
    x1, x2 = x.chunk(2, dim=-1)
    return torch.cat((-x2, x1), dim=-1)


def apply_rotary_pos_emb(
    q: torch.Tensor,
    k: torch.Tensor,
    cos: torch.Tensor,
    sin: torch.Tensor,
    position_ids: torch.Tensor,
) -> Tuple[torch.Tensor, torch.Tensor]:
    """Apply precomputed rotary embeddings to Q and K at given positions.

    Args:
      q, k: (batch, heads, seq, head_dim)
      cos, sin: (max_len, head_dim), precomputed rotary cos/sin.
      position_ids: (batch, seq) int64 positions.
    Returns rotated ``q``/``k``.
    """
    # cos/sin gathered at each position -> (batch, seq, head_dim)
    cos = cos[position_ids].unsqueeze(1)  # (B, 1, S, D)
    sin = sin[position_ids].unsqueeze(1)  # (B, 1, S, D)
    q_embed = (q * cos) + (rotate_half(q) * sin)
    k_embed = (k * cos) + (rotate_half(k) * sin)
    return q_embed, k_embed


# ---------------------------------------------------------------------------
# SwiGLU — https://arxiv.org/abs/2002.05202  swish(x) = x * sigmoid(x)
# ---------------------------------------------------------------------------
class SwiGLU(nn.Module):
    """Feed-forward gated linear unit.

    ``FFN(x) = (SiLU(x @ gate.T) * (x @ up.T)) @ down.T``
    Three bias-free projections; ``gate`` and ``up`` expand to ``ff_dim``, and
    ``down`` contracts back to ``model_dim``. This is what LLaMA 2/3 use
    (their subword uses ``SiLU`` as the Swish/activation).
    """

    def __init__(self, in_dim: int, hidden_dim: int, out_dim: int):
        super().__init__()
        self.gate = nn.Linear(in_dim, hidden_dim, bias=False)
        self.up = nn.Linear(in_dim, hidden_dim, bias=False)
        self.down = nn.Linear(hidden_dim, out_dim, bias=False)

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        return self.down(F.silu(self.gate(x)) * self.up(x))


# ---------------------------------------------------------------------------
# Grouped-Query Attention (GQA) — https://arxiv.org/abs/2305.13245
# ---------------------------------------------------------------------------
class GroupedQueryAttention(nn.Module):
    """Self-attention with grouped KV heads.

    ``n_heads`` query heads but only ``n_kv_heads`` key/value heads. Each KV
    head is shared by ``n_heads // n_kv_heads`` query heads, shrinking the KV
    cache during autoregressive inference by a factor of ``n_heads/n_kv_heads``
    (here 12/4 = 3x) while keeping near-MHA quality.
    """

    def __init__(
        self,
        dim: int = C.MODEL_DIM,
        n_heads: int = C.N_HEADS,
        n_kv_heads: int = C.N_KV_HEADS,
        head_dim: int = C.HEAD_DIM,
        dropout: float = C.DROPOUT,
    ):
        super().__init__()
        self.n_heads = n_heads
        self.n_kv_heads = n_kv_heads if n_kv_heads is not None else n_heads
        self.n_rep = n_heads // self.n_kv_heads  # queries per KV head
        self.head_dim = head_dim

        self.q_proj = nn.Linear(dim, n_heads * head_dim, bias=False)
        self.k_proj = nn.Linear(dim, self.n_kv_heads * head_dim, bias=False)
        self.v_proj = nn.Linear(dim, self.n_kv_heads * head_dim, bias=False)
        self.o_proj = nn.Linear(n_heads * head_dim, dim, bias=False)
        self.dropout = nn.Dropout(dropout)

    def forward(
        self,
        x: torch.Tensor,
        cos: torch.Tensor,
        sin: torch.Tensor,
        position_ids: torch.Tensor,
        past_kv: torch.Tensor | None = None,
        use_cache: bool = False,
    ) -> Tuple[torch.Tensor, Tuple[torch.Tensor, torch.Tensor] | None]:
        """Args:
              x: (batch, seq, dim)
              past_kv: optional (key, value) to append for incremental decode.
            Returns ``(attn_out, (key, value))``; ``attn_out: (B, S, dim)``.
        """
        bsz, seq_len, _ = x.shape

        # 1. Project Q/K/V.
        q = self.q_proj(x).view(bsz, seq_len, self.n_heads, self.head_dim).transpose(1, 2)
        k = self.k_proj(x).view(bsz, seq_len, self.n_kv_heads, self.head_dim).transpose(1, 2)
        v = self.v_proj(x).view(bsz, seq_len, self.n_kv_heads, self.head_dim).transpose(1, 2)

        # 2. Apply rotary embeddings to Q and K.
        q, k = apply_rotary_pos_emb(q, k, cos, sin, position_ids)

        # 3. KV cache (autoregressive decode): concatenate along sequence dim.
        if use_cache and past_kv is not None:
            past_key, past_value = past_kv
            k = torch.cat([past_key, k], dim=-2)
            v = torch.cat([past_value, v], dim=-2)
        present_kv = (k, v) if use_cache else None

        # 4. Grouped-query expansion: repeat KV heads so they align with Q.
        #    (B, S, n_kv_heads, D) -> (B, S, n_heads, D) -> (B, n_heads, S, D)
        if self.n_rep > 1:
            k = k.repeat_interleave(self.n_rep, dim=1)
            v = v.repeat_interleave(self.n_rep, dim=1)

        # 5. Scaled dot-product attention with causal mask.
        scale = 1.0 / math.sqrt(self.head_dim)
        attn = (q @ k.transpose(-2, -1)) * scale  # (B, n_heads, S_q, S_k)
        # Causal mask: prevent attending to future positions.
        mask = torch.full((seq_len, seq_len), float("-inf"), device=x.device)
        mask = torch.triu(mask, diagonal=1)
        attn = attn + mask[None, None, :seq_len, :seq_len]
        attn = torch.softmax(attn, dim=-1)
        attn = self.dropout(attn)

        out = attn @ v                                   # (B, n_heads, S, D)
        out = out.transpose(1, 2).contiguous().view(bsz, seq_len, -1)
        return self.o_proj(out), present_kv


# ---------------------------------------------------------------------------
# Single transformer block
# ---------------------------------------------------------------------------
class SenlightCoderBlock(nn.Module):
    """One Llama-style decoder block: Pre-Norm attention + Pre-Norm SwiGLU FFN."""

    def __init__(
        self,
        dim: int = C.MODEL_DIM,
        n_heads: int = C.N_HEADS,
        n_kv_heads: int = C.N_KV_HEADS,
        head_dim: int = C.HEAD_DIM,
        ff_dim: int = C.FF_DIM,
        rms_eps: float = C.RMS_EPS,
    ):
        super().__init__()
        self.input_layernorm = RMSNorm(dim, rms_eps)
        self.self_attn = GroupedQueryAttention(dim, n_heads, n_kv_heads, head_dim)
        self.post_attention_layernorm = RMSNorm(dim, rms_eps)
        self.mlp = SwiGLU(dim, ff_dim, dim)

    def forward(
        self,
        x: torch.Tensor,
        cos: torch.Tensor,
        sin: torch.Tensor,
        position_ids: torch.Tensor,
        past_kv: torch.Tensor | None = None,
        use_cache: bool = False,
    ) -> Tuple[torch.Tensor, Tuple[torch.Tensor, torch.Tensor] | None]:
        # Self-attention branch (Pre-Norm).
        h, present_kv = self.self_attn(
            self.input_layernorm(x), cos, sin, position_ids, past_kv, use_cache
        )
        x = x + h
        # SwiGLU FFN branch (Pre-Norm).
        x = x + self.mlp(self.post_attention_layernorm(x))
        return x, present_kv


# ---------------------------------------------------------------------------
# Full model
# ---------------------------------------------------------------------------
class SenlightCoder(nn.Module):
    """Decoder-only transformer for causal code generation."""

    def __init__(
        self,
        vocab_size: int = C.VOCAB_SIZE,
        dim: int = C.MODEL_DIM,
        n_layers: int = C.N_LAYERS,
        n_heads: int = C.N_HEADS,
        n_kv_heads: int = C.N_KV_HEADS,
        head_dim: int = C.HEAD_DIM,
        ff_dim: int = C.FF_DIM,
        rope_theta: float = C.ROPE_THETA,
        max_len: int = C.ROPE_MAX_LEN,
        tie_embeddings: bool = C.TIE_EMBEDDINGS,
        rms_eps: float = C.RMS_EPS,
    ):
        super().__init__()
        self.dim = dim
        self.n_layers = n_layers
        self.n_heads = n_heads
        self.n_kv_heads = n_kv_heads
        self.ff_dim = ff_dim
        self.vocab_size = vocab_size
        self.tie_embeddings = tie_embeddings
        self.head_dim = head_dim

        # Token embedding and output projection.
        self.embed_tokens = nn.Embedding(vocab_size, dim)
        if tie_embeddings:
            # Weight tying: lm_head shares the embedding matrix.
            self.lm_head = nn.Linear(dim, vocab_size, bias=False)
            self.lm_head.weight = self.embed_tokens.weight
        else:
            self.lm_head = nn.Linear(dim, vocab_size, bias=False)

        self.layers = nn.ModuleList(
            [
                SenlightCoderBlock(dim, n_heads, n_kv_heads, head_dim, ff_dim, rms_eps)
                for _ in range(n_layers)
            ]
        )
        self.norm = RMSNorm(dim, rms_eps)

        # Precompute rotary tables.
        inv_freq, cos, sin = precompute_rope_frequencies(head_dim, max_len, rope_theta)
        self.register_buffer("cos", cos)      # (max_len, head_dim)
        self.register_buffer("sin", sin)      # (max_len, head_dim)
        self.register_buffer("inv_freq", inv_freq)

        self._post_init_weights()

    # ----- forward ----------------------------------------------------------
    def forward(
        self,
        input_ids: torch.Tensor,
        position_ids: torch.Tensor | None = None,
        past_kv: list | None = None,
        use_cache: bool = False,
    ) -> tuple[torch.Tensor, list]:
        """Args:
              input_ids: (batch, seq) token ids.
            Returns ``(logits, past_kvs)``; ``logits: (B, S, vocab)``.
        """
        bsz, seq_len = input_ids.shape

        if position_ids is None:
            if past_kv is None or use_cache is False:
                position_ids = torch.arange(seq_len, device=input_ids.device).unsqueeze(0)
                position_ids = position_ids.expand(bsz, -1)
            else:
                # Incremental decode: new positions start after cached length.
                past_len = past_kv[0][-1].size(-2) if past_kv else 0
                position_ids = (torch.arange(seq_len, device=input_ids.device) + past_len).unsqueeze(0)
                position_ids = position_ids.expand(bsz, -1)

        h = self.embed_tokens(input_ids)  # (B, S, dim)

        present_kvs: list = []
        for layer, block in enumerate(self.layers):
            layer_past = past_kv[layer] if (use_cache and past_kv) else None
            h, kv = block(h, self.cos, self.sin, position_ids, layer_past, use_cache)
            if use_cache:
                present_kvs.append(kv)

        h = self.norm(h)
        logits = self.lm_head(h)
        return logits, present_kvs

    def get_hidden_states(
        self, input_ids: torch.Tensor, position_ids: torch.Tensor | None = None
    ) -> torch.Tensor:
        """Return the final normalized hidden states (B, S, dim) — no lm_head.

        Used by the affective reward model / RL stage to build an emotional
        scoring head on top of the frozen backbone.
        """
        bsz, seq_len = input_ids.shape
        if position_ids is None:
            position_ids = torch.arange(seq_len, device=input_ids.device).unsqueeze(0)
            position_ids = position_ids.expand(bsz, -1)
        h = self.embed_tokens(input_ids)
        for block in self.layers:
            h, _ = block(h, self.cos, self.sin, position_ids)
        h = self.norm(h)
        return h

    def compute_loss(
        self, input_ids: torch.Tensor, labels: torch.Tensor
    ) -> Tuple[torch.Tensor, torch.Tensor]:
        """Causal next-token cross-entropy loss.

        The DataLoader already supplies **position-aligned** ``input_ids`` and
        ``labels``: ``input_ids[:, t]`` is the token at position ``t`` and
        ``labels[:, t]`` holds the *target* (token ``t+1``) that the model must
        predict there. A single forward on ``input_ids`` yields ``logits[:, t]``
        which we compare directly against ``labels[:, t]``. PAD tokens (from
        left-padded short windows) are masked with ``ignore_index``.

        Returns ``(loss, logits)`` where ``logits`` has shape ``(B, S, V)``.
        """
        logits, _ = self(input_ids)                      # (B, S, V)
        loss = F.cross_entropy(
            logits.reshape(-1, logits.size(-1)),
            labels.reshape(-1),
            ignore_index=C.PAD_ID,
        )
        return loss, logits

    # ----- weight init ------------------------------------------------------
    def _post_init_weights(self) -> None:
        """LLaMA-compatible parameter initialization (bias-free transformers)."""
        fan_in = self.dim
        for module in self.modules():
            if isinstance(module, (nn.Linear, nn.Embedding)):
                module.weight.data.normal_(mean=0.0, std=fan_in ** -0.5)
                if getattr(module, "bias", None) is not None:
                    module.bias.data.zero_()
        # Zero-init the FFN "down" projection and attention output projection
        # so residual streams begin as identity (no random signal at start).
        for module in self.modules():
            if isinstance(module, SwiGLU):
                module.down.weight.data.zero_()
            if isinstance(module, GroupedQueryAttention):
                module.o_proj.weight.data.zero_()
        # Re-tie embeddings after any re-init of the lm_head.
        if self.tie_embeddings:
            self.lm_head.weight = self.embed_tokens.weight


# Re-export the types for easy import in Phase 3.
__all__ = [
    "RMSNorm",
    "SwiGLU",
    "GroupedQueryAttention",
    "SenlightCoderBlock",
    "SenlightCoder",
    "precompute_rope_frequencies",
    "apply_rotary_pos_emb",
    "rotate_half",
]