// Write a C++ function that takes an unsigned integer and returns the number of 1 bits (also known as the Hamming weight or population count) in its binary representation. The function should work for any valid `unsigned int` value, including zero, and must not rely on built-in bit-counting functions like `__builtin_popcount`. Provide the implementation as a free function named `countSetBits` that accepts an `unsigned int` parameter and returns an `int`.
The solution repeatedly examines the least significant bit of the number and shifts the number right by one bit in each iteration. Since the input is an unsigned integer, right shifts logically fill the vacated high-order bits with zeros, so the loop can safely run while the value is greater than zero. In each iteration, the remainder when dividing by 2 (`A % 2`) is either 0 or 1; adding this to a running sum counts the current least significant bit's contribution. Then the number is shifted right by one to move the next bit into the least significant position. Edge cases include `0`, which has zero set bits and the loop body never executes, and the maximum `unsigned int` value, where all bits are 1 and the result equals the number of bits in the type (typically 32). The algorithm runs in O(b) time, where b is the number of bits in the unsigned integer (e.g., 32), and uses O(1) auxiliary space. No special handling for negative numbers is needed since the parameter type is `unsigned int`.
#include <cstdint>

// Counts the number of 1 bits in the binary representation of the given unsigned integer.
// Returns the population count (Hamming weight) of the input value.
int countSetBits(unsigned int value) {
    int count = 0;
    while (value > 0) {
        count += value % 2;  // Add the least significant bit (0 or 1).
        value >>= 1;         // Shift right to examine the next bit.
    }
    return count;
}
#include <cassert>
#include <climits>

int countSetBits(unsigned int value);

int main() {
    assert(countSetBits(0) == 0);
    assert(countSetBits(1) == 1);
    assert(countSetBits(2) == 1);   // binary 10
    assert(countSetBits(3) == 2);   // binary 11
    assert(countSetBits(11) == 3);  // binary 1011
    assert(countSetBits(255) == 8); // binary 11111111
    assert(countSetBits(1024) == 1); // binary 10000000000
    assert(countSetBits(UINT_MAX) == static_cast<int>(sizeof(unsigned int) * CHAR_BIT));
    return 0;
}
