"""Shared fixtures.

Every test runs on CPU in float32. The interesting numerical properties here (logit
parity between prefill and cached decode, bias injection actually changing the output) hold
at any precision, and testing in float32 means a failure means a real bug rather than an
accumulated rounding difference.
"""

from __future__ import annotations

import pytest
import torch

from elafry.config import PRESETS, AffectConfig
from elafry.models.elafry import Elafry


def make_tiny_model(seed: int = 0, **affect_kwargs) -> Elafry:
    """A real Elafry, small enough that the whole suite runs in seconds."""
    torch.manual_seed(seed)
    preset = PRESETS["elafry-tiny"]
    affect = AffectConfig(**affect_kwargs)
    return Elafry(preset["model"], affect)


@pytest.fixture
def tiny_model() -> Elafry:
    return make_tiny_model(seed=0)


@pytest.fixture
def tiny_ids() -> torch.Tensor:
    torch.manual_seed(1234)
    return torch.randint(0, PRESETS["elafry-tiny"]["model"].vocab_size, (2, 12))


@pytest.fixture(autouse=True)
def deterministic():
    """Pin the seed around every test so a failure is reproducible."""
    torch.manual_seed(0)
    yield