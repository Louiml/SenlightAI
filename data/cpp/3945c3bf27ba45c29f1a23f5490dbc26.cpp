// Write a C++ function `double distanceBetweenSegments(const Point& a1, const Point& a2, const Point& b1, const Point& b2)` that computes the Euclidean distance between two line segments in 3D space. A segment is defined by two endpoints. Input coordinates are real numbers (use `double`). The function must handle degenerate segments (where both endpoints coincide, i.e., a point) correctly. Return the minimum distance between any point on the first segment and any point on the second segment. The solution must be robust for arbitrary segment orientations, including parallel, intersecting, skew, and zero-length cases. The function should be self-contained and not rely on any global state. Provide a `Point` struct with `x`, `y`, `z` fields and appropriate constructors.
#include <cassert>
#include <cmath>

// Point definition is expected here (as in solution)
// Include the solution code above, then:

int main() {
    // Test 1: Intersecting segments
    {
        Point a1(0,0,0), a2(2,0,0);
        Point b1(1,-1,0), b2(1,1,0);
        double d = distanceBetweenSegments(a1, a2, b1, b2);
        assert(std::abs(d - 0.0) < 1e-9);
    }
    // Test 2: Parallel segments
    {
        Point a1(0,0,0), a2(1,0,0);
        Point b1(0,1,0), b2(1,1,0);
        double d = distanceBetweenSegments(a1, a2, b1, b2);
        assert(std::abs(d - 1.0) < 1e-9);
    }
    // Test 3: Skew segments
    {
        Point a1(0,0,0), a2(1,0,0);
        Point b1(0,1,1), b2(1,1,1);
        double d = distanceBetweenSegments(a1, a2, b1, b2);
        assert(std::abs(d - std::sqrt(2.0)) < 1e-9);
    }
    // Test 4: Degenerate segment (point) to segment
    {
        Point a1(0,0,0), a2(0,0,0); // point at origin
        Point b1(2,3,4), b2(2,3,10);
        double d = distanceBetweenSegments(a1, a2, b1, b2);
        // distance from (0,0,0) to segment along z from z=4 to z=10
        // closest point is (2,3,4), distance = sqrt(4+9+16)=sqrt(29)
        assert(std::abs(d - std::sqrt(29.0)) < 1e-9);
    }
    // Test 5: Both degenerate
    {
        Point a1(1,2,3), a2(1,2,3);
        Point b1(4,5,6), b2(4,5,6);
        double d = distanceBetweenSegments(a1, a2, b1, b2);
        assert(std::abs(d - std::sqrt(27.0)) < 1e-9);
    }
    // Test 6: Overlapping endpoints (same segment)
    {
        Point a1(0,0,0), a2(1,0,0);
        Point b1(0,0,0), b2(1,0,0);
        double d = distanceBetweenSegments(a1, a2, b1, b2);
        assert(std::abs(d - 0.0) < 1e-9);
    }
    // Test 7: Non-overlapping collinear segments
    {
        Point a1(0,0,0), a2(1,0,0);
        Point b1(2,0,0), b2(3,0,0);
        double d = distanceBetweenSegments(a1, a2, b1, b2);
        assert(std::abs(d - 1.0) < 1e-9);
    }
    // Test 8: Perpendicular segments touching at a point
    {
        Point a1(0,0,0), a2(1,0,0);
        Point b1(0,0,0), b2(0,1,0);
        double d = distanceBetweenSegments(a1, a2, b1, b2);
        assert(std::abs(d - 0.0) < 1e-9);
    }
    // Test 9: Random non-parallel, non-intersecting segments
    {
        Point a1(0,0,0), a2(1,0,0);
        Point b1(0,0,1), b2(1,0,1);
        // Closest points: p1 on first segment with y=0,z=0; p2 on second with y=0,z=1
        // distance = 1
        double d = distanceBetweenSegments(a1, a2, b1, b2);
        assert(std::abs(d - 1.0) < 1e-9);
    }
    // Test 10: Closest point parameter outside both segments
    {
        Point a1(0,0,0), a2(1,0,0);
        Point b1(3,1,1), b2(4,1,1);
        // Closest infinite-line point on first segment would be at x=3 (outside), so clamp
        // The closest is from a2=(1,0,0) to segment b: distance sqrt((2)^2+1+1)=sqrt(6)
        double d = distanceBetweenSegments(a1, a2, b1, b2);
        assert(std::abs(d - std::sqrt(6.0)) < 1e-9);
    }
    return 0;
}
#include <cmath>
#include <algorithm>

struct Point {
    double x, y, z;
    Point() : x(0), y(0), z(0) {}
    Point(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
};

// Helper: vector operations
static Point operator-(const Point& a, const Point& b) {
    return Point(a.x - b.x, a.y - b.y, a.z - b.z);
}
static Point operator+(const Point& a, const Point& b) {
    return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}
static Point operator*(double s, const Point& v) {
    return Point(s * v.x, s * v.y, s * v.z);
}
static double dot(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
static Point cross(const Point& a, const Point& b) {
    return Point(a.y * b.z - a.z * b.y,
                 a.z * b.x - a.x * b.z,
                 a.x * b.y - a.y * b.x);
}
static double norm(const Point& v) {
    return std::sqrt(dot(v, v));
}

// Distance from point P to segment AB
static double pointToSegment(const Point& p, const Point& a, const Point& b) {
    Point ab = b - a;
    double len2 = dot(ab, ab);
    if (len2 < 1e-30) return norm(p - a);  // degenerate segment
    double t = dot(p - a, ab) / len2;
    t = std::max(0.0, std::min(1.0, t));
    Point closest = a + t * ab;
    return norm(p - closest);
}

// Main function: distance between two 3D segments
double distanceBetweenSegments(const Point& a1, const Point& a2,
                               const Point& b1, const Point& b2) {
    const double EPS = 1e-12;
    Point d1 = a2 - a1;  // direction of segment 1
    Point d2 = b2 - b1;  // direction of segment 2
    double len1 = norm(d1);
    double len2 = norm(d2);

    // Handle degenerate segments
    if (len1 < 1e-30 && len2 < 1e-30) {
        return norm(a1 - b1);
    }
    if (len1 < 1e-30) {
        return pointToSegment(a1, b1, b2);
    }
    if (len2 < 1e-30) {
        return pointToSegment(b1, a1, a2);
    }

    // Check if segments are parallel
    Point crossDir = cross(d1, d2);
    double crossNorm = norm(crossDir);
    if (crossNorm < EPS) {
        // Parallel: use endpoint-to-other-segment distances and endpoint distances
        double ans = pointToSegment(a1, b1, b2);
        ans = std::min(ans, pointToSegment(a2, b1, b2));
        ans = std::min(ans, pointToSegment(b1, a1, a2));
        ans = std::min(ans, pointToSegment(b2, a1, a2));
        return ans;
    }

    // Non-parallel: find closest points on infinite lines
    Point r = b1 - a1;
    double t1 = dot(cross(r, d2), crossDir) / dot(crossDir, crossDir);
    double t2 = dot(cross(r, d1), crossDir) / dot(crossDir, crossDir);

    // Clamp parameters to segment ranges
    double t1_clamped = std::max(0.0, std::min(1.0, t1));
    double t2_clamped = std::max(0.0, std::min(1.0, t2));

    // If both closest points are inside segments, that's the answer
    if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
        Point p1 = a1 + t1 * d1;
        Point p2 = b1 + t2 * d2;
        return norm(p1 - p2);
    }

    // Otherwise, check all four endpoint-to-other-segment distances
    double ans = pointToSegment(a1, b1, b2);
    ans = std::min(ans, pointToSegment(a2, b1, b2));
    ans = std::min(ans, pointToSegment(b1, a1, a2));
    ans = std::min(ans, pointToSegment(b2, a1, a2));
    return ans;
}
// The distance between two segments can be computed by considering three cases:
//
// 1. **Degenerate segments**: If either segment has zero length, reduce to the distance from a point to a segment. For a point \(P\) and segment \(AB\), project \(P\) onto the infinite line through \(AB\). If the projection parameter \(t\) lies within \([0,1]\), the distance is the perpendicular distance; otherwise, it is the minimum of distances to endpoints \(A\) and \(B\).
//
// 2. **Parallel segments**: If the direction vectors are parallel (cross product is near zero), the distance is constant along the segments, but we still need to find the exact minimum. Since the segments are parallel, the shortest distance between them is the minimal distance between any point on one segment and the other segment. This can be found by sampling endpoints and checking if the perpendicular projection from any endpoint falls onto the other segment, plus checking endpoint-to-endpoint distances.
//
// 3. **Skew or intersecting segments (non-parallel)**: Compute the closest points on the infinite lines containing the segments using standard line-line closest point formulas (using the cross product and parameters). If both closest points fall within their respective segment parameter ranges \([0,1]\), then that distance is the answer. Otherwise, the minimum occurs at one of the four endpoint-to-other-segment distances. So compute distances from each endpoint of one segment to the other segment (using point-to-segment distance) and take the minimum.
//
// To avoid numerical issues, use a small epsilon (e.g., `1e-12`) for comparing cross-product norms and checking if parameters are within bounds (allowing a small tolerance). The algorithm runs in \(O(1)\) time and uses \(O(1)\) space. All operations are basic vector arithmetic.
