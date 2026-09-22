/*
Write a C++ function that takes a vector of 4 three-dimensional points (each represented by a struct with `x`, `y`, and `z` members) and returns a vector of interpolated points along a cubic Bézier curve defined by these control points. The curve must start at the first point, end at the fourth point, and use the second and third points as control points that influence the shape but are not necessarily on the curve. The function should accept an integer parameter `N` specifying the number of uniformly spaced parameter values (excluding the starting point at `t = 0`) to sample, generating exactly `N` output points. For each `i` from 1 to N, compute `t = i / N` and evaluate the cubic Bézier formula:  
`P(t) = (1-t)^3 * P0 + 3*(1-t)^2*t * P1 + 3*(1-t)*t^2 * P2 + t^3 * P3`, applied independently to each coordinate. Use `double` for all arithmetic to ensure precision. The function must be named `generateBezierCurve`, accept parameters in the order `(std::vector<Point3D> controlPoints, int N)`, and return `std::vector<Point3D>`. Assume the input vector always has exactly 4 points. The output should not include the starting point at `t=0`, but must include the endpoint at `t=1`. Handle `N <= 0` by returning an empty vector. The solution must be self-contained, include necessary headers, and demonstrate proper use of `const` where applicable.
*/

#include <vector>
#include <cmath>

struct Point3D {
    double x, y, z;
};

// Generate N points on a cubic Bezier curve defined by 4 control points.
// The output excludes the start point (t=0) but includes the end point (t=1).
// Returns an empty vector if N <= 0.
std::vector<Point3D> generateBezierCurve(const std::vector<Point3D>& controlPoints, int N) {
    std::vector<Point3D> result;
    if (N <= 0 || controlPoints.size() != 4) {
        return result;
    }
    
    const Point3D& p0 = controlPoints[0];
    const Point3D& p1 = controlPoints[1];
    const Point3D& p2 = controlPoints[2];
    const Point3D& p3 = controlPoints[3];
    
    result.reserve(N);
    
    for (int i = 1; i <= N; ++i) {
        double t = static_cast<double>(i) / N;
        double oneMinusT = 1.0 - t;
        double b0 = oneMinusT * oneMinusT * oneMinusT;  // (1-t)^3
        double b1 = 3.0 * oneMinusT * oneMinusT * t;   // 3(1-t)^2 t
        double b2 = 3.0 * oneMinusT * t * t;           // 3(1-t) t^2
        double b3 = t * t * t;                         // t^3
        
        Point3D point;
        point.x = b0 * p0.x + b1 * p1.x + b2 * p2.x + b3 * p3.x;
        point.y = b0 * p0.y + b1 * p1.y + b2 * p2.y + b3 * p3.y;
        point.z = b0 * p0.z + b1 * p1.z + b2 * p2.z + b3 * p3.z;
        result.push_back(point);
    }
    
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>

// Point3D and generateBezierCurve are assumed to be included from the solution.

int main() {
    // Define a simple 2D curve in the XY-plane (z=0) for easy verification.
    std::vector<Point3D> points = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0}};
    
    // Test N = 1: only the endpoint at t=1.
    auto curve1 = generateBezierCurve(points, 1);
    assert(curve1.size() == 1);
    assert(std::fabs(curve1[0].x - 1.0) < 1e-9);
    assert(std::fabs(curve1[0].y - 1.0) < 1e-9);
    
    // Test N = 2: t = 0.5 and t = 1.0.
    auto curve2 = generateBezierCurve(points, 2);
    assert(curve2.size() == 2);
    // At t=0.5, the curve point is (0.5, 0.5) for these symmetric control points.
    assert(std::fabs(curve2[0].x - 0.5) < 1e-9);
    assert(std::fabs(curve2[0].y - 0.5) < 1e-9);
    assert(std::fabs(curve2[1].x - 1.0) < 1e-9);
    assert(std::fabs(curve2[1].y - 1.0) < 1e-9);
    
    // Test N = 3: t = 1/3, 2/3, 1.0.
    auto curve3 = generateBezierCurve(points, 3);
    assert(curve3.size() == 3);
    // Precomputed values for these control points:
    // t=1/3: x = (2/3)^3*0 + 3*(2/3)^2*(1/3)*1 + 3*(2/3)*(1/3)^2*0 + (1/3)^3*1 = 3*(4/9)*(1/3) + 1/27 = 4/9 + 1/27 = 13/27 ≈ 0.481481
    // y = (2/3)^3*0 + 3*(2/3)^2*(1/3)*0 + 3*(2/3)*(1/3)^2*1 + (1/3)^3*1 = 3*(2/3)*(1/9) + 1/27 = 2/9 + 1/27 = 7/27 ≈ 0.259259
    assert(std::fabs(curve3[0].x - 13.0/27.0) < 1e-9);
    assert(std::fabs(curve3[0].y - 7.0/27.0) < 1e-9);
    assert(std::fabs(curve3[2].x - 1.0) < 1e-9);
    assert(std::fabs(curve3[2].y - 1.0) < 1e-9);
    
    // Test that the curve starts at P0 when t=0, but we don't include it.
    // Instead verify the first point is close to P0 for large N.
    auto curveLarge = generateBezierCurve(points, 1000);
    assert(curveLarge.size() == 1000);
    assert(std::fabs(curveLarge[0].x - 0.0) < 0.01);  // near 0 but not exactly 0
    assert(std::fabs(curveLarge[0].y - 0.0) < 0.01);
    
    // Test degenerate N <= 0 returns empty.
    auto curveEmpty = generateBezierCurve(points, 0);
    assert(curveEmpty.empty());
    auto curveNegative = generateBezierCurve(points, -5);
    assert(curveNegative.empty());
    
    // Test that all coordinates are 0 when all control points are at the origin.
    std::vector<Point3D> origin = {{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}};
    auto curveOrigin = generateBezierCurve(origin, 10);
    for (const auto& p : curveOrigin) {
        assert(std::fabs(p.x) < 1e-12);
        assert(std::fabs(p.y) < 1e-12);
        assert(std::fabs(p.z) < 1e-12);
    }
    
    return 0;
}

// The core algorithm directly implements the parametric definition of a cubic Bézier curve. Given four control points `P0`, `P1`, `P2`, `P3`, the curve is a weighted combination of these points where the weights are Bernstein polynomials of degree 3: `B0(t) = (1-t)^3`, `B1(t) = 3*(1-t)^2*t`, `B2(t) = 3*(1-t)*t^2`, `B3(t) = t^3`. For each sample index `i` from 1 to N, we set `t = static_cast<double>(i) / N` (ensuring floating-point division) and compute each output coordinate as `B0*P0.coord + B1*P1.coord + B2*P2.coord + B3*P3.coord`. Edge cases include `N <= 0` (return empty) and potential precision issues when testing equality—so verification should compare with a small epsilon tolerance rather than exact equality. Time complexity is O(N) since we perform a constant number of arithmetic operations per output point. Space complexity is O(N) for the output vector, plus O(1) auxiliary space. The implementation must avoid integer division, use `double` consistently, and ensure the loop increments `t` correctly to include `t=1` exactly; using `i/N` as shown avoids floating-point accumulation errors.
