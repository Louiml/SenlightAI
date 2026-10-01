"""Does the affective bias actually do anything?

The architecture note's claim is that emotion reaches token probabilities through the
attention bias. It is entirely possible to implement that formula and have it do nothing:
zero-initialised projections, a state that never leaves the origin, a bias applied to the
wrong tensor. Every one of those produces a model that runs, trains, and is emotionally
identical to a plain transformer.

So this file measures the mechanism rather than asserting it works.

## The identity trap

Elafry zero-initialises ``o_proj`` and ``mlp.down`` so every block starts as the identity,
which is what lets a deep stack train from step 1. The consequence, which this file has to
route around, is that a freshly initialised model computes

    h = embed(x)        # attention and MLP both contribute exactly zero
    logits = h @ embed.T

so its output depends on the current token alone and is completely insensitive to the
affective state. ``_wake_residuals`` puts a random model back underneath before anything is
measured. Testing the bias on an untouched model would "pass" for the wrong reason: the
bias is present and irrelevant, because nothing else in the network is present either.

Once the residuals are live, the two properties that matter are checked separately:

* at initialisation the bias is exactly zero, so the model is vanilla attention
* with a trained-style bias, changing the state must move the logits
"""

from __future__ import annotations

import pytest
import torch

from elafry.affective.state import all_primary_states
from elafry.config import AffectConfig, PRESETS
from elafry.models.attention import AffectiveBias
from elafry.models.elafry import Elafry

from conftest import make_tiny_model


def _wake_residuals(model: Elafry, seed: int = 0) -> Elafry:
    """Replace every zero-initialised projection with a small random one.

    Gives a random model, not a trained one. That is deliberate: it isolates whether the
    wiring is correct, which is the only question at this stage.
    """
    gen = torch.Generator().manual_seed(seed)
    with torch.no_grad():
        for layer in model.layers:
            layer.self_attn.o_proj.weight.normal_(0.0, 0.05, generator=gen)
            layer.mlp.down.weight.normal_(0.0, 0.05, generator=gen)
    return model


def _wake_bias(model: Elafry, scale: float = 0.3, seed: int = 0) -> Elafry:
    """Give the affective projections non-zero weights.

    ``Elafry._init_weights`` sweeps every 2D parameter, which overwrites the zero-init on
    ``AffectiveBias.state_proj``. That is fine at training time (the sweep runs once, and
    training moves the weights anyway) but it means a constructed model has no bias to
    measure. This restores one in the shape training would produce.
    """
    gen = torch.Generator().manual_seed(seed)
    with torch.no_grad():
        for layer in model.layers:
            # Layers excluded by inject_layers have no bias module at all.
            if layer.self_attn.affect is None:
                continue
            layer.self_attn.affect.state_proj.weight.normal_(0.0, scale, generator=gen)
    return model


def _live_model(seed: int = 0, bias_seed: int = 100, **affect_kwargs) -> Elafry:
    """A model with a live residual path and a live affective bias."""
    model = make_tiny_model(seed=seed, **affect_kwargs)
    _wake_residuals(model, seed=seed)
    _wake_bias(model, seed=bias_seed)
    model.eval()
    return model


def test_identity_init_makes_the_model_insensitive_to_state():
    """Documenting the trap from the module docstring. If this ever stops being true, the
    zero-init contract has been broken and the residual path needs re-examining."""
    model = make_tiny_model(seed=0)
    model.eval()
    ids = torch.randint(0, model.cfg.vocab_size, (2, 8))

    with torch.no_grad():
        a = model(ids, state=torch.tensor([[-0.9, 0.9, 0.5]])).logits
        b = model(ids, state=torch.tensor([[0.9, -0.9, -0.5]])).logits
    assert (a - b).abs().max().item() == 0.0


def test_bias_is_zero_before_the_model_init_sweep():
    """Constructed in isolation, ``AffectiveBias`` produces exactly zero. This is what makes
    the mechanism safe to leave switched on."""
    bias = AffectiveBias(3, n_heads=4, n_kv_heads=2, head_dim=8)
    out = bias(torch.randn(2, 3))
    assert torch.equal(out, torch.zeros_like(out))


def test_changing_the_state_changes_the_logits():
    """The load-bearing test. Two different states, measurably different predictions."""
    model = _live_model(seed=1)
    ids = torch.randint(0, model.cfg.vocab_size, (2, 10))

    with torch.no_grad():
        a = model(ids, state=torch.tensor([[-0.65, 0.75, 0.60]])).logits[:, -1, :]
        b = model(ids, state=torch.tensor([[0.85, -0.20, 0.35]])).logits[:, -1, :]

    diff = (a - b).abs().max().item()
    assert diff > 1e-4, (
        f"state barely moved the logits (max delta {diff:.2e}); the bias is either not wired "
        "up or the state is being discarded before it reaches attention"
    )


def test_every_plutchik_state_produces_a_distinct_distribution():
    """All eight primaries, pairwise. If two emotions collapse to the same distribution the
    model cannot tell them apart, which is the one thing this mechanism exists to prevent."""
    model = _live_model(seed=2)
    ids = torch.randint(0, model.cfg.vocab_size, (1, 12))
    states = all_primary_states()
    names = list(states)

    with torch.no_grad():
        logits = torch.stack(
            [model(ids, state=states[n]).logits[0, -1, :] for n in names], dim=0
        )  # (8, vocab)

    centred = logits - logits.mean(dim=-1, keepdim=True)
    normed = centred / centred.norm(dim=-1, keepdim=True)
    sims = normed @ normed.transpose(0, 1)

    offenders = [
        (names[i], names[j], float(sims[i, j]))
        for i in range(len(names))
        for j in range(i + 1, len(names))
        if float(sims[i, j]) > 0.999
    ]
    assert not offenders, f"these states produce near-identical distributions: {offenders}"


def test_adjacent_emotions_are_more_similar_than_opposite_ones():
    """A sanity check on the mechanism rather than on Plutchik. Joy and trust sit next to
    each other on the wheel; joy and sadness do not. The bias should reflect that."""
    model = _live_model(seed=3)
    ids = torch.randint(0, model.cfg.vocab_size, (1, 10))
    states = all_primary_states()

    def centred(name: str) -> torch.Tensor:
        with torch.no_grad():
            lg = model(ids, state=states[name]).logits[0, -1, :]
        c = lg - lg.mean()
        return c / c.norm()

    adjacent = float(torch.dot(centred("joy"), centred("trust")))
    opposite = float(torch.dot(centred("joy"), centred("sadness")))

    assert adjacent > opposite, (
        f"joy/trust ({adjacent:.4f}) should be more similar than joy/sadness "
        f"({opposite:.4f}); the bias is not tracking the VAD geometry"
    )


def test_dominance_separates_anger_from_fear():
    """The architecture note leans on this. Anger and fear share negative valence and high
    arousal, and differ mainly in dominance: one dominates, the other submits. A mechanism
    that only encoded valence and arousal could not tell them apart."""

    model = _live_model(seed=4)
    ids = torch.randint(0, model.cfg.vocab_size, (1, 10))

    with torch.no_grad():
        anger = model(ids, state=torch.tensor([[-0.65, 0.75, 0.60]])).logits[:, -1, :]
        fear = model(ids, state=torch.tensor([[-0.70, 0.65, -0.65]])).logits[:, -1, :]
        # Same valence and arousal, flipped dominance.
        anger_mirror = model(ids, state=torch.tensor([[-0.65, 0.75, -0.60]])).logits[:, -1, :]

    assert (anger - fear).abs().max().item() > 1e-4
    assert (anger - anger_mirror).abs().max().item() > 1e-4, (
        "flipping dominance alone must change the output, otherwise dominance is not being "
        "read"
    )


def test_inject_layers_restricts_which_layers_carry_the_bias():
    model = _live_model(seed=5, inject_layers=(-1,))
    injecting = [i for i, layer in enumerate(model.layers) if layer.inject_affect]
    assert injecting == [len(model.layers) - 1]

    ids = torch.randint(0, model.cfg.vocab_size, (1, 8))
    with torch.no_grad():
        a = model(ids, state=torch.tensor([[0.9, 0.9, 0.9]])).logits
        b = model(ids, state=torch.tensor([[-0.9, -0.9, -0.9]])).logits
    assert (a - b).abs().max().item() > 1e-5


def test_disabled_affect_is_exactly_vanilla():
    """With the subsystem off, state must have no effect at all. Anything else means the
    bias reaches attention through a path the config does not control."""
    torch.manual_seed(0)
    model = Elafry(PRESETS["elafry-tiny"]["model"], AffectConfig(enabled=False))
    model.eval()
    assert all(layer.self_attn.affect is None for layer in model.layers)

    ids = torch.randint(0, model.cfg.vocab_size, (2, 8))
    with torch.no_grad():
        a = model(ids, state=torch.tensor([[0.9, 0.9, 0.9]])).logits
        b = model(ids, state=torch.tensor([[-0.9, -0.9, -0.9]])).logits
    assert torch.equal(a, b), "state should have no effect when the subsystem is disabled"


@pytest.mark.parametrize("granularity", ["head", "kv"])
def test_both_granularities_move_the_logits(granularity: str):
    model = _live_model(seed=6, bias_granularity=granularity)
    ids = torch.randint(0, model.cfg.vocab_size, (1, 8))
    with torch.no_grad():
        a = model(ids, state=torch.tensor([[0.8, -0.8, 0.5]])).logits
        b = model(ids, state=torch.tensor([[-0.8, 0.8, -0.5]])).logits
    assert (a - b).abs().max().item() > 1e-5


def test_kv_granularity_is_cheaper_than_head():
    head = _live_model(seed=7, bias_granularity="head")
    kv = _live_model(seed=7, bias_granularity="kv")

    head_params = sum(p.numel() for p in head.parameters())
    kv_params = sum(p.numel() for p in kv.parameters())
    assert kv_params < head_params

    # The saving is the n_rep-1 duplicate bias matrices per layer.
    cfg = head.cfg
    n_bias_head = cfg.n_heads * cfg.head_dim * cfg.head_dim * head.state_dim
    n_bias_kv = cfg.n_kv_heads * cfg.head_dim * cfg.head_dim * kv.state_dim
    expected = cfg.n_layers * (n_bias_head - n_bias_kv)
    assert head_params - kv_params == expected


def test_per_kv_bias_repeats_within_groups():
    """The ``kv`` granularity has to expand in the same order the keys and values do, or
    query heads get paired with the wrong group's bias."""
    bias = AffectiveBias(3, n_heads=8, n_kv_heads=2, head_dim=4, granularity="kv")
    torch.nn.init.normal_(bias.state_proj.weight, std=0.5)

    state = torch.randn(1, 3)
    raw = bias(state)
    expanded = bias.bias_for_heads(state)

    assert expanded.shape == (1, 8, 4, 4)
    # 2 groups of 4 query heads: heads 0-3 share group 0, heads 4-7 share group 1.
    for head in range(4):
        assert torch.equal(expanded[0, head], expanded[0, 0])
        assert torch.equal(expanded[0, head + 4], expanded[0, 4])
    assert torch.equal(raw[0, 0], expanded[0, 0])
    assert torch.equal(raw[0, 1], expanded[0, 4])
    assert not torch.equal(expanded[0, 0], expanded[0, 4])


def test_per_head_granularity_gives_every_head_its_own_bias():
    bias = AffectiveBias(3, n_heads=4, n_kv_heads=2, head_dim=4, granularity="head")
    torch.nn.init.normal_(bias.state_proj.weight, std=0.5)

    expanded = bias.bias_for_heads(torch.randn(1, 3))
    assert expanded.shape == (1, 4, 4, 4)
    for a in range(4):
        for b in range(a + 1, 4):
            assert not torch.equal(expanded[0, a], expanded[0, b])


def test_bias_rejects_a_mismatched_state_width():
    bias = AffectiveBias(3, n_heads=4, n_kv_heads=2, head_dim=8)
    with pytest.raises(ValueError, match="does not match state_dim"):
        bias(torch.randn(1, 5))


def test_bias_rejects_unknown_granularity():
    with pytest.raises(ValueError, match="must be 'head' or 'kv'"):
        AffectiveBias(3, n_heads=4, n_kv_heads=2, head_dim=8, granularity="nonsense")


def test_gradients_reach_the_affective_projection():
    """The bias is zero at init, so it is easy to write an implementation where the gradient
    is also zero and the mechanism silently never trains. This is the check that it is not.
    """
    model = _live_model(seed=8)
    ids = torch.randint(0, model.cfg.vocab_size, (2, 8))

    out = model(ids)
    loss = out.logits.float().pow(2).mean()
    loss.backward()

    for i, layer in enumerate(model.layers):
        grad = layer.self_attn.affect.state_proj.weight.grad
        assert grad is not None, f"layer {i} affective projection received no gradient"
        assert grad.abs().sum().item() > 0, f"layer {i} affective gradient is exactly zero"


def test_gradients_reach_the_internal_state_predictor():
    """Same question for the other half: does the state predictor actually get trained, or
    does the loss never route back through it?"""
    model = _live_model(seed=9)
    ids = torch.randint(0, model.cfg.vocab_size, (2, 8))

    out = model(ids)
    out.logits.float().pow(2).mean().backward()

    assert model.internal_state.net[0].weight.grad is not None
    assert model.internal_state.net[0].weight.grad.abs().sum().item() > 0