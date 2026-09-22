// Given four 2D points (p0, p1, p2, p3) that define a Catmull-Rom spline segment, write a standalone C++ function that computes and returns a `std::vector<std::pair<double,double>>` of sampled points along the curve from parameter t=0 to t=1 (inclusive) using a specified step size `dt`. The function must use the standard Catmull-Rom basis functions: b0(t) = 0.5*(-t^3 + 2t^2 - t), b1(t) = 0.5*(2 - 5t^2 + 3t^3), b2(t) = 0.5*(t + 4t^2 - 3t^3), b3(t) = 0.5*(-t^2 + t^3). The output should include the endpoint at t=1 even if it is not an exact multiple of `dt`. The function should handle `dt` values that do not divide 1 evenly, and should not rely on any external libraries beyond the standard headers. The function signature should take four point pairs and a positive double `dt` (default 0.01), and return the vector of sampled (x,y) coordinates in order of increasing t.
// The solution directly evaluates the Catmull-Rom cubic polynomial at equally spaced parameter values. The main algorithm iterates t from 0 upwards, computing the basis coefficients b0, b1, b2, b3 for each t, then combining them with the control points to produce (x,y). The loop must include t=1 explicitly; a `while (t - 1 < EPS)` loop is used to ensure the endpoint is added regardless of floating-point rounding. Edge cases include `dt` being very small (leading to many points) or very large (e.g., dt=0.5 → 3 points: t=0, t=0.5, t=1). If dt > 1, the function should still output at least the endpoints (t=0 and t=1). Time complexity is O(1/dt) and space complexity is O(1/dt) to store the result. The implementation uses `std::pair<double,double>` for points and `std::vector` for storage, with `const` correctness applied to parameters.
#include <vector>
#include <utility>
#include <cmath>

// Compute sampled points on a Catmull-Rom spline segment.
// p0, p1, p2, p3 are the four control points; the curve passes through p1 and p2.
// dt is the parameter step size (must be > 0). Returns points for t in [0,1].
std::vector<std::pair<double,double>> catmullRomSamples(
    const std::pair<double,double>& p0,
    const std::pair<double,double>& p1,
    const std::pair<double,double>& p2,
    const std::pair<double,double>& p3,
    double dt = 0.01) {
    
    std::vector<std::pair<double,double>> vertices;
    const double EPS = 1e-12;
    double t = 0.0;
    
    while (t - 1.0 < EPS) {
        double tt = t * t;
        double ttt = tt * t;
        
        double b0 = 0.5 * (-ttt + 2.0 * tt - t);
        double b1 = 0.5 * (2.0 - 5.0 * tt + 3.0 * ttt);
        double b2 = 0.5 * (t + 4.0 * tt - 3.0 * ttt);
        double b3 = 0.5 * (-tt + ttt);
        
        double x = p0.first * b0 + p1.first * b1 + p2.first * b2 + p3.first * b3;
        double y = p0.second * b0 + p1.second * b1 + p2.second * b2 + p3.second * b3;
        
        vertices.emplace_back(x, y);
        t += dt;
    }
    
    return vertices;
}
#include <cassert>
#include <cmath>

// Helper to compare two points with tolerance
bool pointsEqual(const std::pair<double,double>& a, const std::pair<double,double>& b, double eps = 1e-9) {
    return std::fabs(a.first - b.first) < eps && std::fabs(a.second - b.second) < eps;
}

int main() {
    // Straight-line case: p0=(-1,0), p1=(0,0), p2=(1,0), p3=(2,0)
    // At t=0.5, the curve should be at (0.5,0) exactly.
    auto pts1 = catmullRomSamples({-1,0}, {0,0}, {1,0}, {2,0}, 0.5);
    assert(pts1.size() == 3); // t=0, t=0.5, t=1
    assert(pointsEqual(pts1[0], {0,0}));       // t=0 -> p1
    assert(pointsEqual(pts1[1], {0.5,0}));     // t=0.5 -> midpoint
    assert(pointsEqual(pts1[2], {1,0}));       // t=1 -> p2

    // Non-divisible dt: dt=0.33 → must include t=1 anyway
    auto pts2 = catmullRomSamples({0,0}, {1,1}, {2,0}, {3,-1}, 0.33);
    assert(pts2.back().first >= 1.999999 && pts2.back().first <= 2.000001);
    assert(pts2.back().second >= -0.000001 && pts2.back().second <= 0.000001);

    // dt larger than 1: only endpoints
    auto pts3 = catmullRomSamples({0,0}, {1,1}, {2,0}, {3,-1}, 5.0);
    assert(pts3.size() == 2);
    assert(pointsEqual(pts3[0], {1,1}));
    assert(pointsEqual(pts3[1], {2,0}));

    // Vertical line test
    auto pts4 = catmullRomSamples({0,-1}, {0,0}, {0,1}, {0,2}, 0.25);
    for (const auto& p : pts4) {
        assert(std::fabs(p.first) < 1e-9);
    }
    assert(pts4.size() == 5); // t=0,0.25,0.5,0.75,1
    
    // Empty? Not possible; dt must be positive, but test dt very small produces many points
    auto pts5 = catmullRomSamples({0,0}, {1,0}, {2,0}, {3,0}, 0.001);
    assert(pts5.size() == 1001); // t=0..1 with 1001 points

    return 0;
}
