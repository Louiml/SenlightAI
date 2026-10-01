"""Model components: the decoder, the attention with affective injection, and the router.

The affective primitives (VAD, Plutchik, the transition, the encoders) live in
``elafry.affective`` rather than here, because ``elafry.config`` needs them too and this
package imports the config. The most common names are re-exported for convenience.
"""

from elafry.affective.state import (
    PLUTCHIK_DYADS,
    PLUTCHIK_PRIMARIES,
    PLUTCHIK_VAD,
    AffectiveState,
    classify_plutchik,
    transition,
)
from elafry.models.attention import AffectiveBias, GroupedQueryAttention
from elafry.models.block import ElafryBlock
from elafry.models.elafry import Elafry, ElafryOutput, param_breakdown, sample_from_logits
from elafry.models.mlp import SwiGLU
from elafry.models.norm import RMSNorm
from elafry.models.router import INTENTS, ROUTING_MODES, IntentRouter
from elafry.models.rope import apply_rope, precompute_rope

__all__ = [
    "AffectiveBias",
    "AffectiveState",
    "Elafry",
    "ElafryBlock",
    "ElafryOutput",
    "GroupedQueryAttention",
    "INTENTS",
    "PLUTCHIK_DYADS",
    "PLUTCHIK_PRIMARIES",
    "PLUTCHIK_VAD",
    "RMSNorm",
    "ROUTING_MODES",
    "IntentRouter",
    "SwiGLU",
    "apply_rope",
    "classify_plutchik",
    "param_breakdown",
    "precompute_rope",
    "sample_from_logits",
    "transition",
]