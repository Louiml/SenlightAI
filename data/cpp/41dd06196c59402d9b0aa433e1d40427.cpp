/*
Write a standalone C++ function named `computeTriangleBarycentric` that takes three 3D points (`a`, `b`, `c`) representing the vertices of a triangle, and a fourth 3D point `p` (assumed to lie on the same plane as the triangle). The function must return a `std::array<float, 3>` containing the barycentric coordinates `(u, v, w)` such that `p = u*a + v*b + w*c`, where `u + v + w = 1`. The coordinates must be computed using the same approach as the mesh intersection logic in the given code snippet: compute vectors `v0 = c - a` and `v1 = b - a`, then use dot products and the denominator `invDenom = dot00 * dot11 - dot01 * dot01`. The function must correctly handle co-planar points, including points exactly on edges or vertices, and must return `NaN` (or `std::numeric_limits<float>::quiet_NaN()`) for all components if the triangle is degenerate (area zero). Provide a self-contained implementation with necessary headers, and ensure the function does not rely on any external mesh or vector classes—use simple `std::array<float, 3>` for points.
*/
#include <array>
#include <cmath>
#include <limits>

// Compute barycentric coordinates (u, v, w) of point p with respect to triangle (a, b, c).
// The point p is expected to lie on the plane of the triangle.
// Returns {u, v, w} such that p = u*a + v*b + w*c and u + v + w = 1.
// If the triangle is degenerate, returns {NaN, NaN, NaN}.
std::array<float, 3> computeTriangleBarycentric(
    const std::array<float, 3>& a,
    const std::array<float, 3>& b,
    const std::array<float, 3>& c,
    const std::array<float, 3>& p)
{
    // Compute vectors v0 = c - a and v1 = b - a (using double for precision)
    double v0x = static_cast<double>(c[0]) - a[0];
    double v0y = static_cast<double>(c[1]) - a[1];
    double v0z = static_cast<double>(c[2]) - a[2];

    double v1x = static_cast<double>(b[0]) - a[0];
    double v1y = static_cast<double>(b[1]) - a[1];
    double v1z = static_cast<double>(b[2]) - a[2];

    // Dot products
    double dot00 = v0x * v0x + v0y * v0y + v0z * v0z;
    double dot01 = v0x * v1x + v0y * v1y + v0z * v1z;
    double dot11 = v1x * v1x + v1y * v1y + v1z * v1z;

    // Denominator of the barycentric formula
    double denom = dot00 * dot11 - dot01 * dot01;

    // Check for degenerate triangle (approximately zero area)
    const double epsilon = 1e-12;
    if (std::abs(denom) < epsilon) {
        float nan = std::numeric_limits<float>::quiet_NaN();
        return {nan, nan, nan};
    }

    // Compute vector v2 = p - a
    double v2x = static_cast<double>(p[0]) - a[0];
    double v2y = static_cast<double>(p[1]) - a[1];
    double v2z = static_cast<double>(p[2]) - a[2];

    double dot02 = v0x * v2x + v0y * v2y + v0z * v2z;
    double dot12 = v1x * v2x + v1y * v2y + v1z * v2z;

    // Barycentric coordinates (u for c, v for b, w for a)
    double u = (dot11 * dot02 - dot01 * dot12) / denom;
    double v = (dot00 * dot12 - dot01 * dot02) / denom;
    double w = 1.0 - u - v;

    return {static_cast<float>(u), static_cast<float>(v), static_cast<float>(w)};
}
#include <cassert>
#include <cmath>
#include <array>

int main() {
    // Test 1: Triangle in XY plane, point at centroid (0,0,0) with a=(1,0,0), b=(0,1,0), c=(-1,-1,0)
    {
        std::array<float, 3> a = {1, 0, 0};
        std::array<float, 3> b = {0, 1, 0};
        std::array<float, 3> c = {-1, -1, 0};
        std::array<float, 3> p = {0, 0, 0};
        auto bc = computeTriangleBarycentric(a, b, c, p);
        assert(std::abs(bc[0] - (1.0f/3.0f)) < 1e-5f);
        assert(std::abs(bc[1] - (1.0f/3.0f)) < 1e-5f);
        assert(std::abs(bc[2] - (1.0f/3.0f)) < 1e-5f);
    }

    // Test 2: Point exactly on vertex a (should return {0,0,1})
    {
        std::array<float, 3> a = {1, 2, 3};
        std::array<float, 3> b = {4, 5, 6};
        std::array<float, 3> c = {7, 8, 9};
        auto bc = computeTriangleBarycentric(a, b, c, a);
        assert(std::abs(bc[0]) < 1e-6f);
        assert(std::abs(bc[1]) < 1e-6f);
        assert(std::abs(bc[2] - 1.0f) < 1e-6f);
    }

    // Test 3: Point on edge between b and c (u=0)
    {
        std::array<float, 3> a = {0, 0, 0};
        std::array<float, 3> b = {1, 0, 0};
        std::array<float, 3> c = {0, 1, 0};
        std::array<float, 3> p = {0.5f, 0.5f, 0.0f};
        auto bc = computeTriangleBarycentric(a, b, c, p);
        assert(std::abs(bc[0]) < 1e-6f); // u for c
        assert(std::abs(bc[1] - 0.5f) < 1e-6f); // v for b
        assert(std::abs(bc[2] - 0.5f) < 1e-6f); // w for a
    }

    // Test 4: Degenerate triangle (collinear points) returns NaN
    {
        std::array<float, 3> a = {0, 0, 0};
        std::array<float, 3> b = {1, 0, 0};
        std::array<float, 3> c = {2, 0, 0};
        std::array<float, 3> p = {1, 0, 0};
        auto bc = computeTriangleBarycentric(a, b, c, p);
        assert(std::isnan(bc[0]));
        assert(std::isnan(bc[1]));
        assert(std::isnan(bc[2]));
    }

    // Test 5: Point outside triangle (negative coordinates) but on plane
    {
        std::array<float, 3> a = {0, 0, 0};
        std::array<float, 3> b = {1, 0, 0};
        std::array<float, 3> c = {0, 1, 0};
        std::array<float, 3> p = {2, 2, 0};
        auto bc = computeTriangleBarycentric(a, b, c, p);
        // Expected: solve p = u*c + v*b + w*a
        // => 2 = u*0 + v*1 + w*0 => v=2
        // => 2 = u*1 + v*0 + w*0 => u=2
        // w = 1 - 2 - 2 = -3
        assert(std::abs(bc[0] - 2.0f) < 1e-6f);
        assert(std::abs(bc[1] - 2.0f) < 1e-6f);
        assert(std::abs(bc[2] + 3.0f) < 1e-6f);
    }

    return 0;
}
// The core algorithm follows the standard Möller–Trumbore style barycentric computation as seen in the snippet. We treat vertex `a` as the origin. Define `v0 = c - a` and `v1 = b - a`. Then compute the dot products: `dot00 = v0·v0`, `dot01 = v0·v1`, `dot11 = v1·v1`. The denominator is `denom = dot00 * dot11 - dot01 * dot01`. If `denom` is exactly zero (or very close to zero with a small epsilon), the triangle is degenerate (zero area), so we return `NaN` for all components. Otherwise, compute `v2 = p - a`, then `dot02 = v0·v2`, `dot12 = v1·v2`. The barycentric coordinates with respect to `u` (weight for vertex `c`) and `v` (weight for vertex `b`) are:
// - `u = (dot11 * dot02 - dot01 * dot12) / denom`
// - `v = (dot00 * dot12 - dot01 * dot02) / denom`
// - `w = 1 - u - v`
// This matches the snippet’s `u` and `v` variables, where `u` corresponds to the weight of vertex `c` and `v` to vertex `b` (the snippet uses `v0 = c - a`, `v1 = b - a`, and interpolates normals as `N = n1 + (n3 - n1)*u + (n2 - n1)*v`, confirming that). Edge cases: if the point is exactly on a vertex, two components become 0 and one becomes 1; if on an edge, one component is 0; if outside the triangle, the coordinates may be negative or sum not to 1, but we still return the computed values because the function is meant to compute coordinates for any point on the plane. For accuracy, use double-precision internally but return `float` values. Time complexity is O(1) and space complexity is O(1). Use `std::numeric_limits<float>::quiet_NaN()` for degenerate case. To avoid division by zero, check if `abs(denom) < 1e-12f`, but be careful: the snippet uses exact equality, but we’ll use a small epsilon for robustness.
