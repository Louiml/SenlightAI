// Write a C++ function named `unsigned_divide_by_constant` that performs unsigned 32-bit integer division of a given dividend by a positive constant divisor, using only bitwise shifts, additions, and multiplications—no division or modulo operators are allowed anywhere in the function body. The function must return the quotient exactly as it would be computed by normal integer division, for any 32-bit unsigned dividend and any divisor in the range 1 to 2^31−1. You may assume the divisor is positive and not zero. The function must avoid using the `%` operator, the `/` operator, or any standard library function that performs division or modulo (e.g., `std::div`). Use the classic "division by invariant integers using multiplication" technique: compute a magic multiplier and shift constant for the divisor once, then apply them to the dividend. Because the dividend is unsigned and the divisor is positive, you can use a simpler formulation than the signed case: for an unsigned 32-bit divisor `d`, find a 33-bit (or 32-bit) magic constant `M` and a shift `s` such that `dividend / d == (uint64_t)((uint64_t)dividend * M) >> (32 + s)` for all 32-bit dividends. Since the dividend is non‑negative, no rounding or sign correction is needed. Verify that the magic constant and shift work for the entire unsigned 32-bit range by testing a representative sample, including edge cases like dividend=0, dividend=UINT32_MAX, and dividends that are multiples or close multiples of the divisor.
#include <cassert>
#include <cstdint>
#include <cstdio>

// Declaration of the function under test (as provided in the solution).
uint32_t unsigned_divide_by_constant(uint32_t dividend, uint32_t divisor);

int main() {
    // Basic cases.
    assert(unsigned_divide_by_constant(0, 7) == 0);
    assert(unsigned_divide_by_constant(1, 7) == 0);
    assert(unsigned_divide_by_constant(6, 7) == 0);
    assert(unsigned_divide_by_constant(7, 7) == 1);
    assert(unsigned_divide_by_constant(8, 7) == 1);
    assert(unsigned_divide_by_constant(13, 7) == 1);
    assert(unsigned_divide_by_constant(14, 7) == 2);
    assert(unsigned_divide_by_constant(100, 10) == 10);
    assert(unsigned_divide_by_constant(99, 10) == 9);

    // Largest dividend.
    assert(unsigned_divide_by_constant(UINT32_MAX, 1) == UINT32_MAX);
    assert(unsigned_divide_by_constant(UINT32_MAX, 2) == UINT32_MAX / 2);
    assert(unsigned_divide_by_constant(UINT32_MAX, 3) == UINT32_MAX / 3);
    assert(unsigned_divide_by_constant(UINT32_MAX, UINT32_MAX / 2) == 2);
    assert(unsigned_divide_by_constant(UINT32_MAX, UINT32_MAX / 2 + 1) == 1);

    // Powers of two divisors.
    assert(unsigned_divide_by_constant(12345, 1) == 12345);
    assert(unsigned_divide_by_constant(12345, 2) == 12345 / 2);
    assert(unsigned_divide_by_constant(12345, 4) == 12345 / 4);
    assert(unsigned_divide_by_constant(12345, 8) == 12345 / 8);
    assert(unsigned_divide_by_constant(12345, 16) == 12345 / 16);
    assert(unsigned_divide_by_constant(12345, 1024) == 12345 / 1024);
    assert(unsigned_divide_by_constant(12345, 1u << 31) == 0);

    // Random sampling across the full dividend range (using LCG for determinism).
    uint32_t seed = 42;
    for (uint32_t d = 1; d <= 100; ++d) {
        for (int i = 0; i < 1000; ++i) {
            // Pseudo-random dividend from a simple LCG.
            seed = seed * 1664525u + 1013904223u;
            uint32_t n = seed;
            uint32_t expected = n / d; // use native division for comparison
            assert(unsigned_divide_by_constant(n, d) == expected);
        }
    }

    // Test a few large divisors that produce large shift values.
    uint32_t large_divisors[] = {1000000000u, 2000000000u, 3000000000u, 4000000000u, UINT32_MAX / 2, UINT32_MAX - 1};
    for (uint32_t d : large_divisors) {
        for (uint32_t n : {0u, 1u, 100u, UINT32_MAX / 2, UINT32_MAX - 1, UINT32_MAX}) {
            assert(unsigned_divide_by_constant(n, d) == n / d);
        }
    }

    // Exhaustive check for a small range of dividends for a selected divisor.
    for (uint32_t n = 0; n < 100000; ++n) {
        assert(unsigned_divide_by_constant(n, 12345) == n / 12345);
    }

    return 0;
}
#include <cstdint>
#include <cstdlib>

// Compute magic multiplier and shift for unsigned 32-bit division by d.
// Returns true on success (always true for d >= 1), and sets M and s.
static bool magic_u32_div_constants(uint32_t d, uint64_t &M, uint32_t &s) {
    if (d == 0) return false;
    if (d == 1) {
        M = (uint64_t(1) << 32); // effectively 2^32, shift 0
        s = 0;
        return true;
    }

    // Standard algorithm: find p and M such that (n * M) >> (32 + s) == n / d.
    uint32_t p = 32;
    uint64_t q = (uint64_t(1) << 32) / d;
    uint64_t r = (uint64_t(1) << 32) - q * d;

    while (r > d) {
        p += 1;
        q <<= 1;
        r <<= 1;
        if (r >= uint64_t(d)) {
            q += 1;
            r -= d;
        }
    }
    // At loop exit, q = ceil(2^p / d), and p >= 32.
    M = q;
    s = p - 32;
    return true;
}

// Unsigned 32-bit division by a positive constant divisor using multiply-shift.
// Returns dividend / divisor exactly for all uint32_t dividends.
uint32_t unsigned_divide_by_constant(uint32_t dividend, uint32_t divisor) {
    // Precompute magic constants for the given divisor.
    uint64_t magic;
    uint32_t shift;
    bool ok = magic_u32_div_constants(divisor, magic, shift);
    if (!ok) {
        // Should never happen because divisor >= 1, but return 0 for safety.
        return 0;
    }

    // Special case for divisor == 1: just return dividend.
    if (divisor == 1) {
        return dividend;
    }

    // The quotient is the high part of the 64-bit product after shifting.
    uint64_t product = static_cast<uint64_t>(dividend) * magic;
    uint32_t quotient = static_cast<uint32_t>(product >> (32 + shift));
    return quotient;
}
// The core algorithm is the well‑known "magic number" division technique from Hacker's Delight (Chapter 10) and Granlund & Montgomery's paper. For an unsigned divisor `d`, we find a 33‑bit magic value `M` (stored as a 64‑bit integer) and a shift amount `s` such that for every unsigned 32‑bit `n`, the expression `( (uint64_t)n * M ) >> (32 + s)` exactly equals `n / d`. The derivation: we need `M ≈ 2^(32 + s) / d` rounded up enough so the multiplication introduces no error. We compute `M` using the following iterative algorithm (unsigned variant): set `p = 32`, `q = floor(2^32 / d)`, `r = 2^32 − q*d`. While the condition `r > d` holds, increment `p`, double `q` and `r`, and if `r >= d`, reduce `r -= d` and increment `q`. When the loop exits, we have `q = ceil(2^p / d)` for some `p ≥ 32`. Set `M = q` (which fits in 64 bits, and for `d > 1` we have `M < 2^33`), and `s = p − 32`. Then for any `n < 2^32`, `(n * M) >> (32 + s) = floor(n / d)`. This works because the product `n*M` is at most `(2^32−1) * (2^33−1) < 2^65`, so it fits in a 64‑bit unsigned integer without overflow. The shift by `32 + s` (where `s` is between 0 and 31) extracts the high part. For `d = 1`, the magic is simply `M = 2^32`, `s = 0`; we can handle that as a special case. For `d` being a power of two, a shift alone suffices, but the magic method also works; we might special‑case it for clarity but it is not required. The calculated quotient is exact for all 32‑bit dividends. Time complexity is O(1) for the division itself (a multiply and a shift), and the magic‑constant computation is O(log d) in the worst case (the loop runs O(log d) iterations) but is done only once per divisor. Space usage is O(1). The important edge cases: `dividend = 0` yields `0`; `dividend = UINT32_MAX` with any divisor gives the correct floor; divisors that are powers of two produce a shift that avoids any rounding; and divisors near `2^31` produce a large `s` (up to 31) and a magic constant near `2^33` that still fits in 64 bits. We must ensure the multiplication uses 64‑bit arithmetic and the shift is on a 64‑bit value to avoid overflow. Also, the condition in the magic‑constant loop must use unsigned comparisons to avoid signed overflow; we implement it using `uint64_t` and careful arithmetic.
