// Write a standalone C++ function named `insideTriangle2D` that takes three 2D triangle vertices as `std::array<float, 2>` (or a custom simple struct) and a 2D point `(x, y)`, and returns a boolean indicating whether the point lies strictly inside the triangle (excluding the edges). The function must handle triangles with arbitrary orientation (both clockwise and counter-clockwise vertex ordering) and must work correctly for degenerate triangles (where all three vertices are collinear) by returning `false` for any point. Use the cross-product sign test (as shown in the original snippet) but make it robust: you may choose a small epsilon (e.g., `1e-6`) for floating-point comparisons, but the core logic must rely on consistent sign of cross products. Do not use any external libraries like Eigen or OpenCV; only standard C++ headers. The function should be declared `constexpr`-compatible if possible, and must be self-contained with all necessary includes.
// The algorithm computes the cross product of each edge vector (from vertex i to i+1) with the vector from that vertex to the point `(x, y)`. For a point to be inside a triangle (including edges), all three cross products must have the same sign (all positive or all negative). However, the task requires strictly inside, so we also reject points where any cross product is exactly zero (on an edge). To handle floating-point imprecision and degenerate triangles, we use an epsilon value: if the triangle area is approximately zero (determined by the cross product of two edges being near zero), we return `false`. For non-degenerate triangles, we check that each cross product is either all greater than `epsilon` or all less than `-epsilon`. Edge cases: points exactly at vertices, on edges, or collinear triangles. For degenerate triangles, the cross products for any point will be zero or near zero, so the strict inequality with epsilon will correctly return `false`. Time complexity is O(1) with a fixed number of arithmetic operations. Space complexity is O(1).
#include <array>
#include <cmath>

using Point2D = std::array<float, 2>;

// Return true if point (x, y) is strictly inside the triangle defined by v0, v1, v2.
// Returns false for points on edges, at vertices, or for degenerate (collinear) triangles.
bool insideTriangle2D(float x, float y, const Point2D& v0, const Point2D& v1, const Point2D& v2) {
    const float eps = 1e-6f;

    // Compute cross product of (v1 - v0) and (v2 - v0) to detect degeneracy
    float area2 = (v1[0] - v0[0]) * (v2[1] - v0[1]) - (v1[1] - v0[1]) * (v2[0] - v0[0]);
    if (std::abs(area2) < eps) {
        return false; // Degenerate triangle (collinear or zero-area)
    }

    // Compute cross products for point relative to each edge
    float c1 = (v1[0] - v0[0]) * (y - v0[1]) - (v1[1] - v0[1]) * (x - v0[0]);
    float c2 = (v2[0] - v1[0]) * (y - v1[1]) - (v2[1] - v1[1]) * (x - v1[0]);
    float c3 = (v0[0] - v2[0]) * (y - v2[1]) - (v0[1] - v2[1]) * (x - v2[0]);

    // Strictly inside: all positive or all negative with appropriate epsilon
    return (c1 > eps && c2 > eps && c3 > eps) || (c1 < -eps && c2 < -eps && c3 < -eps);
}
#include <cassert>
#include <array>
#include <cmath>

using Point2D = std::array<float, 2>;

// Function declaration from solution
bool insideTriangle2D(float x, float y, const Point2D& v0, const Point2D& v1, const Point2D& v2);

int main() {
    // Counter-clockwise triangle: (0,0), (4,0), (0,3)
    Point2D v0 = {0.0f, 0.0f};
    Point2D v1 = {4.0f, 0.0f};
    Point2D v2 = {0.0f, 3.0f};

    // Points clearly inside
    assert(insideTriangle2D(1.0f, 1.0f, v0, v1, v2) == true);
    assert(insideTriangle2D(0.5f, 0.5f, v0, v1, v2) == true);

    // Points clearly outside
    assert(insideTriangle2D(5.0f, 5.0f, v0, v1, v2) == false);
    assert(insideTriangle2D(-1.0f, 1.0f, v0, v1, v2) == false);
    assert(insideTriangle2D(2.0f, -1.0f, v0, v1, v2) == false);

    // Points on edges or vertices should be false (strict inside)
    assert(insideTriangle2D(0.0f, 0.0f, v0, v1, v2) == false); // vertex
    assert(insideTriangle2D(2.0f, 0.0f, v0, v1, v2) == false); // edge v0-v1
    assert(insideTriangle2D(0.0f, 1.5f, v0, v1, v2) == false); // edge v0-v2

    // Clockwise triangle: (0,0), (0,3), (4,0) – reversed order
    Point2D c0 = {0.0f, 0.0f};
    Point2D c1 = {0.0f, 3.0f};
    Point2D c2 = {4.0f, 0.0f};
    assert(insideTriangle2D(1.0f, 1.0f, c0, c1, c2) == true);
    assert(insideTriangle2D(5.0f, 5.0f, c0, c1, c2) == false);

    // Degenerate triangle (collinear)
    Point2D d0 = {0.0f, 0.0f};
    Point2D d1 = {1.0f, 1.0f};
    Point2D d2 = {2.0f, 2.0f};
    assert(insideTriangle2D(0.5f, 0.5f, d0, d1, d2) == false);
    assert(insideTriangle2D(3.0f, 3.0f, d0, d1, d2) == false);

    // Very thin but non-degenerate triangle
    Point2D t0 = {0.0f, 0.0f};
    Point2D t1 = {10.0f, 0.001f};
    Point2D t2 = {10.0f, -0.001f};
    assert(insideTriangle2D(5.0f, 0.0f, t0, t1, t2) == true);
    assert(insideTriangle2D(5.0f, 0.01f, t0, t1, t2) == false);

    // High-precision near edge
    Point2D e0 = {0.0f, 0.0f};
    Point2D e1 = {1.0f, 0.0f};
    Point2D e2 = {0.0f, 1.0f};
    // Slightly inside near edge x=0.999999, y=0.000001
    assert(insideTriangle2D(0.999999f, 0.000001f, e0, e1, e2) == true);
    // Exactly on edge x=0.5, y=0.5 (hypotenuse) should be false
    assert(insideTriangle2D(0.5f, 0.5f, e0, e1, e2) == false);

    return 0;
}
