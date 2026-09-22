Write a C++ function `sumOfSquares(long long n)` that takes a single positive integer `n` and returns the sum of the squares of all integers from 1 to `n` inclusive, i.e., `1² + 2² + ... + n²`. The function must handle `n` up to at least `10^6` without overflow, and must return the result as a `long long`. The function should be self-contained and not rely on any global state. This task is inspired by a code snippet that repeatedly reads integers and prints the closed-form solution `n*(n+1)*(2*n+1)/6` for each valid input until a zero is encountered. Your implementation must use this closed-form formula to achieve O(1) time complexity, but must correctly handle the intermediate multiplication to avoid overflow for larger `n`. The function must be pure and deterministic.

The sum of squares from 1 to `n` has a well-known closed-form expression: `n*(n+1)*(2*n+1)/6`. This formula is derived from Faulhaber's formula and holds for all positive integers `n`. The main challenge is avoiding integer overflow during the multiplication step. Since `n` can be up to at least `10^6`, `n*(n+1)*(2*n+1)` can be as large as `(10^6)*(10^6+1)*(2*10^6+1) ≈ 2*10^18`, which exceeds the 32-bit signed integer range. Therefore, the function must use a 64-bit type (e.g., `long long`) for all intermediate computations. To further reduce overflow risk, one can divide one of the factors by 6 before performing the remaining multiplications, but since `6` divides the product exactly, a direct multiplication with `long long` is safe for `n ≤ 10^6` (the product is about `2e18`, which is less than `9.22e18` for `long long`). For maximal safety, we can perform the multiplication as `n * (n+1) / 2 * (2*n+1) / 3`, but the standard order also works within the stated constraints. Edge cases: `n = 1` returns 1; `n = 0` should return 0 if called (though not required), and negative inputs are not expected. Time complexity is O(1) and space complexity is O(1).

#include <cstdint>

// Returns the sum of squares from 1 to n (inclusive).
// Uses the closed-form formula: n*(n+1)*(2*n+1)/6.
// The product is computed in 64-bit to avoid overflow for n up to at least 1e6.
long long sumOfSquares(long long n) noexcept {
    if (n <= 0) {
        return 0;
    }
    // n, n+1, 2n+1 are all integers; the product is always divisible by 6.
    // Multiplying in this order fits in a signed 64-bit for n <= 1e6.
    return n * (n + 1) * (2 * n + 1) / 6;
}

#include <cassert>

// Free function from the solution.
long long sumOfSquares(long long n) noexcept;

int main() {
    // Basic cases.
    assert(sumOfSquares(1) == 1);
    assert(sumOfSquares(2) == 5);      // 1 + 4
    assert(sumOfSquares(3) == 14);     // 1 + 4 + 9
    assert(sumOfSquares(4) == 30);     // 1 + 4 + 9 + 16
    assert(sumOfSquares(5) == 55);     // 1 + 4 + 9 + 16 + 25

    // Larger value to ensure no overflow.
    assert(sumOfSquares(1000000LL) == 333333833333500000LL);

    // Edge case: zero (not required by spec, but safe).
    assert(sumOfSquares(0) == 0);

    // Negative input (not expected, but safe).
    assert(sumOfSquares(-5) == 0);

    return 0;
}
