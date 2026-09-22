Write a C++ function named `computeHypotenuse` that takes two `float` parameters representing the lengths of the two shorter sides of a right-angled triangle, and returns the length of the hypotenuse as a `float`. The function must compute the result using the Pythagorean theorem (i.e., square root of the sum of squares of the two inputs). Handle the general case where both inputs may be zero, positive, or negative numbers—although lengths are non-negative by definition, the function should still produce a mathematically meaningful result (absolute value effectively) for any real inputs because squaring eliminates sign. You do not need to validate input; assume the arguments are finite. The function should be declared `const`-correct (i.e., parameters passed by value, no mutation, function itself `const` not applicable since it's a free function). Additionally, write a separate test program (in the Test section) that uses `assert` to verify correctness with typical and edge-case values, comparing the function's output to `std::sqrt`-based expected values using an appropriate tolerance because floating-point equality is not exact.

// The solution is straightforward: for two input floats `a` and `b`, compute `std::sqrt(a*a + b*b)`. The main algorithm uses the standard library's `std::sqrt` function, which is efficient and accurate. Potential edge cases include zero inputs (result should be the other side length), negative inputs (squares become positive, so result is the same as using absolute values), and very large or very small values where overflow or underflow could theoretically occur. For the scope of this task, we assume the inputs are within reasonable ranges where `a*a` and `b*b` do not overflow (for typical float range, this works for values up to about 1e19). To avoid overflow, one could use the more robust formula `max(|a|,|b|) * sqrt(1 + (min/max)^2)`, but for simplicity and clarity in a teaching context, the direct formula is acceptable. Time complexity is O(1) constant time and O(1) auxiliary space. When testing, use a tolerance like `1e-5` because `float` arithmetic has limited precision; direct equality comparison would be fragile. For example, `computeHypotenuse(3.0f, 4.0f)` yields exactly `5.0f` in most implementations, but to be safe use `fabs(actual - expected) < 1e-5`.

#include <cmath>   // for std::sqrt

// Compute the hypotenuse of a right triangle given two leg lengths.
// Uses the Pythagorean theorem: sqrt(a^2 + b^2).
// The inputs may be negative; squaring makes the result non-negative.
float computeHypotenuse(const float a, const float b) {
    return std::sqrt(a * a + b * b);
}

#include <cassert>
#include <cmath>

// Ensure the solution function is declared or included here.
// For this test file, the function is defined in the Solution section above.

int main() {
    // Basic Pythagorean triple.
    assert(std::fabs(computeHypotenuse(3.0f, 4.0f) - 5.0f) < 1e-5);

    // Zero side lengths.
    assert(std::fabs(computeHypotenuse(0.0f, 0.0f) - 0.0f) < 1e-5);
    assert(std::fabs(computeHypotenuse(0.0f, 6.0f) - 6.0f) < 1e-5);
    assert(std::fabs(computeHypotenuse(2.5f, 0.0f) - 2.5f) < 1e-5);

    // Negative inputs (squares yield positive contributions).
    assert(std::fabs(computeHypotenuse(-3.0f, -4.0f) - 5.0f) < 1e-5);
    assert(std::fabs(computeHypotenuse(-5.0f, 12.0f) - 13.0f) < 1e-5);

    // Equal sides (isosceles right triangle).
    assert(std::fabs(computeHypotenuse(1.0f, 1.0f) - std::sqrt(2.0f)) < 1e-5);

    // Simple non-integer result.
    assert(std::fabs(computeHypotenuse(1.0f, 2.0f) - std::sqrt(5.0f)) < 1e-5);

    // Very small values.
    assert(std::fabs(computeHypotenuse(0.0001f, 0.0002f) - std::sqrt(0.00000005f)) < 1e-5);

    // Large values (still within float range).
    assert(std::fabs(computeHypotenuse(1000.0f, 2000.0f) - std::sqrt(5000000.0f)) < 0.01f);

    return 0;
}
