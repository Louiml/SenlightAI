// Write a C++ function that, given a positive integer `base` and a non-negative integer `exponent`, returns the value of `base` raised to the power `exponent` using iterative multiplication. The function must handle `exponent == 0` by returning `1.0`, and it must work for both integer and floating-point `base` values (e.g., `2.5^3`). The result should be a `double`. Also, ensure the function is efficient: it must use exactly one loop and no built-in power functions like `pow()`. Provide a standalone function `powerByMult` with proper `const` qualifiers.
// The solution uses a straightforward iterative approach: start with an accumulator `result = 1.0`, then multiply it by `base` exactly `exponent` times. Because `exponent` is non-negative, no special handling for negative powers is needed. Edge cases include `exponent == 0` (loop runs zero times, returning `1.0`), `base == 0` with positive exponent (returns `0.0`), and `base` being fractional (multiplication preserves precision as a `double`). Time complexity is `O(exponent)` because the loop runs exactly `exponent` iterations. Space complexity is `O(1)` since we only use a few scalar variables. The function is safe for any `double` base, but note that extreme exponents may overflow to `inf` or underflow to `0.0` which is acceptable behavior. For very large exponents, an optimized divide-and-conquer approach could be faster, but the task explicitly requires iterative multiplication.
#include <cstddef> // for size_t

// Compute base^exponent by repeated multiplication.
// exponent must be non-negative.
double powerByMult(const double base, const int exponent) {
    double result = 1.0;
    for (int i = 0; i < exponent; ++i) {
        result *= base;
    }
    return result;
}
int main() {
    // Basic integer base
    assert(powerByMult(2.0, 3) == 8.0);
    // Exponent zero
    assert(powerByMult(5.0, 0) == 1.0);
    assert(powerByMult(0.0, 0) == 1.0);
    // Fractional base
    assert(powerByMult(0.5, 3) == 0.125);
    // Negative base with even exponent
    assert(powerByMult(-2.0, 4) == 16.0);
    // Negative base with odd exponent
    assert(powerByMult(-2.0, 3) == -8.0);
    // Base zero with positive exponent
    assert(powerByMult(0.0, 5) == 0.0);
    // Larger exponent
    assert(powerByMult(1.1, 10) > 2.5 && powerByMult(1.1, 10) < 2.6);
    return 0;
}
