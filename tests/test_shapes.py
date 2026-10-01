"""Forward and backward shape checks across the preset ladder.

Small and cheap enough to run on CPU in float32. Each preset is instantiated at a short
sequence length, so this covers every rung of the ladder including 8B's geometry without
allocating 32GB.
"""

from __future__ import annotations

import pytest
import torch

from elafry.affective.state import initial_state
from elafry.config import AffectConfig, ModelConfig, get_preset, list_presets
from elafry.models.elafry import Elafry


def _short_cfg(cfg: ModelConfig, budget: float = 8e6) -> ModelConfig:
    """A faithful stand-in for a large preset at a size CPU can actually hold.

    What is preserved: ``n_layers``, ``n_heads``, ``n_kv_heads``, ``head_dim`` and
    ``rope_theta``. Those are where architecture bugs live. A mask applied only to the final
    layer, a GQA group count that does not divide, a depth-dependent bug, none of those can
    hide behind a smaller width.

    What is shrunk: ``vocab_size``, ``dim`` and ``ff_dim``, which scale the parameter count
    without changing the shape of the computation. Note that ``n_heads * head_dim`` is
    allowed to exceed ``dim`` here, which is why shrinking ``dim`` alone is enough.
    """
    scale = min(1.0, (budget / max(cfg.vocab_size * cfg.dim, 1)) ** 0.5)
    # Round dim up to a whole number of query heads; the config validator insists on it.
    dim = max(cfg.n_heads, int(cfg.dim * scale))
    dim += (-dim) % cfg.n_heads
    ff = max(cfg.head_dim * 4, int(cfg.ff_dim * scale))
    return ModelConfig.from_dict(
        {
            **cfg.to_dict(),
            "vocab_size": max(cfg.n_kv_heads, int(cfg.vocab_size * scale)),
            "dim": dim,
            "ff_dim": ff,
            "max_seq_len": 64,
        }
    )


@pytest.mark.parametrize("name", list_presets())
def test_forward_and_backward_at_every_preset(name):
    """Every rung of the ladder, including 8B's depth and head geometry, at a size CPU can
    hold. Depth and head structure are where implementation bugs live; width is not."""
    preset = get_preset(name)
    cfg = _short_cfg(preset["model"])
    torch.manual_seed(0)
    model = Elafry(cfg, preset["affect"])

    bsz, seq = 2, 8
    ids = torch.randint(0, cfg.vocab_size, (bsz, seq))
    out = model(ids)

    assert out.logits.shape == (bsz, seq, cfg.vocab_size)
    assert out.state.shape == (bsz, 3)
    assert out.next_token_logits.shape == (bsz, cfg.vocab_size)
    assert torch.isfinite(out.logits).all(), "logits contain NaN or inf"

    out.logits.float().mean().backward()
    grads = [p.grad for p in model.parameters() if p.requires_grad and p.grad is not None]
    assert grads, "no parameter received a gradient"
    assert any(g.abs().sum().item() > 0 for g in grads), "every gradient is exactly zero"


@pytest.mark.parametrize("name", list_presets())
def test_short_cfg_preserves_the_structure_that_matters(name):
    """Guards the guard. If _short_cfg quietly changed depth or GQA grouping, the test above
    would be checking a different model than the preset describes."""
    preset = get_preset(name)
    cfg = _short_cfg(preset["model"])
    assert cfg.n_layers == preset["model"].n_layers, "depth was changed"
    assert cfg.n_heads == preset["model"].n_heads, "query head count was changed"
    assert cfg.n_kv_heads == preset["model"].n_kv_heads, "KV head count was changed"
    assert cfg.n_rep == preset["model"].n_rep, "GQA grouping was changed"
    assert cfg.head_dim == preset["model"].head_dim, "head width was changed"
    assert cfg.rope_theta == preset["model"].rope_theta, "RoPE base was changed"


def test_backward_propagates_to_every_layer():
    """A dead layer looks identical from the outside. This walks them."""
    torch.manual_seed(0)
    preset = get_preset("elafry-tiny")
    model = Elafry(preset["model"], preset["affect"])
    ids = torch.randint(0, preset["model"].vocab_size, (2, 8))
    model(ids).logits.float().mean().backward()

    for i, layer in enumerate(model.layers):
        for name, p in layer.named_parameters():
            assert p.grad is not None, f"layer {i} {name} got no gradient"
            assert torch.isfinite(p.grad).all(), f"layer {i} {name} has a non-finite gradient"


def test_padding_does_not_change_unpadded_logits():
    """Right-padding must be a no-op for the real tokens. If it is not, batch composition is
    leaking into the prediction, which for this model means leaking into the affective state.
    """
    torch.manual_seed(0)
    preset = get_preset("elafry-tiny")
    model = Elafry(preset["model"], preset["affect"])
    model.eval()

    seq = 8
    ids = torch.randint(1, preset["model"].vocab_size, (1, seq))
    pad_id = 0

    padded = torch.cat([ids, torch.full((1, 3), pad_id)], dim=1)
    mask = torch.cat([torch.ones(1, seq, dtype=torch.long), torch.zeros(1, 3, dtype=torch.long)], dim=1)

    with torch.no_grad():
        plain = model(ids)
        with_pad = model(padded, attention_mask=mask)

    diff = (plain.logits - with_pad.logits[:, :seq]).abs().max().item()
    assert diff < 1e-4, f"padding moved the logits by {diff:.2e}"


def test_hidden_states_bypass_lm_head():
    """The reward model and any critic need a representation, not a 128k-way distribution."""
    torch.manual_seed(0)
    preset = get_preset("elafry-tiny")
    model = Elafry(preset["model"], preset["affect"])
    model.eval()

    ids = torch.randint(0, preset["model"].vocab_size, (2, 8))
    with torch.no_grad():
        hidden = model.get_hidden_states(ids)
        logits = model(ids).logits
    assert hidden.shape == (2, 8, preset["model"].dim)
    # The output projection is what makes logits much larger in magnitude than the hidden
    # state, so this is a real distinction rather than an alias.
    assert hidden.shape[-1] != logits.shape[-1]


def test_externally_supplied_state_overrides_the_derived_one():
    """The external path is the control condition, so it has to actually override."""
    torch.manual_seed(0)
    preset = get_preset("elafry-tiny")
    model = Elafry(preset["model"], preset["affect"])
    model.eval()
    ids = torch.randint(0, preset["model"].vocab_size, (1, 8))

    supplied = torch.tensor([[-0.9, 0.9, 0.9]])
    with torch.no_grad():
        out = model(ids, state=supplied)
    assert torch.allclose(out.state, supplied)


def test_single_state_broadcasts_across_the_batch():
    torch.manual_seed(0)
    preset = get_preset("elafry-tiny")
    model = Elafry(preset["model"], preset["affect"])
    model.eval()
    ids = torch.randint(0, preset["model"].vocab_size, (3, 8))

    with torch.no_grad():
        out = model(ids, state=torch.tensor([0.5, 0.5, 0.5]))
    assert out.state.shape == (3, 3)
    assert torch.allclose(out.state, torch.full((3, 3), 0.5))


def test_state_stays_inside_the_cube_across_many_steps():
    """A long conversation must not be able to walk the state out of range."""
    torch.manual_seed(0)
    preset = get_preset("elafry-tiny")
    model = Elafry(preset["model"], preset["affect"])
    ids = torch.randint(0, preset["model"].vocab_size, (1, 8))

    state = initial_state(1)
    for _ in range(50):
        with torch.no_grad():
            out = model(ids, state=state)
        state = out.state
        assert state.abs().max().item() <= preset["affect"].clamp + 1e-6


def test_disabled_model_still_trains():
    """With the affective subsystem off this should be an ordinary transformer. If it does
    not train, the control condition is unusable as a baseline."""
    torch.manual_seed(0)
    preset = get_preset("elafry-tiny")
    model = Elafry(preset["model"], AffectConfig(enabled=False))
    ids = torch.randint(0, preset["model"].vocab_size, (2, 8))

    out = model(ids)
    out.logits.float().mean().backward()
    grads = [p.grad for p in model.parameters() if p.grad is not None]
    assert grads
    assert any(g.abs().sum().item() > 0 for g in grads)


def test_per_token_state_is_explicitly_unimplemented():
    """Rather than silently doing something different from what the caller asked for."""
    torch.manual_seed(0)
    preset = get_preset("elafry-tiny")
    model = Elafry(preset["model"], preset["affect"])
    ids = torch.randint(0, preset["model"].vocab_size, (1, 8))
    with pytest.raises(NotImplementedError, match="per-token state recurrence"):
        model(ids, state_per_token=True)


def test_all_presets_agree_on_max_seq_len_versus_generation_budget():
    """generate() reserves room for what it is about to generate. A preset whose context is
    smaller than the default generation budget would truncate to nothing."""
    for name in list_presets():
        cfg = get_preset(name)["model"]
        assert cfg.max_seq_len >= 128, name
