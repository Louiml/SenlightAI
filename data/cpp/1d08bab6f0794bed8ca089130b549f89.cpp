/*
Write a C++ function named `computeExponentialSeries` that takes a positive integer `n` and a real number `x`, and returns the sum of the first `n+1` terms of the Taylor series approximation of `e^x`: \(1 + x + x^2/2! + x^3/3! + \dots + x^n/n!\). The function must compute each term independently (i.e., recalculate the factorial from scratch for each power of `x`) rather than using an iterative update from the previous term. The result should be a `double`. Assume `n` is at least 1, and `x` can be any finite `double` value, including negative values. The function must not rely on any external math library calls beyond standard arithmetic; specifically, you may not use `pow` or `factorial` functions—you must compute powers and factorials using loops.
*/
#include<cstddef> // for std::size_t

// Computes the sum of the first n+1 terms of the Taylor series for e^x.
// Each term is computed independently by looping to compute x^i and i!.
double computeExponentialSeries(double x, int n) {
    double sum = 1.0; // term for i=0 (x^0 / 0! = 1)

    for (int i = 1; i <= n; ++i) {
        // Compute x^i
        double power = 1.0;
        for (int k = 0; k < i; ++k) {
            power *= x;
        }

        // Compute i!
        double factorial = 1.0;
        for (int j = 2; j <= i; ++j) {
            factorial *= static_cast<double>(j);
        }

        sum += power / factorial;
    }

    return sum;
}
#include <cassert>
#include <cmath>

int main() {
    // n=1: 1 + x
    assert(std::fabs(computeExponentialSeries(0.3, 1) - (1.0 + 0.3)) < 1e-12);

    // n=2: 1 + x + x^2/2
    double x = 0.3;
    double expected2 = 1 + x + x*x/2.0;
    assert(std::fabs(computeExponentialSeries(x, 2) - expected2) < 1e-12);

    // Compare with known value for x=0: all terms vanish except the first, sum = 1
    assert(std::fabs(computeExponentialSeries(0.0, 10) - 1.0) < 1e-12);

    // Negative x: for n=3, check against direct formula
    x = -0.5;
    double expected3 = 1 + x + x*x/2.0 + x*x*x/6.0;
    assert(std::fabs(computeExponentialSeries(x, 3) - expected3) < 1e-12);

    // Large n, compare with exp(0.3) (true value = 1.3498588...)
    // Using n=20, the series should be very close to exp(0.3)
    assert(std::fabs(computeExponentialSeries(0.3, 20) - std::exp(0.3)) < 1e-8);

    // n=1 and x=1: 1 + 1 = 2
    assert(std::fabs(computeExponentialSeries(1.0, 1) - 2.0) < 1e-12);

    // n=5 and x=1: compare with manual sum
    double manual = 1 + 1 + 1.0/2.0 + 1.0/6.0 + 1.0/24.0 + 1.0/120.0; // = 2.716666...
    assert(std::fabs(computeExponentialSeries(1.0, 5) - manual) < 1e-12);

    // n=1 and x=2: 1 + 2 = 3
    assert(std::fabs(computeExponentialSeries(2.0, 1) - 3.0) < 1e-12);

    // n=0 case? Task says n>=1, but test n=1 as smallest
    // Check that for x=0.1 and n=1, sum is 1.1
    assert(std::fabs(computeExponentialSeries(0.1, 1) - 1.1) < 1e-12);
}
// The approach is to iterate `i` from 1 to `n`, and for each `i`, compute two things: (1) `x^i` by multiplying `x` by itself `i` times in an inner loop, and (2) `i!` by multiplying integers from 1 to `i` in another inner loop. Then add the quotient `x^i / i!` to an accumulator that starts at 1 (representing the 0-th term). This matches the provided code's logic, which recomputes both the power and factorial for every term. Edge cases: for large `n` (e.g., >170), `n!` overflows to infinity, but since the result is `double`, this still returns a finite approximation for reasonable `x`; for very large `x` and `n`, terms may grow large, but the loop will still complete. For negative `x`, the sign alternates correctly because the power loop multiplies `x` repeatedly. Time complexity is \(O(n^2)\) because each term requires inner loops of total length roughly \(i + i = 2i\), summing to \(O(n^2)\). Space complexity is \(O(1)\) since only a few scalar variables are used.
