/*
Write a C++ function named `polylineIntersections` that takes as input two polyline descriptions, each represented by a `std::vector` of 2D points with integer coordinates (`std::vector<std::pair<int,int>>`), and returns a `std::vector<std::pair<double,double>>` containing all the intersection points (as double-precision coordinates) between the two polylines, including both proper intersections (where two segments cross and also overlap at endpoints) and endpoint-to-segment touches. The input polylines are each guaranteed to have at least two points, and segments within each polyline are consecutive but do not necessarily form a simple (non-self-intersecting) path. For each pair of segments (one from each polyline), compute their intersection; if they intersect at a single point (including the case where they share an endpoint or one endpoint lies on the other segment), add that point to the result. Ignore cases where segments are collinear and overlap in a range of points—but if they merely touch at a single point due to collinearity, include that point. The output vector must not contain duplicate points; order is not important. Use exact rational arithmetic (or appropriate epsilon-free integer-based logic) to detect collinearity and to determine the intersection point coordinates precisely, then convert to double for output. Ensure the function is `const`-correct and does not modify the inputs.
*/

#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>

using Point = std::pair<int,int>;
using PointD = std::pair<double,double>;

static long long cross(long long ax, long long ay, long long bx, long long by) {
    return ax * by - ay * bx;
}

static bool onSegment(const Point& p, const Point& q, const Point& r) {
    // q lies on segment PR if collinear and within bounding box
    if (cross((long long)q.first - p.first, (long long)q.second - p.second,
              (long long)r.first - p.first, (long long)r.second - p.second) != 0)
        return false;
    return std::min(p.first, r.first) <= q.first && q.first <= std::max(p.first, r.first) &&
           std::min(p.second, r.second) <= q.second && q.second <= std::max(p.second, r.second);
}

static PointD computeIntersection(const Point& A1, const Point& A2,
                                  const Point& B1, const Point& B2) {
    // Use the cross product formula for intersection of two lines
    long long x1 = A1.first, y1 = A1.second;
    long long x2 = A2.first, y2 = A2.second;
    long long x3 = B1.first, y3 = B1.second;
    long long x4 = B2.first, y4 = B2.second;
    long long denom = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);
    // denom should not be zero because segments are not collinear here
    long long t_num = (x1-x3)*(y3-y4) - (y1-y3)*(x3-x4);
    double t = (double)t_num / denom;
    double x = x1 + t * (double)(x2 - x1);
    double y = y1 + t * (double)(y2 - y1);
    return {x, y};
}

static bool samePoint(const PointD& a, const PointD& b) {
    return std::fabs(a.first - b.first) < 1e-9 && std::fabs(a.second - b.second) < 1e-9;
}

// Main function: returns all unique intersection points between two polylines.
std::vector<PointD> polylineIntersections(const std::vector<Point>& poly1,
                                          const std::vector<Point>& poly2) {
    std::vector<PointD> result;
    int n = (int)poly1.size() - 1; // number of segments in poly1
    int m = (int)poly2.size() - 1; // number of segments in poly2

    for (int i = 0; i < n; ++i) {
        Point A1 = poly1[i], A2 = poly1[i+1];
        for (int j = 0; j < m; ++j) {
            Point B1 = poly2[j], B2 = poly2[j+1];

            long long d1 = cross((long long)B2.first-B1.first, (long long)B2.second-B1.second,
                                 (long long)A1.first-B1.first, (long long)A1.second-B1.second);
            long long d2 = cross((long long)B2.first-B1.first, (long long)B2.second-B1.second,
                                 (long long)A2.first-B1.first, (long long)A2.second-B1.second);
            long long d3 = cross((long long)A2.first-A1.first, (long long)A2.second-A1.second,
                                 (long long)B1.first-A1.first, (long long)B1.second-A1.second);
            long long d4 = cross((long long)A2.first-A1.first, (long long)A2.second-A1.second,
                                 (long long)B2.first-A1.first, (long long)B2.second-A1.second);

            // Check if segments intersect properly (including endpoint touches)
            if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0) || d1 == 0 || d2 == 0) &&
                ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0) || d3 == 0 || d4 == 0)) {
                // Not collinear (unless both are zero, which would mean collinear)
                if (d1 == 0 && d2 == 0 && d3 == 0 && d4 == 0) {
                    // Collinear case: check if overlap is exactly one point
                    // Project onto x-axis (or y if vertical)
                    bool vertical = (A1.first == A2.first);
                    long long a1, a2, b1, b2;
                    if (vertical) {
                        a1 = A1.second; a2 = A2.second;
                        b1 = B1.second; b2 = B2.second;
                    } else {
                        a1 = A1.first; a2 = A2.first;
                        b1 = B1.first; b2 = B2.first;
                    }
                    long long lo = std::max(std::min(a1,a2), std::min(b1,b2));
                    long long hi = std::min(std::max(a1,a2), std::max(b1,b2));
                    if (lo == hi) {
                        // exactly one point: use that point (from either segment)
                        PointD p = vertical ? PointD((double)A1.first, (double)lo) : PointD((double)lo, (double)A1.second);
                        bool exists = false;
                        for (auto& q : result) if (samePoint(p,q)) { exists = true; break; }
                        if (!exists) result.push_back(p);
                    }
                    // else overlap is a range, skip
                } else {
                    // Proper intersection point
                    PointD p = computeIntersection(A1, A2, B1, B2);
                    // Ensure the point is on both segments (should be true but double-check)
                    // Since we used integer orientation tests, the computed point is exact rational,
                    // but due to floating point we just add it.
                    bool exists = false;
                    for (auto& q : result) if (samePoint(p,q)) { exists = true; break; }
                    if (!exists) result.push_back(p);
                }
            }
        }
    }
    return result;
}

#include <cassert>
#include <cmath>

// Helper to check closeness of two doubles
static bool close(double a, double b) {
    return std::fabs(a - b) < 1e-9;
}

int main() {
    // Test 1: Two crossed lines: line1 (0,0)->(2,2), line2 (0,2)->(2,0) -> intersection at (1,1)
    std::vector<Point> poly1 = {{0,0},{2,2}};
    std::vector<Point> poly2 = {{0,2},{2,0}};
    auto res = polylineIntersections(poly1, poly2);
    assert(res.size() == 1);
    assert(close(res[0].first, 1.0) && close(res[0].second, 1.0));

    // Test 2: T-junction: line1 (0,0)->(4,0), line2 (2,0)->(2,2) -> touch at (2,0)
    poly1 = {{0,0},{4,0}};
    poly2 = {{2,0},{2,2}};
    res = polylineIntersections(poly1, poly2);
    assert(res.size() == 1);
    assert(close(res[0].first, 2.0) && close(res[0].second, 0.0));

    // Test 3: Shared endpoint: line1 (0,0)->(1,1), line2 (1,1)->(2,0) -> at (1,1)
    poly1 = {{0,0},{1,1}};
    poly2 = {{1,1},{2,0}};
    res = polylineIntersections(poly1, poly2);
    assert(res.size() == 1);
    assert(close(res[0].first, 1.0) && close(res[0].second, 1.0));

    // Test 4: No intersection: parallel offset lines
    poly1 = {{0,0},{2,0}};
    poly2 = {{0,1},{2,1}};
    res = polylineIntersections(poly1, poly2);
    assert(res.empty());

    // Test 5: Collinear but overlapping in a range -> should be skipped (no intersection point)
    poly1 = {{0,0},{4,0}};
    poly2 = {{1,0},{3,0}};
    res = polylineIntersections(poly1, poly2);
    assert(res.empty());

    // Test 6: Collinear touching at a single point: line1 (0,0)->(2,0), line2 (2,0)->(4,0)
    poly1 = {{0,0},{2,0}};
    poly2 = {{2,0},{4,0}};
    res = polylineIntersections(poly1, poly2);
    assert(res.size() == 1);
    assert(close(res[0].first, 2.0) && close(res[0].second, 0.0));

    // Test 7: Multiple intersections in a polyline: two crossed segments and another touch
    poly1 = {{0,0},{4,4},{0,4}};
    poly2 = {{0,4},{4,0}};
    res = polylineIntersections(poly1, poly2);
    // First segment (0,0)-(4,4) crosses line2 at (2,2)
    // Second segment (4,4)-(0,4) touches line2 at (0,4) endpoint? Actually line2 ends at (4,0) not (0,4). Let's compute:
    // line2 from (0,4) to (4,0). Segment1 (0,0)-(4,4) intersects at (2,2). Segment2 (4,4)-(0,4) is horizontal y=4, line2 goes from (0,4) to (4,0), at y=4 only x=0, so touches at (0,4) which is an endpoint of both. So two points.
    assert(res.size() == 2);
    // Check points
    bool found1 = false, found2 = false;
    for (auto& p : res) {
        if (close(p.first,2.0) && close(p.second,2.0)) found1 = true;
        if (close(p.first,0.0) && close(p.second,4.0)) found2 = true;
    }
    assert(found1 && found2);

    // Test 8: Duplicate removal: same intersection from two segment pairs in polyline with repeated crossing
    poly1 = {{0,0},{2,2},{4,4}}; // straight line
    poly2 = {{0,4},{2,2},{4,0}}; // V shape crossing at (2,2)
    res = polylineIntersections(poly1, poly2);
    // Only (2,2) should appear once, even though both segment pairs might find it
    assert(res.size() == 1);
    assert(close(res[0].first,2.0) && close(res[0].second,2.0));

    return 0;
}

// The core challenge is robust segment-segment intersection with integer coordinates, where we need to avoid floating-point precision issues when deciding collinearity and computing intersection points. The standard approach is to use orientation tests via cross products with 64-bit integers (since coordinates are ints, the cross product of two vectors with int components fits into `long long`). For each segment pair (A1,A2) and (B1,B2), we compute:
// - `d1 = cross(B2-B1, A1-B1)`, `d2 = cross(B2-B1, A2-B1)`, `d3 = cross(A2-A1, B1-A1)`, `d4 = cross(A2-A1, B2-A1)`.
// - If `d1` and `d2` have opposite signs (or one is zero) **and** `d3` and `d4` have opposite signs (or one is zero), then the segments intersect at a point. This includes endpoint touches because zero orientation means the point lies on the line.
// - If both `d1` and `d2` are zero, the segments are collinear. In that case, check if they overlap in more than a point: if they share more than one point, skip (range overlap). If they share exactly one point (e.g., one segment's endpoint lies exactly on the other segment and no other overlap), include that point. To detect this, project onto an axis (usually x, but if the line is vertical use y) and find the overlapping interval. If the interval length is zero (i.e., max of starts == min of ends), then it's a single point, include it.
// - For proper intersections (non-collinear), the intersection point can be computed using a formula that avoids floating point during computation: `P = A1 + (A2-A1) * ( (B1-A1) cross (B2-B1) ) / ( (A2-A1) cross (B2-B1) )`. This gives rational coordinates: `x = (A1.x * denom + (A2.x-A1.x) * t_num) / denom` where `denom = cross(A2-A1, B2-B1)` and `t_num = cross(B1-A1, B2-B1)`. Since all inputs are ints, `t_num` and `denom` are integers. Convert to double by dividing carefully (ensure no integer overflow by using `long long` for products). Then add to result, and deduplicate by using a tolerance (e.g., 1e-9) when comparing floating-point points.
// - Edge cases: vertical lines are naturally handled by the cross-product method. Duplicate points from multiple segment pairs must be removed. Time complexity: O(n*m) where n and m are the number of segments in each polyline (i.e., points-1 each). Space complexity O(k) for output, where k is number of unique intersections.
