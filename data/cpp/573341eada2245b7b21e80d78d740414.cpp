// Implement a C++ function `double mySqrt(double x)` that computes the square root of a double-precision floating-point number using the bit-by-bit (digit-by-digit) integer algorithm described in the given code snippet. The function must handle special cases correctly: return `x` unchanged for `+0.0`, `-0.0`, and positive infinity; return a quiet NaN for negative inputs (including negative infinity); return NaN if the input is NaN. For normal and subnormal positive inputs, the result must be correctly rounded to the nearest double (round-to-nearest-even in case of a tie, though ties cannot occur for square root). You may use bit manipulation on the IEEE-754 bit representation of doubles. Do not use the standard `sqrt` function or any hardware sqrt instruction in the implementation. Provide the function in a self-contained manner with only standard headers (e.g., `<cstdint>`, `<cmath>` for `NAN` if needed). The function must be deterministic and portable across platforms that use IEEE-754 double format.

// The solution follows the classic portable square root algorithm. The core idea is to normalize the input `x` into a value `y` in the range `[1,4)` by multiplying or dividing by powers of 4 (i.e., even powers of 2). Because sqrt(2^(2k) * y) = 2^k * sqrt(y), we can compute sqrt of the scaled number and then adjust the exponent by shifting by `k`. The algorithm extracts the sign, exponent, and mantissa bits from the IEEE-754 representation using bit shifts. For subnormal numbers (exponent field zero), we first normalize by shifting the mantissa until the implicit leading bit appears. Then, we adjust the exponent and perform the bit-by-bit computation. The bit-by-bit process maintains two 32-bit words representing the current partial square root `q` and the remainder `s`. For each bit position from the most significant down to the least significant, we test whether adding the candidate bit to the square root still keeps the squared value ≤ the original normalized mantissa; if so, we update the remainder and the square root. After generating 53 bits (the full precision of a double mantissa), we perform final rounding. The rounding logic uses a floating-point addition trick: adding tiny to one to detect the rounding direction (round-to-nearest). If the remainder is non-zero, we determine whether to round up or down based on the last bit and the remainder's magnitude. The special cases are handled upfront: if the exponent field is all ones (Inf or NaN), we return `x*x + x`, which yields NaN for NaN and Inf for +Inf, and NaN for -Inf. If `x` is zero, we return `x` unchanged. If `x` is negative (not NaN), we return a NaN (using `(x-x)/(x-x)`). The time complexity is O(1) as it processes a fixed number of bits (53) per call. Space complexity is O(1) auxiliary.

#include <cstdint>
#include <cstring>

// Compute the square root of a double using a portable bit-by-bit algorithm.
// Handles special cases: +0, -0, +inf, NaN, and negative inputs.
double mySqrt(double x) {
    // Extract the bit representation.
    uint64_t bits;
    std::memcpy(&bits, &x, sizeof(bits));
    
    // Sign bit.
    int sign = (bits >> 63) & 1;
    // Exponent bits (11 bits).
    int exponent = (bits >> 52) & 0x7FF;
    // Mantissa bits (52 bits).
    uint64_t mantissa = bits & 0xFFFFFFFFFFFFFULL;

    // Special cases: NaN or Inf.
    if (exponent == 0x7FF) {
        // If mantissa != 0, it's NaN; if mantissa == 0, it's Inf.
        // For +inf, return +inf. For NaN, return NaN (x*x+x produces NaN for NaN).
        // For -inf, x*x+x is NaN.
        return x * x + x;
    }
    // Zero.
    if (exponent == 0 && mantissa == 0) {
        return x; // +0 or -0
    }
    // Negative non-zero.
    if (sign) {
        return (x - x) / (x - x); // NaN
    }

    // Normalize x to [1,4) by adjusting the exponent.
    // We'll work with the mantissa as an integer with an implicit leading 1.
    // For subnormals, we shift until we get a leading 1.
    int m = exponent;
    if (m == 0) {
        // Subnormal: normalize.
        m = 1; // The effective exponent is 1 - 1023 = -1022.
        while ((mantissa & (1ULL << 52)) == 0) {
            mantissa <<= 1;
            m--;
        }
        mantissa <<= 1; // Shift to align with normal numbers? Actually we want the leading 1 at bit 52.
        // After the loop, mantissa has a leading 1 at bit 52? Let's adjust: 
        // The normalization loop shifts until bit 52 is set, but we might have shifted too far.
        // Simpler: use the standard approach from the snippet: use two 32-bit words.
        // To keep this solution manageable, we'll use a simpler fixed-point approach.
        // Let's restart with a cleaner implementation using 64-bit integer arithmetic.
    }
    
    // Instead of the above partial approach, let's implement a robust version.
    // Re-extract bits cleanly.
    // Actually, we can use the classic algorithm with 32-bit words, but that's verbose.
    // Let's use a simpler but correct approach: use the algorithm in the snippet directly.
    // However, the snippet relies on helper macros __JHI/__JLO, which we'll emulate.

    // Define helper to get high and low 32-bit words.
    auto get_hi = [](uint64_t b) -> uint32_t { return (uint32_t)(b >> 32); };
    auto get_lo = [](uint64_t b) -> uint32_t { return (uint32_t)(b & 0xFFFFFFFFULL); };
    auto make_double = [](uint32_t hi, uint32_t lo) -> double {
        uint64_t b = ((uint64_t)hi << 32) | (uint64_t)lo;
        double d;
        std::memcpy(&d, &b, sizeof(d));
        return d;
    };

    int ix0 = (int)get_hi(bits);
    int ix1 = (int)get_lo(bits);
    const int signMask = 0x80000000;

    // Special cases already handled above, but we re-check for consistency.
    if ((ix0 & 0x7ff00000) == 0x7ff00000) {
        return x * x + x;
    }
    if (ix0 <= 0) {
        if (((ix0 & (~signMask)) | ix1) == 0) {
            return x;
        } else if (ix0 < 0) {
            return (x - x) / (x - x);
        }
    }

    // Normalize.
    int m_exp;
    m_exp = (ix0 >> 20);
    if (m_exp == 0) {
        while (ix0 == 0) {
            m_exp -= 21;
            ix0 |= (ix1 >> 11);
            ix1 <<= 21;
        }
        int i;
        for (i = 0; (ix0 & 0x00100000) == 0; i++) {
            ix0 <<= 1;
        }
        m_exp -= i - 1;
        ix0 |= (ix1 >> (32 - i));
        ix1 <<= i;
    }
    m_exp -= 1023;
    ix0 = (ix0 & 0x000fffff) | 0x00100000;
    if (m_exp & 1) {
        ix0 += ix0 + ((ix1 & signMask) >> 31);
        ix1 += ix1;
    }
    m_exp >>= 1;

    // Bit-by-bit computation.
    ix0 += ix0 + ((ix1 & signMask) >> 31);
    ix1 += ix1;
    unsigned q = 0, q1 = 0, s0 = 0, s1 = 0;
    unsigned r, t1, s1_tmp;
    int t;
    r = 0x00200000;
    while (r != 0) {
        t = (int)(s0 + r);
        if (t <= ix0) {
            s0 = (unsigned)(t + r);
            ix0 -= t;
            q += r;
        }
        ix0 += ix0 + ((ix1 & signMask) >> 31);
        ix1 += ix1;
        r >>= 1;
    }

    r = signMask;
    while (r != 0) {
        t1 = s1 + r;
        t = (int)s0;
        if ((t < ix0) || ((t == ix0) && (t1 <= (unsigned)ix1))) {
            s1_tmp = t1 + r;
            if ((((int)t1 & signMask) == signMask) && ((int)(s1_tmp & signMask) == 0)) {
                s0 += 1;
            }
            ix0 -= t;
            if ((unsigned)ix1 < t1) {
                ix0 -= 1;
            }
            ix1 -= (int)t1;
            q1 += r;
        }
        ix0 += ix0 + ((ix1 & signMask) >> 31);
        ix1 += ix1;
        r >>= 1;
    }

    // Rounding.
    double one = 1.0;
    double tiny = 1.0e-300;
    double z;
    if ((ix0 | ix1) != 0) {
        z = one - tiny; // trigger inexact
        if (z >= one) {
            z = one + tiny;
            if (q1 == 0xffffffffU) {
                q1 = 0;
                q += 1;
            } else if (z > one) {
                if (q1 == 0xfffffffeU) {
                    q += 1;
                }
                q1 += 2;
            } else {
                q1 += (q1 & 1);
            }
        }
    }
    int hi = (q >> 1) + 0x3fe00000;
    int lo = (int)(q1 >> 1);
    if ((q & 1) == 1) {
        lo |= signMask;
    }
    hi += (m_exp << 20);
    return make_double((uint32_t)hi, (uint32_t)lo);
}

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>

int main() {
    // Special cases
    assert(mySqrt(0.0) == 0.0);
    assert(mySqrt(-0.0) == -0.0);
    assert(mySqrt(std::numeric_limits<double>::infinity()) == std::numeric_limits<double>::infinity());
    assert(std::isnan(mySqrt(-1.0)));
    assert(std::isnan(mySqrt(-std::numeric_limits<double>::infinity())));
    assert(std::isnan(mySqrt(std::numeric_limits<double>::quiet_NaN())));

    // Perfect squares
    assert(mySqrt(1.0) == 1.0);
    assert(mySqrt(4.0) == 2.0);
    assert(mySqrt(9.0) == 3.0);
    assert(mySqrt(16.0) == 4.0);
    assert(mySqrt(0.25) == 0.5);
    assert(mySqrt(100.0) == 10.0);

    // Non-perfect squares (within 1 ulp)
    double x = 2.0;
    double result = mySqrt(x);
    double expected = std::sqrt(x);
    assert(std::fabs(result - expected) <= std::numeric_limits<double>::epsilon() * std::fabs(expected));

    x = 3.0;
    result = mySqrt(x);
    expected = std::sqrt(x);
    assert(std::fabs(result - expected) <= std::numeric_limits<double>::epsilon() * std::fabs(expected));

    x = 0.5;
    result = mySqrt(x);
    expected = std::sqrt(x);
    assert(std::fabs(result - expected) <= std::numeric_limits<double>::epsilon() * std::fabs(expected));

    // Subnormal
    x = std::numeric_limits<double>::denorm_min();
    result = mySqrt(x);
    expected = std::sqrt(x);
    assert(std::fabs(result - expected) <= std::numeric_limits<double>::epsilon() * std::fabs(expected));

    // Large/small
    x = 1e300;
    result = mySqrt(x);
    expected = std::sqrt(x);
    assert(std::fabs(result - expected) <= std::numeric_limits<double>::epsilon() * std::fabs(expected));

    x = 1e-300;
    result = mySqrt(x);
    expected = std::sqrt(x);
    assert(std::fabs(result - expected) <= std::numeric_limits<double>::epsilon() * std::fabs(expected));

    // Random-ish values
    for (int i = 1; i <= 100; ++i) {
        double val = i * 0.1;
        result = mySqrt(val);
        expected = std::sqrt(val);
        assert(std::fabs(result - expected) <= std::numeric_limits<double>::epsilon() * std::fabs(expected));
    }

    return 0;
}
