"""Distributed training scaffolding.

Everything here is written so it is *correct on one device first*. A distributed wrapper that
only runs under ``torchrun`` cannot be tested on a laptop, and distributed code that cannot
be tested is where the worst bugs live: a wrong ``rank`` swap produces a model that trains,
reports a falling loss, and is subtly broken.

So each function has a single-device path that is the default, and the sharded paths activate
only when the process group actually exists. ``tests/test_train.py`` exercises the
single-device path and asserts the sharded paths are *not* silently taken.
"""

from __future__ import annotations

import os
from dataclasses import dataclass
from typing import Dict, Optional

import torch
import torch.distributed as dist
from torch import nn

__all__ = [
    "dist_info",
    "is_distributed",
    "is_main_process",
    "barrier",
    "all_reduce_mean",
    "setup_distributed",
    "cleanup_distributed",
    "wrap_fsdp",
    "Linear4DConfig",
    "ACTIVATION_CHECKPOINT_DOC",
]


ACTIVATION_CHECKPOINT_DOC = """Activation checkpointing trades compute for memory.

Each block's activations are dropped during the forward pass and recomputed during the
backward pass. That roughly halves the activation memory at the cost of one extra forward
per block, and it is what makes a 1B model trainable on a 12GB card at a usable sequence
length.

In ``elafry`` the checkpointed region is the whole block, so the recomputation re-runs
attention including the affective bias. That is the right boundary: the bias depends on the
state, and recomputing it against a different state would be a silent correctness bug.
"""


@dataclass(frozen=True)
class Linear4DConfig:
    """1D parallelism degrees, for FSDP.

    ``dp`` is data parallelism, ``cp`` context parallelism, ``tp`` tensor parallelism. On a
    single consumer GPU all three are 1 and every sharded code path reduces to the
    unsharded one.
    """

    dp: int = 1
    cp: int = 1
    tp: int = 1

    def __post_init__(self) -> None:
        for name, value in (("dp", self.dp), ("cp", self.cp), ("tp", self.tp)):
            if value < 1:
                raise ValueError(f"{name} must be at least 1, got {value}")

    @property
    def mesh_size(self) -> int:
        return self.dp * self.cp * self.tp


def is_distributed() -> bool:
    return dist.is_available() and dist.is_initialized()


def dist_info() -> Dict[str, int]:
    if not is_distributed():
        return {"rank": 0, "world_size": 1, "local_rank": 0}
    return {
        "rank": dist.get_rank(),
        "world_size": dist.get_world_size(),
        "local_rank": int(os.environ.get("LOCAL_RANK", 0)),
    }


def is_main_process() -> bool:
    return dist_info()["rank"] == 0


def barrier() -> None:
    if is_distributed():
        dist.barrier()


def all_reduce_mean(value: torch.Tensor) -> torch.Tensor:
    """Mean across ranks. A no-op on one device, which is the common case in this repo."""
    if not is_distributed():
        return value
    tensor = value.detach().clone()
    dist.all_reduce(tensor, op=dist.ReduceOp.SUM)
    return tensor / dist.get_world_size()


def setup_distributed(backend: Optional[str] = None) -> Dict[str, int]:
    """Initialise the process group from torchrun's environment variables."""
    info = dist_info()
    if info["world_size"] > 1:
        backend = backend or ("nccl" if torch.cuda.is_available() else "gloo")
        if not dist.is_initialized():
            dist.init_process_group(backend=backend)
        return dist_info()

    # Single process. torchrun sets these even with nproc_per_node=1, and torch's defaults
    # expect them to be present.
    os.environ.setdefault("MASTER_ADDR", "127.0.0.1")
    os.environ.setdefault("MASTER_PORT", "29500")
    os.environ.setdefault("RANK", "0")
    os.environ.setdefault("WORLD_SIZE", "1")
    os.environ.setdefault("LOCAL_RANK", "0")
    return dist_info()


def cleanup_distributed() -> None:
    if is_distributed():
        dist.destroy_process_group()


def wrap_fsdp(
    model: nn.Module,
    config: Optional[Linear4DConfig] = None,
    device_type: str = "cuda",
    auto_wrap_policy=None,
) -> nn.Module:
    """Wrap in FSDP when distributed, otherwise return the model unchanged.

    The no-op path matters. It is what runs during development and in CI, and having it be a
    genuine no-op rather than a stripped-down FSDP means the single-device path and the
    distributed path exercise the same model code.
    """
    config = config or Linear4DConfig()
    if not is_distributed() or config.mesh_size == 1:
        return model

    from torch.distributed.fsdp import FullyShardedDataParallel as FSDP
    from torch.distributed.fsdp import ShardingStrategy

    policy = auto_wrap_policy
    if policy is None:
        from torch.distributed.fsdp.wrap import transformer_auto_wrap_policy

        from elafry.models.block import ElafryBlock

        policy = transformer_auto_wrap_policy({ElafryBlock})

    return FSDP(
        model,
        auto_wrap_policy=policy,
        device_id=torch.cuda.current_device() if device_type == "cuda" else None,
        sharding_strategy=ShardingStrategy.FULL_SHARD,
        use_orig_params=True,
    )


def enable_activation_checkpointing(model: nn.Module) -> int:
    """Turn on gradient checkpointing per block. Returns how many were enabled.

    Uses the module-level flag rather than wrapping each block, because wrapping loses the
    per-layer hooks the affective bias needs.
    """
    count = 0
    for layer in model.layers:
        if hasattr(layer, "gradient_checkpointing"):
            layer.gradient_checkpointing = True
            count += 1
        elif hasattr(layer, "forward"):
            layer.gradient_checkpointing = True
            count += 1
    return count