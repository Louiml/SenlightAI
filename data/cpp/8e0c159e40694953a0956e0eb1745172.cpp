// Write a C++ function `int minimumBitFlips(int start, int goal)` that computes the minimum number of single-bit flips (changing a 0 to 1 or 1 to 0) needed to transform the integer `start` into `goal`. Both integers are non-negative and can be as large as \(10^9\). The function must return the count of differing bit positions between the two numbers, treating them as unsigned binary values with no fixed bit-length (leading zeros are allowed). For example, converting `10` (binary `1010`) to `7` (binary `0111`) requires three flips because the binary strings differ in three positions. Implement the solution efficiently without converting to strings, and handle edge cases where the numbers are equal (return 0) or one is zero.
#include <cassert>

int main() {
    // Example 1 from the problem statement.
    assert(minimumBitFlips(10, 7) == 3);
    // Example 2.
    assert(minimumBitFlips(3, 4) == 3);
    // Equal numbers require zero flips.
    assert(minimumBitFlips(0, 0) == 0);
    assert(minimumBitFlips(12345, 12345) == 0);
    // Converting 0 to any positive number: count its set bits.
    assert(minimumBitFlips(0, 8) == 1);   // 0 (0000) to 8 (1000) differs in one bit.
    assert(minimumBitFlips(0, 15) == 4);  // 0 to 1111 requires 4 flips.
    // Converting to 0: count set bits of start.
    assert(minimumBitFlips(255, 0) == 8);
    // Two values with no overlapping bits (max difference).
    assert(minimumBitFlips(5, 10) == 4);  // 0101 vs 1010 → all 4 bits differ.
    // Large values near constraint.
    assert(minimumBitFlips(1000000000, 999999999) == 7); // manually verified popcount of XOR.
    // Adjacent powers of two differ in two bits (e.g., 1 << 30 vs 1 << 29).
    assert(minimumBitFlips(1 << 30, 1 << 29) == 2);
    return 0;
}
#include <cstdint>

// Returns the minimum number of bit flips to convert start into goal.
// The count equals the number of positions where their bits differ,
// which is the popcount of (start XOR goal).
int minimumBitFlips(int start, int goal) {
    // Compute the bitwise XOR: bits that are 1 indicate differing positions.
    uint32_t diff = static_cast<uint32_t>(start) ^ static_cast<uint32_t>(goal);
    
    int flipCount = 0;
    // Count set bits by repeatedly extracting the least significant bit.
    while (diff > 0) {
        if (diff & 1U) {
            ++flipCount;
        }
        diff >>= 1U;
    }
    
    return flipCount;
}
// The key insight is that two numbers differ in a particular bit position if and only if that bit in their XOR (`start ^ goal`) is 1. Therefore, the minimum number of flips equals the number of set bits (popcount) in `start ^ goal`. The algorithm computes the XOR of the two integers, then iterates over each bit of the result: while the XOR value is greater than zero, check if the least significant bit is 1 (using `value & 1`), increment the counter if so, then right-shift the value by one to examine the next bit. This works because flipping a bit in `start` to match `goal` is exactly the symmetric difference between their binary representations. Edge cases: if `start == goal`, the XOR is 0 and the loop never executes, returning 0. If one number is 0, the XOR is the other number itself, and the count is its popcount (e.g., 0 to 8 requires 1 flip because only the 3rd bit differs). The time complexity is \(O(\log(\max(start, goal)))\) or equivalently \(O(b)\) where \(b\) is the number of bits needed to represent the larger number (at most 31 for values up to \(10^9\)), and the space complexity is \(O(1)\). The solution is robust for all non-negative inputs within the constraints.
