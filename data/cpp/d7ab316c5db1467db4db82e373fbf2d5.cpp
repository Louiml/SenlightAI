Write a C++ function `int countSetBits(int n)` that takes a non-negative integer `n` and returns the number of 1-bits in its binary representation (also known as the population count or Hamming weight). Your solution must not use any built-in bit-counting functions (like `__builtin_popcount`), and must handle the value `0` correctly by returning `0`. The input is guaranteed to fit within a signed 32-bit integer range, but treat the input as an unsigned pattern to avoid sign-extension issues during bitwise shifts. The function should be efficient and use only constant extra space.
// The classic approach is to iteratively check the least significant bit (LSB) using a bitwise AND with `1`, then shift the number right by one bit (`>>= 1`) to process the next bit. This works because shifting right on an unsigned integer is a logical shift, filling zeros from the left, so we never risk infinite loops or negative-value sign extension. The loop continues until the number becomes zero. Edge case: when input is `0`, the loop never runs and we return `0`. For a 32-bit integer, the loop runs at most 32 times (or exactly the number of bits until the highest set bit is processed). Alternative faster methods exist (like Brian Kernighan’s algorithm that clears the lowest set bit each iteration), but the simple bit-by-bit method is perfectly acceptable here. Time complexity is O(number of bits) = O(32) worst-case, and space complexity is O(1).
#include <cstdint>

// Count the number of 1-bits in the binary representation of a non-negative integer.
// Uses a simple bit-by-bit check, handling 0 correctly.
int countSetBits(int n) {
    // Treat as unsigned to ensure logical right shift (zero-fill) on all platforms.
    std::uint32_t value = static_cast<std::uint32_t>(n);
    int count = 0;

    while (value > 0) {
        // Increment count if the least significant bit is set.
        count += static_cast<int>(value & 1u);
        // Shift right by one bit, zero-fill from the left.
        value >>= 1u;
    }

    return count;
}
#include <cassert>

int main() {
    // Basic cases
    assert(countSetBits(0) == 0);          // 0 -> binary 0
    assert(countSetBits(1) == 1);          // 1 -> binary 1
    assert(countSetBits(2) == 1);          // 2 -> binary 10
    assert(countSetBits(3) == 2);          // 3 -> binary 11
    assert(countSetBits(7) == 3);          // 7 -> binary 111
    assert(countSetBits(8) == 1);          // 8 -> binary 1000
    assert(countSetBits(15) == 4);         // 15 -> binary 1111
    // Larger and mixed-bit numbers
    assert(countSetBits(255) == 8);        // 255 -> 11111111
    assert(countSetBits(1024) == 1);       // 1024 -> 10000000000
    // Negative input: treated as 32-bit unsigned pattern, e.g., -1 is all 32 bits set
    assert(countSetBits(-1) == 32);        // -1 -> all 32 bits are 1
    assert(countSetBits(-2) == 31);        // -2 -> 111...1110 (31 ones)
    return 0;
}
