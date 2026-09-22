/*
You are given a geometric problem: implement a C++ function `bool isInsideTriangle(Point const a, Point const b, Point const c, Point const p)` that determines whether a point `p` lies strictly inside (not on the boundary and not outside) the triangle formed by vertices `a`, `b`, and `c`. The `Point` class is predefined and has public methods `Fixed getX() const` and `Fixed getY() const` that return coordinates of type `Fixed` (a fixed-point numeric class supporting arithmetic, comparison, and `.abs()`). The triangle vertices are guaranteed distinct and non-collinear, so no degeneracy checks are required, but the point `p` may coincide with a vertex, lie on an edge, lie inside, or lie outside. Your function must return `true` only when `p` is strictly inside the triangle (not on any edge, not on a vertex). Use the cross‑product sign method to determine orientation consistently: compute the signed cross products of the point with each edge and compare their signs to the sign of the triangle’s own cross product. Additionally, ensure strict interiority by rejecting points where any of these cross products is zero (which indicates that the point lies on an edge or is collinear with a vertex). Finally, as a safety check (though not strictly necessary for exact arithmetic), verify that the sum of the absolute values of the three sub‑cross products equals the absolute value of the triangle’s cross product, with a small tolerance `1e-4` to account for floating‑point rounding; if the difference exceeds this tolerance, return `false`. The function should be `const`‑correct, taking all `Point` parameters by const reference.
*/

#include "Point.hpp"

// Helper: compute the 2D cross product of vectors (b-a) and (c-a)
Fixed cross(const Point& a, const Point& b, const Point& c) {
    return (b.getX() - a.getX()) * (c.getY() - a.getY()) -
           (c.getX() - a.getX()) * (b.getY() - a.getY());
}

// Determine if point p is strictly inside triangle (a, b, c)
bool isInsideTriangle(const Point& a, const Point& b, const Point& c, const Point& p) {
    Fixed triCross = cross(a, b, c);
    // Triangle is non-collinear by precondition; triCross should not be zero,
    // but guard against degenerate input just in case.
    if (triCross.abs() == Fixed(0)) {
        return false;
    }

    Fixed pbc = cross(p, b, c);
    Fixed pca = cross(p, c, a);
    Fixed pab = cross(p, a, b);

    // Boundary or vertex: any cross product zero means p is on an edge line
    if (pbc.abs() == Fixed(0) || pca.abs() == Fixed(0) || pab.abs() == Fixed(0)) {
        return false;
    }

    // All signs must match the triangle's orientation
    if ((pbc > Fixed(0)) != (triCross > Fixed(0)) ||
        (pca > Fixed(0)) != (triCross > Fixed(0)) ||
        (pab > Fixed(0)) != (triCross > Fixed(0))) {
        return false;
    }

    // Area sanity check: sum of sub-areas should equal triangle area.
    Fixed sumAbs = pbc.abs() + pca.abs() + pab.abs();
    return (sumAbs - triCross.abs()).abs() < Fixed(0.0001f);
}

#include <cassert>
#include "Point.hpp"

// Assume Point has a constructor Point(Fixed x, Fixed y) and Fixed has constructor from float.
int main() {
    // Triangle with vertices (0,0), (4,0), (0,4) — right triangle.
    Point a(0, 0), b(4, 0), c(0, 4);

    // Inside point
    assert(isInsideTriangle(a, b, c, Point(1, 1)) == true);

    // Outside points
    assert(isInsideTriangle(a, b, c, Point(5, 5)) == false);
    assert(isInsideTriangle(a, b, c, Point(-1, 2)) == false);

    // On edge (x=0) but not vertex
    assert(isInsideTriangle(a, b, c, Point(0, 2)) == false);

    // On hypotenuse (line x+y=4), inside edge
    assert(isInsideTriangle(a, b, c, Point(2, 2)) == false);

    // Vertex
    assert(isInsideTriangle(a, b, c, Point(0, 4)) == false);

    // Another triangle with opposite orientation (counter-clockwise vs clockwise)
    Point a2(0, 0), b2(0, 4), c2(4, 0);
    assert(isInsideTriangle(a2, b2, c2, Point(1, 1)) == true);
    assert(isInsideTriangle(a2, b2, c2, Point(3, 3)) == false);

    // Point very close to boundary but still inside
    assert(isInsideTriangle(a, b, c, Point(0.5f, 0.5f)) == true);

    return 0;
}

// The solution uses the classic “same‑side” test for point‑in‑triangle. For a triangle `(a,b,c)`, compute the signed cross product `crossABC = cross(a,b,c)` which defines the triangle’s orientation. A point `p` is inside if and only if it is on the same side of each edge as the opposite vertex. For each edge, compute `cross(p,b,c)`, `cross(p,c,a)`, and `cross(p,a,b)`. The point is inside if all three have the same sign as `crossABC`. If any sign differs, the point is outside. Boundary cases: if any cross product is exactly zero, the point lies on an edge line (possibly on the edge itself), which is not strictly inside, so return `false`. Additionally, the area check (sum of absolute sub‑cross products equals the absolute triangle cross product) mathematically holds for any point inside or on the boundary when using exact arithmetic, and for outside points the sum is strictly greater. This check is redundant if the sign test already excludes outside and boundary points, but it provides robustness for fixed‑point arithmetic with small rounding errors. Complexity: constant time, `O(1)` operations, and `O(1)` auxiliary space.
