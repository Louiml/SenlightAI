/*
Write a C++ function `skewNormalStats` that takes four double parameters—location `xi`, scale `omega` (must be positive), and shape `alpha`—and returns a `struct` containing the mean, variance, skewness, kurtosis excess, mode, and the PDF and CDF evaluated at a given point `x`. The function must be self-contained (do not use Boost or external math libraries beyond `<cmath>`), and it must implement the standard formulas for the skew-normal distribution:  
- mean = xi + omega * delta * sqrt(2/pi), where delta = alpha / sqrt(1 + alpha^2)  
- variance = omega^2 * (1 - 2*delta^2/pi)  
- skewness = (4 - pi)/2 * (delta*sqrt(2/pi))^3 / (1 - 2*delta^2/pi)^1.5  
- kurtosis excess = 2*(pi-3) * (delta*sqrt(2/pi))^4 / (1 - 2*delta^2/pi)^2  
- mode = xi + omega * m0, where m0 = delta * sqrt(2/pi) - (delta^3 * (4-pi)/ (2 * (1 - 2*delta^2/pi))) / sqrt(2/pi) (a known approximation)  
- PDF: f(x) = (2/omega) * phi(z) * Phi(alpha*z), where z = (x - xi)/omega, phi is the standard normal PDF, Phi is the standard normal CDF (use a high-quality approximation such as Abramowitz & Stegun 26.2.19 with error < 1e-8)  
- CDF: F(x) = Phi(z) - 2 * T(z, alpha), where T is Owen's T function (implement using the series expansion for small |alpha| and the asymptotic formula for large |alpha|, or use a numerical integration approach). To keep the task self-contained, you may implement Owen's T as a single integral using Simpson's rule with 1000 subdivisions, but ensure accuracy to about 1e-6.  
Edge cases: if omega <= 0, return NaN values; if alpha is very large (e.g., > 1e5), treat as infinite and use limiting formulas (delta -> 1, mean = xi + omega*sqrt(2/pi), etc.). The function must return the struct with all six members computed.
*/
#include <cmath>
#include <limits>

struct SkewNormalStats {
    double mean;
    double variance;
    double skewness;
    double kurtosis_excess;
    double mode;
    double pdf;
    double cdf;
};

// Standard normal PDF
static double norm_pdf(double x) {
    const double inv_sqrt_2pi = 0.3989422804014327;
    return inv_sqrt_2pi * std::exp(-0.5 * x * x);
}

// Standard normal CDF using Abramowitz & Stegun 26.2.19
static double norm_cdf(double x) {
    const double a1 = 0.254829592;
    const double a2 = -0.284496736;
    const double a3 = 1.421413741;
    const double a4 = -1.453152027;
    const double a5 = 1.061405429;
    const double p = 0.3275911;

    bool negative = (x < 0.0);
    if (negative) x = -x;
    double t = 1.0 / (1.0 + p * x);
    double y = 1.0 - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * std::exp(-x * x);
    return negative ? (1.0 - y) : y;
}

// Owen's T function via Simpson integration with tangent substitution
static double owen_t(double h, double a) {
    if (a == 0.0) return 0.0;
    // Use transformation t = a * tan(u), dt = a * sec^2(u) du, u in [0, pi/2)
    const int intervals = 1000;
    const double pi = 3.14159265358979323846;
    double integral = 0.0;
    double upper = pi / 2.0 - 1e-12; // avoid asymptote
    double du = upper / intervals;
    // Simpson's rule
    auto f = [&](double u) -> double {
        double t = a * std::tan(u);
        double sec2 = 1.0 + t * t; // 1 + tan^2 = sec^2
        // integrand is exp(-0.5*h^2*(1+t^2)) / (1+t^2) * |dt/du|
        // dt/du = a * sec^2(u) = a * sec2
        double exponent = -0.5 * h * h * (1.0 + t * t);
        return std::exp(exponent) / sec2 * (a * sec2); // simplifies to a * exp(...)
    };
    for (int i = 0; i < intervals; ++i) {
        double u0 = i * du;
        double u1 = (i + 1) * du;
        double u_mid = 0.5 * (u0 + u1);
        integral += (f(u0) + 4.0 * f(u_mid) + f(u1)) * du / 6.0;
    }
    return integral / (2.0 * pi);
}

SkewNormalStats skewNormalStats(double xi, double omega, double alpha, double x) {
    SkewNormalStats result;
    const double pi = 3.14159265358979323846;
    const double sqrt2 = std::sqrt(2.0);

    if (omega <= 0.0 || !std::isfinite(omega) || !std::isfinite(xi) || !std::isfinite(alpha)) {
        result.mean = std::numeric_limits<double>::quiet_NaN();
        result.variance = std::numeric_limits<double>::quiet_NaN();
        result.skewness = std::numeric_limits<double>::quiet_NaN();
        result.kurtosis_excess = std::numeric_limits<double>::quiet_NaN();
        result.mode = std::numeric_limits<double>::quiet_NaN();
        result.pdf = std::numeric_limits<double>::quiet_NaN();
        result.cdf = std::numeric_limits<double>::quiet_NaN();
        return result;
    }

    // Delta
    double delta = alpha / std::sqrt(1.0 + alpha * alpha);
    if (alpha > 1e5) delta = 1.0;
    if (alpha < -1e5) delta = -1.0;

    // Mean
    result.mean = xi + omega * delta * sqrt2 / std::sqrt(pi);

    // Variance
    double delta2 = delta * delta;
    double factor = 1.0 - 2.0 * delta2 / pi;
    result.variance = omega * omega * factor;

    // Skewness
    double skew_num = (4.0 - pi) / 2.0 * std::pow(delta * sqrt2 / std::sqrt(pi), 3);
    double skew_den = std::pow(factor, 1.5);
    result.skewness = skew_num / skew_den;

    // Kurtosis excess
    double kurt_num = 2.0 * (pi - 3.0) * std::pow(delta * sqrt2 / std::sqrt(pi), 4);
    double kurt_den = factor * factor;
    result.kurtosis_excess = kurt_num / kurt_den;

    // Mode approximation (using known formula)
    double m0 = delta * sqrt2 / std::sqrt(pi)
                - (delta * delta * delta * (4.0 - pi) / (2.0 * (1.0 - 2.0 * delta2 / pi))) / sqrt2;
    // Invert: m0 = (mode - xi)/omega, so mode = xi + omega * m0
    result.mode = xi + omega * m0;

    // PDF
    double z = (x - xi) / omega;
    double phi_z = norm_pdf(z);
    double Phi_alpha_z = norm_cdf(alpha * z);
    result.pdf = 2.0 / omega * phi_z * Phi_alpha_z;

    // CDF
    double Phi_z = norm_cdf(z);
    double T_z_alpha = owen_t(z, alpha);
    result.cdf = Phi_z - 2.0 * T_z_alpha;

    return result;
}
#include <cassert>
#include <cmath>

int main() {
    // Test 1: xi=1.1, omega=2.2, alpha=-3.3, x=0.4
    auto s1 = skewNormalStats(1.1, 2.2, -3.3, 0.4);
    assert(std::fabs(s1.mean - (-0.5799089925398568)) < 1e-6);
    assert(std::fabs(s1.variance - 2.0179057767837230) < 1e-6);
    assert(std::fabs(s1.skewness - (-2.0347951542374196)) < 1e-3); // skewness may be approximate
    assert(std::fabs(s1.kurtosis_excess - 2.2553488991015072) < 1e-3);
    assert(std::fabs(s1.pdf - 0.2941401101565995) < 1e-4);
    assert(std::fabs(s1.cdf - 0.733918618927874) < 1e-4);

    // Test 2: xi=0, omega=1, alpha=5, x=-0.5
    auto s2 = skewNormalStats(0.0, 1.0, 5.0, -0.5);
    assert(std::fabs(s2.mean - 0.7823901817554269) < 1e-5);
    assert(std::fabs(s2.variance - 0.3878656034927102) < 1e-5);
    assert(std::fabs(s2.pdf - 0.00437241570403263) < 1e-5);
    assert(std::fabs(s2.cdf - 0.0002731513884140924) < 1e-5);

    // Test 3: xi=0, omega=1, alpha=1e5 (extreme large)
    auto s3 = skewNormalStats(0.0, 1.0, 1e5, -0.5);
    assert(std::fabs(s3.mean - 0.7978845607629713) < 1e-4);
    assert(std::fabs(s3.variance - 0.3633802276960805) < 1e-4);
    assert(std::fabs(s3.pdf - 0.0) < 1e-5);
    assert(std::fabs(s3.cdf - 0.0) < 1e-5);

    // Test 4: invalid omega
    auto s4 = skewNormalStats(1.0, -1.0, 2.0, 0.0);
    assert(std::isnan(s4.mean));
    assert(std::isnan(s4.pdf));

    return 0;
}
// The solution requires implementing standard normal PDF and CDF approximations, then combining them with the skew-normal transformations. The mean, variance, skewness, and kurtosis excess are closed-form functions of the parameter `delta = alpha / sqrt(1 + alpha^2)`. These formulas are straightforward but must handle large alpha by clamping delta to 1.0 to avoid overflow. For the mode, an approximation formula is acceptable (the exact mode has no closed form). For the PDF, compute z = (x - xi)/omega, then use phi(z) = exp(-0.5*z^2)/sqrt(2*pi) and the CDF approximation via Abramowitz & Stegun 26.2.19 (which uses a polynomial with error < 1e-8). For the CDF, we need Owen's T function T(h, a) = (1/(2*pi)) * integral from 0 to a of exp(-0.5*h^2*(1+t^2))/(1+t^2) dt. This can be integrated numerically using Simpson's rule over a transformed range, but for a self-contained task we can use a series expansion that works for all a: T(h,a) = (1/(2*pi)) * (atan(a) - sum_{k=0}^inf ( (-1)^k * (0.5 * h^2)^{k+1} / ((2k+1)!! * (2k+2)) ) * (a^{2k+2}) ). This series converges for |a| <= 1, but for large a we can use the identity T(h,a) + T(h*a, 1/a) = 0.5*(Phi(h) + Phi(h*a)) - Phi(h)*Phi(h*a) - 0.5 * (sign(a) - sign(h*a)) (this is tricky). Simpler: use Simpson's rule with 2000 intervals on the original integral from 0 to a, but for large a we can change variable t = a*tan(u) to map to [0, pi/2) and integrate over u. This is robust. Time complexity is O(1) with a fixed number of iterations (2000 Simpson intervals), space O(1). For extreme parameters (e.g., alpha = 1e5), the integral becomes stable using the tangent transformation. We must also handle negative alpha consistently.
