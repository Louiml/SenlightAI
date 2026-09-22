/*
Write a C++ function named `isPerfectSquare` that takes a single positive integer `n` and returns `true` if `n` is a perfect square (i.e., the square of an integer), and `false` otherwise. The function must handle `n = 1` correctly (which is a perfect square) and must be efficient for values up to 2<sup>31</sup>−1 (i.e., the maximum `int`). You may not use a loop that iterates up to `n`; instead, use the integer square root via binary search or the standard `sqrt` function with careful rounding, and verify by multiplication to avoid floating-point precision errors. The solution must be a standalone free function with `const` correctness (the parameter should be passed by value or `const int`, and the function should be `const`-qualified if it were a member, but as a free function just ensure no mutation). Provide only the function definition, no `main`.
*/

#include <cstdint>

// Returns true if n is a perfect square, false otherwise.
// Precondition: n > 0.
bool isPerfectSquare(const int n) {
    // Binary search for the integer square root in [1, n].
    int low = 1;
    int high = n;
    while (low <= high) {
        // Use long long to avoid overflow in mid*mid.
        int mid = low + (high - low) / 2;
        long long square = static_cast<long long>(mid) * mid;
        if (square == n) {
            return true;
        } else if (square < n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return false;
}

#include <cassert>

int main() {
    // Small perfect squares
    assert(isPerfectSquare(1) == true);
    assert(isPerfectSquare(4) == true);
    assert(isPerfectSquare(9) == true);
    // Non-squares
    assert(isPerfectSquare(2) == false);
    assert(isPerfectSquare(3) == false);
    assert(isPerfectSquare(10) == false);
    // Larger values near boundaries
    assert(isPerfectSquare(2147395600) == true); // 46340^2
    assert(isPerfectSquare(2147483647) == false); // INT_MAX, not a square
    // Edge: a large perfect square just below INT_MAX
    assert(isPerfectSquare(2147395600) == true);
    // Edge: n=1 (already tested) and n=0 is out of scope, but test 1 again
    assert(isPerfectSquare(1) == true);
    // Check a non-square just above a perfect square
    assert(isPerfectSquare(2147395601) == false);
    return 0;
}

// The core idea is to compute the integer square root of `n` and check whether squaring it equals `n`. Because floating-point `sqrt` can suffer from rounding errors near perfect squares, the safest approach is to compute `int root = static_cast<int>(std::sqrt(n))` and then check `root * root == n`; but since `root` may be off by one due to rounding, we also check `(root + 1) * (root + 1) == n` and `(root - 1) * (root - 1) == n` (the latter only if `root > 1`). Alternatively, a binary search in `[1, n]` finds the exact integer root in `O(log n)` time with no floating-point issues. Edge cases: `n = 1` (root = 1, true), `n = 0` (if allowed, but task says positive, so ignore), `n` being a large prime (binary search still works), and `n` being a perfect square like 2<sup>30</sup> (root = 32768). Time complexity: if using binary search, O(log n) per call; if using `sqrt` with correction, O(1). Space complexity: O(1). For robustness and clarity, the reference solution uses binary search to avoid any floating-point pitfalls, making it fully deterministic and safe for all `int` values.
