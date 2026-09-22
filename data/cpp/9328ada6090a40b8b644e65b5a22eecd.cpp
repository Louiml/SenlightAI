// Implement a C++ function that computes an approximation of the natural logarithm for a vector of two double-precision values using a polynomial approximation with SIMD intrinsics. The function should take a `__m128d` input (two doubles) and return a `__m128d` with the logarithm of each element. The approximation should use the following mathematical decomposition: represent each input `a` as `m * 2^e` where `m` is in the range [0.75, 1.5) and `e` is an integer exponent. Then compute `log(a) = log(m) + e * ln(2)`, where `log(m)` is approximated using a polynomial in `(m-1)`. Handle special cases: for `0`, return `-infinity`; for negative inputs, return `NaN`; for `+infinity` or `NaN` inputs, return the input itself. Use no branches for the main computation path (only for special cases via a mask check), and use `_mm_fmadd_pd` for fused multiply-add operations. The polynomial coefficients are provided in an array; you must implement the polynomial evaluation using an Estrin scheme for the first 16 terms and Horner for the remaining lower-order terms. Assume the input is in IEEE-754 double format and that all values are properly aligned.
// The core algorithm decomposes each double `a` into a mantissa `m` and an exponent `e` by manipulating the IEEE-754 bit representation. First, we isolate the mantissa bits by masking the sign and exponent, then set the exponent to a bias that makes the mantissa in [1, 2). To improve accuracy, we check if the mantissa is above a threshold (e.g., sqrt(2)): if so, we halve the mantissa (by subtracting a half-adder constant) and increment the exponent by 1, so `m` ends up in [0.75, 1.5). The exponent is computed by shifting the original bits right by 52, then subtracting a bias constant (1023 for the true exponent, but here we use a tuned constant `TEN_23` to shift the range). For the polynomial, we compute `m' = m - 1` and evaluate `log(m)` as a degree-24 polynomial using Estrin's scheme for the highest 16 terms (grouped into 8 pairs then combined with m², m⁴, m⁸) and then Horner for the remaining 8 terms. Finally, we add `e * ln(2)` using two-part `LN2_HI` and `LN2_LO` for precision. Special cases are detected by checking if the input is non-positive or has an all-ones exponent (inf/nan). For non-positive, we return NaN (for negative) or -infinity (for zero). For inf/nan, we return the result of `a + a` (which yields inf for inf, nan for nan). The main function avoids branches using blend instructions, and only at the end does it check a mask to call the special-case handler. Time complexity is O(1) constant for the vector, and space is O(1).
#include <cstdint>
#include <cstring>
#include <immintrin.h>

// Polynomial coefficients for log(1+x) approximation (degree 24)
// These are example constants; in a real implementation, they would be precomputed.
static const double LOG_COEFFS[25] = {
    1.0,                     // C1
    -0.5,                    // C2
    0.3333333333333333,       // C3
    -0.25,                   // C4
    0.2,                     // C5
    -0.16666666666666666,     // C6
    0.14285714285714285,      // C7
    -0.125,                  // C8
    0.1111111111111111,       // C9
    -0.1,                    // C10
    0.09090909090909091,      // C11
    -0.08333333333333333,     // C12
    0.07692307692307693,      // C13
    -0.07142857142857142,     // C14
    0.06666666666666667,      // C15
    -0.0625,                 // C16
    0.058823529411764705,     // C17
    -0.05555555555555555,     // C18
    0.05263157894736842,      // C19
    -0.05,                   // C20
    0.047619047619047616,     // C21
    -0.045454545454545456,    // C22
    0.043478260869565216,     // C23
    -0.041666666666666664,    // C24
    0.04                      // C25 (unused, but keep array aligned)
};

// Convert 64-bit integers to doubles using a magic constant trick.
static inline __m128d int_to_double(__m128i a) {
    // Magic constant for conversion: 2^52 + 2^51
    const __m128i MAGIC = _mm_set1_epi64x(0x4330000000000000LL);
    __m128i t = _mm_add_epi64(a, MAGIC);
    return _mm_castsi128_pd(t);
}

// Compute natural log for two doubles.
__m128d my_log2(__m128d a) {
    // Bit masks and constants
    const __m128i SIGN_MASK = _mm_set1_epi64x(0x8000000000000000LL);
    const __m128i EXP_MASK  = _mm_set1_epi64x(0x7FF0000000000000LL);
    const __m128i MAN_MASK  = _mm_set1_epi64x(0x000FFFFFFFFFFFFFLL);
    const __m128d ONE       = _mm_set1_pd(1.0);
    const __m128d HALF      = _mm_set1_pd(0.5);
    const __m128d SQRT2     = _mm_set1_pd(1.4142135623730951);
    const __m128d LN2_HI    = _mm_set1_pd(0.6931471803691238);
    const __m128d LN2_LO    = _mm_set1_pd(-4.319965779496036e-17);

    // Extract exponent and mantissa bits
    __m128i a_bits = _mm_castpd_si128(a);
    __m128i exponent_bits = _mm_and_si128(a_bits, EXP_MASK);
    __m128i mantissa_bits = _mm_and_si128(a_bits, MAN_MASK);

    // Build normalized mantissa: set exponent to 0x3FF (i.e., 1023)
    __m128i normalized_bits = _mm_or_si128(mantissa_bits, _mm_set1_epi64x(0x3FF0000000000000LL));
    __m128d m = _mm_castsi128_pd(normalized_bits);

    // Threshold check: if m > sqrt(2), halve m and increment exponent
    __m128d mask = _mm_cmpgt_pd(m, SQRT2);
    __m128d m_halved = _mm_mul_pd(m, HALF);
    m = _mm_blendv_pd(m, m_halved, mask);

    // Compute exponent as integer: e = (exponent_bits >> 52) - 1023 + (mask? 1 : 0)
    __m128i e_int = _mm_srli_epi64(exponent_bits, 52);
    e_int = _mm_sub_epi64(e_int, _mm_set1_epi64x(1023));
    __m128i e_inc = _mm_and_si128(mask, _mm_set1_epi64x(1));
    e_int = _mm_add_epi64(e_int, e_inc);
    __m128d e = int_to_double(e_int);

    // Compute x = m - 1
    __m128d x = _mm_sub_pd(m, ONE);

    // Polynomial evaluation using Estrin for highest 16 terms (coefficients 10..25)
    // We'll evaluate grouped pairs.
    __m128d c9  = _mm_set1_pd(LOG_COEFFS[9]);
    __m128d c10 = _mm_set1_pd(LOG_COEFFS[10]);
    __m128d c11 = _mm_set1_pd(LOG_COEFFS[11]);
    __m128d c12 = _mm_set1_pd(LOG_COEFFS[12]);
    __m128d c13 = _mm_set1_pd(LOG_COEFFS[13]);
    __m128d c14 = _mm_set1_pd(LOG_COEFFS[14]);
    __m128d c15 = _mm_set1_pd(LOG_COEFFS[15]);
    __m128d c16 = _mm_set1_pd(LOG_COEFFS[16]);
    __m128d c17 = _mm_set1_pd(LOG_COEFFS[17]);
    __m128d c18 = _mm_set1_pd(LOG_COEFFS[18]);
    __m128d c19 = _mm_set1_pd(LOG_COEFFS[19]);
    __m128d c20 = _mm_set1_pd(LOG_COEFFS[20]);
    __m128d c21 = _mm_set1_pd(LOG_COEFFS[21]);
    __m128d c22 = _mm_set1_pd(LOG_COEFFS[22]);
    __m128d c23 = _mm_set1_pd(LOG_COEFFS[23]);
    __m128d c24 = _mm_set1_pd(LOG_COEFFS[24]);

    __m128d z9  = _mm_fmadd_pd(c10, x, c9);
    __m128d z11 = _mm_fmadd_pd(c12, x, c11);
    __m128d z13 = _mm_fmadd_pd(c14, x, c13);
    __m128d z15 = _mm_fmadd_pd(c16, x, c15);
    __m128d z17 = _mm_fmadd_pd(c18, x, c17);
    __m128d z19 = _mm_fmadd_pd(c20, x, c19);
    __m128d z21 = _mm_fmadd_pd(c22, x, c21);
    __m128d z23 = _mm_fmadd_pd(c24, x, c23);

    __m128d x2 = _mm_mul_pd(x, x);
    z9  = _mm_fmadd_pd(z11, x2, z9);
    z13 = _mm_fmadd_pd(z15, x2, z13);
    z17 = _mm_fmadd_pd(z19, x2, z17);
    z21 = _mm_fmadd_pd(z23, x2, z21);

    __m128d x4 = _mm_mul_pd(x2, x2);
    z9  = _mm_fmadd_pd(z13, x4, z9);
    z17 = _mm_fmadd_pd(z21, x4, z17);

    __m128d x8 = _mm_mul_pd(x4, x4);
    z9 = _mm_fmadd_pd(z17, x8, z9);

    // Estrin for next 8 terms (coefficients 2..9)
    __m128d c2 = _mm_set1_pd(LOG_COEFFS[2]);
    __m128d c3 = _mm_set1_pd(LOG_COEFFS[3]);
    __m128d c4 = _mm_set1_pd(LOG_COEFFS[4]);
    __m128d c5 = _mm_set1_pd(LOG_COEFFS[5]);
    __m128d c6 = _mm_set1_pd(LOG_COEFFS[6]);
    __m128d c7 = _mm_set1_pd(LOG_COEFFS[7]);
    __m128d c8 = _mm_set1_pd(LOG_COEFFS[8]);

    __m128d z8 = _mm_fmadd_pd(z9, x, c8);
    __m128d z6 = _mm_fmadd_pd(c7, x, c6);
    __m128d z4 = _mm_fmadd_pd(c5, x, c4);
    __m128d z2 = _mm_fmadd_pd(c3, x, c2);

    z6 = _mm_fmadd_pd(z8, x2, z6);
    z2 = _mm_fmadd_pd(z4, x2, z2);
    __m128d z = _mm_fmadd_pd(z6, x4, z2);

    // Horner for constant term (C1 = 1.0) and multiply by x
    __m128d c1 = _mm_set1_pd(LOG_COEFFS[1]);
    z = _mm_fmadd_pd(z, x, c1);
    z = _mm_mul_pd(z, x);

    // Add e * ln2 parts
    z = _mm_fmadd_pd(e, LN2_HI, z);
    z = _mm_fmadd_pd(e, LN2_LO, z); // add low part

    // Special cases: 0 -> -inf, negative -> NaN, inf/nan -> inf/nan
    __m128d zero = _mm_setzero_pd();
    __m128d neg_inf = _mm_set1_pd(-INFINITY);
    __m128d nan = _mm_set1_pd(NAN);

    // Detect inf and nan: exponent all ones
    __m128i exp_all_ones = _mm_cmpeq_epi64(exponent_bits, EXP_MASK);
    __m128i non_positive_bits = _mm_castpd_si128(_mm_cmple_pd(a, zero));
    __m128i special_mask = _mm_or_si128(exp_all_ones, non_positive_bits);

    // For inf/nan, return a itself (since a+a gives inf/nan)
    __m128d inf_nan_result = _mm_add_pd(a, a);
    // For zero, return -inf
    __m128d zero_mask = _mm_castsi128_pd(_mm_cmpeq_epi64(_mm_castpd_si128(a), _mm_setzero_si128()));
    __m128d neg_mask = _mm_castsi128_pd(_mm_cmplt_epi64(_mm_castpd_si128(a), _mm_setzero_si128()));

    __m128d result = _mm_blendv_pd(z, inf_nan_result, _mm_castsi128_pd(exp_all_ones));
    result = _mm_blendv_pd(result, nan, _mm_castsi128_pd(neg_mask));
    result = _mm_blendv_pd(result, neg_inf, zero_mask);

    return result;
}
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <immintrin.h>

// Function prototype (same as in solution)
__m128d my_log2(__m128d a);

int main() {
    // Test 1: log(1) = 0
    __m128d a1 = _mm_set_pd(1.0, 1.0);
    __m128d r1 = my_log2(a1);
    double r1_arr[2];
    _mm_storeu_pd(r1_arr, r1);
    assert(std::fabs(r1_arr[0] - 0.0) < 1e-9);
    assert(std::fabs(r1_arr[1] - 0.0) < 1e-9);

    // Test 2: log(e) ≈ 1
    __m128d a2 = _mm_set_pd(M_E, M_E);
    __m128d r2 = my_log2(a2);
    double r2_arr[2];
    _mm_storeu_pd(r2_arr, r2);
    assert(std::fabs(r2_arr[0] - 1.0) < 1e-6);

    // Test 3: log(2) ≈ 0.693147
    __m128d a3 = _mm_set_pd(2.0, 2.0);
    __m128d r3 = my_log2(a3);
    double r3_arr[2];
    _mm_storeu_pd(r3_arr, r3);
    assert(std::fabs(r3_arr[0] - 0.6931471805599453) < 1e-6);

    // Test 4: log(0) = -inf
    __m128d a4 = _mm_set_pd(0.0, 0.0);
    __m128d r4 = my_log2(a4);
    double r4_arr[2];
    _mm_storeu_pd(r4_arr, r4);
    assert(std::isinf(r4_arr[0]) && r4_arr[0] < 0);
    assert(std::isinf(r4_arr[1]) && r4_arr[1] < 0);

    // Test 5: log(negative) = NaN
    __m128d a5 = _mm_set_pd(-1.0, -1.0);
    __m128d r5 = my_log2(a5);
    double r5_arr[2];
    _mm_storeu_pd(r5_arr, r5);
    assert(std::isnan(r5_arr[0]));
    assert(std::isnan(r5_arr[1]));

    // Test 6: log(inf) = inf
    __m128d a6 = _mm_set_pd(INFINITY, INFINITY);
    __m128d r6 = my_log2(a6);
    double r6_arr[2];
    _mm_storeu_pd(r6_arr, r6);
    assert(std::isinf(r6_arr[0]) && r6_arr[0] > 0);
    assert(std::isinf(r6_arr[1]) && r6_arr[1] > 0);

    // Test 7: log(0.5) ≈ -0.693147
    __m128d a7 = _mm_set_pd(0.5, 0.5);
    __m128d r7 = my_log2(a7);
    double r7_arr[2];
    _mm_storeu_pd(r7_arr, r7);
    assert(std::fabs(r7_arr[0] + 0.6931471805599453) < 1e-6);

    // Test 8: log(10) ≈ 2.302585
    __m128d a8 = _mm_set_pd(10.0, 10.0);
    __m128d r8 = my_log2(a8);
    double r8_arr[2];
    _mm_storeu_pd(r8_arr, r8);
    assert(std::fabs(r8_arr[0] - 2.302585092994046) < 1e-5);

    // Test 9: mixed values with two elements
    __m128d a9 = _mm_set_pd(4.0, 0.25);
    __m128d r9 = my_log2(a9);
    double r9_arr[2];
    _mm_storeu_pd(r9_arr, r9);
    // For 0.25, log(0.25) = -log(4) ≈ -1.386294
    assert(std::fabs(r9_arr[0] - 1.3862943611198906) < 1e-6);
    assert(std::fabs(r9_arr[1] + 1.3862943611198906) < 1e-6);

    // Test 10: near 1.0
    __m128d a10 = _mm_set_pd(1.0 + 1e-8, 1.0 - 1e-8);
    __m128d r10 = my_log2(a10);
    double r10_arr[2];
    _mm_storeu_pd(r10_arr, r10);
    assert(std::fabs(r10_arr[0] - 1e-8) < 1e-12);
    assert(std::fabs(r10_arr[1] + 1e-8) < 1e-12);

    // If all tests passed
    printf("All tests passed.\n");
    return 0;
}
