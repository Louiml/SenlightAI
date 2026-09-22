/*
Write a C++ function `adaptiveTrapezoid` that computes an adaptive approximation of the definite integral of a user-provided mathematical function \( f(x) \) over the interval \([a, b]\). The function must take as parameters: the lower limit `a`, the upper limit `b`, an absolute error tolerance `epsilon`, and a callable `f` (e.g., a function pointer, lambda, or `std::function<double(double)>`). The algorithm must use an adaptive composite trapezoidal rule: start with a single trapezoid on \([a,b]\), then recursively subdivide each interval into `NChild = 2` subintervals (i.e., bisection). For each interval, compare the trapezoidal sum on that interval with the sum of the two child trapezoids; if the absolute difference is less than `epsilon`, accept the child sum. Otherwise, recursively refine each child (with the same tolerance). The return value is the approximate integral. You must handle edge cases: \(a == b\) (return 0), non-positive tolerance (return the single-trapezoid result), and extreme function values that might cause recursion depth issues (limit recursion depth to, say, 1000). Your function must be thread-safe (no global mutable state) and use `const` correctness where possible.
*/
#include <cmath>
#include <functional>
#include <stdexcept>

// Adaptive trapezoidal integration with bisection.
// f: callable taking double and returning double.
// a, b: integration limits.
// epsilon: absolute error tolerance (must be > 0 for meaningful refinement).
// Returns approximate integral over [a,b].
double adaptiveTrapezoid(const std::function<double(double)>& f, double a, double b, double epsilon) {
    // Handle trivial case: zero-length interval
    if (a == b) return 0.0;

    // If tolerance is not positive, simply return single trapezoid
    if (epsilon <= 0.0) {
        return (b - a) * (f(a) + f(b)) / 2.0;
    }

    const int MAX_DEPTH = 1000;

    // Recursive helper function
    std::function<double(double, double, double, int)> refine = [&](double left, double right, double tol, int depth) -> double {
        // Base case: if depth exceeded, use trapezoid (prevent infinite recursion)
        if (depth > MAX_DEPTH) {
            return (right - left) * (f(left) + f(right)) / 2.0;
        }

        double mid = (left + right) / 2.0;
        double f_left = f(left);
        double f_mid = f(mid);
        double f_right = f(right);

        // Trapezoid on whole interval
        double whole = (right - left) * (f_left + f_right) / 2.0;

        // Sum of two trapezoids on subintervals
        double left_part = (mid - left) * (f_left + f_mid) / 2.0;
        double right_part = (right - mid) * (f_mid + f_right) / 2.0;
        double split = left_part + right_part;

        // If refinement does not change significantly, accept split
        if (std::abs(whole - split) < tol) {
            return split;
        }

        // Otherwise, refine both halves
        double left_result = refine(left, mid, tol, depth + 1);
        double right_result = refine(mid, right, tol, depth + 1);
        return left_result + right_result;
    };

    return refine(a, b, epsilon, 0);
}
#include <cassert>
#include <cmath>
#include <functional>

// Include the solution function here or link appropriately
// For completeness, assume the above solution is in the same translation unit.

int main() {
    // Test 1: zero-length interval
    assert(adaptiveTrapezoid([](double x) { return x; }, 2.0, 2.0, 1e-6) == 0.0);

    // Test 2: linear function f(x)=x over [0,1] => 0.5
    double result_linear = adaptiveTrapezoid([](double x) { return x; }, 0.0, 1.0, 1e-6);
    assert(std::abs(result_linear - 0.5) < 1e-6);

    // Test 3: constant function f(x)=5 over [0,10] => 50
    double result_const = adaptiveTrapezoid([](double x) { return 5.0; }, 0.0, 10.0, 1e-6);
    assert(std::abs(result_const - 50.0) < 1e-6);

    // Test 4: quadratic f(x)=x^2 over [0,1] => 1/3
    double result_quad = adaptiveTrapezoid([](double x) { return x * x; }, 0.0, 1.0, 1e-6);
    assert(std::abs(result_quad - 1.0/3.0) < 1e-5);

    // Test 5: sine over [0,pi] => 2
    double result_sin = adaptiveTrapezoid([](double x) { return std::sin(x); }, 0.0, M_PI, 1e-6);
    assert(std::abs(result_sin - 2.0) < 1e-5);

    // Test 6: non-positive tolerance returns single trapezoid: f(x)=1 over [0,1] => 1
    assert(adaptiveTrapezoid([](double x) { return 1.0; }, 0.0, 1.0, 0.0) == 1.0);
    assert(adaptiveTrapezoid([](double x) { return 1.0; }, 0.0, 1.0, -1.0) == 1.0);

    // Test 7: negative interval direction: f(x)=x over [1,0] => -0.5
    double result_reverse = adaptiveTrapezoid([](double x) { return x; }, 1.0, 0.0, 1e-6);
    assert(std::abs(result_reverse - (-0.5)) < 1e-6);

    // Test 8: high accuracy on smooth function: f(x)=e^x over [0,1] => e-1
    double result_exp = adaptiveTrapezoid([](double x) { return std::exp(x); }, 0.0, 1.0, 1e-9);
    assert(std::abs(result_exp - (std::exp(1.0) - 1.0)) < 1e-7);

    // Test 9: function with large values: f(x)=x^3 over [0,10] => 2500
    double result_cube = adaptiveTrapezoid([](double x) { return x * x * x; }, 0.0, 10.0, 1e-6);
    assert(std::abs(result_cube - 2500.0) < 1e-3);

    // Test 10: deep recursion prevention: function with a spike (not smooth) but bounded depth
    double result_spike = adaptiveTrapezoid([](double x) { 
        return (x > 0.49 && x < 0.51) ? 1000.0 : 0.0; 
    }, 0.0, 1.0, 1e-6);
    // No crash, result is finite (may not be accurate due to discontinuity)
    assert(std::isfinite(result_spike));

    return 0;
}
// The algorithm is a classic adaptive integration based on the trapezoidal rule. The main idea: for a given interval \([x_1, x_2]\), compute the trapezoidal estimate \(T = (x_2 - x_1) * (f(x_1) + f(x_2))/2\). Then split the interval at the midpoint \(m\), and compute the sum of two trapezoids on \([x_1,m]\) and \([m,x_2]\), call it \(T_{split}\). If \(|T - T_{split}| < \epsilon\), we accept \(T_{split}\) as the approximation for that interval. Otherwise, we recursively call the same procedure on each half, but with the same tolerance (since we are not using error estimation techniques like Simpson's rule, we keep tolerance constant—this is a simplified but valid approach). The recursion terminates either when tolerance is met or when the interval is too small (we use a depth limit to prevent infinite recursion from floating-point precision issues). Important edge cases: if \(a == b\), the integral is zero; if \(epsilon <= 0\), we simply return the single-trapezoid result; if the user provides a function that is discontinuous or has singularities, the algorithm may not converge, so the depth limit is essential. Time complexity is \(O(2^d)\) where \(d\) is the recursion depth, which depends on the function smoothness and tolerance—worst-case is exponential in depth, but typical smooth functions converge quickly (depth ~10-20). Space complexity is \(O(d)\) for recursion stack. We'll implement a recursive helper that takes the interval endpoints, the tolerance, current depth, and the function, and returns the approximate integral for that interval. To handle the recursion efficiently, we'll pass the function by reference to avoid copying.
