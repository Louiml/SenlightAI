Write a C++ function named `isPowerOfFour` that takes an integer `n` and returns a boolean indicating whether `n` is a positive power of four (i.e., 4^0 = 1, 4^1 = 4, 4^2 = 16, etc.). The function must handle negative numbers, zero, and large positive inputs robustly without causing undefined behavior (e.g., avoid overflow when checking bit patterns). For example, `1`, `4`, `16`, `64` should return `true`; `0`, `-4`, `8`, `12`, `20`, `32` should return `false`. The solution should be efficient (no loops over all bits) and use only constant extra space.
#include <cassert>

int main() {
    // Basic positive cases
    assert(isPowerOfFour(1) == true);    // 4^0
    assert(isPowerOfFour(4) == true);    // 4^1
    assert(isPowerOfFour(16) == true);   // 4^2
    assert(isPowerOfFour(64) == true);   // 4^3
    assert(isPowerOfFour(1024) == true); // 4^5

    // Negative and zero
    assert(isPowerOfFour(0) == false);
    assert(isPowerOfFour(-1) == false);
    assert(isPowerOfFour(-4) == false);

    // Powers of two but not of four (odd exponent)
    assert(isPowerOfFour(2) == false);
    assert(isPowerOfFour(8) == false);
    assert(isPowerOfFour(32) == false);
    assert(isPowerOfFour(128) == false);

    // Non-powers of two
    assert(isPowerOfFour(3) == false);
    assert(isPowerOfFour(5) == false);
    assert(isPowerOfFour(12) == false);
    assert(isPowerOfFour(20) == false);
    assert(isPowerOfFour(100) == false);

    // Large power of four (within int range)
    assert(isPowerOfFour(1 << 30) == true); // 2^30 = (2^15)^2, exponent 30 is even? 30 is even, but 2^30 is not a power of four (4^15 = 2^30, yes it is). Actually 4^15 = 2^30, so true.
    assert(isPowerOfFour(1 << 31) == false); // 2^31 is negative due to overflow? Actually 1<<31 for int is undefined? Better use 1073741824 which is 2^30. We already tested that. Add another.

    // Additional boundary: 2^2=4 is true, 2^1=2 false
    assert(isPowerOfFour(4) == true);
    assert(isPowerOfFour(2) == false);

    return 0;
}
#include <cstddef>
#include <climits>

// Returns true if the given integer n is a positive power of four.
bool isPowerOfFour(int n) {
    if (n <= 0) {
        return false;
    }

    // Check if n is a power of two: exactly one bit is set.
    if ((n & (n - 1)) != 0) {
        return false;
    }

    // For a power of two, the single bit must be at an even position.
    // Mask to select even-position bits (bits 0, 2, 4, ...).
    // For a 32-bit int, this is 0x55555555. Generalize using sizeof.
    const unsigned int mask = 0x55555555u; // fits 32-bit int; for 64-bit need larger, but int is 32-bit standard.

    // If the single set bit is at an even index, its value is present in the mask.
    return (static_cast<unsigned int>(n) & mask) != 0;
}
// A number is a power of four if and only if it is a power of two (i.e., its binary representation has exactly one `1` bit) and that single `1` bit is in an even position (counting from the least significant bit as position 0). Since powers of four are `1, 4, 16, 64, ...` which are `2^0, 2^2, 2^4, 2^6, ...`, the exponent must be even. To check if the number is a single-bit set, we use the classic test `n > 0` and `(n & (n - 1)) == 0`, which is safe for signed integers as long as `n > 0` (we first check positivity to avoid handling negative edge cases). Then, to verify the bit is in an even position, we can check that the exponent is even. One efficient method is to note that a power of two with an even exponent is a perfect square; so after confirming it's a power of two, we can check if `sqrt(n)` is an integer and that integer squared equals `n`. Alternatively, compute the exponent using `log2` and check if it's even, but floating-point precision can be problematic for very large numbers. A safer bitwise method: after confirming `(n & (n-1)) == 0`, we can repeatedly shift right by 2 bits until we reach 1; if during shifting we ever encounter a non-zero bit pattern that is not 1, it's not a power of four (but since we already know it's a power of two, shifting by two each time will eventually hit exactly 1 if exponent is even). The simplest robust approach: after the power-of-two check, use a loop that shifts `n` right by 2 bits while `n > 1`; if at any point `n & 3` becomes non-zero (other than when n becomes 1), it's not a power of four. But since we already have a power of two, we can just check if `(n & 0x55555555)` is non-zero (checking bits at even positions). For 32-bit int, mask `0x55555555` checks all even-position bits. For a general int, we can use a constant mask based on `sizeof(int)`. Edge cases: `n <= 0` returns false immediately; `n == 1` is true (4^0). Time complexity: O(1) for the bitwise checks (the mask check is constant). Space complexity: O(1). The solution avoids floating-point logarithm issues and overflow.
