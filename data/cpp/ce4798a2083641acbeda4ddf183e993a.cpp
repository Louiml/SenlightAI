/*
Write a C++ function named `computeHypotenuse` that takes two positive integer values representing the lengths of the legs (catheti) of a right triangle, and returns the length of the hypotenuse as an integer, truncated toward zero (i.e., using integer arithmetic after applying the square root). The function must handle the case where either leg is zero or negative by returning `0` as an error indicator, and it must work correctly for the largest possible integer input values without causing overflow in intermediate calculations.
*/

#include <cmath>

// Compute the integer hypotenuse given two leg lengths.
// Returns 0 if either leg is non-positive.
int computeHypotenuse(const int a, const int b) {
    if (a <= 0 || b <= 0) {
        return 0;
    }
    // Use double to avoid integer overflow for large inputs.
    const double hypotenuse = std::sqrt(static_cast<double>(a) * a +
                                        static_cast<double>(b) * b);
    return static_cast<int>(hypotenuse);
}

#include <cassert>

int computeHypotenuse(const int a, const int b); // declaration for testing

int main() {
    // Basic 3-4-5 triangle
    assert(computeHypotenuse(3, 4) == 5);
    // 5-12-13 triangle
    assert(computeHypotenuse(5, 12) == 13);
    // Symmetry: swapping legs gives same result
    assert(computeHypotenuse(4, 3) == 5);
    // Large values without overflow (hypotenuse ≈ 1.414e9)
    assert(computeHypotenuse(1000000000, 1000000000) == 1414213562);
    // Invalid: zero or negative input returns 0
    assert(computeHypotenuse(0, 5) == 0);
    assert(computeHypotenuse(3, -1) == 0);
    // Single leg non-zero, other zero → invalid, returns 0
    assert(computeHypotenuse(-7, 0) == 0);
    // Edge case: one leg is 1, other 1 → sqrt(2) ≈ 1.414 truncated to 1
    assert(computeHypotenuse(1, 1) == 1);
    // Edge case: both legs 1, but ensure symmetrical result
    assert(computeHypotenuse(1, 1) == 1);
    return 0;
}

// The problem requires computing the Euclidean norm (`sqrt(a^2 + b^2)`) of two integers, but with an integer return type. The straightforward approach using `std::pow` and `std::sqrt` from `<cmath>` works for typical values, but `std::pow(c1, 2)` with `int` arguments returns a `double`, which could lose precision for very large integers (e.g., near `INT_MAX`). To avoid overflow and precision loss, we can convert the inputs to `double` before squaring, then use `std::sqrt`, and finally cast the result back to `int`, which truncates the fractional part. However, for extremely large values (e.g., `c1 = 1000000000`), `c1*c1` as an `int` would overflow; but if we cast each input to `double` first, squaring is done in floating point, which has 53-bit precision—sufficient for integers up to about 9e15, which is far beyond `int` range. Edge cases: negative or zero inputs should return `0` (invalid). The time complexity is O(1) since we perform a fixed number of arithmetic operations. Space complexity is O(1). The solution must be `const`-correct, so the function parameters are `const int` (though `int` by value is fine, we can mark them `const` inside) and the function itself can be marked `noexcept` if desired.
