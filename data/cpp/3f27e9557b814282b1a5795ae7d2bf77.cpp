// Write a standalone C++ function `double minimumBoundingBoxWidth(const std::vector<std::pair<double, double>>& points)` that, given a set of 2D points (with possible duplicates and collinear points), returns the minimum width of a strip (the smallest distance between two parallel lines) that contains all the points. The width is defined as the minimum over all orientations of the perpendicular distance between the two supporting parallel lines. If the set has fewer than 3 points, the width is 0. The input points may be in any order, and the function must handle floating-point comparisons robustly. You may use the provided geometric primitives (convex hull, cross products, distance) but must not modify the input vector.
// The minimum width of a set of points is determined by the minimum distance between two parallel supporting lines of the convex hull. For a convex polygon, the minimum width occurs when one of the parallel lines passes through an edge of the hull and the other touches the farthest vertex from that edge. This can be found using the rotating calipers technique. First, compute the convex hull in counterclockwise order (or any consistent order) without duplicate consecutive points. If the hull has fewer than 3 points, the width is 0. Then, for each edge of the hull, find the vertex farthest from that edge using the fact that as we iterate edges in order, the farthest vertex index moves monotonically. The width for that edge is the perpendicular distance from that vertex to the edge’s line. The minimum of all such distances is the answer. Important edge cases: handle collinear points by ensuring the hull is minimal (no redundant middle points on edges). Complexity: convex hull is O(n log n), rotating calipers is O(m) where m is hull size, so total O(n log n) time and O(n) space.
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>

using Point = std::pair<double, double>;

// Cross product (b-a) x (c-a); positive if c is counterclockwise from b around a.
double cross(const Point& a, const Point& b, const Point& c) {
    return (b.first - a.first) * (c.second - a.second) -
           (b.second - a.second) * (c.first - a.first);
}

// Euclidean distance between two points.
double dist(const Point& a, const Point& b) {
    double dx = a.first - b.first;
    double dy = a.second - b.second;
    return std::sqrt(dx * dx + dy * dy);
}

// Compute the convex hull (monotone chain) in counterclockwise order, without duplicate consecutive points.
std::vector<Point> convexHull(std::vector<Point> p) {
    int n = p.size();
    if (n <= 1) return p;
    std::sort(p.begin(), p.end());
    std::vector<Point> hull(2 * n);
    int k = 0;
    // Lower hull
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(hull[k-2], hull[k-1], p[i]) <= 0) --k;
        hull[k++] = p[i];
    }
    // Upper hull
    for (int i = n-2, t = k+1; i >= 0; --i) {
        while (k >= t && cross(hull[k-2], hull[k-1], p[i]) <= 0) --k;
        hull[k++] = p[i];
    }
    hull.resize(k-1);
    // Remove duplicate last point (same as first) if present (for n>1)
    if (hull.size() > 1 && hull.front() == hull.back()) hull.pop_back();
    return hull;
}

// Returns the minimum width of a strip that contains all points.
double minimumBoundingBoxWidth(const std::vector<Point>& points) {
    if (points.size() < 3) return 0.0;
    std::vector<Point> hull = convexHull(points);
    int m = hull.size();
    if (m < 3) return 0.0; // all collinear

    // Rotating calipers: for each edge, find farthest vertex.
    // Since hull is CCW, farthest index moves monotonically.
    double minWidth = 1e100;
    for (int i = 0, j = 1; i < m; ++i) {
        // Move j forward while the area increases (i.e., distance increases).
        while (std::fabs(cross(hull[i], hull[(i+1)%m], hull[(j+1)%m])) >
               std::fabs(cross(hull[i], hull[(i+1)%m], hull[j]))) {
            j = (j + 1) % m;
        }
        // Distance from hull[j] to line through hull[i] and hull[(i+1)%m]
        double area = std::fabs(cross(hull[i], hull[(i+1)%m], hull[j]));
        double base = dist(hull[i], hull[(i+1)%m]);
        if (base > 1e-12) {
            double width = area / base; // area = base * height
            if (width < minWidth) minWidth = width;
        }
    }
    return minWidth;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

int main() {
    // Single point
    assert(std::fabs(minimumBoundingBoxWidth({{0.0, 0.0}}) - 0.0) < 1e-9);
    // Two points
    assert(std::fabs(minimumBoundingBoxWidth({{0.0, 0.0}, {3.0, 4.0}}) - 0.0) < 1e-9);
    // Three non-collinear points: width = height from longest edge? Test an equilateral triangle side 2
    // Coordinates: (0,0), (2,0), (1, sqrt(3)). Minimal width = sqrt(3) ≈ 1.73205
    {
        double w = minimumBoundingBoxWidth({{0.0,0.0}, {2.0,0.0}, {1.0, std::sqrt(3.0)}});
        assert(std::fabs(w - std::sqrt(3.0)) < 1e-9);
    }
    // Square of side 2: width = 2
    {
        std::vector<Point> sq = {{0.0,0.0}, {2.0,0.0}, {2.0,2.0}, {0.0,2.0}};
        assert(std::fabs(minimumBoundingBoxWidth(sq) - 2.0) < 1e-9);
    }
    // Collinear points
    {
        std::vector<Point> col = {{0.0,0.0}, {1.0,1.0}, {2.0,2.0}, {3.0,3.0}};
        assert(minimumBoundingBoxWidth(col) == 0.0);
    }
    // Rectangle with width 1 and height 5: minimal width = 1.0
    {
        std::vector<Point> rect = {{0.0,0.0}, {5.0,0.0}, {5.0,1.0}, {0.0,1.0}};
        assert(std::fabs(minimumBoundingBoxWidth(rect) - 1.0) < 1e-9);
    }
    // A triangle with height 3, base 4: minimal width = 2.4 (area*2/base = 12/5 = 2.4)
    {
        std::vector<Point> tri = {{0.0,0.0}, {4.0,0.0}, {0.0,3.0}};
        double w = minimumBoundingBoxWidth(tri);
        assert(std::fabs(w - 2.4) < 1e-9);
    }
    // Duplicate points
    {
        std::vector<Point> dup = {{1.0,1.0}, {1.0,1.0}, {5.0,1.0}, {3.0,4.0}};
        double w = minimumBoundingBoxWidth(dup);
        // Hull is triangle with base 4, height 3 -> width = 2.4
        assert(std::fabs(w - 2.4) < 1e-9);
    }
    return 0;
}
