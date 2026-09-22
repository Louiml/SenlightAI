// Write a C++ function `chopCubicAt` that takes a cubic Bézier curve defined by four 2D points (`p0`, `p1`, `p2`, `p3`) and a parameter `t` in the range [0, 1], and returns a pair of cubic Bézier curves (two arrays of four points each) such that the two curves exactly reproduce the original curve when concatenated: the first curve covers the parameter range [0, t] and the second covers [t, 1]. The first curve's last point and the second curve's first point must both equal the original curve's point at parameter `t`. The function must handle `t == 0.5` using direct averaging formulas, and for any other `t` it must use the standard de Casteljau algorithm (repeated linear interpolation). The two curves must be returned as a `std::pair` where `first` and `second` are `std::array<Point, 4>`; define a simple `Point` struct with `double x, y` and support the `-` and `+` operators for vector arithmetic. The function should be designed as a standalone utility with no dependencies on external libraries beyond the C++ standard library. Ensure that for `t = 0` the first curve degenerates to a single point (all four points equal to `p0`) and the second curve is the original curve, and symmetrically for `t = 1`.
The core algorithm is the de Casteljau algorithm for Bézier curve subdivision. For a cubic with control points `P0, P1, P2, P3`, computing the point at parameter `t` involves repeated linear interpolation. For subdivision, we compute intermediate points `Q0 = P0`, `Q1 = lerp(P0, P1, t)`, `Q2 = lerp(Q1, lerp(P1, P2, t), t)`, and `Q3 = lerp(Q2, lerp(lerp(P1, P2, t), lerp(P2, P3, t), t), t)`. The left curve is `{P0, Q1, Q2, Q3}` and the right curve is `{Q3, R1, R2, P3}` where `R1 = lerp(lerp(P1, P2, t), lerp(P2, P3, t), t)` and `R2 = lerp(P2, P3, t)`. More formally: let `A = lerp(P0, P1, t)`, `B = lerp(P1, P2, t)`, `C = lerp(P2, P3, t)`, then `D = lerp(A, B, t)`, `E = lerp(B, C, t)`, and `F = lerp(D, E, t)`. The first curve is `{P0, A, D, F}` and the second is `{F, E, C, P3}`. For `t == 0.5`, the averaging formulas can be used directly: `A = (P0+P1)/2`, `B = (P1+P2)/2`, `C = (P2+P3)/2`, `D = (P0+2P1+P2)/4`, `E = (P1+2P2+P3)/4`, `F = (P0+3P1+3P2+P3)/8`. Edge cases: when `t = 0`, the first curve collapses to all `P0`, and the second is the original; when `t = 1`, the first is the original and the second collapses to all `P3`. The function should check `t` against 0 and 1 explicitly to avoid division by zero or singular interpolations. Time complexity is O(1) with constant number of arithmetic operations per coordinate; space complexity is O(1) for storing the output.
#include <array>
#include <utility>

struct Point {
    double x, y;

    Point operator+(const Point& other) const { return {x + other.x, y + other.y}; }
    Point operator-(const Point& other) const { return {x - other.x, y - other.y}; }
    Point operator*(double scalar) const { return {x * scalar, y * scalar}; }
};

static Point lerp(const Point& a, const Point& b, double t) {
    return a + (b - a) * t;
}

// Subdivide a cubic Bézier curve at parameter t into two curves.
// The returned pair: first = left curve [0,t], second = right curve [t,1].
std::pair<std::array<Point, 4>, std::array<Point, 4>> chopCubicAt(
    const std::array<Point, 4>& cubic, double t) {
    Point p0 = cubic[0], p1 = cubic[1], p2 = cubic[2], p3 = cubic[3];

    if (t == 0.0) {
        // Left curve degenerates to a point at p0.
        std::array<Point, 4> left = {p0, p0, p0, p0};
        return {left, cubic};
    }
    if (t == 1.0) {
        // Right curve degenerates to a point at p3.
        std::array<Point, 4> right = {p3, p3, p3, p3};
        return {cubic, right};
    }

    Point a = lerp(p0, p1, t);
    Point b = lerp(p1, p2, t);
    Point c = lerp(p2, p3, t);
    Point d = lerp(a, b, t);
    Point e = lerp(b, c, t);
    Point f = lerp(d, e, t);

    return {{p0, a, d, f}, {f, e, c, p3}};
}
#include <cassert>
#include <cmath>
#include <array>

// The Point struct and chopCubicAt are assumed to be defined above.

static bool pointNear(const Point& p, const Point& q, double eps = 1e-9) {
    return std::fabs(p.x - q.x) < eps && std::fabs(p.y - q.y) < eps;
}

int main() {
    std::array<Point, 4> cubic = {{ {0.0, 0.0}, {1.0, 2.0}, {3.0, -1.0}, {4.0, 3.0} }};

    // Test t = 0.5 with direct averaging.
    auto mid = chopCubicAt(cubic, 0.5);
    assert(pointNear(mid.first[0], {0.0, 0.0}));
    assert(pointNear(mid.first[1], {0.5, 1.0}));
    assert(pointNear(mid.first[2], {1.25, 0.75}));
    assert(pointNear(mid.first[3], {2.0, 1.0}));
    assert(pointNear(mid.second[0], {2.0, 1.0}));
    assert(pointNear(mid.second[1], {2.75, 1.25}));
    assert(pointNear(mid.second[2], {3.5, 1.0}));
    assert(pointNear(mid.second[3], {4.0, 3.0}));

    // Test t = 0.
    auto at0 = chopCubicAt(cubic, 0.0);
    for (const auto& p : at0.first) {
        assert(pointNear(p, {0.0, 0.0}));
    }
    for (int i = 0; i < 4; ++i) {
        assert(pointNear(at0.second[i], cubic[i]));
    }

    // Test t = 1.
    auto at1 = chopCubicAt(cubic, 1.0);
    for (const auto& p : at1.second) {
        assert(pointNear(p, {4.0, 3.0}));
    }
    for (int i = 0; i < 4; ++i) {
        assert(pointNear(at1.first[i], cubic[i]));
    }

    // Test generic t = 0.25.
    auto quarter = chopCubicAt(cubic, 0.25);
    // Verify the joint point equals the original curve at t=0.25 using de Casteljau manually.
    Point p0 = cubic[0], p1 = cubic[1], p2 = cubic[2], p3 = cubic[3];
    Point a = p0 + (p1 - p0) * 0.25;
    Point b = p1 + (p2 - p1) * 0.25;
    Point c = p2 + (p3 - p2) * 0.25;
    Point d = a + (b - a) * 0.25;
    Point e = b + (c - b) * 0.25;
    Point f = d + (e - d) * 0.25;
    assert(pointNear(quarter.first[3], f));
    assert(pointNear(quarter.second[0], f));
    // Check endpoint continuity.
    assert(pointNear(quarter.first[0], cubic[0]));
    assert(pointNear(quarter.second[3], cubic[3]));

    // Test that the concatenated curves reproduce the original curve at several parameter values.
    // For a parameter s in [0,1], evaluate original at s, and either left or right curve accordingly.
    auto checkConcat = [&](double s) {
        // original point
        double u = 1 - s;
        Point orig = {u*u*u*p0.x + 3*u*u*s*p1.x + 3*u*s*s*p2.x + s*s*s*p3.x,
                      u*u*u*p0.y + 3*u*u*s*p1.y + 3*u*s*s*p2.y + s*s*s*p3.y};
        if (s <= 0.25) {
            // map s into [0,1] for left curve: t_left = s / 0.25
            double tl = s / 0.25;
            double v = 1 - tl;
            Point leftCurve = {v*v*v*quarter.first[0].x + 3*v*v*tl*quarter.first[1].x + 3*v*tl*tl*quarter.first[2].x + tl*tl*tl*quarter.first[3].x,
                               v*v*v*quarter.first[0].y + 3*v*v*tl*quarter.first[1].y + 3*v*tl*tl*quarter.first[2].y + tl*tl*tl*quarter.first[3].y};
            assert(pointNear(orig, leftCurve, 1e-8));
        } else {
            // map s into [0,1] for right curve: t_right = (s - 0.25) / 0.75
            double tr = (s - 0.25) / 0.75;
            double w = 1 - tr;
            Point rightCurve = {w*w*w*quarter.second[0].x + 3*w*w*tr*quarter.second[1].x + 3*w*tr*tr*quarter.second[2].x + tr*tr*tr*quarter.second[3].x,
                                w*w*w*quarter.second[0].y + 3*w*w*tr*quarter.second[1].y + 3*w*tr*tr*quarter.second[2].y + tr*tr*tr*quarter.second[3].y};
            assert(pointNear(orig, rightCurve, 1e-8));
        }
    };
    checkConcat(0.0);
    checkConcat(0.1);
    checkConcat(0.25);
    checkConcat(0.5);
    checkConcat(0.9);
    checkConcat(1.0);

    return 0;
}
