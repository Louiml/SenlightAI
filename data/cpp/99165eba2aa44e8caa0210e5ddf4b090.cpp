// Write a C++ function `long long countBinarySubstringsSum(int n)` that, for a non-negative integer `n`, returns the sum of the squared binomial coefficients \(\sum_{r=0}^{n} \binom{n}{r}^2\). The result must be computed using integer arithmetic only (no floating point). The function should handle `n` up to 33 so that intermediate and final values fit within a 64-bit signed integer (since \(\binom{33}{16} \approx 1.17 \times 10^9\), and the sum is the central binomial coefficient \(\binom{2n}{n}\), which for \(n=33\) is \(7.2 \times 10^{18}\), just below \(9.22 \times 10^{18}\) max of `long long`). For `n=0`, the sum is 1. The function must be efficient and avoid recomputing binomial coefficients from scratch for each term.
// The expression \(\sum_{r=0}^{n} \binom{n}{r}^2\) equals the central binomial coefficient \(\binom{2n}{n}\) (Vandermonde's identity). However, the provided snippet iteratively computes each binomial coefficient using a recurrence: \(\binom{n}{r} = \binom{n}{r-1} \cdot \frac{n-r+1}{r}\). For each `r` from 1 to `n`, we update `nCr` and add `nCr * nCr` to the result. The formula starts with `nCr = 1` (for `r=0`) and `res = 1` (the square of the first coefficient). This avoids using large factorials directly and works in integer arithmetic because the recurrence yields integer values at each step — division is exact due to mathematical properties. Edge case: for `n=0`, the loop does not execute, and `res` remains 1, which is correct. For `n>0`, careful handling of intermediate multiplication is needed: `nCr * (n + 1 - r)` can overflow if done naively before division, but since `n ≤ 33`, the maximum value of `nCr` is about \(1.17 \times 10^9\) and `(n+1-r) ≤ 33`, so the product is about \(3.9 \times 10^{10}\), which fits in `long long`. The sum `res` grows to ~\(7.2 \times 10^{18}\), which fits in signed 64-bit but must use `long long` (not `int`). Time complexity is O(n), space is O(1).
#include <cstdint>

// Returns sum_{r=0}^{n} (C(n, r))^2, where C(n, r) is the binomial coefficient.
// Uses iterative recurrence for binomial coefficients. Works for n up to 33.
long long countBinarySubstringsSum(int n) {
    // n must be non-negative.
    if (n < 0) {
        return 0; // Or handle as error; but spec says non-negative.
    }

    long long nCr = 1;       // C(n, 0)
    long long result = 1;    // (C(n, 0))^2

    for (int r = 1; r <= n; ++r) {
        // Update nCr to C(n, r) using the recurrence:
        // C(n, r) = C(n, r-1) * (n - r + 1) / r
        // Multiplication is safe for n <= 33.
        nCr = nCr * (n + 1 - r) / r;
        result += nCr * nCr;
    }

    return result;
}
int main() {
    // Base case
    assert(countBinarySubstringsSum(0) == 1);

    // Small known values: sum_{r=0}^{n} C(n,r)^2 = C(2n, n)
    assert(countBinarySubstringsSum(1) == 2);   // 1 + 1
    assert(countBinarySubstringsSum(2) == 6);   // 1 + 4 + 1
    assert(countBinarySubstringsSum(3) == 20);  // 1 + 9 + 9 + 1
    assert(countBinarySubstringsSum(4) == 70);  // 1 + 16 + 36 + 16 + 1

    // More values via C(2n, n)
    assert(countBinarySubstringsSum(5) == 252);      // C(10,5)
    assert(countBinarySubstringsSum(10) == 184756);  // C(20,10)

    // Boundary value: n=33 (max safe for 64-bit)
    assert(countBinarySubstringsSum(33) == 7219428434016265740LL);

    // Negative input handling (function returns 0 for invalid input)
    assert(countBinarySubstringsSum(-1) == 0);
}
