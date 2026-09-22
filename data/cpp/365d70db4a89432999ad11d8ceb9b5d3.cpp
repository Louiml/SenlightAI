// Write a C++ function named `computeFactorial` that takes a non-negative integer `n` (with `0 <= n <= 18`) and returns its factorial as a `long long`. The function must compute the result iteratively in O(n) time, without using recursion or any built-in factorial function. Handle the base cases `n == 0` and `n == 1` explicitly, and ensure the result fits within the `long long` range for all valid inputs (18! = 6,402,373,705,728,000 < 9.22 × 10^18). The function should be `const`-correct and include appropriate comments. No `main` function is required; only the free function definition with necessary headers.
// The solution uses an iterative loop starting from 2 up to `n`, multiplying an accumulator initialized to 1 by each integer `i`. This directly computes `n!` by repeated multiplication. Edge cases: for `n == 0` or `n == 1`, the loop is skipped (or we can return 1 early), as the factorial of both is 1. Since the valid input range is bounded (0–18), the `long long` type safely holds the maximum result (18! ≈ 6.4 × 10^15), which is well below the 64-bit signed maximum. Time complexity is O(n) because the loop runs `n-1` times for `n ≥ 2`. Space complexity is O(1), only using a single accumulator variable. No recursion avoids stack overhead and potential overflow for large `n`.
#include <cstdint>  // for int64_t / long long guarantees

// Compute the factorial of a non-negative integer n (0 <= n <= 18).
// Returns n! as a long long. O(n) time, O(1) space.
long long computeFactorial(int n) {
    long long result = 1;  // 0! = 1! = 1
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}
#include <cassert>

int main() {
    assert(computeFactorial(0) == 1);
    assert(computeFactorial(1) == 1);
    assert(computeFactorial(2) == 2);
    assert(computeFactorial(5) == 120);
    assert(computeFactorial(10) == 3628800);
    assert(computeFactorial(18) == 6402373705728000LL);
    assert(computeFactorial(3) == 6);
    assert(computeFactorial(7) == 5040);
    assert(computeFactorial(12) == 479001600);
    assert(computeFactorial(15) == 1307674368000LL);
    return 0;
}
