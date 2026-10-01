"""Affective primitives: the VAD convention, Plutchik anchors, and the state machine.

This package is a leaf. It imports nothing from ``elafry.config`` or ``elafry.models``, and
that constraint is deliberate: ``config`` needs the state-feature width contract and
``models`` needs the encoders, so the shared vocabulary has to live in a module both can
reach without a cycle.

Contents:

* ``state``     VAD convention, Plutchik anchors, the transition, feature-vector width
* ``encoders``  ``MLP_affect`` and the internal state predictor
"""

from elafry.affective.encoders import (
    AffectEncoder,
    AffectiveStateBank,
    InternalStatePredictor,
    Velocity,
)
from elafry.affective.state import (
    PLUTCHIK_DYADS,
    PLUTCHIK_PRIMARIES,
    PLUTCHIK_VAD,
    STATE_FEATURE_NAMES,
    AffectiveState,
    all_primary_states,
    build_state_features,
    classify_plutchik,
    clamp_state,
    initial_state,
    intensity_ladder,
    plutchik_tensor,
    state_feature_dim,
    transition,
)

__all__ = [
    "AffectEncoder",
    "AffectiveState",
    "AffectiveStateBank",
    "InternalStatePredictor",
    "PLUTCHIK_DYADS",
    "PLUTCHIK_PRIMARIES",
    "PLUTCHIK_VAD",
    "STATE_FEATURE_NAMES",
    "Velocity",
    "all_primary_states",
    "build_state_features",
    "classify_plutchik",
    "clamp_state",
    "initial_state",
    "intensity_ladder",
    "plutchik_tensor",
    "state_feature_dim",
    "transition",
]