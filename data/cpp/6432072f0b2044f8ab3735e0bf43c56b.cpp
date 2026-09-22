// Write a standalone C++ function that implements Brent's method for finding the local minimum of a unimodal function on a given interval. The function should be templated on the floating-point type and on the callable functor type. It must take as input: a callable object `f`, a lower bound `lo`, an upper bound `hi` (with `lo < hi`), a maximum number of iterations `max_iter`, and a desired precision in bits `bits`. The function must return a `std::pair<T, T>` where the first element is the approximate location of the minimum and the second is the function value at that location. The algorithm must not rely on any external libraries (like Boost) and must work correctly for any type `T` that supports basic arithmetic operations, comparison, and `std::numeric_limits<T>::epsilon()`. After the iterations, if the desired precision was not achieved, the function should still return the best estimate found so far. The function must be self-contained, with no use of global state, and must be robust for edge cases such as when the initial bracket already contains the minimum at one endpoint or when the function is flat.

Brent's method for minimization is a golden-section-search hybrid that combines parabolic interpolation with golden-section steps to ensure robustness and fast convergence. The algorithm maintains three points `a`, `b`, `c` such that `a` and `c` bracket the minimum and `b` is the current best estimate (i.e., `f(b) <= f(a)` and `f(b) <= f(c)`). In each iteration, it attempts a parabolic fit through `(a,f(a))`, `(b,f(b))`, and `(c,f(c))` to find a candidate point `x`. If the parabola yields a point that lies between `a` and `c` and is not too close to `b`, and if the step satisfies certain conditions (to avoid slow convergence), the candidate is accepted; otherwise, a golden-section step is performed. The intervals are updated based on the relative function values. The process repeats until the difference between the bracketing points is less than a tolerance derived from `sqrt(epsilon)` scaled by the magnitude of `b`, or until the maximum iteration count is exhausted. The key edge cases include: (1) ensuring the initial bracket is valid (function decreases then increases), (2) handling cases where the minimum lies exactly at an endpoint but the bracket is still valid, (3) preventing division by zero in the parabolic interpolation when points are identical, and (4) ensuring the step size does not underflow or produce points outside the bracket. The time complexity is `O(max_iter)` and space complexity is `O(1)`.

#include <cmath>
#include <limits>
#include <utility>
#include <algorithm>

// Brent's method for finding a local minimum of a unimodal function.
// f: callable object with operator()(T) returning T.
// lo, hi: bracketing interval with lo < hi.
// max_iter: maximum number of iterations.
// bits: desired precision in bits of the result (typically std::numeric_limits<T>::digits/2).
// Returns pair {x_min, f(x_min)}.
template <class F, class T>
std::pair<T, T> brent_minimize(F f, T lo, T hi, int max_iter, int bits) {
    using std::abs;
    using std::sqrt;
    using std::numeric_limits;

    const T golden = T(0.381966011250105151795413165634); // (3 - sqrt(5)) / 2

    T a = lo;
    T b = hi;
    T c = lo;
    T d = T(0);
    T e = T(0);
    T fa = f(a);
    T fb = f(b);
    T fc = fa;

    // Ensure b is the best point so far (lowest function value).
    if (fb > fa) {
        std::swap(a, b);
        std::swap(fa, fb);
    }
    c = a;
    fc = fa;

    T tol = sqrt(numeric_limits<T>::epsilon());

    for (int iter = 0; iter < max_iter; ++iter) {
        T tol1 = T(2) * numeric_limits<T>::epsilon() * abs(b) + T(0.5) * tol;
        T xm = T(0.5) * (c - b);
        if (abs(xm) <= tol1) {
            break; // Achieved desired precision
        }

        // Try parabolic fit.
        if (abs(e) > tol1 && abs(fa) > abs(fb)) {
            T s = fb / fa;
            T p, q, r;
            if (a == c) {
                // Linear interpolation (only two points available)
                p = T(2) * xm * s;
                q = T(1) - s;
            } else {
                // Parabolic interpolation with three points
                q = fa / fc;
                r = fb / fc;
                p = s * (T(2) * xm * q * (q - r) - (b - a) * (r - T(1)));
                q = (q - T(1)) * (r - T(1)) * (s - T(1));
            }
            if (p > T(0)) {
                q = -q;
            } else {
                p = -p;
            }
            if (T(2) * p < std::min(T(3) * xm * q - abs(tol1 * q), abs(e * q))) {
                // Accept parabolic step.
                e = d;
                d = p / q;
                T u = b + d;
                // Ensure u is not too close to a or c.
                if (abs(u - b) < tol1 || abs(u - a) < tol1 || abs(u - c) < tol1) {
                    d = (xm > T(0)) ? tol1 : -tol1;
                    u = b + d;
                }
                T fu = f(u);
                if (fu <= fb) {
                    // Move bracket to (a, u, b) or (b, u, c)
                    if (u < b) {
                        c = b; fc = fb;
                        b = u; fb = fu;
                    } else {
                        a = b; fa = fb;
                        b = u; fb = fu;
                    }
                } else {
                    if (u < b) {
                        a = u; fa = fu;
                    } else {
                        c = u; fc = fu;
                    }
                }
            } else {
                // Golden section step.
                e = (b < xm ? c - b : a - b);
                d = golden * e;
                T u = b + d;
                T fu = f(u);
                if (fu <= fb) {
                    if (u < b) {
                        c = b; fc = fb;
                        b = u; fb = fu;
                    } else {
                        a = b; fa = fb;
                        b = u; fb = fu;
                    }
                } else {
                    if (u < b) {
                        a = u; fa = fu;
                    } else {
                        c = u; fc = fu;
                    }
                }
            }
        } else {
            // Golden section step.
            e = (b < xm ? c - b : a - b);
            d = golden * e;
            T u = b + d;
            T fu = f(u);
            if (fu <= fb) {
                if (u < b) {
                    c = b; fc = fb;
                    b = u; fb = fu;
                } else {
                    a = b; fa = fb;
                    b = u; fb = fu;
                }
            } else {
                if (u < b) {
                    a = u; fa = fu;
                } else {
                    c = u; fc = fu;
                }
            }
        }
    }

    return std::make_pair(b, fb);
}

#include <cassert>
#include <cmath>
#include <utility>

// Test function 1: f(x) = (x + 3)(x - 1)^2, minimum at x=1.
struct Func1 {
    template <class T>
    T operator()(const T& x) const {
        return (x + T(3)) * (x - T(1)) * (x - T(1));
    }
};

// Test function 2: f(x) = x^2 + 2, minimum at x=0.
struct Func2 {
    template <class T>
    T operator()(const T& x) const {
        return x * x + T(2);
    }
};

// Test function 3: f(x) = (x - 2)^4, minimum at x=2.
struct Func3 {
    template <class T>
    T operator()(const T& x) const {
        T d = x - T(2);
        return d * d * d * d;
    }
};

int main() {
    using std::abs;

    // Test with double, bracket [-4, 4/3], desired precision 26 bits.
    {
        auto result = brent_minimize(Func1(), -4.0, 4.0/3.0, 20, 26);
        double x = result.first;
        double fx = result.second;
        assert(abs(x - 1.0) < 1e-4);
        assert(abs(fx) < 1e-4);
    }

    // Test with double, bracket [-2, 3], desired precision half bits.
    {
        auto result = brent_minimize(Func2(), -2.0, 3.0, 20, 26);
        assert(abs(result.first) < 1e-4);
        assert(abs(result.second - 2.0) < 1e-4);
    }

    // Test with float.
    {
        auto result = brent_minimize(Func3(), 0.0f, 5.0f, 20, 10);
        assert(abs(result.first - 2.0f) < 0.01f);
        assert(abs(result.second) < 0.01f);
    }

    // Test with long double and tight tolerance.
    {
        auto result = brent_minimize(Func1(), -4.0L, 4.0L/3.0L, 20, 30);
        assert(abs(result.first - 1.0L) < 1e-5L);
        assert(abs(result.second) < 1e-5L);
    }

    // Edge case: minimum exactly at lower bound (function f(x) = (x+1)^2, min at -1).
    struct Func4 {
        template <class T>
        T operator()(const T& x) const {
            return (x + T(1)) * (x + T(1));
        }
    };
    {
        auto result = brent_minimize(Func4(), T(-1), T(2), 20, 26); // Use double
        auto result_d = brent_minimize(Func4(), -1.0, 2.0, 20, 26);
        assert(abs(result_d.first + 1.0) < 1e-4);
    }

    // Edge case: flat function, f(x)=5, bracket [0,1].
    struct FuncFlat {
        template <class T>
        T operator()(const T&) const {
            return T(5);
        }
    };
    {
        auto result = brent_minimize(FuncFlat(), 0.0, 1.0, 5, 10);
        // Any point is acceptable; just ensure it returns something in range.
        assert(result.first >= 0.0 && result.first <= 1.0);
        assert(abs(result.second - 5.0) < 1e-6);
    }

    // Edge case: very small interval.
    {
        auto result = brent_minimize(Func2(), 0.1, 0.100001, 20, 26);
        // The minimum is at 0, but interval does not contain it; should converge to lower bound.
        assert(result.first >= 0.1 && result.first <= 0.100001);
        assert(abs(result.second - (result.first*result.first + 2.0)) < 1e-8);
    }

    // Edge case: fewer iterations than needed.
    {
        auto result = brent_minimize(Func1(), -4.0, 4.0/3.0, 2, 26);
        // Should still return some point within bracket.
        assert(result.first >= -4.0 && result.first <= 4.0/3.0);
        // The function value should be finite.
        assert(std::isfinite(result.second));
    }

    return 0;
}
