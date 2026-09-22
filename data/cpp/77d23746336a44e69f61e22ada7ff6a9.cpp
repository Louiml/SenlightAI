Write a C++ function `float splitFloat(float x, float* integerPart)` that decomposes a floating-point number into its integer and fractional parts. The function must store the integer part (as a float, with the same sign as `x`) in the location pointed to by `integerPart` and return the fractional part (also with the same sign as `x`). The fractional part is defined as `x - trunc(x)`. The function must handle positive, negative, zero, and values with large magnitudes correctly. For example, `splitFloat(3.75f, &i)` should set `i = 3.0f` and return `0.75f`; for `-2.5f`, `i` should be `-2.0f` and return `-0.5f`; and for `0.0f` or `-0.0f`, both parts must be zero with the same sign as `x` (i.e., the sign bit of the integer part and fractional part should match `x`). Ensure no use of standard library math functions like `modf` or `trunc`; implement the decomposition from first principles using bit manipulation and arithmetic.
The core idea is to replicate the behavior of the C standard `modff` function. The integer part is the value truncated toward zero. A straightforward way to compute the integer part is to cast the float to a signed integer type, but this is problematic for floats with magnitude greater than the maximum representable integer (e.g., for a 32-bit int, floats beyond ~2^31) or for NaN/infinity. A robust approach is to use bit manipulation on the IEEE 754 representation. Given a 32-bit float, extract the exponent and mantissa. If the exponent is less than 0 (i.e., the number's magnitude is less than 1), the truncated integer part is zero (but preserve sign: for negative numbers, zero should be `-0.0f`). If the exponent is >= 0 and less than 23 (the number has a fractional part), we can mask out the lower bits of the mantissa that correspond to the fractional part. If the exponent is >= 23, the number has no fractional part, so the integer part is `x` itself and the fractional part is zero (preserve sign). For infinity and NaN, the function should return the input as the integer part and zero as fractional part? Actually the standard behavior is undefined for NaN and infinity, but we can choose to return `x` for integer part and `0.0f` for fractional part for infinity; for NaN, return `x` and `0.0f` or propagate NaN. We'll handle these by checking exponent bits. The algorithm: reinterpret the float as `uint32_t` bits. Extract sign (bit 31), exponent (bits 30-23), mantissa (bits 22-0). If exponent == 0xFF (NaN or Inf), set integer part = x and return 0.0f with sign? For simplicity, set integer part = x and return 0.0f. If exponent < 127 (i.e., biased exponent < 127, so unbiased exponent < 0, magnitude < 1), the integer part is zero with sign: construct `0x80000000 | (sign << 31)`? Actually zero with sign is just `sign << 31`. Return that for integer part, and return `x` as the fractional part. If exponent >= 127+23 = 150, the number is a whole number. Then integer part = x, fractional part = `0x00000000 | (sign << 31)` (i.e., signed zero). Otherwise, the fractional bits are the lower bits of the mantissa. Let `shift = 23 - (exponent - 127)`. The integer mantissa is `mantissa & (~((1 << shift) - 1))`. Reconstruct the integer part with that masked mantissa and the original sign and exponent. The fractional part is `x - integer_part` using floating-point subtraction (safe because both are finite). This gives correct results. Time complexity O(1), space O(1). Edge cases: negative zero, large numbers (exponent >= 150), numbers with magnitude < 1, and values like 0.5, -0.5, 100.0. For negative numbers, the subtraction `x - integer_part` correctly yields a negative fractional part because integer_part is also negative (e.g., -2.5 - (-2.0) = -0.5). We must ensure that for `-0.0f`, integer_part is `-0.0f` and fractional part is `-0.0f`.
#include <cstdint>
#include <cstring>

// Decompose a float into integer and fractional parts, both preserving the sign of x.
// The integer part (truncated toward zero) is stored in *integerPart, and the fractional part is returned.
// Handles positive, negative, zero, negative zero, and large-magnitude values correctly.
float splitFloat(float x, float* integerPart) {
    // Obtain the IEEE 754 bit representation of x
    uint32_t bits;
    std::memcpy(&bits, &x, sizeof(bits));
    
    // Extract fields
    uint32_t sign = bits & 0x80000000u;
    uint32_t exponent = (bits >> 23) & 0xFFu;
    uint32_t mantissa = bits & 0x7FFFFFu;
    
    // Special case: infinity or NaN
    if (exponent == 0xFFu) {
        *integerPart = x;
        // Return signed zero as fractional part
        uint32_t fracBits = sign; // exponent=0, mantissa=0, sign preserved
        float fracPart;
        std::memcpy(&fracPart, &fracBits, sizeof(fracPart));
        return fracPart;
    }
    
    // Case: magnitude < 1 (exponent < 127), so integer part is zero with sign
    if (exponent < 127u) {
        uint32_t intBits = sign; // zero exponent, zero mantissa
        float intPart;
        std::memcpy(&intPart, &intBits, sizeof(intPart));
        *integerPart = intPart;
        return x; // since x - 0 = x
    }
    
    // Case: magnitude >= 2^23 (exponent >= 150), so x is already an integer
    if (exponent >= 150u) {
        *integerPart = x;
        uint32_t fracBits = sign;
        float fracPart;
        std::memcpy(&fracPart, &fracBits, sizeof(fracPart));
        return fracPart;
    }
    
    // General case: some bits are fractional
    uint32_t shift = 23u - (exponent - 127u); // number of fractional bits
    uint32_t integerMantissa = mantissa & (~((1u << shift) - 1u));
    uint32_t intBits = sign | (exponent << 23) | integerMantissa;
    float intPart;
    std::memcpy(&intPart, &intBits, sizeof(intPart));
    *integerPart = intPart;
    
    // Fractional part is the difference (preserves sign)
    return x - intPart;
}
#include <cassert>
#include <cmath>
#include <cstring>

// Declaration of the solution function (assume it is defined above)
float splitFloat(float x, float* integerPart);

int main() {
    float intPart, fracPart;

    // Basic positive fraction
    fracPart = splitFloat(3.75f, &intPart);
    assert(intPart == 3.0f);
    assert(fracPart == 0.75f);

    // Negative fraction
    fracPart = splitFloat(-2.5f, &intPart);
    assert(intPart == -2.0f);
    assert(fracPart == -0.5f);

    // Positive integer
    fracPart = splitFloat(10.0f, &intPart);
    assert(intPart == 10.0f);
    assert(fracPart == 0.0f);

    // Negative integer
    fracPart = splitFloat(-7.0f, &intPart);
    assert(intPart == -7.0f);
    assert(fracPart == -0.0f); // sign preserved? -0.0f == 0.0f is true, but check sign bit
    {
        uint32_t bits;
        std::memcpy(&bits, &fracPart, sizeof(bits));
        assert((bits & 0x80000000u) != 0u); // must be negative zero
    }

    // Zero
    fracPart = splitFloat(0.0f, &intPart);
    assert(intPart == 0.0f);
    assert(fracPart == 0.0f);

    // Negative zero
    float negZero = -0.0f;
    fracPart = splitFloat(negZero, &intPart);
    {
        uint32_t bits;
        std::memcpy(&bits, &intPart, sizeof(bits));
        assert((bits & 0x80000000u) != 0u); // integer part is -0.0f
        std::memcpy(&bits, &fracPart, sizeof(bits));
        assert((bits & 0x80000000u) != 0u); // fractional part is -0.0f
    }

    // Small magnitude less than 1
    fracPart = splitFloat(0.25f, &intPart);
    assert(intPart == 0.0f);
    assert(fracPart == 0.25f);

    // Negative small magnitude
    fracPart = splitFloat(-0.75f, &intPart);
    assert(intPart == -0.0f);
    assert(fracPart == -0.75f);

    // Large number (no fractional part)
    float large = 16777216.0f; // 2^24, exactly representable
    fracPart = splitFloat(large, &intPart);
    assert(intPart == large);
    assert(fracPart == 0.0f);

    // Large number with fraction (e.g., 2^23 + 0.5)
    float largeFrac = 8388608.5f; // 2^23 + 0.5
    fracPart = splitFloat(largeFrac, &intPart);
    assert(intPart == 8388608.0f);
    assert(fracPart == 0.5f);

    // Negative large fraction
    fracPart = splitFloat(-8388608.5f, &intPart);
    assert(intPart == -8388608.0f);
    assert(fracPart == -0.5f);

    // Infinity (treat as integer + zero)
    float inf = INFINITY;
    fracPart = splitFloat(inf, &intPart);
    assert(intPart == inf);
    assert(fracPart == 0.0f);

    // Negative infinity
    float negInf = -INFINITY;
    fracPart = splitFloat(negInf, &intPart);
    assert(intPart == negInf);
    assert(fracPart == -0.0f); // sign preserved

    return 0;
}
