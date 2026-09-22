Write a C++ function named `countOneBits` that takes an unsigned 32-bit integer (`uint32_t`) as input and returns the number of set bits (i.e., bits equal to 1) in its binary representation. The function must be implemented using an efficient bit-manipulation technique that repeatedly clears the lowest set bit, and it must correctly handle all possible `uint32_t` values, including zero and values with the highest bit set. The function should be declared as `int countOneBits(uint32_t n)` and be self-contained with appropriate headers.

// The core algorithm leverages the bitwise operation `n &= (n - 1)`, which clears the lowest set bit of `n`. For example, if `n = 0b1100` (12), then `n - 1 = 0b1011`, and `n & (n - 1) = 0b1000` (8). Each iteration of a while loop counts one set bit and then clears it. This way, the loop runs exactly as many times as the number of 1-bits in `n` (unlike a shift-based approach that iterates over all 32 bits). The algorithm handles `n = 0` naturally: the while loop condition fails immediately, returning 0. For `n` with all 32 bits set (i.e., `0xFFFFFFFF`), it returns 32. The time complexity is O(k) where k is the number of set bits (at most 32), and the space complexity is O(1) since only an integer counter is used. Edge cases include `n = 0` (expect 0), `n = 1` (expect 1), and large values like `0x80000000` (expect 1), all of which are handled correctly by the bit-clearing operation.

#include <cstdint>

// Counts the number of 1-bits in the binary representation of n.
// Uses the bit trick n & (n - 1) to clear the lowest set bit each iteration.
int countOneBits(uint32_t n) {
    int count = 0;
    while (n != 0) {
        ++count;
        n &= n - 1;  // Clears the lowest set bit
    }
    return count;
}

#include <cassert>
#include <cstdint>

int countOneBits(uint32_t n);  // Forward declaration for testing

int main() {
    // Example from problem: 00000000000000000000000000001011 has 3 ones
    assert(countOneBits(0b00000000000000000000000000001011u) == 3);
    // Example: 00000000000000000000000010000000 has 1 one
    assert(countOneBits(0b00000000000000000000000010000000u) == 1);
    // Example: 11111111111111111111111111111101 has 31 ones
    assert(countOneBits(0b11111111111111111111111111111101u) == 31);
    // Zero has no bits set
    assert(countOneBits(0u) == 0);
    // A single lowest bit set
    assert(countOneBits(1u) == 1);
    // Highest bit set (0x80000000) has exactly one 1
    assert(countOneBits(0x80000000u) == 1);
    // All 32 bits set
    assert(countOneBits(0xFFFFFFFFu) == 32);
    // Arbitrary value: 0x12345678 -> count bits: 0x1=1,0x2=1,0x3=2,0x4=1,0x5=2,0x6=2,0x7=3,0x8=1 => total 13
    assert(countOneBits(0x12345678u) == 13);
    // Power of two: 0x40000000 (bit 30) has exactly one 1
    assert(countOneBits(0x40000000u) == 1);
    // 0x0F0F0F0F has four groups of 4 ones each => 16 ones
    assert(countOneBits(0x0F0F0F0Fu) == 16);
    return 0;
}
