"""Encoders that turn a turn into the drive on the affective state.

These are the trainable half of the state machine. ``elafry.affective.state`` holds the
non-trainable half: the VAD convention, the Plutchik anchors, the transition, and the
feature-vector width contract.

The split matters for imports. ``elafry.config`` needs the width contract, and
``elafry.models`` needs the encoders. If both sat in ``elafry.models``, importing the config
would pull in the decoder and the cycle would close.
"""

from __future__ import annotations

from typing import Optional

import torch
from torch import Tensor, nn

from elafry.affective.state import (
    STATE_FEATURE_NAMES,
    build_state_features,
    state_feature_dim,
)

__all__ = [
    "AffectEncoder",
    "InternalStatePredictor",
    "Velocity",
    "AffectiveStateBank",
    "STATE_FEATURE_NAMES",
    "build_state_features",
    "state_feature_dim",
]

class AffectEncoder(nn.Module):
    """``MLP_affect`` over the user turn. Produces the drive for the state transition.

    The output passes through ``tanh`` and is then scaled by ``max_drive``, so the drive is
    bounded by construction and the transition's clamp is a backstop rather than the primary
    range control. A linear output here lets a badly initialised layer push the state
    straight to the clamp boundary, which is a state the model can never leave.

    Args:
        dim: hidden size of the turn representation fed in.
        hidden: width of the MLP trunk. Small on purpose: the state is 3 numbers, so a wide
            trunk is capacity spent on a coordinate system.
        max_drive: bound on the per-component drive magnitude.
    """

    def __init__(self, dim: int, hidden: int = 256, max_drive: float = 1.0):
        super().__init__()
        self.dim = dim
        self.max_drive = max_drive
        self.net = nn.Sequential(
            nn.Linear(dim, hidden),
            nn.SiLU(),
            nn.Linear(hidden, hidden),
            nn.SiLU(),
            nn.Linear(hidden, 3),
        )
        # Small init so the initial drive is near zero and the state starts near its
        # previous value rather than jumping on turn one.
        nn.init.normal_(self.net[-1].weight, std=0.02)
        nn.init.zeros_(self.net[-1].bias)

    def forward(self, turn: Tensor) -> Tensor:
        """``turn: (B, dim) -> drive (B, 3)``."""
        if turn.shape[-1] != self.dim:
            raise ValueError(f"expected turn width {self.dim}, got {turn.shape[-1]}")
        return torch.tanh(self.net(turn)) * self.max_drive


class InternalStatePredictor(nn.Module):
    """Reads the model's own input representation and predicts the next state.

    Operates on the mean of the input embeddings, which is a deliberate choice rather than an
    oversight: taking the state from the model's *final* hidden states would be circular,
    because those depend on the state through the attention bias. Embeddings depend only on
    the input ids, so the graph stays acyclic.

    ``attention_mask`` matters more than it looks. Without it, pad tokens drag the mean
    toward the embedding of ``<pad>`` and the state becomes a function of batch composition,
    which is not a state at all.

    The loss has to reach this module, so the caller keeps the graph alive by threading the
    returned drive through the forward pass rather than discarding it.
    """

    def __init__(self, dim: int, hidden: int = 256, max_drive: float = 1.0):
        super().__init__()
        self.dim = dim
        self.max_drive = max_drive
        self.net = nn.Sequential(
            nn.Linear(dim, hidden),
            nn.SiLU(),
            nn.Linear(hidden, 3),
        )
        nn.init.normal_(self.net[-1].weight, std=0.02)
        nn.init.zeros_(self.net[-1].bias)

    def forward(self, hidden: Tensor, attention_mask: Optional[Tensor] = None) -> Tensor:
        """``hidden: (B, S, dim) -> drive (B, 3)``.

        ``attention_mask`` is ``(B, S)`` with 1 for real tokens and 0 for padding.
        """
        if hidden.dim() != 3:
            raise ValueError(f"expected (B, S, dim), got {tuple(hidden.shape)}")
        if hidden.shape[-1] != self.dim:
            raise ValueError(f"expected width {self.dim}, got {hidden.shape[-1]}")

        if attention_mask is None:
            pooled = hidden.mean(dim=1)
        else:
            mask = attention_mask.to(hidden.dtype).unsqueeze(-1)  # (B, S, 1)
            denom = mask.sum(dim=1).clamp(min=1.0)
            pooled = (hidden * mask).sum(dim=1) / denom

        return torch.tanh(self.net(pooled)) * self.max_drive


class Velocity(nn.Module):
    """Exponential-moving-average change in the state, one scalar channel.

    Feeds the ``arousal_velocity`` feature. Whether the model is becoming more or less
    activated across a conversation is not recoverable from the instantaneous state, and it
    is part of what a model needs in order to decide whether to escalate or de-escalate.
    """

    def __init__(self, beta: float = 0.8):
        super().__init__()
        if not 0.0 <= beta <= 1.0:
            raise ValueError(f"beta must be in [0, 1], got {beta}")
        self.beta = beta
        self.register_buffer("ema", torch.zeros(1), persistent=False)

    def forward(self, arousal: Tensor) -> Tensor:
        """``arousal: (B,) or (B, 1) -> velocity (B,)``, signed change since the last step."""
        if arousal.dim() == 2 and arousal.shape[-1] == 1:
            arousal = arousal.squeeze(-1)
        if arousal.dim() != 1:
            raise ValueError(f"expected (B,) or (B, 1), got {tuple(arousal.shape)}")
        bsz = arousal.shape[0]

        if self.ema.numel() != bsz:
            self.ema = torch.zeros(bsz, device=arousal.device, dtype=arousal.dtype)

        velocity = arousal - self.ema
        self.ema.mul_(self.beta).add_(arousal.detach(), alpha=1.0 - self.beta)
        return velocity


class AffectiveStateBank(nn.Module):
    """Holds the state across a conversation as a rolling buffer of trailing values.

    A module rather than a Python variable so the state moves with the model across devices
    and shows up in the state dict. Not a parameter, so it is never optimised; it is a
    running value that gets saved, restored and inspected.
    """

    def __init__(self, batch_size: int = 1, max_seq: int = 4096, dim: int = 3):
        super().__init__()
        self.dim = dim
        self.max_seq = max_seq
        self.register_buffer("states", torch.zeros(batch_size, max_seq, dim), persistent=False)

    def reset(self, batch_size: Optional[int] = None) -> None:
        """Zero the bank. Call at the start of a conversation."""
        bsz = batch_size or self.states.shape[0]
        self.states = torch.zeros(
            bsz, self.max_seq, self.dim, device=self.states.device, dtype=self.states.dtype
        )

    def append(self, state: Tensor) -> Tensor:
        """Append a ``(B, dim)`` state, rolling the buffer left when full."""
        if state.shape[-1] != self.dim:
            raise ValueError(f"state width {state.shape[-1]} != bank dim {self.dim}")
        # Roll left, then write the new column. In-place so the buffer stays one allocation.
        self.states[:, :-1] = self.states[:, 1:].clone()
        self.states[:, -1] = state.detach()
        return state

    def latest(self) -> Tensor:
        return self.states[:, -1]

    def history(self) -> Tensor:
        """``(B, seq, dim)`` of everything held. For trend and volatility analysis."""
        return self.states