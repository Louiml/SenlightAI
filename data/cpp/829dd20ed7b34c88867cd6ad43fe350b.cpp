/*
Write a C++ function that computes the first-order and second-order Greeks for a European call option under the Black-Scholes model, given spot price \(S\), strike price \(K\), volatility \(\sigma\), time to expiration \(\tau\), and risk-free rate \(r\). The function must accept these parameters as `double` values and return a struct containing: the option price, delta, vega, theta (as the negative partial derivative with respect to time-to-expiration), rho, gamma, and vanna. Use exact Black-Scholes formulas, not numerical differentiation, and include all necessary standard mathematical functions. Assume zero dividend yield.
*/

#include <cmath>
#include <numbers>
#include <stdexcept>

// Structure holding option price and first/second-order Greeks.
struct CallGreeks {
    double price;
    double delta;
    double vega;
    double theta;
    double rho;
    double gamma;
    double vanna;
};

// Compute Black-Scholes call price and Greeks for a European option.
// Assumes zero dividend yield.
CallGreeks blackScholesCallGreeks(double S, double K, double sigma, double tau, double r) {
    if (tau <= 0.0 || sigma <= 0.0) {
        throw std::invalid_argument("tau and sigma must be positive.");
    }

    const double sqrtTau = std::sqrt(tau);
    const double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * tau) / (sigma * sqrtTau);
    const double d2 = d1 - sigma * sqrtTau;

    // Standard normal pdf and cdf.
    const double phi_d1 = std::exp(-0.5 * d1 * d1) / std::sqrt(2.0 * std::numbers::pi);
    const double Phi_d1 = 0.5 * (1.0 + std::erf(d1 / std::sqrt(2.0)));
    const double Phi_d2 = 0.5 * (1.0 + std::erf(d2 / std::sqrt(2.0)));
    const double exp_neg_r_tau = std::exp(-r * tau);

    CallGreeks g;
    g.price = S * Phi_d1 - exp_neg_r_tau * K * Phi_d2;
    g.delta = Phi_d1;
    g.vega = S * phi_d1 * sqrtTau;
    g.theta = -S * phi_d1 * sigma / (2.0 * sqrtTau) - r * K * exp_neg_r_tau * Phi_d2;
    g.rho = K * tau * exp_neg_r_tau * Phi_d2;
    g.gamma = phi_d1 / (S * sigma * sqrtTau);
    g.vanna = -phi_d1 * d2 / sigma;
    return g;
}

#include <cassert>
#include <cmath>

// Assume the solution function is declared above.

int main() {
    // Example from the snippet: S=105, K=100, sigma=5, tau=30/365, r=0.0125
    double S = 105.0;
    double K = 100.0;
    double sigma = 5.0;
    double tau = 30.0 / 365.0;
    double r = 0.0125;

    CallGreeks g = blackScholesCallGreeks(S, K, sigma, tau, r);

    // Compare with expected values from the given output (tolerance for floating point).
    assert(std::abs(g.price - 56.5136030677739) < 1e-9);
    assert(std::abs(g.delta - 0.773818444921274) < 1e-12);
    assert(std::abs(g.vega - 9.05493427705736) < 1e-12);
    assert(std::abs(g.theta - (-275.73013426444)) < 1e-9);
    assert(std::abs(g.rho - 2.03320550539396) < 1e-12);
    assert(std::abs(g.gamma - 0.00199851912993254) < 1e-15);
    assert(std::abs(g.vanna - 0.0410279463126531) < 1e-15);

    // Edge case: at-the-money call, short time, moderate vol.
    S = 100.0; K = 100.0; sigma = 0.2; tau = 1.0; r = 0.05;
    g = blackScholesCallGreeks(S, K, sigma, tau, r);
    assert(std::abs(g.price - 10.4505835721858) < 1e-10);
    assert(std::abs(g.delta - 0.636830651175619) < 1e-12);
    assert(std::abs(g.gamma - 0.0198290883673947) < 1e-12);
    assert(std::abs(g.vega - 39.8856077458832) < 1e-10);

    // Deep in-the-money call: delta should be close to 1.
    S = 200.0; K = 100.0; sigma = 0.1; tau = 0.5; r = 0.0;
    g = blackScholesCallGreeks(S, K, sigma, tau, r);
    assert(g.delta > 0.9999);
    assert(g.gamma > 0.0);

    // Deep out-of-the-money call: delta close to 0.
    S = 50.0; K = 200.0; sigma = 0.5; tau = 0.25; r = 0.03;
    g = blackScholesCallGreeks(S, K, sigma, tau, r);
    assert(g.delta < 1e-6);
    assert(g.vega > 0.0);

    return 0;
}

// The Black-Scholes call price is \(C = S \Phi(d_1) - K e^{-r\tau} \Phi(d_2)\), where \(d_1 = \frac{\ln(S/K) + (r + \sigma^2/2)\tau}{\sigma \sqrt{\tau}}\) and \(d_2 = d_1 - \sigma \sqrt{\tau}\). The Greeks are derived analytically:  
// - Delta: \(\partial C/\partial S = \Phi(d_1)\)  
// - Vega: \(\partial C/\partial \sigma = S \phi(d_1) \sqrt{\tau}\)  
// - Theta: \(-\partial C/\partial \tau = -\frac{S \phi(d_1)\sigma}{2\sqrt{\tau}} - r K e^{-r\tau} \Phi(d_2)\)  
// - Rho: \(\partial C/\partial r = K \tau e^{-r\tau} \Phi(d_2)\)  
// - Gamma: \(\partial^2 C/\partial S^2 = \phi(d_1) / (S \sigma \sqrt{\tau})\)  
// - Vanna: \(\partial^2 C/\partial S \partial \sigma = -\phi(d_1) d_2 / \sigma\)  
//
// We compute \(\phi(x) = \frac{1}{\sqrt{2\pi}} e^{-x^2/2}\) and \(\Phi(x) = \frac{1}{2}(1 + \text{erf}(x/\sqrt{2}))\) using standard library functions. Use `<cmath>` and `<numbers>` for \(\pi\). Edge cases: if \(\tau \leq 0\) or \(\sigma \leq 0\), return zeros or throw an exception; we choose to return a struct with all zeros for simplicity. The algorithm is \(O(1)\) time and \(O(1)\) space.
