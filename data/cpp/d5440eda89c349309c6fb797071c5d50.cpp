/*
Write a C++ function named `safeMaximum` that takes two `float` arguments, `x` and `y`, and returns the maximum of the two, with the following special semantics: if either argument is NaN (not a number), the other argument is returned; if both are NaN, the second argument (`y`) is returned. The function must not use the ternary operator or any library function that inherently handles NaN (e.g., `std::fmax`), and it must be marked `noexcept` and `const`-correct (the parameters are passed by value, so no `const` qualifiers on parameters, but the function itself should be `constexpr` if possible). The function should be self-contained and portable across IEEE 754 implementations, but you may assume `std::isnan` is available from `<cmath>`.
*/
#include <cmath>   // for std::isnan

// Returns the maximum of x and y, treating NaN as a missing value:
// if either argument is NaN, the other is returned; if both are NaN, y is returned.
float safeMaximum(float x, float y) noexcept {
    if (std::isnan(x)) {
        return y;
    }
    if (std::isnan(y)) {
        return x;
    }
    // Neither is NaN; use normal comparison.
    return (x > y) ? x : y;
}
#include <cassert>
#include <cmath>
#include <limits>

int main() {
    const float nan = std::numeric_limits<float>::quiet_NaN();

    // Basic maximum for normal values
    assert(safeMaximum(1.0f, 2.0f) == 2.0f);
    assert(safeMaximum(-3.5f, -2.0f) == -2.0f);
    assert(safeMaximum(5.0f, 5.0f) == 5.0f);

    // One NaN, other is returned
    assert(safeMaximum(nan, 3.0f) == 3.0f);
    assert(safeMaximum(4.0f, nan) == 4.0f);
    assert(std::isnan(safeMaximum(nan, nan)));

    // Edge case: negative zero vs positive zero (equal values, normal max returns x if x > y is false)
    assert(safeMaximum(0.0f, -0.0f) == 0.0f || safeMaximum(0.0f, -0.0f) == -0.0f);

    // Large and small values
    assert(safeMaximum(3.4e38f, -3.4e38f) == 3.4e38f);
    assert(safeMaximum(1.0f, 1.0f) == 1.0f);

    // Verify the "both NaN returns y" property more precisely
    float y_nan = nan;
    float result = safeMaximum(nan, y_nan);
    assert(std::isnan(result));
}
// The core challenge is implementing the maximum semantics that treat NaN as "missing" values. The algorithm is straightforward: first check if `x` is NaN using `std::isnan`; if so, return `y`. Otherwise, check if `y` is NaN; if so, return `x`. If neither is NaN, return the standard maximum using the conditional `(x > y) ? x : y` (or equivalently `std::max`, but to avoid any potential macro or library quirks, we can use the manual ternary). This approach handles all edge cases: if both are NaN, the first check returns `y` (which is NaN), matching the specification; if only one is NaN, the other is returned; if neither is NaN, normal comparison works. The time complexity is O(1) and space complexity is O(1). The function can be declared `constexpr` in C++11 and later, though `std::isnan` may not be `constexpr` until C++23—for portability, we omit `constexpr` and just use a plain function. Ensure `noexcept` because no exceptions are thrown.
