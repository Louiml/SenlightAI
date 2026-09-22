// Write a C++ function `double sumOfSquares(double a, double b)` that takes two real numbers and returns the sum of their squares (i.e., \(a^2 + b^2\)). The function must be `const`-correct, meaning it should not modify its parameters, and it should be declared with `const` where appropriate. The function should handle any finite double values, including negative numbers, zero, and very large or very small magnitudes, without overflow or underflow concerns beyond what C++ double arithmetic naturally provides. The function must not rely on any external libraries beyond the standard headers, and it should be reusable in any context without a `main` function.

// The solution is straightforward: compute `a * a` and `b * b` and return their sum. To improve clarity and reuse, we can define a helper `double square(double x)` that returns `x * x`, then have `sumOfSquares` call it. Since the parameters are passed by value, they are already copies, so no modification is possible; however, we mark them as `const` in the parameter list to emphasize immutability. Edge cases include negative numbers (squaring removes the sign), zero (returns zero), and large values (potential overflow if the square exceeds `DBL_MAX`, but that's inherent to the operation). The algorithm runs in \(O(1)\) time and uses \(O(1)\) auxiliary space. No special handling is needed for NaN or infinity unless explicitly required, but the natural behavior of double arithmetic will propagate them.

// Returns the square of a real number.
double square(double x) {
    return x * x;
}

// Returns the sum of the squares of two real numbers.
double sumOfSquares(const double a, const double b) {
    return square(a) + square(b);
}

#include <cassert>
#include <cmath>

int main() {
    // Basic positive numbers
    assert(sumOfSquares(3.0, 4.0) == 25.0);
    // Negative numbers
    assert(sumOfSquares(-3.0, -4.0) == 25.0);
    // Mixed signs
    assert(sumOfSquares(-5.0, 2.0) == 29.0);
    // Zero
    assert(sumOfSquares(0.0, 0.0) == 0.0);
    // One zero, one positive
    assert(sumOfSquares(0.0, 7.0) == 49.0);
    // Small fractional values
    assert(fabs(sumOfSquares(0.5, 0.5) - 0.5) < 1e-12);
    // Large values (but not overflowing double)
    assert(sumOfSquares(1e6, 2e6) == 5e12);
    // Verify const-correctness does not alter input (implicit, but check values)
    double x = 2.0, y = 3.0;
    sumOfSquares(x, y);
    assert(x == 2.0 && y == 3.0);
    // Edge: negative zero (treats as zero)
    assert(sumOfSquares(-0.0, 0.0) == 0.0);
}
