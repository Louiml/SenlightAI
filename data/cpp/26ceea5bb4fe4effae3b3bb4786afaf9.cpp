// Write a C++ function that determines whether a given point lies strictly inside a convex polygon (not on the border). The function should take a 2D point (with double coordinates) and a polygon represented as a vector of points in counterclockwise order (no duplicate first/last vertex). Return `true` if the point is strictly inside (not on the boundary), otherwise `false`. The polygon is guaranteed to be convex and have at least 3 vertices. Use cross products to implement a robust orientation check.
For a convex polygon in counterclockwise order, every edge has the polygon interior to its left. A point is strictly inside if, for every edge, the cross product of the edge vector with the vector from the edge's start to the point is strictly positive (meaning the point is consistently to the left of all edges). If any cross product is zero, the point lies on that edge (boundary), so return `false` immediately. If any cross product is negative, the point is outside. This works because convex polygons satisfy the property that a point is inside iff it is on the same side of all edges. The algorithm runs in O(n) time for n vertices and uses O(1) extra space. Edge cases: degenerate polygons (area zero) are not provided, but if they were, the cross product logic would still apply; floating-point precision may require an epsilon tolerance, but here we use exact comparison with zero for simplicity.
#include <vector>

struct Point {
    double x, y;
};

// Helper to compute cross product of vectors (b-a) and (c-a)
double cross(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

// Determine if point p is strictly inside the convex polygon (counterclockwise order).
bool isStrictlyInside(const std::vector<Point>& polygon, const Point& p) {
    const int n = static_cast<int>(polygon.size());
    for (int i = 0; i < n; ++i) {
        const Point& a = polygon[i];
        const Point& b = polygon[(i + 1) % n];
        double cp = cross(a, b, p);
        if (cp <= 0.0) {
            return false; // Outside or on boundary
        }
    }
    return true;
}
#include <cassert>

int main() {
    // Square: (0,0) -> (2,0) -> (2,2) -> (0,2) in CCW order
    std::vector<Point> square = {{0,0}, {2,0}, {2,2}, {0,2}};
    assert(isStrictlyInside(square, {1,1}) == true);
    assert(isStrictlyInside(square, {0,0}) == false); // vertex
    assert(isStrictlyInside(square, {1,0}) == false); // edge
    assert(isStrictlyInside(square, {-1,1}) == false); // outside left
    assert(isStrictlyInside(square, {3,3}) == false); // outside top-right

    // Triangle: (0,0) -> (3,0) -> (0,3)
    std::vector<Point> triangle = {{0,0}, {3,0}, {0,3}};
    assert(isStrictlyInside(triangle, {1,1}) == true);
    assert(isStrictlyInside(triangle, {0,1.5}) == false); // on edge
    assert(isStrictlyInside(triangle, {2,2}) == false); // outside

    // Pentagon (convex) with a point near center
    std::vector<Point> pentagon = {{0,0}, {2,1}, {2,3}, {0,4}, {-2,3}};
    assert(isStrictlyInside(pentagon, {0,2}) == true);
    assert(isStrictlyInside(pentagon, {0,0}) == false); // vertex
    assert(isStrictlyInside(pentagon, {-2,3}) == false); // vertex

    return 0;
}
