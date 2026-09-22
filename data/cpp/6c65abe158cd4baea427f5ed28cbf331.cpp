Write a C++ function `knn_brute_force` that takes a vector of 3D points (represented as `std::array<double, 3>`), a query point, and an integer `k` (where `k` is at least 1 and at most the number of points), and returns a vector of the indices of the `k` nearest points to the query, sorted by increasing distance. If there are ties in distance, break them by smaller index first. The function must not modify the input points, must handle empty input (return empty vector), and must compute distances using squared Euclidean distance to avoid square roots. The solution should be provided as a standalone free function named as specified.

The simplest and most robust approach for small to medium point sets is a brute-force scan over all points, computing the squared Euclidean distance from the query to each point. We maintain a result list of up to `k` pairs `(distance, index)`. For each point, we compare its squared distance with the current maximum distance in the result list. If the result list is not full, we insert the point. If it is full and the new distance is smaller than the current worst distance (or equal but with a smaller index), we replace the worst element. To keep the result sorted by distance (and index), after collecting candidates we sort the result by distance and then by index. A naive but clear approach is to collect all points with distances, sort the entire list, and take the first `k`. That is simpler and correct; for \(n\) points it is \(O(n \log n)\) time and \(O(n)\) space. Edge cases: empty input returns empty; if `k` is larger than input size, return all indices sorted by distance (ties by index); duplicate points are allowed. Since we sort by `(dist, index)`, ties resolve correctly. The function should be `const`-correct by taking inputs by const reference and not modifying them. Complexity: \(O(n \log n)\) time and \(O(n)\) auxiliary space for storing distance-index pairs, which is acceptable for the intended scope.

#include <array>
#include <algorithm>
#include <cstddef>
#include <vector>

// Returns indices of the k points closest to query, sorted by distance (ties by index).
std::vector<size_t> knn_brute_force(
    const std::vector<std::array<double, 3>>& points,
    const std::array<double, 3>& query,
    size_t k)
{
    std::vector<size_t> result;
    if (points.empty() || k == 0) {
        return result;
    }

    // Build list of (squared_distance, index) for all points.
    std::vector<std::pair<double, size_t>> candidates;
    candidates.reserve(points.size());
    for (size_t i = 0; i < points.size(); ++i) {
        double dx = points[i][0] - query[0];
        double dy = points[i][1] - query[1];
        double dz = points[i][2] - query[2];
        double dist_sq = dx * dx + dy * dy + dz * dz;
        candidates.emplace_back(dist_sq, i);
    }

    // Sort by distance, then by index for ties.
    std::sort(candidates.begin(), candidates.end());

    // Take first k candidates.
    size_t limit = std::min(k, candidates.size());
    result.reserve(limit);
    for (size_t i = 0; i < limit; ++i) {
        result.push_back(candidates[i].second);
    }
    return result;
}

#include <cassert>
#include <vector>
#include <array>

int main() {
    using Point = std::array<double, 3>;

    std::vector<Point> pts = {{0,0,0}, {1,0,0}, {0,1,0}, {10,10,10}};
    Point q = {0,0,0};

    auto r1 = knn_brute_force(pts, q, 2);
    assert((r1 == std::vector<size_t>{0, 1}) || (r1 == std::vector<size_t>{0, 2})); // Only one of these due to tie? Both have same distance? Actually 1 and 2 both have distance 1, so sorted by index gives {0,1}.
    assert(r1 == std::vector<size_t>{0, 1}); // 0 then 1 because index 1 before 2.

    auto r2 = knn_brute_force(pts, q, 1);
    assert(r2 == std::vector<size_t>{0});

    auto r3 = knn_brute_force(pts, q, 10); // k larger than size returns all, sorted by distance.
    assert(r3 == std::vector<size_t>{0, 1, 2, 3}); // distances: 0,1,1,300

    // Empty input
    std::vector<Point> empty;
    auto r4 = knn_brute_force(empty, q, 3);
    assert(r4.empty());

    // Ties with duplicate points
    std::vector<Point> dup = {{1,0,0}, {1,0,0}, {0,0,0}};
    auto r5 = knn_brute_force(dup, q, 2);
    assert(r5 == std::vector<size_t>{2, 0}); // index 2 distance 0, then 0 and 1 both distance 1, index 0 first.

    // Negative coordinates
    std::vector<Point> neg = {{-1,-1,-1}, {1,1,1}, {0,0,0}};
    auto r6 = knn_brute_force(neg, {0,0,0}, 2);
    assert(r6 == std::vector<size_t>{2, 0}); // distance 0 for index 2, then both 0 and 1 have same dist sqrt(3) but index 0 first.

    return 0;
}
