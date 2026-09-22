// Write a C++ function that takes two line segments in a 2D plane, each defined by two endpoints with double-precision coordinates, and returns a vector of intersection points between them. The function should handle all valid segment configurations: intersecting at a single point, overlapping collinearly (return both endpoints of the overlapping portion), touching at a common endpoint (return that point), or having no intersection (return an empty vector). For numerical robustness, treat points as equal if their coordinates differ by at most 1e-9, and treat near-zero cross products (absolute value ≤ 1e-9) as zero for collinearity and parallel detection tests. Include a concise comment block describing the function's behavior and assumptions.
#include <cassert>
#include <cmath>

bool pointClose(const Point& a, const Point& b) {
    return std::fabs(a.x - b.x) <= 1e-9 && std::fabs(a.y - b.y) <= 1e-9;
}

int main() {
    // Simple crossing
    Segment s1(Point(1,1), Point(2,2));
    Segment s2(Point(2,1), Point(1,2));
    auto res = segmentIntersections(s1, s2);
    assert(res.size() == 1);
    assert(pointClose(res[0], Point(1.5, 1.5)));

    // No intersection
    Segment s3(Point(0,0), Point(1,0));
    Segment s4(Point(0,1), Point(1,1));
    assert(segmentIntersections(s3, s4).empty());

    // Touching at a common endpoint
    Segment s5(Point(0,0), Point(1,0));
    Segment s6(Point(1,0), Point(1,1));
    res = segmentIntersections(s5, s6);
    assert(res.size() == 1);
    assert(pointClose(res[0], Point(1,0)));

    // Overlapping collinear segments
    Segment s7(Point(0,0), Point(2,0));
    Segment s8(Point(1,0), Point(3,0));
    res = segmentIntersections(s7, s8);
    assert(res.size() == 2);
    assert(pointClose(res[0], Point(1,0)) && pointClose(res[1], Point(2,0)));

    // One segment completely inside another
    Segment s9(Point(0,0), Point(3,0));
    Segment s10(Point(1,0), Point(2,0));
    res = segmentIntersections(s9, s10);
    assert(res.size() == 2);
    assert(pointClose(res[0], Point(1,0)) && pointClose(res[1], Point(2,0)));

    // Collinear but disjoint
    Segment s11(Point(0,0), Point(1,0));
    Segment s12(Point(2,0), Point(3,0));
    assert(segmentIntersections(s11, s12).empty());

    // Vertical and horizontal crossing
    Segment s13(Point(0,0), Point(0,2));
    Segment s14(Point(-1,1), Point(1,1));
    res = segmentIntersections(s13, s14);
    assert(res.size() == 1);
    assert(pointClose(res[0], Point(0,1)));

    // Touching at one endpoint that lies on the other segment (not at its endpoint)
    Segment s15(Point(0,0), Point(2,0));
    Segment s16(Point(1,0), Point(1,1));
    res = segmentIntersections(s15, s16);
    assert(res.size() == 1);
    assert(pointClose(res[0], Point(1,0)));

    // Parallel but not collinear
    Segment s17(Point(0,0), Point(1,0));
    Segment s18(Point(0,1), Point(1,1));
    assert(segmentIntersections(s17, s18).empty());

    return 0;
}
#include <vector>
#include <cmath>
#include <algorithm>

struct Point {
    double x, y;
    Point(double x_ = 0.0, double y_ = 0.0) : x(x_), y(y_) {}
};

struct Segment {
    Point a, b;
    Segment(const Point& a_, const Point& b_) : a(a_), b(b_) {}
};

static const double EPS = 1e-9;

static bool isZero(double v) {
    return std::fabs(v) <= EPS;
}

static bool pointsEqual(const Point& p, const Point& q) {
    return isZero(p.x - q.x) && isZero(p.y - q.y);
}

static double cross(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static bool onSegment(const Point& p, const Segment& seg) {
    return isZero(cross(seg.a, seg.b, p)) &&
           p.x >= std::min(seg.a.x, seg.b.x) - EPS && p.x <= std::max(seg.a.x, seg.b.x) + EPS &&
           p.y >= std::min(seg.a.y, seg.b.y) - EPS && p.y <= std::max(seg.a.y, seg.b.y) + EPS;
}

static Point intersectionPoint(const Segment& s1, const Segment& s2) {
    double t = cross(s2.a, s2.b, s1.a) / cross(s1.a, s1.b, s2.b - s2.a);
    // Actually compute using cross products correctly:
    // Use s1.a, s1.b as A,B; s2.a, s2.b as C,D.
    // Formula: t = cross(C-A, D-C) / cross(B-A, D-C)
    double dx1 = s1.b.x - s1.a.x;
    double dy1 = s1.b.y - s1.a.y;
    double dx2 = s2.b.x - s2.a.x;
    double dy2 = s2.b.y - s2.a.y;
    double denom = dx1 * dy2 - dy1 * dx2;
    double cx = s2.a.x - s1.a.x;
    double cy = s2.a.y - s1.a.y;
    double t_val = (cx * dy2 - cy * dx2) / denom;
    return Point(s1.a.x + t_val * dx1, s1.a.y + t_val * dy1);
}

// Computes all intersection points between two segments.
// Returns a vector of distinct points (with tolerance) where the segments intersect.
// Handles single-point intersections, overlapping collinear segments (returns both endpoints of overlap),
// and no intersections (empty vector).
std::vector<Point> segmentIntersections(const Segment& s1, const Segment& s2) {
    std::vector<Point> result;

    double d1 = cross(s2.a, s2.b, s1.a);
    double d2 = cross(s2.a, s2.b, s1.b);
    double d3 = cross(s1.a, s1.b, s2.a);
    double d4 = cross(s1.a, s1.b, s2.b);

    // Check if strictly non-intersecting (both pairs on opposite sides)
    if ((d1 > EPS && d2 > EPS) || (d1 < -EPS && d2 < -EPS) ||
        (d3 > EPS && d4 > EPS) || (d3 < -EPS && d4 < -EPS)) {
        // No intersection, but check for coincident endpoints separately
        // (already handled by robust check below)
    }

    // Check for coincident endpoints: any endpoint on the other segment
    if (onSegment(s1.a, s2)) result.push_back(s1.a);
    if (onSegment(s1.b, s2)) result.push_back(s1.b);
    if (onSegment(s2.a, s1)) result.push_back(s2.a);
    if (onSegment(s2.b, s1)) result.push_back(s2.b);

    if (!result.empty()) {
        // Deduplicate points that are equal within tolerance
        std::vector<Point> unique;
        for (const auto& p : result) {
            bool found = false;
            for (const auto& u : unique) {
                if (pointsEqual(p, u)) { found = true; break; }
            }
            if (!found) unique.push_back(p);
        }
        // If we already have a complete overlap (two distinct points), return them
        if (unique.size() >= 2) return unique;
        // Otherwise, may need to compute proper intersection or overlap
    }

    double denom = cross(s1.a, s1.b, Point(s2.b.x - s2.a.x, s2.b.y - s2.a.y));
    // Actually, the above cross is wrong; we need cross of direction vectors.
    // Correct: denom = cross(B-A, D-C)
    double dirCross = (s1.b.x - s1.a.x) * (s2.b.y - s2.a.y) - (s1.b.y - s1.a.y) * (s2.b.x - s2.a.x);

    if (isZero(dirCross)) {
        // Parallel: either collinear or disjoint
        if (isZero(d1) && isZero(d2)) {
            // Collinear: compute overlap interval along x-axis (or y if vertical)
            bool useX = std::fabs(s1.b.x - s1.a.x) > EPS;
            double min1, max1, min2, max2;
            if (useX) {
                min1 = std::min(s1.a.x, s1.b.x);
                max1 = std::max(s1.a.x, s1.b.x);
                min2 = std::min(s2.a.x, s2.b.x);
                max2 = std::max(s2.a.x, s2.b.x);
            } else {
                min1 = std::min(s1.a.y, s1.b.y);
                max1 = std::max(s1.a.y, s1.b.y);
                min2 = std::min(s2.a.y, s2.b.y);
                max2 = std::max(s2.a.y, s2.b.y);
            }
            double overlapMin = std::max(min1, min2);
            double overlapMax = std::min(max1, max2);
            if (overlapMin > overlapMax + EPS) return {}; // no overlap
            if (isZero(overlapMin - overlapMax)) {
                // Single point of overlap (touch)
                Point p;
                if (useX) {
                    p.y = s1.a.y + (overlapMin - s1.a.x) * (s1.b.y - s1.a.y) / (s1.b.x - s1.a.x);
                    p.x = overlapMin;
                } else {
                    p.x = s1.a.x + (overlapMin - s1.a.y) * (s1.b.x - s1.a.x) / (s1.b.y - s1.a.y);
                    p.y = overlapMin;
                }
                return {p};
            }
            // Two overlap endpoints
            Point p1, p2;
            if (useX) {
                p1.y = s1.a.y + (overlapMin - s1.a.x) * (s1.b.y - s1.a.y) / (s1.b.x - s1.a.x);
                p1.x = overlapMin;
                p2.y = s1.a.y + (overlapMax - s1.a.x) * (s1.b.y - s1.a.y) / (s1.b.x - s1.a.x);
                p2.x = overlapMax;
            } else {
                p1.x = s1.a.x + (overlapMin - s1.a.y) * (s1.b.x - s1.a.x) / (s1.b.y - s1.a.y);
                p1.y = overlapMin;
                p2.x = s1.a.x + (overlapMax - s1.a.y) * (s1.b.x - s1.a.x) / (s1.b.y - s1.a.y);
                p2.y = overlapMax;
            }
            return {p1, p2};
        }
        // Parallel but not collinear -> no intersection
        return {};
    }

    // Non-parallel: compute intersection point
    Point p = intersectionPoint(s1, s2);
    // Verify the point lies on both segments (should be true, but robust check)
    if (onSegment(p, s1) && onSegment(p, s2)) {
        // Avoid duplicates if already present
        bool found = false;
        for (const auto& pt : result) {
            if (pointsEqual(pt, p)) { found = true; break; }
        }
        if (!found) result.push_back(p);
    } else {
        // If the computed point is not on segments, no intersection
        // But this may happen due to numerical issues; fallback to no intersection
        return {};
    }

    // Deduplicate final result
    std::vector<Point> unique;
    for (const auto& p : result) {
        bool found = false;
        for (const auto& u : unique) {
            if (pointsEqual(p, u)) { found = true; break; }
        }
        if (!found) unique.push_back(p);
    }
    return unique;
}
// The solution uses geometric primitives: orientation tests and point-on-segment checks. Represent each segment by its two endpoints (P1, P2) and (Q1, Q2). Compute the cross product `d1 = cross(B-A, P1-A)` and `d2 = cross(B-A, P2-A)` for checking relative positions of the second segment's endpoints to the first segment line, and similarly `d3 = cross(D-C, A-C)` and `d4 = cross(D-C, B-C)` for the reverse. If both `d1*d2 > 0` and `d3*d4 > 0`, the segments are strictly separated and no intersection exists. If the signs indicate that one endpoint of each segment lies on opposite sides of the other's line (or one is zero), compute the intersection using the line intersection formula: for segments A-B and C-D, the intersection point is `A + t*(B-A)` with `t = cross(C-A, D-C) / cross(B-A, D-C)` (guard against division by zero). Special cases: if the cross product of the direction vectors is near zero, the segments are parallel; if they are also collinear (both `d1` and `d2` near zero), compute the overlapping interval along a chosen axis (e.g., x or y depending on which direction is more horizontal), collect the overlap's endpoints if the overlap is non-empty, and return them (possibly one point if they touch). If collinear but no overlap, return empty. If not collinear and parallel, return empty. To handle endpoint coincidences correctly, check if any endpoint of one segment lies on the other segment (using cross product near zero and within bounding box with epsilon) and include that point only once. Sorting and deduplication of result points by coordinates with epsilon tolerance avoids duplicate entries. Time complexity: O(1) since only constant operations are performed. Space complexity: O(1) additional space besides the output vector.
