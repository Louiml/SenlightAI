// Write a C++ function named `isPowerOfFour` that takes a single integer `n` (which may be negative or zero) and returns a `bool` indicating whether `n` is an exact power of 4 (i.e., `4^k` for some non-negative integer `k`, including `1`, `4`, `16`, ...). The function must handle `int` values safely and should not rely on floating-point arithmetic, loops, or recursion. The solution should use only bitwise and arithmetic operations, and the logic must be based on the mathematical property that a positive power of 4 is always a power of 2 and satisfies `(n-1) % 3 == 0` (because `4^k ≡ 1 mod 3`). Provide a self-contained implementation with a clear helper comment, and ensure it works correctly for the full `int` range, including edge cases like `0`, negative numbers, and `INT_MAX`.

The key insight is that powers of 4 are a subset of powers of 2. For any `n > 0`, checking if it is a power of 2 can be done with `__builtin_popcountll(static_cast<unsigned long long>(n)) == 1` (or equivalently `(n & (n - 1)) == 0`). Once we know it is a power of 2, we must determine if the exponent is even (since `4^k = 2^(2k)`). The property `(n - 1) % 3 == 0` works because for any power of 4, `n ≡ 1 (mod 3)`, while for powers of 2 that are not powers of 4 (i.e., `2^(odd)`), `n ≡ 2 (mod 3)` or `n ≡ 1 (mod 3)`? Specifically: for `k ≥ 0`, `4^k ≡ 1 (mod 3)`. For `2^m` with `m` odd, `2^1 = 2 ≡ 2 (mod 3)`, `2^3 = 8 ≡ 2 (mod 3)`, etc., so they do not satisfy the congruence. For `m = 0`, `n = 1` is a power of 4 (since `4^0 = 1`). Edge cases: `n <= 0` returns false because powers of 4 are positive. `n = 1` passes the power-of-2 check and `(1-1)%3 == 0` is true. `n = 0` fails the `n > 0` check. Negative numbers fail `n > 0`. `INT_MAX` (2147483647) is not a power of 2, so it fails early. The time complexity is O(1) and space complexity is O(1). The use of `__builtin_popcountll` handles large `int` values safely when cast to `unsigned long long`.

#include <cstdint>

// Returns true if n is a positive integer that is an exact power of 4 (1,4,16,...).
// Uses: power of 2 check + congruence modulo 3 property.
bool isPowerOfFour(int n) {
    // Must be positive, and exactly one bit set (power of 2).
    if (n <= 0) return false;
    if (__builtin_popcountll(static_cast<unsigned long long>(n)) != 1) return false;
    // For n = 4^k, n ≡ 1 (mod 3). Handles 1 (4^0) correctly.
    return (n - 1) % 3 == 0;
}

#include <cassert>

int main() {
    // Positive powers of 4.
    assert(isPowerOfFour(1) == true);
    assert(isPowerOfFour(4) == true);
    assert(isPowerOfFour(16) == true);
    assert(isPowerOfFour(64) == true);
    assert(isPowerOfFour(1024) == true);
    // Powers of 2 that are NOT powers of 4.
    assert(isPowerOfFour(2) == false);
    assert(isPowerOfFour(8) == false);
    assert(isPowerOfFour(32) == false);
    assert(isPowerOfFour(128) == false);
    // Non-powers of 2.
    assert(isPowerOfFour(0) == false);
    assert(isPowerOfFour(-4) == false);
    assert(isPowerOfFour(3) == false);
    assert(isPowerOfFour(5) == false);
    assert(isPowerOfFour(12) == false);
    // Large values.
    assert(isPowerOfFour(2147483647) == false);
    assert(isPowerOfFour(1073741824) == true); // 4^15 = 2^30
    assert(isPowerOfFour(536870912) == false); // 2^29
    return 0;
}
