// Write a C++ function named `toggleRightmostSetBit` that accepts a single non-zero positive integer `n` and returns a new integer obtained by flipping (toggling) only the rightmost (least significant) set bit of `n` from `1` to `0`. All other bits must remain unchanged. The function should work for any positive integer that fits in a signed 32-bit `int`. If the input is `0`, the behavior is undefined, so you may assume the input is always positive. Provide a self-contained implementation with proper `const` correctness and a descriptive comment.

// The key insight is to isolate the rightmost set bit using the bitwise property: for any integer `n`, the expression `n & (-n)` yields a number with only that rightmost `1` bit set (all other bits are `0`). This works because two's complement negation flips all bits and adds `1`, causing the rightmost `1` in `n` to remain the only set bit in the result. Once we have that isolated bit, we can toggle it off by using XOR (`^`) with `n`, because XORing with a mask that has exactly one bit set will flip that bit. For example, if `n = 12` (binary `1100`), `n & (-n)` equals `4` (`0100`), and `12 ^ 4` equals `8` (`1000`). Edge cases: The input may be a power of two (e.g., `8`), in which case the result becomes `0`; that is valid. The input is guaranteed positive and non-zero, so no special handling of zero is needed, but if extended to zero, the expression would fail, so the contract excludes it. Time complexity is O(1) (constant number of bitwise operations), and auxiliary space is O(1).

// Toggle the rightmost set bit of a positive integer n to 0.
// Assumes n is a positive (non-zero) 32-bit signed integer.
int toggleRightmostSetBit(int n) {
    // Isolate the rightmost set bit: only that bit is 1, all others 0.
    const int rightmostBit = n & (-n);
    // XOR flips that bit from 1 to 0 while leaving others unchanged.
    return n ^ rightmostBit;
}

#include <cassert>

int toggleRightmostSetBit(int n);

int main() {
    // Rightmost set bit of 1 (binary 1) -> 0
    assert(toggleRightmostSetBit(1) == 0);
    // 2 (10) -> 0
    assert(toggleRightmostSetBit(2) == 0);
    // 3 (11) -> 2 (10)
    assert(toggleRightmostSetBit(3) == 2);
    // 4 (100) -> 0
    assert(toggleRightmostSetBit(4) == 0);
    // 5 (101) -> 4 (100)
    assert(toggleRightmostSetBit(5) == 4);
    // 6 (110) -> 4 (100)
    assert(toggleRightmostSetBit(6) == 4);
    // 7 (111) -> 6 (110)
    assert(toggleRightmostSetBit(7) == 6);
    // 8 (1000) -> 0
    assert(toggleRightmostSetBit(8) == 0);
    // 12 (1100) -> 8 (1000)
    assert(toggleRightmostSetBit(12) == 8);
    // 255 (11111111) -> 254 (11111110)
    assert(toggleRightmostSetBit(255) == 254);
    return 0;
}
