Write a C++ function named `longestRunOfSetBits` that takes a single `const int&` parameter (a signed integer in decimal) and returns a `size_t` representing the maximum number of consecutive 1-bits (set bits) in that integer's binary representation. The function should handle positive integers, zero, and negative integers (which use two's complement representation in C++). For example, the integer `0` should return `0` (no set bits), `7` (binary `111`) should return `3`, and a negative number like `-1` (all bits set to 1) should return the total number of bits in an `int` on that platform. The function must not use any standard library bit-manipulation helpers like `std::bitset` or compiler builtins; implement the bit counting manually using bitwise operators and loops. The function must be `const`-correct with respect to the parameter, and the implementation should be self-contained in a single source file that can be compiled and tested independently.

#include <cassert>
#include <cstddef>

// The solution function declaration (assumed to be in the same translation unit or included here).
std::size_t longestRunOfSetBits(const int& num);

int main() {
    // Zero has no set bits.
    assert(longestRunOfSetBits(0) == 0);
    // 1 is binary ...0001 -> max run = 1.
    assert(longestRunOfSetBits(1) == 1);
    // 2 is binary ...0010 -> max run = 1.
    assert(longestRunOfSetBits(2) == 1);
    // 7 is binary ...0111 -> max run = 3.
    assert(longestRunOfSetBits(7) == 3);
    // 15 is binary ...1111 -> max run = 4.
    assert(longestRunOfSetBits(15) == 4);
    // 0b101101 has runs of 1, 2, and 1 -> max = 2.
    assert(longestRunOfSetBits(0b101101) == 2);
    // -1 in two's complement is all bits 1 -> max run = number of bits in int.
    assert(longestRunOfSetBits(-1) == sizeof(int) * 8);
    // -2 is ...1110 (all bits 1 except LSB) -> max run = bits - 1? Actually run is from bit1 to top: length = sizeof(int)*8 -1.
    // For a 32-bit int, -2 binary is 11111111111111111111111111111110, max run = 31.
    assert(longestRunOfSetBits(-2) == sizeof(int) * 8 - 1);
    // A negative number with a break: -3 is ...1101 -> max run = 29? Wait, for 32-bit: 11111111111111111111111111111101 → runs: 30 ones then 0 then 1 → max = 30.
    // But the top run is from bit1 up to bit31 = 31 bits? Actually bits: bit0=1, bit1=0, bits2-31 all 1 → that's 30 ones contiguous? Let's count: bits 2..31 = 30 bits, so max = 30.
    assert(longestRunOfSetBits(-3) == sizeof(int) * 8 - 2);
    // A positive with alternating bits: 0b1010101 (85) has max run 1.
    assert(longestRunOfSetBits(85) == 1);
    return 0;
}

#include <cstddef>

// Returns the length of the longest run of consecutive 1-bits in the binary representation of num.
// Handles positive, zero, and negative (two's complement) integers.
std::size_t longestRunOfSetBits(const int& num) {
    std::size_t currentRun = 0;
    std::size_t maxRun = 0;
    const std::size_t totalBits = sizeof(num) * 8;

    for (std::size_t i = 0; i < totalBits; ++i) {
        if ((num >> i) & 1) {
            ++currentRun;
            if (currentRun > maxRun) {
                maxRun = currentRun;
            }
        } else {
            currentRun = 0;
        }
    }
    return maxRun;
}

// The core algorithm iterates over each bit position of the integer from the least significant bit (LSB, bit 0) up to the most significant bit (bit `sizeof(int) * 8 - 1`). For each bit position `i`, we extract the bit using `(num >> i) & 1`. If the bit is 1, we increment a running counter `currentRun`. If the counter exceeds a stored `maxRun`, we update `maxRun`. If the bit is 0, we reset `currentRun` to 0. At the end, `maxRun` holds the longest sequence of consecutive set bits. Edge cases include zero (where all bits are 0, so the loop never increments the counter, and `maxRun` remains 0), positive numbers (handled normally), and negative numbers (where the sign bit and all higher bits are 1 in two's complement, leading to a long run of 1s from the sign bit upward, but the loop must correctly count runs across all bits). The time complexity is O(b), where b is the number of bits in an `int` (typically 32 or 64), which is constant for a given platform. The space complexity is O(1) since only a few local variables are used.
