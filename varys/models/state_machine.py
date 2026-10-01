"""Torch state machine: the 21-dim recurrence, on tensors.

## Why this is a second implementation rather than a call into ``affective.state``

``varys.affective.state.transition`` operates on Python floats, which is the right thing for
a module that has to be readable, inspectable and testable without a GPU. It is the wrong
thing inside a training loop: a 72B run does millions of state updates, and Python-level
float arithmetic over a 21-vector turns each one into hundreds of interpreter operations
with no chance of ever being fused into a kernel.

So the recurrence exists twice, and that is a real risk: two implementations of the same
equations that quietly disagree is a genuinely bad failure mode, because the model trains
against one of them and every evaluation, every export and every explanation of the model's
behaviour is computed against the other. The difference would be small, it would be
invisible in a loss curve, and it would be wrong.

``tests/test_models.py::test_torch_transition_matches_python`` pins them to a tight
tolerance. If someone changes one recurrence and not the other, that test fails. It is the
single most important test in this module.

## The gate is here, not in the encoders

The evidence gate is stateful across blocks, so it cannot live in a stateless encoder. It
lives here, as buffers on a module, which means it moves with the model across devices, is
saved and restored, and is visible in ``state_dict`` for inspection. Buffers, not
parameters: a gate is running bookkeeping, and training it would be training the model to
decide what counts as evidence about itself.
"""

from __future__ import annotations

from typing import Dict, Optional

import torch
from torch import Tensor, nn

from varys.affective.clinical import SPECTRA_DIMS
from varys.affective.state import (
    MORAL_SLICE,
    SPECTRA_BASELINE,
    SPECTRA_SLICE,
    STATE_DIMS,
    VAD_SLICE,
)

__all__ = [
    "STATE_BLOCK_SLICES",
    "zero_state",
    "slice_state",
    "transition_tensor",
    "GatedStateMachine",
]


#: Where each block lives in the 21-dim state. The single source of truth for the layout;
#: everything that needs to know the shape reads it from here.
STATE_BLOCK_SLICES: Dict[str, slice] = {
    "vad": VAD_SLICE,
    "spectra": SPECTRA_SLICE,
    "moral": MORAL_SLICE,
}


def zero_state(batch_size: int, device=None, dtype=torch.float32) -> Tensor:
    """A neutral state. Not a neutral *person*: a profile with all-zero clinical baselines
    is someone with nothing to report."""
    return torch.zeros(batch_size, len(STATE_DIMS), device=device, dtype=dtype)


def slice_state(state: Tensor) -> Dict[str, Tensor]:
    """Split a ``(B, 21)`` state into its blocks."""
    return {name: state[:, sl] for name, sl in STATE_BLOCK_SLICES.items()}


def transition_tensor(
    state: Tensor,
    drive: Optional[Tensor] = None,
    vad_decay: float = 0.70,
    spectra_decay: float = 0.94,
    moral_decay: float = 0.98,
    baseline: float = SPECTRA_BASELINE,
    spectra_rise: float = 0.60,
    moral_rise: float = 0.35,
    drive_gain: float = 1.0,
) -> Tensor:
    """Advance one block, on tensors. Numerically equivalent to the Python recurrence.

    Three blocks, three recurrences, for the reason in
    :mod:`varys.affective.state`: affect is fast and mean-reverting to the origin, clinical
    spectra are slow and floored once raised, moral grounding is slower still and has no
    floor.

    The clinical latch (``current > 0``) is the subtle part and it is easy to write
    differently from the Python version by accident. See
    :func:`varys.affective.state.transition` for why comparing against the baseline rather
    than against zero fails exactly at the boundary.
    """
    if state.dim() != 2 or state.shape[-1] != len(STATE_DIMS):
        raise ValueError(f"state must be (B, {len(STATE_DIMS)}), got {tuple(state.shape)}")

    if drive is None:
        d = torch.zeros_like(state)
    else:
        if drive.shape != state.shape:
            raise ValueError(f"drive {tuple(drive.shape)} must match state {tuple(state.shape)}")
        d = drive * drive_gain

    out = state.clone()

    # Every block is clamped explicitly. The Python recurrence in affective.state gets its
    # clamping for free from VAD.__post_init__ and clamp_spectra/clamp_moral, which run when
    # the state object is constructed. Relying on that here would be the whole problem: two
    # implementations of one recurrence that agree while the values are in range and diverge
    # the moment anything saturates. Spectra reached 1.47 and VAD left the cube before this
    # was caught, and the divergence is invisible in a loss curve.
    #
    # Ranges differ by block and that is load-bearing: VAD is signed in [-1, 1], the other
    # two are intensities in [0, 1] with no meaningful zero-crossing.
    v = VAD_SLICE
    out[:, v] = torch.clamp(vad_decay * state[:, v] + d[:, v], -1.0, 1.0)

    # Clinical: slow to rise, floored once raised, never raised means exactly zero.
    s = SPECTRA_SLICE
    current = state[:, s]
    nxt = spectra_decay * current + spectra_rise * d[:, s]
    nxt = torch.where(current > 0.0, torch.clamp(nxt, min=baseline), nxt)
    out[:, s] = torch.clamp(nxt, 0.0, 1.0)

    # Moral: slowest, stickiest, no floor.
    m = MORAL_SLICE
    out[:, m] = torch.clamp(moral_decay * state[:, m] + moral_rise * d[:, m], 0.0, 1.0)

    return out


class GatedStateMachine(nn.Module):
    """The state machine as a module: buffers that move with the model, not parameters.

    Holds the 21-dim state, the per-dimension gate confidence for the clinical block, and
    the learned peak-spectrum readout used by the velocity channel.

    Args:
        state_dim: always 21, asserted rather than configured. The block layout is not a
            hyperparameter; changing it changes what the state *means*.
        gate_clinical: when false, the clinical block is driven unconditionally. Off by
            default because the gate is what stops a passing mention of death from being
            reported as a spectrum.
        gate_min_confidence: confidence at which a clinical dimension starts driving.
        gate_decay: EMA rate for the confidence. Higher forgets faster.
    """

    def __init__(
        self,
        state_dim: int = len(STATE_DIMS),
        gate_clinical: bool = True,
        gate_min_confidence: float = 0.5,
        gate_decay: float = 0.5,
    ):
        super().__init__()
        if state_dim != len(STATE_DIMS):
            raise ValueError(
                f"state_dim is {len(STATE_DIMS)} by construction, got {state_dim}; the block "
                "layout is not configurable"
            )
        self.state_dim = state_dim
        self.gate_clinical = gate_clinical
        self.gate_min_confidence = gate_min_confidence
        self.gate_decay = gate_decay

        # persistent=False: the gate is inference bookkeeping. Saving it would put a
        # conversation's evidence counts in a checkpoint and make a checkpoint depend on
        # where in a conversation it was saved.
        self.register_buffer("state", zero_state(1), persistent=False)
        self.register_buffer("gate_conf", torch.zeros(len(SPECTRA_DIMS)), persistent=False)

    @torch.no_grad()
    def reset(self, batch_size: int = 1) -> Tensor:
        """Zero the state and the gate. Call at the start of every conversation."""
        self.state = zero_state(batch_size, device=self.state.device)
        self.gate_conf = torch.zeros(
            len(SPECTRA_DIMS), device=self.gate_conf.device, dtype=self.gate_conf.dtype
        )
        return self.state

    @torch.no_grad()
    def step(
        self,
        drive: Tensor,
        clinical_hits: Optional[Tensor] = None,
        cfg: Optional[object] = None,
        state: Optional[Tensor] = None,
    ) -> Tensor:
        """Advance one block.

        Args:
            drive: ``(B, 21)`` raw drive from an encoder.
            clinical_hits: ``(B, 6)`` per-spectrum observation strength in ``[0, 1]``.
                ``None`` means "nothing observed", which is an all-zero observation rather
                than a bypass: the confidence decays toward closed and the clinical block is
                held. A tensor is the normalised-EMA input, the observed strength rather
                than a score, and the gate decides what to do with it.
            cfg: an :class:`~varys.config.base.AffectConfig`, for the decay constants. The
                constants live on the config rather than here so a checkpoint carries the
                dynamics it was trained with.
            state: the state to advance. ``None`` uses the module's own, which is the
                decode path. Pass it explicitly when a caller is holding a state it wants to
                keep, such as a batch wider than the buffer: reading the buffer and ignoring
                it is how a batch-2 call ends up trying to broadcast a batch-1 state.
        """
        cur = self.state if state is None else state
        if cur.shape[0] != drive.shape[0]:
            raise ValueError(
                f"state batch {cur.shape[0]} does not match drive batch {drive.shape[0]}; "
                "pass state= explicitly or reset the machine to this batch size"
            )
        drive = drive.to(cur.dtype)
        d = drive

        if self.gate_clinical:
            # The clinical block is gated whenever the gate is on, *including* on a step
            # where nothing was observed.
            #
            # The earlier version only gated when `clinical_hits is not None`, which made
            # "no clinical detector" mean "no gating, pass the drive straight through". That
            # is the unsafe default: a caller without a detector got unconditional clinical
            # updates, which is exactly the case the gate exists to prevent. Silence is
            # evidence of nothing, so a None hits tensor is an all-zero observation and the
            # confidence decays toward closed like any other absent observation.
            if clinical_hits is None:
                hits = torch.zeros_like(drive[:, SPECTRA_SLICE])
            else:
                if clinical_hits.shape != drive[:, SPECTRA_SLICE].shape:
                    raise ValueError(
                        f"clinical_hits {tuple(clinical_hits.shape)} must match the spectra "
                        f"block {tuple(drive[:, SPECTRA_SLICE].shape)}"
                    )
                hits = clinical_hits.to(drive.dtype)

            conf = (1.0 - self.gate_decay) * self.gate_conf + self.gate_decay * hits
            self.gate_conf = conf
            open_mask = (conf >= self.gate_min_confidence).to(drive.dtype)
            gated = drive[:, SPECTRA_SLICE] * open_mask
            d = torch.cat([drive[:, VAD_SLICE], gated, drive[:, MORAL_SLICE]], dim=-1)

        self.state = transition_tensor(
            cur,
            d,
            vad_decay=getattr(cfg, "vad_decay", 0.70),
            spectra_decay=getattr(cfg, "spectra_decay", 0.94),
            moral_decay=getattr(cfg, "moral_decay", 0.98),
            baseline=getattr(cfg, "spectra_baseline", SPECTRA_BASELINE),
            drive_gain=getattr(cfg, "drive_gain", 1.0),
        )
        return self.state

    def state_features(
        self,
        include_moral: bool = False,
        include_velocity: bool = False,
        include_intent: bool = False,
        velocity: Optional[Tensor] = None,
        intent: Optional[Tensor] = None,
    ) -> Tensor:
        """Slice the state down to what the attention bias reads.

        Slicing happens here rather than in the model so the block layout has exactly one
        owner, and so this function can be handed to a test that checks the width against
        :func:`varys.affective.state.state_feature_dim` without building a model.
        """
        from varys.affective.encoders import build_state_features

        return build_state_features(
            self.state,
            include_moral=include_moral,
            velocity=velocity,
            intent=intent,
            include_velocity=include_velocity,
            include_intent=include_intent,
        )

    def peak_spectrum(self) -> Tensor:
        """The most marked clinical spectrum, and its index. ``(B,)`` each.

        Index -1 when nothing is marked. Returned as a tensor rather than a label because
        this is read inside the forward pass; the caller that wants a name calls
        :func:`varys.affective.state.describe`.
        """
        spectra = self.state[:, SPECTRA_SLICE]
        marked = spectra > 0.0
        any_marked = marked.any(dim=-1)
        idx = spectra.argmax(dim=-1)
        idx = torch.where(any_marked, idx, torch.full_like(idx, -1))
        peak = torch.where(any_marked, spectra.max(dim=-1).values, torch.zeros_like(spectra[:, 0]))
        return idx, peak

    def extra_repr(self) -> str:
        return (
            f"state_dim={self.state_dim}, gate_clinical={self.gate_clinical}, "
            f"min_confidence={self.gate_min_confidence}, gate_decay={self.gate_decay}"
        )
