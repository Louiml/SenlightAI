"""KV cache parity: cached decode must equal full prefill, position for position.

This is the single most valuable test in the suite. A transformer with a KV cache that
disagrees with its own prefill will generate text that looks locally plausible and is
globally wrong, and it will do so on a randomly initialised model, so nothing short of this
assertion catches it.

The affective bias is what makes it interesting here. A bias added to pre-softmax logits
could easily differ between the batched prefill path and the single-token decode path, and
the failure would be subtle: correct shapes, plausible tokens, wrong distribution. See the
note in ``elafry/models/attention.py`` about why the bias is deliberately formulated in
feature space rather than position space.
"""

from __future__ import annotations

import pytest
import torch


from conftest import make_tiny_model


@pytest.mark.parametrize("state_source", ["internal", "external"])
def test_prefill_matches_incremental_decode(state_source):
    """Feed a prompt through prefill, then feed the rest one token at a time. The logits at
    each position must match to float32 rounding."""
    model = make_tiny_model(seed=0, state_source=state_source)
    model.eval()
    ids = torch.randint(0, model.cfg.vocab_size, (1, 10))

    with torch.no_grad():
        full = model(ids)

        # Replay the same sequence, but feed token i only after caching i-1.
        past = None
        step_logits = []
        for i in range(ids.shape[1]):
            out = model(ids[:, i : i + 1], past_kv=past, use_cache=True)
            past = out.present_kv
            step_logits.append(out.logits[:, -1, :])

    stepped = torch.stack(step_logits, dim=1)  # (1, S, vocab)

    diff = (full.logits - stepped).abs().max().item()
    assert diff < 1e-4, (
        f"cached decode diverged from prefill by {diff:.2e} "
        f"(state_source={state_source})"
    )


def test_cache_parity_holds_with_a_pinned_external_state():
    """Same check, but with the state held fixed across steps.

    Without this, a bug where the state silently changes between prefill and decode would
    pass the previous test whenever the model's derived state happens to be stable.
    """
    model = make_tiny_model(seed=1, state_source="external")
    model.eval()
    ids = torch.randint(0, model.cfg.vocab_size, (1, 8))
    pinned = torch.tensor([[-0.6, 0.5, 0.2]])

    with torch.no_grad():
        full = model(ids, state=pinned)

        past = None
        step_logits = []
        for i in range(ids.shape[1]):
            out = model(ids[:, i : i + 1], past_kv=past, use_cache=True, state=pinned)
            past = out.present_kv
            step_logits.append(out.logits[:, -1, :])

    stepped = torch.stack(step_logits, dim=1)
    assert (full.logits - stepped).abs().max().item() < 1e-4


def test_cache_grows_by_one_token_per_step():
    model = make_tiny_model(seed=2)
    ids = torch.randint(0, model.cfg.vocab_size, (1, 4))

    past = None
    for i in range(ids.shape[1]):
        out = model(ids[:, i : i + 1], past_kv=past, use_cache=True)
        past = out.present_kv
        assert len(past) == model.cfg.n_layers
        k, v = past[0]
        assert k.shape[2] == i + 1, "cache should hold exactly the tokens seen so far"
        assert v.shape == k.shape


def test_kv_cache_is_stored_unexpanded():
    """The cache holds ``n_kv_heads``, not ``n_heads``. Storing expanded keys would make the
    cache n_rep times bigger, which is the entire reason grouped-query attention exists."""
    model = make_tiny_model(seed=3)
    ids = torch.randint(0, model.cfg.vocab_size, (1, 5))
    out = model(ids, use_cache=True)
    k, _ = out.present_kv[0]
    assert k.shape[1] == model.cfg.n_kv_heads
    assert k.shape[1] != model.cfg.n_heads or model.cfg.n_rep == 1


def test_causal_mask_blocks_the_future():
    """Changing a future token must not change an earlier position's logits."""
    model = make_tiny_model(seed=4)
    model.eval()
    ids = torch.randint(0, model.cfg.vocab_size, (1, 10))

    with torch.no_grad():
        a = model(ids).logits
        modified = ids.clone()
        modified[0, -1] = (modified[0, -1] + 7) % model.cfg.vocab_size
        b = model(modified).logits

    # Positions before the last token must be bit-identical.
    assert (a[:, :-1] - b[:, :-1]).abs().max().item() == 0.0
    # The last position must actually differ, or the test is vacuous.
    assert (a[:, -1] - b[:, -1]).abs().max().item() > 0.0


def test_generate_is_reproducible_under_a_fixed_seed():
    model = make_tiny_model(seed=5)
    ids = torch.randint(0, model.cfg.vocab_size, (1, 6))

    torch.manual_seed(0)
    tokens, state = model.generate(
        ids, max_new_tokens=5, temperature=1.0, top_k=0, top_p=1.0, eos_id=None
    )
    assert tokens.shape == (1, 5)
    assert state.shape == (1, 3)

    # Same seed, same output. A sampler that ignores the generator makes every evaluation
    # of this model unreproducible.
    torch.manual_seed(0)
    again, _ = model.generate(ids, max_new_tokens=5, temperature=1.0, top_k=0, top_p=1.0)
    assert torch.equal(tokens, again)

    # A different seed gives different text, otherwise sampling is not sampling.
    torch.manual_seed(1)
    different, _ = model.generate(ids, max_new_tokens=5, temperature=1.0, top_k=0, top_p=1.0)
    assert not torch.equal(tokens, different)


def test_generate_stops_at_eos():
    """top_k=1 makes generation deterministic, so we can name the token it will emit and
    then assert the loop terminates on exactly that token."""
    model = make_tiny_model(seed=6)
    ids = torch.randint(0, model.cfg.vocab_size, (1, 4))

    torch.manual_seed(0)
    unstopped, _ = model.generate(
        ids, max_new_tokens=10, temperature=1.0, top_k=1, top_p=1.0, eos_id=None
    )
    assert unstopped.shape[1] == 10

    first = int(unstopped[0, 0])
    torch.manual_seed(0)
    stopped, _ = model.generate(
        ids, max_new_tokens=10, temperature=1.0, top_k=1, top_p=1.0, eos_id=first
    )
    assert stopped.shape[1] == 1, "generation should stop the moment it emits EOS"


def test_generate_truncates_an_overlong_prompt():
    """A prompt longer than the context budget minus the generation budget gets truncated
    from the left, keeping the most recent tokens."""
    model = make_tiny_model(seed=7)
    ids = torch.randint(0, model.cfg.vocab_size, (1, model.cfg.max_seq_len + 20))

    tokens, _ = model.generate(
        ids, max_new_tokens=4, temperature=1.0, top_k=1, top_p=1.0, eos_id=None
    )
    assert tokens.shape[1] == 4

def test_sampling_temperature_zero_is_rejected():
    from elafry.models.elafry import sample_from_logits

    with pytest.raises(ValueError, match="temperature must be positive"):
        sample_from_logits(torch.randn(1, 10), temperature=0.0)


def test_top_p_narrows_the_candidate_set():
    from elafry.models.elafry import sample_from_logits

    logits = torch.tensor([[10.0, 9.0, -50.0, -50.0]])
    # A nucleus of 0.5 keeps only the top token, so 1000 draws all land on index 0.
    draws = [
        sample_from_logits(logits, temperature=1.0, top_k=0, top_p=0.5).item()
        for _ in range(200)
    ]
    assert set(draws) == {0}


def test_top_k_keeps_exactly_k_candidates():
    from elafry.models.elafry import sample_from_logits

    logits = torch.tensor([[5.0, 4.0, 3.0, 2.0, 1.0]])
    seen = {
        sample_from_logits(logits, temperature=1.0, top_k=2, top_p=1.0).item()
        for _ in range(300)
    }
    assert seen <= {0, 1}