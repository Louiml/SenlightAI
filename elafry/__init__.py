"""Senlight Elafry: Artificial Emotional Intelligence.

The architectural claim, in one paragraph. A standard RLHF-trained language model is
subservient by construction: it agrees with the user because agreement is what the reward
signal pays for. Elafry replaces that with an internal affective state that acts as the
*prior* on generation rather than a metric logged after it. The state enters the
Transformer through a bias added to the pre-softmax attention logits, so emotion changes
token probabilities at the arithmetic level instead of through prompt wording.

Modules are grouped as:

* ``config``   model, affect and training configuration, plus the named preset ladder
* ``models``   the decoder, the affective state machine, and the intent router
* ``tokenization`` control tokens and the BPE trainer that emits them
* ``data``     record contracts and sequence packing (no corpora ship with this package)
* ``train``    SFT, the affective reward model, and PG-DPO
* ``eval``     the sycophancy harness and the pinned-state ablation
* ``export``   safetensors writer that emits a ``config.json`` sidecar
"""

__version__ = "0.1.0"

from elafry.config.base import AffectConfig, ModelConfig, TrainConfig
from elafry.config.presets import PRESETS, get_preset, list_presets, param_count

__all__ = [
    "AffectConfig",
    "ModelConfig",
    "TrainConfig",
    "PRESETS",
    "get_preset",
    "list_presets",
    "param_count",
    "__version__",
]