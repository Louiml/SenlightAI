// Write a C++ function `std::pair<size_t, double> findFarthestPoint(const std::vector<std::pair<double, double>>& points, size_t start, size_t end)` that, given a vector of 2D points and two indices `start` and `end` (where `start < end`), returns a pair consisting of the index of the point between `start` and `end` (exclusive) that is farthest from the line segment connecting `points[start]` and `points[end]`, and the corresponding distance. If the segment length is zero (i.e., the two endpoints coincide), the distance should be measured as the Euclidean distance from each intermediate point to the common endpoint. The function must handle the edge case where there are no intermediate points (i.e., `end - start <= 1`) by returning a pair with `first = start` and `second = 0.0`. Use `double` precision and avoid modifying the input vector; mark the function `const`-correct by taking the vector by `const` reference. The distance should be the perpendicular distance from the point to the infinite line through the segment, but only if the projection lies within the segment; otherwise use the distance to the nearest endpoint. Do not assume any external libraries—only use standard C++ headers.
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// Function declaration (as in solution)
std::pair<size_t, double> findFarthestPoint(const std::vector<std::pair<double, double>>& points,
                                             size_t start, size_t end);

int main() {
    // Simple segment with a point above the middle.
    std::vector<std::pair<double, double>> pts1 = {{0,0}, {0,1}, {1,1}, {1,0}};
    auto r1 = findFarthestPoint(pts1, 0, 3);
    assert(r1.first == 1); // point (0,1) is farthest? Actually (0,1) is 0 distance? Let's test properly.
    // For correctness, (0,1) is on the line from (0,0) to (1,0)? No, that's not collinear. Use a clear case.
    
    // Clear case: points (0,0), (0.5, 0.5), (1,0). Farthest should be (0.5,0.5) with distance 0.35355.
    std::vector<std::pair<double,double>> pts2 = {{0,0}, {0.5,0.5}, {1,0}};
    auto r2 = findFarthestPoint(pts2, 0, 2);
    assert(r2.first == 1);
    assert(std::fabs(r2.second - 0.353553) < 1e-4);

    // Degenerate segment: start == end coordinates.
    std::vector<std::pair<double,double>> pts3 = {{1,1}, {2,3}, {4,5}, {1,1}};
    auto r3 = findFarthestPoint(pts3, 0, 3);
    // Farthest from (1,1) among indices 1,2? distances: (2,3) ~2.236, (4,5)~5.0. So index 2.
    assert(r3.first == 2);
    assert(std::fabs(r3.second - 5.0) < 1e-4);

    // No intermediate points.
    std::vector<std::pair<double,double>> pts4 = {{0,0}, {2,2}};
    auto r4 = findFarthestPoint(pts4, 0, 1);
    assert(r4.first == 0);
    assert(r4.second == 0.0);

    // Point that lies exactly on the segment.
    std::vector<std::pair<double,double>> pts5 = {{0,0}, {0.5,0}, {1,0}};
    auto r5 = findFarthestPoint(pts5, 0, 2);
    assert(r5.first == 1);
    assert(std::fabs(r5.second) < 1e-6);

    // Point beyond the segment endpoint (projection outside).
    std::vector<std::pair<double,double>> pts6 = {{0,0}, {2,0}, {3,4}};
    auto r6 = findFarthestPoint(pts6, 0, 2);
    // Intermediate index 1: (2,0) is on segment? It's at t=1, distance 0. So best is index 1? But it's the only intermediate. Distance 0.
    // Actually we need a case with projection outside: use start=0, end=2, intermediate index 1 at (3,0). 
    std::vector<std::pair<double,double>> pts7 = {{0,0}, {3,0}, {1,0}};
    auto r7 = findFarthestPoint(pts7, 0, 2);
    // (3,0) is beyond p1 (1,0)? Wait p1=(1,0). The segment is from (0,0) to (1,0). Point (3,0) projects at t=3>1, distance to p1=2.0.
    assert(r7.first == 1);
    assert(std::fabs(r7.second - 2.0) < 1e-6);

    return 0;
}
#include <vector>
#include <cmath>
#include <utility>
#include <cstddef>

// Returns index and distance of the point farthest from the line segment [start, end].
// If segment length is zero, uses Euclidean distance to the common endpoint.
// If no points lie between start and end, returns {start, 0.0}.
std::pair<size_t, double> findFarthestPoint(const std::vector<std::pair<double, double>>& points,
                                             size_t start, size_t end) {
    // Handle degenerate range with no intermediate points.
    if (end - start <= 1) return {start, 0.0};

    const auto& p0 = points[start];
    const auto& p1 = points[end];
    double dx = p1.first - p0.first;
    double dy = p1.second - p0.second;
    double seg_len_sq = dx * dx + dy * dy;
    double seg_len = std::sqrt(seg_len_sq);

    size_t best_idx = start;
    double best_dist = 0.0;

    for (size_t i = start + 1; i < end; ++i) {
        double vx = points[i].first - p0.first;
        double vy = points[i].second - p0.second;
        double dist;

        if (seg_len_sq < 1e-12) { // Segment is nearly a point.
            dist = std::sqrt(vx * vx + vy * vy);
        } else {
            // Projection parameter t along segment from p0 to p1.
            double t = (vx * dx + vy * dy) / seg_len_sq;
            if (t < 0.0) {
                // Closest to p0.
                dist = std::sqrt(vx * vx + vy * vy);
            } else if (t > 1.0) {
                // Closest to p1.
                double ux = points[i].first - p1.first;
                double uy = points[i].second - p1.second;
                dist = std::sqrt(ux * ux + uy * uy);
            } else {
                // Perpendicular distance: |cross| / |segment|.
                double cross = dx * vy - dy * vx;
                dist = std::fabs(cross) / seg_len;
            }
        }

        if (dist > best_dist) {
            best_dist = dist;
            best_idx = i;
        }
    }

    return {best_idx, best_dist};
}
// The solution computes the farthest point from the line segment between `points[start]` and `points[end]`. First, we compute the vector from start to end and its magnitude. If the magnitude is near zero (within `1e-9`), all intermediate points are evaluated by their Euclidean distance to the shared endpoint—this covers degenerate segments where any perpendicular concept is undefined. Otherwise, we compute the direction vector and the normal vector (perpendicular to the segment). For each intermediate point, we project it onto the segment's direction to determine whether the perpendicular foot lies within the segment: if the projection parameter `t` is outside [0,1], the distance is to the nearer endpoint; otherwise it is the absolute cross product (or dot with normal) magnitude. We track the maximum distance and its index. Complexity: O(n) time where n = end - start, O(1) extra space. Edge cases: no intermediate points (return start, 0.0), coincident endpoints (use Euclidean distance), and numerical precision handled with a small epsilon.
