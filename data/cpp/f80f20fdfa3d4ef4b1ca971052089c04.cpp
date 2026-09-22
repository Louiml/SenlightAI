Write a C++ function named `isPowerOfThree` that takes a single `int` parameter `n` and returns a `bool` indicating whether `n` is a power of three (i.e., `3^k` for some non-negative integer `k`). The function must correctly handle all possible `int` values, including zero, negative numbers, and the largest representable integers. It should return `false` for any non-positive input immediately, and for positive inputs, it should divide by 3 repeatedly while divisible, finally checking if the result equals 1. This is a self-contained task; you only need to provide the function definition, no `main` is required.

#include <assert.h>

// The function under test (declared here for clarity; normally it would be included from a header)
bool isPowerOfThree(int n);

int main() {
    // Positive powers of three
    assert(isPowerOfThree(1) == true);       // 3^0
    assert(isPowerOfThree(3) == true);       // 3^1
    assert(isPowerOfThree(9) == true);       // 3^2
    assert(isPowerOfThree(81) == true);      // 3^4
    assert(isPowerOfThree(1162261467) == true); // 3^19, largest power fitting in int

    // Non-powers (including multiples of 3)
    assert(isPowerOfThree(0) == false);      // Non-positive
    assert(isPowerOfThree(-27) == false);    // Negative
    assert(isPowerOfThree(2) == false);      // Not divisible by 3
    assert(isPowerOfThree(6) == false);      // 2*3, not a power
    assert(isPowerOfThree(12) == false);     // 4*3, not a power

    // Edge: largest int and values that might cause overflow
    assert(isPowerOfThree(2147483647) == false); // Close to int max, not a power
    assert(isPowerOfThree(1162261468) == false); // Just above 3^19, not a power

    return 0;
}

#include <assert.h>

// Returns true if n is a non-negative integer power of 3 (3^k for k >= 0).
// Handles all int values: non-positive inputs yield false.
bool isPowerOfThree(int n) {
    if (n <= 0) {
        return false;
    }
    while (n % 3 == 0) {
        n /= 3;
    }
    return n == 1;
}

// The algorithm works by first rejecting all non-positive inputs since powers of three are always positive (3^0 = 1, and any positive exponent yields a positive integer). For positive `n`, we repeatedly divide by 3 as long as `n % 3 == 0`. If `n` is a power of three, this loop will eventually reduce it to exactly 1 (because 3^k / 3 repeatedly k times equals 1). If `n` is not a power of three, the loop stops at a value greater than 1 (e.g., 12 → 4 → loop stops because 4 % 3 != 0, result is 4). After the loop, we compare the final value to 1. Edge cases include `n = 0` (returns false immediately), `n = 1` (3^0, loop never runs, returns true), negative numbers (false), very large powers like 3^19 = 1162261467 (fits in `int`, returns true), and numbers like 2 or 6 that are multiples of 3 but not pure powers (6 → 2, returns false). The time complexity is O(log₃ n) because each division reduces the number by a factor of 3, and for the maximum `int` that's about 20 iterations. Space complexity is O(1) since only a few scalar variables are used.
