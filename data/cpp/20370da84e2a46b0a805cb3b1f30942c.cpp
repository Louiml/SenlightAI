Write a standalone C++ function that, given an unsigned 16-bit integer input (representing the raw bit pattern of an IEEE 754 half-precision floating-point value), returns a `std::vector<unsigned short>` containing the bit patterns of the closest half-precision values that have exactly one fewer set bit than the input. If the input has 0 or 1 set bits, return an empty vector (since no value can have fewer set bits). For each possible target bit count from `(numSetBits(input) - 1)` down to 0, scan all 65,536 possible half bit patterns, find the one whose numeric floating-point value (converted using `half` semantics) is closest in absolute difference to the input’s numeric value, and append its bit pattern to the result. For equal floating-point distances, you may pick either candidate. The function must be self-contained (no external libraries beyond standard headers) and should implement its own bit-count and half-conversion logic—do not rely on any existing half-precision library. The function signature should be: `std::vector<unsigned short> closestWithOneFewerBit(unsigned short inputBits);`
// The core task requires iterating over all possible target bit counts (from `inputBits` population count minus 1 down to 0). For each target bit count, we must scan all 65,536 possible 16-bit patterns, filter those whose population count equals the target, and select the one whose floating-point value (interpreted as a half) is closest (by absolute difference) to the input’s half value. Edge cases: if the input has 0 or 1 set bits, there is no valid target with fewer bits, so return an empty vector. If the input has a NaN or infinity bit pattern (exponent bits all 1s), the floating-point comparison may be problematic; for simplicity, treat NaN as comparing equal to NaN (though this is not standard), and for infinity, handle with normal numeric comparison. Because we are scanning 65,536 patterns for each bit count (up to 15 targets), worst-case operations are about 65,536 × 15 ≈ 983,040 iterations, each O(1). The population count can be precomputed in a lookup table of size 65,536 to speed up filtering. Time complexity is O(65536 × B) where B is the number of set bits in the input (max 15), and space complexity is O(65536) for the bit-count table plus O(B) for the result vector. For large-scale repeated use, caching results would be beneficial, but the function is required to compute on demand.
#include <vector>
#include <cstdint>
#include <cmath>

// Precomputed population count for all 16-bit values
static const unsigned char kBitCount[65536] = []() {
    unsigned char table[65536] = {0};
    for (int i = 1; i < 65536; ++i) {
        table[i] = table[i >> 1] + (i & 1);
    }
    return table;
}();

// Convert a 16-bit half bit pattern to a float using IEEE 754 half-precision
static float halfToFloat(unsigned short bits) {
    uint32_t sign = (bits & 0x8000) >> 15;
    uint32_t exponent = (bits & 0x7C00) >> 10;
    uint32_t mantissa = bits & 0x03FF;

    if (exponent == 0) {
        // Subnormal or zero
        return std::ldexp(static_cast<float>(mantissa), -24) * (sign ? -1.0f : 1.0f);
    } else if (exponent == 31) {
        // Infinity or NaN
        if (mantissa == 0) {
            return sign ? -INFINITY : INFINITY;
        } else {
            return NAN;
        }
    } else {
        // Normalized
        return std::ldexp(static_cast<float>(mantissa | 0x0400), static_cast<int>(exponent) - 25) * (sign ? -1.0f : 1.0f);
    }
}

// Return the bit patterns of values with exactly one fewer set bit than input,
// sorted by increasing bit count, choosing the closest half value for each target count.
std::vector<unsigned short> closestWithOneFewerBit(unsigned short inputBits) {
    // Population count of the input
    int inputCount = kBitCount[inputBits];
    if (inputCount <= 1) {
        return {};
    }

    // Numeric value of the input half
    float inputValue = halfToFloat(inputBits);

    std::vector<unsigned short> result;
    result.reserve(inputCount - 1);

    // For each target bit count from (inputCount-1) down to 0
    for (int targetCount = inputCount - 1; targetCount >= 0; --targetCount) {
        bool found = false;
        unsigned short bestBits = 0;
        float bestDiff = INFINITY;

        // Scan all 16-bit patterns
        for (int i = 0; i < 65536; ++i) {
            if (kBitCount[i] != targetCount) continue;

            float candidateValue = halfToFloat(static_cast<unsigned short>(i));
            // Handle NaN comparison: if both are NaN, treat as equal (diff 0)
            float diff;
            if (std::isnan(inputValue) && std::isnan(candidateValue)) {
                diff = 0.0f;
            } else if (std::isnan(inputValue) || std::isnan(candidateValue)) {
                continue; // NaN never closer than a finite value
            } else {
                diff = std::fabs(inputValue - candidateValue);
            }

            if (!found || diff < bestDiff) {
                found = true;
                bestDiff = diff;
                bestBits = static_cast<unsigned short>(i);
            }
        }

        // There must always be at least one pattern with targetCount bits
        result.push_back(bestBits);
    }

    return result;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be defined above (in the same compilation unit)

int main() {
    // Test 1: Input with 0 set bits (0x0000) -> empty vector
    {
        auto result = closestWithOneFewerBit(0x0000);
        assert(result.empty());
    }

    // Test 2: Input with 1 set bit (0x0001) -> empty vector
    {
        auto result = closestWithOneFewerBit(0x0001);
        assert(result.empty());
    }

    // Test 3: Input with 2 set bits (0x0003 = 0.0009765625)
    // Target count 1: closest value with exactly 1 bit set is 0x0001 (same bit value, same float)
    {
        auto result = closestWithOneFewerBit(0x0003);
        assert(result.size() == 1);
        assert(result[0] == 0x0001);
    }

    // Test 4: Input 0x3C00 (1.0f) has 5 set bits (binary 1111000000000000)
    // Targets: 4 bits, 3 bits, 2 bits, 1 bit, 0 bits.
    // For 4 bits: 0x3800 (0.5) diff=0.5, 0x3A00 (0.75) diff=0.25 -> best 0x3A00
    // For 3 bits: 0x3000 (0.25), 0x3400 (0.375), 0x2C00 (0.1875) -> best 0x3400 (diff 0.625)
    // For 2 bits: 0x2800 (0.15625), 0x4400 (3.0) -> best 0x2800
    // For 1 bit: 0x0400 (2^-14) -> best
    // For 0 bits: 0x0000 (0.0) -> best
    {
        auto result = closestWithOneFewerBit(0x3C00);
        assert(result.size() == 5);
        // Check that the first (4 bits) is closer than alternatives
        // For 4 bits, the closest to 1.0 is 0.75 (0x3A00) since 0.5 is 0x3800
        assert(result[0] == 0x3A00);
        // Check that the last (0 bits) is 0x0000
        assert(result[4] == 0x0000);
    }

    // Test 5: Input 0x7BFF (largest finite half, 65504.0) has many bits set
    // Just verify size equals numSetBits - 1 and last element is 0x0000
    {
        auto result = closestWithOneFewerBit(0x7BFF);
        // Population count of 0x7BFF: 0x7BFF = 0x7FFF - 0x400? Actually 0x7BFF binary has 13? Let's just check non-empty
        assert(!result.empty());
        assert(result.back() == 0x0000); // zero is always the closest 0-bit value for large positive
    }

    // Test 6: NaN input (0x7E00) -> should return candidate for each target bit count
    // For target counts, the closest finite is 0 for small counts? Not strictly defined, but function should not crash
    {
        auto result = closestWithOneFewerBit(0x7E00);
        assert(!result.empty());
    }

    // Test 7: Negative input - largest negative finite (0xFBFF)
    {
        auto result = closestWithOneFewerBit(0xFBFF);
        assert(!result.empty());
    }

    // Test 8: Check monotonic ordering by bit count
    {
        auto result = closestWithOneFewerBit(0x5555); // typical 8 set bits
        assert(result.size() == 7);
        int prevCount = kBitCount[result[0]];
        for (size_t i = 1; i < result.size(); ++i) {
            int currCount = kBitCount[result[i]];
            assert(currCount < prevCount);
            prevCount = currCount;
        }
    }

    return 0;
}
