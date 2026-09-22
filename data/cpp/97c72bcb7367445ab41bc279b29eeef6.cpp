// Write a C++ function named `isPowerOfFour` that takes a single integer `n` and returns a boolean value indicating whether `n` is a positive power of four (i.e., 4^k for some integer k ≥ 0). The function must handle all 32-bit signed integer inputs, including zero and negative values, returning `false` for them. You are not allowed to use loops, recursion, or built-in logarithm functions; instead, use bitwise operations and integer arithmetic to determine the result efficiently. Additionally, ensure the function is `const`-correct and does not modify its input.
A positive integer is a power of four if it is a power of two and its only set bit is at an even position (counting from 0, where bit 0 is the least significant bit). We can check for a power of two first using `(n & (n - 1)) == 0`, which is true only when `n` has exactly one set bit (and `n > 0`). Then, to verify that this set bit is at an even index, we can use a mask with alternating bits `0x55555555` (binary `0101...0101`). If `n & 0x55555555` is non-zero, the unique set bit is at an even position, confirming a power of four. Edge cases: `n = 0` fails the power-of-two check because `(0 & -1) == 0`, but we explicitly require `n > 0`. Negative numbers fail due to the sign bit or the `n > 0` condition. For example, `n = 1` (4^0) has bit 0 set, which is in the mask, so returns true. `n = 2` has bit 1 set, which is not in the mask, so returns false. Time complexity is O(1) and space complexity is O(1).
#include <cstdint>

// Returns true if n is a positive power of four (4^k for k >= 0).
bool isPowerOfFour(int n) {
    // Must be positive and have exactly one set bit (power of two).
    if (n <= 0) return false;
    if ((n & (n - 1)) != 0) return false;
    // The single set bit must be at an even position (0-indexed).
    return (n & 0x55555555) != 0;
}
#include <cassert>

int main() {
    assert(isPowerOfFour(1) == true);      // 4^0
    assert(isPowerOfFour(4) == true);      // 4^1
    assert(isPowerOfFour(16) == true);     // 4^2
    assert(isPowerOfFour(64) == true);     // 4^3
    assert(isPowerOfFour(0) == false);     // zero
    assert(isPowerOfFour(-4) == false);    // negative
    assert(isPowerOfFour(2) == false);     // power of two but not four
    assert(isPowerOfFour(8) == false);     // power of two but not four
    assert(isPowerOfFour(2147483647) == false); // max int, not power of four
    assert(isPowerOfFour(1073741824) == true);  // 4^15, fits in int
    return 0;
}
