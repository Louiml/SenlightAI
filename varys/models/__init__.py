"""The Varys model, in dependency order.

    1. norm.py          RMSNorm
    2. mlp.py           SwiGLU
    3. rope.py          rotary embeddings plus linear, NTK and YaRN long-context extension
    4. attention.py     grouped-query attention with multi-axis affective bias
    5. block.py         one pre-norm transformer block. Depends on 1, 2 and 4.
    6. state_machine.py the 21-dim recurrence on tensors, with the evidence gate
    7. router.py        intent routing and clinical response stance
    8. varys.py         the decoder. Depends on everything above it.

Each entry depends only on the ones above it, and nothing imports ``varys.py``, so a test can
pull any single piece in isolation.

Two ordering notes worth having written down:

* ``state_machine`` sits after ``block`` for readability, not because it depends on the block.
  It depends only on ``varys.affective.state`` and torch. It is the tensor implementation of
  the same recurrence ``varys.affective.state.transition`` expresses in Python, and the two are
  pinned to agree by ``test_torch_transition_matches_python``.
* ``rope`` is deliberately third. It has no internal dependencies, but it is the module most
  likely to be reached for when a long-context model misbehaves, and putting it above
  ``attention`` means a reader chasing a position bug does not pass through the bias code
  first.

As in the package root, this ordering is documentation rather than filesystem layout. Git
records no directory order and GitHub sorts the tree alphabetically, so a dependency ordering
can only live here and in the README.
"""

from varys.models.attention import GroupedQueryAttention, MultiAxisBias
from varys.models.block import VarysBlock
from varys.models.mlp import SwiGLU
from varys.models.norm import RMSNorm
from varys.models.rope import (
    apply_rope,
    ntk_theta,
    precompute_rope,
    rope_from_config,
    rotate_half,
    yarn_mscale,
)
from varys.models.router import (
    COMPETING_INTENTS,
    INTENTS,
    ROUTING_MODES,
    STANCES,
    IntentDecision,
    IntentRouter,
    StanceDecision,
    clinical_stance,
)
from varys.models.state_machine import (
    STATE_BLOCK_SLICES,
    GatedStateMachine,
    slice_state,
    transition_tensor,
    zero_state,
)
from varys.models.varys import Varys, VarysOutput, param_breakdown, sample_from_logits

__all__ = [
    # primitives
    "RMSNorm", "SwiGLU",
    # rope
    "precompute_rope", "rope_from_config", "apply_rope", "rotate_half",
    "ntk_theta", "yarn_mscale",
    # attention
    "GroupedQueryAttention", "MultiAxisBias", "VarysBlock",
    # state machine
    "GatedStateMachine", "transition_tensor", "zero_state", "slice_state",
    "STATE_BLOCK_SLICES",
    # routing
    "INTENTS", "ROUTING_MODES", "COMPETING_INTENTS", "STANCES", "IntentRouter", "IntentDecision",
    "clinical_stance", "StanceDecision",
    # decoder
    "Varys", "VarysOutput", "sample_from_logits", "param_breakdown",
]
