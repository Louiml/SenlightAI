// Write a C++ function that computes the definite integral of the periodic function \( f(x) = \frac{1}{5 - 4\cos(x)} \) over the interval \([0, 2\pi]\) using the adaptive trapezoidal rule with a specified absolute error tolerance (default `1e-6`). The function should accept a tolerance as an optional parameter and return the approximate integral as a `double`. Additionally, provide a way to verify the result against the known exact value \(\frac{2\pi}{3}\) by computing the relative error. The solution must not rely on external libraries beyond the standard C++ library (e.g., no Boost), so implement your own adaptive trapezoidal integrator with recursive interval subdivision until the estimated error falls below the tolerance. For simplicity, you may assume the function is sufficiently smooth over the interval, but ensure the integrator handles the convergence criterion robustly by comparing successive refinements within each subinterval.
The main algorithm is a recursive adaptive trapezoidal integration. Starting with the interval \([a,b]\) and the trapezoidal estimate \(T(a,b) = \frac{b-a}{2}(f(a)+f(b))\), we compute the refined estimate by splitting the interval at the midpoint \(m\): \(T(a,m)+T(m,b)\). The error estimate for the subinterval is the absolute difference between the refined and original estimates. If this error is less than the tolerance scaled appropriately (e.g., tolerance times the interval length divided by the total length, or simply tolerance times a factor), the refined estimate is accepted; otherwise, the interval is recursively split. Edge cases include: the function is periodic and smooth, so no singularities; the integral is positive; the tolerance must be positive; the endpoints are finite. The recursion depth is bounded by the tolerance and the function's variation—here, since the function is smooth and the interval is finite, the depth is modest. Time complexity in the worst case is \(O(n)\) where \(n\) is the number of subintervals needed to meet the tolerance, typically proportional to \(\sqrt{\frac{|b-a|}{\text{tolerance}}}\) for smooth functions. Space complexity is \(O(d)\) for recursion depth, which is \(O(\log n)\) in practice. We then compare the numeric result to the exact value \(\frac{2\pi}{3}\) and compute the relative error to verify correctness within an acceptable epsilon.
#include <cmath>
#include <functional>
#include <algorithm>

// Adaptive trapezoidal integration of a function f on [a,b] to a given absolute tolerance.
double adaptiveTrapezoidal(const std::function<double(double)>& f, double a, double b, double tol = 1e-6) {
    // Recursive helper: integrates f over [a,b] with error estimate tol.
    std::function<double(double,double,double,double,double)> integrate;
    integrate = [&](double left, double right, double f_left, double f_right, double tol_rec) -> double {
        double mid = 0.5 * (left + right);
        double f_mid = f(mid);
        // Trapezoidal estimate on the full interval.
        double whole = 0.5 * (right - left) * (f_left + f_right);
        // Sum of trapezoidal estimates on two halves.
        double left_part = 0.5 * (mid - left) * (f_left + f_mid);
        double right_part = 0.5 * (right - mid) * (f_mid + f_right);
        double refined = left_part + right_part;

        // Error estimate: difference between refined and whole.
        double error = std::abs(refined - whole);
        if (error <= tol_rec) {
            return refined;
        }
        // Split tolerance for each subinterval.
        double tol_half = 0.5 * tol_rec;
        return integrate(left, mid, f_left, f_mid, tol_half) +
               integrate(mid, right, f_mid, f_right, tol_half);
    };

    double fa = f(a);
    double fb = f(b);
    return integrate(a, b, fa, fb, tol);
}

// Convenience overload: integrate a C-style function pointer or lambda without std::function.
template <typename Func>
double adaptiveTrapezoidal(Func&& f, double a, double b, double tol = 1e-6) {
    return adaptiveTrapezoidal(std::function<double(double)>(f), a, b, tol);
}
#include <cassert>
#include <cmath>

// Declaration of the solution function (assumed to be in the same translation unit).
template <typename Func>
double adaptiveTrapezoidal(Func&& f, double a, double b, double tol = 1e-6);

int main() {
    // The target function: 1/(5 - 4*cos(x)).
    auto f = [](double x) { return 1.0 / (5.0 - 4.0 * std::cos(x)); };

    // Exact integral over one period: 2*pi/3.
    const double exact = 2.0 * M_PI / 3.0;

    // 1. Default tolerance.
    double result1 = adaptiveTrapezoidal(f, 0.0, 2.0 * M_PI);
    assert(std::abs(result1 - exact) < 1e-5);
    assert(result1 > 2.0 && result1 < 2.1);

    // 2. Tight tolerance.
    double result2 = adaptiveTrapezoidal(f, 0.0, 2.0 * M_PI, 1e-9);
    assert(std::abs(result2 - exact) < 1e-8);

    // 3. Over a subinterval (half period) with known symmetry: integral over [0, pi] = pi/3.
    double half = adaptiveTrapezoidal(f, 0.0, M_PI, 1e-9);
    assert(std::abs(half - (M_PI / 3.0)) < 1e-8);

    // 4. Very loose tolerance should still be in the correct ballpark.
    double loose = adaptiveTrapezoidal(f, 0.0, 2.0 * M_PI, 1e-2);
    assert(std::abs(loose - exact) < 0.01);

    // 5. Test with an alternate function: f(x)=1 over [0,1] should give 1.
    double one = adaptiveTrapezoidal([](double x) { return 1.0; }, 0.0, 1.0, 1e-12);
    assert(std::abs(one - 1.0) < 1e-10);

    // 6. Test with a linear function: f(x)=x over [0,2] gives 2.
    double linear = adaptiveTrapezoidal([](double x) { return x; }, 0.0, 2.0, 1e-12);
    assert(std::abs(linear - 2.0) < 1e-10);

    // 7. Test with a smooth curve: f(x)=sin(x) over [0, pi] gives 2.
    double sine = adaptiveTrapezoidal([](double x) { return std::sin(x); }, 0.0, M_PI, 1e-9);
    assert(std::abs(sine - 2.0) < 1e-8);

    return 0;
}
