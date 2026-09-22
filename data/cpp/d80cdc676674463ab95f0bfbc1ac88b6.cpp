Write a C++ function named `rosePoints` that takes an integer `k` (the number of petals/loops in a rose curve) and a positive integer `samples` (the number of line segments to use for the curve), and returns a `std::vector<std::pair<double, double>>` of 2D points that trace the rose curve defined by `radius = sin(k * angle)` for `angle` from 0 to `2*pi` inclusive. The function should generate exactly `samples + 1` points (so that consecutive points can form a continuous line strip). Handle the edge case where `samples` is 1 or less by returning a single point at the origin. Use `const double PI = 3.14159265358979323846` in your implementation. Ensure the function is self-contained, includes all necessary headers, and uses `const` correctness where appropriate. The returned points should be in order of increasing angle, starting at angle 0.
// The solution iterates over a uniform step `delta = 2*pi / samples` for `angle` from 0 to `2*pi` inclusive, producing `samples + 1` points. For each angle, compute `radius = sin(k * angle)` (where `k` can be any integer, including negative or zero – for zero, `sin(0)=0`, so all points are at the origin; for negative `k`, `sin` is odd, so the curve is mirrored). Then compute `x = radius * cos(angle)` and `y = radius * sin(angle)`. Special care: if `samples <= 0`, we cannot divide by zero or negative, so return a vector containing a single point `(0,0)`. For large `samples`, the step is small, and the curve will be smooth. Time complexity is O(samples) since we generate exactly `samples + 1` points. Space complexity is O(samples) for the output vector. Edge cases: `k=0` gives all points at origin; negative `k` works fine mathematically; `samples=1` returns two points but the segment is nearly straight; `samples` being very large may cause floating-point precision issues but is acceptable for typical values.
#include <vector>
#include <utility>
#include <cmath>

// Generate points for a rose curve r = sin(k * theta)
// over theta from 0 to 2*pi inclusive.
// Returns exactly (samples + 1) points if samples > 0,
// otherwise a single point at the origin.
std::vector<std::pair<double, double>> rosePoints(int k, int samples) {
    const double PI = 3.14159265358979323846;
    std::vector<std::pair<double, double>> points;

    // Handle degenerate samples
    if (samples <= 0) {
        points.emplace_back(0.0, 0.0);
        return points;
    }

    double delta = 2.0 * PI / static_cast<double>(samples);
    points.reserve(samples + 1);

    for (int i = 0; i <= samples; ++i) {
        double angle = i * delta;
        double radius = std::sin(static_cast<double>(k) * angle);
        double x = radius * std::cos(angle);
        double y = radius * std::sin(angle);
        points.emplace_back(x, y);
    }

    return points;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// forward declaration (already provided in solution)
std::vector<std::pair<double, double>> rosePoints(int k, int samples);

int main() {
    // Test sample count
    auto pts = rosePoints(3, 4);
    assert(pts.size() == 5);

    // Test first point is at origin (angle=0 => sin(0)=0)
    assert(std::fabs(pts[0].first) < 1e-9);
    assert(std::fabs(pts[0].second) < 1e-9);

    // Test last point is also at origin (angle=2pi)
    auto last = pts.back();
    assert(std::fabs(last.first) < 1e-9);
    assert(std::fabs(last.second) < 1e-9);

    // Test for k=0: all points are at origin
    auto zero = rosePoints(0, 10);
    for (const auto& p : zero) {
        assert(std::fabs(p.first) < 1e-9);
        assert(std::fabs(p.second) < 1e-9);
    }

    // Test for k=1: circle of radius sin(theta) (max radius 1 at pi/2)
    auto circle = rosePoints(1, 8);
    // At theta = pi/2 (index 2), radius should be ~1
    double mid_angle = 2.0 * 3.14159265358979323846 / 8.0 * 2; // pi/2
    double expected_r = std::sin(1.0 * mid_angle);
    assert(std::fabs(circle[2].first - expected_r * std::cos(mid_angle)) < 1e-6);
    assert(std::fabs(circle[2].second - expected_r * std::sin(mid_angle)) < 1e-6);

    // Test negative k: should be same as positive k but mirrored? Actually sin(-k*theta) = -sin(k*theta)
    // For k=2 vs k=-2, the radius is negated, so points are symmetric about origin.
    auto neg = rosePoints(-2, 6);
    auto pos = rosePoints(2, 6);
    assert(neg.size() == pos.size());
    for (size_t i = 0; i < pos.size(); ++i) {
        // neg point = -pos point? Actually radius = -sin(2*theta), then x = -sin*cos, y = -sin*sin
        // So neg = -pos
        assert(std::fabs(neg[i].first + pos[i].first) < 1e-6);
        assert(std::fabs(neg[i].second + pos[i].second) < 1e-6);
    }

    // Test samples = 1: should return 2 points, first at origin, second at sin(2pi)=0 also at origin
    auto one = rosePoints(5, 1);
    assert(one.size() == 2);
    assert(std::fabs(one[0].first) < 1e-9);
    assert(std::fabs(one[0].second) < 1e-9);
    assert(std::fabs(one[1].first) < 1e-9);
    assert(std::fabs(one[1].second) < 1e-9);

    // Test samples = 0: returns a single point at origin
    auto zero_samp = rosePoints(4, 0);
    assert(zero_samp.size() == 1);
    assert(std::fabs(zero_samp[0].first) < 1e-9);
    assert(std::fabs(zero_samp[0].second) < 1e-9);

    // Test large k (e.g., k=10) still returns correct number of points
    auto large = rosePoints(10, 1000);
    assert(large.size() == 1001);

    return 0;
}
