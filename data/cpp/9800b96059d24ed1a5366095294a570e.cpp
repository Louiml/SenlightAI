// Write a standalone C++ function named `roundToNearestEven` that takes a single `float` argument and returns a `float` rounded to the nearest integer, with ties (fractional part exactly 0.5) rounded to the nearest even integer (banker’s rounding). The function must not use any library rounding functions (like `std::round`, `std::nearbyint`, etc.) or assembly; instead, implement the rounding manually using arithmetic and bit-level operations. Handle all edge cases: positive and negative numbers, values whose magnitude is less than 0.5 (round to +0 or -0), values with large magnitudes beyond the precision where fractional parts are lost (should return the input unchanged), and infinities or NaN (should return the input unchanged). The result must be exactly representable as a `float` with the correct sign for zero.
// The key idea is to manipulate the IEEE-754 single-precision representation directly. A `float` consists of a sign bit, an 8-bit biased exponent, and a 23-bit fraction. To round to nearest even: first, identify the position of the round bit (the highest bit of the fractional part that will be discarded) and the sticky information (whether any lower bits are set). If the exponent is such that the float has no fractional bits (e.g., exponent >= 127 for normal numbers, or too large), return `x` unchanged. For numbers with a fractional part, compute the integer part by truncating the fraction bits, then examine the discarded bits to decide rounding: if the discarded part is > 0.5, round up; if exactly 0.5, round to even (i.e., if the least significant bit of the truncated integer part is 1, round up; else truncate). For negative numbers, the same rules apply (rounding towards nearest even, not towards zero or away). This can be done by taking the absolute value (clearing the sign bit), performing the rounding on the magnitude, and then reapplying the sign (while preserving -0 if the result is zero and the input was negative). Special cases: if the exponent is all ones (infinity/NaN) or the input is zero, return as-is. If the exponent is very small (denormal or small), the number may have no integer part; handle by checking if the absolute value is < 0.5 and rounding to ±0. The algorithm runs in O(1) time and O(1) space, as it uses a fixed set of bit operations.
#include <cstdint>
#include <cstring>

// Round a float to the nearest integer, with ties rounding to even (banker's rounding).
// Handles negative numbers, zero signs, denormals, infinities, and NaN.
float roundToNearestEven(float x) {
    uint32_t bits;
    std::memcpy(&bits, &x, sizeof(bits));

    uint32_t sign = bits & 0x80000000u;
    uint32_t exp = (bits >> 23) & 0xFFu;
    uint32_t frac = bits & 0x007FFFFFu;

    // Special: NaN, infinity, exact zero
    if (exp == 0xFFu || (exp == 0u && frac == 0u)) {
        return x;
    }

    // Work on magnitude for simplicity, preserve sign later.
    uint32_t mag_bits = bits & 0x7FFFFFFFu;
    float mag;
    std::memcpy(&mag, &mag_bits, sizeof(mag));

    // If magnitude < 0.5, round to ±0
    if (mag < 0.5f) {
        uint32_t zero_bits = sign;
        float result;
        std::memcpy(&result, &zero_bits, sizeof(result));
        return result;
    }

    // If magnitude is already an integer (no fractional part), return x
    // This happens when exp >= 127 + 23? Actually for exp >= 150, fraction bits are all integer bits.
    // But we can check using truncation: if mag == truncf(mag) but we avoid truncf.
    // Bit check: for normal numbers, if exp >= 150, then shift <= 0 => no fractional bits.
    if (exp >= 150u) {
        return x;
    }

    // Number of fractional bits after the binary point in the fraction field.
    // For a normal number with exponent e (bias 127), the fraction field represents bits of 1.fraction.
    // The integer part includes the implicit leading 1 plus (e-127) bits from the fraction.
    // The remaining 23 - (e-127) lower fraction bits are fractional.
    int shift = 23 - static_cast<int>(exp - 127u);
    // shift >= 1 here (since exp < 150)
    uint32_t frac_mask = (1u << shift) - 1u;
    uint32_t discarded = frac & frac_mask;
    uint32_t truncated_frac = frac & ~frac_mask;

    // If nothing discarded, it's an integer
    if (discarded == 0u) {
        return x;
    }

    // Determine if we need to round up.
    // Check the round bit (highest bit of discarded) and sticky bits.
    uint32_t round_bit = (discarded >> (shift - 1u)) & 1u;
    uint32_t sticky_mask = (shift >= 2u) ? ((1u << (shift - 1u)) - 1u) : 0u;
    uint32_t sticky = discarded & sticky_mask;
    bool round_up = false;

    if (round_bit == 1u) {
        if (sticky != 0u) {
            round_up = true; // > 0.5
        } else {
            // Exactly 0.5: round to even
            // The integer part's LSB is the bit right above the round bit, if it exists.
            // For shift >= 1, the integer LSB is the bit at position shift (0-indexed from LSB of fraction field)
            // but careful: the fraction field's bit 0 is the least significant fraction bit.
            // The integer part includes bits from position shift to 23 (the implicit leading 1 is above 23).
            // The LSB of the integer part is the bit at position shift (if shift <= 23).
            // Since shift <= 23 here, it's at position shift.
            uint32_t int_lsb = (truncated_frac >> shift) & 1u;
            // For shift == 23? That happens when exp == 127, then truncated_frac = frac & ~( (1<<23)-1 ) = 0, and the integer LSB is the implicit 1 (which is at bit 23), but int_lsb would be (0 >> 23) which is 0. Actually we need to include the implicit leading 1. For exp == 127, the integer part is exactly 1 (since frac is all fractional). Then LSB is 1. So we must handle when shift == 23: trunc_frac=0, and the integer part is 1, which is odd. So we can compute integer part value and check parity.
            // Simpler: compute the integer part as unsigned (fits in 32 bits because exp <= 149).
            uint32_t integer_part = (1u << (exp - 127u)) | truncated_frac;
            round_up = (integer_part & 1u) == 1u;
        }
    }

    if (!round_up) {
        // Return the truncated magnitude with original sign
        uint32_t result_bits = sign | (exp << 23) | truncated_frac;
        float result;
        std::memcpy(&result, &result_bits, sizeof(result));
        return result;
    }

    // Round up: add 1 to the integer part
    // Build the integer part value
    uint32_t integer_part = (1u << (exp - 127u)) | truncated_frac;
    integer_part += 1u;

    // Convert back to float bits: find exponent and fraction from integer_part
    // The integer_part is at least 2 and fits in 32 bits.
    // Determine the highest set bit to compute new exponent.
    uint32_t msb = 31u - __builtin_clz(integer_part); // index of highest bit
    // The new exponent = 127 + msb
    uint32_t new_exp = 127u + msb;
    // The fraction bits are the lower 23 bits after removing the leading 1.
    uint32_t new_frac = integer_part & ((1u << 23) - 1u);

    // If new_exp is 0xFF, that would be overflow to infinity, but that can't happen for our range.
    uint32_t result_bits = sign | (new_exp << 23) | new_frac;
    float result;
    std::memcpy(&result, &result_bits, sizeof(result));
    return result;
}
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

// Include the solution function declaration here (the above function).

int main() {
    // Basic positive rounding
    assert(roundToNearestEven(0.5f) == 0.0f);   // tie -> even
    assert(roundToNearestEven(1.5f) == 2.0f);   // tie -> even
    assert(roundToNearestEven(2.5f) == 2.0f);   // tie -> even
    assert(roundToNearestEven(3.5f) == 4.0f);   // tie -> even
    assert(roundToNearestEven(1.4f) == 1.0f);
    assert(roundToNearestEven(1.6f) == 2.0f);
    assert(roundToNearestEven(-0.5f) == -0.0f); // tie -> even (0 is even)
    assert(roundToNearestEven(-1.5f) == -2.0f); // tie -> even
    assert(roundToNearestEven(-2.5f) == -2.0f); // tie -> even

    // Exact integers unchanged
    assert(roundToNearestEven(0.0f) == 0.0f);
    assert(roundToNearestEven(-0.0f) == -0.0f);
    assert(roundToNearestEven(3.0f) == 3.0f);
    assert(roundToNearestEven(-3.0f) == -3.0f);

    // Values near half
    assert(roundToNearestEven(0.4999999f) == 0.0f);
    assert(roundToNearestEven(0.5000001f) == 1.0f);
    assert(roundToNearestEven(-0.4999999f) == -0.0f);
    assert(roundToNearestEven(-0.5000001f) == -1.0f);

    // Large values (no fractional part representable)
    float large = 16777216.0f; // 2^24, integer
    assert(roundToNearestEven(large) == large);
    float large_frac = 16777217.0f; // not representable, remains 16777216
    assert(roundToNearestEven(large_frac) == large_frac);

    // Infinities and NaN
    float inf = std::numeric_limits<float>::infinity();
    float nan = std::numeric_limits<float>::quiet_NaN();
    assert(roundToNearestEven(inf) == inf);
    assert(roundToNearestEven(-inf) == -inf);
    assert(std::isnan(roundToNearestEven(nan)));

    // Denormal small values
    float denorm = 1e-40f; // subnormal
    assert(roundToNearestEven(denorm) == 0.0f);
    assert(roundToNearestEven(-denorm) == -0.0f);

    // Sign preservation for zero
    float neg_zero = -0.0f;
    float result = roundToNearestEven(neg_zero);
    uint32_t bits;
    std::memcpy(&bits, &result, sizeof(bits));
    assert((bits & 0x80000000u) != 0); // sign bit set

    return 0;
}
