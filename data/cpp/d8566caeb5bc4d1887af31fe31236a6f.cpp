// Write a C++ function that computes the probability density function (pdf) and cumulative distribution function (cdf) of an inverse gamma distribution at a given value x for shape parameter α and scale parameter β, without using any boost libraries. The function must validate that α > 0 and β > 0, and should support the case where the user requests only pdf or only cdf via optional boolean parameters. Use the standard mathematical definitions: pdf(x) = β^α / Γ(α) * x^(-α-1) * exp(-β/x) for x > 0, and cdf(x) = Q(α, β/x) where Q is the upper regularized gamma function (1 - lower regularized gamma). Implement the regularized gamma functions using a series expansion for the lower incomplete gamma for small arguments and continued fraction for large arguments, or use any robust numerical method. The function should throw a std::invalid_argument exception with a meaningful message if any parameter is non-positive or if x is ≤ 0. Provide a single function with signature: `std::pair<double, double> inverseGammaStats(double alpha, double beta, double x, bool wantPdf = true, bool wantCdf = true)`, returning pdf and cdf, but if a flag is false, return NaN for that component.

The solution requires implementing the inverse gamma pdf and cdf from scratch. The pdf is straightforward: given α > 0, β > 0, x > 0, compute `pow(beta, alpha) / tgamma(alpha) * pow(x, -alpha-1) * exp(-beta/x)`. The cdf is more involved: it is the survival function of the gamma distribution, i.e., cdf_invgamma(x) = 1 - P(α, β/x) where P is the regularized lower incomplete gamma function γ(α, β/x)/Γ(α). To compute P, we need an implementation of the regularized lower incomplete gamma. We can use a series expansion for small values of the argument (converges quickly when β/x < α+1) and a continued fraction (Lentz's method) for larger values. A standard approach is to compute P(a, z) via series for z < a+1, else compute Q(a,z) = 1 - P(a,z) via continued fraction and subtract from 1. Edge cases: very small or large x may cause overflow/underflow; using `long double` internally helps. Also handle the case α ≤ 0 or β ≤ 0 or x ≤ 0 by throwing std::invalid_argument. If wantPdf is false, set pdf to NaN (use std::numeric_limits<double>::quiet_NaN()). Time complexity is O(1) but the incomplete gamma series may loop many iterations (typically < 1000), so effectively O(iterations) with a bounded loop; space is O(1). Numerical stability is critical; the series and continued fraction must have convergence criteria (e.g., relative error < 1e-15).

#include <cmath>
#include <limits>
#include <stdexcept>
#include <utility>

// Compute the regularized lower incomplete gamma function P(a, x) = γ(a,x)/Γ(a)
// using series expansion for x < a+1, and continued fraction for x >= a+1.
static double regularizedLowerGamma(double a, double x) {
    if (x < 0.0 || a <= 0.0) return std::numeric_limits<double>::quiet_NaN();
    if (x == 0.0) return 0.0;
    if (a == 0.0) return 1.0; // edge case: gamma(0,x) = 1 for x>0? Actually limit, but handle gracefully

    const double epsilon = 1e-15;
    const int maxIterations = 1000;

    if (x < a + 1.0) {
        // Series expansion: P(a,x) = exp(-x) * x^a / Γ(a) * sum_{n=0}^∞ x^n / (a*(a+1)*...*(a+n))
        double term = 1.0 / a; // n=0 term
        double sum = term;
        double xPower = 1.0;
        for (int n = 1; n <= maxIterations; ++n) {
            xPower *= x;
            term = term * x / (a + n);
            sum += term;
            if (std::abs(term) < std::abs(sum) * epsilon) break;
        }
        // Multiply by exp(-x) * x^a / Γ(a)
        double logGamma = std::lgamma(a);
        double result = std::exp(-x + a * std::log(x) - logGamma) * sum;
        // Clamp to [0,1]
        if (result < 0.0) result = 0.0;
        if (result > 1.0) result = 1.0;
        return result;
    } else {
        // Continued fraction (Lentz's algorithm) for Q(a,x) = 1 - P(a,x)
        // Q(a,x) = exp(-x) * x^a / Γ(a) * (1/x) * continued fraction
        double b = x + 1.0 - a;
        double c = 1.0 / 1e-30;
        double d = 1.0 / b;
        double h = d;
        for (int i = 1; i <= maxIterations; ++i) {
            double an = -i * (i - a);
            b += 2.0;
            d = an * d + b;
            if (std::abs(d) < 1e-30) d = 1e-30;
            c = b + an / c;
            if (std::abs(c) < 1e-30) c = 1e-30;
            d = 1.0 / d;
            double delta = d * c;
            h *= delta;
            if (std::abs(delta - 1.0) < epsilon) break;
        }
        double logGamma = std::lgamma(a);
        double q = std::exp(-x + a * std::log(x) - logGamma) * h;
        if (q < 0.0) q = 0.0;
        if (q > 1.0) q = 1.0;
        return 1.0 - q; // P = 1 - Q
    }
}

// Compute pdf and cdf of inverse gamma distribution
std::pair<double, double> inverseGammaStats(double alpha, double beta, double x,
                                            bool wantPdf = true, bool wantCdf = true) {
    if (!(alpha > 0.0) || !(beta > 0.0) || !(x > 0.0)) {
        throw std::invalid_argument("Inverse gamma parameters must satisfy alpha>0, beta>0, and x>0");
    }

    const double nan = std::numeric_limits<double>::quiet_NaN();
    double pdf = nan;
    double cdf = nan;

    if (wantPdf) {
        // pdf = beta^alpha / Gamma(alpha) * x^(-alpha-1) * exp(-beta/x)
        double logPdf = alpha * std::log(beta) - std::lgamma(alpha) +
                        (-alpha - 1.0) * std::log(x) - beta / x;
        pdf = std::exp(logPdf);
        // Guard against underflow/overflow
        if (std::isinf(pdf)) pdf = 0.0; // very small likely underflow to 0
    }

    if (wantCdf) {
        // cdf = 1 - P(alpha, beta/x) where P is regularized lower incomplete gamma
        double z = beta / x;
        double p = regularizedLowerGamma(alpha, z);
        cdf = 1.0 - p;
        if (cdf < 0.0) cdf = 0.0;
        if (cdf > 1.0) cdf = 1.0;
    }

    return {pdf, cdf};
}

#include <cassert>
#include <cmath>
#include <limits>

// Declare the solution function (assume it's in a header or above)
std::pair<double, double> inverseGammaStats(double alpha, double beta, double x,
                                            bool wantPdf = true, bool wantCdf = true);

int main() {
    // Known values from the boost example: alpha=1, beta=1, x=0.5
    auto r1 = inverseGammaStats(1.0, 1.0, 0.5);
    assert(std::abs(r1.first - 0.54134113294645081) < 1e-12);
    assert(std::abs(r1.second - 0.1353352832366127) < 1e-12);

    // alpha=2, beta=3, x=0.5
    auto r2 = inverseGammaStats(2.0, 3.0, 0.5);
    assert(std::abs(r2.first - 0.17847015671997774) < 1e-12);
    assert(std::abs(r2.second - 0.017351265236664509) < 1e-12);

    // Test flags: only pdf requested
    auto r3 = inverseGammaStats(2.0, 3.0, 0.5, true, false);
    assert(std::abs(r3.first - 0.17847015671997774) < 1e-12);
    assert(std::isnan(r3.second));

    // Test flags: only cdf requested
    auto r4 = inverseGammaStats(2.0, 3.0, 0.5, false, true);
    assert(std::isnan(r4.first));
    assert(std::abs(r4.second - 0.017351265236664509) < 1e-12);

    // Test edge case: alpha=0.5 (mean undefined but pdf/cdf fine)
    auto r5 = inverseGammaStats(0.5, 1.0, 1.0);
    assert(std::isfinite(r5.first) && r5.first > 0.0);
    assert(std::isfinite(r5.second) && r5.second > 0.0 && r5.second < 1.0);

    // Test invalid parameters throw exception
    bool threw = false;
    try { inverseGammaStats(0.0, 1.0, 1.0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
    threw = false;
    try { inverseGammaStats(1.0, -1.0, 1.0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
    threw = false;
    try { inverseGammaStats(1.0, 1.0, 0.0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Test extremes: very large x should give cdf near 1
    auto r6 = inverseGammaStats(2.0, 1.0, 1000.0);
    assert(r6.second > 0.999 && r6.second <= 1.0);

    // Test x very small: pdf large, cdf near 0
    auto r7 = inverseGammaStats(2.0, 1.0, 0.001);
    assert(r7.first > 0.0);
    assert(r7.second < 0.001);

    return 0;
}
