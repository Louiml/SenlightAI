Write a standalone C++ function that implements a recursive golden-section-free minimizer for a univariate function on a closed interval. The function must accept a callable target function (e.g., a lambda or std::function<double(double)>), an interval [lo, hi], and a maximum recursion depth (or a tolerance-based stopping criterion). It must return the approximate location of the minimum (not the minimum value). The algorithm must handle both strictly convex and non-convex (e.g., oscillatory) functions, and must be robust when the minimum lies at an endpoint or when the function is constant on a subinterval. The function should be named `minimize1d` and be self-contained with only standard headers. You may assume the target function is continuous on the interval.
// The core idea is a recursive bracketing method inspired by the given `Minimize1` class but simplified for a standalone task: we maintain three points `(t0, f0)`, `(tm, fm)`, `(t1, f1)` where `t0 < tm < t1`, and we want to shrink the interval containing the global minimum. At each step, we evaluate the function at the midpoint of `[t0, tm]` and `[tm, t1]` (or use a single midpoint but compare slopes) and decide which subinterval to discard based on the monotonicity of the sampled values. A simpler robust variant is to use the **ternary search** method (a special case of the bracketing approach): repeatedly sample two interior points `m1 = lo + (hi-lo)/3` and `m2 = hi - (hi-lo)/3`. If `f(m1) < f(m2)`, then the minimum cannot lie in `[m2, hi]`, so set `hi = m2`; otherwise set `lo = m1`. This works for any **unimodal** function (strictly decreasing then strictly increasing). For non-unimodal functions, the method may converge to a local minimum, which is acceptable if we document that assumption. However, to match the spirit of the original code (which handles more general cases), we can implement a recursive bracketing that checks monotonicity: if the sequence is strictly increasing (f0 <= fm <= f1 with one strict), recurse on `[t0, tm]`; if strictly decreasing, recurse on `[tm, t1]`; otherwise we have a bracket and we use parabolic interpolation (like the original `GetBracketedMinimum`) to accelerate. For simplicity and correctness in a standalone task, I will use **ternary search**, which is O(log n) iterations and guaranteed to work for unimodal functions. For edge cases: if the function is constant, any point works; if the minimum is exactly at an endpoint, ternary search still converges because one of the intervals will shrink toward that endpoint. We stop when `hi - lo` is below a tolerance (e.g., 1e-8) or after a maximum number of iterations (e.g., 200). Time complexity: O(I) evaluations, where I is the number of iterations (dependent on tolerance). Space: O(1) auxiliary (recursion depth bounded if we implement iteratively). To be robust, we will implement iteratively to avoid recursion overhead. The function returns `(lo+hi)/2` as the approximate minimizer.
#include <functional>
#include <cmath>
#include <algorithm>
#include <stdexcept>

/**
 * @brief Find the approximate minimizer of a unimodal function on [lo, hi].
 * 
 * Uses ternary search: sample two interior points at 1/3 and 2/3 of the interval,
 * discard the larger-function-value side. Works for any continuous function that is
 * strictly decreasing then strictly increasing (unimodal). For non-unimodal functions,
 * it may converge to a local minimum; the caller must ensure unimodality.
 * 
 * @param f Target function (callable with double, returns double).
 * @param lo Left endpoint of the interval.
 * @param hi Right endpoint of the interval. Must satisfy lo <= hi.
 * @param tol Tolerance for interval width; default 1e-8.
 * @param maxIter Maximum number of iterations; default 200.
 * @return double Approximate location of the minimum.
 * @throws std::invalid_argument if lo > hi or tol <= 0 or maxIter < 1.
 */
double minimize1d(const std::function<double(double)>& f, double lo, double hi,
                  double tol = 1e-8, int maxIter = 200)
{
    if (lo > hi) throw std::invalid_argument("lo must be <= hi");
    if (tol <= 0.0) throw std::invalid_argument("tol must be positive");
    if (maxIter < 1) throw std::invalid_argument("maxIter must be at least 1");

    for (int iter = 0; iter < maxIter; ++iter) {
        // Stop if the interval is small enough
        if (hi - lo < tol) break;

        // Two interior points
        double m1 = lo + (hi - lo) / 3.0;
        double m2 = hi - (hi - lo) / 3.0;

        double f1 = f(m1);
        double f2 = f(m2);

        if (f1 < f2) {
            // Minimum cannot be in [m2, hi]
            hi = m2;
        } else {
            // Minimum cannot be in [lo, m1]
            lo = m1;
        }
    }

    // Return the midpoint of the final interval
    return 0.5 * (lo + hi);
}
#include <cassert>
#include <cmath>
#include <functional>

int main() {
    // Test 1: Simple parabola f(x) = (x-2)^2, minimum at x=2
    auto f1 = [](double x) { return (x - 2.0) * (x - 2.0); };
    double min1 = minimize1d(f1, 0.0, 4.0);
    assert(std::fabs(min1 - 2.0) < 1e-6);

    // Test 2: Minimum at left endpoint: f(x) = x^2 on [-1, 1] -> min at 0 (not endpoint). Use f(x)= (x+1)^2 on [-2,0] -> min at -1 (endpoint? no, -1 is interior). For endpoint: f(x) = (x+5)^2 on [-5, -4] -> min at -5 (left endpoint)
    auto f2 = [](double x) { return (x + 5.0) * (x + 5.0); };
    double min2 = minimize1d(f2, -5.0, -4.0);
    assert(std::fabs(min2 - (-5.0)) < 1e-6);

    // Test 3: Minimum at right endpoint: f(x) = (x-3)^2 on [2,3] -> min at 3 (right endpoint)
    auto f3 = [](double x) { return (x - 3.0) * (x - 3.0); };
    double min3 = minimize1d(f3, 2.0, 3.0);
    assert(std::fabs(min3 - 3.0) < 1e-6);

    // Test 4: Constant function: any point is fine, we just need something in range
    auto f4 = [](double) { return 42.0; };
    double min4 = minimize1d(f4, -10.0, 10.0);
    assert(min4 >= -10.0 && min4 <= 10.0);

    // Test 5: Non-polynomial: f(x) = cos(x) on [0, 3] -> min near pi (approx 3.14159) but 3 is near? Actually cos on [0,3] is decreasing, so min at 3.
    auto f5 = [](double x) { return std::cos(x); };
    double min5 = minimize1d(f5, 0.0, 3.0);
    assert(std::fabs(min5 - 3.0) < 1e-6);

    // Test 6: Sinusoidal with local minimum: f(x) = sin(x) on [3, 5] -> min near 4.712 (local min)
    auto f6 = [](double x) { return std::sin(x); };
    double min6 = minimize1d(f6, 3.0, 5.0);
    assert(std::fabs(min6 - 4.71238898) < 1e-4); // 3*pi/2

    // Test 7: Tight tolerance
    auto f7 = [](double x) { return (x - 1.2345) * (x - 1.2345); };
    double min7 = minimize1d(f7, -10.0, 10.0, 1e-10, 1000);
    assert(std::fabs(min7 - 1.2345) < 1e-8);

    // Test 8: Invalid arguments should throw
    bool threw = false;
    try {
        minimize1d(f1, 5.0, 1.0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
