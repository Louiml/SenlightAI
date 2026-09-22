Write a C++ function `roundToNearestEven(double x)` that rounds a given double-precision floating-point value to the nearest integer, using the round-half-to-even (banker’s rounding) rule: if the fractional part is exactly 0.5, the result is the nearest even integer. The function must correctly handle positive and negative values, zeros, infinities, and NaNs (return them unchanged). It must not rely on the platform's default rounding mode or on functions like `std::nearbyint` that depend on the current rounding direction. Instead, implement the rounding by manually inspecting the bit representation of the double and using a large power of two (2^52) as a rounding anchor, mimicking the logic from the provided `rint` implementation. The function must be `const`-correct and include the necessary bit-manipulation helpers.
// The solution inspects the IEEE 754 double-precision bit layout. Extract the sign bit, the biased exponent, and the mantissa. Compute the unbiased exponent `j0 = (exponent bits) - 0x3ff`. If `j0 < 0`, the number is between -1 and 1 (excluding 0). For a nonzero small number, we need to round to -1, 0, or +1. The trick is to add `TWO52[sx]` (where `sx` is 0 for positive, 1 for negative) to force rounding into the integer range, then subtract it back. For `0 <= j0 < 20`, the integer part and fractional bits span only the high word, so we mask off the fractional bits and handle the 0.5 rounding by examining the bit just below the integer rounding point. For `20 <= j0 <= 51`, the integer part spans into the low word, so we process the low word similarly. If `j0 > 51`, the number is already integral (or infinity/NaN) and we return it unchanged. We must be careful with the rounding-half-to-even rule: when the fractional part is exactly 0.5 (the rounding bit is set and all lower bits are zero), the result should go to the even integer; otherwise, if any bit below the rounding bit is set, we round away from zero. The provided code uses a clever trick with `TWO52[sx]` to naturally produce correctly rounded results after performing the necessary bit adjustments. Edge cases: zeros (return immediately), infinities/NaNs (`j0 == 0x400`), and values that already have zero fractional part (return unchanged). Time complexity is O(1), space complexity O(1).
#include <cstdint>
#include <cstring>

// Helper to extract high/low 32-bit words from a double.
static void extractWords(uint32_t& hi, uint32_t& lo, double x) {
    uint64_t bits;
    std::memcpy(&bits, &x, sizeof(bits));
    hi = static_cast<uint32_t>(bits >> 32);
    lo = static_cast<uint32_t>(bits & 0xFFFFFFFFULL);
}

// Helper to reassemble a double from high/low words.
static double insertWords(uint32_t hi, uint32_t lo) {
    uint64_t bits = (static_cast<uint64_t>(hi) << 32) | lo;
    double result;
    std::memcpy(&result, &bits, sizeof(result));
    return result;
}

// Round to nearest integer, ties to even (banker's rounding).
double roundToNearestEven(double x) {
    const double TWO52[2] = {
        4503599627370496.0,  // 2^52
        -4503599627370496.0  // -2^52
    };

    uint32_t i0, i1;
    extractWords(i0, i1, x);

    int sx = (i0 >> 31) & 1;
    int j0 = static_cast<int>((i0 >> 20) & 0x7ff) - 0x3ff;

    // Handle zero.
    if (((i0 & 0x7fffffff) | i1) == 0) return x;

    // Numbers with magnitude < 1 (exponent < 0).
    if (j0 < 0) {
        // Set the guard bit for rounding.
        i1 |= (i0 & 0x0fffff);
        i0 &= 0xfffe0000;
        i0 |= ((i1 | static_cast<uint32_t>(-static_cast<int32_t>(i1))) >> 12) & 0x80000;
        double w = TWO52[sx] + insertWords(i0, i1);
        double t = w - TWO52[sx];
        uint32_t tHi;
        extractWords(tHi, i1, t);
        return insertWords((tHi & 0x7fffffff) | (sx << 31), i1);
    }

    // Numbers with 0 <= exponent < 20 (fractional bits in high word).
    if (j0 < 20) {
        uint32_t mask = 0x000fffff >> j0;
        if (((i0 & mask) | i1) == 0) return x; // already integral
        mask >>= 1;
        if ((i0 & mask) != 0 || i1 != 0) {
            // Some bit below the 0.5 bit is set => round up (away from zero).
            if (j0 == 19) i1 = 0x40000000;
            else if (j0 == 18) i1 = 0x80000000;
            else i0 = (i0 & ~mask) | ((0x20000) >> j0);
        }
        // Otherwise exactly 0.5 -> keep even (do nothing extra).
        double w = TWO52[sx] + insertWords(i0, i1);
        return w - TWO52[sx];
    }

    // Numbers with 20 <= exponent <= 51 (fractional bits in low word or both).
    if (j0 > 51) {
        if (j0 == 0x400) return x + x; // inf or NaN
        else return x; // already integral
    }

    uint32_t mask = static_cast<uint32_t>(0xFFFFFFFFULL) >> (j0 - 20);
    if ((i1 & mask) == 0) return x; // already integral
    mask >>= 1;
    if ((i1 & mask) != 0) {
        i1 = (i1 & ~mask) | ((0x40000000) >> (j0 - 20));
    }
    double w = TWO52[sx] + insertWords(i0, i1);
    return w - TWO52[sx];
}
#include <cassert>
#include <cmath>
#include <limits>

int main() {
    // Basic integer values (no change).
    assert(roundToNearestEven(0.0) == 0.0);
    assert(roundToNearestEven(1.0) == 1.0);
    assert(roundToNearestEven(-3.0) == -3.0);

    // Halfway cases -> ties to even.
    assert(roundToNearestEven(0.5) == 0.0);   // ties to even (0)
    assert(roundToNearestEven(1.5) == 2.0);   // ties to even (2)
    assert(roundToNearestEven(2.5) == 2.0);   // ties to even (2)
    assert(roundToNearestEven(-0.5) == 0.0);  // ties to even (0)
    assert(roundToNearestEven(-1.5) == -2.0); // ties to even (-2)
    assert(roundToNearestEven(-2.5) == -2.0); // ties to even (-2)

    // Non-half fractions round normally.
    assert(roundToNearestEven(0.4) == 0.0);
    assert(roundToNearestEven(0.6) == 1.0);
    assert(roundToNearestEven(1.49) == 1.0);
    assert(roundToNearestEven(1.51) == 2.0);
    assert(roundToNearestEven(-0.4) == 0.0);
    assert(roundToNearestEven(-0.6) == -1.0);

    // Larger values, including near 2^52 boundary.
    assert(roundToNearestEven(4503599627370495.5) == 4503599627370496.0); // tie to even
    assert(roundToNearestEven(4503599627370496.5) == 4503599627370496.0); // tie to even
    assert(roundToNearestEven(-4503599627370495.5) == -4503599627370496.0);

    // Values already integral after 2^52.
    assert(roundToNearestEven(4503599627370497.0) == 4503599627370497.0);

    // Special values.
    assert(std::isinf(roundToNearestEven(std::numeric_limits<double>::infinity())));
    assert(std::isnan(roundToNearestEven(std::numeric_limits<double>::quiet_NaN())));

    // Negative zero preserved.
    assert(std::signbit(roundToNearestEven(-0.0)));

    // Very small subnormal.
    assert(roundToNearestEven(1e-300) == 0.0);
    assert(roundToNearestEven(-1e-300) == 0.0);

    return 0;
}
