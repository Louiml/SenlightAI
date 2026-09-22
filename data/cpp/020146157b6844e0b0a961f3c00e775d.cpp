// Given a set of points in the 2D plane represented as pairs of doubles, write a C++ function `std::vector<std::pair<double,double>> convexHull(std::vector<std::pair<double,double>> pts)` that returns the vertices of the convex hull in counter-clockwise order. The hull must include all collinear boundary points (i.e., if multiple points lie on the same hull edge, include all of them). If there are fewer than 3 points, return all points (the hull is degenerate: either empty for empty input, one point, or two points). The output should have no duplicate consecutive points and should not include the same starting point at the end. Points with very close coordinates (within `1e-8`) should be treated as identical; if duplicate points exist, the output should contain each unique point at most once. The input points are not necessarily sorted and may contain duplicates. Use the provided geometric primitives (cross product, orientation) to implement the monotone chain algorithm (Andrew's algorithm) with a modification to include collinear points on hull edges.
The solution uses the monotone chain algorithm. First, sort the points lexicographically by x then y, and remove duplicate points (using the `EPS` tolerance to merge points within `1e-8`). Then build the lower hull by iterating through sorted points, and while the last turn is clockwise (or collinear, i.e., cross product <= -EPS), pop the middle point. For collinear points, we want to keep all of them along the hull boundary, so we pop only when the turn is strictly clockwise (cross < -EPS), not when collinear. However, to avoid including interior collinear points that are not extremal (e.g., in a straight line, the middle point is on the hull edge and should be kept, but if there are multiple points on the same segment, all are on the hull), the standard condition `while (cross <= 0)` would remove collinear points; we instead use `while (cross < 0)` to keep collinear points. For the upper hull, process the sorted points in reverse order with the same condition. After building both halves, concatenate them, removing the last duplicate point from each half. If the resulting hull has more than 2 points, ensure the first and last points are not duplicates. If all points are collinear, the lower and upper hulls will each contain all points in sorted order, and concatenating them will produce a duplicate of the endpoints; handle by removing the first element of the second half if it equals the last of the first. The algorithm runs in O(n log n) due to sorting and O(n) for hull construction. Space is O(n). Edge cases: empty input returns empty vector; one point returns that point; two distinct points return both; all collinear points returns them in sorted order along the line (from leftmost to rightmost) — the algorithm naturally produces this because the upper hull will be the reverse of the lower, and after concatenation we get the correct set. Duplicates are removed beforehand. Time complexity dominated by sorting.
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>

using pt = std::pair<double, double>;

const double EPS = 1e-8;

#define x first
#define y second

static double cross(const pt& a, const pt& b) {
    return a.x * b.y - a.y * b.x;
}

static pt operator-(const pt& a, const pt& b) {
    return pt(a.x - b.x, a.y - b.y);
}

static bool same_point(const pt& a, const pt& b) {
    return std::fabs(a.x - b.x) <= EPS && std::fabs(a.y - b.y) <= EPS;
}

// Build one half of the hull. If include_collinear is true, keep collinear boundary points.
static std::vector<pt> build_half(std::vector<pt> sorted, bool include_collinear) {
    std::vector<pt> hull;
    for (const pt& p : sorted) {
        // Pop while last turn is clockwise (or collinear if we don't want collinear).
        while (hull.size() >= 2) {
            pt a = hull[hull.size()-2];
            pt b = hull[hull.size()-1];
            double cr = cross(b - a, p - a);
            if (include_collinear ? cr < -EPS : cr <= EPS) {
                hull.pop_back();
            } else {
                break;
            }
        }
        hull.push_back(p);
    }
    return hull;
}

// Compute convex hull with all collinear boundary points included.
std::vector<pt> convexHull(std::vector<pt> pts) {
    if (pts.empty()) return {};

    // Sort and remove duplicates (using EPS tolerance).
    std::sort(pts.begin(), pts.end());
    std::vector<pt> unique_pts;
    for (const pt& p : pts) {
        if (unique_pts.empty() || !same_point(unique_pts.back(), p)) {
            unique_pts.push_back(p);
        }
    }
    if (unique_pts.size() <= 2) return unique_pts;

    // Build lower and upper hulls, including collinear points.
    std::vector<pt> lower = build_half(unique_pts, true);
    std::vector<pt> upper_sorted = unique_pts;
    std::reverse(upper_sorted.begin(), upper_sorted.end());
    std::vector<pt> upper = build_half(upper_sorted, true);

    // Concatenate: remove last point of each half to avoid duplicates.
    lower.pop_back();
    upper.pop_back();
    std::vector<pt> result = lower;
    result.insert(result.end(), upper.begin(), upper.end());

    // Remove any consecutive duplicates (due to tolerance).
    std::vector<pt> final_hull;
    for (const pt& p : result) {
        if (final_hull.empty() || !same_point(final_hull.back(), p)) {
            final_hull.push_back(p);
        }
    }
    // Check if first and last are duplicate (e.g., all collinear case).
    if (final_hull.size() > 1 && same_point(final_hull.front(), final_hull.back())) {
        final_hull.pop_back();
    }
    return final_hull;
}
#include <cassert>
#include <vector>
#include <utility>
#include <cmath>

// The solution function is assumed to be defined above.
// Test code below.

bool contains_point(const std::vector<pt>& hull, const pt& p) {
    for (const pt& h : hull) {
        if (std::fabs(h.first - p.first) <= 1e-8 && std::fabs(h.second - p.second) <= 1e-8) return true;
    }
    return false;
}

int main() {
    // Empty input
    assert(convexHull({}).empty());

    // Single point
    auto one = convexHull({{0.0, 0.0}});
    assert(one.size() == 1 && contains_point(one, {0.0, 0.0}));

    // Two points
    auto two = convexHull({{1.0, 2.0}, {3.0, 4.0}});
    assert(two.size() == 2);
    assert(contains_point(two, {1.0, 2.0}) && contains_point(two, {3.0, 4.0}));

    // Square (including collinear edges not possible here; standard hull)
    std::vector<pt> square = {{0,0}, {1,0}, {1,1}, {0,1}};
    auto hull_sq = convexHull(square);
    assert(hull_sq.size() == 4);
    assert(contains_point(hull_sq, {0,0}) && contains_point(hull_sq, {1,0}) &&
           contains_point(hull_sq, {1,1}) && contains_point(hull_sq, {0,1}));

    // Collinear points: all on a line, should return all in order
    std::vector<pt> line = {{0,0}, {1,1}, {2,2}, {3,3}};
    auto hull_line = convexHull(line);
    assert(hull_line.size() == 4);
    assert(contains_point(hull_line, {0,0}) && contains_point(hull_line, {1,1}) &&
           contains_point(hull_line, {2,2}) && contains_point(hull_line, {3,3}));

    // Collinear points on one edge of a triangle (must include all on that edge)
    std::vector<pt> tri = {{0,0}, {2,0}, {1,0}, {1,1}}; // Edge from (0,0)->(2,0) has (1,0) in middle
    auto hull_tri = convexHull(tri);
    assert(hull_tri.size() == 4); // (0,0), (2,0), (1,1), and also (1,0)? Actually (1,0) is on the edge between (0,0) and (2,0), so should be included.
    // Since collinear points on hull edge are kept, we expect 4 points total.
    assert(contains_point(hull_tri, {0,0}) && contains_point(hull_tri, {2,0}) &&
           contains_point(hull_tri, {1,1}) && contains_point(hull_tri, {1,0}));

    // Duplicate points
    std::vector<pt> dup = {{0,0}, {0,0}, {1,1}, {2,2}, {2,2}};
    auto hull_dup = convexHull(dup);
    assert(hull_dup.size() == 3); // (0,0), (1,1), (2,2) (collinear, all kept)

    // Points inside a triangle
    std::vector<pt> inside = {{0,0}, {4,0}, {2,3}, {2,1}, {1,1}, {3,1}};
    auto hull_in = convexHull(inside);
    assert(hull_in.size() == 3); // only triangle vertices remain
    assert(contains_point(hull_in, {0,0}) && contains_point(hull_in, {4,0}) &&
           contains_point(hull_in, {2,3}));

    // All collinear with negative slope
    std::vector<pt> neg_line = {{3,3}, {2,2}, {1,1}};
    auto hull_neg = convexHull(neg_line);
    assert(hull_neg.size() == 3);
    assert(contains_point(hull_neg, {1,1}) && contains_point(hull_neg, {2,2}) &&
           contains_point(hull_neg, {3,3}));

    return 0;
}
