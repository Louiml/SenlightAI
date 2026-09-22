/*
Write a C++ function named `integerSquareRoot` that takes a single non-negative integer `x` and returns the greatest integer `r` such that `r * r <= x` (i.e., the floor of the square root of `x`). The function must work correctly for all possible 32-bit signed integer inputs (0 to 2147483647) without using floating-point arithmetic or the standard library’s `sqrt` or `pow` functions. The function must be `const`-correct, use only integer operations, and handle the edge case `x = 0` explicitly. You may assume the input is always non-negative.
*/

#include <cstdint>

// Returns the floor of the square root of x for x >= 0.
// Uses integer binary search; handles x = 0 explicitly.
int integerSquareRoot(int x) {
    if (x < 0) {
        return -1; // Not defined for negative inputs, but function contract says non-negative.
    }
    if (x == 0) {
        return 0;
    }
    // Highest possible integer square root for 32-bit signed int is 46340.
    int low = 1;
    int high = 46340;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        long long midSq = static_cast<long long>(mid) * mid;
        long long nextSq = static_cast<long long>(mid + 1) * (mid + 1);
        if (midSq <= x && x < nextSq) {
            return mid;
        } else if (midSq > x) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return 0; // Unreachable for x > 0.
}

#include <cassert>

int main() {
    // Edge cases
    assert(integerSquareRoot(0) == 0);
    assert(integerSquareRoot(1) == 1);
    // Perfect squares
    assert(integerSquareRoot(4) == 2);
    assert(integerSquareRoot(9) == 3);
    assert(integerSquareRoot(16) == 4);
    // Non-perfect squares
    assert(integerSquareRoot(2) == 1);
    assert(integerSquareRoot(8) == 2);
    assert(integerSquareRoot(15) == 3);
    // Large values
    assert(integerSquareRoot(2147483647) == 46340);
    assert(integerSquareRoot(46340 * 46340) == 46340);
    assert(integerSquareRoot(46340 * 46340 + 1) == 46340);
    // Typical values
    assert(integerSquareRoot(100) == 10);
    assert(integerSquareRoot(101) == 10);
    return 0;
}

// The problem asks for the integer floor of the square root, which can be found using binary search over the range of possible square roots. Since the maximum input is 2^31 - 1 ≈ 2.147e9, its square root is about 46340, so the search range can be safely bounded from `1` to `46340` (the largest integer whose square fits in a signed 32-bit integer; `46341^2 = 2147488281` exceeds INT_MAX, so it is not needed). For `x = 0`, the answer is directly `0`. For any `x > 0`, we perform binary search: initialize `low = 1` and `high = 46340`. While `low <= high`, compute `mid = low + (high - low) / 2` (avoiding overflow). If `mid * mid <= x` and `(mid + 1) * (mid + 1) > x`, then `mid` is the answer. If `mid * mid < x`, then the answer is at least `mid`, so set `low = mid + 1`; otherwise ( `mid * mid > x`), set `high = mid - 1`. Edge case: for `x = 1`, the binary search returns `1`; for `x = 0`, the function returns `0` directly. Complexity: the binary search performs at most about 16 iterations (since log2(46340) ≈ 15.5), so time is O(log(max_value)) = O(16) ≈ O(1), and space is O(1). The use of `low + (high - low) / 2` avoids any integer overflow in the calculation of `mid`.
