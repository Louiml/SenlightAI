Write a standalone C++ function `int32_to_fp16_array(const int* input, int length, float scale, const float* bias, bool has_bias, uint16_t* output)` that converts an array of signed 32-bit integers to an array of IEEE 754 half-precision (binary16) floating-point values. For each input element `x`, the function must compute `output[i] = (half-precision)(x * scale + (has_bias ? bias[i] : 0.0f))`. The function must handle `length` up to any positive integer, including values not divisible by 4, and must produce results equivalent to casting the computed `float` to `uint16_t` via standard float-to-half conversion (with round-to-nearest-even behavior). The output array must contain bit-exact IEEE 754 half-precision representations. The function should be self-contained, using only standard C++ (no NEON intrinsics), and must be correct for negative integers, zero, fractional scales, and positive/negative bias values. Assume the input pointer is valid and length ≥ 1.

#include <cassert>
#include <cstdint>
#include <cmath>

// Provide a reference half conversion using standard conversion (assuming hardware support for __fp16-like behavior is absent)
// For testing, we implement a simpler but correct function using bit manipulation for verification.
// Here we reuse the same float_to_half from the solution since it's deterministic.

// Test helper to compare output with expected half values converted from float.
uint16_t expected_half(float value) {
    return float_to_half(value);
}

int main() {
    // Test 1: simple positive conversion
    int input1[] = {1, 2, 3, 4};
    uint16_t out1[4];
    int32_to_fp16_array(input1, 4, 1.0f, nullptr, false, out1);
    for (int i = 0; i < 4; ++i) {
        float expected_float = static_cast<float>(input1[i]) * 1.0f;
        assert(out1[i] == expected_half(expected_float));
    }

    // Test 2: negative numbers and fractional scale
    int input2[] = {-5, -2, 0, 7};
    uint16_t out2[4];
    int32_to_fp16_array(input2, 4, 0.5f, nullptr, false, out2);
    for (int i = 0; i < 4; ++i) {
        float expected_float = static_cast<float>(input2[i]) * 0.5f;
        assert(out2[i] == expected_half(expected_float));
    }

    // Test 3: with bias (per-element)
    int input3[] = {10, -10, 5, 0};
    float bias3[] = {1.5f, -2.0f, 0.25f, 3.0f};
    uint16_t out3[4];
    int32_to_fp16_array(input3, 4, 0.1f, bias3, true, out3);
    for (int i = 0; i < 4; ++i) {
        float expected_float = static_cast<float>(input3[i]) * 0.1f + bias3[i];
        assert(out3[i] == expected_half(expected_float));
    }

    // Test 4: edge cases: zero, overflow to infinity, underflow to zero
    int input4[] = {0, 100000, -100000, 1};
    uint16_t out4[4];
    int32_to_fp16_array(input4, 4, 1000.0f, nullptr, false, out4);
    assert(out4[0] == expected_half(0.0f));
    assert(out4[1] == expected_half(100000000.0f)); // likely infinity
    assert(out4[2] == expected_half(-100000000.0f)); // negative infinity
    assert(out4[3] == expected_half(1000.0f));

    // Test 5: length not multiple of 4 (e.g., 5 elements)
    int input5[] = {1, -1, 2, -2, 3};
    uint16_t out5[5];
    int32_to_fp16_array(input5, 5, 3.0f, nullptr, false, out5);
    for (int i = 0; i < 5; ++i) {
        float expected_float = static_cast<float>(input5[i]) * 3.0f;
        assert(out5[i] == expected_half(expected_float));
    }

    // Test 6: bias of size 1 vs per-element (here has_bias true with per-element bias)
    int input6[] = {100, 200};
    float bias6[] = {0.5f, 0.5f};
    uint16_t out6[2];
    int32_to_fp16_array(input6, 2, 0.01f, bias6, true, out6);
    assert(out6[0] == expected_half(100 * 0.01f + 0.5f));
    assert(out6[1] == expected_half(200 * 0.01f + 0.5f));

    // Test 7: negative scale
    int input7[] = {1, 2, 3};
    uint16_t out7[3];
    int32_to_fp16_array(input7, 3, -2.0f, nullptr, false, out7);
    for (int i = 0; i < 3; ++i) {
        float expected_float = static_cast<float>(input7[i]) * -2.0f;
        assert(out7[i] == expected_half(expected_float));
    }

    return 0;
}

#include <cstdint>
#include <cmath>

// Convert a 32-bit float to IEEE 754 half-precision (binary16) bit pattern.
// Handles normal, subnormal, zero, infinity, and NaN with round-to-nearest-even.
uint16_t float_to_half(float value) {
    uint32_t float_bits;
    // Copy float bits into integer
    std::memcpy(&float_bits, &value, sizeof(float));
    uint32_t sign = (float_bits >> 16) & 0x8000u;
    uint32_t exponent = (float_bits >> 23) & 0xffu;
    uint32_t mantissa = float_bits & 0x7fffffu;

    // Zero or subnormal float
    if (exponent == 0) {
        if (mantissa == 0) {
            return static_cast<uint16_t>(sign);
        }
        // Float subnormal: convert to half with possible truncation/rounding
        // Find highest set bit to normalize
        int shift = 0;
        while ((mantissa & 0x00800000u) == 0) {
            mantissa <<= 1;
            shift++;
        }
        // Now mantissa has bit 23 set, exponent = 1 - shift after normalization
        int half_exp = 1 - shift;
        mantissa &= 0x007fffff; // remove leading 1
        if (half_exp <= -24) {
            // Too small to represent even as subnormal -> zero
            return static_cast<uint16_t>(sign);
        }
        if (half_exp < -14) {
            // Subnormal half: shift mantissa appropriately
            int extra_shift = -14 - half_exp;
            uint32_t round_bit = 1u << (extra_shift - 1);
            uint32_t tie = round_bit << 1;
            uint32_t half_mant = mantissa >> extra_shift;
            uint32_t rem = mantissa & (tie - 1);
            if (rem > round_bit) half_mant++;
            else if (rem == round_bit && (half_mant & 1)) half_mant++;
            return static_cast<uint16_t>(sign | half_mant);
        }
        // Normal half after normalization
        uint32_t half_exp_bits = static_cast<uint32_t>(half_exp + 15);
        return static_cast<uint16_t>(sign | (half_exp_bits << 10) | (mantissa >> 13));
    }

    // Infinity or NaN
    if (exponent == 0xff) {
        if (mantissa == 0) {
            return static_cast<uint16_t>(sign | 0x7c00u); // infinity
        }
        return static_cast<uint16_t>(sign | 0x7e00u); // NaN (keep most significant mantissa bit)
    }

    // Normal float: adjust exponent and round mantissa
    int half_exp = static_cast<int>(exponent) - 127 + 15;
    if (half_exp >= 0x1f) {
        return static_cast<uint16_t>(sign | 0x7c00u); // overflow to infinity
    }
    if (half_exp <= 0) {
        // Underflow to subnormal or zero
        if (half_exp < -10) {
            return static_cast<uint16_t>(sign);
        }
        // Subnormal half
        int shift = 1 - half_exp;
        uint32_t half_mant = (mantissa | 0x00800000u) >> shift;
        uint32_t round_bit = 1u << (shift - 1);
        uint32_t tie = round_bit << 1;
        uint32_t rem = (mantissa | 0x00800000u) & (tie - 1);
        if (rem > round_bit) half_mant++;
        else if (rem == round_bit && (half_mant & 1)) half_mant++;
        return static_cast<uint16_t>(sign | half_mant);
    }

    uint32_t half_mant = mantissa >> 13;
    uint32_t round_bit = 0x1000u;
    uint32_t tie = 0x2000u;
    uint32_t rem = mantissa & (tie - 1);
    if (rem > round_bit) half_mant++;
    else if (rem == round_bit && (half_mant & 1)) half_mant++;
    if (half_mant == 0x400) {
        half_mant = 0;
        half_exp++;
        if (half_exp >= 0x1f) {
            return static_cast<uint16_t>(sign | 0x7c00u);
        }
    }
    return static_cast<uint16_t>(sign | (static_cast<uint32_t>(half_exp) << 10) | half_mant);
}

// Convert an array of int32 values to half-precision using scale and optional bias.
void int32_to_fp16_array(const int* input, int length, float scale, const float* bias, bool has_bias, uint16_t* output) {
    for (int i = 0; i < length; ++i) {
        float val = static_cast<float>(input[i]) * scale;
        if (has_bias) {
            val += bias[i];
        }
        output[i] = float_to_half(val);
    }
}

// The core task is a per-element linear transformation followed by a conversion from 32-bit float to 16-bit half-precision. Since the problem specifies a scalar (non-vectorized) implementation, we loop from `i = 0` to `length - 1`. For each element, we compute `float val = static_cast<float>(input[i]) * scale + (has_bias ? bias[i] : 0.0f)`. The conversion to half-precision is the trickiest part: a `uint16_t` must be generated with the correct IEEE 754 binary16 format. We implement this conversion manually without relying on hardware `_Float16` (since that is non-standard). A robust approach is to use the standard bit-manipulation algorithm for `float` to `half` conversion. Key steps: extract sign, exponent, and mantissa from the float32 bit pattern; handle special cases (zero, NaN, infinity, and subnormals); round the mantissa to 10 bits using round-to-nearest-even; and pack the result. Alternative simpler method is to use `std::frexp` and `std::ldexp`, but bit manipulation is more direct. Edge cases: very large values overflow to infinity, very small values underflow to zero or subnormal, and negative numbers set the sign bit. The algorithm is O(n) time and O(1) auxiliary space, with the float-to-half conversion being O(1) per element.
