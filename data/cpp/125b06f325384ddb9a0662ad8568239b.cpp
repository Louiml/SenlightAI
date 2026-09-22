Write a C++ function that takes a non-empty vector of 32-bit integers where every number appears exactly three times except for one number that appears exactly once, and returns that unique number. The input vector may contain negative numbers, zero, and positive values. You must implement the solution using bit manipulation and cannot use unordered maps, sorting, or modifying the input vector.

#include <cassert>
#include <vector>

// Single function declaration is already above; main provided here for test:
int main() {
    // Simple case
    std::vector<int> v1 = {2, 2, 3, 2};
    assert(singleNumberOnce(v1) == 3);

    // Mixed positive and negative
    std::vector<int> v2 = {-5, 1, -5, 1, -5, 1, 7};
    assert(singleNumberOnce(v2) == 7);

    // Negative unique number
    std::vector<int> v3 = {-3, 4, 4, 4, 5, 5, 5};
    assert(singleNumberOnce(v3) == -3);

    // Zero appears once
    std::vector<int> v4 = {9, 0, 9, 9, 2, 2, 2};
    assert(singleNumberOnce(v4) == 0);

    // All numbers except one are the same triple
    std::vector<int> v5 = {1, 1, 1, 2, 3, 3, 3, 4, 4, 4};
    assert(singleNumberOnce(v5) == 2);

    // Single element vector
    std::vector<int> v6 = {42};
    assert(singleNumberOnce(v6) == 42);

    // Large numbers
    std::vector<int> v7 = {100000, 3, 3, 3, 100000, 100000, -123456};
    assert(singleNumberOnce(v7) == -123456);

    // Larger test with many repetitions
    std::vector<int> v8;
    for (int i = 0; i < 10; ++i) {
        v8.push_back(8);
        v8.push_back(6);
        v8.push_back(4);
    }
    v8.push_back(7);
    assert(singleNumberOnce(v8) == 7);

    return 0;
}

#include <vector>
#include <cstdint>

// Returns the single number that appears exactly once in a vector where every
// other number appears exactly three times.
int singleNumberOnce(const std::vector<int>& nums) {
    // Count occurrences of each bit across all numbers.
    int bitCounts[32] = {0};
    for (int num : nums) {
        for (int bit = 0; bit < 32; ++bit) {
            if (num & (1 << bit)) {
                ++bitCounts[bit];
            }
        }
    }
    // Reconstruct the unique number by taking each bit modulo 3.
    int result = 0;
    for (int bit = 0; bit < 32; ++bit) {
        if (bitCounts[bit] % 3 != 0) {
            result |= (1 << bit);
        }
    }
    return result;
}

// The key insight is that for each bit position (0 to 30 for non-negative values, but we can safely consider 32 bits to handle negatives using two's complement), the sum of that bit across all numbers modulo 3 gives the bit value of the unique number. This works because any number that appears three times contributes either 0 or 3 to each bit's count, which is congruent to 0 modulo 3, while the unique number contributes either 0 or 1. The algorithm initializes a 32-element integer array `counts` to zero, iterates through each number in the vector, and for each bit position from 0 to 31, if the bit is set in the number, increments the corresponding count. After processing all numbers, each `counts[i]` is reduced modulo 3 to yield the bit of the answer. The answer is then constructed by setting the `i`-th bit of the result if `counts[i] % 3 == 1`. Edge cases include negative numbers (which have the sign bit set), but since we process all 32 bits uniformly and use bitwise operations on signed integers, the two's complement representation ensures correct behavior. The time complexity is O(32 * n) which simplifies to O(n), and the space complexity is O(1) since we use a fixed-size array of 32 integers. An alternative more space-efficient approach uses three bitmasks (ones, twos) but is less intuitive; the counting approach is clearer and easily verified.
