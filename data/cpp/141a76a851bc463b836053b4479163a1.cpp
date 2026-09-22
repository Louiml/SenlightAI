// Write a C++ function `bool isInsideHull(const std::vector<Point>& points, const Point& query)` that takes a vector of 2D integer points and a query point, and returns `true` if the query point lies strictly inside the convex hull of the given points (not on the boundary), and `false` otherwise. The input points are guaranteed to be in general position (no three collinear) and there are at least 3 points. The function must handle negative coordinates. The convex hull must be computed using the gift-wrapping (Jarvis march) algorithm. The function should not modify the input vector and should not use any global state.
The approach begins by copying the input points and shifting all coordinates so that the minimum x and y values become zero, which simplifies handling negative coordinates during arithmetic. Then, the convex hull is computed using the Jarvis march: start with the leftmost point; repeatedly select the next hull vertex by iterating through all points and choosing the one that makes the smallest clockwise turn (or the most counterclockwise) relative to the current edge, using the cross product (direction test). This process continues until the starting point is reached again. After obtaining the hull vertices in counterclockwise order, the point-in-convex-polygon test is performed: since the hull is convex, check that the query point is strictly to the left of every directed edge (from vertex `i` to `i+1`). If it is strictly left for all edges, the point is strictly inside; if it lies on any edge (cross product equals zero), it is on the boundary and should return `false`. Edge cases: if the hull has fewer than 3 points (degenerate), return `false`; if the query point coincides with a vertex, the cross product with adjacent edges may be zero, and the point is on the boundary, so return `false`. Time complexity is O(n * h) for hull construction, where n is the number of points and h is the number of hull vertices, and O(h) for the point-in-polygon test, so overall O(n*h). Space complexity is O(n) for the copy and hull storage.
#include <vector>
#include <algorithm>
#include <stdexcept>

struct Point {
    int x;
    int y;
};

// Cross product of vectors (q-p) and (r-p). Positive if r is to the left of directed line p->q.
static int cross(const Point& p, const Point& q, const Point& r) {
    return (q.x - p.x) * (r.y - p.y) - (q.y - p.y) * (r.x - p.x);
}

// Checks if query is strictly inside the convex hull of points.
bool isInsideHull(const std::vector<Point>& points, const Point& query) {
    const int n = static_cast<int>(points.size());
    if (n < 3) return false;

    // Shift coordinates to non-negative to handle negative inputs safely.
    int minX = points[0].x, minY = points[0].y;
    for (const auto& pt : points) {
        minX = std::min(minX, pt.x);
        minY = std::min(minY, pt.y);
    }
    std::vector<Point> shifted;
    shifted.reserve(n);
    for (const auto& pt : points) {
        shifted.push_back({pt.x - minX, pt.y - minY});
    }

    // Jarvis march to find convex hull vertices in counterclockwise order.
    std::vector<Point> hull;
    int start = 0;
    for (int i = 1; i < n; ++i) {
        if (shifted[i].x < shifted[start].x ||
            (shifted[i].x == shifted[start].x && shifted[i].y < shifted[start].y)) {
            start = i;
        }
    }
    int current = start;
    do {
        hull.push_back(shifted[current]);
        int next = (current + 1) % n;
        for (int i = 0; i < n; ++i) {
            if (i == current || i == next) continue;
            if (cross(shifted[current], shifted[next], shifted[i]) > 0) {
                // If i is to the left of current->next, then i is more "outer" than next.
                // For counterclockwise hull, we want the rightmost turn (max negative cross).
                // Actually adjust: We want the point that makes the smallest clockwise turn,
                // i.e., the one with the most negative cross product. So we check < 0.
                // Simplification: We choose the point that is lexicographically "rightmost" from current.
                // Let's re-implement correctly: We need the point that minimizes the angle (most clockwise).
                // A common approach: choose q such that for all i, cross(current, q, i) <= 0 (non-positive).
                // Let's just use the classic condition: if cross(shifted[current], shifted[i], shifted[next]) <= 0,
                // then i is a better next candidate.
                if (cross(shifted[current], shifted[i], shifted[next]) <= 0) {
                    next = i;
                }
            }
        }
        current = next;
    } while (current != start);

    const int h = static_cast<int>(hull.size());
    if (h < 3) return false; // Degenerate hull.

    // Check if query is strictly inside: must be strictly left of every directed edge.
    // Shift query coordinates as well.
    Point q = {query.x - minX, query.y - minY};
    for (int i = 0; i < h; ++i) {
        int j = (i + 1) % h;
        if (cross(hull[i], hull[j], q) <= 0) {
            return false; // On boundary or outside.
        }
        // For strictness, require cross > 0. If cross == 0, boundary.
    }
    return true;
}
#include <cassert>
#include <vector>

// Assume isInsideHull and Point are defined above.

int main() {
    // Square hull around origin.
    std::vector<Point> square = {{0,0}, {10,0}, {10,10}, {0,10}};
    assert(isInsideHull(square, {5,5}) == true);
    assert(isInsideHull(square, {0,5}) == false);  // on edge
    assert(isInsideHull(square, {10,10}) == false); // vertex
    assert(isInsideHull(square, {11,5}) == false); // outside

    // Triangle with negative coordinates.
    std::vector<Point> triangle = {{-3,-3}, {3,-3}, {0,4}};
    assert(isInsideHull(triangle, {0,0}) == true);
    assert(isInsideHull(triangle, {0,-3}) == false); // on edge
    assert(isInsideHull(triangle, {-3,-3}) == false); // vertex
    assert(isInsideHull(triangle, {-5,0}) == false); // outside

    // Points inside but not on hull (interior points).
    std::vector<Point> with_inside = {{0,0}, {4,0}, {4,4}, {0,4}, {2,2}, {1,1}};
    assert(isInsideHull(with_inside, {2,3}) == true);
    assert(isInsideHull(with_inside, {0,0}) == false); // on hull
    assert(isInsideHull(with_inside, {2,1}) == true); // interior

    // Degenerate with repeated points? Not tested here, but general position is guaranteed.

    // All points collinear.
    std::vector<Point> collinear = {{0,0}, {1,1}, {2,2}};
    assert(isInsideHull(collinear, {1,1}) == false); // degenerate hull
    assert(isInsideHull(collinear, {0,0}) == false);

    // Small number of points.
    std::vector<Point> triple = {{0,0}, {2,0}, {1,3}};
    assert(isInsideHull(triple, {1,1}) == true);
    assert(isInsideHull(triple, {0,0}) == false);
    assert(isInsideHull(triple, {3,0}) == false);

    return 0;
}
