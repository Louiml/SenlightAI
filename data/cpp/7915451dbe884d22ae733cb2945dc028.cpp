/*
Write a C++ function named `computeFloor` that accepts a single `double` argument and returns its mathematical floor value as a `double`. The function must handle positive numbers, negative numbers, zero, fractional values, and very large magnitudes. You are not allowed to use the standard library `std::floor` or `std::trunc` inside the implementation; instead, you must compute the result using type casting and arithmetic. Include appropriate handling for values that are already integers (the result should be unchanged) and for negative fractional values (e.g., `-3.7` should yield `-4.0`, not `-3.0`). The function should be declared `const`-correct and should not modify its input.
*/

#include <cstdint>
#include <limits>

// Compute the mathematical floor of a double without using std::floor.
// Handles positive, negative, zero, and extreme magnitude values.
double computeFloor(double x) {
    // For very large magnitudes (beyond long long range), the double
    // is already an integer due to precision loss, so return it unchanged.
    if (x >= static_cast<double>(std::numeric_limits<long long>::max()) ||
        x <= static_cast<double>(std::numeric_limits<long long>::min())) {
        return x;
    }

    long long truncated = static_cast<long long>(x);

    // If x is non-negative, truncation toward zero equals floor.
    if (x >= 0.0) {
        return static_cast<double>(truncated);
    }

    // For negative x, if the truncated value equals the original,
    // then x is already an integer; otherwise subtract 1.0.
    if (static_cast<double>(truncated) == x) {
        return static_cast<double>(truncated);
    }
    return static_cast<double>(truncated - 1);
}

#include <cassert>
#include <cmath>

int main() {
    // Positive fractions and integers
    assert(computeFloor(3.0) == 3.0);
    assert(computeFloor(3.7) == 3.0);
    assert(computeFloor(0.5) == 0.0);

    // Negative fractions and integers
    assert(computeFloor(-3.0) == -3.0);
    assert(computeFloor(-3.7) == -4.0);
    assert(computeFloor(-0.5) == -1.0);

    // Zero
    assert(computeFloor(0.0) == 0.0);

    // Large magnitudes (still integers due to precision)
    double big = 1.0e300;
    assert(computeFloor(big) == big);
    double small = -1.0e300;
    assert(computeFloor(small) == small);

    // Confirm with std::floor for random double values within normal range
    double testValues[] = {123.456, -987.654, 100.0, -100.0, 1e-9};
    for (double v : testValues) {
        assert(computeFloor(v) == std::floor(v));
    }

    return 0;
}

// The floor of a number `x` is the greatest integer less than or equal to `x`. For non‑negative `x`, we can simply cast `x` to `long long`, which truncates toward zero, and then convert back to `double` — this yields the correct floor for all non‑negative values, including integers (truncation leaves them unchanged) and fractions (truncation gives the integer part). For negative `x`, truncation toward zero gives a number that is greater than or equal to the mathematical floor when the value is fractional. For example, `-3.7` truncated becomes `-3`, but the true floor is `-4`. Therefore, for negative values, we must detect whether the truncated value equals the original (i.e., the input is already an integer) or not. If the input is negative and fractional, we subtract `1.0` from the truncated result. If the input is negative and already an integer, we return it unchanged. Edge cases: zero is non‑negative, so it is handled by the first branch and returns `0.0`. Very large values beyond `long long` range: to keep the solution self‑contained and robust, we can handle extremes by checking if the value's magnitude exceeds `LLONG_MAX`. In practice, for a double, if the magnitude is that large, the value is already an integer (since precision is lost), so we can return it unchanged. The algorithm runs in O(1) time and O(1) auxiliary space, using only a few arithmetic operations and a cast.
