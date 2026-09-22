/*
Write a C++ function that computes the approximate arc length of a cubic Bézier curve defined by four 2D points (start, control1, control2, end), using recursive subdivision until the difference between the polyline approximation length and the straight-line distance between endpoints falls below a given tolerance. The function should take the four points and a float tolerance as parameters, and return the approximate length as a float. The recursive subdivision should split the curve into two halves using the De Casteljau algorithm, and the base case should use the sum of the three straight-line segments (start→control1, control1→control2, control2→end) as the approximation. Edge cases include very short curves, zero-length segments, and the symmetry of the subdivision (the function should produce the same result regardless of curve direction).
*/

#include <cmath>

struct Point2D {
    float x, y;
};

// Helper: Euclidean distance between two points
static float distance(const Point2D& a, const Point2D& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Approximate arc length of a cubic Bézier curve by recursive subdivision
float cubicBezierLength(const Point2D& p0, const Point2D& p1, const Point2D& p2, const Point2D& p3, float tolerance) {
    float polyline = distance(p0, p1) + distance(p1, p2) + distance(p2, p3);
    float chord = distance(p0, p3);
    
    if (std::fabs(polyline - chord) <= tolerance) {
        return polyline;
    }
    
    // De Casteljau subdivision
    Point2D mid01 = { (p0.x + p1.x) / 2.0f, (p0.y + p1.y) / 2.0f };
    Point2D mid12 = { (p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f };
    Point2D mid23 = { (p2.x + p3.x) / 2.0f, (p2.y + p3.y) / 2.0f };
    Point2D mid012 = { (mid01.x + mid12.x) / 2.0f, (mid01.y + mid12.y) / 2.0f };
    Point2D mid123 = { (mid12.x + mid23.x) / 2.0f, (mid12.y + mid23.y) / 2.0f };
    Point2D mid0123 = { (mid012.x + mid123.x) / 2.0f, (mid012.y + mid123.y) / 2.0f };
    
    return cubicBezierLength(p0, mid01, mid012, mid0123, tolerance) +
           cubicBezierLength(mid0123, mid123, mid23, p3, tolerance);
}

#include <cassert>
#include <cmath>

// The solution function and Point2D must be defined above this test.

int main() {
    // Straight line: length is exactly the distance
    Point2D p0 = {0.0f, 0.0f};
    Point2D p1 = {1.0f, 1.0f};
    Point2D p2 = {2.0f, 2.0f};
    Point2D p3 = {3.0f, 3.0f};
    float straightLen = cubicBezierLength(p0, p1, p2, p3, 1e-4f);
    assert(std::fabs(straightLen - 3.0f * std::sqrt(2.0f)) < 1e-3f);

    // Zero-length curve (all points identical)
    float zeroLen = cubicBezierLength(p0, p0, p0, p0, 1e-4f);
    assert(zeroLen == 0.0f);

    // Symmetric curve: reversing the control points gives the same length
    float forward = cubicBezierLength(p0, p1, p2, p3, 1e-4f);
    float backward = cubicBezierLength(p3, p2, p1, p0, 1e-4f);
    assert(std::fabs(forward - backward) < 1e-4f);

    // Simple half-circle approximation (control points far from line)
    Point2D q0 = {0.0f, 0.0f};
    Point2D q1 = {0.0f, 2.0f};
    Point2D q2 = {2.0f, 2.0f};
    Point2D q3 = {2.0f, 0.0f};
    float approxLen = cubicBezierLength(q0, q1, q2, q3, 1e-3f);
    // The true arc length is > chord length (2.0) but < polyline length (6.0)
    assert(approxLen > 2.0f && approxLen < 6.0f);

    // Coincident control points degenerate to a line segment
    Point2D r0 = {0.0f, 0.0f};
    Point2D r1 = {0.0f, 0.0f};
    Point2D r2 = {1.0f, 0.0f};
    Point2D r3 = {1.0f, 0.0f};
    float degenerateLen = cubicBezierLength(r0, r1, r2, r3, 1e-4f);
    assert(std::fabs(degenerateLen - 1.0f) < 1e-3f);

    // Tolerance affects subdivision: tight tolerance gives more accurate result
    float loose = cubicBezierLength(q0, q1, q2, q3, 0.5f);
    float tight = cubicBezierLength(q0, q1, q2, q3, 1e-5f);
    assert(tight >= loose);
    assert(tight - loose < 0.5f);

    return 0;
}

// The solution uses a recursive or iterative subdivision approach based on the De Casteljau algorithm. For a cubic Bézier with control points P0, P1, P2, P3, the approximate length is the sum of distances |P0P1| + |P1P2| + |P2P3|. The stopping condition compares this approximate length to the straight-line distance |P0P3|; if their difference is greater than a tolerance, the curve is split into two halves. The split points are computed as follows:  
// - mid01 = (P0 + P1) / 2, mid12 = (P1 + P2) / 2, mid23 = (P2 + P3) / 2  
// - mid012 = (mid01 + mid12) / 2, mid123 = (mid12 + mid23) / 2  
// - mid0123 = (mid012 + mid123) / 2  
// The left curve is (P0, mid01, mid012, mid0123), and the right curve is (mid0123, mid123, mid23, P3). This process repeats recursively until the difference between the approximate length and the chord distance is within tolerance. The total length is the sum of all base-case lengths.  
// Edge cases: if all points are collinear or coincident, the difference is zero and the function returns the approximate length immediately. For very small curves, the tolerance check works correctly because the difference is small. The algorithm is symmetric — reversing the control points yields the same result because the same subdivision structure is used. Time complexity is O(n) where n is the number of subdivisions, which is bounded by the tolerance; in practice it’s O(k) where k is the recursion depth, typically logarithmic in the ratio of the curve’s bounding box diagonal to the tolerance. Space complexity is O(k) due to recursion stack.
