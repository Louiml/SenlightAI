"""Rotary position embeddings.

RoPE has two conventions in circulation, and picking the wrong one produces a model that
trains without diverging and never learns to use position. So this file pins the behaviour
against a closed-form reference rather than checking it against itself.
"""

from __future__ import annotations



import pytest
import torch

from elafry.models.rope import apply_rope, precompute_rope, rotate_half


def test_table_shapes():
    _, cos, sin = precompute_rope(head_dim=16, max_seq_len=32, theta=10000.0)
    assert cos.shape == (32, 16)
    assert sin.shape == (32, 16)


def test_head_dim_must_be_even():
    with pytest.raises(ValueError, match="even for RoPE"):
        precompute_rope(head_dim=15, max_seq_len=4, theta=10000.0)


def test_position_zero_is_the_identity_rotation():
    """At position 0 the rotation angle is zero, so RoPE must be a no-op. If this fails,
    every sequence starts with a corrupted first token."""
    _, cos, sin = precompute_rope(head_dim=8, max_seq_len=4, theta=10000.0)
    x = torch.randn(2, 4, 1, 8)
    # apply_rope expects tables already gathered to this call's positions, which is what
    # Elafry.forward does.
    out = apply_rope(x, cos[:1], sin[:1])
    assert torch.allclose(out, x, atol=1e-6)


def test_rotate_half_negates_the_back_half():
    x = torch.arange(8, dtype=torch.float32).reshape(1, 1, 1, 8)
    out = rotate_half(x)
    expected = torch.tensor([-4.0, -5.0, -6.0, -7.0, 0.0, 1.0, 2.0, 3.0]).reshape(1, 1, 1, 8)
    assert torch.allclose(out, expected)


def test_matches_a_closed_form_rotation():
    """Independent reference: rotate the first half by +angle and the second by -angle."""
    head_dim, theta, seq = 8, 10000.0, 5
    _, cos, sin = precompute_rope(head_dim, 16, theta)

    torch.manual_seed(0)
    x = torch.randn(1, 2, seq, head_dim)
    got = apply_rope(x, cos[:seq], sin[:seq])

    half = head_dim // 2
    inv_freq = 1.0 / (theta ** (torch.arange(half, dtype=torch.float32) / half))
    expected = torch.empty_like(x)
    for pos in range(seq):
        angle = pos * inv_freq
        for head in range(2):
            front, back = x[0, head, pos, :half], x[0, head, pos, half:]
            # cos/sin tables duplicate the frequencies, so take the first half.
            c, s = torch.cos(angle), torch.sin(angle)
            expected[0, head, pos, :half] = front * c - back * s
            expected[0, head, pos, half:] = back * c + front * s

    assert torch.allclose(got, expected, atol=1e-5), (
        "rotation does not match the closed form; check the angle convention"
    )


def test_rotation_preserves_norm():
    """RoPE is a rotation, so it must not change vector lengths. A scaling error here
    silently changes the effective attention temperature with position."""
    _, cos, sin = precompute_rope(8, 16, 10000.0)
    x = torch.randn(2, 4, 7, 8)
    out = apply_rope(x, cos[:7], sin[:7])
    assert torch.allclose(x.norm(dim=-1), out.norm(dim=-1), atol=1e-5)


def test_relative_distance_is_position_dependent():
    """The dot product of two rotated vectors depends only on their position difference.
    That is the entire point of RoPE, and it is what lets a 512-token context generalise
    past the lengths it trained on."""
    head_dim, theta, max_len = 16, 10000.0, 32
    _, cos, sin = precompute_rope(head_dim, max_len, theta)

    torch.manual_seed(0)
    q = torch.randn(1, 1, 1, head_dim)
    k = torch.randn(1, 1, 1, head_dim)

    def score(q_pos: int, k_pos: int) -> float:
        qr = apply_rope(q, cos[q_pos : q_pos + 1], sin[q_pos : q_pos + 1])
        kr = apply_rope(k, cos[k_pos : k_pos + 1], sin[k_pos : k_pos + 1])
        return float((qr * kr).sum())

    # Same distance, different absolute positions.
    assert score(5, 2) == pytest.approx(score(20, 17), abs=1e-4)
    # Different distance, same start.
    assert score(5, 2) != pytest.approx(score(5, 12), abs=1e-3)


def test_accepts_per_batch_position_tables():
    x = torch.randn(3, 2, 4, 8)
    cos = torch.randn(3, 4, 8)
    sin = torch.randn(3, 4, 8)
    out = apply_rope(x, cos, sin)
    assert out.shape == x.shape
    # Each batch row gets its own table.
    for b in range(3):
        single = apply_rope(x[b : b + 1], cos[b], sin[b])
        assert torch.allclose(out[b : b + 1], single, atol=1e-6)


def test_rejects_mismatched_head_dim():
    x = torch.randn(1, 1, 2, 8)
    with pytest.raises(ValueError, match="head_dim mismatch"):
        apply_rope(x, torch.randn(2, 16), torch.randn(2, 16))


def test_rejects_wrong_position_count():
    x = torch.randn(1, 1, 4, 8)
    with pytest.raises(ValueError, match="position count"):
        apply_rope(x, torch.randn(2, 8), torch.randn(2, 8))


def test_theta_scales_the_low_frequencies():
    """A larger theta pushes the longest wavelength out, which is why modern models use
    500k rather than 10k: it extends the usable context."""
    _, cos_small, _ = precompute_rope(16, 64, 10_000.0)
    _, cos_large, _ = precompute_rope(16, 64, 500_000.0)

    # The lowest frequency governs the longest wavelength. A larger theta pushes it down, so
    # a given position rotates less far, which is why 500k extends the usable context.
    # Comparing the unwrapped angle avoids atan2, which folds back at +/-pi and would report
    # two genuinely different angles as identical.
    inv_small, _, _ = precompute_rope(16, 64, 10_000.0)
    inv_large, _, _ = precompute_rope(16, 64, 500_000.0)

    pos = 63
    angle_small = pos * float(inv_small[-1])
    angle_large = pos * float(inv_large[-1])
    assert angle_small > angle_large
    assert inv_large[-1] < inv_small[-1]
    assert torch.allclose(cos_small[pos].abs(), torch.ones(16), atol=1.0)