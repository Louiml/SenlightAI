/*
Given the Heston model parameters k (mean reversion speed), v_bar (long-term variance), sigma (volatility of variance), rho (correlation), v_0 (initial variance), a time horizon tau, and an array of characteristic-function arguments u (stored in a std::vector<double>), write a standalone C++ function `computeHestonCharacteristicValues` that, for each u value, computes and returns a `std::vector<std::complex<double>>` containing the Heston characteristic function value divided by the position-compression factor (i.e., the “compressed” characteristic function). The Heston characteristic function formula must be implemented directly (do not rely on external libraries). Use the standard Heston characteristic function: φ(u) = exp(i·u·x + A + B) where A = (k·v_bar/σ²)·[ (k - i·ρ·σ·u - d)·τ - 2·ln((1 - g·e^{-d·τ})/(1 - g)) ], B = (v_0/σ²)·(k - i·ρ·σ·u - d)·(1 - e^{-d·τ})/(1 - g·e^{-d·τ}), with d = sqrt((i·ρ·σ·u - k)² + σ²·(i·u + u²)), g = (k - i·ρ·σ·u - d)/(k - i·ρ·σ·u + d), and x = ln(S/K) + (r - q)·τ. The dividing position‑compression factor is exp(i·u·x). The function must accept all parameters as arguments (S, K, r, q, tau, and the Heston parameters as doubles) plus the vector of u values, and return the vector of complex numbers. Handle the case τ = 0 gracefully by returning 1+0i for all u. Use `std::complex<double>` and proper `const` correctness.
*/

#include <vector>
#include <complex>
#include <cmath>

// Compute the Heston characteristic function divided by the position compression factor
// for each u in the input vector. Parameters: S, K, r, q, tau, Heston parameters (k, v_bar, sigma, rho, v_0).
// Returns a vector of complex numbers of the same size as u_tests.
std::vector<std::complex<double>> computeHestonCharacteristicValues(
    const std::vector<double>& u_tests,
    double S, double K, double r, double q, double tau,
    double k, double v_bar, double sigma, double rho, double v_0) 
{
    std::vector<std::complex<double>> results;
    results.reserve(u_tests.size());

    // If tau is zero, the characteristic function is 1 for all u.
    if (tau == 0.0) {
        for (std::size_t i = 0; i < u_tests.size(); ++i) {
            results.push_back(std::complex<double>(1.0, 0.0));
        }
        return results;
    }

    // Position compression factor x = ln(S/K) + (r - q) * tau.
    double x = std::log(S / K) + (r - q) * tau;

    // Precompute common constants that do not depend on u.
    const std::complex<double> im(0.0, 1.0);
    const double sigma2 = sigma * sigma;
    const double k_vbar_over_sigma2 = k * v_bar / sigma2;
    const double v0_over_sigma2 = v_0 / sigma2;

    for (double u : u_tests) {
        // Complex u as a complex number (real part u, imaginary part 0).
        std::complex<double> uc(u, 0.0);

        // Compute d = sqrt( (i*rho*sigma*u - k)^2 + sigma^2 * (i*u + u^2) ).
        std::complex<double> temp1 = im * rho * sigma * uc - k;
        std::complex<double> temp2 = sigma2 * (im * uc + uc * uc);
        std::complex<double> d = std::sqrt(temp1 * temp1 + temp2);

        // Compute g = (k - i*rho*sigma*u - d) / (k - i*rho*sigma*u + d).
        std::complex<double> numerator_g = k - im * rho * sigma * uc - d;
        std::complex<double> denominator_g = k - im * rho * sigma * uc + d;
        std::complex<double> g = numerator_g / denominator_g;

        // Compute A = (k*v_bar/sigma^2) * [ (k - i*rho*sigma*u - d)*tau - 2*ln( (1 - g*exp(-d*tau)) / (1 - g) ) ].
        std::complex<double> exp_minus_d_tau = std::exp(-d * tau);
        std::complex<double> term1 = (k - im * rho * sigma * uc - d) * tau;
        std::complex<double> numerator_ln = 1.0 - g * exp_minus_d_tau;
        std::complex<double> denominator_ln = 1.0 - g;
        std::complex<double> log_term = std::log(numerator_ln / denominator_ln);
        std::complex<double> A = k_vbar_over_sigma2 * (term1 - 2.0 * log_term);

        // Compute B = (v_0/sigma^2) * (k - i*rho*sigma*u - d) * (1 - exp(-d*tau)) / (1 - g*exp(-d*tau)).
        std::complex<double> B = v0_over_sigma2 * (k - im * rho * sigma * uc - d) * (1.0 - exp_minus_d_tau) / numerator_ln;

        // Compute full characteristic function: exp(i*u*x + A + B).
        std::complex<double> exponent = im * uc * x + A + B;
        std::complex<double> characteristic_full = std::exp(exponent);

        // Divide by position compression factor exp(i*u*x).
        std::complex<double> position_factor = std::exp(im * uc * x);
        std::complex<double> compressed = characteristic_full / position_factor;

        results.push_back(compressed);
    }

    return results;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <complex>

// Declare the solution function (as if from the solution block).
std::vector<std::complex<double>> computeHestonCharacteristicValues(
    const std::vector<double>& u_tests,
    double S, double K, double r, double q, double tau,
    double k, double v_bar, double sigma, double rho, double v_0);

int main() {
    // Test 1: tau = 0 should return 1+0i for all u.
    std::vector<double> u1 = {0.0, 0.5, 1.0};
    auto res1 = computeHestonCharacteristicValues(u1, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.05, 0.2, -0.7, 0.04);
    for (const auto& val : res1) {
        assert(std::abs(val.real() - 1.0) < 1e-12);
        assert(std::abs(val.imag()) < 1e-12);
    }

    // Test 2: At u = 0, the compressed characteristic function should be exactly 1+0i (since φ(0)=1 and exp(i*0*x)=1).
    std::vector<double> u2 = {0.0};
    auto res2 = computeHestonCharacteristicValues(u2, 1.0, 1.1, 0.02, 0.0, 15.0, 3.0, 0.1, 0.25, -0.8, 0.08);
    assert(std::abs(res2[0].real() - 1.0) < 1e-12);
    assert(std::abs(res2[0].imag()) < 1e-12);

    // Test 3: For a given u and parameters, compare with manually computed value using the formula (spot check).
    // Use simple parameters: k=1, v_bar=0.05, sigma=0.2, rho=-0.7, v_0=0.04, tau=1, S=1, K=1, r=0, q=0.
    // Then x=0, so the division factor is 1, and φ(u) should match the characteristic function directly.
    std::vector<double> u3 = {0.3};
    auto res3 = computeHestonCharacteristicValues(u3, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.05, 0.2, -0.7, 0.04);
    // Compute manually:
    double u = 0.3;
    std::complex<double> im(0.0, 1.0);
    std::complex<double> uc(u, 0.0);
    std::complex<double> temp1 = im * (-0.7) * 0.2 * uc - 1.0;
    std::complex<double> temp2 = 0.04 * (im * uc + uc * uc);
    std::complex<double> d = std::sqrt(temp1 * temp1 + temp2);
    std::complex<double> g = (1.0 - im * (-0.7) * 0.2 * uc - d) / (1.0 - im * (-0.7) * 0.2 * uc + d);
    std::complex<double> exp_minus = std::exp(-d * 1.0);
    std::complex<double> A = (1.0 * 0.05 / 0.04) * ((1.0 - im * (-0.7) * 0.2 * uc - d) * 1.0 - 2.0 * std::log((1.0 - g * exp_minus) / (1.0 - g)));
    std::complex<double> B = (0.04 / 0.04) * (1.0 - im * (-0.7) * 0.2 * uc - d) * (1.0 - exp_minus) / (1.0 - g * exp_minus);
    std::complex<double> expected = std::exp(im * uc * 0.0 + A + B);
    assert(std::abs(res3[0].real() - expected.real()) < 1e-10);
    assert(std::abs(res3[0].imag() - expected.imag()) < 1e-10);

    // Test 4: For u=0 with x not zero, the compressed function should still be 1 (since φ(0)=1 and exp(i*0*x)=1).
    std::vector<double> u4 = {0.0};
    auto res4 = computeHestonCharacteristicValues(u4, 100.0, 50.0, 0.03, 0.01, 2.0, 2.5, 0.08, 0.3, -0.5, 0.06);
    assert(std::abs(res4[0].real() - 1.0) < 1e-12);
    assert(std::abs(res4[0].imag()) < 1e-12);

    // Test 5: The result vector has the same size as input.
    std::vector<double> u5 = {0.1, 0.2, 0.3, 0.4};
    auto res5 = computeHestonCharacteristicValues(u5, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.05, 0.2, -0.7, 0.04);
    assert(res5.size() == u5.size());

    return 0;
}

// The main algorithm computes the Heston characteristic function φ(u) for a set of u values, then divides each by exp(i·u·x) where x is the log forward price relative to the strike, x = ln(S/K) + (r - q)·τ. For each u, the computation proceeds in stages: (1) compute the complex square root d; (2) compute g; (3) compute the exponential argument exp(i·u·x + A + B) where A and B are complex expressions as given. Edge cases: when τ = 0, the characteristic function is trivially 1 for all u because the process has no time to evolve; the code must return 1+0i. For numerical stability, when the denominator (k - i·ρ·σ·u + d) is near zero, g may blow up, but the formula is continuous; in practice, using the standard formula with double precision is sufficient. Time complexity is O(N) where N is the number of u values, as each characteristic function evaluation is O(1). Space complexity is O(N) to store the output vector. Use `std::sqrt` and `std::exp` on `std::complex<double>` (which are available via `<complex>`).
