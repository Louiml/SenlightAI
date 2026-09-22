/*
Create a standalone C++ function that performs Lagrange interpolation given a set of n data points (x_i, y_i) and a target x value xp. The function must accept the data as two vectors (or arrays) of equal length representing the x and y coordinates, plus a double target value, and return the interpolated y value as a double. The function must validate that n ≥ 1, that all x_i are distinct (throwing a runtime error or returning NaN on duplicate x values to avoid division by zero), and must not rely on any global state. The input size is not fixed, so the implementation must handle variable-length data efficiently. The function should be const-correct and use meaningful parameter names. The main challenge is computing the Lagrange basis polynomial for each i efficiently, avoiding redundant recomputation of the denominator products where possible, but for clarity a straightforward nested loop is acceptable. Edge cases include n=1 (return y_0 regardless of xp), duplicate x values (must be detected and handled), and very large n where rounding errors may occur (but no special precision handling is required). The final output must be the interpolated value at xp.
*/
#include <vector>
#include <stdexcept>
#include <cmath>

// Perform Lagrange interpolation for given data points (x, y) at target xp.
// Requires x and y to have the same non-zero size and all x values distinct.
// Returns the interpolated y value. Throws std::invalid_argument on invalid input.
double lagrangeInterpolation(const std::vector<double>& x, const std::vector<double>& y, double xp) {
    const std::size_t n = x.size();
    if (n == 0 || y.size() != n) {
        throw std::invalid_argument("Data vectors must be non-empty and same size.");
    }

    double sum = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        double term = y[i];
        for (std::size_t j = 0; j < n; ++j) {
            if (i == j) continue;
            if (x[i] == x[j]) {
                throw std::invalid_argument("Duplicate x values cause division by zero.");
            }
            term *= (xp - x[j]) / (x[i] - x[j]);
        }
        sum += term;
    }
    return sum;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <stdexcept>

// Declaration of the solution function (assumed to be included from above).
double lagrangeInterpolation(const std::vector<double>&, const std::vector<double>&, double);

int main() {
    // Linear function: y = 2x + 1, test at x=2.5
    std::vector<double> x1 = {0.0, 1.0, 2.0};
    std::vector<double> y1 = {1.0, 3.0, 5.0};
    double result = lagrangeInterpolation(x1, y1, 2.5);
    assert(std::fabs(result - 6.0) < 1e-9);

    // Single point: always returns that y
    std::vector<double> x2 = {3.0};
    std::vector<double> y2 = {7.0};
    assert(lagrangeInterpolation(x2, y2, 100.0) == 7.0);

    // Quadratic: y = x^2 - 1, test at x=1.5
    std::vector<double> x3 = {-1.0, 0.0, 1.0};
    std::vector<double> y3 = {0.0, -1.0, 0.0};
    result = lagrangeInterpolation(x3, y3, 1.5);
    assert(std::fabs(result - (1.5*1.5 - 1.0)) < 1e-9);

    // Duplicate x values should throw
    std::vector<double> x4 = {1.0, 1.0, 2.0};
    std::vector<double> y4 = {5.0, 6.0, 7.0};
    bool threw = false;
    try {
        lagrangeInterpolation(x4, y4, 0.0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Empty input should throw
    std::vector<double> x5;
    std::vector<double> y5;
    threw = false;
    try {
        lagrangeInterpolation(x5, y5, 0.0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Mismatched sizes should throw
    std::vector<double> x6 = {0.0, 1.0};
    std::vector<double> y6 = {2.0};
    threw = false;
    try {
        lagrangeInterpolation(x6, y6, 0.5);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
// The solution uses Lagrange interpolation formula:  
// s = Σ_{i=0}^{n-1} y_i * Π_{j≠i} (xp - x_j) / (x_i - x_j)  
// The algorithm iterates over each i, computes the product of ratios for all j ≠ i, multiplies by y_i, and accumulates. To avoid division by zero, before computing each ratio we check if x_i equals x_j; if so, throw a runtime error (or return NaN). For n=1, the inner loop is skipped, and the result is simply y[0], since the product over an empty set is 1. Time complexity is O(n^2) due to the nested loops over i and j, and space complexity is O(1) extra space (excluding input vectors). The function should copy inputs into std::vector for range-based access, but can also accept raw pointers and size. The main edge cases: distinct x check is done inside the inner loop for each pair, which is O(n^2) but acceptable; alternatively a pre-check using a set reduces it to O(n log n) on average, but the inner loop already O(n^2). For simplicity, keep the check inside the loop to also catch division by zero as soon as possible.
