"""Training: optimisation, checkpointing, and the three PG-RL stages."""

from elafry.train.checkpoint import (
    CONFIG_SIDECAR,
    TrainState,
    checkpoint_payload,
    load_checkpoint,
    load_into_model,
    read_config,
    save_checkpoint,
)
from elafry.train.distributed import (
    Linear4DConfig,
    all_reduce_mean,
    is_distributed,
    is_main_process,
    setup_distributed,
    wrap_fsdp,
)
from elafry.train.optim import (
    AmpContext,
    GradientAccumulator,
    build_optimizer,
    build_scheduler,
    lr_at_step,
)
from elafry.train.pgdpo import dpo_loss, sequence_logprob
from elafry.train.reward import (
    AffectiveRewardModel,
    empathy_target,
    reward_loss,
    sycophancy_penalty,
    validate_pair,
)
from elafry.train.sft import SFTResult, sft_step, train_sft

__all__ = [
    "AmpContext",
    "AffectiveRewardModel",
    "CONFIG_SIDECAR",
    "GradientAccumulator",
    "Linear4DConfig",
    "SFTResult",
    "TrainState",
    "all_reduce_mean",
    "build_optimizer",
    "build_scheduler",
    "checkpoint_payload",
    "dpo_loss",
    "empathy_target",
    "is_distributed",
    "is_main_process",
    "load_checkpoint",
    "load_into_model",
    "lr_at_step",
    "read_config",
    "reward_loss",
    "save_checkpoint",
    "sequence_logprob",
    "setup_distributed",
    "sft_step",
    "sycophancy_penalty",
    "train_sft",
    "validate_pair",
    "wrap_fsdp",
]