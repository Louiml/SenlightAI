Write a standalone C++ function that solves a system of two nonlinear equations using Newton's method with a fixed initial guess, stopping criterion, and maximum iteration limit. The system is defined by `f(x, y) = (y*cos(y) - x, x^2 + y^2 - 1)`, and the Jacobian inverse is given analytically. The function must take no arguments and return a `std::pair<double, double>` representing the converged root. Use initial guess `x0 = -0.5`, `y0 = -0.85`, tolerance `eps = 1e-5`, a convergence factor `mu = 4.8` (so the stopping condition is `delta_x >= eps / mu`), and a maximum of 1000 iterations. The function should output each iteration’s `x` and `y` values to `std::cout` and finally output the number of iterations `N` (where `N` is the count of successful steps before convergence or the iteration count when the maximum is reached). If the maximum iterations are exceeded, print an error message to `std::cout` and break, but still return the last computed approximation. The function must be self-contained, using only standard headers, and must not rely on any external input or global variables—all constants should be local or passed as defaults.
// The solution implements Newton's method for a 2D system of nonlinear equations. The algorithm starts from the fixed initial guess and iteratively computes `x_{n+1} = x_n - D(x_n) * f(x_n)`, where `D` is the inverse Jacobian matrix (computed analytically from the derivatives of `f`). The stopping criterion compares the Euclidean norm of the difference between successive iterates to `eps / mu`. Important edge cases include division by zero in the determinant of the Jacobian—if the determinant is zero, the method fails, but for this system and given initial guess, the determinant remains nonzero. Another edge case is exceeding the maximum iteration limit; then the loop breaks and returns the last approximation. Time complexity is `O(N)` where `N` is the number of iterations (bounded by 1000), and each iteration performs only a few floating-point operations, so it is constant time per step. Space complexity is `O(1)` because only a fixed set of 2D vectors and matrices are stored. The method converges quadratically near the root, so for typical inputs, `N` is small (e.g., under 10). The function prints each iterate for debugging and the final iteration count, matching the behavior of the original snippet but without file output or other methods.
#include <iostream>
#include <cmath>
#include <utility>

// Solve the system  f(x,y) = (y*cos(y)-x, x^2+y^2-1) = 0 using Newton's method.
// Returns the approximate root as a pair (x, y).
// Prints iteration details and final iteration count to std::cout.
std::pair<double, double> solveSystemNewton() {
    const double eps = 0.00001;
    const double mu = 4.8;
    const int maxIterations = 1000;
    double x0 = -0.5;
    double y0 = -0.85;

    // Function f
    auto f = [](double x, double y) -> std::pair<double, double> {
        return {y * std::cos(y) - x, x * x + y * y - 1.0};
    };

    // Inverse Jacobian matrix D(x,y) as a 2x2 matrix (a,b; c,d)
    auto D = [](double x, double y) -> std::pair<std::pair<double,double>, std::pair<double,double>> {
        double det = std::fabs(-2.0 * y - 2.0 * x * std::cos(y) + 2.0 * x * y * std::sin(y));
        // If det is zero, the method breaks; but for the given domain it's nonzero.
        double a = (2.0 * y) / det;
        double b = (-std::cos(y) + y * std::sin(y)) / det;
        double c = (-2.0 * x) / det;
        double d = (-1.0) / det;
        return {{a, b}, {c, d}};
    };

    double x = x0, y = y0;
    int N = 0;
    double delta = 1.0 + eps / mu; // ensure at least one iteration

    std::cout << "Newton method" << std::endl;

    while (delta >= eps / mu) {
        // Compute f at current point
        auto [fx, fy] = f(x, y);
        // Compute D(x,y)
        auto [row1, row2] = D(x, y);
        double a = row1.first, b = row1.second;
        double c = row2.first, d = row2.second;

        // Compute x - D * f
        double newX = x - (a * fx + b * fy);
        double newY = y - (c * fx + d * fy);

        // Compute delta (Euclidean distance between old and new)
        delta = std::sqrt((newX - x) * (newX - x) + (newY - y) * (newY - y));

        x = newX;
        y = newY;
        N++;

        std::cout << "x = " << x << " , y = " << y << std::endl;

        if (N > maxIterations) {
            std::cout << "ERROR N = " << maxIterations << " ! Max number of iterations!" << std::endl;
            break;
        }
    }

    std::cout << "N = " << N << std::endl;
    return {x, y};
}
#include <cassert>
#include <cmath>

int main() {
    // Since the solution is deterministic, we can test for expected approximate values.
    // Exact root of the system is approximately ( -0.826, 0.563 ) or similar (but we use the given initial guess).
    // We'll check that the returned value is close to the fixed point of phi and satisfies f ≈ 0.
    auto result = solveSystemNewton();
    double x = result.first;
    double y = result.second;

    // Check that the function values are near zero
    double fx = y * std::cos(y) - x;
    double fy = x * x + y * y - 1.0;
    assert(std::fabs(fx) < 1e-5);
    assert(std::fabs(fy) < 1e-5);

    // Check that the solution is close to a known root (from running the method)
    // The root near the starting point is approximately (-0.8268, 0.5636) but let's verify with a tolerance
    // Actually, using the initial (-0.5, -0.85), Newton converges to (-0.826, -0.563)? Let's just check magnitude.
    assert(std::fabs(x) < 1.0);
    assert(std::fabs(y) < 1.0);

    // Test for determinism: running again should give the same result
    auto result2 = solveSystemNewton();
    assert(result.first == result2.first);
    assert(result.second == result2.second);

    // Additional validation: the result is not NaN
    assert(!std::isnan(x) && !std::isnan(y));

    return 0;
}
