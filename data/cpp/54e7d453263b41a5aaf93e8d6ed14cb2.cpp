/*
Write a standalone C++ function that, given a vector of 2D points (each with integer `x` and `y` coordinates), computes the coefficient vector `c = (c0, c1)` for a least-squares logarithmic fit of the form `y = c0 + c1 * log2(x)`. The function must return a `std::array<double, 2>` containing `c0` and `c1`. Assume all points have strictly positive `x` values, and at least two distinct `x` values are present so the system is solvable. The implementation must solve the normal equations `A * c = b`, where `A` is the 2×2 matrix given by:
- `A(0,0) = n` (number of points)
- `A(0,1) = sum(log2(x))`
- `A(1,0) = sum(log2(x))`
- `A(1,1) = sum((log2(x))^2)`
and `b` is the vector:
- `b(0) = sum(y)`
- `b(1) = sum(y * log2(x))`
Use Cramer's rule or Gaussian elimination to solve the system. The function must be named `logarithmicFitCoefficients` and take a `const std::vector<Point>&` where `Point` is a simple struct with `double x, y`. Provide the complete implementation with necessary headers and a descriptive comment, but do not include a `main` function.
*/

#include <array>
#include <vector>
#include <cmath>
#include <stdexcept>

struct Point {
    double x;
    double y;
};

// Computes the least-squares coefficients (c0, c1) for y = c0 + c1 * log2(x)
// Returns {c0, c1} in that order. Throws std::invalid_argument if fewer than
// two distinct x values exist (singular system).
std::array<double, 2> logarithmicFitCoefficients(const std::vector<Point>& points) {
    if (points.size() < 2) {
        throw std::invalid_argument("At least two points are required.");
    }

    double n = 0.0;
    double sum_log = 0.0;
    double sum_log_sq = 0.0;
    double sum_y = 0.0;
    double sum_y_log = 0.0;

    for (const Point& p : points) {
        double lx = std::log2(p.x);
        n += 1.0;
        sum_log += lx;
        sum_log_sq += lx * lx;
        sum_y += p.y;
        sum_y_log += p.y * lx;
    }

    // Matrix A = [[n, sum_log], [sum_log, sum_log_sq]]
    // Vector b = [sum_y, sum_y_log]
    double detA = n * sum_log_sq - sum_log * sum_log;
    if (std::abs(detA) < 1e-12) {
        throw std::invalid_argument("Points must have at least two distinct x values.");
    }

    // Cramer's rule: c0 = det(A0)/det(A), c1 = det(A1)/det(A)
    double detA0 = sum_y * sum_log_sq - sum_log * sum_y_log;
    double detA1 = n * sum_y_log - sum_y * sum_log;

    return {detA0 / detA, detA1 / detA};
}

#include <cassert>
#include <cmath>
#include <vector>
#include <array>

// Reuse the Point struct and function above (assume included).
int main() {
    // Exact fit: y = 2 + 3*log2(x)
    std::vector<Point> exact = {{1, 2}, {2, 5}, {4, 8}};
    auto c = logarithmicFitCoefficients(exact);
    assert(std::abs(c[0] - 2.0) < 1e-9);
    assert(std::abs(c[1] - 3.0) < 1e-9);

    // Two points always give exact line
    std::vector<Point> two = {{1, 0}, {2, 1}};
    auto c2 = logarithmicFitCoefficients(two);
    assert(std::abs(c2[0] - 0.0) < 1e-9);
    assert(std::abs(c2[1] - 1.0) < 1e-9);

    // Constant y (horizontal line) => c1 should be 0
    std::vector<Point> const_y = {{1, 4}, {2, 4}, {3, 4}};
    auto c3 = logarithmicFitCoefficients(const_y);
    assert(std::abs(c3[0] - 4.0) < 1e-9);
    assert(std::abs(c3[1] - 0.0) < 1e-9);

    // Larger dataset with noise, verify against known solution via manual computation
    // Points: (1,0), (2,1), (4,2), (8,3) => perfect fit y = log2(x)
    std::vector<Point> perfect = {{1, 0}, {2, 1}, {4, 2}, {8, 3}};
    auto c4 = logarithmicFitCoefficients(perfect);
    assert(std::abs(c4[0] - 0.0) < 1e-9);
    assert(std::abs(c4[1] - 1.0) < 1e-9);

    // Scaled version: y = 5 - 2*log2(x)
    std::vector<Point> scaled = {{1, 5}, {2, 3}, {4, 1}};
    auto c5 = logarithmicFitCoefficients(scaled);
    assert(std::abs(c5[0] - 5.0) < 1e-9);
    assert(std::abs(c5[1] - (-2.0)) < 1e-9);

    // Throw on singular input
    bool threw = false;
    try {
        logarithmicFitCoefficients({{1, 0}, {1, 0}});
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
}

// The task is to fit a logarithmic model `y = c0 + c1 * log2(x)` to given points using the least-squares method. The normal equations arise from minimizing the sum of squared residuals `Σ(y_i - c0 - c1*log2(x_i))^2`. Taking partial derivatives with respect to `c0` and `c1` and setting them to zero yields the 2×2 linear system described. We must compute sums from all points: `n`, `sum_log = Σ log2(x)`, `sum_log2 = Σ (log2(x))^2`, `sum_y = Σ y`, `sum_y_log = Σ y*log2(x)`. Then build matrix `A` and vector `b`. Since `A` is symmetric and positive definite when there are at least two distinct `x` values (which ensures the columns of the design matrix are linearly independent), the system has a unique solution. We solve using Cramer's rule: `c0 = det(A0)/det(A)`, `c1 = det(A1)/det(A)`, where `A0` replaces the first column with `b`, and `A1` replaces the second column. Edge cases: if all `x` values are identical, the system becomes singular (det A = 0), but the problem guarantees at least two distinct values. For numerical stability, we use `std::log2` (which returns double). Time complexity is O(n) to compute sums, plus O(1) for solving. Space complexity is O(1) aside from the input vector.
