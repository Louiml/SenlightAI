// Write a C++ function named `countTrailingZeros` that accepts an `unsigned int` and returns the number of trailing zero bits in its 32‑bit binary representation. The function must handle the special case where the input is zero, in which case the number of trailing zeros is defined as 32 (all bits are zero). Your implementation must be self‑contained and correct for all possible `unsigned int` values, including those with exactly one set bit (e.g., powers of two), those with multiple set bits, and the maximum value `0xFFFFFFFF` (which should return 0). The function must not use any external libraries or built‑in bit‑counting intrinsics such as `__builtin_ctz`; instead, implement the algorithm from scratch using bitwise operations and loops. Provide a clean, efficient solution that works on typical platforms where `unsigned int` is 32 bits.
The core problem is to determine the count of consecutive zero bits starting from the least significant bit (LSB) of a 32‑bit unsigned integer. For a nonzero value `x`, the rightmost set bit can be isolated using `x & -x` (two's complement trick). However, the simplest approach that matches the given snippet’s spirit is to repeatedly shift the number right and count until a set bit is found or the value becomes zero. For the input zero, special handling returns 32. The algorithm directly counts each trailing zero bit: while the least significant bit is 0, shift right by one and increment a counter. For a random input, the expected number of shifts is small (on average less than 2), but in the worst case (e.g., `0x80000000`) it will shift 31 times. Time complexity is O(number of trailing zeros) which is O(32) worst case; space complexity is O(1). An alternative using `~x & (x-1)` and a popcount is also possible, but a simple loop is clear and correct.
#include <cstdint>

// Returns the number of trailing zero bits in the 32-bit representation of x.
// For x == 0, returns 32.
int countTrailingZeros(unsigned int x) {
    if (x == 0) {
        return 32;
    }

    int count = 0;
    while ((x & 1u) == 0) {
        x >>= 1;
        ++count;
    }
    return count;
}
#include <cassert>

int main() {
    // Edge cases
    assert(countTrailingZeros(0) == 32);
    assert(countTrailingZeros(0xFFFFFFFF) == 0); // all bits set
    assert(countTrailingZeros(1) == 0);          // LSB is 1
    assert(countTrailingZeros(2) == 1);          // binary 10
    assert(countTrailingZeros(4) == 2);          // binary 100
    assert(countTrailingZeros(8) == 3);          // binary 1000
    assert(countTrailingZeros(0x80000000) == 31); // highest bit only
    assert(countTrailingZeros(0x3000FF00) == 8);  // binary ends with 8 zeros
    assert(countTrailingZeros(0xC0000000) == 30); // two high bits set
    assert(countTrailingZeros(0x60000000) == 29); // bits 29 and 30 set
    assert(countTrailingZeros(0x00011000) == 12); // check from snippet
}
