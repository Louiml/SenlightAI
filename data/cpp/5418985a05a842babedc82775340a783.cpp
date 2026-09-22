// Write a standalone C++ function `piecewiseValue(double t, const double* times, const double* values, int n)` that, given a time `t` and two arrays `times` and `values` each of length `n` (where `n >= 2` and the times are strictly increasing), returns the piecewise linear interpolated value at `t`. The function must behave as follows: for `t < times[0]`, return `values[0]`; for `t >= times[n-1]`, return `values[n-1]`; for `t` between consecutive times `times[i]` and `times[i+1]` (inclusive on the left), linearly interpolate between `values[i]` and `values[i+1]`. The input arrays are not modified, and the function must be `const`-correct and not allocate dynamic memory. The function must handle duplicate times gracefully (if any two consecutive times are equal, return the value at that time). This is a simplified, generalized version of the interpolation logic used in the given snippet (which used fixed 19 breakpoints); your function works for arbitrary `n`.
#include <cassert>
#include <cstddef>

// Declaration of the function (would be in a header in practice)
double piecewiseValue(double t, const double* times, const double* values, std::size_t n);

int main() {
    // Example 1: strictly increasing breakpoints
    double times1[] = {0.0, 10.0, 20.0, 30.0};
    double values1[] = {5.0, 7.0, 6.0, 8.0};
    // Before first
    assert(piecewiseValue(-1.0, times1, values1, 4) == 5.0);
    // At first breakpoint
    assert(piecewiseValue(0.0, times1, values1, 4) == 5.0);
    // In first segment (should be 5 + 0.5*(7-5) = 6.0)
    assert(piecewiseValue(5.0, times1, values1, 4) == 6.0);
    // At second breakpoint
    assert(piecewiseValue(10.0, times1, values1, 4) == 7.0);
    // In second segment (should be 7 + 0.5*(6-7) = 6.5)
    assert(piecewiseValue(15.0, times1, values1, 4) == 6.5);
    // At last breakpoint
    assert(piecewiseValue(30.0, times1, values1, 4) == 8.0);
    // After last
    assert(piecewiseValue(40.0, times1, values1, 4) == 8.0);

    // Example 2: duplicate times
    double times2[] = {0.0, 5.0, 5.0, 10.0};
    double values2[] = {1.0, 2.0, 3.0, 4.0};
    // Exactly at duplicate time
    assert(piecewiseValue(5.0, times2, values2, 4) == 2.0);
    // Just after duplicate time (still should return value at that time? Actually t=6 is > times[2]=5 but < times[3]=10, the loop hits i=1 (times1==times2) and returns values[1]=2.0)
    assert(piecewiseValue(6.0, times2, values2, 4) == 2.0);
    // Before duplicate
    assert(piecewiseValue(2.5, times2, values2, 4) == 1.5); // interpolate between 0 and 5

    // Example 3: n=2 minimal case
    double times3[] = {1.0, 2.0};
    double values3[] = {10.0, 20.0};
    assert(piecewiseValue(0.5, times3, values3, 2) == 10.0);
    assert(piecewiseValue(1.5, times3, values3, 2) == 15.0);
    assert(piecewiseValue(2.5, times3, values3, 2) == 20.0);

    // Example 4: constant values
    double times4[] = {0.0, 1.0, 2.0};
    double values4[] = {5.0, 5.0, 5.0};
    assert(piecewiseValue(0.5, times4, values4, 3) == 5.0);
    assert(piecewiseValue(1.5, times4, values4, 3) == 5.0);
    assert(piecewiseValue(3.0, times4, values4, 3) == 5.0);

    return 0;
}
#include <cstddef>

// Generalized piecewise linear interpolation with constant extrapolation.
// times must be strictly increasing, except duplicates are allowed (then the value at that time is returned).
// n >= 2. Returns the interpolated value at time t.
double piecewiseValue(double t, const double* times, const double* values, std::size_t n) {
    // Handle times before the first breakpoint
    if (t < times[0]) {
        return values[0];
    }
    // Handle times after the last breakpoint
    if (t >= times[n - 1]) {
        return values[n - 1];
    }
    // Scan for the correct segment
    for (std::size_t i = 0; i < n - 1; ++i) {
        if (times[i + 1] == times[i]) {
            // Duplicate times: if t >= this time, it's effectively constant here
            // Since times are sorted, returning values[i] is safe for this segment
            return values[i];
        }
        if (t < times[i + 1]) {
            // Linear interpolation
            double ratio = (t - times[i]) / (times[i + 1] - times[i]);
            return values[i] + ratio * (values[i + 1] - values[i]);
        }
    }
    // Should never reach here, but for safety return last value
    return values[n - 1];
}
// The core algorithm is a single pass or binary search over the sorted `times` array. Since `times` is provided and assumed sorted (as stated in the task), we can use a linear scan from index `0` to `n-2` to find the first segment where `t < times[i+1]`. If found, compute the interpolation as `values[i] + (t - times[i]) / (times[i+1] - times[i]) * (values[i+1] - values[i])`. However, note that if `times[i+1] == times[i]` (duplicate), this division would be by zero; to handle this, when a duplicate is detected, simply return `values[i]` for any `t` in that region. Also, if `t` is less than `times[0]`, return `values[0]`, and if `t` is greater than or equal to `times[n-1]`, return `values[n-1]`. Edge cases include: `n=1` (but specification says `n>=2`), `t` exactly at a breakpoint (choose the left segment, so interpolation yields the exact value), and empty or null pointers (but assume valid input as per contract). Time complexity is `O(n)` due to linear scan, which is acceptable even for large `n` and simpler than binary search. Space complexity is `O(1)` since no extra storage is used. The function should be `static` or placed in a namespace to avoid clashes. The solution uses `const` for parameters and returns a `double`.
