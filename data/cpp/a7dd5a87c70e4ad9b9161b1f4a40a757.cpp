// Given a simple polygon represented as a sequence of 2D points (with integer or floating-point coordinates), write a C++ function that determines whether the polygon is convex. The function should accept a `std::vector` of points (using a simple `Point` struct with `x` and `y` coordinates) and return a `bool` indicating convexity. The polygon must have at least 3 vertices and must be simple (non-self-intersecting) for the result to be meaningful; however, your function should handle degenerate cases (fewer than 3 points) by returning `false`. Convexity is defined as: for every consecutive triple of vertices, the cross product of the vectors formed by (current → next) and (next → next-next) must have the same sign (all positive or all negative, ignoring zero for collinear points, which are allowed in a convex polygon). The polygon may be given in either clockwise or counterclockwise orientation, and the function should work correctly regardless of orientation.

#include <cassert>
#include <vector>

// Assume Point struct and isConvex function are defined above (or included from solution).

int main() {
    // Convex square (counterclockwise)
    std::vector<Point> square = {Point(0,0), Point(1,0), Point(1,1), Point(0,1)};
    assert(isConvex(square) == true);

    // Convex triangle
    std::vector<Point> triangle = {Point(0,0), Point(2,0), Point(1,2)};
    assert(isConvex(triangle) == true);

    // Non-convex (concave) quadrilateral
    std::vector<Point> concave = {Point(0,0), Point(2,0), Point(1,1), Point(0,2)};
    assert(isConvex(concave) == false);

    // Clockwise orientation (still convex)
    std::vector<Point> clockwise = {Point(0,0), Point(0,1), Point(1,1), Point(1,0)};
    assert(isConvex(clockwise) == true);

    // Fewer than 3 points
    std::vector<Point> twoPoints = {Point(0,0), Point(1,1)};
    assert(isConvex(twoPoints) == false);

    // Collinear points (all points on a line) - considered convex (no mixed signs)
    std::vector<Point> collinear = {Point(0,0), Point(1,0), Point(2,0), Point(3,0)};
    assert(isConvex(collinear) == true);

    // Degenerate with duplicate consecutive points (still no mixed signs)
    std::vector<Point> degenerate = {Point(0,0), Point(1,1), Point(1,1), Point(2,0)};
    assert(isConvex(degenerate) == false); // Actually this is concave? Let's check: 
    // p0=(0,0), p1=(1,1) => v1=(1,1); p1=(1,1), p2=(1,1) => v2=(0,0) cross=0; 
    // p1=(1,1), p2=(1,1) => v1=(0,0); p2=(1,1), p3=(2,0) => v2=(1,-1) cross=0;
    // p2=(1,1), p3=(2,0) => v1=(1,-1); p3=(2,0), p0=(0,0) => v2=(-2,0) cross=-2 (negative);
    // p3=(2,0), p0=(0,0) => v1=(-2,0); p0=(0,0), p1=(1,1) => v2=(1,1) cross=-2 (negative);
    // All non-zero cross products are negative, so technically convex? But it's self-intersecting? 
    // Actually the polygon is a bow-tie? (0,0)->(1,1)->(1,1)->(2,0)->(0,0) is degenerate but doesn't self-intersect in standard sense. 
    // Since all cross products are <=0, it returns true. The test should reflect this.
    assert(isConvex(degenerate) == true);

    // A more clear concave case: star shape (non-convex)
    std::vector<Point> star = {Point(0,0), Point(2,1), Point(1,2), Point(0,1)};
    assert(isConvex(star) == false); // this is actually a simple concave quadrilateral

    return 0;
}

#include <vector>

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }
};

// Compute the 2D cross product (z-component) of two vectors.
double cross(const Point& a, const Point& b) {
    return a.x * b.y - a.y * b.x;
}

// Determine if a simple polygon is convex.
bool isConvex(const std::vector<Point>& polygon) {
    const size_t n = polygon.size();
    if (n < 3) return false;

    // Track the sign of cross products: positive, negative, or mixed.
    bool hasPositive = false;
    bool hasNegative = false;

    for (size_t i = 0; i < n; ++i) {
        const Point& p0 = polygon[i];
        const Point& p1 = polygon[(i + 1) % n];
        const Point& p2 = polygon[(i + 2) % n];

        Point v1 = p1 - p0;
        Point v2 = p2 - p1;
        double cr = cross(v1, v2);

        if (cr > 0) {
            hasPositive = true;
        } else if (cr < 0) {
            hasNegative = true;
        }

        // Early exit if both signs appear.
        if (hasPositive && hasNegative) {
            return false;
        }
    }

    return true;
}

// The solution uses the standard cross-product sign test for convexity. For each vertex `p[i]` in the polygon, compute vectors `v1 = p[i+1] - p[i]` and `v2 = p[i+2] - p[i+1]` (indices modulo `n` for wrap-around). The cross product `v1.x * v2.y - v1.y * v2.x` gives the turning direction. For a convex polygon, all cross products must be either non-negative or non-positive (in other words, they cannot have mixed signs). Collinear points give a cross product of zero, which is acceptable because they lie on the boundary and do not change convexity. However, if there are more than two consecutive collinear points or if the entire polygon is degenerate (area zero), it may still be considered convex by this test—that is acceptable for this task. The algorithm iterates through all vertices once, so time complexity is O(n) where n is the number of vertices, and space complexity is O(1) beyond storing the input. Edge cases include polygons with fewer than 3 vertices (return false), polygons with all collinear points (cross products are all zero, so the function returns true, which is arguably correct since a line segment is convex), and polygons with repeated consecutive points (should be avoided but the test will still work as cross products become zero).
