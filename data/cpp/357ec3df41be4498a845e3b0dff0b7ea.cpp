Write a standalone C++ function `sampleBezierPath` that takes a vector of cubic Bézier curves (each defined by four 2D points: start, control1, control2, end) and a vector of integer sample counts (one per curve, all positive), and returns a `std::vector<std::pair<double,double>>` containing the sampled points along each curve in order. For each curve, sample it at `t = 0, 1/samples, 2/samples, ..., 1.0` inclusive, using the standard cubic Bézier formula `P(t) = (1-t)^3*P0 + 3*(1-t)^2*t*P1 + 3*(1-t)*t^2*P2 + t^3*P3`. The output must concatenate the sampled points of all curves in the same order as the input. You may assume the input vectors are non‑empty and have equal size, and that all sample counts are ≥ 1. Use `std::pair<double,double>` for points and `const` references for inputs.
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// The solution function is assumed to be defined above.

int main() {
    using Point = std::pair<double,double>;
    
    // Single curve, linear from (0,0) to (10,0) with 2 samples -> t=0,0.5,1
    {
        std::vector<Point> p0 = {{0.0, 0.0}};
        std::vector<Point> p1 = {{10.0/3.0, 0.0}};  // control points on line
        std::vector<Point> p2 = {{20.0/3.0, 0.0}};
        std::vector<Point> p3 = {{10.0, 0.0}};
        std::vector<int> samples = {2};
        auto pts = sampleBezierPath(p0, p1, p2, p3, samples);
        assert(pts.size() == 3);
        assert(std::fabs(pts[0].first - 0.0) < 1e-12 && std::fabs(pts[0].second) < 1e-12);
        assert(std::fabs(pts[1].first - 5.0) < 1e-12 && std::fabs(pts[1].second) < 1e-12);
        assert(std::fabs(pts[2].first - 10.0) < 1e-12 && std::fabs(pts[2].second) < 1e-12);
    }
    
    // Two curves: first a straight line, second a vertical line, sample counts 1 and 3
    {
        std::vector<Point> p0 = {{0.0, 0.0}, {5.0, 0.0}};
        std::vector<Point> p1 = {{1.0, 0.0}, {5.0, 1.0}};
        std::vector<Point> p2 = {{4.0, 0.0}, {5.0, 4.0}};
        std::vector<Point> p3 = {{5.0, 0.0}, {5.0, 5.0}};
        std::vector<int> samples = {1, 3};
        auto pts = sampleBezierPath(p0, p1, p2, p3, samples);
        // First curve: 2 points -> (0,0) and (5,0)
        // Second curve: 4 points -> (5,0), (5,1.25), (5,3.75), (5,5)
        assert(pts.size() == 6);
        assert(pts[0] == Point(0.0, 0.0));
        assert(pts[1] == Point(5.0, 0.0));
        assert(std::fabs(pts[2].first - 5.0) < 1e-12 && std::fabs(pts[2].second - 0.0) < 1e-12);
        assert(std::fabs(pts[3].first - 5.0) < 1e-12 && std::fabs(pts[3].second - 1.25) < 1e-12);
        assert(std::fabs(pts[4].first - 5.0) < 1e-12 && std::fabs(pts[4].second - 3.75) < 1e-12);
        assert(pts[5] == Point(5.0, 5.0));
    }
    
    // Single sample (s=1) on a quadratic-like curve: check midpoint is correct
    {
        std::vector<Point> p0 = {{0.0, 0.0}};
        std::vector<Point> p1 = {{1.0, 2.0}};
        std::vector<Point> p2 = {{3.0, 2.0}};
        std::vector<Point> p3 = {{4.0, 0.0}};
        std::vector<int> samples = {1};
        auto pts = sampleBezierPath(p0, p1, p2, p3, samples);
        assert(pts.size() == 2);
        assert(pts[0] == Point(0.0, 0.0));
        // At t=0.5, formula gives (0.125*0 + 0.375*1 + 0.375*3 + 0.125*4) = 2.0
        // y = 0.375*2 + 0.375*2 = 1.5
        assert(std::fabs(pts[1].first - 0.0) < 1e-12);
        assert(std::fabs(pts[0].second - 0.0) < 1e-12);
        // The only intermediate t=0.5 is not included because s=1 -> only t=0 and t=1.
        // So we check the endpoint:
        assert(pts[1] == Point(4.0, 0.0));
    }
    
    // Test that sample counts are respected (3 samples -> 4 points per curve)
    {
        std::vector<Point> p0 = {{0.0, 0.0}};
        std::vector<Point> p1 = {{0.0, 0.0}};
        std::vector<Point> p2 = {{0.0, 0.0}};
        std::vector<Point> p3 = {{0.0, 0.0}};
        std::vector<int> samples = {3};
        auto pts = sampleBezierPath(p0, p1, p2, p3, samples);
        assert(pts.size() == 4);
        for (const auto& pt : pts) {
            assert(pt == Point(0.0, 0.0));
        }
    }
    
    // Mixed curves with different sample counts
    {
        std::vector<Point> p0 = {{0.0, 0.0}, {10.0, 0.0}};
        std::vector<Point> p1 = {{0.0, 0.0}, {10.0, 0.0}};
        std::vector<Point> p2 = {{0.0, 0.0}, {10.0, 0.0}};
        std::vector<Point> p3 = {{10.0, 0.0}, {10.0, 10.0}};
        std::vector<int> samples = {2, 1};
        auto pts = sampleBezierPath(p0, p1, p2, p3, samples);
        // Curve0: 3 points (0,0,5,10) ; Curve1: 2 points (10,0,10,10)
        assert(pts.size() == 5);
        assert(pts[0] == Point(0.0, 0.0));
        assert(pts[1] == Point(5.0, 0.0));
        assert(pts[2] == Point(10.0, 0.0));
        assert(pts[3] == Point(10.0, 0.0));
        assert(pts[4] == Point(10.0, 10.0));
    }
    
    return 0;
}
#include <vector>
#include <utility>
#include <cstddef>

// Sample a list of cubic Bezier curves at evenly spaced t values including endpoints.
std::vector<std::pair<double,double>> sampleBezierPath(
    const std::vector<std::pair<double,double>>& p0,
    const std::vector<std::pair<double,double>>& p1,
    const std::vector<std::pair<double,double>>& p2,
    const std::vector<std::pair<double,double>>& p3,
    const std::vector<int>& samples)
{
    std::vector<std::pair<double,double>> result;
    const std::size_t n = p0.size();
    
    for (std::size_t i = 0; i < n; ++i) {
        const int s = samples[i];
        const double dx0 = p0[i].first,  dy0 = p0[i].second;
        const double dx1 = p1[i].first,  dy1 = p1[i].second;
        const double dx2 = p2[i].first,  dy2 = p2[i].second;
        const double dx3 = p3[i].first,  dy3 = p3[i].second;
        
        // Sample s+1 points: t = 0, 1/s, ..., 1
        for (int k = 0; k <= s; ++k) {
            double t = static_cast<double>(k) / s;
            if (k == s) {
                // Force exact endpoint to avoid floating point drift
                result.push_back(std::make_pair(dx3, dy3));
            } else {
                double mt = 1.0 - t;
                double mt2 = mt * mt;
                double mt3 = mt2 * mt;
                double t2 = t * t;
                double t3 = t2 * t;
                
                double x = mt3 * dx0 + 3.0 * mt2 * t * dx1
                         + 3.0 * mt * t2 * dx2 + t3 * dx3;
                double y = mt3 * dy0 + 3.0 * mt2 * t * dy1
                         + 3.0 * mt * t2 * dy2 + t3 * dy3;
                result.push_back(std::make_pair(x, y));
            }
        }
    }
    return result;
}
// The solution iterates over each curve index from `0` to `n-1` (where `n` is the number of curves). For each curve, we compute the number of sample points as `samples[i] + 1` (because we include both `t=0` and `t=1`). We then loop over `k = 0` to `samples[i]`, compute `t = static_cast<double>(k) / samples[i]`, and evaluate the cubic Bézier formula. The formula is applied component‑wise to x and y coordinates. Important edge cases: (1) `samples[i]` = 1 yields exactly two points (t=0 and t=1). (2) Floating‑point precision: use `double` for all intermediate calculations; the last point (t=1) exactly equals the end point but may differ by a tiny epsilon from direct assignment, so for robustness we can set the last sample to the explicit end point if `k == samples[i]`. Time complexity is `O(sum(samples[i]))` because each curve is sampled `samples[i]+1` times; space complexity is `O(sum(samples[i]))` for the output vector, plus `O(1)` auxiliary space.
