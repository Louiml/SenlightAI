"""Root-mean-square layer normalisation.

Pre-norm placement (norm before attention and before the MLP, not after) is what makes deep
stacks trainable without a warmup so long you would not believe the schedule. The gain is a
learned weight; there is no bias and no mean subtraction.
"""

from __future__ import annotations

import torch
from torch import Tensor, nn

__all__ = ["RMSNorm"]


class RMSNorm(nn.Module):
    """``y = x * rsqrt(mean(x^2) + eps) * gamma``

    Normalising in float32 even under autocast is deliberate: the mean of squares is a
    reduction over thousands of elements, and bf16 has enough mantissa to drift on it.
    """

    def __init__(self, dim: int, eps: float = 1e-5):
        super().__init__()
        self.dim = dim
        self.eps = eps
        self.weight = nn.Parameter(torch.ones(dim))

    def forward(self, x: Tensor) -> Tensor:
        orig_dtype = x.dtype
        x32 = x.float()
        # rsqrt rather than pow(x, -0.5): one op instead of two, and no negative-base risk.
        scale = torch.rsqrt(x32.pow(2).mean(dim=-1, keepdim=True) + self.eps)
        out = (x32 * scale).to(orig_dtype) * self.weight
        return out

    def extra_repr(self) -> str:
        return f"dim={self.dim}, eps={self.eps}"