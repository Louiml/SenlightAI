// Write a C++ function named `pointInTriangle` that determines whether a given point lies strictly inside a triangle defined by three other points in a 2D plane. The function should accept four arguments: three `Point` objects representing the vertices of the triangle (in any order) and one `Point` object representing the query point. Each `Point` has two `float` members `x` and `y`. The function must return `true` if the query point is strictly inside the triangle (not on any edge and not on a vertex), and `false` otherwise. Use the sign-of-cross-product (barycentric) method. The function must be `const`-correct: it should take all points by `const&` and not modify them. The definition of `Point` is already provided externally (assume it has a constructor `Point(float x, float y)` and public `float x, y` members). You do not need to implement `Point`, only the free function.

// The solution uses the cross-product sign test. For a triangle with vertices A, B, C and a query point P, the point lies strictly inside if and only if P is on the same side of each edge (AB, BC, CA) when traversed in a consistent orientation. Compute the cross product of each edge vector with the vector from the edge's start point to P. If all three cross products have the same sign (all positive or all negative) and none are zero, then P is strictly inside. A zero cross product means P lies on that edge line; if zero occurs, we must check if P actually lies on the segment (or is a vertex), and return false in that case. To simplify, we can compute the signed area of the triangle formed by each edge and P; if any area is zero, P is collinear with that edge, so it cannot be strictly inside (it may be on the edge or outside). Then check that all three signed areas share the same sign. Edge cases: duplicate vertices in the triangle — the problem assumes a valid triangle (non-degenerate), but if vertices are collinear or coincident, the function should return false because no interior exists. For time complexity, the algorithm performs a constant number of floating-point operations (3 cross products and comparisons), so it is O(1) time and O(1) space.

#include <cmath>

// Point structure assumed available externally:
// struct Point { float x; float y; Point(float x, float y); };

// Helper: computes signed area of triangle (p, q, r) using cross product.
static float crossProduct(const Point& p, const Point& q, const Point& r) {
    return (q.x - p.x) * (r.y - p.y) - (q.y - p.y) * (r.x - p.x);
}

// Returns true iff point p is strictly inside triangle a, b, c.
// Returns false if p is on an edge, a vertex, or if triangle is degenerate.
bool pointInTriangle(const Point& a, const Point& b, const Point& c, const Point& p) {
    // Compute signed areas for the three subtriangles formed by p and each edge.
    float area1 = crossProduct(a, b, p);
    float area2 = crossProduct(b, c, p);
    float area3 = crossProduct(c, a, p);

    // If the triangle is degenerate (all points collinear), no interior exists.
    float totalArea = crossProduct(a, b, c);
    if (totalArea == 0.0f) {
        return false;
    }

    // Strictly inside requires all three signed areas to be nonzero and have the same sign.
    bool hasPositive = (area1 > 0.0f) || (area2 > 0.0f) || (area3 > 0.0f);
    bool hasNegative = (area1 < 0.0f) || (area2 < 0.0f) || (area3 < 0.0f);

    // If any area is zero, p is on the edge line (or vertex) — not strictly inside.
    if (area1 == 0.0f || area2 == 0.0f || area3 == 0.0f) {
        return false;
    }

    // Inside iff all areas have the same sign (all positive or all negative).
    return !(hasPositive && hasNegative);
}

#include <cassert>

// Minimal Point implementation for testing purposes.
struct Point {
    float x;
    float y;
    Point(float px, float py) : x(px), y(py) {}
};

// Declaration of the function under test.
bool pointInTriangle(const Point& a, const Point& b, const Point& c, const Point& p);

int main() {
    // Triangle with vertices (1,1), (1,2), (2,2) — right triangle.
    Point a(1, 1);
    Point b(1, 2);
    Point c(2, 2);

    // Point clearly inside.
    assert(pointInTriangle(a, b, c, Point(1.3f, 1.5f)) == true);

    // Point on a vertex.
    assert(pointInTriangle(a, b, c, Point(1, 1)) == false);

    // Point on an edge (between a and b).
    assert(pointInTriangle(a, b, c, Point(1, 1.5f)) == false);

    // Point on an edge (between b and c).
    assert(pointInTriangle(a, b, c, Point(1.5f, 2)) == false);

    // Point clearly outside.
    assert(pointInTriangle(a, b, c, Point(2, 1)) == false);

    // Point outside but aligned with an edge extension.
    assert(pointInTriangle(a, b, c, Point(0, 1.5f)) == false);

    // Degenerate triangle (collinear points) — no interior.
    Point d(0, 0);
    Point e(1, 1);
    Point f(2, 2);
    assert(pointInTriangle(d, e, f, Point(0.5f, 0.5f)) == false);

    // Another triangle — interior point near bottom-left.
    Point g(0, 0);
    Point h(4, 0);
    Point i(2, 3);
    assert(pointInTriangle(g, h, i, Point(2, 1)) == true);

    // Same triangle — point outside but with same orientation sign for two edges.
    assert(pointInTriangle(g, h, i, Point(3, 1)) == false);

    return 0;
}
