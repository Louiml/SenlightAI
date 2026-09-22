/*
Write a C++ function that takes four floating-point numbers as parameters (named `a`, `b`, `c`, and `d`) and returns their arithmetic mean as a `double`. The function must be pure (no input/output), handle any finite `double` values including negatives and zeros, and compute the sum using a separate variable that is initialized to `0.0`. The function should be named `arithmeticMean` and must use `const` references for the parameters to avoid unnecessary copies. The function should work correctly even if the values are extremely large, so prefer adding in a stable order (smallest to largest) to reduce floating-point rounding error, though simple sequential addition is acceptable for typical cases.
*/
#include <algorithm>
#include <array>

// Compute the arithmetic mean of four floating-point numbers.
// Parameters are const references to avoid copying and to guarantee no modification.
// Values are sorted in ascending order before summation to minimize rounding error.
double arithmeticMean(const double& a, const double& b, const double& c, const double& d) {
    // Store in an array for easy sorting
    std::array<double, 4> values = {a, b, c, d};
    std::sort(values.begin(), values.end());

    double sum = 0.0;
    for (double v : values) {
        sum += v;
    }
    return sum / 4.0;
}
#include <cassert>
#include <cmath>

int main() {
    // Basic positive numbers
    assert(std::abs(arithmeticMean(2.0, 4.0, 6.0, 8.0) - 5.0) < 1e-9);
    // Negative numbers
    assert(std::abs(arithmeticMean(-1.0, -3.0, -5.0, -7.0) - (-4.0)) < 1e-9);
    // Mixed sign
    assert(std::abs(arithmeticMean(-10.0, 0.0, 10.0, 20.0) - 5.0) < 1e-9);
    // All zeros
    assert(arithmeticMean(0.0, 0.0, 0.0, 0.0) == 0.0);
    // Repeated values
    assert(std::abs(arithmeticMean(3.5, 3.5, 3.5, 3.5) - 3.5) < 1e-9);
    // One large and one small (test stability)
    assert(std::abs(arithmeticMean(1e308, 1e308, 1e308, -1e308) - 5e307) < 1e292);
    // Fractional values
    assert(std::abs(arithmeticMean(1.5, 2.5, 3.5, 4.5) - 3.0) < 1e-9);
    return 0;
}
// The core algorithm is straightforward: sum all four numbers and divide by 4. To improve numerical stability, we can first sort the four values (or just add them in ascending order) to reduce catastrophic cancellation when mixing very large and very small numbers. However, for most practical cases, simple sequential addition is fine. Edge cases include all zeros (mean = 0), negative numbers (sum and mean will be negative), and mixed signs. The function should return a `double` because the mean of integers can be fractional. Time complexity is O(1) with constant space O(1) because we only perform a fixed number of arithmetic operations. Since the number of inputs is fixed at four, no loops are needed. The main potential issue is floating-point precision; we mitigate this by sorting the values before summation.
