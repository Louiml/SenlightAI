/*
Write a standalone C++ function named `computeSineFourierHalfPi` that computes the integral \( \int_0^\infty \frac{\sin(\omega x)}{x}\, dx \) for a given positive real value of \( \omega \) using numerical quadrature, and returns the result as a `double`. The exact mathematical value of this integral is \( \frac{\pi}{2} \) for any \( \omega > 0 \), but your function must actually perform numerical integration—do not hardcode the answer. Use the `ooura_fourier_sin` integrator from Boost.Math (as shown in the snippet) to evaluate the integral, and handle the case where `omega` is zero or negative by throwing a `std::invalid_argument`. The function should return the computed integral value with default quadrature settings (default tolerance and levels for `double`). Your solution should include appropriate headers, apply `const` correctness where possible, and be self-contained as a free function.
*/

#include <boost/math/quadrature/ooura_fourier_integrals.hpp>
#include <cmath>
#include <stdexcept>
#include <utility>

// Computes the integral ∫_0^∞ sin(ω x) / x dx for ω > 0.
// The exact value is π/2, but this function performs numerical integration.
// Throws std::invalid_argument if ω is not finite and positive.
double computeSineFourierHalfPi(const double omega) {
    if (!std::isfinite(omega) || omega <= 0.0) {
        throw std::invalid_argument("omega must be finite and positive");
    }

    // f(x) = 1/x for the integrand sin(ω x) * f(x)
    auto f = [](const double x) {
        return 1.0 / x;
    };

    boost::math::quadrature::ooura_fourier_sin<double> integrator;
    // The integrate method returns a pair: result and error estimate.
    std::pair<double, double> result = integrator.integrate(f, omega);

    return result.first;
}

#include <cassert>
#include <cmath>
#include <stdexcept>

// The solution function declaration is assumed to be available.
double computeSineFourierHalfPi(const double omega);

int main() {
    // For any positive omega, the integral should be close to pi/2.
    const double pi = 3.14159265358979323846;
    const double tolerance = 1e-7; // default error goal is ~1.49e-8

    double result1 = computeSineFourierHalfPi(1.0);
    assert(std::abs(result1 - pi / 2.0) < tolerance);

    double result2 = computeSineFourierHalfPi(5.0);
    assert(std::abs(result2 - pi / 2.0) < tolerance);

    double result3 = computeSineFourierHalfPi(0.1);
    assert(std::abs(result3 - pi / 2.0) < tolerance);

    // Test that invalid omega throws.
    bool threw = false;
    try {
        computeSineFourierHalfPi(0.0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        computeSineFourierHalfPi(-2.0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        computeSineFourierHalfPi(std::numeric_limits<double>::quiet_NaN());
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        computeSineFourierHalfPi(std::numeric_limits<double>::infinity());
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}

// The solution uses the Boost.Math quadrature routine `ooura_fourier_sin<double>` which is specifically designed for integrals of the form \( \int_0^\infty f(x) \sin(\omega x)\, dx \). Here \( f(x) = 1/x \). The integrand is singular at \( x=0 \), but the Ooura method handles this by using a change of variables and a double-exponential transformation that converges for well-behaved functions (the singularity is integrable). The input `omega` must be positive because the method expects a positive frequency; for non-positive values, we throw an exception. The integrator's `integrate` method returns a `std::pair<double, double>` where the first element is the integral value and the second is a relative error estimate. We return only the first element. The algorithm is adaptive with default tolerance (root epsilon for `double`, about 1.49e-8) and 8 levels. The complexity is not easily expressible in terms of `n` because it is adaptive, but typically it converges in a small number of evaluations (around 10–20 calls to `f`). Space usage is constant aside from internal quadrature data. Edge cases: `omega` must be > 0, and `f` must be finite for positive `x` (it is). The function should be `const`-correct by marking the parameter `omega` as a const reference or value? Since `omega` is a simple `double`, we take it by value and make it `const`. We also declare the function itself as `noexcept`? But since we throw for non-positive, we omit `noexcept`. The solution includes necessary headers: `<boost/math/quadrature/ooura_fourier_integrals.hpp>`, `<stdexcept>`, `<utility>`, `<cmath>` for `std::isfinite`? Actually we just check `omega > 0` and also check if `omega` is finite? The snippet uses `double`, so we check `omega > 0` and also `std::isfinite(omega)` to avoid NaN. If `omega` is NaN, the comparison `omega > 0` is false, so we throw. For infinity, it is > 0 but the integral may not converge? The integral for infinity is still pi/2? Actually as omega → ∞, the integral still equals pi/2, but the quadrature may have trouble? Safer to require finite positive. So check `std::isfinite(omega) && omega > 0`. We include `<cmath>` for `std::isfinite`. The function returns `double`.
