// Write a C++ function named `secantRoot` that implements the secant method to find a real root of the equation `cos(x) + 5x - 6 = 0`. The function should take two initial guesses (`xi` and `xii`) as `double` arguments, along with a tolerance value `epsilon` (defaulting to 0.005), and return the approximate root as a `double`. The method must iteratively update the guesses using the secant formula `x_new = xi - (xi - xii) / (f(xi) - f(xii)) * f(xi)`, stopping when the relative error `|(x_new - xii) / x_new|` is less than or equal to `epsilon`. The function must handle division-by-zero errors gracefully (e.g., if `f(xi) == f(xii)`), and must cap the maximum number of iterations (e.g., 100) to avoid infinite loops. The problem is standalone; do not include input/output in the function itself, only compute and return the root.

#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    // The actual root is approximately 1.065 (within tolerance 0.005)
    double root1 = secantRoot(0.0, 1.0, 0.005);
    assert(std::abs(root1 - 1.065) < 0.05); // loose check due to tolerance

    // Test with different guesses
    double root2 = secantRoot(0.5, 1.5, 0.001);
    assert(std::abs(root2 - 1.065) < 0.01);

    // Test with larger tolerance gives less precise result
    double root3 = secantRoot(0.0, 2.0, 0.1);
    assert(std::abs(root3 - 1.065) < 0.2);

    // Identical guesses should return NaN
    assert(std::isnan(secantRoot(1.0, 1.0)));

    // Very close guesses but not equal should still work
    double root4 = secantRoot(1.0, 1.0001, 0.005);
    assert(std::abs(root4 - 1.065) < 0.05);

    // Guesses far away (still converges)
    double root5 = secantRoot(-10.0, 10.0, 0.005);
    assert(std::abs(root5 - 1.065) < 0.05);

    std::cout << "All tests passed." << std::endl;
    return 0;
}

#include <cmath>
#include <limits>
#include <algorithm>

// Evaluate f(x) = cos(x) + 5x - 6
double secantFunction(double x) {
    return std::cos(x) + 5.0 * x - 6.0;
}

// Find root of secantFunction using the secant method.
// Returns the approximate root, or NaN if convergence fails.
double secantRoot(double xi, double xii, double epsilon = 0.005) {
    const int maxIterations = 100;
    const double NaN = std::numeric_limits<double>::quiet_NaN();

    if (xi == xii) {
        return NaN; // identical guesses cause division by zero
    }

    double fxi = secantFunction(xi);
    double fxii = secantFunction(xii);
    double x = xii;
    double error = std::numeric_limits<double>::infinity();

    for (int iter = 0; iter < maxIterations && error > epsilon; ++iter) {
        double denominator = fxi - fxii;
        if (std::abs(denominator) < 1e-15) {
            return NaN; // avoid division by zero
        }

        x = xi - ((xi - xii) / denominator) * fxi;

        // Compute relative error; if x is zero, use absolute error
        error = (std::abs(x) > 1e-15) ? std::abs((x - xii) / x) : std::abs(x - xii);

        // Shift variables for next iteration
        xi = xii;
        fxi = fxii;
        xii = x;
        fxii = secantFunction(xii);
    }

    // If loop ended due to max iterations without meeting tolerance,
    // return the last computed root anyway (could be improved).
    return x;
}

// The solution uses the secant method, a root-finding algorithm that approximates the derivative via a finite difference between two successive points. Starting from `xi` and `xii`, compute `f(xi)` and `f(xii)` using the given function `f(x) = cos(x) + 5x - 6`. Then compute the new guess `x` using the secant formula. The relative error is `abs((x - xii) / x)`. After each iteration, shift the previous `xii` into `xi`, and the new `x` into `xii`. The loop continues while the error is greater than the tolerance and the iteration count is below the cap. Edge cases: if `f(xi) - f(xii)` is zero, the denominator becomes zero — return `xi` or a sentinel value like `NaN`; if the initial guesses are equal, the error is undefined, so return `NaN`. The algorithm typically converges superlinearly, so for reasonable guesses it converges in a small number of iterations; time complexity is O(k) where k is the number of iterations (bounded by 100), and space complexity is O(1) as only a few local variables are used.
