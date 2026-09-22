/*
Write a C++ function named `harmonicSeriesSum` that takes an integer `n` (where `1 <= n <= 1000`) and returns the harmonic series sum: `1 + 1/2 + 1/3 + ... + 1/n`, rounded to exactly 2 decimal places. The function should return the result as a `double` rounded to two decimal places (e.g., for `n=2`, return `1.50`; for `n=3`, return `1.83`). Additionally, the function must handle the edge case where `n` is 0 by returning `0.00`. The function must not use any loops with integer division that could truncate the fractions—ensure each term is computed as a floating-point division.
*/
#include <cmath>   // for std::round
#include <stdexcept> // for std::invalid_argument (optional, but not needed)

// Compute harmonic series sum up to n terms, rounded to 2 decimal places.
double harmonicSeriesSum(int n) {
    if (n < 0) {
        n = 0; // treat negative input as zero
    }

    double sum = 0.0;
    for (int i = 1; i <= n; ++i) {
        sum += 1.0 / i; // floating-point division
    }

    // Round to 2 decimal places
    double rounded = std::round(sum * 100.0) / 100.0;
    return rounded;
}
#include <cassert>
#include <cmath>

int main() {
    // Basic cases
    assert(harmonicSeriesSum(1) == 1.00);
    assert(harmonicSeriesSum(2) == 1.50);
    assert(harmonicSeriesSum(3) == 1.83);
    assert(harmonicSeriesSum(4) == 2.08);

    // Edge cases
    assert(harmonicSeriesSum(0) == 0.00);
    assert(harmonicSeriesSum(-5) == 0.00);

    // Larger n
    assert(harmonicSeriesSum(10) == 2.93);
    assert(harmonicSeriesSum(100) == 5.19);

    // Verify with known value for n=1000 (approx 7.485470860550...)
    assert(std::abs(harmonicSeriesSum(1000) - 7.49) < 0.01);
}
// The solution computes the harmonic sum by iterating from 1 to `n` and adding `1.0 / i` to an accumulator, which is initialized to `0.0`. Because `i` is an integer, using `1.0 / i` ensures floating-point division. To round to two decimal places, use `std::round(value * 100.0) / 100.0` before returning. Edge cases:  
// - If `n == 0`, the loop does not execute, and the sum remains `0.0`, which rounds to `0.00`.  
// - For negative `n`, treat it like `0` (or optionally clamp to 0) to avoid an infinite loop.  
// - Time complexity: `O(n)` because we perform `n` additions.  
// - Space complexity: `O(1)` since we only use a few scalar variables.  
// - Precision: Using `double` is sufficient for `n <= 1000`; rounding after multiplication by 100 and dividing back produces consistent two-decimal results.
