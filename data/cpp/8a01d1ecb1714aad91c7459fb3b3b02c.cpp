// Write a C++ function named `computeSquareRoot` that takes a single `double` parameter and returns its square root using Newton's method (also known as the Babylonian method). The function must handle negative inputs by printing an error message to standard output and returning `0.0`. For zero input, it must print a specific message indicating the square root is zero and return `0.0`. For positive inputs, it should iteratively refine an initial guess (start with the input value itself) until the absolute difference between successive approximations is less than a tolerance of `1e-5`. The function should return the final approximation. The problem requires careful handling of edge cases (negative, zero, very small, and very large positive numbers) and should not use any library square root functions.
// The core algorithm is Newton's method for finding square roots: given an initial guess `x_n`, the next guess is `x_{n+1} = 0.5 * (x_n + a / x_n)`, which converges quadratically for positive `a`. Start with `x_n = a` (or `1.0` if `a` is very small to avoid division issues). The loop continues while `abs(x_{n+1} - x_n) > 1e-5`. Edge cases: 
// - **Negative input**: Print an error message and return `0.0`. 
// - **Zero input**: Print a message and return `0.0` immediately (though the loop would also work, it's cleaner to handle separately). 
// - **Very small positive numbers** (e.g., `1e-10`): The initial guess `a` might be too small, leading to division by very small numbers—still fine, but convergence is better if we start with `1.0` when `a < 1`. 
// - **Large numbers**: The method works but may need many iterations; the fixed tolerance ensures termination. 
// Time complexity is O(log(1/tolerance)) iterations in practice (quadratic convergence), but worst-case for pathological inputs it could be more; still, with double precision it converges in under ~100 iterations. Space is O(1).
#include <cmath>
#include <iostream>

// Computes the square root of a non-negative double using Newton's method.
// Prints appropriate messages for negative or zero input.
// Returns 0.0 for negative/zero input; otherwise returns the approximated root.
double computeSquareRoot(double a) {
    if (a < 0) {
        std::cout << "Error: negative input, no real square root." << std::endl;
        return 0.0;
    }
    if (a == 0) {
        std::cout << "Square root of 0 is 0." << std::endl;
        return 0.0;
    }

    // Start with a reasonable initial guess; for small numbers, use 1.0 to avoid slow convergence.
    double xn = (a < 1.0) ? 1.0 : a;
    double xn1 = 0.5 * (xn + a / xn);

    const double tolerance = 1e-5;
    while (std::abs(xn1 - xn) > tolerance) {
        xn = xn1;
        xn1 = 0.5 * (xn + a / xn);
    }
    return xn1;
}
#include <cassert>
#include <cmath>
#include <iostream>

// Declaration of the function under test (in a real setup, this would be in a header).
double computeSquareRoot(double a);

int main() {
    // Zero input case (prints message, returns 0)
    assert(computeSquareRoot(0.0) == 0.0);

    // Perfect squares (within tolerance)
    assert(std::abs(computeSquareRoot(4.0) - 2.0) < 1e-5);
    assert(std::abs(computeSquareRoot(9.0) - 3.0) < 1e-5);
    assert(std::abs(computeSquareRoot(100.0) - 10.0) < 1e-5);

    // Non-perfect squares
    assert(std::abs(computeSquareRoot(2.0) - std::sqrt(2.0)) < 1e-5);
    assert(std::abs(computeSquareRoot(0.5) - std::sqrt(0.5)) < 1e-5);
    assert(std::abs(computeSquareRoot(12345.678) - std::sqrt(12345.678)) < 1e-5);

    // Very small positive number
    assert(std::abs(computeSquareRoot(1e-8) - std::sqrt(1e-8)) < 1e-5);

    // Very large number
    assert(std::abs(computeSquareRoot(1e12) - std::sqrt(1e12)) < 1e-5);

    // Negative input returns 0 (error message printed)
    assert(computeSquareRoot(-3.0) == 0.0);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
