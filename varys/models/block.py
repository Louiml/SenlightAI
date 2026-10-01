"""One transformer block: pre-norm attention, then pre-norm SwiGLU, both residual.

Pre-norm throughout. The residual projections ``o_proj`` and ``down`` are zero-initialised so
every block starts as the identity, which means a deep stack trains as a shallow one and
degrades gracefully instead of diverging at step 1.

The one difference from Elafry's block is the signature of the attention call. Elafry passes
a single pre-sliced state vector; Varys passes the state blocks the bias actually reads, so
the block decides nothing about which blocks are enabled and the model does.
"""

from __future__ import annotations

from typing import Dict, Optional, Tuple

from torch import Tensor, nn

from varys.models.attention import GroupedQueryAttention
from varys.models.mlp import SwiGLU
from varys.models.norm import RMSNorm

__all__ = ["VarysBlock"]


class VarysBlock(nn.Module):
    def __init__(
        self,
        dim: int,
        n_heads: int,
        n_kv_heads: int,
        head_dim: int,
        ff_dim: int,
        rms_eps: float = 1e-5,
        block_widths: Optional[Dict[str, int]] = None,
        block_scales: Optional[Dict[str, float]] = None,
        affect_granularity: Optional[str] = "head",
        inject_affect: bool = True,
    ):
        super().__init__()
        self.inject_affect = inject_affect

        self.input_layernorm = RMSNorm(dim, eps=rms_eps)
        self.self_attn = GroupedQueryAttention(
            dim=dim,
            n_heads=n_heads,
            n_kv_heads=n_kv_heads,
            head_dim=head_dim,
            block_widths=block_widths,
            block_scales=block_scales,
            affect_granularity=affect_granularity if inject_affect else None,
        )
        self.post_attention_layernorm = RMSNorm(dim, eps=rms_eps)
        self.mlp = SwiGLU(dim, ff_dim)

        self._init_residuals()

    def _init_residuals(self) -> None:
        """Zero the projections that write into the residual stream.

        ``o_proj`` and ``down`` both write additively into ``h``. Starting them at zero means
        the block computes ``h + 0 = h`` on the first forward pass, so a freshly
        initialised 80-layer model is numerically identical to a 1-layer one. Gradients still
        reach the zeroed weights because the upstream activations are non-zero.

        This is what makes the 72B preset safe to instantiate at all: without it, a
        randomly-initialised 80-layer residual stack is a diverging model, and the only way
        to find out is to allocate 143GB of bf16 to watch it.
        """
        nn.init.zeros_(self.self_attn.o_proj.weight)
        nn.init.zeros_(self.mlp.down.weight)

    def forward(
        self,
        x: Tensor,
        cos: Tensor,
        sin: Tensor,
        past_kv: Optional[Tuple[Tensor, Tensor]] = None,
        use_cache: bool = False,
        state: Optional[Tensor] = None,
    ) -> Tuple[Tensor, Optional[Tuple[Tensor, Tensor]]]:
        """``state`` is passed through untouched; layers without injection ignore it."""
        residual = x
        h = self.input_layernorm(x)
        h, present = self.self_attn(h, cos, sin, past_kv, use_cache, state)
        x = residual + h

        residual = x
        h = self.post_attention_layernorm(x)
        h = self.mlp(h)
        x = residual + h

        return x, present

    def extra_repr(self) -> str:
        return f"inject_affect={self.inject_affect}"
