"""Intent routing.

The architecture note's sharpest example is the reason this module exists. Given *"I feel
completely lost and alone"* the model should drop its own valence and meet the user there.
Given *"why do serial killers feel calm after their crimes"* it should not, because adopting
a serial killer's affect would be grotesque.

The critical property is that these two produce *different* behaviour. A router that
classifies everything as empathetic passes every test that only checks empathy works.
"""

from __future__ import annotations

import pytest
import torch

from elafry.affective.state import all_primary_states
from elafry.models.router import (
    COMPETING_INTENTS,
    INTENTS,
    ROUTING_MODES,
    IntentDecision,
    IntentRouter,
)


@pytest.fixture
def router() -> IntentRouter:
    torch.manual_seed(0)
    return IntentRouter(dim=16)


def _force_intent(router: IntentRouter, intent: str, turn: torch.Tensor) -> torch.Tensor:
    """Point ``classifier`` at ``intent`` so ``argmax`` returns it deterministically."""
    index = COMPETING_INTENTS.index(intent)
    weights = torch.zeros(len(COMPETING_INTENTS), router.dim)
    weights[index] = 10.0
    with torch.no_grad():
        router.classifier.weight.copy_(weights)
        router.classifier.bias.zero_()
        router.crisis_head.weight.zero_()
        router.crisis_head.bias.fill_(-10.0)  # keep the crisis head quiet
    return turn


def _force_crisis(router: IntentRouter, amount: float = 4.0) -> None:
    with torch.no_grad():
        router.crisis_head.weight.fill_(1.0)
        router.crisis_head.bias.fill_(amount)


def test_every_intent_has_a_mode():
    for intent in INTENTS:
        assert intent in ROUTING_MODES


def test_crisis_is_not_in_the_competing_softmax():
    """It has its own head, which is what makes the gate able to fire at all."""
    assert "crisis" in INTENTS
    assert "crisis" not in COMPETING_INTENTS
    assert len(COMPETING_INTENTS) == len(INTENTS) - 1


def test_forward_shapes(router):
    logits, crisis = router(torch.randn(4, 16))
    assert logits.shape == (4, len(COMPETING_INTENTS))
    assert crisis.shape == (4,)


def test_router_rejects_wrong_width(router):
    with pytest.raises(ValueError, match="expected width"):
        router(torch.randn(4, 8))


@pytest.mark.parametrize("intent", COMPETING_INTENTS)
def test_each_competing_intent_routes_to_its_mode(router, intent):
    turn = _force_intent(router, intent, torch.randn(router.dim))
    decision = router.decide(turn)
    assert decision.intent == intent
    assert decision.mode == ROUTING_MODES[intent]
    assert 0.0 <= decision.confidence <= 1.0


def test_crisis_head_overrides_a_competing_argmax(router):
    """The load-bearing case. Venting is the most likely intent, but the crisis head is
    firing, so the turn gets de-escalation rather than reciprocity.

    This is impossible with a single softmax. A probability at or above a threshold implies
    argmax, and one below it means the gate does not fire, so both branches return the same
    answer the argmax already gave.
    """
    turn = _force_intent(router, "venting", torch.ones(router.dim))
    assert router.decide(turn).intent == "venting"

    _force_crisis(router)
    decision = router.decide(turn)
    assert decision.intent == "crisis"
    assert decision.mode == "de_escalate"
    assert decision.crisis_score > router.crisis_threshold


def test_crisis_threshold_is_configurable():
    torch.manual_seed(0)
    strict = IntentRouter(dim=8, crisis_threshold=0.99)
    turn = _force_intent(strict, "casual", torch.ones(8))
    _force_crisis(strict, amount=-6.0)  # sigmoid(2) is about 0.88

    score = strict.decide(turn).crisis_score
    assert 0.8 < score < 0.9

    strict.crisis_threshold = 0.95
    assert strict.decide(turn).intent == "casual", "above threshold, argmax wins"

    strict.crisis_threshold = 0.5
    assert strict.decide(turn).intent == "crisis", "below threshold, the crisis head fires"


def test_quiet_crisis_head_leaves_the_argmax_alone(router):
    turn = _force_intent(router, "disclosure", torch.randn(router.dim))
    decision = router.decide(turn)
    assert decision.crisis_score < router.crisis_threshold
    assert decision.intent == "disclosure"


# ------------------------------------------------- the serial-killer distinction


def test_disclosure_pulls_valence_and_third_party_emotion_does_not():
    """Mirroring a user's sadness is empathy. Mirroring a serial killer's calm is grotesque,
    and the difference has to be in the routing, not a hope the model figures it out
    downstream."""
    router = IntentRouter(dim=8, headroom=0.4)
    user_valence = torch.tensor([-0.8])

    reciprocal = router.valence_pull("disclosure", user_valence)
    assert float(reciprocal[0]) < 0, "disclosure should pull the model's valence down"
    assert float(reciprocal[0]) > float(user_valence[0]), "but only partway"

    analytical = router.valence_pull("third_party_emotion", user_valence)
    assert float(analytical[0]) == 0.0, (
        "the model must not acquire the affect of someone it is only analysing"
    )


def test_crisis_suppresses_valence_pull_entirely():
    router = IntentRouter(dim=8, headroom=0.9)
    assert float(router.valence_pull("crisis", torch.tensor([-1.0]))[0]) == 0.0


def test_analytical_intents_do_not_pull():
    router = IntentRouter(dim=8)
    for intent in ("advice_seeking", "third_party_emotion", "casual"):
        assert float(router.valence_pull(intent, torch.tensor([-0.7]))[0]) == 0.0


def test_every_reciprocal_intent_pulls():
    router = IntentRouter(dim=8, headroom=0.5)
    reciprocal = [i for i, m in ROUTING_MODES.items() if m == "reciprocity"]
    assert reciprocal, "the taxonomy needs at least one reciprocal mode"
    for intent in reciprocal:
        assert float(router.valence_pull(intent, torch.tensor([-0.6]))[0]) < 0, intent


def test_headroom_bounds_the_pull():
    tight = IntentRouter(dim=8, headroom=0.1)
    loose = IntentRouter(dim=8, headroom=0.9)
    v = torch.tensor([-1.0])
    assert float(tight.valence_pull("disclosure", v)[0]) > float(
        loose.valence_pull("disclosure", v)[0]
    )


def test_valence_pull_handles_a_column_vector():
    router = IntentRouter(dim=8)
    out = router.valence_pull("disclosure", torch.tensor([[-0.5], [-0.5]]))
    assert out.shape == (2,)


def test_decision_serialises():
    d = IntentDecision("venting", "reciprocity", 0.75, (0.1, 0.2), crisis_score=0.02)
    payload = d.to_dict()
    assert payload["intent"] == "venting"
    assert payload["mode"] == "reciprocity"
    assert payload["crisis_score"] == 0.02
    assert payload["logits"] == [0.1, 0.2]


# ------------------------------------------------- integration with the model


def test_model_exposes_a_route_helper():
    from conftest import make_tiny_model

    model = make_tiny_model(seed=0)
    ids = torch.randint(0, model.cfg.vocab_size, (1, 8))
    decision = model.route(ids)
    assert decision.intent in INTENTS


def test_routing_is_inference_only(router):
    """``decide`` is no_grad on purpose: it returns a decision, not a differentiable routing.
    Gradients for the router come from calling ``forward`` explicitly during training."""
    turn = torch.randn(router.dim, requires_grad=True)
    router.decide(turn)
    assert turn.grad is None


def test_router_heads_receive_gradients_in_training():
    router = IntentRouter(dim=8)
    logits, crisis = router(torch.randn(3, 8))
    (logits.sum() + crisis.sum()).backward()

    assert router.classifier.weight.grad.abs().sum().item() > 0
    assert router.crisis_head.weight.grad.abs().sum().item() > 0


def test_vad_and_intent_are_independent_signals():
    """A model can be irritated while staying in reciprocal mode, which is the interesting
    case: firm but present. Collapsing the two would force them to be the same thing."""
    states = all_primary_states()
    router = IntentRouter(dim=8, headroom=0.5)

    angry = states["anger"]
    assert float(angry[0, 0]) < 0, "anger is negative-valence"
    assert ROUTING_MODES["disclosure"] == "reciprocity"

    pull = router.valence_pull("disclosure", angry[:, 0])
    assert pull.shape == (1,)
    # The state keeps its own anger; the pull is a separate decision layered on top.
    assert float(states["anger"][0, 0]) < 0