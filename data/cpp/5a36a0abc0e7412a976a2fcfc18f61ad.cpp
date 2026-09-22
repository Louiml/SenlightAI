/*
Write a C++ function `float exp2_float(float x)` that computes the base-2 exponential of a single-precision floating-point input, matching the accuracy and exceptional-case behavior of the standard `exp2f`. The implementation must use a table-driven approach: reduce `x` into an integer part `k` and a fractional part `y` such that `x = k + y`, then split `y` into a table index and a small remainder, compute the exponential via table lookup and a degree-4 polynomial, and scale by powers of two. Handle all IEEE-754 special values: NaN, positive/negative infinity, overflow (for `x >= 128`), underflow (for `x <= -150`), and the subnormal/normal boundary (|x| ≤ 2^-25 returns `1.0f + x`). The function must return the correctly rounded result within 0.501 ulp for normal inputs.
*/
#include <cstdint>
#include <cstring>
#include <cmath>

// Compute base-2 exponential of a float using table-driven method.
float exp2_float(float x) {
    // Table of exp2(i/16) for i = 0..15 (double precision).
    static const double exp2ft[16] = {
        0x1.6a09e667f3bcdp-1, 0x1.7a11473eb0187p-1,
        0x1.8ace5422aa0dbp-1, 0x1.9c49182a3f090p-1,
        0x1.ae89f995ad3adp-1, 0x1.c199bdd85529cp-1,
        0x1.d5818dcfba487p-1, 0x1.ea4afa2a490dap-1,
        0x1.0000000000000p+0, 0x1.0b5586cf9890fp+0,
        0x1.172b83c7d517bp+0, 0x1.2387a6e756238p+0,
        0x1.306fe0a31b715p+0, 0x1.3dea64c123422p+0,
        0x1.4bfdad5362a27p+0, 0x1.5ab07dd485429p+0
    };

    const float redux = 0x1.8p23f / 16.0f;  // 2^19
    const float P1 = 0x1.62e430p-1f;
    const float P2 = 0x1.ebfbe0p-3f;
    const float P3 = 0x1.c6b348p-5f;
    const float P4 = 0x1.3b2c9cp-7f;
    const float huge = 0x1p100f;
    const float twom100 = 0x1p-100f;

    uint32_t hx, ix;
    std::memcpy(&hx, &x, sizeof(hx));
    ix = hx & 0x7fffffff;

    // Handle exceptional cases.
    if (ix >= 0x43000000) {  // |x| >= 128
        if (ix >= 0x7f800000) {  // NaN or Inf
            if ((ix & 0x7fffff) != 0 || (hx & 0x80000000) == 0)
                return x + x;  // NaN or +Inf
            else
                return 0.0f;    // -Inf
        }
        if (x >= 0x1.0p7f)
            return huge * huge;  // overflow
        if (x <= -0x1.2cp7f)
            return twom100 * twom100;  // underflow
    } else if (ix <= 0x33000000) {  // |x| <= 2^-25
        return 1.0f + x;
    }

    // Reduce x: x = t + z, where t is integer multiple of 1/16.
    float t = x + redux;
    uint32_t i0;
    std::memcpy(&i0, &t, sizeof(i0));
    i0 += 8;  // TBLSIZE/2 = 8
    int32_t k = static_cast<int32_t>(i0 >> 4) << 20;  // exponent bits
    i0 &= 15;  // table index (0..15)
    t -= redux;
    double z = static_cast<double>(x) - t;

    // Build 2^k as double.
    uint64_t twopk_bits = (static_cast<uint64_t>(0x3ff00000 + k)) << 32;
    double twopk;
    std::memcpy(&twopk, &twopk_bits, sizeof(twopk));

    // Compute exp2(y) = exp2ft[i0] * exp2(z) with polynomial.
    double tv = exp2ft[i0];
    double u = tv * z;
    tv = tv + u * (P1 + z * P2) + u * (z * z) * (P3 + z * P4);

    // Scale by 2^k.
    return static_cast<float>(tv * twopk);
}
#include <cassert>
#include <cmath>
#include <cstdio>

// Prototype of the solution function.
float exp2_float(float x);

int main() {
    // Basic values
    assert(exp2_float(0.0f) == 1.0f);
    assert(exp2_float(1.0f) == 2.0f);
    assert(exp2_float(-1.0f) == 0.5f);
    assert(exp2_float(2.0f) == 4.0f);

    // Fractional exponents
    assert(std::fabs(exp2_float(0.5f) - std::sqrt(2.0f)) < 1e-6f);
    assert(std::fabs(exp2_float(-0.5f) - (1.0f / std::sqrt(2.0f))) < 1e-6f);

    // Small magnitude: should return 1 + x
    assert(exp2_float(1e-7f) == 1.0f + 1e-7f);

    // Special values
    assert(std::isnan(exp2_float(NAN)));
    assert(exp2_float(HUGE_VALF) == HUGE_VALF);
    assert(exp2_float(-HUGE_VALF) == 0.0f);

    // Overflow and underflow
    assert(exp2_float(128.0f) == HUGE_VALF);  // or large
    assert(exp2_float(-150.0f) == 0.0f);      // underflow

    // Accuracy check against standard library for a few points
    float test_vals[] = { -100.0f, -10.0f, -2.5f, -0.1f, 0.25f, 1.75f, 10.0f, 127.9f };
    for (float v : test_vals) {
        float ref = std::exp2(v);
        float got = exp2_float(v);
        // Allow slight tolerance due to different implementations.
        assert(std::fabs(got - ref) < 1e-4f * std::fabs(ref));
    }

    // Negative zero
    assert(exp2_float(-0.0f) == 1.0f);

    std::printf("All tests passed.\n");
    return 0;
}
// The core algorithm follows Tang’s table-driven method for `exp2f`. First, the input is inspected via bit manipulation (`GET_FLOAT_WORD`) to check special cases: if `|x|` is NaN or +Inf, returning `x + x` propagates the appropriate value; for −Inf return 0; if `x >= 128` return a huge constant to signal overflow; if `x <= -150` return a tiny constant for underflow; if `|x| ≤ 2^-25`, approximate `exp2(x) ≈ 1 + x` since the error is below half ulp. For normal reduction, add a precomputed `redux` constant (0x1.8p23f / 16 = 2^19) so that rounding to float splits `x` into `t` (an integer multiple of 1/16) and `z = x - t` with `|z| ≤ 2^-5`. Extract the table index bits (`i0`) and integer exponent `k` from `t`. The table `exp2ft` stores `exp2(i/16)` for `i` from 0 to 15, covering the range [0.5, 1.414). Use the polynomial `P1..P4` to compute `exp2(z)` via Horner-like evaluation: `tv = exp2ft[i0]`, `u = tv * z`, then `tv = tv + u*(P1 + z*P2) + u*z*z*(P3 + z*P4)`. Finally, scale by `twopk` (constructed as a double with exponent bits set to `0x3ff00000 + k`) to apply the power-of-two factor. The algorithm runs in O(1) time and uses O(1) auxiliary space, with the table constant-size. Edge cases are handled before reduction, ensuring no overflow in intermediate operations, and the use of double precision for computation reduces roundoff error.
