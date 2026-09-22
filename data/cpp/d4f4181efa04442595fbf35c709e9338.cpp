/*
Write a C++ function that computes the numerical integral of a user-provided mathematical function \( f(x) \) over a closed interval \([a, b]\) using the composite trapezoidal rule with an even strip count. The function must accept \( a \), \( b \), the number of strips \( n \), and a callable (e.g., `std::function<double(double)>`) representing \( f \). It should return the approximate integral as a `double`. The implementation must handle the degenerate case where \( a = b \) by returning `0.0` directly, avoid division by zero, and use a straightforward loop without external parallel libraries.
*/
#include <functional>
#include <stdexcept>

// Approximate the integral of f over [a, b] using the trapezoidal rule with n strips.
double trapezoidalIntegral(double a, double b, int n, const std::function<double(double)>& f) {
    if (n <= 0) {
        throw std::invalid_argument("Number of strips must be positive.");
    }
    if (a == b) {
        return 0.0;
    }

    const double step = (b - a) / n;
    double sum = 0.0;

    for (int i = 0; i < n; ++i) {
        double x0 = a + i * step;          // left endpoint
        double x1 = a + (i + 1) * step;    // right endpoint
        sum += f(x0) + f(x1);
    }

    return sum * step / 2.0;
}
#include <cassert>
#include <cmath>
#include <functional>

int main() {
    // Test a simple linear function: integral of x from 0 to 1 = 0.5
    auto f1 = [](double x) { return x; };
    assert(std::fabs(trapezoidalIntegral(0.0, 1.0, 1000, f1) - 0.5) < 1e-9);

    // Test constant function: integral of 3 from 2 to 5 = 3 * 3 = 9
    auto f2 = [](double x) { return 3.0; };
    assert(std::fabs(trapezoidalIntegral(2.0, 5.0, 100, f2) - 9.0) < 1e-12);

    // Test with a==b: integral must be 0
    auto f3 = [](double x) { return x * x; };
    assert(trapezoidalIntegral(0.0, 0.0, 10, f3) == 0.0);

    // Test with large n for a known integral: integral of sin(x) from 0 to pi = 2
    auto f4 = [](double x) { return std::sin(x); };
    double result_pi = trapezoidalIntegral(0.0, M_PI, 10000, f4);
    assert(std::fabs(result_pi - 2.0) < 1e-6);

    // Test negative interval: integral of x from -1 to 1 = 0
    auto f5 = [](double x) { return x; };
    assert(std::fabs(trapezoidalIntegral(-1.0, 1.0, 500, f5)) < 1e-12);

    // Test invalid n throws
    bool threw = false;
    try {
        trapezoidalIntegral(0.0, 1.0, 0, f1);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test a simple quadratic: integral of 1 from 0 to 2 = 2
    auto f6 = [](double x) { return 1.0; };
    assert(std::fabs(trapezoidalIntegral(0.0, 2.0, 1, f6) - 2.0) < 1e-12);
    return 0;
}
// The composite trapezoidal rule approximates the integral \(\int_a^b f(x)\,dx\) by dividing the interval into \(n\) equal strips of width \(h = \frac{b-a}{n}\). For each strip \(i\) from \(0\) to \(n-1\), the area is approximated as \(\frac{h}{2}[f(a+ih) + f(a+(i+1)h)]\). Summing these areas gives the total approximation. The algorithm simply iterates over all strips, evaluating the function at the required points (each interior point is evaluated twice, once as an endpoint of one strip and once as the starting point of the next, but that is acceptable per the reference implementation). Edge cases: (1) If \(a = b\), the integral is zero regardless of \(f\), so return `0.0` immediately. (2) If \(n = 0\), the problem is ill-posed; the function may assume \(n > 0\) as a precondition (or handle it by returning `0.0` to avoid division by zero). Time complexity is \(O(n)\) because we perform \(O(n)\) function evaluations and basic arithmetic operations. Space complexity is \(O(1)\) auxiliary, aside from the input callable.
