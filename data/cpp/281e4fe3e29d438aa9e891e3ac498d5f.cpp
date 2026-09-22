// Write a C++ function that takes a non-negative integer `n` and returns a `std::vector<int>` containing all the bit positions (0-indexed, from least significant to most significant) where `n` has a 1 in its 64-bit binary representation, in ascending order. For example, for `n = 11` (binary `1011`), the function should return `{0, 1, 3}`. The function must handle `n = 0` correctly (returning an empty vector), and should work for values up to `2^63 - 1` (assuming 64-bit signed long long). Do not use any built-in bit-scanning functions; iterate through all 64 bits explicitly.

// The solution translates the numeric value into its binary representation by checking each of the 64 bits of a `long long` (or `unsigned long long` to avoid sign issues, though for non-negative inputs signed also works). The approach is to iterate over indices from 0 to 63, using a bitmask `1ULL << i` to test whether the i-th bit is set. If the bit is set, push the index into the result vector. This naturally produces ascending order because we go from least significant bit (i=0) to most significant (i=63). The edge case `n = 0` results in no bits being set, so the vector is empty. Time complexity is O(64) = O(1) per call, and space complexity is O(number of set bits) for the output vector, which is at most 64. The implementation uses `const` references and values where appropriate, and returns by value.

#include <vector>

// Return a vector of bit positions (0-indexed, LSB first) where the given
// non-negative integer has a 1 in its 64-bit binary representation.
std::vector<int> bitPositions(const unsigned long long n) {
    std::vector<int> result;
    for (int i = 0; i < 64; ++i) {
        if ((n >> i) & 1ULL) {
            result.push_back(i);
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// Assume bitPositions is declared above (for completeness, shown here again).
std::vector<int> bitPositions(const unsigned long long n) {
    std::vector<int> result;
    for (int i = 0; i < 64; ++i) {
        if ((n >> i) & 1ULL) {
            result.push_back(i);
        }
    }
    return result;
}

int main() {
    // Test zero
    assert(bitPositions(0ULL) == std::vector<int>{});

    // Test simple numbers
    assert(bitPositions(1ULL) == std::vector<int>{0});
    assert(bitPositions(2ULL) == std::vector<int>{1});
    assert(bitPositions(3ULL) == std::vector<int>{0, 1});
    assert(bitPositions(4ULL) == std::vector<int>{2});
    assert(bitPositions(11ULL) == std::vector<int>{0, 1, 3});

    // Test a number with alternating bits
    assert(bitPositions(0b10101010ULL) == std::vector<int>{1, 3, 5, 7});

    // Test a large number with bits in higher positions
    assert(bitPositions(1ULL << 40) == std::vector<int>{40});
    assert(bitPositions((1ULL << 40) | (1ULL << 2)) == std::vector<int>{2, 40});

    // Test maximum possible value within 64 bits (all ones)
    std::vector<int> all_bits;
    for (int i = 0; i < 64; ++i) all_bits.push_back(i);
    assert(bitPositions(0xFFFFFFFFFFFFFFFFULL) == all_bits);

    // Test number with a single high bit near the sign boundary
    assert(bitPositions(1ULL << 63) == std::vector<int>{63});

    return 0;
}
