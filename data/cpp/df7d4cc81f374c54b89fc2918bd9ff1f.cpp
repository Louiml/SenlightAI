// Implement a C++ function `newtonDividedDifference` that takes a vector of distinct x-coordinates, a vector of corresponding y-values, and a query x-value, and returns the interpolated y-value using Newton's divided difference method. The function must handle at least two points, assume x-coordinates are strictly increasing, and return a `double` result. The function should not print anything; it should only compute and return the interpolated value. For example, with points (5,12), (6,13), (9,14), (11,16) and query x=7, the result should be approximately 13.4667.

#include <cassert>
#include <cmath>
#include <vector>

int main() {
    // Example from the snippet
    std::vector<double> x1 = {5, 6, 9, 11};
    std::vector<double> y1 = {12, 13, 14, 16};
    double result1 = newtonDividedDifference(x1, y1, 7.0);
    assert(std::abs(result1 - 13.4667) < 1e-3);

    // Linear interpolation: two points
    std::vector<double> x2 = {0.0, 10.0};
    std::vector<double> y2 = {0.0, 100.0};
    assert(std::abs(newtonDividedDifference(x2, y2, 5.0) - 50.0) < 1e-9);

    // Constant function
    std::vector<double> x3 = {1.0, 2.0, 3.0};
    std::vector<double> y3 = {7.0, 7.0, 7.0};
    assert(std::abs(newtonDividedDifference(x3, y3, 2.5) - 7.0) < 1e-9);

    // Query at existing point
    std::vector<double> x4 = {1.0, 2.0, 4.0};
    std::vector<double> y4 = {3.0, 5.0, 9.0};
    assert(std::abs(newtonDividedDifference(x4, y4, 2.0) - 5.0) < 1e-9);

    // More points, slightly irregular
    std::vector<double> x5 = {-2.0, -1.0, 1.0, 3.0};
    std::vector<double> y5 = {4.0, 1.0, 2.0, 8.0};
    double expected = 4.0 * ( (-1.0)*(-3.0)*(-5.0) / ((-2+1)*(-2-1)*(-2-3)) ) + 
                     1.0 * ( (3.0)*(-3.0)*(-5.0) / ((-1+2)*(-1-1)*(-1-3)) ) +
                     2.0 * ( (3.0)*(1.0)*(-5.0) / ((1+2)*(1+1)*(1-3)) ) +
                     8.0 * ( (3.0)*(1.0)*(-1.0) / ((3+2)*(3+1)*(3-1)) );
    // That's brute force Lagrange, but we trust divided difference gives same
    // Instead, compute via our function and compare to a known result.
    double val = newtonDividedDifference(x5, y5, -2.0);
    assert(std::abs(val - 4.0) < 1e-9);  // at x=-2

    // Negative query
    assert(std::abs(newtonDividedDifference(x5, y5, 0.0) - 1.0) < 1e-6); // approximate

    // Large number of points (10) – just ensure no crash and reasonable value
    std::vector<double> x6(10);
    std::vector<double> y6(10);
    for (int i = 0; i < 10; ++i) {
        x6[i] = i * 0.5;
        y6[i] = i * i;  // quadratic function
    }
    double result6 = newtonDividedDifference(x6, y6, 2.5);
    // Actual f(2.5) = 25, but interpolation of quadratic with 10 points should be exact
    assert(std::abs(result6 - 25.0) < 1e-6);

    return 0;
}

#include <vector>
#include <stdexcept>

// Computes Newton's divided difference interpolation at a given x value.
// x: strictly increasing distinct x-coordinates.
// y: corresponding y-coordinates (same size as x).
// queryX: the x-value to interpolate.
// Returns the interpolated double value.
double newtonDividedDifference(const std::vector<double>& x,
                               const std::vector<double>& y,
                               double queryX) {
    if (x.size() != y.size() || x.size() < 2) {
        throw std::invalid_argument("x and y must have same size >= 2");
    }
    int n = static_cast<int>(x.size());

    // Build divided difference table (n x n)
    std::vector<std::vector<double>> table(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        table[i][0] = y[i];
    }

    // Fill the table
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < n - i; ++j) {
            table[j][i] = (table[j][i - 1] - table[j + 1][i - 1]) / (x[j] - x[i + j]);
        }
    }

    // Evaluate using the first row coefficients
    double result = table[0][0];
    double productTerm = 1.0;
    for (int i = 1; i < n; ++i) {
        productTerm *= (queryX - x[i - 1]);
        result += productTerm * table[0][i];
    }
    return result;
}

// Newton's divided difference interpolation builds a table of divided differences from the given points. The first column (index 0) is the input y-values. For each subsequent column i (1 ≤ i < n), each entry is computed as `(table[j][i-1] - table[j+1][i-1]) / (x[j] - x[i+j])`. This gives the coefficients for the Newton interpolation polynomial. The final interpolation value is evaluated by Horner-like accumulation: start with the first coefficient `table[0][0]`, then for each i from 1 to n-1, multiply a running product of `(query - x[j])` for j=0..i-1 and add `table[0][i]` times that product. Since all x-coordinates are distinct, no division by zero occurs. Edge cases include two points (linear interpolation) and duplicative queries that exactly match a given x (the function still works correctly). Time complexity is O(n²) due to building the table and O(n) for evaluating; space complexity is O(n²) if a full table is stored, or O(n²) can be optimized to O(n) if only the first row is kept, but for clarity we store the full table.
