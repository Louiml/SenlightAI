// Write a C++ function named `countSetBits` that takes a non-negative 32-bit integer `n` and returns the number of 1-bits in its binary representation (also known as the population count or Hamming weight). The function must handle the full unsigned 32-bit range (0 to 4,294,967,295). Do not use any built-in bit-counting functions such as `__builtin_popcount` or `std::bitset::count`. The solution should be explicit about iterating over all 32 bits. The input is guaranteed to be representable as an unsigned 32-bit integer, but you should treat it as a 32-bit pattern regardless of the platform's `int` size.
// The standard approach is to examine each of the 32 bits of the integer one by one. For each bit, check whether it is set (i.e., equals 1) by using a bitwise AND with the value 1. If the result is 1, increment a counter. Then shift the number right by one position (`n >>= 1`) to move the next bit into the least significant position. Repeat this process for exactly 32 iterations to cover all bits. This works for any non-negative 32-bit value, including the maximum 4,294,967,295. Edge cases include `0` (returns 0), `1` (returns 1), and values with all 32 bits set (returns 32). The algorithm runs in O(32) time = O(1) time because the number of bits is constant, and uses O(1) auxiliary space. Since we are only shifting right and checking the low bit, there is no risk of sign extension issues if we treat the input as an `unsigned int` (or `uint32_t`) to avoid implementation-defined behavior of right-shifting a signed negative number.
#include <cstdint>

// Count the number of 1-bits in the 32-bit representation of a non-negative integer.
int countSetBits(uint32_t n) {
    int count = 0;
    for (int i = 0; i < 32; ++i) {
        if ((n & 1U) == 1U) {
            ++count;
        }
        n >>= 1;
    }
    return count;
}
#include <cassert>

int main() {
    // Basic cases
    assert(countSetBits(0) == 0);
    assert(countSetBits(1) == 1);
    assert(countSetBits(2) == 1);   // 10
    assert(countSetBits(3) == 2);   // 11
    assert(countSetBits(7) == 3);   // 111
    
    // Full 32-bit range
    assert(countSetBits(0xFFFFFFFF) == 32);      // All ones
    assert(countSetBits(0x80000000) == 1);       // Only highest bit
    assert(countSetBits(0x55555555) == 16);      // Alternating bits starting with 1
    assert(countSetBits(0xAAAAAAAA) == 16);      // Alternating bits starting with 0
    assert(countSetBits(0x0F0F0F0F) == 16);      // Pattern with 16 ones
    
    // Larger values without literals that exceed int range (use unsigned literals)
    assert(countSetBits(2147483647U) == 31);     // 2^31 - 1
    assert(countSetBits(4294967295U) == 32);     // Maximum 32-bit value
}
