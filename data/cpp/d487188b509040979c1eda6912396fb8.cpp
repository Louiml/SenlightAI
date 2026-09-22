/*
Write a C++ function named `distancePointToGeometries` that takes a 2D point (with double coordinates), a polygon (represented as a vector of points forming the outer ring), and a line segment (represented as two points), and returns a struct `DistanceResult` containing three double values: distance from the point to the polygon, distance from the point to the line segment, and distance from the point to the "multi-point" consisting of the polygon's first vertex and the line segment's midpoint. The input polygon is guaranteed to be convex and given in counter-clockwise order; the point may be inside the polygon (distance 0) or outside. The function must not use any external geometry library; implement all geometric primitives manually. Distances should be computed with double precision, and the function should handle edge cases where the point matches a vertex or lies exactly on an edge.
*/
#include <vector>
#include <cmath>
#include <algorithm>

struct Point2D {
    double x;
    double y;
};

struct DistanceResult {
    double toPolygon;
    double toLineSegment;
    double toMultiPoint;
};

// Helper: Euclidean distance between two points
double pointDistance(const Point2D& a, const Point2D& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Helper: Distance from point p to segment defined by a and b
double distancePointToSegment(const Point2D& p, const Point2D& a, const Point2D& b) {
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    if (dx == 0.0 && dy == 0.0) {
        return pointDistance(p, a);
    }
    double t = ((p.x - a.x) * dx + (p.y - a.y) * dy) / (dx * dx + dy * dy);
    t = std::max(0.0, std::min(1.0, t));
    Point2D proj = {a.x + t * dx, a.y + t * dy};
    return pointDistance(p, proj);
}

// Helper: Cross product of vectors (a->b) and (a->c)
double crossProduct(const Point2D& a, const Point2D& b, const Point2D& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

// Main function: computes distances from point to polygon, line segment, and a multi-point
DistanceResult distancePointToGeometries(const Point2D& p,
                                         const std::vector<Point2D>& polygon,
                                         const Point2D& lineStart,
                                         const Point2D& lineEnd) {
    DistanceResult result;

    // 1. Distance to convex polygon (CCW order)
    size_t n = polygon.size();
    bool inside = true;
    for (size_t i = 0; i < n; ++i) {
        const Point2D& v1 = polygon[i];
        const Point2D& v2 = polygon[(i + 1) % n];
        if (crossProduct(v1, v2, p) < 0.0) {
            inside = false;
            break;
        }
    }
    if (inside) {
        result.toPolygon = 0.0;
    } else {
        double minDist = std::numeric_limits<double>::infinity();
        for (size_t i = 0; i < n; ++i) {
            const Point2D& v1 = polygon[i];
            const Point2D& v2 = polygon[(i + 1) % n];
            minDist = std::min(minDist, distancePointToSegment(p, v1, v2));
        }
        result.toPolygon = minDist;
    }

    // 2. Distance to line segment
    result.toLineSegment = distancePointToSegment(p, lineStart, lineEnd);

    // 3. Distance to multi-point (polygon's first vertex and line's midpoint)
    Point2D midpoint = { (lineStart.x + lineEnd.x) / 2.0,
                         (lineStart.y + lineEnd.y) / 2.0 };
    double d1 = pointDistance(p, polygon[0]);
    double d2 = pointDistance(p, midpoint);
    result.toMultiPoint = std::min(d1, d2);

    return result;
}
#include <cassert>
#include <cmath>
#include <vector>

// Include the solution code here (the structs and functions above)

int main() {
    // Simple square polygon (CCW)
    std::vector<Point2D> square = {{0,0}, {2,0}, {2,2}, {0,2}};
    Point2D lineA = {0,0};
    Point2D lineB = {4,0};

    // Test 1: point inside polygon
    Point2D p1 = {1,1};
    DistanceResult r1 = distancePointToGeometries(p1, square, lineA, lineB);
    assert(r1.toPolygon == 0.0);
    assert(std::abs(r1.toLineSegment - 1.0) < 1e-9); // vertical distance to segment y=0
    // Multi-point: min(distance to (0,0), distance to midpoint (2,0)) -> sqrt(2) vs 1 -> 1
    assert(std::abs(r1.toMultiPoint - 1.0) < 1e-9);

    // Test 2: point outside polygon, near right edge
    Point2D p2 = {3,1};
    DistanceResult r2 = distancePointToGeometries(p2, square, lineA, lineB);
    assert(std::abs(r2.toPolygon - 1.0) < 1e-9); // distance to right edge x=2
    assert(std::abs(r2.toLineSegment - 1.0) < 1e-9); // closest point on line is (3,0) or (4,0)? clamp to (4,0) gives sqrt(2), but (3,0) is outside segment; clamp to (4,0) => sqrt(2)~1.414? Actually line from (0,0) to (4,0), projection of (3,1) is t=0.75 -> point (3,0) is on segment, distance=1.0
    assert(std::abs(r2.toMultiPoint - 1.0) < 1e-9); // distance to midpoint (2,0) is sqrt(2)~1.414, to (0,0) is sqrt(10)~3.16, min is 1.414? Wait forgot: distance to (0,0) = sqrt(9+1)=3.162, to (2,0)=sqrt(1+1)=1.414, min=1.414, but line segment distance is 1.0? Actually toMultiPoint is only comparing to polygon[0] (0,0) and midpoint (2,0). So min(3.162, 1.414)=1.414, not 1.0. Let's correct assertion.
    assert(std::abs(r2.toMultiPoint - std::sqrt(2.0)) < 1e-9);

    // Test 3: point exactly on polygon vertex
    Point2D p3 = {0,0};
    DistanceResult r3 = distancePointToGeometries(p3, square, lineA, lineB);
    assert(r3.toPolygon == 0.0);
    assert(r3.toLineSegment == 0.0); // point is on line endpoint
    assert(r3.toMultiPoint == 0.0); // distance to polygon[0] is 0

    // Test 4: point outside, far from polygon, with non-trivial line
    Point2D p4 = { -1, 5 };
    Point2D lineC = {0, -10};
    Point2D lineD = {0, 10};
    DistanceResult r4 = distancePointToGeometries(p4, square, lineC, lineD);
    // polygon distance: closest edge is top edge (0,2)-(2,2) -> distance = 3.0
    assert(std::abs(r4.toPolygon - 3.0) < 1e-9);
    // line segment (vertical x=0): perpendicular distance = 1.0 (since x=-1, line x=0)
    assert(std::abs(r4.toLineSegment - 1.0) < 1e-9);
    // multi-point: polygon[0]=(0,0) distance sqrt(1+25)=~5.099, midpoint=(0,0) distance sqrt(1+25)=~5.099, min=~5.099
    assert(std::abs(r4.toMultiPoint - std::sqrt(26.0)) < 1e-9);

    // Test 5: point on polygon edge
    Point2D p5 = {1, 0};
    DistanceResult r5 = distancePointToGeometries(p5, square, lineA, lineB);
    assert(r5.toPolygon == 0.0);
    // line distance: from (1,0) to segment (0,0)-(4,0) is 0
    assert(r5.toLineSegment == 0.0);
    // multi-point: distance to (0,0)=1, to midpoint (2,0)=1 -> min=1
    assert(r5.toMultiPoint == 1.0);

    // Test 6: empty polygon? Not valid per spec, but we assume at least 3 vertices. Skip.

    // Test 7: degenerate line segment (same start and end)
    Point2D p6 = {3, 4};
    Point2D lineE = {1,1};
    Point2D lineF = {1,1};
    DistanceResult r6 = distancePointToGeometries(p6, square, lineE, lineF);
    // polygon distance: from (3,4) to square -> closest edge is top-right corner (2,2) distance sqrt(1+4)=sqrt(5)
    assert(std::abs(r6.toPolygon - std::sqrt(5.0)) < 1e-9);
    // line segment is a point (1,1): distance sqrt(4+9)=sqrt(13)
    assert(std::abs(r6.toLineSegment - std::sqrt(13.0)) < 1e-9);
    // multi-point: distance to polygon[0]=(0,0) sqrt(9+16)=5, to midpoint of degenerate (1,1) sqrt(4+9)=sqrt(13) -> min=sqrt(13)
    assert(std::abs(r6.toMultiPoint - std::sqrt(13.0)) < 1e-9);

    return 0;
}
// The solution requires implementing three distinct distance computations from scratch. For the polygon distance, since the polygon is convex and vertices are given in CCW order, we can compute the point-in-polygon test using the sign of cross products between each edge and the point. If the point is inside, distance is 0. Otherwise, the distance to the polygon is the minimum distance from the point to each edge segment. For each edge (p1, p2), we project the point onto the infinite line and clamp the projection parameter to [0,1] to get the closest point on the segment. The Euclidean distance between the point and that clamped projection gives the segment distance. The line segment distance uses the same projection formula but directly on the given two points. The multi-point distance is simply the Euclidean distance to the first polygon vertex and to the midpoint of the line segment, taking the minimum. Edge cases: if the point is exactly on an edge, the projection yields a distance of 0. For a point inside the convex polygon, the cross-product signs are all non-negative (or all non-positive depending on orientation); using CCW orientation, we ensure all signs are >= 0 for inside. Time complexity is O(V) for the polygon distance (where V is number of vertices), plus O(1) for line and multi-point. Space complexity is O(1) auxiliary.
