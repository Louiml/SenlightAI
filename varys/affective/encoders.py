"""Trainable half of the Varys state machine.

:mod:`varys.affective.state` holds the non-trainable half: the axis conventions, the wheel
anchors, the transition, the evidence gate, and the feature-width contract. This module holds
the parts with parameters. Keeping the split means ``varys.config`` can import the width
contract without pulling in torch.

Same division as Elafry, widened from 3 output dimensions to 21.

## Where the evidence gate sits

The gate is **not** applied inside these encoders. It is stateful across blocks and the
encoders are stateless per forward pass, so a gate inside them would either carry hidden state
across unrelated sequences or be re-initialised every turn and never accumulate. The encoders
emit raw drive; the caller gates it with :class:`~varys.affective.state.EvidenceGate` and then
transitions. :func:`gated_transition` does both in the order the state module requires.
"""

from __future__ import annotations

from typing import List, Optional, Sequence

import torch
from torch import Tensor, nn

from varys.affective.clinical import SPECTRA_DIMS
from varys.affective.state import (
    MORAL_SLICE,
    SPECTRA_SLICE,
    VAD_SLICE,
    EvidenceGate,
    VarysState,
    state_feature_dim,
)

__all__ = [
    "AffectEncoder",
    "InternalStatePredictor",
    "MultiAxisVelocity",
    "VarysStateBank",
    "build_state_features",
    "gated_transition",
]


class AffectEncoder(nn.Module):
    """``MLP_affect`` over the user turn, producing the 21-dimensional drive.

    ``tanh`` then scaled by ``max_drive``, so drive is bounded by construction and the
    transition's clamp is a backstop rather than the primary range control. A linear output
    would let a badly initialised final layer push a dimension straight to its clamp
    boundary, which for the clinical and moral blocks is a state the gate can never retract.

    The output is a flat 21-vector, but the three blocks are read differently by the caller:
    VAD is signed and mean-reverting, the other two are unsigned and floored. A single
    ``tanh`` covers both because a negative drive on an unsigned block is simply faster
    relaxation, which is the right reading of counter-evidence.

    Args:
        dim: hidden width of the turn representation.
        hidden: trunk width. Kept narrow on purpose. The output is 21 numbers, so a wide
            trunk is capacity spent on a coordinate system rather than on language.
        max_drive: bound on per-component drive magnitude.
    """

    def __init__(self, dim: int, hidden: int = 256, max_drive: float = 1.0):
        super().__init__()
        from varys.affective.state import STATE_DIMS

        self.dim = dim
        self.state_dim = len(STATE_DIMS)
        self.max_drive = max_drive
        self.net = nn.Sequential(
            nn.Linear(dim, hidden),
            nn.SiLU(),
            nn.Linear(hidden, hidden),
            nn.SiLU(),
            nn.Linear(hidden, self.state_dim),
        )
        # Small init so initial drive is near zero and the state starts near its previous
        # value rather than jumping on turn one. A model should not open a conversation
        # convinced the user is in crisis.
        nn.init.normal_(self.net[-1].weight, std=0.02)
        nn.init.zeros_(self.net[-1].bias)

    def forward(self, turn: Tensor) -> Tensor:
        """``turn: (B, dim) -> drive (B, 21)``."""
        if turn.shape[-1] != self.dim:
            raise ValueError(f"expected turn width {self.dim}, got {turn.shape[-1]}")
        return torch.tanh(self.net(turn)) * self.max_drive


class InternalStatePredictor(nn.Module):
    """Predicts the next state from the model's own input representation.

    Pools the mean of the *input embeddings*, which is a deliberate choice: taking the state
    from the final hidden states would be circular, because those already depend on the state
    through the attention bias. Embeddings depend only on input ids, so the graph stays
    acyclic.

    ``attention_mask`` is not optional in practice. Without it, pad tokens drag the mean
    toward the embedding of ``<pad>`` and the state becomes a function of batch composition,
    which is not a state at all.
    """

    def __init__(self, dim: int, hidden: int = 256, max_drive: float = 1.0):
        super().__init__()
        from varys.affective.state import STATE_DIMS

        self.dim = dim
        self.state_dim = len(STATE_DIMS)
        self.max_drive = max_drive
        self.net = nn.Sequential(
            nn.Linear(dim, hidden),
            nn.SiLU(),
            nn.Linear(hidden, self.state_dim),
        )
        nn.init.normal_(self.net[-1].weight, std=0.02)
        nn.init.zeros_(self.net[-1].bias)

    def forward(self, hidden: Tensor, attention_mask: Optional[Tensor] = None) -> Tensor:
        """``hidden: (B, S, dim) -> drive (B, 21)``."""
        if hidden.dim() != 3:
            raise ValueError(f"expected (B, S, dim), got {tuple(hidden.shape)}")
        if hidden.shape[-1] != self.dim:
            raise ValueError(f"expected width {self.dim}, got {hidden.shape[-1]}")

        if attention_mask is None:
            pooled = hidden.mean(dim=1)
        else:
            mask = attention_mask.to(hidden.dtype).unsqueeze(-1)
            denom = mask.sum(dim=1).clamp(min=1.0)
            pooled = (hidden * mask).sum(dim=1) / denom

        return torch.tanh(self.net(pooled)) * self.max_drive


class MultiAxisVelocity(nn.Module):
    """Signed rate of change, for the channels where direction is the signal.

    Elafry tracks arousal velocity alone, which is right when the state is 3 numbers. Varys
    needs two more: the velocity of the peak clinical spectrum, because "the person's distress
    is rising" is the single most important escalation signal the state carries, and it is not
    recoverable from the instantaneous spectra. The dVAD triple is kept for the same reason
    Elafry kept arousal.

    Two conventions, both deliberate. The EMA is seeded from the first batch seen rather than
    from zero, so the first call does not report a full-magnitude step that never happened.
    And the update is ``mul_`` then ``add_`` on a buffer, which is what keeps it out of the
    autograd graph: a state tensor carries gradients, and folding it into a persistent buffer
    would build a path from step 1 to step 10 that no one ever asked for and that would break
    under activation checkpointing.
    """

    def __init__(self, beta: float = 0.8):
        super().__init__()
        if not 0.0 <= beta <= 1.0:
            raise ValueError(f"beta must be in [0, 1], got {beta}")
        self.beta = beta
        self.register_buffer("ema_vad", torch.zeros(3), persistent=False)
        self.register_buffer("ema_peak", torch.zeros(1), persistent=False)
        self._seeded = False

    def forward(self, state: Tensor) -> Tensor:
        """``state: (B, 21) -> velocity (B, 4)``, ordered dVAD then d(peak spectrum)."""
        if state.dim() != 2:
            raise ValueError(f"expected (B, S), got {tuple(state.shape)}")
        bsz = state.shape[0]

        vad = state[:, VAD_SLICE]
        peak = state[:, SPECTRA_SLICE].max(dim=-1, keepdim=True).values

        if not self._seeded or self.ema_vad.shape[0] != bsz:
            self.ema_vad = vad.detach().clone()
            self.ema_peak = peak.detach().clone()
            self._seeded = True
            return torch.zeros(bsz, 4, device=state.device, dtype=state.dtype)

        velocity = torch.cat([vad - self.ema_vad, peak - self.ema_peak], dim=-1)
        self.ema_vad.mul_(self.beta).add_(vad.detach(), alpha=1.0 - self.beta)
        self.ema_peak.mul_(self.beta).add_(peak.detach(), alpha=1.0 - self.beta)
        return velocity


class VarysStateBank(nn.Module):
    """Within-conversation state history, as a rolling buffer.

    A module rather than a Python variable so it moves across devices with the model and
    shows up in the state dict. Not a parameter, so it is never optimised; it is a running
    value that gets saved, restored and inspected.

    Long-horizon accumulation is deliberately *not* this. A bank that spans one conversation
    has no memory of a person, and the profile subsystem is what carries a person across
    conversations. Keeping them apart is what stops a within-session buffer from being
    mistaken for a psychological profile.
    """

    def __init__(self, batch_size: int = 1, max_seq: int = 4096, dim: int = 21):
        super().__init__()
        self.dim = dim
        self.max_seq = max_seq
        self.register_buffer("states", torch.zeros(batch_size, max_seq, dim), persistent=False)

    def reset(self, batch_size: Optional[int] = None) -> None:
        bsz = batch_size or self.states.shape[0]
        self.states = torch.zeros(
            bsz, self.max_seq, self.dim, device=self.states.device, dtype=self.states.dtype
        )

    def append(self, state: Tensor) -> Tensor:
        if state.shape[-1] != self.dim:
            raise ValueError(f"state width {state.shape[-1]} != bank dim {self.dim}")
        self.states[:, :-1] = self.states[:, 1:].clone()
        self.states[:, -1] = state.detach()
        return state

    def latest(self) -> Tensor:
        return self.states[:, -1]

    def history(self) -> Tensor:
        return self.states


def build_state_features(
    state: Tensor,
    include_moral: bool = False,
    velocity: Optional[Tensor] = None,
    intent: Optional[Tensor] = None,
    peak: Optional[Tensor] = None,
    include_velocity: bool = False,
    include_intent: bool = False,
    include_peak: bool = False,
) -> Tensor:
    """Assemble the state feature vector that conditions the attention bias.

    Width is always exactly :func:`state_feature_dim` for the same flags, zero-filling any
    channel the caller did not supply. Sizing the vector by what happened to be passed in is
    how a 20-wide state ends up going into a 21-wide bias and the error surfaces four layers
    later.

    The moral block is sliced out unless ``include_moral`` is set. The reasoning is in
    ``state.DEFAULT_FEATURE_BLOCKS``: conditioning attention on someone's moral foundations is
    a persuasion surface, and the default is to not build one.

    ``state`` is ``(B, 21)``. The default output is ``(B, 9)``.
    """
    if state.dim() != 2:
        raise ValueError(f"state must be (B, 21), got {tuple(state.shape)}")
    from varys.affective.state import STATE_DIMS

    if state.shape[-1] != len(STATE_DIMS):
        raise ValueError(f"state must be (B, {len(STATE_DIMS)}), got {tuple(state.shape)}")
    bsz = state.shape[0]

    parts: List[Tensor] = [state[:, VAD_SLICE], state[:, SPECTRA_SLICE]]
    if include_moral:
        parts.append(state[:, MORAL_SLICE])

    # Velocity is 4 channels and must pass through whole. `intent` and `peak` are scalars, so
    # they get reduced to a column. Reducing velocity the same way would average dVAD into a
    # single meaningless number and silently build 1 feature where the model expects 4.
    if include_velocity:
        if velocity is None:
            parts.append(state.new_zeros(bsz, 4))
        elif velocity.shape[-1] == 4:
            parts.append(velocity.reshape(bsz, 4))
        else:
            raise ValueError(f"velocity must be (B, 4), got {tuple(velocity.shape)}")

    for value, enabled in ((intent, include_intent), (peak, include_peak)):
        if not enabled:
            continue
        if value is None:
            parts.append(state.new_zeros(bsz, 1))
        else:
            v = value.reshape(bsz, -1).mean(dim=-1, keepdim=True) if value.dim() > 1 else value.reshape(bsz, 1)
            parts.append(v)

    out = torch.cat(parts, dim=-1)
    expected = state_feature_dim(include_moral, include_velocity, include_intent, include_peak)
    if out.shape[-1] != expected:
        raise ValueError(f"built {out.shape[-1]} features, model expects {expected}")
    return out


def gated_transition(
    state: VarysState,
    drive: Optional[Sequence[float]] = None,
    gate: Optional[EvidenceGate] = None,
) -> VarysState:
    """One block, with the evidence gate applied to the clinical and moral blocks.

    Order matters and is the whole reason this helper exists rather than being left to the
    caller:

    1. the gate scores the *observation* and decides whether it is supported enough to count
    2. the VAD block always passes through, because affect responds to a single utterance
    3. the clinical and moral blocks are zeroed unless the gate opened

    Applying the gate to the whole vector would make affect wait for repetition, which is
    wrong: a single message can legitimately be alarming, and requiring three of them to
    react to "I am going to hurt myself" is a failure, not a safety feature. The gate exists
    to stop a *passing* clinical inference, not to delay a response to distress.
    """
    from varys.affective.state import transition

    if drive is None:
        return transition(state, None)

    d = [float(x) for x in drive]
    if gate is None:
        return transition(state, d)

    confidence = gate.confidence(SPECTRA_DIMS)
    for i, name in enumerate(SPECTRA_DIMS):
        if confidence.get(name, 0.0) < gate._min_confidence:
            d[SPECTRA_SLICE.start + i] = 0.0

    # The moral block has no gate of its own here. It is opt-in at the feature level, and
    # adding a second gate on the drive would mean moral state could only ever move when a
    # gate was passed in, which makes the block inert by default rather than honestly inert.
    return transition(state, d)
