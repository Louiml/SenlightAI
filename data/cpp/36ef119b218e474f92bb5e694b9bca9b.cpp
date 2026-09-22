/*
Implement a C++ function that performs cubic B-spline interpolation of a sequence of 2D points (given as a vector of `std::pair<double, double>`) and returns a new vector of interpolated points sampled at uniform intervals along the spline's parameter domain. The input points are assumed to be in order and should not be closed (i.e., the spline is open-ended). The function must accept a `step` parameter (defaulting to `1.0/50.0`) that controls the spacing between consecutive parameter values at which the spline is sampled. For fewer than 3 input points, the function should return the original points unchanged. The output should include the first input point as the starting point, then sample the B-spline at parameter values `t = 0`, `t = step`, `t = 2*step`, ... up to and including the last input point's parameter value (which is `n-1` for `n` points). The B-spline must use uniform knot vectors and require at least 4 control points to compute non-trivial spline segments; handle cases with exactly 3 points by returning them as-is (since cubic B-splines need at least 4 points). For 4 or more points, use the standard uniform cubic B-spline basis functions (with knot vector `[0,1,2,...]`) and evaluate each segment between consecutive control points, but note that an open B-spline typically has control points equal to the input points and its curve spans from parameter `t=2` to `t=n-1` (for `n` points) if using the common open-uniform formulation; however, for simplicity, assume the control points directly define the spline and evaluate over the full range `[0, n-1]` using the standard basis functions that only depend on the local parameter within each segment. To simplify the implementation, treat each pair of consecutive input points as a segment and evaluate the cubic polynomial that is a weighted average of four consecutive control points using the standard B-spline blending functions (e.g., `B0(t) = (1-t)^3/6`, `B1(t) = (3t^3 - 6t^2 + 4)/6`, `B2(t) = (-3t^3 + 3t^2 + 3t + 1)/6`, `B3(t) = t^3/6`) with `t` normalized from 0 to 1 over each segment. For the first segment, use control points `p0`, `p1`, `p2`, `p3` (with appropriate clamping if indices go out of bounds); for the last segment, similar clamping applies. The output vector should contain exactly the number of points equal to `1 + floor((n-1)/step)` (plus possibly one more if `(n-1)` is an exact multiple of `step`), ensuring the last point is the final input point. Return the resulting vector.
*/

#include <vector>
#include <utility>
#include <cmath>

// Cubic B-spline interpolation of a sequence of 2D points.
// Given a list of points (x,y) and a sampling step (default 1/50),
// returns uniformly sampled points along the open B-spline curve.
// For <4 points, returns the original points unchanged.
std::vector<std::pair<double,double>> bspline_interpolate(
    const std::vector<std::pair<double,double>>& points,
    double step = 1.0/50.0)
{
    if (points.size() < 4) {
        return points;
    }

    if (step <= 0.0) {
        step = 1.0/50.0;
    }

    const size_t n = points.size();
    const double max_t = static_cast<double>(n - 1);

    // Number of sample points: from t=0 to max_t inclusive.
    int num_samples = static_cast<int>(std::floor(max_t / step + 1e-9)) + 1;

    std::vector<std::pair<double,double>> result;
    result.reserve(num_samples);

    for (int k = 0; k < num_samples; ++k) {
        double t = static_cast<double>(k) * step;
        if (t > max_t) t = max_t;  // avoid overshoot on last sample

        // Determine segment index and local parameter.
        int seg = static_cast<int>(std::floor(t));
        if (seg > static_cast<int>(n) - 2) {
            seg = static_cast<int>(n) - 2;
        }
        double u = t - static_cast<double>(seg);
        if (u < 0.0) u = 0.0;
        if (u > 1.0) u = 1.0;

        // Clamp control point indices for boundaries.
        auto get_point = [&](int idx) -> std::pair<double,double> {
            if (idx < 0) return points[0];
            if (idx >= static_cast<int>(n)) return points[n-1];
            return points[idx];
        };

        // Standard cubic B-spline basis functions.
        double b0 = (1.0 - u) * (1.0 - u) * (1.0 - u) / 6.0;
        double b1 = (3.0*u*u*u - 6.0*u*u + 4.0) / 6.0;
        double b2 = (-3.0*u*u*u + 3.0*u*u + 3.0*u + 1.0) / 6.0;
        double b3 = u * u * u / 6.0;

        auto p0 = get_point(seg - 1);
        auto p1 = get_point(seg);
        auto p2 = get_point(seg + 1);
        auto p3 = get_point(seg + 2);

        double x = b0*p0.first + b1*p1.first + b2*p2.first + b3*p3.first;
        double y = b0*p0.second + b1*p1.second + b2*p2.second + b3*p3.second;

        result.emplace_back(x, y);
    }

    // Ensure the last point matches the original final point exactly.
    result.back() = points.back();
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// (Assume the solution function is declared above this main)
int main() {
    // Helper to compare points with tolerance
    auto approx = [](double a, double b, double eps=1e-6) { return std::fabs(a-b) < eps; };
    auto point_eq = [&](std::pair<double,double> a, std::pair<double,double> b) {
        return approx(a.first, b.first) && approx(a.second, b.second);
    };

    // Test 1: fewer than 4 points returns unchanged
    std::vector<std::pair<double,double>> pts3 = {{0,0}, {1,1}, {2,0}};
    auto r3 = bspline_interpolate(pts3);
    assert(r3.size() == 3);
    assert(point_eq(r3[0], {0,0}));
    assert(point_eq(r3[1], {1,1}));
    assert(point_eq(r3[2], {2,0}));

    // Test 2: 4 points, step=1.0 -> returns exactly 4 points (t=0,1,2,3)
    std::vector<std::pair<double,double>> pts4 = {{0,0}, {1,2}, {2,2}, {3,0}};
    auto r4 = bspline_interpolate(pts4, 1.0);
    assert(r4.size() == 4);
    assert(point_eq(r4[0], {0,0}));
    assert(point_eq(r4[3], {3,0}));
    // At t=1 (u=1 on first segment) should be p1 = (1,2)
    assert(approx(r4[1].first, 1.0) && approx(r4[1].second, 2.0));
    // At t=2 (u=1 on second segment) should be p2 = (2,2)
    assert(approx(r4[2].first, 2.0) && approx(r4[2].second, 2.0));

    // Test 3: 5 points, step=1.0 -> returns 5 points, endpoints match
    std::vector<std::pair<double,double>> pts5 = {{0,0}, {1,1}, {2,2}, {3,1}, {4,0}};
    auto r5 = bspline_interpolate(pts5, 1.0);
    assert(r5.size() == 5);
    assert(point_eq(r5[0], {0,0}));
    assert(point_eq(r5[4], {4,0}));

    // Test 4: step too small -> still works, last point is original last
    auto r_small = bspline_interpolate(pts5, 0.1);
    assert(r_small.size() > 40);
    assert(point_eq(r_small.back(), {4,0}));

    // Test 5: empty and single point
    std::vector<std::pair<double,double>> empty;
    auto r_empty = bspline_interpolate(empty);
    assert(r_empty.empty());

    std::vector<std::pair<double,double>> single = {{5,7}};
    auto r_single = bspline_interpolate(single);
    assert(r_single.size() == 1);
    assert(point_eq(r_single[0], {5,7}));

    // Test 6: step=0 resets default
    auto r_zero_step = bspline_interpolate(pts4, 0.0);
    assert(r_zero_step.size() > 0);

    // Test 7: values are within bounding box (sanity check)
    for (auto& p : r5) {
        assert(p.first >= -0.001 && p.first <= 4.001);
        assert(p.second >= -0.001 && p.second <= 2.001);
    }

    return 0;
}

// The task requires implementing a uniform cubic B-spline interpolation for a list of 2D points. The main challenge is correctly evaluating the spline at uniformly spaced parameter values. Since the problem simplifies to treating each consecutive pair of points as a segment and using the standard cubic B-spline basis functions, the approach is: for each segment between points `p[i]` and `p[i+1]` (where `i` ranges from `0` to `n-2`), we evaluate the spline at local parameter `u` in `[0,1]`. The global parameter `t` for segment `i` is `t = i + u`, and we sample at values `t = 0, step, 2*step, ...` up to `t = n-1`. For each sample `t`, we find the segment index `i = floor(t)` and local `u = t - i`. The spline value at that `t` is computed as `B0(u)*P[i-1] + B1(u)*P[i] + B2(u)*P[i+1] + B3(u)*P[i+2]` where `P` are the input points. However, at segment `i`, the control points used are `P[i-1]`, `P[i]`, `P[i+1]`, `P[i+2]`. For the first segment (`i=0`), `P[-1]` is undefined, so we clamp to `P[0]`. For the last segment (`i=n-2`), `P[i+2]` may be out of bounds if `i+2 >= n`; clamp to `P[n-1]`. Also, for `n=4`, the first segment uses `P[0],P[1],P[2],P[3]` (all valid), and the last segment uses `P[1],P[2],P[3],P[3]` (clamped). Edge cases: if `n < 4`, return the original points (since cubic B-spline requires at least 4 control points). Also, handle `step <= 0` by defaulting to `1.0/50.0`. The output vector is constructed by iterating `t` from `0` to `n-1` inclusive, incrementing by `step`, but ensuring the last point is exactly `(n-1, P[n-1].second)` to avoid floating-point issues. A safer approach is to compute the number of samples as `1 + floor((n-1 - 0) / step + 1e-9)` and then for each sample index `k` from `0` to `num_samples-1`, compute `t = k * step`, clamp `t` to `n-1` if `k == num_samples-1`. The algorithm runs in O(num_samples) time, where `num_samples` is roughly `(n-1)/step`, and uses O(n) space (since we need to store the original points and output vector). The main numerical concern is clamping indices for out-of-bounds control points near the boundaries.
