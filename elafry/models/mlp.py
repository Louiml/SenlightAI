"""SwiGLU feed-forward network.

``down(silu(gate(x)) * up(x))``. Three matrices instead of two, which is why ``ff_dim`` is
roughly 8/3 of ``dim`` rather than 4x: SwiGLU gets more parameters for less compute, so the
hidden width shrinks to compensate.
"""

from __future__ import annotations


import torch.nn.functional as F
from torch import Tensor, nn

__all__ = ["SwiGLU"]


class SwiGLU(nn.Module):
    def __init__(self, dim: int, hidden_dim: int):
        super().__init__()
        self.dim = dim
        self.hidden_dim = hidden_dim
        self.gate = nn.Linear(dim, hidden_dim, bias=False)
        self.up = nn.Linear(dim, hidden_dim, bias=False)
        self.down = nn.Linear(hidden_dim, dim, bias=False)

    def forward(self, x: Tensor) -> Tensor:
        return self.down(F.silu(self.gate(x)) * self.up(x))

    def extra_repr(self) -> str:
        return f"dim={self.dim}, hidden_dim={self.hidden_dim}"