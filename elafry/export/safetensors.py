"""Export to safetensors with a config.json sidecar.

Written as a thin wrapper around :mod:`elafry.train.checkpoint` rather than a second
implementation, because two exporters will drift and the drift will be invisible until an
inference engine refuses to load a file.

The naming scheme follows the sibling implementation in the repo: tensors are stored under
``model.*`` with LLaMA-style keys, so a checkpoint from either codebase is readable by the
other. The difference is that Elafry ships ``config.json`` next to it.
"""

from __future__ import annotations


from pathlib import Path
from typing import Dict, Optional, Tuple

import torch
from torch import nn

from elafry.config.base import AffectConfig, ModelConfig, TrainConfig
from elafry.train.checkpoint import (
    CONFIG_SIDECAR,
load_checkpoint,
    load_into_model,
    save_checkpoint,
)


__all__ = [
    "export",
    "export_pretrained",
    "load_pretrained",
    "KEY_MAP",
    "to_hf_keys",
    "from_hf_keys",
    "CONFIG_SIDECAR",
]


# Elafry module path -> the LLaMA-style key the safetensors file uses. The SwiGLU rename is
# the one that matters: ``mlp.gate`` in the module becomes ``mlp.gate_proj`` on disk, which is
# what every other Llama-derived checkpoint looks like and what makes the two codebases'
# files interchangeable.
KEY_MAP = {
    "embed_tokens": "embed_tokens",
    "norm": "norm",
    "layers.{i}.input_layernorm": "layers.{i}.input_layernorm",
    "layers.{i}.self_attn.q_proj": "layers.{i}.self_attn.q_proj",
    "layers.{i}.self_attn.k_proj": "layers.{i}.self_attn.k_proj",
    "layers.{i}.self_attn.v_proj": "layers.{i}.self_attn.v_proj",
    "layers.{i}.self_attn.o_proj": "layers.{i}.self_attn.o_proj",
    "layers.{i}.post_attention_layernorm": "layers.{i}.post_attention_layernorm",
    "layers.{i}.mlp.gate": "layers.{i}.mlp.gate_proj",
    "layers.{i}.mlp.up": "layers.{i}.mlp.up_proj",
    "layers.{i}.mlp.down": "layers.{i}.mlp.down_proj",
    # The affective subsystems keep their module names. They have no LLaMA equivalent, and
    # inventing one would make them look like something they are not.
    "layers.{i}.self_attn.affect.state_proj": "layers.{i}.self_attn.affect.state_proj",
    "internal_state": "internal_state",
    "router.classifier": "router.classifier",
    "router.crisis_head": "router.crisis_head",
    "state_encoder": "state_encoder",
}


# Tensor-name suffixes that are not part of the module path. `layers.0.mlp.gate.weight` is the
# module path `layers.0.mlp.gate` plus `.weight`, and the mapping has to be applied to the
# module path, not to the whole key.
_PARAM_SUFFIXES = (".weight", ".bias")


def _split_param(key: str) -> Tuple[str, str]:
    for suffix in _PARAM_SUFFIXES:
        if key.endswith(suffix):
            return key[: -len(suffix)], suffix
    return key, ""


def _map_key(key: str) -> str:
    """Module key -> safetensors key."""
    path, suffix = _split_param(key)

    for src, dst in KEY_MAP.items():
        if "{i}" in src:
            sprefix, ssuffix = src.split("{i}")
            dprefix, dsuffix = dst.split("{i}")
            if path.startswith(sprefix) and path.endswith(ssuffix):
                index = path[len(sprefix) : len(path) - len(ssuffix) or None]
                if index.isdigit():
                    return f"{dprefix}{index}{dsuffix}{suffix}"
        elif path == src:
            return f"{dst}{suffix}"

    return key


def _unmap_key(key: str) -> str:
    """safetensors key -> module key. The exact inverse of :func:`_map_key`."""
    path, suffix = _split_param(key)

    for src, dst in KEY_MAP.items():
        if "{i}" in dst:
            dprefix, dsuffix = dst.split("{i}")
            sprefix, ssuffix = src.split("{i}")
            if path.startswith(dprefix) and path.endswith(dsuffix):
                index = path[len(dprefix) : len(path) - len(dsuffix) or None]
                if index.isdigit():
                    return f"{sprefix}{index}{ssuffix}{suffix}"
        elif path == dst:
            return f"{src}{suffix}"

    return key


def to_hf_keys(state: Dict[str, torch.Tensor], prefix: str = "") -> Dict[str, torch.Tensor]:
    """Module keys -> safetensors keys."""
    return {_map_key(k): v for k, v in state.items()}


def from_hf_keys(state: Dict[str, torch.Tensor]) -> Dict[str, torch.Tensor]:
    """safetensors keys -> module keys."""
    return {_unmap_key(k): v for k, v in state.items()}


def export(
    model: nn.Module,
    model_cfg: ModelConfig,
    affect_cfg: AffectConfig,
    output_dir: Path,
    train_cfg: Optional[TrainConfig] = None,
    prefix: str = "model.",
) -> Path:
    """Write ``model.safetensors`` and ``config.json`` into ``output_dir``.

    The sidecar is not optional. :func:`save_checkpoint` writes it, and this function routes
    through it precisely so that no code path can produce a weights file without geometry
    attached.
    """
    output_dir = Path(output_dir)
    directory = save_checkpoint(
        output_dir, model, model_cfg, affect_cfg, train_cfg or TrainConfig()
    )

    # save_checkpoint writes the module's own key names, which is right for a resume but
    # wrong for an artifact: the Rust engine and every other Llama-derived loader expect the
    # _proj suffix on the SwiGLU matrices. Rewrite the file in place.
    if prefix == "model.":
        _rewrite_with_hf_keys(directory)

    return directory


def _rewrite_with_hf_keys(directory: Path) -> None:
    """Rewrite model.safetensors with the mapped key names.

    Done as a post-pass rather than by teaching ``save_checkpoint`` about mapping, because
    checkpointing wants the module's own names (so a resume loads with ``strict=True``) and
    exporting wants portable ones. Keeping them separate means neither has to know about the
    other.
    """
    import os

    from safetensors.torch import load_file, save_file

    path = directory / "model.safetensors"
    tensors = load_file(str(path))

    # Write beside the original and move it into place. Overwriting in place fails on
    # Windows, where the file is still memory-mapped for as long as a tensor referencing it
    # is alive.
    tmp = path.with_suffix(".safetensors.tmp")
    save_file(to_hf_keys(tensors), str(tmp))
    os.replace(str(tmp), str(path))


def export_pretrained(
    model: nn.Module,
    model_cfg: ModelConfig,
    affect_cfg: AffectConfig,
    output_dir: Path,
    tokenizer_path: Optional[Path] = None,
    train_cfg: Optional[TrainConfig] = None,
) -> Path:
    """Export, and optionally record the tokenizer next to it.

    Copying the tokenizer in is a convenience with a real payoff: the inference crate needs
    both files, and a directory that contains both is a directory that works.
    """
    output_dir = Path(output_dir)
    directory = export(model, model_cfg, affect_cfg, output_dir, train_cfg)

    if tokenizer_path is not None and Path(tokenizer_path).exists():
        shutil = __import__("shutil")
        shutil.copyfile(tokenizer_path, directory / "tokenizer.json")

    return directory


def load_pretrained(
    directory: Path,
    device: str = "cpu",
    dtype: Optional[torch.dtype] = None,
    device_map: str = "cpu",
):
    """Rebuild a model from a checkpoint directory.

    The geometry comes from the sidecar, so there is nothing to pass in and nothing to keep
    in sync. This is the whole reason the sidecar exists.
    """
    from elafry.models.elafry import Elafry

    payload = load_checkpoint(directory, device=device)
    model_cfg = ModelConfig.from_dict(payload["config"]["model"])
    affect_cfg = AffectConfig.from_dict(payload["config"]["affect"])

    model = Elafry(model_cfg, affect_cfg)
    load_into_model(model, payload["weights"], strict=False)

    if dtype is not None:
        model = model.to(dtype)
    if device != "cpu":
        model = model.to(device)

    return model, model_cfg, affect_cfg