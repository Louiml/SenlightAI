Given a set of \(n\) points in the 2D plane (with \(n \ge 1\)), write a C++ function `bool isUniqueStableConvexHull(const std::vector<Point>& points)` that determines whether the input set is exactly the set of all points that lie on the boundary of the convex hull of the entire set, and moreover, no two adjacent vertices on that convex hull are collinear with any other input points (i.e., every edge of the convex hull contains at least one additional input point strictly between its endpoints). In other words, after removing all points that are strictly inside the convex hull, the remaining points must form a convex polygon such that each edge of that polygon has at least one input point in its interior (not just the endpoints). If the input points do not all lie on a convex polygon with this property, return `false`. The function may assume that no three input points are perfectly collinear except possibly along hull edges, and that duplicate points are not given.

// The core idea is to first compute the convex hull of the input points using the monotone chain algorithm (Graham scan with sorting by x and then y). Since the hull vertices are returned in order (counter-clockwise), we can then inspect the hull edges. After computing the hull, we must check that for every consecutive pair of hull vertices, there exists at least one other input point that lies exactly on that segment (i.e., collinear and between them). This is done by iterating over all input points and checking for collinearity with the edge endpoints. If any edge has no such interior point, the set is not "unique stable" (as the original problem states that a stable convex hull requires every edge to have at least one point in between). Additionally, the convex hull itself must be non-degenerate (i.e., have at least 3 distinct vertices). Edge cases include: fewer than 6 points (since with 5 or fewer points, no edge can have an interior point, unless degenerate), all points collinear (hull has 2 vertices, return false), and hull edges that are already non-collinear with any interior points (return false). Time complexity is \(O(n \log n)\) for sorting and hull construction, plus \(O(n \cdot h)\) for checking each hull edge against all points, where \(h\) is the number of hull vertices; in worst case \(O(n^2)\). Space complexity is \(O(n)\) for storing points and hull stack.

#include <vector>
#include <algorithm>
#include <cmath>
#include <cstddef>

struct Point {
    double x, y;
    Point() : x(0.0), y(0.0) {}
    Point(double x_, double y_) : x(x_), y(y_) {}
};

// Cross product sign: positive if c is left of a->b, negative if right, zero if collinear
static int cross(const Point& a, const Point& b, const Point& c) {
    double val = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    const double eps = 1e-9;
    if (std::fabs(val) < eps) return 0;
    return val > 0 ? 1 : -1;
}

// Monotone chain convex hull: returns hull vertices in CCW order, no collinear points on edges
static std::vector<Point> convexHull(std::vector<Point> pts) {
    std::sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    });
    if (pts.size() <= 1) return pts;

    std::vector<Point> hull;
    // Lower hull
    for (const Point& p : pts) {
        while (hull.size() >= 2 && cross(hull[hull.size()-2], hull.back(), p) <= 0)
            hull.pop_back();
        hull.push_back(p);
    }
    // Upper hull
    int lower_size = hull.size();
    for (int i = pts.size()-2; i >= 0; --i) {
        Point p = pts[i];
        while (hull.size() > lower_size && cross(hull[hull.size()-2], hull.back(), p) <= 0)
            hull.pop_back();
        hull.push_back(p);
    }
    hull.pop_back(); // remove last duplicate of first point
    if (hull.size() == 2 && hull[0].x == hull[1].x && hull[0].y == hull[1].y) hull.pop_back();
    return hull;
}

// Determine if every edge of the convex hull has at least one input point strictly in its interior.
bool isUniqueStableConvexHull(const std::vector<Point>& points) {
    if (points.empty()) return false;
    if (points.size() <= 5) return false; // with 6 or fewer points, edges cannot have interior points

    // Get hull vertices
    std::vector<Point> hull = convexHull(points);
    int h = hull.size();
    if (h < 3) return false; // degenerate hull (line or point)

    // For each hull edge, check if there is a point strictly between endpoints
    for (int i = 0; i < h; ++i) {
        const Point& a = hull[i];
        const Point& b = hull[(i+1) % h];
        bool found_interior = false;
        for (const Point& p : points) {
            if (p.x == a.x && p.y == a.y) continue;
            if (p.x == b.x && p.y == b.y) continue;
            // Collinear and between? Use dot product to ensure betweenness
            if (cross(a, b, p) == 0) {
                double dot = (p.x - a.x)*(p.x - b.x) + (p.y - a.y)*(p.y - b.y);
                if (dot < 0) { // p is between a and b
                    found_interior = true;
                    break;
                }
            }
        }
        if (!found_interior) return false;
    }
    return true;
}

#include <cassert>
#include <vector>

// function prototype included from solution
bool isUniqueStableConvexHull(const std::vector<Point>& points);

int main() {
    // Case 1: A rectangle with extra points on each edge -> stable
    std::vector<Point> stable {
        Point(0,0), Point(1,0), Point(2,0), // bottom edge with interior point
        Point(2,2), Point(1,2), Point(0,2), // top edge with interior point
        Point(0,1), Point(2,1)              // left and right edges? left edge has (0,1), right has (2,1) -> but those are on vertical edges? Actually left edge from (0,0)-(0,2) has (0,1) as interior; right edge from (2,0)-(2,2) has (2,1) as interior
    };
    // Wait: we need every edge to have an interior point: For rectangle with corners (0,0),(2,0),(2,2),(0,2):
    // bottom edge from (0,0)-(2,0) has (1,0) interior; right edge (2,0)-(2,2) has (2,1); top edge (2,2)-(0,2) has (1,2); left edge (0,2)-(0,0) has (0,1). All provided.
    assert(isUniqueStableConvexHull(stable) == true);

    // Case 2: A square without interior points on any edge -> not stable
    std::vector<Point> square { Point(0,0), Point(1,0), Point(1,1), Point(0,1) };
    assert(isUniqueStableConvexHull(square) == false);

    // Case 3: Pentagon with interior point on one edge, but missing on another
    std::vector<Point> partial {
        Point(0,0), Point(1,0), Point(2,0), // bottom edge has interior
        Point(2,1), Point(1,2), Point(0,1)  // other edges have no interior points
    };
    assert(isUniqueStableConvexHull(partial) == false);

    // Case 4: All points collinear -> degenerate hull
    std::vector<Point> line { Point(0,0), Point(1,0), Point(2,0), Point(3,0), Point(4,0), Point(5,0) };
    assert(isUniqueStableConvexHull(line) == false);

    // Case 5: More than 6 points, but only 3 hull vertices with no interior points
    std::vector<Point> triangle {
        Point(0,0), Point(1,0), Point(2,0), Point(0,2), Point(1,1), Point(2,2), Point(1,2)
    };
    // Convex hull is triangle (0,0)-(2,0)-(0,2) but edge (2,0)-(0,2) has (1,1) interior? Yes, that edge has (1,1). Edge (0,2)-(0,0) has NO interior point (only (0,?) no other). So false.
    assert(isUniqueStableConvexHull(triangle) == false);

    // Case 6: A hexagon where each edge has exactly one interior point (classic stable hull)
    std::vector<Point> hexagon {
        Point(0,0), Point(1,0), Point(2,0), // bottom edge
        Point(2,1), Point(2,2), Point(1,2), Point(0,2), Point(0,1) // right, top, left edges
    };
    // Actually that's 8 points -> bottom has (1,0), right has (2,1), top has (1,2), left has (0,1). Should be stable.
    assert(isUniqueStableConvexHull(hexagon) == true);

    // Case 7: Duplicate points? Not allowed but we can test: a triangle with an extra duplicate of a vertex
    std::vector<Point> dup { Point(0,0), Point(0,0), Point(1,0), Point(0,1) };
    assert(isUniqueStableConvexHull(dup) == false); // no interior points on edges

    // Case 8: Small set of 6 points with interior points on every edge (but 6 is allowed if all edges have interiors)
    std::vector<Point> smallStable {
        Point(0,0), Point(1,0), Point(2,0), Point(2,1), Point(2,2), Point(0,2), Point(0,1), Point(1,1)
    };
    // Here we have 8 points, not exactly 6, but fine. Check stable.
    assert(isUniqueStableConvexHull(smallStable) == true);

    // Case 9: A trapezoid with interior point on one slanted edge but not the other
    std::vector<Point> trap {
        Point(0,0), Point(1,0), Point(2,0), // bottom edge has interior
        Point(3,2), Point(2,2), Point(1,2), Point(0,2), // top edge has interior (1,2)
        Point(1,1) // inside? Actually (1,1) is interior of shape, not on edge, so won't help
    };
    // Hull is trapezoid: (0,0)-(2,0)-(3,2)-(0,2). Edge (2,0)-(3,2) has no interior point (none provided). Left edge (0,2)-(0,0) has no interior either. So false.
    assert(isUniqueStableConvexHull(trap) == false);

    // Case 10: Empty set -> false
    std::vector<Point> empty;
    assert(isUniqueStableConvexHull(empty) == false);

    return 0;
}
