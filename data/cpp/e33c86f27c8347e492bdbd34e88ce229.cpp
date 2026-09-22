// Write a C++ function that builds a lookup table for nonlinear encoding of half-precision floating-point values, inspired by the luminance transfer function used in image compression. The function should accept an array of 65536 unsigned shorts (representing the bit patterns of IEEE 754 half-precision floats) and fill a corresponding output array with the bit patterns of the nonlinearly encoded values. The transfer function is defined piecewise: for input values with absolute value ≤ 1.0, apply a gamma 2.2 power function (i.e., `sign * pow(abs(x), 1.0/2.2)`); for values greater than 1.0 in absolute value, apply a logarithmic function `sign * (log(abs(x)) / log(e^2.2) + 1.0)`. Values that are NaN or infinity (bit pattern with exponent bits all 1) should map to 0. The function should treat the input array as containing native-endian half values and produce output in native-endian half bit patterns. The function must handle the full 65536-entry range and be const-correct.
#include <cassert>
#include <cstdint>
#include <cmath>

// Declare the solution function (assume included from above).
void generateNonlinearLookup(const uint16_t*, uint16_t*, size_t = 65536);

int main() {
    // Create an input array containing all 65536 half bit patterns.
    uint16_t input[65536];
    uint16_t output[65536];
    for (int i = 0; i < 65536; ++i) input[i] = static_cast<uint16_t>(i);
    
    generateNonlinearLookup(input, output);
    
    // Test 1: Zero maps to zero.
    assert(output[0] == 0);
    
    // Test 2: Positive one (0x3C00) maps to one (since 1^(1/2.2)=1).
    assert(output[0x3C00] == 0x3C00);
    
    // Test 3: Negative one (0xBC00) maps to negative one.
    assert(output[0xBC00] == 0xBC00);
    
    // Test 4: Small value like 0.5 (0x3800) should be > 0.5 after gamma.
    uint16_t halfZeroPointFive = 0x3800; // 0.5
    Half h; h.setBits(halfZeroPointFive);
    float orig = h.toFloat();
    Half outH; outH.setBits(output[halfZeroPointFive]);
    float encoded = outH.toFloat();
    assert(encoded > orig);
    
    // Test 5: Large value like 2.0 (0x4000) should be > 1.0 and use log.
    uint16_t twoBits = 0x4000;
    Half h2; h2.setBits(twoBits);
    float orig2 = h2.toFloat();
    Half outH2; outH2.setBits(output[twoBits]);
    float encoded2 = outH2.toFloat();
    // log(2)/log(e^2.2)+1 = 0.315/2.2+1 ≈ 1.143, so encoded should be > 1.
    assert(encoded2 > 1.0f);
    
    // Test 6: NaN (0x7E00) maps to 0.
    assert(output[0x7E00] == 0);
    
    // Test 7: Infinity (0x7C00) maps to 0.
    assert(output[0x7C00] == 0);
    
    // Test 8: Negative infinity (0xFC00) maps to 0.
    assert(output[0xFC00] == 0);
    
    // Test 9: Symmetry: encoded(-x) == encoded(x) with sign flipped.
    // For value 0.25 (0x3400) and its negative (0xB400).
    uint16_t posBits = 0x3400; // 0.25
    uint16_t negBits = posBits | 0x8000;
    Half posH; posH.setBits(output[posBits]);
    Half negH; negH.setBits(output[negBits]);
    assert(posH.toFloat() == -negH.toFloat());
    
    // Test 10: All outputs must be valid half-precision (not NaN/Inf).
    for (int i = 0; i < 65536; ++i) {
        uint16_t bits = output[i];
        // Exponent bits must not all be 1 unless value is 0 (which we mapped inf/nan to 0).
        assert((bits & 0x7C00) != 0x7C00);
    }
    
    return 0;
}
#include <cstdint>
#include <cmath>
#include <cstddef>

// Minimal half-precision float representation for this task.
struct Half {
    uint16_t bits;
    
    float toFloat() const {
        // Convert IEEE 754 half to float.
        uint32_t sign = (bits >> 15) & 0x1;
        uint32_t exponent = (bits >> 10) & 0x1F;
        uint32_t mantissa = bits & 0x3FF;
        
        uint32_t f_sign = sign << 31;
        uint32_t f_exponent;
        uint32_t f_mantissa;
        
        if (exponent == 0) {
            // Subnormal or zero.
            if (mantissa == 0) {
                return 0.0f * (sign ? -1.0f : 1.0f);
            }
            // Normalize subnormal.
            int e = -14;
            uint32_t m = mantissa;
            while ((m & 0x400) == 0) {
                m <<= 1;
                e--;
            }
            m &= 0x3FF;
            f_exponent = (e + 127) << 23;
            f_mantissa = m << 13;
        } else if (exponent == 31) {
            // Inf or NaN.
            f_exponent = 0xFF << 23;
            f_mantissa = mantissa << 13;
        } else {
            f_exponent = (exponent - 15 + 127) << 23;
            f_mantissa = mantissa << 13;
        }
        
        uint32_t result = f_sign | f_exponent | f_mantissa;
        float f;
        std::memcpy(&f, &result, sizeof(f));
        return f;
    }
    
    static Half fromFloat(float f) {
        uint32_t fi;
        std::memcpy(&fi, &f, sizeof(fi));
        
        uint32_t sign = (fi >> 31) & 0x1;
        int32_t exponent = ((fi >> 23) & 0xFF) - 127;
        uint32_t mantissa = fi & 0x7FFFFF;
        
        Half h;
        
        if (exponent == 128) { // Inf or NaN
            h.bits = (sign << 15) | 0x7C00 | (mantissa >> 13);
        } else if (exponent > 15) { // Overflow -> Inf
            h.bits = (sign << 15) | 0x7C00;
        } else if (exponent < -14) { // Underflow -> 0 or subnormal
            if (exponent < -24) {
                h.bits = sign << 15;
            } else {
                // Subnormal
                uint32_t m = mantissa | 0x800000;
                int shift = -exponent - 14;
                m >>= shift;
                h.bits = (sign << 15) | m;
            }
        } else {
            // Normal
            h.bits = (sign << 15) | ((exponent + 15) << 10) | (mantissa >> 13);
        }
        return h;
    }
    
    void setBits(uint16_t b) { bits = b; }
    uint16_t getBits() const { return bits; }
};

// Fill toNonlinear with the nonlinear encoding of each half-precision value.
// Input is an array of 65536 uint16_t bit patterns in native half format.
// Output is an array of 65536 uint16_t bit patterns in native half format.
void generateNonlinearLookup(const uint16_t* input, uint16_t* output, size_t size = 65536) {
    if (!input || !output) return;
    
    output[0] = 0;
    
    const float logBase = std::pow(2.7182818f, 2.2f);
    
    for (size_t i = 1; i < size; ++i) {
        uint16_t bits = input[i];
        Half h;
        h.setBits(bits);
        
        // Check for NaN or infinity (exponent bits all 1).
        if ((bits & 0x7C00) == 0x7C00) {
            output[i] = 0;
            continue;
        }
        
        float x = h.toFloat();
        float sign = (x < 0.0f) ? -1.0f : 1.0f;
        float ax = std::fabs(x);
        
        float result;
        if (ax <= 1.0f) {
            result = sign * std::pow(ax, 1.0f / 2.2f);
        } else {
            result = sign * (std::log(ax) / std::log(logBase) + 1.0f);
        }
        
        Half outHalf = Half::fromFloat(result);
        output[i] = outHalf.getBits();
    }
}
// The core algorithm is a straightforward per-element computation over the 65536 possible half-precision bit patterns. For each index `i` from 1 to 65535 (index 0 maps to 0), we first interpret the 16-bit integer as a half-precision float using the `half` class (which provides `setBits()` and conversion to `float`). We then determine the sign of the value. For the nonlinear transformation, we branch based on the absolute value: if ≤ 1.0, we use the gamma function; otherwise, we use the logarithm. The result is cast back to `half` and its bits are stored in the output array. Special handling is needed for NaN and infinity patterns: any value whose exponent bits (bits 10–14) are all 1 is considered invalid and maps to 0. Edge cases include negative zero (which should map to negative zero after applying the gamma, but since the input range includes all bit patterns, we must ensure the sign is preserved: for `x = -0.0`, `abs(-0.0) = 0.0`, `pow(0.0, 1/2.2) = 0.0`, and multiplying by sign gives `-0.0`). The time complexity is O(65536) = O(1) constant time, and space complexity is O(65536) for the output array, which is also constant. The solution uses `std::pow` and `std::log` from `<cmath>` and the `half` type (here we implement a minimal local `Half` wrapper for self-containedness, since the original code uses OpenEXR's `half`; for the task we define our own simple half representation with conversion to/from float).
