"""Training for Varys.

    reward.py   frozen backbone, Bradley-Terry plus four auxiliary heads
    pgdpo.py    reward-weighted, reference-anchored policy gradient
    sft.py      supervised fine-tuning with per-example state stepping

## The order these run in

SFT first, then the reward model, then PG-DPO. Each stage exists because the previous one
produced something the next one needs: SFT produces a model whose representations mean
something, the reward model is fitted on top of those, and PG-DPO uses the reward to move
the policy while a frozen reference holds the line.

## What is deliberately absent

No data pipeline, no dataset, no tokenizer training, no distributed launcher, and no
checkpoint format beyond what :mod:`varys.models` already implies. The spec's data mix is
45% psychological reasoning, 35% world knowledge, 20% base conversation, and choosing sources
for that is a research decision with an author attached to it. Shipping an empty loader would
mean shipping a training loop that cannot be run and calling it infrastructure.

See ``README.md`` for what a Varys data mix has to contain and ``SCALING.md`` for what the
72B campaign needs.
"""

from varys.train.pgdpo import (
    PGDPOOutput,
    check_state_consistency,
    pgdpo_loss,
    pgdpo_update,
    sequence_logprobs,
)
from varys.train.reward import (
    AffectiveRewardModel,
    RewardOutput,
    clinical_assertion_penalty,
    empathy_target,
    exploitation_penalty,
    reward_loss,
    sycophancy_penalty,
    validate_pair,
)
from varys.train.sft import (
    SFTOutput,
    build_affect_optimizer_groups,
    response_mask,
    sft_loss,
    sft_step,
)

__all__ = [
    # reward
    "AffectiveRewardModel", "RewardOutput", "reward_loss", "empathy_target",
    "sycophancy_penalty", "exploitation_penalty", "clinical_assertion_penalty",
    "validate_pair",
    # pg-dpo
    "PGDPOOutput", "pgdpo_loss", "pgdpo_update", "sequence_logprobs",
    "check_state_consistency",
    # sft
    "SFTOutput", "sft_loss", "sft_step", "response_mask", "build_affect_optimizer_groups",
]
