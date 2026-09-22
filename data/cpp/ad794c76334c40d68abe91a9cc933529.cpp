Write a C++ function `bool segmentsIntersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)` that determines whether two line segments intersect. The first segment runs from point `(x1, y1)` to `(x2, y2)`, and the second from `(x3, y3)` to `(x4, y4)`. The function should return `true` if the segments share any point (including endpoints, touching, or overlapping collinear segments) and `false` otherwise. All coordinates are given as `double` values. The function must handle all cases: proper intersection (crossing), endpoint touching, collinear overlapping, and collinear non-overlapping segments. Use exact arithmetic where feasible by comparing orientations with a tolerance for floating-point precision, or implement a robust orientation test that works with doubles directly.

// The standard approach for segment intersection uses orientation tests. For three points `(a, b, c)`, the orientation is the sign of the cross product `(b - a) × (c - a)`. This tells whether `c` lies clockwise, counterclockwise, or collinear with respect to the directed line from `a` to `b`. Two segments intersect if and only if:
// 1. The endpoints of each segment lie on opposite sides of the other segment (general position case), OR
// 2. At least one endpoint of one segment lies on the other segment (including collinear overlap).
//
// More precisely, let `o1 = orient(p1, p2, p3)`, `o2 = orient(p1, p2, p4)`, `o3 = orient(p3, p4, p1)`, `o4 = orient(p3, p4, p2)`. The segments intersect if:
// - `o1` and `o2` have opposite signs AND `o3` and `o4` have opposite signs (proper crossing), or
// - Any of the orientations is zero and the corresponding point lies within the bounding box of the segment it is being tested against (endpoint or collinear touch).
//
// For collinear segments (all four orientations zero), we check whether their projections on either axis overlap. Because the segments are collinear, x-axis or y-axis overlap is sufficient. Using an exact orientation function that works with `double` values can be subject to floating-point error, so a small epsilon (e.g., 1e-12) is often used in practice. However, for exact arithmetic we can use a cross product comparison with an epsilon relative to the magnitudes. A simpler and robust approach for doubles is to compute orientations directly and compare against an epsilon derived from `std::numeric_limits<double>::epsilon()` scaled by the coordinates, but since the task emphasizes exactness, we use a tolerance-based orientation that works well. The algorithm runs in O(1) time and O(1) space, performing constant arithmetic operations.
//
// Edge cases include: segments sharing only one endpoint, segments that are collinear and disjoint, collinear segments that overlap partially, and zero-length segments (point segments). Zero-length segments: if one segment is a point, it intersects if that point lies on the other segment. The implementation handles this naturally via the endpoint-on-segment check.

#include <cmath>
#include <algorithm>

// Helper: orientation of point c relative to directed line a->b.
// Returns >0 for counterclockwise, <0 for clockwise, ~0 for collinear (with tolerance).
double orientation(double ax, double ay, double bx, double by, double cx, double cy) {
    double cross = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
    return cross;
}

// Helper: check if point p lies on segment given by endpoints a and b.
bool onSegment(double ax, double ay, double bx, double by, double px, double py) {
    double eps = 1e-12;
    // Check collinearity
    if (std::fabs(orientation(ax, ay, bx, by, px, py)) > eps) return false;
    // Check bounding box
    if (px < std::min(ax, bx) - eps || px > std::max(ax, bx) + eps) return false;
    if (py < std::min(ay, by) - eps || py > std::max(ay, by) + eps) return false;
    return true;
}

// Return true if the two line segments intersect (including endpoints and collinear overlap).
bool segmentsIntersect(double x1, double y1, double x2, double y2,
                       double x3, double y3, double x4, double y4) {
    double eps = 1e-12;

    // Orientations
    double o1 = orientation(x1, y1, x2, y2, x3, y3);
    double o2 = orientation(x1, y1, x2, y2, x4, y4);
    double o3 = orientation(x3, y3, x4, y4, x1, y1);
    double o4 = orientation(x3, y3, x4, y4, x2, y2);

    bool o1_zero = std::fabs(o1) <= eps;
    bool o2_zero = std::fabs(o2) <= eps;
    bool o3_zero = std::fabs(o3) <= eps;
    bool o4_zero = std::fabs(o4) <= eps;

    // Proper intersection: opposite signs for both pairs
    if (o1 * o2 < 0 && o3 * o4 < 0) return true;

    // Endpoint or collinear overlap: any endpoint on the other segment
    if (o1_zero && onSegment(x1, y1, x2, y2, x3, y3)) return true;
    if (o2_zero && onSegment(x1, y1, x2, y2, x4, y4)) return true;
    if (o3_zero && onSegment(x3, y3, x4, y4, x1, y1)) return true;
    if (o4_zero && onSegment(x3, y3, x4, y4, x2, y2)) return true;

    return false;
}

#include <cassert>
#include <cmath>

// Function declaration (matching the solution)
bool segmentsIntersect(double x1, double y1, double x2, double y2,
                       double x3, double y3, double x4, double y4);

int main() {
    // Proper crossing
    assert(segmentsIntersect(0, 0, 2, 2, 0, 2, 2, 0) == true);
    // Non-intersecting
    assert(segmentsIntersect(0, 0, 1, 1, 2, 2, 3, 3) == false);
    // Sharing endpoint
    assert(segmentsIntersect(0, 0, 1, 1, 1, 1, 2, 0) == true);
    // Collinear overlapping
    assert(segmentsIntersect(0, 0, 2, 2, 1, 1, 3, 3) == true);
    // Collinear disjoint
    assert(segmentsIntersect(0, 0, 1, 1, 2, 2, 3, 3) == false);
    // Point segment on line
    assert(segmentsIntersect(1, 1, 1, 1, 0, 0, 2, 2) == true);
    // Point segment off line
    assert(segmentsIntersect(1, 1, 1, 1, 0, 0, 1, 2) == false);
    // Vertical and horizontal crossing
    assert(segmentsIntersect(0, 0, 0, 2, -1, 1, 1, 1) == true);
    // Parallel non-intersecting
    assert(segmentsIntersect(0, 0, 1, 1, 0, 1, 1, 2) == false);
    // Touching at a single endpoint (T-junction)
    assert(segmentsIntersect(0, 0, 2, 0, 2, 0, 3, 1) == true);
    return 0;
}
