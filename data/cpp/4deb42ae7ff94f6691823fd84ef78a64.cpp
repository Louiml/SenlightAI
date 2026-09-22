// Write a C++ function `int bitDistance(int a, int b)` that computes the Hamming distance between two non-negative integers, defined as the number of positions at which their corresponding binary bits differ. The function must handle values up to 2^31 - 1 (i.e., fit in a signed 32-bit integer). The result should be an integer between 0 and 31 inclusive. Do not rely on built-in bit-counting functions (like `__builtin_popcount`); implement the counting manually.
// The Hamming distance between `a` and `b` is the number of bit positions where they differ, which equals the number of set bits (1s) in `a ^ b`. The XOR operation yields a 1 at each differing position. The algorithm iterates over the bits of `x = a ^ b` by repeatedly checking the least significant bit and then shifting right. A counter increments whenever the lowest bit is 1. The loop continues until `x` becomes zero. Edge cases: when `a == b`, XOR is 0 and the result is 0; when one number is 0 and the other is large, the loop processes all set bits. Time complexity is O(number of bits in the XOR result), worst-case O(32) for fixed-width integers; space complexity is O(1).
#include <cstdint>

// Computes the Hamming distance between two non-negative integers.
// Returns the number of bit positions where the two integers differ.
int bitDistance(int a, int b) {
    // XOR gives a 1 at each differing bit position.
    int diff = a ^ b;
    int count = 0;
    // Count set bits by checking the least significant bit and shifting.
    while (diff != 0) {
        if (diff & 1) {
            ++count;
        }
        diff >>= 1;
    }
    return count;
}
#include <cassert>

int main() {
    // Basic example from the snippet: 1 (001) and 4 (100) differ in 2 bits.
    assert(bitDistance(1, 4) == 2);
    // Same numbers: distance 0.
    assert(bitDistance(5, 5) == 0);
    // One zero and a power of two: exactly 1 bit set.
    assert(bitDistance(0, 8) == 1);
    // 0 and 0: distance 0.
    assert(bitDistance(0, 0) == 0);
    // 7 (111) and 0 (000): all 3 bits differ.
    assert(bitDistance(7, 0) == 3);
    // Large values: 2147483647 (all 31 bits set) vs 0 -> 31.
    assert(bitDistance(2147483647, 0) == 31);
    // Values differing in all bits within the 8-bit range.
    assert(bitDistance(0b10101010, 0b01010101) == 8);
    // Mixed case: 3 (011) and 6 (110) differ in all 3 bits.
    assert(bitDistance(3, 6) == 3);
    // Random check: 10 (1010) and 5 (0101) differ in all 4 bits.
    assert(bitDistance(10, 5) == 4);
    // 255 (11111111) vs 0 -> 8 bits.
    assert(bitDistance(255, 0) == 8);
    return 0;
}
