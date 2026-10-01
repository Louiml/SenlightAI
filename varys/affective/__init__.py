"""The Varys affective state.

Six modules, and the dependency order is not arbitrary:

    vad.py       3 signed dimensions, carried over from Elafry unchanged
    plutchik.py  32 nodes on the VAD cube, as a hierarchy
    clinical.py  6 HiTOP spectra, unsigned
    moral.py     12 Moral Foundation slots, unsigned, both poles
    state.py     composes the three blocks, torch-free
    encoders.py  the trainable half, the only module that imports torch

The order is the reason ``state.py`` holds no torch import. ``varys.config`` needs the feature
width, ``varys.models`` needs the encoders; if the width lived alongside the encoders, reading
a config would pull in the tensor stack and the import graph would stop being a tree.

The one thing to know before using any of it: the clinical block is a set of organising axes,
not a diagnosis, and the moral block is a persuasion surface, which is why it is excluded from
the attention bias by default. Both are explained where they are defined.
"""

from varys.affective.clinical import (
    SPECTRA,
    SPECTRA_DIMS,
    SpectrumVector,
    cooccurring,
    screening_flags,
)
from varys.affective.encoders import (
    AffectEncoder,
    InternalStatePredictor,
    MultiAxisVelocity,
    VarysStateBank,
    build_state_features,
    gated_transition,
)
from varys.affective.moral import (
    FOUNDATIONS,
    MORAL_DIMS,
    MoralVector,
    is_manipulation_risk,
)
from varys.affective.plutchik import (
    DYADS,
    PRIMARIES,
    TERTIARIES,
    WHEEL,
    classify_affect,
    derive_anchor,
)
from varys.affective.state import (
    DEFAULT_FEATURE_BLOCKS,
    MORAL_DECAY,
    MORAL_SLICE,
    SPECTRA_BASELINE,
    SPECTRA_DECAY,
    SPECTRA_SLICE,
    STATE_DIMS,
    VAD_DECAY,
    VAD_SLICE,
    EvidenceGate,
    VarysState,
    describe,
    feature_names,
    neutral_state,
    state_feature_dim,
    transition,
)
from varys.affective.vad import VAD, VAD_AXES, neutral_vad, vad_distance, vad_similarity

__all__ = [
    # VAD
    "VAD", "VAD_AXES", "neutral_vad", "vad_distance", "vad_similarity",
    # Plutchik
    "PRIMARIES", "DYADS", "TERTIARIES", "WHEEL", "classify_affect", "derive_anchor",
    # Clinical
    "SPECTRA", "SPECTRA_DIMS", "SpectrumVector", "cooccurring", "screening_flags",
    # Moral
    "FOUNDATIONS", "MORAL_DIMS", "MoralVector", "is_manipulation_risk",
    # State
    "VarysState", "neutral_state", "transition", "EvidenceGate", "STATE_DIMS",
    "VAD_DECAY", "SPECTRA_DECAY", "MORAL_DECAY", "SPECTRA_BASELINE",
    "VAD_SLICE", "SPECTRA_SLICE", "MORAL_SLICE",
    "state_feature_dim", "feature_names", "describe", "DEFAULT_FEATURE_BLOCKS",
    # Encoders
    "AffectEncoder", "InternalStatePredictor", "MultiAxisVelocity", "VarysStateBank",
    "build_state_features", "gated_transition",
]
