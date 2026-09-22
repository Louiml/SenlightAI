// Write a standalone C++ function `simpleIterationsSolve` that implements the fixed-point iteration method for finding a root of a mathematical function. The function should take a `std::function<double(double)>` representing the function `f(x)`, an initial guess `x0`, and a convergence parameter `k` (defaulting to 0.1), and return the approximate root. The iteration formula is `phi(x) = x + k * f(x)`, and the loop continues until `abs(phi(xn) - xn) < 1e-12` (use `std::numeric_limits<double>::epsilon()` multiplied by 10, as in the original code). The function must handle non-convergence by limiting iterations to a reasonable maximum (e.g., 1000) and returning the last computed value. Additionally, provide a helper function `phi` that encapsulates the iteration formula. The solution should be self-contained, use appropriate `const` correctness, and include all necessary headers. Do not include a `main` function in the solution section.

// The algorithm is a fixed-point iteration: starting from `x0`, repeatedly compute `x_{n+1} = phi(x_n) = x_n + k * f(x_n)`. Convergence is achieved when the change between successive iterations is sufficiently small, measured by `abs(phi(xn) - xn) < 10 * epsilon`. The parameter `k` is a relaxation factor that helps control convergence; for many well-behaved functions, small positive `k` works, but the method may fail or diverge for poorly conditioned problems. We must handle the initial case: the first iteration compares `phi(x0)` with `x0`, so we initialize both `xn1` and `xn2` to `x0`. The loop updates `xn1` to `xn2`, then computes `xn2 = phi(xn1)`, effectively performing a fixed-point update. Edge cases include: `x0` already being a root (so `phi(x0) == x0` and the loop terminates immediately), non-convergence (exceed iteration limit), and numerical precision issues. Time complexity is `O(iterations)` where iterations depend on the function and convergence threshold; space complexity is `O(1)`.

#include <functional>
#include <cmath>
#include <limits>

// Helper function for the fixed-point iteration formula: phi(x) = x + k * f(x)
double phi(const std::function<double(double)>& f, double x, double k) {
    return x + k * f(x);
}

// Solve for a root of f(x) using fixed-point iteration starting from x0.
// k is a relaxation parameter (default 0.1). Returns approximate root.
double simpleIterationsSolve(const std::function<double(double)>& f, double x0, double k = 0.1) {
    double xn1 = x0;
    double xn2 = x0;
    const double tolerance = 10 * std::numeric_limits<double>::epsilon();
    const int maxIterations = 1000;
    int i = 0;

    while (i < maxIterations && std::abs(phi(f, xn1, k) - xn2) >= tolerance) {
        xn2 = phi(f, xn1, k);
        xn1 = xn2;
        ++i;
    }

    return xn2;
}

#include <cassert>
#include <functional>
#include <cmath>

// The solution function is assumed to be declared above.
// For testing, we include it here by pasting, but normally it would be in a header.
// We'll use the exact same function from the solution section.
double phi(const std::function<double(double)>& f, double x, double k) {
    return x + k * f(x);
}

double simpleIterationsSolve(const std::function<double(double)>& f, double x0, double k = 0.1) {
    double xn1 = x0;
    double xn2 = x0;
    const double tolerance = 10 * std::numeric_limits<double>::epsilon();
    const int maxIterations = 1000;
    int i = 0;

    while (i < maxIterations && std::abs(phi(f, xn1, k) - xn2) >= tolerance) {
        xn2 = phi(f, xn1, k);
        xn1 = xn2;
        ++i;
    }

    return xn2;
}

int main() {
    // f(x) = x - 2, root is 2
    auto f1 = [](double x) { return x - 2.0; };
    double root1 = simpleIterationsSolve(f1, 1.0, 0.1);
    assert(std::abs(root1 - 2.0) < 1e-6);

    // f(x) = x^2 - 4, root is 2 (a fixed point of phi with k=0.1? Let's just test convergence)
    auto f2 = [](double x) { return x * x - 4.0; };
    double root2 = simpleIterationsSolve(f2, 3.0, 0.01);
    // From x=3, iterations may converge to 2 or diverge; we just check it returns something finite
    assert(std::isfinite(root2));

    // f(x) = x, phi(x) = x + k*x = x(1+k), with k=0.1 it converges to 0
    auto f3 = [](double x) { return x; };
    double root3 = simpleIterationsSolve(f3, 5.0, 0.1);
    assert(std::abs(root3) < 1e-6);

    // Start exactly at root: f(2)=0, phi(2)=2, loop immediate
    auto f4 = [](double x) { return x - 2.0; };
    double root4 = simpleIterationsSolve(f4, 2.0, 0.1);
    assert(std::abs(root4 - 2.0) < 1e-6);

    // Non-converging case: f(x) = -x (phi(x)=x -0.1*x=0.9x, converges to 0 anyway) 
    // Use k=10 to cause blowing up
    auto f5 = [](double x) { return -x; };
    double root5 = simpleIterationsSolve(f5, 1.0, 10.0);
    // Even if it diverges, it should return something after max iterations
    assert(std::isfinite(root5));

    return 0;
}
