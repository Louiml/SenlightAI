/*
Write a C++ function named `inverseChiSquaredPdfMatch` that takes three `double` parameters: `df` (degrees of freedom, must be > 0), `scale` (must be > 0), and `x` (must be ≥ 0). The function must return a `bool` indicating whether the Boost.Math `pdf` value for an inverse chi‑squared distribution with those parameters matches the value computed by the provided `naive_pdf3` formula (scaled inverse chi‑square from Wikipedia) within a relative tolerance of `1e-9`. Specifically, the function should compute both values and return `true` if the two values are both finite, and either both are exactly equal or the absolute difference divided by the larger absolute value is ≤ `1e-9`. If either value is non‑finite (e.g., due to `x = 0` or extreme parameters), the function must return `false`. Use `boost::math::inverse_chi_squared_distribution<double>` and `boost::math::pdf` for the Boost computation, and implement the naive formula exactly as given, using `std::pow`, `std::exp`, and `boost::math::tgamma`. The function must be `const`-correct and should not perform any input/output. Provide the solution function in a single code block without a `main` function.
*/

#include <cmath>
#include <boost/math/distributions/inverse_chi_squared.hpp>
#include <boost/math/special_functions/gamma.hpp>

// Compare Boost's inverse chi-squared PDF with naive scaled formula within a relative tolerance.
// Returns true if both values are finite and relative error <= 1e-9; false otherwise.
bool inverseChiSquaredPdfMatch(double df, double scale, double x) {
    using boost::math::inverse_chi_squared_distribution;
    using boost::math::pdf;
    using boost::math::tgamma;

    // Construct Boost distribution (throws if parameters invalid, but inputs are assumed valid here)
    inverse_chi_squared_distribution<double> dist(df, scale);
    double boost_val = pdf(dist, x);

    // Naive scaled inverse chi-squared PDF from Wikipedia definition 3
    double df2 = df / 2.0;
    double naive_val = std::pow(scale * df2, df2) * std::exp(-df2 * scale / x) /
                       (tgamma(df2) * std::pow(x, 1.0 + df2));

    // Check finiteness
    if (!std::isfinite(boost_val) || !std::isfinite(naive_val)) {
        return false;
    }

    // Exact equality or relative error tolerance
    if (boost_val == naive_val) {
        return true;
    }
    double max_abs = std::max(std::fabs(boost_val), std::fabs(naive_val));
    double rel_err = std::fabs(boost_val - naive_val) / max_abs;
    return rel_err <= 1e-9;
}

#include <cassert>
#include <cmath>

// Prototype of the function under test
bool inverseChiSquaredPdfMatch(double df, double scale, double x);

int main() {
    // Typical parameters from the example: df=5, scale=1/5
    assert(inverseChiSquaredPdfMatch(5.0, 0.2, 0.5) == true);
    assert(inverseChiSquaredPdfMatch(5.0, 0.2, 0.1) == true);
    assert(inverseChiSquaredPdfMatch(5.0, 0.2, 0.9) == true);

    // Different df and scale values
    assert(inverseChiSquaredPdfMatch(10.0, 1.0, 0.2) == true);
    assert(inverseChiSquaredPdfMatch(2.0, 0.5, 1.0) == true);
    assert(inverseChiSquaredPdfMatch(1.0, 1.0, 0.7) == true);

    // Edge: x = 0 should return false (division by zero produces NaN/inf)
    assert(inverseChiSquaredPdfMatch(5.0, 0.2, 0.0) == false);

    // Edge: very small x near 1e-300 should still be finite and match
    assert(inverseChiSquaredPdfMatch(5.0, 0.2, 1e-300) == true);

    // Edge: very large x should give extremely small but finite values, still match
    assert(inverseChiSquaredPdfMatch(5.0, 0.2, 1e6) == true);

    // Edge: unusual df and scale
    assert(inverseChiSquaredPdfMatch(0.5, 2.0, 0.3) == true);
    assert(inverseChiSquaredPdfMatch(100.0, 0.01, 0.5) == true);

    // Edge: extreme combination may cause overflow in naive formula but finite check fails
    // For safety we test a moderate case that should pass
    assert(inverseChiSquaredPdfMatch(20.0, 10.0, 5.0) == true);

    return 0;
}

// The task requires comparing two different implementations of the probability density function (PDF) of a scaled inverse chi‑squared distribution. The Boost library provides a robust, optimized implementation, while the naive formula from Wikipedia (`naive_pdf3`) is mathematically equivalent but may suffer from floating‑point rounding differences. The core algorithm: 1) Construct a `boost::math::inverse_chi_squared_distribution<double>` object with the given `df` and `scale`. 2) Compute `boost_val = boost::math::pdf(dist, x)`. 3) Compute `naive_val` using the formula: `pow(scale * df/2, df/2) * exp(-df/2 * scale/x) / (tgamma(df/2) * pow(x, 1 + df/2))`. 4) Check both values are finite using `std::isfinite`. 5) If not finite, return `false`. 6) Otherwise, if exactly equal, return `true`; else compute the relative error as `|boost_val - naive_val| / max(|boost_val|, |naive_val|)` and return `true` if that error ≤ `1e-9`. Edge cases: `x = 0` leads to division by zero and `NaN`/inf, returned as `false`. Very small or large `x` may cause underflow/overflow; the finite check handles that. Time complexity is O(1) as it involves a few arithmetic operations and library calls; space complexity is O(1). The `const` correctness applies to the inputs but the function itself is not a member, so no `const` qualifier on the function is needed; however, avoid modifying parameters (they are passed by value anyway). The comparison tolerance must handle cases where the naive formula might be slightly inaccurate due to intermediate overflow (e.g., `pow(scale*df/2, df/2)` can overflow for large `df`), but since both are compared with a relative tolerance, it should be acceptable for typical inputs.
