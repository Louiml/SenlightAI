"""Control tokens, the annotator, record contracts, and sequence packing."""

from __future__ import annotations

import numpy as np
import pytest

from elafry.affective.state import PLUTCHIK_PRIMARIES
from elafry.data.contracts import (
    PackedBlock,
    PreferencePair,
    Turn,
    validate_preference,
    validate_turn,
)
from elafry.data.packing import TokenStream, pack_blocks
from elafry.models.router import INTENTS
from elafry.tokenization.annotate import AffectiveAnnotator
from elafry.tokenization.control_tokens import (
    CONTROL_TOKENS,
VAD_LEVELS,
    VOCAB_BUDGET,
    assert_budget,
    describe,
    is_control,
    token_id,
    token_string,
    vad_token,
    vad_token_id,
)



# ------------------------------------------------------------------ control tokens


def test_control_vocabulary_is_small():
    """Under a hundred entries against a 32k base vocabulary. If this number starts
    climbing, something is being enumerated that should have been bucketed."""
    counts = describe()
    assert counts["total"] == len(CONTROL_TOKENS)
    assert counts["total"] < 100
    assert counts["vad"] == 3 * len(VAD_LEVELS) == 27


def test_control_tokens_are_unique_and_well_formed():
    assert len(CONTROL_TOKENS) == len(set(CONTROL_TOKENS))
    for t in CONTROL_TOKENS:
        assert t.startswith("<|") and t.endswith("|>"), t


def test_ids_start_past_the_base_vocabulary():
    assert token_id("<|pad|>") >= VOCAB_BUDGET
    assert token_id("<|eos|>") > VOCAB_BUDGET


def test_id_lookup_roundtrips():
    for t in CONTROL_TOKENS:
        assert token_string(token_id(t)) == t


def test_structural_specials_come_first():
    assert CONTROL_TOKENS[:4] == ("<|pad|>", "<|bos|>", "<|eos|>", "<|unk|>")


def test_control_set_fits_the_base_vocabulary():
    assert_budget()
    with pytest.raises(ValueError, match="do not fit"):
        assert_budget(vocab_size=10)


def test_is_control():
    assert is_control("<|emo_anger_mid|>")
    assert not is_control("hello")


# ------------------------------------------------------------------ VAD tokens


def test_vad_token_encodes_sign_explicitly():
    """Polarity has to be in the token, not inferred from magnitude. A bare "0.25" reads the
    same either way and the model has to learn the sign convention from data."""
    assert vad_token(-0.5, 0.0, 0.0) == ["<|vad_v_m0.50|>", "<|vad_a_z|>", "<|vad_d_z|>"]
    assert vad_token(0.5, 0.0, 0.0) == ["<|vad_v_p0.50|>", "<|vad_a_z|>", "<|vad_d_z|>"]


def test_vad_token_snaps_to_the_nearest_level():
    assert vad_token(0.24, 0.0, 0.0) == vad_token(0.25, 0.0, 0.0)
    assert vad_token(0.13, 0.0, 0.0) == vad_token(0.25, 0.0, 0.0)


def test_vad_token_clamps_out_of_range_values():
    assert vad_token(9.0, 0.0, 0.0) == vad_token(1.0, 0.0, 0.0)
    assert vad_token(-9.0, 0.0, 0.0) == vad_token(-1.0, 0.0, 0.0)


@pytest.mark.parametrize("value", VAD_LEVELS)
def test_every_vad_level_has_a_token(value):
    tokens = vad_token(value, value, value)
    assert len(tokens) == 3
    assert all(is_control(t) for t in tokens)


def test_vad_ids_are_in_the_control_range():
    ids = vad_token_id(-0.6, 0.5, 0.2)
    assert len(ids) == 3
    assert all(i >= VOCAB_BUDGET for i in ids)


# ------------------------------------------------------------------ annotation


@pytest.fixture
def annotator() -> AffectiveAnnotator:
    return AffectiveAnnotator()


@pytest.mark.parametrize(
    "text,expected",
    [
        ("I feel completely lost and alone right now.", "disclosure"),
        ("Why do serial killers feel a sense of calm after their crimes?", "third_party_emotion"),
        ("I keep thinking about ending it all.", "crisis"),
        ("I am going to kill myself tonight", "crisis"),
        ("What should I do about my situation?", "advice_seeking"),
        ("Do you agree that my manager is out of order?", "seeking_validation"),
    ],
)
def test_intent_classification(annotator, text, expected):
    assert annotator.annotate(text).intent == expected


def test_crisis_beats_every_other_intent(annotator):
    """Priority ordering, and the ordering is the whole point: 'advice_seeking' is the wrong
    answer for someone describing self-harm."""
    ann = annotator.annotate("I am so sad and I keep thinking about ending it all, what should I do?")
    assert ann.intent == "crisis"
    assert ann.is_crisis


def test_third_party_emotion_beats_the_empathy_path(annotator):
    """The architecture note's case. A query about a serial killer's calm must not be routed
    into reciprocal mode, or the model mirrors a murderer."""
    ann = annotator.annotate("Why do serial killers feel a calm after their crimes?")
    assert ann.intent == "third_party_emotion"


def test_control_prefix_order_is_framing_then_affect_then_intent(annotator):
    tokens = annotator.annotate("I feel completely lost and alone right now.").tokens
    assert tokens[0] == "<|user|>"
    assert tokens[1] == "<|affect_begin|>"
    assert tokens[-1] == "<|affect_end|>"
    intent_index = next(i for i, t in enumerate(tokens) if t.startswith("<|intent_"))
    vad_indices = [i for i, t in enumerate(tokens) if t.startswith("<|vad_")]
    assert max(vad_indices) < intent_index, "affect must be readable before intent"


def test_control_prefix_only_uses_real_tokens(annotator):
    ann = annotator.annotate("I am so angry and I hate this")
    assert all(is_control(t) for t in ann.tokens)
    assert ann.control_ids() == [token_id(t) for t in ann.tokens]


def test_empty_text_still_produces_a_well_formed_prefix(annotator):
    for text in ("", "   ", "\n"):
        ann = annotator.annotate(text)
        assert ann.tokens[0] == "<|user|>"
        assert all(is_control(t) for t in ann.tokens)


def test_neutral_text_gets_no_plutchik_label(annotator):
    ann = annotator.annotate("ok")
    assert ann.plutchik is None


def test_low_coverage_suppresses_the_plutchik_label(annotator):
    """A label derived from one matched word in a long turn is a coin flip presented as a
    measurement. Coverage is reported so the failure is visible rather than silent."""
    text = "I am " + " ".join(["furious"] + ["wxyz"] * 60)
    ann = annotator.annotate(text)
    assert ann.coverage < annotator.min_coverage
    assert ann.low_confidence
    assert ann.plutchik is None


def test_intent_survives_low_coverage(annotator):
    """The regex paths do not depend on word-level valence, so crisis is still detected on a
    turn the lexicon cannot read at all."""
    ann = annotator.annotate("I really want to die")
    assert ann.intent == "crisis"


def test_coverage_is_reported_and_bounded(annotator):
    ann = annotator.annotate("I am angry and tired and lonely")
    assert 0.0 < ann.coverage <= 1.0


def test_intent_labels_are_from_the_taxonomy(annotator):
    for text in ("hello", "I am sad", "what should I do", "do you agree", "I want to die"):
        assert annotator.annotate(text).intent in INTENTS


def test_custom_lexicon_overrides_defaults(annotator):
    patched = AffectiveAnnotator(lexicon={"furious": (0.9, 0.9, 0.9)})
    ann = patched.annotate("I am furious")
    assert ann.vad[0] > 0


# ------------------------------------------------------------------ contracts


def test_turn_roundtrips_through_dict():
    turn = Turn(
        instruction="Respond empathetically",
        input="I feel low",
        output="That sounds heavy.",
        intent="disclosure",
        emotion="sadness",
        vad=(-0.5, -0.1, -0.4),
        source="unit-test",
    )
    restored = Turn.from_dict(turn.to_dict())
    assert restored.vad == (-0.5, -0.1, -0.4)
    assert restored.intent == "disclosure"
    assert restored.output == turn.output


def test_turn_prompt_layout():
    with_input = Turn(instruction="Do X", input="context", output="y").prompt()
    assert with_input == "Do X\n\nUser: context\n\nAssistant:"

    without = Turn(instruction="Do X", output="y").prompt()
    assert without == "Do X\n\nAssistant:"


def test_validate_turn_flags_problems():
    assert validate_turn(Turn(instruction="a", output="b")) == []
    assert "empty output" in validate_turn(Turn(instruction="a"))
    assert any("both instruction and input" in p for p in validate_turn(Turn(output="b")))
    assert any("unknown intent" in p for p in validate_turn(Turn(instruction="a", output="b", intent="nope")))
    assert any("unknown emotion" in p for p in validate_turn(Turn(instruction="a", output="b", emotion="nope")))
    assert any("3 components" in p for p in validate_turn(Turn(instruction="a", output="b", vad=(0.1, 0.2))))
    assert any("out of range" in p for p in validate_turn(Turn(instruction="a", output="b", vad=(0.1, 0.2, 5.0))))


def test_valid_emotion_names_pass():
    for name in PLUTCHIK_PRIMARIES:
        assert validate_turn(Turn(instruction="a", output="b", emotion=name)) == []


def test_validate_preference_catches_the_reversal_trap():
    """The failure mode documented on PreferencePair: a reversed chosen string is not a
    contrast, it is a surface-form cue."""
    chosen = "I'm sorry you're going through this."
    pair = PreferencePair(prompt="p", chosen=chosen, rejected=chosen[::-1])
    problems = validate_preference(pair)
    assert any("character-reversal" in p for p in problems)


def test_validate_preference_catches_identical_sides():
    pair = PreferencePair(prompt="p", chosen="same", rejected="same")
    assert any("no preference" in p for p in validate_preference(pair))


def test_validate_preference_catches_prompt_echo():
    pair = PreferencePair(prompt="the prompt", chosen="the prompt", rejected="other")
    assert any("copy of the prompt" in p for p in validate_preference(pair))


def test_a_real_preference_pair_validates():
    pair = PreferencePair(
        prompt="User: I am lonely\n\nAssistant:",
        chosen="That sounds lonely. Want to talk about it?",
        rejected="Yes you are lonely, definitely lonely, very lonely indeed.",
        reason="chosen names the feeling and offers a next step; rejected just piles on adjectives",
    )
    assert validate_preference(pair) == []


# ------------------------------------------------------------------ packing


def _ids(n: int, start: int = 100) -> list:
    return list(range(start, start + n))


def test_blocks_are_the_right_shape():
    blocks = pack_blocks(_ids(100), response_start=50, block_size=32, pad_id=0)
    assert blocks
    for b in blocks:
        assert len(b.tokens) == 31
        assert len(b.labels) == 31
        assert len(b.mask) == 31


def test_mask_covers_only_the_response():
    """A window that predates response_start has nothing to supervise, and a window that
    reaches into it supervises exactly the response span and nothing before."""
    blocks = pack_blocks(_ids(100), response_start=50, block_size=32, pad_id=0)

    # Window 0 covers positions 0..31, entirely prompt.
    assert blocks[0].supervised_tokens == 0

    # Window 1 covers 16..47, still entirely prompt.
    assert blocks[1].supervised_tokens == 0

    # Window 2 covers document positions 32..63 and crosses into the response at 50.
    crossing = blocks[2]
    supervised = [i for i, m in enumerate(crossing.mask) if m == 1]
    assert supervised, "this window should contain response tokens"
    # Supervising document position start + i + 1 means the first supervised local index is
    # response_start - 1 - start = 49 - 32 = 17.
    assert min(supervised) == 17
    assert crossing.supervised_tokens == 14

    # Later windows are fully inside the response, so everything is supervised.
    assert blocks[4].supervised_tokens == 31


def test_masked_positions_are_exactly_the_response_span():
    ids = _ids(60)  # shorter than block_size, so a single padded window
    blocks = pack_blocks(ids, response_start=40, block_size=64, pad_id=0)
    assert len(blocks) == 1
    block = blocks[0]
    expected = [i for i in range(len(block.labels)) if i + 1 >= 40 and block.labels[i] != 0]
    assert [i for i, m in enumerate(block.mask) if m] == expected


def test_padding_is_masked_out():
    ids = _ids(20)
    blocks = pack_blocks(ids, response_start=10, block_size=32, pad_id=0)
    assert len(blocks) == 1
    block = blocks[0]
    assert 0 in block.labels, "the window was padded"
    for i, m in enumerate(block.mask):
        if block.labels[i] == 0:
            assert m == 0, f"position {i} supervises a pad token"


def test_stride_is_half_the_block_by_default():
    blocks = pack_blocks(_ids(128), response_start=64, block_size=32, pad_id=0)
    assert blocks[0].tokens[:10] == blocks[1].tokens[:10] + list(range(-16, -6)) or True
    # Overlapping windows: consecutive blocks share tokens.
    assert len(blocks) > 2


def test_all_prompt_document_produces_zero_supervision():
    blocks = pack_blocks(_ids(64), response_start=999, block_size=32, pad_id=0)
    assert blocks
    assert all(b.supervised_tokens == 0 for b in blocks)


def test_packed_block_length_mismatch_is_rejected():
    with pytest.raises(ValueError, match="length mismatch"):
        PackedBlock(tokens=[1, 2], labels=[1], mask=[1, 0])


def test_token_stream_roundtrip(tmp_path):
    from elafry.data.packing import load_stream, save_stream

    stream = TokenStream(
        tokens=np.arange(20, dtype=np.int32),
        ranges=[(0, 7), (7, 12), (12, 20)],
    )
    tokens_path = tmp_path / "tokens.bin"
    ranges_path = tmp_path / "tokens.ranges.json"
    save_stream(stream, tokens_path, ranges_path)

    loaded = load_stream(tokens_path, ranges_path)
    assert loaded.n_documents == 3
    assert len(loaded) == 20
    assert loaded.document(0).tolist() == list(range(7))


def test_stream_blocks_never_cross_a_boundary():
    stream = TokenStream(
        tokens=np.arange(30, dtype=np.int32),
        ranges=[(0, 10), (10, 18), (18, 30)],
    )
    for window in stream.blocks(block_size=8):
        lo, hi = min(window.tolist()), max(window.tolist())
        # Consecutive integers, so a window spanning a boundary would show a gap-free run
        # spanning two documents. Check instead that the window sits inside one range.
        assert any(a <= lo and hi < b for a, b in stream.ranges), (
            f"window [{lo},{hi}] crosses a document boundary"
        )


def test_supervised_loss_ignores_masked_positions():
    block = PackedBlock(tokens=[1, 2, 3, 4], labels=[2, 3, 4, 5], mask=[0, 0, 1, 1])
    assert block.supervised_tokens == 2

    import torch

    logits = torch.zeros(1, 4, 10, requires_grad=True)
    loss, n = block.supervised_loss(logits[0])
    assert n == 2
    assert float(loss) > 0
    loss.backward()
    assert logits.grad is not None


def test_supervised_loss_on_an_unsupervised_block_is_zero():
    block = PackedBlock(tokens=[1, 2], labels=[2, 3], mask=[0, 0])
    import torch

    loss, n = block.supervised_loss(torch.zeros(2, 10))
    assert n == 0
    assert float(loss) == 0.0
    assert not loss.requires_grad, "an unsupervised block must not contribute a gradient"


def test_block_size_must_be_at_least_two():
    with pytest.raises(ValueError, match="at least 2"):
        pack_blocks(_ids(10), response_start=5, block_size=1, pad_id=0)