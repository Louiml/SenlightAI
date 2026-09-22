Given an array of non-negative 32-bit integers, write a C++ function that computes the bitwise OR of all numbers’ bits and the bitwise AND of all numbers’ bits across a fixed 13-bit field (bits 0 through 12), then returns the difference (OR value minus AND value) as a single integer. The input array may be empty (in which case return 0), and all numbers are assumed to fit within 13 bits. The function should treat missing bits as 0 for the OR and as 1 for the AND, matching the snippet’s initialization of `maxi` to 0 and `mini` to 1.

#include <cassert>
#include <vector>

// Declaration of the tested function (assume it is defined above).
int orAndDifference(const std::vector<int>& numbers);

int main() {
    // Single element: OR and AND are both the element, difference is 0.
    assert(orAndDifference({5}) == 0);
    // All same numbers: OR == AND, difference 0.
    assert(orAndDifference({7, 7, 7}) == 0);
    // Mixed numbers: OR=7 (0b111), AND=0, difference 7.
    assert(orAndDifference({5, 2, 7}) == 7);
    // Empty array returns 0.
    assert(orAndDifference({}) == 0);
    // Larger values: OR=31 (0b11111), AND=0, difference 31.
    assert(orAndDifference({1, 2, 4, 8, 16}) == 31);
    // Overlapping bits: OR=15, AND=3, difference 12.
    assert(orAndDifference({3, 5, 7, 11}) == 12);
    // All bits set in all numbers: OR=8191, AND=8191, difference 0.
    assert(orAndDifference({4095, 4095}) == 0);
    // One number with only bit 0 set, another with only bit 12 set: OR has both, AND has none.
    assert(orAndDifference({1, 4096}) == 4097);
    // More complex: numbers {0, 1, 2} -> OR=3, AND=0, diff=3.
    assert(orAndDifference({0, 1, 2}) == 3);
    // Numbers {2048, 1024} -> OR=3072, AND=0, diff=3072.
    assert(orAndDifference({2048, 1024}) == 3072);
    return 0;
}

#include <vector>
#include <cmath>
#include <cstdint>

// Compute (bitwise OR of all numbers) - (bitwise AND of all numbers) over 13 bits.
int orAndDifference(const std::vector<int>& numbers) {
    if (numbers.empty()) {
        return 0;
    }
    constexpr int kBits = 13;
    std::vector<int> maxi(kBits, 0);
    std::vector<int> mini(kBits, 1);
    for (int x : numbers) {
        for (int j = 0; j < kBits; ++j) {
            if ((x >> j) & 1) {
                maxi[j] = 1;
            } else {
                mini[j] = 0;
            }
        }
    }
    int max_value = 0;
    int min_value = 0;
    for (int j = 0; j < kBits; ++j) {
        if (maxi[j] == 1) {
            max_value += static_cast<int>(std::pow(2, j));
        }
        if (mini[j] == 1) {
            min_value += static_cast<int>(std::pow(2, j));
        }
    }
    return max_value - min_value;
}

// The core idea is to process each number at the bit level. For a fixed field of 13 bits (indices 0 to 12), we maintain two arrays: one for the OR result (`maxi`) initialized to 0, and one for the AND result (`mini`) initialized to 1. For every number, we iterate over all 13 bit positions. At each position `j`, check if the `j`-th bit of the current number is set (by shifting right by `j` and masking with 1). If set, we update `maxi[j] = 1`; otherwise, we update `mini[j] = 0`. After processing all numbers, compute the integer values of `maxi` and `mini` by summing `bit_value * 2^j` for each position where the bit is 1. The final answer is `max_value - min_value`. Edge cases: if the array is empty, both `maxi` and `mini` retain initial values (0 and all 1s), giving `0 - (2^13 - 1) = -8191`, but the problem statement likely expects 0 for empty input, so handle explicitly. For non-empty arrays, because `maxi[j]` becomes 1 if any number has that bit set, and `mini[j]` becomes 0 if any number has that bit clear, the difference is always non-negative. Time complexity is O(n * 13) = O(n) with constant factors, space O(13) = O(1) extra.
