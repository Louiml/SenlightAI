Write a C++ function named `countSetBits` that takes a non-negative integer `num` (with `0 <= num <= 10^6`) and returns the number of 1-bits in its binary representation (also known as the Hamming weight or popcount). The function must not use any built-in bit-counting functions (like `std::bitset::count` or `__builtin_popcount`) and must compute the result manually. Handle the case `num = 0` correctly (returns 0). The function signature should be `int countSetBits(int num)` and must be `const`-correct (i.e., take its parameter by value and not modify any external state).

#include <cassert>

int countSetBits(int num); // declaration

int main() {
    // Basic cases
    assert(countSetBits(0) == 0);
    assert(countSetBits(1) == 1);
    assert(countSetBits(2) == 1);   // 10
    assert(countSetBits(3) == 2);   // 11
    assert(countSetBits(4) == 1);   // 100
    assert(countSetBits(5) == 2);   // 101
    assert(countSetBits(7) == 3);   // 111
    assert(countSetBits(15) == 4);  // 1111
    assert(countSetBits(255) == 8); // 11111111
    assert(countSetBits(1023) == 10);
    assert(countSetBits(1000000) == 7); // 11110100001001000000 has seven 1s

    return 0;
}

#include <cstdint>

// Returns the number of 1-bits in the binary representation of a non-negative integer.
// Works for any non-negative input, including 0, and uses manual bit iteration.
int countSetBits(int num) {
    // Cast to unsigned to avoid undefined behavior on right-shift for negative numbers.
    unsigned int value = static_cast<unsigned int>(num);
    int count = 0;
    while (value != 0) {
        count += (value & 1); // Add 1 if LSB is set, else add 0.
        value >>= 1;          // Shift right to examine next bit.
    }
    return count;
}

// The simplest robust approach is to iterate over each bit of the integer (using a fixed number of bits, e.g., 31 bits for non-negative integers under 10^6, or up to 20 bits as in the original snippet). For each bit position from `0` to `31` (or until `num` becomes zero), check if the least significant bit is set by using `num & 1`, increment a counter if so, and then shift `num` right by one (`num >>= 1`). Continue until `num` becomes `0`, which naturally handles all positive numbers and also correctly returns 0 for input `0` because the loop never runs. This algorithm runs in `O(number of bits in num)` time, which is at most 31 operations for a 32-bit integer, and uses `O(1)` auxiliary space. Edge cases: `num = 0` → returns 0; large numbers near 10^6 (which fit comfortably in a 32-bit integer) are handled without overflow. Another subtle edge case is if the function is called with a negative number; we can assume the problem guarantees non-negative input, but to be safe we could convert to `unsigned int` to avoid undefined behavior from right-shifting a negative signed integer. For the solution, we'll cast to `unsigned int` to ensure well-defined behavior.
