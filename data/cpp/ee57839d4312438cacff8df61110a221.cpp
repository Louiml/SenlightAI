/*
Write a C++ function named `reverseBits` that takes a 32-bit unsigned integer (`uint32_t`) and returns a new 32-bit unsigned integer whose bits are the exact reverse of the input's bits (i.e., bit 0 becomes bit 31, bit 1 becomes bit 30, and so on). The function should operate on all 32 bits, including leading zeros, and must not rely on any predefined bit-reversal library functions. Your implementation should work for any possible `uint32_t` value, including 0 (whose reversal is also 0) and values with only a single set bit at any position. You must implement the reversal using a loop that swaps the outermost unmatched bit pairs, using bitwise operations (shifts, AND, OR, XOR) and avoiding any arithmetic operations on the integer.
*/

#include <cstdint>

// Reverse the order of all 32 bits in a 32-bit unsigned integer.
// Returns a new value with bits reversed (bit 0 <-> bit 31, bit 1 <-> bit 30, etc.).
uint32_t reverseBits(uint32_t n) {
    // Iterate over the first half of the bits (0..15)
    for (int i = 0; i < 16; ++i) {
        // Get the bit at position i and the mirrored position (31 - i)
        const uint32_t low_bit = (n >> i) & 1u;
        const uint32_t high_bit = (n >> (31 - i)) & 1u;
        
        // If they differ, swap them using XOR with a mask of 1s at both positions
        if (low_bit != high_bit) {
            const uint32_t mask = (1u << i) | (1u << (31 - i));
            n ^= mask;
        }
    }
    return n;
}

#include <cassert>
#include <cstdint>

// Declaration of the function being tested (reuse the solution's implementation or include it)
uint32_t reverseBits(uint32_t n); // Assume the solution is included above

int main() {
    // Case 1: n = 0 -> all bits reversed is still 0
    assert(reverseBits(0) == 0);

    // Case 2: n = 1 (bit 0 set) -> becomes 0x80000000 (bit 31 set)
    assert(reverseBits(1u) == 0x80000000u);

    // Case 3: n = 0x80000000 (bit 31 set) -> becomes 1 (bit 0 set)
    assert(reverseBits(0x80000000u) == 1u);

    // Case 4: n = 0xFFFFFFFF (all bits set) -> reversed is still all bits set
    assert(reverseBits(0xFFFFFFFFu) == 0xFFFFFFFFu);

    // Case 5: n = 7 (binary 0...0111) -> reversed becomes 0xE0000000 (binary 1110...0)
    assert(reverseBits(7u) == 0xE0000000u); // 3758096384 in decimal

    // Case 6: n = 0x12345678 -> manually verify: reverse of 0x12345678 is 0x1E6A2C48
    assert(reverseBits(0x12345678u) == 0x1E6A2C48u);

    // Case 7: n = 0x0000FFFF -> reversed is 0xFFFF0000
    assert(reverseBits(0x0000FFFFu) == 0xFFFF0000u);

    // Case 8: n = 0xFFFF0000 -> reversed is 0x0000FFFF
    assert(reverseBits(0xFFFF0000u) == 0x0000FFFFu);

    // Case 9: n = 0x55555555 (alternating bits) -> reversed is 0xAAAAAAAA
    assert(reverseBits(0x55555555u) == 0xAAAAAAAAu);

    // Case 10: n = 0xAAAAAAAA -> reversed is 0x55555555
    assert(reverseBits(0xAAAAAAAAu) == 0x55555555u);

    return 0;
}

// The solution approach is to iterate over the first 16 bit positions (from bit 0 to bit 15) and for each position `i`, compare the bit at position `i` with the bit at the mirrored position `31 - i`. If they differ, we need to swap them. A swap can be done by toggling both bits using XOR with a mask that has 1s at exactly those two positions. Specifically, we compute the mask `(1u << i) | (1u << (31 - i))` and XOR it with `n`. If the bits are the same, no action is needed. This works because XOR with 1 flips a bit, and flipping both bits effectively swaps their values (since they are different). Edge cases: when `n` is 0, all bits are equal (0), so no swap occurs and the result is 0. When `n` has the highest bit (bit 31) set, it moves to bit 0. The algorithm handles all inputs uniformly. Time complexity is O(1) because the loop runs exactly 16 iterations (constant). Space complexity is O(1) since we only use a few local variables.
