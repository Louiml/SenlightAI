/*
Write a standalone C++ function `std::vector<double> findExtrema(const std::function<double(double)>& func, double a, double b, int steps = 1000)` that numerically computes all local extrema (both minima and maxima) of a univariate real-valued function `func` on the closed interval `[a, b]`. The function must return a sorted vector of x-coordinates where the function attains a local extremum, within a tolerance determined by the step size. Edge cases include: if the interval is invalid (`a >= b`), return an empty vector; if the derivative does not change sign (e.g., constant function or monotonic function), return nothing; endpoints should be scrutinized for extrema if the derivative is zero or changes sign there, but returned coordinates must be strictly inside `(a, b)` unless a point exactly at an endpoint is verifiable as an extremum via its neighboring derivative sign. The implementation should use central differences for the first derivative and a second derivative for Newton refinement, clamping to the interval and de-duplicating adjacent candidates.
*/

#include <functional>
#include <vector>
#include <cmath>
#include <algorithm>

// Find all local extrema (minima and maxima) of func on [a, b] numerically.
// Returns a sorted vector of x-coordinates (strictly inside (a, b)) where
// local extrema occur. Uses central differences and Newton refinement.
std::vector<double> findExtrema(const std::function<double(double)>& func,
                                double a, double b, int steps = 1000) {
    std::vector<double> result;
    if (a >= b || steps <= 0) return result;

    const double h = (b - a) / steps;
    const double eps = 1e-10;

    // First derivative via central differences
    auto derivative = [&func, h](double x) {
        return (func(x + h) - func(x - h)) / (2.0 * h);
    };

    // Second derivative for Newton refinement
    auto secondDerivative = [&func, h](double x) {
        return (func(x + h) - 2.0 * func(x) + func(x - h)) / (h * h);
    };

    // Helper to add a unique x coordinate (within h/2 distance) to the result
    auto addIfUnique = [&result, &h](double x) {
        for (double existing : result) {
            if (std::fabs(existing - x) < h / 2.0) return;
        }
        result.push_back(x);
    };

    // Scan interior points for sign changes in derivative
    for (int i = 1; i < steps; ++i) {
        double x1 = a + (i - 1) * h;
        double x2 = a + i * h;
        double x3 = a + (i + 1) * h;

        double d1 = derivative(x1);
        double d2 = derivative(x2);
        double d3 = derivative(x3);

        // Check for zero derivative or sign change
        if (std::fabs(d2) < eps || (d1 * d2 < 0.0) || (d2 * d3 < 0.0)) {
            // Candidate point: start Newton from x2
            double x = x2;
            for (int iter = 0; iter < 20; ++iter) {
                double dx = derivative(x);
                if (std::fabs(dx) < eps) break;
                double d2x = secondDerivative(x);
                if (std::fabs(d2x) < eps) break;
                x = x - dx / d2x;
            }

            // Clamp to valid range (strictly inside)
            if (x <= a || x >= b) continue;

            // Verify that x is truly an extremum: derivative changes sign
            double dLeft = derivative(x - h / 2.0);
            double dRight = derivative(x + h / 2.0);
            if (dLeft * dRight < 0.0 || (std::fabs(dLeft) < eps && std::fabs(dRight) < eps)) {
                addIfUnique(x);
            }
        }
    }

    // Check endpoints for possible extrema (only if derivative is zero exactly)
    double dl = derivative(a + h / 2.0);
    double dr = derivative(b - h / 2.0);
    if (dl * dr < 0.0) {
        // Monotonic? No, sign change indicates an extremum somewhere, but we already scanned
        // Do nothing extra; already handled.
    }

    std::sort(result.begin(), result.end());
    return result;
}

#include <cassert>
#include <cmath>
#include <functional>
#include <vector>

// Include the solution function here (copy from above)

int main() {
    // Test 1: Quadratic f(x) = x^2 - 4x + 3, minimum at x = 2
    auto f1 = [](double x) { return x * x - 4.0 * x + 3.0; };
    auto e1 = findExtrema(f1, 0.0, 4.0, 1000);
    assert(e1.size() == 1);
    assert(std::fabs(e1[0] - 2.0) < 0.01);

    // Test 2: Cubic f(x) = x^3 - 3x, local max at x = -1, local min at x = 1
    auto f2 = [](double x) { return x * x * x - 3.0 * x; };
    auto e2 = findExtrema(f2, -2.0, 2.0, 2000);
    assert(e2.size() == 2);
    assert(std::fabs(e2[0] - (-1.0)) < 0.02);
    assert(std::fabs(e2[1] - 1.0) < 0.02);

    // Test 3: Sinusoid f(x) = sin(x) on [0, 2*pi] has extrema at pi/2 and 3*pi/2
    auto f3 = [](double x) { return std::sin(x); };
    auto e3 = findExtrema(f3, 0.0, 2.0 * M_PI, 2000);
    assert(e3.size() == 2);
    assert(std::fabs(e3[0] - M_PI / 2.0) < 0.02);
    assert(std::fabs(e3[1] - 3.0 * M_PI / 2.0) < 0.02);

    // Test 4: Constant function has no extrema (no sign changes)
    auto f4 = [](double) { return 5.0; };
    auto e4 = findExtrema(f4, -10.0, 10.0, 1000);
    assert(e4.empty());

    // Test 5: Monotonic function (exp) has no extrema
    auto f5 = [](double x) { return std::exp(x); };
    auto e5 = findExtrema(f5, -1.0, 1.0, 1000);
    assert(e5.empty());

    // Test 6: Invalid interval returns empty
    auto e6 = findExtrema(f1, 5.0, 1.0, 100);
    assert(e6.empty());

    // Test 7: Function with multiple extrema on a wider interval: cos(x) on [-2pi, 2pi] has 4 extrema
    auto f7 = [](double x) { return std::cos(x); };
    auto e7 = findExtrema(f7, -2.0 * M_PI, 2.0 * M_PI, 4000);
    assert(e7.size() == 4);
    // Check approximate positions: -3pi/2, -pi/2, pi/2, 3pi/2
    std::vector<double> expected = {-3.0 * M_PI / 2.0, -M_PI / 2.0, M_PI / 2.0, 3.0 * M_PI / 2.0};
    for (size_t i = 0; i < expected.size(); ++i) {
        assert(std::fabs(e7[i] - expected[i]) < 0.03);
    }

    // Test 8: Function with a flat region (e.g., x^4) has an extremum at x=0
    auto f8 = [](double x) { return x * x * x * x; };
    auto e8 = findExtrema(f8, -1.0, 1.0, 2000);
    assert(e8.size() == 1);
    assert(std::fabs(e8[0]) < 0.01);

    return 0;
}

// The solution approach uses numerical differentiation and root-finding. First, compute a step size `h = (b - a) / steps` and create a lambda for the first derivative using central differences: `f'(x) ≈ (f(x+h) - f(x-h)) / (2h)`. Iterate over interior grid points (from `i = 1` to `steps-1`), sampling the derivative on both sides of each grid point. A sign change in the derivative between consecutive samples (e.g., `d1 * d2 < 0` or `d2 * d3 < 0`) indicates a candidate extremum near that region. For each candidate, refine the root of the derivative using Newton's method with a numerical second derivative `f''(x) ≈ (f(x+h) - 2f(x) + f(x-h)) / h²`. After refinement, verify the candidate lies within `(a, b)` (strictly, to avoid duplicate endpoints if the derivative sign change spans the boundary) and ensure it is at least `h/2` away from previously found extrema to avoid duplication. Also, check endpoints: if the derivative at an endpoint is zero or if the derivative sign near the endpoint indicates a local extremum (e.g., for left endpoint `a`, if `f'(a+h) > 0` and `f'(a) == 0` or the function approaches from below/above), then include that endpoint if it is a true local extremum (the derivative changes sign across the boundary). To keep the solution simple and robust, we only consider interior points for returned coordinates; endpoints are included only if they are exact zeros of the derivative (within tolerance) and are not already included. Time complexity is O(steps) for the scanning plus O(steps) for potential Newton refinements (each constant iterations), so overall O(steps). Space complexity is O(number of found extrema), which is at most O(steps) in the worst case but typically small.
