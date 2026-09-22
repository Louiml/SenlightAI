// Write a standalone C++ function named `negative_binomial_quantile_range` that, given the parameters `successes` (a non-negative integer), `p` (a double, 0 < p < 1), and a confidence level `alpha` (a double, 0 < alpha < 1), returns a `std::pair<double, double>` containing the lower quantile at probability `alpha` and the upper quantile at probability `1 - alpha` for a negative binomial distribution with a **real-valued discrete quantile policy** (i.e., the quantile function returns a continuous (real) value, not an integer). The function must use the Boost.Math library's `negative_binomial_distribution` with the policy `boost::math::policies::policy<boost::math::policies::discrete_quantile<boost::math::policies::real>>`. Ensure the function handles edge cases: if `successes` is 0 or `p` is outside (0,1) or `alpha` is outside (0,1), throw `std::invalid_argument`. Also, the lower quantile must be computed using `quantile(dist, alpha)` and the upper using `quantile(complement(dist, alpha))`. For testing, you can compare with known values from Boost's documentation (e.g., for `successes=20, p=0.3, alpha=0.05`, the results are approximately `27.3898` and `68.1584`). You may include a `const` qualification on parameters where appropriate.

The core algorithm is to construct a `negative_binomial_distribution` object with the given `successes` and `p` values, explicitly using the policy that makes the quantile function return real (continuous) values rather than rounding to the nearest integer (the default behavior). Then, the lower quantile is obtained by calling `quantile(dist, alpha)` which returns the value such that the CDF at that point is at least `alpha`. The upper quantile is obtained by calling `quantile(complement(dist, alpha))` which returns the value such that the survival function (1 - CDF) is at least `alpha`, i.e., the CDF is at most `1 - alpha`. This is the correct way to find the two-sided confidence interval bounds for a given confidence level. Edge cases: `successes` must be > 0 (since a negative binomial with zero successes is undefined), `p` must be strictly between 0 and 1, and `alpha` must be strictly between 0 and 1. These are validated at the start of the function to avoid undefined behavior from the Boost library. The time complexity is O(1) as it's a direct library call, and space complexity is O(1) aside from the returned pair. The key nuance is that using the `real` policy yields non-integer quantiles, which is important for interpolation purposes.

#include <boost/math/distributions/negative_binomial.hpp>
#include <boost/math/policies/policy.hpp>
#include <utility>
#include <stdexcept>
#include <cmath>

// Quantile range for a negative binomial distribution with continuous (real) quantiles.
// Returns {lower, upper} such that P(X <= lower) >= alpha and P(X >= upper) >= alpha.
std::pair<double, double> negative_binomial_quantile_range(
    unsigned int successes,       // number of successes required (must be > 0)
    double p,                     // probability of success (0 < p < 1)
    double alpha                  // confidence level (0 < alpha < 1)
) {
    // Validate input parameters.
    if (successes == 0) {
        throw std::invalid_argument("successes must be positive");
    }
    if (!(p > 0.0 && p < 1.0)) {
        throw std::invalid_argument("p must be in (0, 1)");
    }
    if (!(alpha > 0.0 && alpha < 1.0)) {
        throw std::invalid_argument("alpha must be in (0, 1)");
    }

    // Define a distribution with real-valued quantiles (no integer rounding).
    using namespace boost::math::policies;
    using dist_type = boost::math::negative_binomial_distribution<
        double,
        policy<discrete_quantile<real> >
    >;

    dist_type dist(static_cast<double>(successes), p);

    // Lower quantile: value where CDF is at least alpha.
    double lower = quantile(dist, alpha);
    // Upper quantile: value where survival function is at least alpha.
    double upper = quantile(complement(dist, alpha));

    return {lower, upper};
}

#include <cassert>
#include <cmath>
#include <utility>

// Declaration from solution (included for completeness).
std::pair<double, double> negative_binomial_quantile_range(unsigned int successes, double p, double alpha);

int main() {
    // Known values from Boost documentation (real-valued quantile policy).
    auto result1 = negative_binomial_quantile_range(20, 0.3, 0.05);
    assert(std::abs(result1.first - 27.3898) < 1e-3);
    assert(std::abs(result1.second - 68.1584) < 1e-3);

    // Another check: for alpha=0.5, lower and upper should be same.
    auto result2 = negative_binomial_quantile_range(10, 0.5, 0.5);
    assert(std::abs(result2.first - result2.second) < 1e-6);

    // Edge case: small successes and p.
    auto result3 = negative_binomial_quantile_range(1, 0.9, 0.1);
    assert(result3.first >= 0.0);
    assert(result3.second > result3.first);

    // Invalid inputs should throw.
    bool threw = false;
    try { negative_binomial_quantile_range(0, 0.5, 0.05); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    threw = false;
    try { negative_binomial_quantile_range(5, 1.0, 0.05); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    threw = false;
    try { negative_binomial_quantile_range(5, 0.5, 0.0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Symmetry: alpha and 1-alpha swap lower and upper approximately.
    auto r1 = negative_binomial_quantile_range(30, 0.2, 0.1);
    auto r2 = negative_binomial_quantile_range(30, 0.2, 0.9);
    assert(std::abs(r1.first - r2.second) < 1e-3);
    assert(std::abs(r1.second - r2.first) < 1e-3);
}
