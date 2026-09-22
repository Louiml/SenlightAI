Write a standalone C++ function that, given a target percentile value \(p\) (a probability between 0 and 1), a critical z-value \(z\), and a fixed location (mean) \( \mu \), uses the normal (Gaussian) distribution to compute the required standard deviation (scale) \(\sigma\) such that the probability of observing a value less than or equal to \(z\) is exactly \(p\). The function must also support a version that computes \(\sigma\) from the complement probability \(q = 1 - p\) (i.e., the probability of exceeding \(z\)). The task is to implement the mathematical inverse of the normal CDF by solving for the scale parameter analytically, without relying on external libraries beyond standard C++ headers (e.g., `<cmath>` for `std::erf` and `std::sqrt`). The function should validate inputs (e.g., \(p \in (0,1)\), \(z\) finite) and handle edge cases such as \(p = 0.5\) (where \(\sigma = 0\) leads to a degenerate distribution, so treat as a special case returning a small positive epsilon or throwing an exception as appropriate). Provide a reference implementation that is self-contained and does not use Boost or any third-party libraries.

The normal distribution with mean \(\mu\) and standard deviation \(\sigma\) has cumulative distribution function (CDF) given by:
\[
F(z) = \frac{1}{2} \left[ 1 + \operatorname{erf}\left( \frac{z - \mu}{\sigma \sqrt{2}} \right) \right].
\]
We need to find \(\sigma\) such that \(F(z) = p\). Setting \(p = \frac{1}{2} \left[ 1 + \operatorname{erf}\left( \frac{z - \mu}{\sigma \sqrt{2}} \right) \right]\), we solve for \(\sigma\):
\[
\operatorname{erf}\left( \frac{z - \mu}{\sigma \sqrt{2}} \right) = 2p - 1.
\]
Let \(x = \frac{z - \mu}{\sigma \sqrt{2}}\). Then \(x = \operatorname{erf}^{-1}(2p - 1)\). Since \(\operatorname{erf}^{-1}\) is odd and monotonic, and we can compute it using the inverse of the error function. However, standard C++ only provides `std::erf` and not its inverse, so we need to either approximate it numerically (e.g., Newton–Raphson) or, since the relationship is direct, we can use the inverse of the normal CDF: let \(\Phi^{-1}(p)\) be the standard normal quantile function (z-score). Then:
\[
\Phi^{-1}(p) = \frac{z - \mu}{\sigma} \quad \Rightarrow \quad \sigma = \frac{z - \mu}{\Phi^{-1}(p)}.
\]
But this is problematic when \(z = \mu\) and \(p = 0.5\), leading to 0/0. Instead, we directly solve:
\[
\sigma = \frac{z - \mu}{\sqrt{2} \operatorname{erf}^{-1}(2p - 1)}.
\]
To compute \(\operatorname{erf}^{-1}\), we can use a rational approximation or Newton–Raphson iteration on the equation \(g(x) = \operatorname{erf}(x) - y = 0\), with derivative \(\operatorname{erf}'(x) = \frac{2}{\sqrt{\pi}} e^{-x^2}\). The algorithm: if \(p = 0.5\), then \(\operatorname{erf}^{-1}(0) = 0\), yielding division by zero; in that case, as \(\sigma\) approaches 0, the distribution becomes degenerate at \(\mu\), so if \(z = \mu\) any \(\sigma\) works (but the probability of ≤ z is exactly 0.5 for any positive \(\sigma\)? Actually, for any \(\sigma > 0\), \(F(\mu) = 0.5\), so any positive \(\sigma\) works; but we can return a very small positive value like `std::numeric_limits<double>::epsilon()`). If \(p < 0.5\), then \(z - \mu\) must be negative for a valid positive \(\sigma\) (since the quantile is negative). Similarly, if \(p > 0.5\), \(z - \mu\) must be positive. If the sign is wrong, throw `std::invalid_argument`. For the complement case, we replace \(p\) with \(q = 1 - p\) and use the same formula. Time complexity is \(O(1)\) with a few Newton iterations (each iteration is constant time), and space complexity \(O(1)\). Edge cases include \(p\) outside (0,1), \(z\) or \(\mu\) non-finite, and sign mismatches, which should be validated.

#include <cmath>
#include <stdexcept>
#include <limits>

// Compute the inverse error function using Newton–Raphson.
// Assumes y in (-1,1). Returns x such that erf(x) = y.
double inverseErf(double y, int maxIterations = 100) {
    if (y <= -1.0 || y >= 1.0) {
        throw std::domain_error("inverseErf: y must be strictly between -1 and 1");
    }
    if (y == 0.0) return 0.0;
    // Initial guess from a simple approximation.
    double x = y * std::sqrt(3.14159265358979323846) / 2.0;
    const double sqrtPi = std::sqrt(3.14159265358979323846);
    for (int i = 0; i < maxIterations; ++i) {
        double erf_x = std::erf(x);
        double diff = erf_x - y;
        if (std::abs(diff) < 1e-15) break;
        double derivative = 2.0 / sqrtPi * std::exp(-x * x);
        x -= diff / derivative;
    }
    return x;
}

// Given a target probability p (0 < p < 1) that a normal random variable
// with known mean mu is <= z, return the required standard deviation sigma.
// If useComplement is true, p is interpreted as the probability of > z (i.e., q = 1-p).
double findNormalScale(double z, double p, double mu, bool useComplement = false) {
    if (!std::isfinite(z) || !std::isfinite(mu) || !std::isfinite(p)) {
        throw std::domain_error("findNormalScale: all arguments must be finite");
    }
    if (p <= 0.0 || p >= 1.0) {
        throw std::domain_error("findNormalScale: probability must be in (0,1)");
    }
    double effectiveP = useComplement ? (1.0 - p) : p;
    if (effectiveP <= 0.0 || effectiveP >= 1.0) {
        throw std::domain_error("findNormalScale: complement probability must be in (0,1)");
    }

    double delta = z - mu;
    if (std::abs(effectiveP - 0.5) < 1e-14) {
        // If p is exactly 0.5, the CDF at z equals 0.5 when z == mu for any sigma > 0.
        // Return a tiny positive sigma as a degenerate limit.
        if (std::abs(delta) < 1e-14) {
            return std::numeric_limits<double>::epsilon();
        } else {
            // For p=0.5, the equation erf((z-mu)/(sigma*sqrt2)) = 0 requires delta = 0.
            // If delta != 0, no finite sigma exists.
            throw std::domain_error("findNormalScale: for p=0.5, z must equal mu");
        }
    }

    double arg = 2.0 * effectiveP - 1.0; // in (-1,1)
    double invErf = inverseErf(arg);
    if (std::abs(invErf) < 1e-15) {
        // Should not happen due to above check, but guard.
        throw std::domain_error("findNormalScale: internal error, inverse erf near zero");
    }
    double sigma = delta / (std::sqrt(2.0) * invErf);
    // Ensure sigma is positive and finite.
    if (!std::isfinite(sigma) || sigma <= 0.0) {
        throw std::domain_error("findNormalScale: computed sigma is non-positive or non-finite; check sign of z-mu relative to p");
    }
    return sigma;
}

#include <cassert>
#include <cmath>

int main() {
    // Example from the prompt: mu=0, z=-2, p=0.001 -> sigma ~= 0.647201
    double s1 = findNormalScale(-2.0, 0.001, 0.0);
    // Verify that the computed sigma gives cdf(z, mu=0, sigma) = p approximately.
    double check1 = 0.5 * (1.0 + std::erf((-2.0 - 0.0) / (s1 * std::sqrt(2.0))));
    assert(std::abs(check1 - 0.001) < 1e-6);

    // Symmetric case: mu=0, z=2, p=0.999 (i.e., p>0.5, positive z)
    double s2 = findNormalScale(2.0, 0.999, 0.0);
    double check2 = 0.5 * (1.0 + std::erf((2.0 - 0.0) / (s2 * std::sqrt(2.0))));
    assert(std::abs(check2 - 0.999) < 1e-6);

    // Complement version: same as above but via complement probability q=0.001
    double s3 = findNormalScale(-2.0, 0.001, 0.0, true);
    double check3 = 0.5 * (1.0 + std::erf((-2.0 - 0.0) / (s3 * std::sqrt(2.0))));
    assert(std::abs(check3 - 0.999) < 1e-6); // because p was complement, effectiveP = 0.999

    // Edge case: p=0.5 and z=mu -> tiny positive sigma
    double s4 = findNormalScale(3.0, 0.5, 3.0);
    assert(s4 > 0.0 && s4 < 1e-12);

    // Edge case: invalid p
    bool threw = false;
    try {
        findNormalScale(-2.0, 0.0, 0.0);
    } catch (const std::domain_error&) {
        threw = true;
    }
    assert(threw);

    // Edge case: sign mismatch (z-mu negative but p>0.5) should throw
    threw = false;
    try {
        findNormalScale(-2.0, 0.9, 0.0);
    } catch (const std::domain_error&) {
        threw = true;
    }
    assert(threw);

    // Non-zero mean
    double s5 = findNormalScale(1.0, 0.1, 0.5);
    double check5 = 0.5 * (1.0 + std::erf((1.0 - 0.5) / (s5 * std::sqrt(2.0))));
    assert(std::abs(check5 - 0.1) < 1e-6);
}
