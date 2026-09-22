// Write a standalone C++ function that implements a linear interpolator for a real-valued function on a uniform grid, matching the behavior of the provided `Linear_interpolator` class. The function should take the domain boundaries `x_min`, `x_max`, the number of grid cells `N`, and a callable (e.g., `std::function<double(double)>` or a generic functor) representing the function to interpolate. It should return a callable object (e.g., a lambda or a small struct with `operator()`) that, when called with a query point `x`, returns the linearly interpolated value if `x` lies within the precomputed grid range `[x_min, x_max)`, otherwise falls back to evaluating the original function directly. The solution must be self-contained, use appropriate `const` correctness, and include a method to compute the maximum and average absolute interpolation errors relative to the maximum function magnitude over a set of random points in the domain (as in the snippet's `max_and_average_mismatches_relative_to_max`). The grid is uniform with `N` cells, so there are `N+1` grid points including both boundaries, and interpolation is piecewise linear between adjacent grid points.
// The main algorithm is straightforward: 
// 1. Precompute the function values at all `N+1` grid points: `x_i = x_min + i * step_x` for `i = 0, ..., N`, where `step_x = (x_max - x_min) / N`. Store these in a `std::vector<double>`.
// 2. For a query `x`, compute `translated_x = x - x_min`, then `ix = floor(translated_x / step_x)`. If `0 <= ix < N`, the point lies inside the interpolation interval. Let `dx_down = translated_x - ix * step_x` and `dx_up = step_x - dx_down`. The interpolated value is `(m[ix] * dx_up + m[ix+1] * dx_down) / step_x`. This is standard linear interpolation.
// 3. If `ix` is out of bounds (i.e., `x < x_min` or `x >= x_max`), return `fun(x)` directly as a fallback. Note the exact condition: `ix` is `floor`, so negative `x` gives negative `ix`, and `x >= x_max` gives `ix >= N` because `translated_x` can equal `N * step_x` at the right boundary, but the interval condition excludes `ix == N` (since we need `ix < N` to access `m[ix+1]` with `ix+1 <= N`). For `x` exactly equal to `x_min`, `ix=0` works; for `x` exactly equal to `x_max` (the right boundary), we fall back to the exact function value, which is fine because that point is not inside any cell.
// 4. The error metric: For a given number of random points `k`, generate `k` uniformly random `x` in `[x_min, x_max)`, compute `precise = fun(x)` and `approx = interpolated(x)`, accumulate the absolute error and track the maximum absolute error, and also track the maximum absolute function value. Return the pair `(max_error / max_field, average_error / max_field)` where `max_field` is the maximum absolute function value across the sampled points. Handle the edge case where `max_field == 0` by returning `(0,0)` or a sentinel like `(1,1)` to avoid division by zero.
// 5. Complexity: Precomputation is `O(N)` time and `O(N)` space. Each query is `O(1)`. Error evaluation over `k` random points is `O(k)` time and `O(1)` extra space. The implementation should use `std::function` for genericity, or a template parameter for the functor.
//
// Edge cases: `N=0` is degenerate; either reject or handle by ensuring at least one cell (e.g., require `N>0`). Domain with `x_min == x_max` would cause division by zero; handle by requiring `x_max > x_min`. The fallback for out-of-range is essential to mimic the original behavior. The error metric uses `drand48()`; we should replace it with C++11 `<random>` to be self-contained and thread-safe, but the logic remains the same.
#include <vector>
#include <cmath>
#include <functional>
#include <random>
#include <utility>
#include <algorithm>
#include <cstddef>

// A linear interpolator for a real function on a uniform grid.
// Precomputes function values at grid points and provides operator() that 
// interpolates within the domain and falls back to the original function outside.
// Also provides a method to measure interpolation accuracy on random points.
class LinearInterpolator {
public:
    // Create the interpolator for function f on [x_min, x_max] with N cells.
    // N must be >= 1 and x_max > x_min.
    LinearInterpolator(double x_min, double x_max, std::size_t N,
                       const std::function<double(double)>& f)
        : xmin(x_min), step_x((x_max - x_min) / static_cast<double>(N)),
          N(N), fun(f) {
        // Precompute function values at all grid points.
        m.resize(N + 1);
        for (std::size_t i = 0; i <= N; ++i) {
            double x = xmin + static_cast<double>(i) * step_x;
            m[i] = fun(x);
        }
    }

    // Interpolate at point x. If x is within [xmin, xmax), use linear interpolation.
    // Otherwise, evaluate the original function directly (fallback).
    double operator()(double x) const {
        double translated_x = x - xmin;
        int ix = static_cast<int>(std::floor(translated_x / step_x));
        if (ix >= 0 && ix < static_cast<int>(N)) {
            double dx_down = translated_x - static_cast<double>(ix) * step_x;
            double dx_up = step_x - dx_down;
            return (m[ix] * dx_up + m[ix + 1] * dx_down) / step_x;
        }
        return fun(x); // fallback outside the grid
    }

    // Compute max and average absolute interpolation error relative to the 
    // maximum absolute function value, over k random points in [xmin, xmax).
    // Returns a pair (max_relative_error, average_relative_error).
    std::pair<double, double> max_and_average_relative_errors(std::size_t k) const {
        double max_error = 0.0, sum_error = 0.0, max_field = 0.0;
        // Use a fixed seed for reproducibility (can be changed).
        std::mt19937 gen(12345);
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        double domain_len = step_x * static_cast<double>(N);
        for (std::size_t i = 0; i < k; ++i) {
            double x = xmin + dist(gen) * domain_len;
            double precise = fun(x);
            double approx = (*this)(x);
            double err = std::fabs(approx - precise);
            max_field = std::max(max_field, std::fabs(precise));
            max_error = std::max(max_error, err);
            sum_error += err;
        }
        if (max_field == 0.0) {
            return {0.0, 0.0}; // avoid division by zero
        }
        double avg = sum_error / static_cast<double>(k);
        return {max_error / max_field, avg / max_field};
    }

private:
    double xmin;
    double step_x;
    std::size_t N;
    std::vector<double> m;
    std::function<double(double)> fun;
};
#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    // Test 1: Linear function f(x)=2x+1, N=10, domain [0,1].
    LinearInterpolator li1(0.0, 1.0, 10, [](double x) { return 2.0*x + 1.0; });
    // At grid points interpolation should be exact.
    assert(std::fabs(li1(0.0) - 1.0) < 1e-12);
    assert(std::fabs(li1(0.5) - 2.0) < 1e-12);
    assert(std::fabs(li1(1.0) - 3.0) < 1e-12); // fallback at right boundary gives exact
    // Midpoint between grid points: e.g., 0.05 (between 0 and 0.1) should be exact for linear.
    assert(std::fabs(li1(0.05) - 1.1) < 1e-12);
    // Outside range, fallback to original function.
    assert(std::fabs(li1(-0.1) - (2.0*(-0.1)+1.0)) < 1e-12);
    assert(std::fabs(li1(1.1) - (2.0*1.1+1.0)) < 1e-12);

    // Test 2: Non-linear function f(x)=sin(x) on [0, pi], N=100.
    LinearInterpolator li2(0.0, 3.141592653589793, 100, [](double x) { return std::sin(x); });
    // At grid points interpolation is exact.
    double grid_spacing = 3.141592653589793 / 100.0;
    assert(std::fabs(li2(0.0) - 0.0) < 1e-12);
    assert(std::fabs(li2(3.141592653589793) - std::sin(3.141592653589793)) < 1e-12);
    // At a midpoint, interpolation error should be small (linear approx of sin is good locally).
    double mid = 0.0 + 0.5 * grid_spacing;
    double expected = std::sin(mid);
    double approx = li2(mid);
    assert(std::fabs(approx - expected) < 0.001); // reasonably small

    // Test 3: Error metric for linear function should be near zero (exact interpolation).
    auto errs1 = li1.max_and_average_relative_errors(1000);
    assert(errs1.first < 1e-12);
    assert(errs1.second < 1e-12);

    // Test 4: Error metric for sin with coarse grid should be larger but still bounded.
    LinearInterpolator li3(0.0, 3.141592653589793, 10, [](double x) { return std::sin(x); });
    auto errs3 = li3.max_and_average_relative_errors(5000);
    // With 10 cells over pi, worst error ~ (dx^2)/8 * max|f''| ~ (0.314^2)/8 ≈ 0.0123.
    assert(errs3.first < 0.05);
    assert(errs3.second < 0.03);

    // Test 5: Constant function, error should be zero everywhere.
    LinearInterpolator li4(0.0, 5.0, 20, [](double) { return 42.0; });
    auto errs4 = li4.max_and_average_relative_errors(100);
    assert(errs4.first < 1e-12);
    assert(errs4.second < 1e-12);

    // Test 6: Fallback outside domain for a function with sharp behavior.
    LinearInterpolator li5(0.0, 1.0, 4, [](double x) { return x*x; });
    assert(std::fabs(li5(-2.0) - 4.0) < 1e-12); // fallback exact
    assert(std::fabs(li5(3.0) - 9.0) < 1e-12);  // fallback exact

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
