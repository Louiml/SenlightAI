/*
Implement a standalone C++ function that computes the implied volatility of a European call option under the Bachelier (normal) model using Newton-Raphson iteration. The function should take as inputs: expiry time (in years), strike price, current option price, and forward price (all as `double`). It must return the implied normal volatility (as `double`). Given the Bachelier call pricing formula `call = (F - K) * N(d) + sigma * sqrt(T) * n(d)` where `d = (F - K) / (sigma * sqrt(T))`, `N` is the standard normal CDF, `n` is the standard normal PDF, and `zeta = sigma * sqrt(T)`, the function must solve for `sigma` such that the model price equals the input price. Use Newton-Raphson with a starting guess of `0.1`, at most 100 iterations, and a tolerance of `1e-10` on the price difference. Ensure the returned volatility is non-negative (clamp to zero if the price is below intrinsic value `max(0, F - K)`). For simplicity, you may not use any external libraries or classes; implement all needed math inline (e.g., normal CDF via `erfc`). The function should be named `bachelierImpliedVol` and should be `const`-correct, taking parameters by value or const reference.
*/
#include <cmath>
#include <algorithm>
#include <limits>

constexpr double PI = 3.14159265358979323846;
constexpr double SQRT2 = 1.41421356237309504880;

// Standard normal CDF
inline double normalCDF(double x) {
    return 0.5 * (1.0 + std::erf(x / SQRT2));
}

// Standard normal PDF
inline double normalPDF(double x) {
    return std::exp(-0.5 * x * x) / std::sqrt(2.0 * PI);
}

// Bachelier call price
inline double bachelierCall(double T, double K, double F, double sigma) {
    if (sigma <= 0.0) return std::max(0.0, F - K);
    double sqrtT = std::sqrt(T);
    double d = (F - K) / (sigma * sqrtT);
    return (F - K) * normalCDF(d) + sigma * sqrtT * normalPDF(d);
}

// Bachelier vega (derivative wrt sigma)
inline double bachelierVega(double T, double K, double F, double sigma) {
    if (sigma <= 0.0) return 0.0;
    double sqrtT = std::sqrt(T);
    double d = (F - K) / (sigma * sqrtT);
    return sqrtT * normalPDF(d);
}

// Compute implied volatility using Newton-Raphson
double bachelierImpliedVol(double expiry, double strike, double price, double forward) {
    const double intrinsic = std::max(0.0, forward - strike);
    if (price <= intrinsic) return 0.0;

    double sigma = 0.1;                     // initial guess
    const double tol = 1e-10;               // price tolerance
    const int maxIter = 100;

    for (int iter = 0; iter < maxIter; ++iter) {
        double modelPrice = bachelierCall(expiry, strike, forward, sigma);
        double diff = modelPrice - price;
        if (std::fabs(diff) < tol) break;

        double vega = bachelierVega(expiry, strike, forward, sigma);
        if (vega < 1e-14) break;            // avoid division by near-zero

        sigma -= diff / vega;
        if (sigma < 0.0) sigma = 0.0;       // clamp
    }

    return std::max(0.0, sigma);
}
#include <cassert>
#include <cmath>

int main() {
    // Basic test: at-the-money, price = 2, T=1, F=K=100
    double vol1 = bachelierImpliedVol(1.0, 100.0, 2.0, 100.0);
    assert(std::fabs(vol1 - 2.0 * (std::sqrt(2.0 / PI))) < 1e-6); // approx 1.59577

    // Out-of-the-money: price = 1, T=2, F=100, K=105
    double vol2 = bachelierImpliedVol(2.0, 105.0, 1.0, 100.0);
    assert(vol2 > 0.0 && vol2 < 1.0);

    // Below intrinsic: price = 0.5, F-K=1 → intrinsic=1, price<1 → return 0
    double vol3 = bachelierImpliedVol(1.0, 99.0, 0.5, 100.0);
    assert(vol3 == 0.0);

    // Exactly intrinsic: price = F-K = 5 → return 0
    double vol4 = bachelierImpliedVol(1.0, 95.0, 5.0, 100.0);
    assert(vol4 == 0.0);

    // Deep OTM: price very small, verify recovery
    double T = 0.5;
    double K = 110.0;
    double F = 100.0;
    double sigma_true = 3.0;
    double price = bachelierCall(T, K, F, sigma_true);
    double vol5 = bachelierImpliedVol(T, K, price, F);
    assert(std::fabs(vol5 - sigma_true) < 1e-6);

    // ITM: price = call(FT, sigma=0.5) + 0.2, verify recovery
    double sigma_true2 = 0.5;
    double price2 = bachelierCall(1.0, 99.0, 100.0, sigma_true2);
    double vol6 = bachelierImpliedVol(1.0, 99.0, price2, 100.0);
    assert(std::fabs(vol6 - sigma_true2) < 1e-6);

    return 0;
}
// The solution approach is to set up a scalar root-finding problem: define `f(sigma) = BachelierCall(F, K, T, sigma) - price`. The derivative of the call price with respect to sigma is the vega, which for Bachelier is `vega = sqrt(T) * n(d)`, where `d = (F - K) / (sigma * sqrt(T))` and `n(d)` is the standard normal PDF `exp(-0.5*d*d)/sqrt(2*pi)`. Starting with `sigma = 0.1`, we repeatedly update `sigma_new = sigma - f(sigma)/vega(sigma)` until `|f| < tol` or iterations exceed 100. Edge cases: if the price is below or equal to intrinsic value, return 0.0 immediately (since negative volatility is impossible and the solver would diverge). Also, if vega becomes extremely small (near zero), we can break to avoid division by zero; but in practice for valid prices vega is positive. The normal CDF is computed via `0.5 * (1.0 + erf(d / sqrt(2)))` using `std::erf`. Complexity is O(iterations) with constant memory, typically converging in 3-5 iterations from the starting guess.
