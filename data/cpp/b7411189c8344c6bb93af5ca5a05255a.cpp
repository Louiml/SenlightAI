// Write a C++ function `lagrangeInterpolation` that takes two vectors of `double` representing the known x-coordinates and y-coordinates of a data set, along with a `double` target value `k`, and returns the interpolated or extrapolated y-value at `k` using Lagrange polynomial interpolation. The function must handle the case where `k` is outside the range of the provided x-data (extrapolation), as well as inputs with duplicate x-values—though duplicates are mathematically invalid, you should detect them and throw a `std::invalid_argument` exception. Additionally, ensure the function works when the input vectors are empty (throw an exception) and when only one point is given (return that point's y-value regardless of `k`). The function must be `const`-correct, taking the vectors by `const` reference. Use this to predict sales for days beyond the given data, and verify that the output matches known reference values.

The solution uses the standard Lagrange interpolation formula:  
For each i from 0 to n-1, compute the Lagrange basis polynomial L_i(k) = product over j != i of (k - x_j)/(x_i - x_j). Then sum y_i * L_i(k). The algorithm is straightforward: initialize result = 0, for each i, compute term = y[i] multiplied by the product over j. Edge cases: if `x.size() != y.size()`, throw `std::invalid_argument`. If vectors are empty, throw `std::invalid_argument`. If there are duplicate x-values, this leads to division by zero in the formula; detect duplicates by checking each pair and throw `std::invalid_argument`. If n == 1, return y[0] because the sum has a single term and the product is empty (multiplies to 1). The time complexity is O(n^2) because for each of the n terms we loop over n other points, and space complexity is O(1) auxiliary (only a few double variables). Extrapolation works naturally because the formula is polynomial and does not assume the target lies between the points.

#include <vector>
#include <stdexcept>

// Perform Lagrange interpolation or extrapolation at point k.
// x: known x-coordinates (must be distinct)
// y: known y-coordinates (same size as x)
// k: target x-value
// Returns interpolated y-value. Throws std::invalid_argument for invalid input.
double lagrangeInterpolation(const std::vector<double>& x, const std::vector<double>& y, double k) {
    const size_t n = x.size();
    if (n == 0 || n != y.size()) {
        throw std::invalid_argument("x and y must be non-empty and have the same size");
    }
    // Check for duplicate x-values (would cause division by zero)
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            if (x[i] == x[j]) {
                throw std::invalid_argument("x values must be distinct");
            }
        }
    }
    if (n == 1) {
        return y[0];
    }

    double result = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double term = y[i];
        for (size_t j = 0; j < n; ++j) {
            if (j != i) {
                term *= (k - x[j]) / (x[i] - x[j]);
            }
        }
        result += term;
    }
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <stdexcept>

// Function declaration (include the solution above, or include header)
double lagrangeInterpolation(const std::vector<double>& x, const std::vector<double>& y, double k);

int main() {
    // Data: days 1 to 10 with given sales values
    std::vector<double> x = {1,2,3,4,5,6,7,8,9,10};
    std::vector<double> y = {150.56,152.78,155.23,158.67,162.78,167.23,172.67,179.78,188.23,198.67};

    // Extrapolation to day 11 (k=11) gives approximately 219.7
    double predicted1 = lagrangeInterpolation(x, y, 11);
    assert(std::abs(predicted1 - 219.7) < 1e-2);

    // Absolute error vs known value 210.56 should be about 9.14
    assert(std::abs(std::abs(predicted1 - 210.56) - 9.14) < 1e-2);

    // Extrapolation to day 15 gives approximately 1172.76
    double predicted15 = lagrangeInterpolation(x, y, 15);
    assert(std::abs(predicted15 - 1172.76) < 1e-2);

    // Single point case: returns that y value no matter k
    std::vector<double> x1 = {5};
    std::vector<double> y1 = {42.0};
    assert(lagrangeInterpolation(x1, y1, 100) == 42.0);

    // Two points linear interpolation: at midpoint should be average
    std::vector<double> x2 = {0, 10};
    std::vector<double> y2 = {0, 100};
    assert(std::abs(lagrangeInterpolation(x2, y2, 5) - 50.0) < 1e-12);

    // Empty input throws
    bool threw = false;
    try { std::vector<double> e; lagrangeInterpolation(e, e, 0); }
    catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Duplicate x throws
    threw = false;
    try { std::vector<double> xd = {1,1,2}; std::vector<double> yd = {3,4,5}; lagrangeInterpolation(xd, yd, 0); }
    catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Mismatched sizes throw
    threw = false;
    try { std::vector<double> xm = {1,2}; std::vector<double> ym = {1}; lagrangeInterpolation(xm, ym, 0); }
    catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
}
