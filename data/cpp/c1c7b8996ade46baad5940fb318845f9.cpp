// Write a C++ function `double bisectionRoot(double a, double b)` that computes a root of the equation \( f(x) = x^3 - 2x - 5 \) in the interval \([a, b]\) using the bisection method with an absolute error tolerance of \(10^{-5}\) between consecutive approximations. The function must check if a root is guaranteed by the intermediate value theorem (i.e., \( f(a) \cdot f(b) < 0 \)). If the interval is invalid, return `NAN`. Otherwise, iteratively halve the interval until the absolute difference between successive midpoints is at most \(10^{-5}\), or until the midpoint exactly equals zero. Return the final midpoint as the approximated root.

// The bisection method repeatedly halves an interval \([a,b]\) known to contain a root because \( f(a) \cdot f(b) < 0 \). At each iteration, compute the midpoint \( x_r = (a+b)/2 \). If \( f(x_r) \) is exactly zero, the root is found. Otherwise, replace either \(a\) or \(b\) with \(x_r\) based on the sign of the product \( f(a) \cdot f(x_r) \): if the product is negative, the root lies in the left subinterval \([a, x_r]\), so set \( b = x_r \); if positive, the root lies in the right subinterval \([x_r, b]\), so set \( a = x_r \). To avoid infinite loops, track the previous midpoint and stop when the absolute difference between consecutive midpoints is \(\leq 10^{-5}\), which guarantees sufficient precision. Edge cases: if the interval is invalid (product ≥ 0), return `NAN`; if the function has a root exactly at an endpoint, the initial check will still be valid if product < 0, but an exact zero at a midpoint is handled. The method converges linearly; for \(n\) iterations, the interval shrinks by half each time, so reaching tolerance requires \( \log_2((b-a)/10^{-5}) \) iterations. Time complexity is \(O(\log_2((b-a)/\epsilon))\) and space complexity is \(O(1)\), as we only store a few variables.

#include <cmath>
#include <cstdlib>

// Evaluate f(x) = x^3 - 2x - 5
double f(double x) noexcept {
    return x * x * x - 2.0 * x - 5.0;
}

// Compute a root of f in [a, b] using bisection, or return NAN if invalid.
double bisectionRoot(double a, double b) {
    const double tolerance = 1e-5;

    // Check that a root is guaranteed by the intermediate value theorem.
    if (std::fabs(f(a) * f(b)) >= 0.0 && f(a) * f(b) >= 0.0) {
        return std::nan("");
    }

    double previousMid = 0.0;
    while (true) {
        double mid = (a + b) / 2.0;

        if (std::fabs(previousMid - mid) <= tolerance) {
            return mid;
        }

        if (std::fabs(f(mid)) < 1e-12) { // exact zero (within machine precision)
            return mid;
        }

        if (f(a) * f(mid) < 0.0) {
            b = mid;
        } else if (f(a) * f(mid) > 0.0) {
            a = mid;
        } else {
            return mid; // f(mid) == 0 exactly (unlikely, but safe)
        }

        previousMid = mid;
    }
}

#include <cassert>
#include <cmath>

// Declaration of the solution function (as defined above)
double f(double x);
double bisectionRoot(double a, double b);

int main() {
    // Valid interval containing the real root (~2.09455)
    double root = bisectionRoot(2.0, 3.0);
    assert(std::fabs(root - 2.09455) < 1e-4);
    assert(std::fabs(f(root)) < 1e-3);

    // Interval where f(a)*f(b) > 0 (no guaranteed root)
    assert(std::isnan(bisectionRoot(0.0, 1.0)));

    // Interval where a root is at the left endpoint? f(2)= -1 <0, f(3)=16>0 -> valid
    // Test a tighter interval
    assert(std::fabs(bisectionRoot(2.0, 2.5) - 2.09455) < 1e-4);

    // Test exact zero: f(c) = 0? c = 2.094551... not exact; but test a case with f(mid)==0 is not available, so skip.
    // Instead, check that tolerance stops correctly
    assert(std::fabs(bisectionRoot(2.09, 2.10) - 2.09455) < 1e-4);

    // Invalid with a == b
    assert(std::isnan(bisectionRoot(2.0, 2.0)));

    // Valid but narrow interval: still converges
    assert(std::fabs(bisectionRoot(2.094, 2.095) - 2.09455) < 1e-3);

    // Multiple checks with swapped endpoints (still valid)
    assert(std::fabs(bisectionRoot(3.0, 2.0) - 2.09455) < 1e-4);

    // Edge case where one endpoint is very close to root
    assert(std::fabs(bisectionRoot(2.0945, 2.095) - 2.09455) < 1e-4);

    return 0;
}
