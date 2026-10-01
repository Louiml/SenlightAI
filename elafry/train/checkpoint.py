"""Checkpointing.

Exports to safetensors with a ``config.json`` sidecar written next to it. The sidecar is the
whole point of this file.

The engine in ``elafry_rs`` used to carry its model geometry as a hardcoded Rust struct. When
the checkpoint on disk did not match it, the engine exited at startup with no way to tell it
what to load, because there was no flag and no environment variable for the geometry. Every
silent failure mode of that design comes back if the config is not a file that travels with
the weights, so the exporter refuses to write a checkpoint without one.
"""

from __future__ import annotations

import json

import shutil
from pathlib import Path
from typing import Any, Dict, Optional

import torch
from torch import Tensor, nn

from elafry.config.base import AffectConfig, ModelConfig, TrainConfig

__all__ = [
    "CONFIG_SIDECAR",
    "checkpoint_payload",
    "save_checkpoint",
    "load_checkpoint",
    "read_config",
    "load_into_model",
    "TrainState",
]

CONFIG_SIDECAR = "config.json"
WEIGHTS_NAME = "model.safetensors"


def checkpoint_payload(model_cfg: ModelConfig, affect_cfg: AffectConfig, train_cfg: TrainConfig) -> Dict[str, Any]:
    """The sidecar contents. Includes a format version so a future change can be detected."""
    return {
        "format_version": 1,
        "model": model_cfg.to_dict(),
        "affect": affect_cfg.to_dict(),
        "train": train_cfg.to_dict(),
    }


def read_config(directory: Path) -> Dict[str, Any]:
    """Read the sidecar, with a message that names the fix rather than the symptom."""
    path = Path(directory) / CONFIG_SIDECAR
    if not path.exists():
        raise FileNotFoundError(
            f"{path} not found. The model geometry travels with the weights: export a "
            "checkpoint with elafry.export.safetensors.save_checkpoint, or pass the "
            "directory that contains config.json."
        )
    return json.loads(path.read_text(encoding="utf-8"))


class TrainState:
    """Mutable training position: step, tokens seen, wall clock.

    Written next to every checkpoint as ``train_state.json`` so a crashed campaign can be
    resumed without guessing. Token accounting has to be durable because token budget is a
    first-class stopping condition, and a resumed run that forgets how many tokens it has
    already burned will silently overrun.
    """

    def __init__(self, step: int = 0, tokens_seen: int = 0, started_at: Optional[float] = None):
        self.step = step
        self.tokens_seen = tokens_seen
        self.started_at = started_at

    def to_dict(self) -> Dict[str, Any]:
        import time

        return {
            "step": self.step,
            "tokens_seen": self.tokens_seen,
            "started_at": self.started_at if self.started_at is not None else time.time(),
            "saved_at": time.time(),
        }

    @classmethod
    def from_dict(cls, d: Dict[str, Any]) -> "TrainState":
        return cls(
            step=int(d.get("step", 0)),
            tokens_seen=int(d.get("tokens_seen", 0)),
            started_at=d.get("started_at"),
        )

    @property
    def elapsed_minutes(self) -> float:
        import time

        if self.started_at is None:
            return 0.0
        return (time.time() - self.started_at) / 60.0


def _strip_tied(state: Dict[str, Tensor], model: nn.Module) -> Dict[str, Tensor]:
    """Drop the duplicate tied embedding and the derived RoPE tables.

    ``lm_head.weight`` is the same tensor as ``embed_tokens.weight`` when tying is on. Saving
    both doubles the largest array in the file for no benefit, and safetensors will complain
    about shared storage if they are passed as-is.
    """
    out = {}
    lm = getattr(model, "lm_head", None)
    # Compare storage pointers, not object identity. state_dict() detaches, so ``lm_head.weight``
    # comes back as a different Python object wrapping the same storage, and an ``id()``
    # comparison silently fails to spot the tie and writes the largest array in the file
    # twice.
    lm_ptr = lm.weight.data_ptr() if lm is not None and hasattr(lm, "weight") else None

    for key, tensor in state.items():
        # cos/sin/inv_freq are rebuilt from (head_dim, max_seq_len, theta) at load time.
        if key.endswith((".cos", ".sin", ".inv_freq")) or key in ("cos", "sin", "inv_freq"):
            continue
        if key == "lm_head.weight" and lm_ptr is not None and tensor.data_ptr() == lm_ptr:
            continue
        out[key] = tensor.detach().clone().contiguous()
    return out


def save_checkpoint(
    directory: Path,
    model: nn.Module,
    model_cfg: ModelConfig,
    affect_cfg: AffectConfig,
    train_cfg: Optional[TrainConfig] = None,
    state: Optional[TrainState] = None,
    optimizer: Optional[torch.optim.Optimizer] = None,
    keep_last: int = 2,
) -> Path:
    """Write ``model.safetensors``, ``config.json`` and (optionally) the optimizer state."""
    from safetensors.torch import save_file

    directory = Path(directory)
    directory.mkdir(parents=True, exist_ok=True)

    weights = _strip_tied(model.state_dict(), model)
    save_file(weights, str(directory / WEIGHTS_NAME))

    payload = checkpoint_payload(model_cfg, affect_cfg, train_cfg or TrainConfig())
    if optimizer is not None and state is not None:
        payload["optimizer"] = {
            "step": state.step,
            "tokens_seen": state.tokens_seen,
        }
    (directory / CONFIG_SIDECAR).write_text(json.dumps(payload, indent=2), encoding="utf-8")

    if state is not None:
        (directory / "train_state.json").write_text(
            json.dumps(state.to_dict(), indent=2), encoding="utf-8"
        )

    if optimizer is not None:
        # A .pt because the optimizer state is Python objects, not tensors. It is a
        # resume-only artifact and never shipped.
        torch.save(
            {"optimizer": optimizer.state_dict(), "state": state.to_dict()},
            str(directory / "optimizer.pt"),
        )

    _prune(directory.parent, keep_last)
    return directory


def _prune(root: Path, keep_last: int) -> None:
    """Keep the newest ``keep_last`` numbered checkpoints under ``root``.

    Pruning looks at the *parent*, since the numbered directories are siblings of the one
    being written. Globbing inside the new directory finds nothing and silently keeps every
    checkpoint forever.
    """
    if keep_last <= 0:
        return
    numbered = sorted(
        (p for p in root.glob("step*") if p.is_dir() and p.name[4:].isdigit()),
        key=lambda p: int(p.name[4:]),
    )
    for stale in numbered[:-keep_last]:
        shutil.rmtree(stale, ignore_errors=True)


def load_checkpoint(directory: Path, device="cpu") -> Dict[str, Any]:
    """Read a checkpoint directory into ``{"config", "weights", "state"}``."""
    from safetensors.torch import load_file

    directory = Path(directory)
    config = read_config(directory)
    weights_path = directory / WEIGHTS_NAME
    if not weights_path.exists():
        raise FileNotFoundError(f"{weights_path} not found")

    weights = load_file(str(weights_path), device=str(device))

    state = None
    state_path = directory / "train_state.json"
    if state_path.exists():
        state = TrainState.from_dict(json.loads(state_path.read_text(encoding="utf-8")))
    return {"config": config, "weights": weights, "state": state}


def load_into_model(model: nn.Module, weights: Dict[str, Tensor], strict: bool = True):
    """Load weights into ``model``, re-tying if the config says so.

    ``strict=False`` is permitted for a fresh model whose RoPE tables and tied head are
    absent from the file, which is expected rather than a mismatch.
    """
    result = model.load_state_dict(weights, strict=strict)
    tied = getattr(model, "lm_head", None)
    if tied is not None and hasattr(model, "embed_tokens") and model.cfg.tie_embeddings:
        tied.weight = model.embed_tokens.weight
    return result