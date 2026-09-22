/*
Write a C++ function `double nearestSquareRoot(int n, int precision)` that computes the square root of a non-negative integer `n` up to `precision` decimal places using a binary-search-based algorithm. The function should return a `double` value. For example, `nearestSquareRoot(10, 2)` should return approximately `3.16`, and `nearestSquareRoot(25, 3)` should return exactly `5.0` (since 25 is a perfect square). Handle edge cases such as `n = 0` (return `0.0`) and `n = 1` (return `1.0`), and ensure the precision parameter is non-negative (`p >= 0`). The solution must not use any built-in square root functions (e.g., `sqrt`) or pow for fractional exponents.
*/
#include <cstdint>

// Computes the square root of a non-negative integer n up to 'precision' decimal places.
// Uses binary search for the integer part, then iterative refinement for the fractional part.
// Preconditions: n >= 0, precision >= 0.
double nearestSquareRoot(int n, int precision) {
    // Binary search for the integer part (floor of sqrt).
    int low = 0;
    int high = n;
    double ans = 0.0; // will hold the final result, initially integer candidate

    while (low <= high) {
        int mid = low + (high - low) / 2; // avoid overflow
        if (static_cast<long long>(mid) * mid == static_cast<long long>(n)) {
            return static_cast<double>(mid); // perfect square
        } else if (static_cast<long long>(mid) * mid < static_cast<long long>(n)) {
            ans = static_cast<double>(mid); // candidate floor
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    // Refine the fractional part.
    double inc = 0.1;
    for (int times = 0; times < precision; ++times) {
        // Increase ans until ans^2 exceeds n, then step back once.
        while ((ans + inc) * (ans + inc) <= static_cast<double>(n)) {
            ans += inc;
        }
        ans -= inc; // step back to last valid value
        inc /= 10.0; // refine to next decimal place
    }

    return ans;
}
#include <cassert>
#include <cmath>

int main() {
    // Perfect squares
    assert(nearestSquareRoot(0, 2) == 0.0);
    assert(nearestSquareRoot(1, 3) == 1.0);
    assert(nearestSquareRoot(25, 4) == 5.0);
    assert(nearestSquareRoot(100, 1) == 10.0);

    // Non-perfect squares with different precision
    assert(std::abs(nearestSquareRoot(2, 2) - 1.41) < 0.01);
    assert(std::abs(nearestSquareRoot(10, 2) - 3.16) < 0.01);
    assert(std::abs(nearestSquareRoot(3, 4) - 1.7320) < 0.0001);

    // Precision 0 means integer part only
    assert(nearestSquareRoot(15, 0) == 3.0);

    // Large n (no overflow)
    assert(std::abs(nearestSquareRoot(123456789, 3) - 11111.111) < 0.001);

    // Edge case n=2 with precision 1
    assert(nearestSquareRoot(2, 1) == 1.4);

    return 0;
}
// The algorithm first finds the integer floor of the square root using binary search on the range `[0, n]`. Initialize `low = 0` and `high = n`, then repeatedly check the middle value `mid`. If `mid * mid == n`, return `mid` immediately (perfect square). If `mid * mid < n`, store `mid` as the current best integer candidate and move `low = mid + 1`; otherwise move `high = mid - 1`. After the loop, `ans` holds the largest integer whose square ≤ `n`. For the fractional part, start with `inc = 0.1` and repeat `precision` times: while `(ans + inc) * (ans + inc) <= n`, add `inc` to `ans`. After the loop, subtract one `inc` (since the loop overshoots), then divide `inc` by 10 for finer precision. Edge cases: `n = 0` and `n = 1` are handled naturally because the binary search works; for `n = 0`, `low=high=0`, mid*mid==0 returns 0. For precision `p=0`, no fractional loop runs. Complexity: binary search on integers takes O(log n) time, and the fractional loop runs `p` iterations each with at most 10 increments (since incrementing from 0.1 to 0.01 etc., each step needs at most 10 additions), so total time O(log n + 10p), space O(1).
