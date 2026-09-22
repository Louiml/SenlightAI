Write a C++ function that computes the integer square root of a non-negative integer using binary search, then refines the result to a specified number of decimal places using a linear-precision adjustment method. The function signature should be `double sqrtWithPrecision(int n, int precision)`, where `n` is the number to compute the square root of (non-negative), and `precision` is the number of digits after the decimal point to include in the result (non-negative, defaulting to 0). The function must return the square root as a `double`, rounded down (floor) at the final decimal place, not rounded to nearest. Handle edge cases such as `n = 0` (return 0) and `n = 1` (return 1) without infinite loops. Do not use any floating-point math library functions like `sqrt`, `pow`, or `fmod`; rely only on integer arithmetic and simple division/multiplication. The function must be `const`-correct (no mutation of inputs) and should be self-contained with no external dependencies beyond standard headers.

#include <cassert>
#include <cmath>

int main() {
    // Basic integer square roots (precision = 0)
    assert(sqrtWithPrecision(0, 0) == 0.0);
    assert(sqrtWithPrecision(1, 0) == 1.0);
    assert(sqrtWithPrecision(4, 0) == 2.0);
    assert(sqrtWithPrecision(10, 0) == 3.0); // floor(sqrt(10)) = 3

    // With precision: check floor to 2 decimal places
    // sqrt(2) ≈ 1.4142, floor to 2 decimals = 1.41
    assert(fabs(sqrtWithPrecision(2, 2) - 1.41) < 1e-9);
    // sqrt(3) ≈ 1.732, floor to 2 decimals = 1.73
    assert(fabs(sqrtWithPrecision(3, 2) - 1.73) < 1e-9);
    // sqrt(5) ≈ 2.236, floor to 2 decimals = 2.23
    assert(fabs(sqrtWithPrecision(5, 2) - 2.23) < 1e-9);
    // sqrt(8) ≈ 2.828, floor to 2 decimals = 2.82
    assert(fabs(sqrtWithPrecision(8, 2) - 2.82) < 1e-9);
    // precision 1: sqrt(15) ≈ 3.873, floor to 1 decimal = 3.8
    assert(fabs(sqrtWithPrecision(15, 1) - 3.8) < 1e-9);
    // Precision 3: sqrt(10) ≈ 3.162277, floor to 3 decimals = 3.162
    assert(fabs(sqrtWithPrecision(10, 3) - 3.162) < 1e-9);
    // Large perfect square
    assert(fabs(sqrtWithPrecision(1000000, 0) - 1000.0) < 1e-9);
}

#include <cstdint>

// Compute the square root of n with the given number of decimal places.
// Returns floor(sqrt(n)) if precision == 0; otherwise returns the floor of
// sqrt(n) to 'precision' decimal places.
double sqrtWithPrecision(int n, int precision) {
    if (n < 0) return -1.0; // assume non-negative input, but guard anyway
    if (n == 0) return 0.0;
    if (n == 1) return 1.0;

    // Stage 1: integer square root via binary search.
    int low = 0;
    int high = n;
    int ans = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int64_t sq = static_cast<int64_t>(mid) * mid;
        if (sq == n) {
            ans = mid;
            break;
        } else if (sq < n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    // Stage 2: refine with precision.
    double result = static_cast<double>(ans);
    double factor = 1.0;
    for (int i = 0; i < precision; ++i) {
        factor /= 10.0;
        // Try adding increments of 'factor' while square stays below n.
        // Use a candidate value that we update only if the square is strictly less.
        for (double candidate = result; candidate * candidate < n; candidate += factor) {
            result = candidate;
        }
    }
    return result;
}

// The solution uses two stages. **Stage 1**: Integer square root via binary search on the range `[0, n]`. For each midpoint `mid`, compute `mid * mid` using `long long` to avoid overflow (since `n` could be up to `INT_MAX`). If `mid*mid == n`, return `mid`; if `< n`, store `mid` as the current best integer root and search the upper half; if `> n`, search the lower half. This finds the largest integer `k` such that `k*k <= n`, which is the floor of the true square root. **Stage 2**: Improve precision by starting from the integer root and iteratively trying to add fractional increments. For each decimal place from 1 to `precision`, use a factor `0.1^i`. For each possible fractional value, test if `(candidate + factor)^2 < n`; if so, keep increasing the candidate by `factor`. This is a greedy linear scan that, for each digit, finds the maximum digit such that the square stays below `n`. This is correct because the square function is strictly increasing for non-negative values, so we can use a simple incremental search. Important edge cases: `n = 0` and `n = 1` must be handled to avoid binary search infinite loops (binary search still works but the integer root is trivially correct); also `precision = 0` should return just the integer root as a `double`. Time complexity: Stage 1 is `O(log n)` for the binary search. Stage 2 is `O(precision * 10)` because for each decimal place we try at most 10 digits (from 0 to 9) before the square exceeds `n`; more formally it's `O(10 * precision)`. Overall `O(log n + 10*precision)`. Space complexity is `O(1)`.
