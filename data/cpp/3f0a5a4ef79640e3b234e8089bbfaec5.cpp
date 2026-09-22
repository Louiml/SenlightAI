Write a C++ function that takes a vector of 3D points (represented as `std::array<double,3>`), an integer `max_neighbors_per_point`, and a double `neighbor_radius`. The function should compute, for each point, the indices of all other points within the given Euclidean distance (`neighbor_radius`), but limit the number of stored neighbors per point to at most `max_neighbors_per_point`. The output should be a `std::vector<std::vector<size_t>>` where element `i` contains the indices of the neighbors of point `i` (sorted in ascending order). Points are considered neighbors if their Euclidean distance is strictly less than `neighbor_radius`. If a point has more qualifying neighbors than the limit, only the first `max_neighbors_per_point` nearest ones (by distance) should be kept; if ties occur at the cutoff distance, prefer the lowest index. The function must handle empty input and points with zero neighbors correctly, and must not modify the input vector. Assume dimensions are meaningful (no NaN or infinity).

// The solution approach involves a brute-force pairwise distance computation because the task does not require optimization for spatial structures—clarity and correctness take priority. For each point `i`, we iterate over all points `j` (where `j != i`), compute the squared Euclidean distance to avoid sqrt overhead, and collect candidates that satisfy `distance < neighbor_radius`. For each candidate, we store a pair `(distance, index)`. After processing all points, we sort the candidates for point `i` by distance (ascending), then by index (ascending) to resolve ties consistently. We then truncate the list to at most `max_neighbors_per_point` and extract the indices, which are automatically sorted by index because we sort by index as secondary key. Edge cases: empty input returns an empty vector; all points isolated yields empty inner vectors; `max_neighbors_per_point` of 0 means no neighbors stored (but we still must compute? For efficiency, we can skip building candidates if limit is 0). If a point has fewer candidates than the limit, all are kept. Complexity: For `n` points, there are `n*(n-1)/2` pairwise checks, each O(1), and sorting each point's candidate list. In the worst case, each point has O(n) candidates, so sorting costs O(n log n) per point, giving overall O(n^2 log n) time. Space is O(n * max_neighbors_per_point) for output plus O(n) temporary candidates, so O(n^2) in the worst case if all points are within radius. This is acceptable for moderate `n` but not for very large datasets.

#include <vector>
#include <array>
#include <cmath>
#include <algorithm>

// Compute up to max_neighbors_per_point neighbors for each point within neighbor_radius.
// Neighbors are sorted by distance ascending, then by index ascending for ties.
// Returns a vector of vectors of neighbor indices for each point.
std::vector<std::vector<size_t>> compute_neighbors(
    const std::vector<std::array<double,3>>& points,
    int max_neighbors_per_point,
    double neighbor_radius)
{
    const size_t n = points.size();
    std::vector<std::vector<size_t>> result(n);
    if (n == 0 || max_neighbors_per_point <= 0) {
        // Return empty neighbor lists if there are no points or no neighbor capacity.
        return result;
    }

    const double radius_sq = neighbor_radius * neighbor_radius;

    for (size_t i = 0; i < n; ++i) {
        // Store pairs of (squared_distance, index) for candidate neighbors.
        std::vector<std::pair<double, size_t>> candidates;
        candidates.reserve(n - 1);

        for (size_t j = 0; j < n; ++j) {
            if (i == j) continue;
            double dx = points[i][0] - points[j][0];
            double dy = points[i][1] - points[j][1];
            double dz = points[i][2] - points[j][2];
            double dist_sq = dx*dx + dy*dy + dz*dz;
            if (dist_sq < radius_sq) {
                candidates.emplace_back(dist_sq, j);
            }
        }

        // Sort by distance ascending, then by index ascending.
        std::sort(candidates.begin(), candidates.end(),
                  [](const std::pair<double, size_t>& a, const std::pair<double, size_t>& b) {
                      if (a.first != b.first) return a.first < b.first;
                      return a.second < b.second;
                  });

        // Keep at most max_neighbors_per_point.
        size_t keep = std::min<size_t>(candidates.size(), static_cast<size_t>(max_neighbors_per_point));
        result[i].reserve(keep);
        for (size_t k = 0; k < keep; ++k) {
            result[i].push_back(candidates[k].second);
        }
    }

    return result;
}

#include <cassert>
#include <vector>
#include <array>

int main() {
    using Point = std::array<double,3>;
    
    // Single point with no neighbors
    {
        std::vector<Point> pts = {{0,0,0}};
        auto result = compute_neighbors(pts, 10, 1.0);
        assert(result.size() == 1);
        assert(result[0].empty());
    }
    
    // Empty input
    {
        std::vector<Point> pts;
        auto result = compute_neighbors(pts, 10, 1.0);
        assert(result.empty());
    }
    
    // Basic two points within radius
    {
        std::vector<Point> pts = {{0,0,0}, {0.5,0,0}};
        auto result = compute_neighbors(pts, 10, 1.0);
        assert(result.size() == 2);
        assert(result[0] == std::vector<size_t>{1});
        assert(result[1] == std::vector<size_t>{0});
    }
    
    // Three points all within radius, max_neighbors=2
    {
        std::vector<Point> pts = {{0,0,0}, {0.1,0,0}, {0.2,0,0}};
        auto result = compute_neighbors(pts, 2, 1.0);
        assert(result.size() == 3);
        // For point 0, nearest are 1 (dist 0.1) and 2 (dist 0.2)
        assert(result[0] == std::vector<size_t>({1,2}));
        // For point 1, nearest are 0 (dist 0.1) and 2 (dist 0.1) -> tie broken by index
        assert(result[1] == std::vector<size_t>({0,2}));
        // For point 2, nearest are 1 (dist 0.1) and 0 (dist 0.2)
        assert(result[2] == std::vector<size_t>({1,0}));
    }
    
    // Zero max_neighbors
    {
        std::vector<Point> pts = {{0,0,0}, {0.5,0,0}};
        auto result = compute_neighbors(pts, 0, 1.0);
        assert(result.size() == 2);
        assert(result[0].empty());
        assert(result[1].empty());
    }
    
    // Points on boundary (distance exactly radius) are not neighbors
    {
        std::vector<Point> pts = {{0,0,0}, {1.0,0,0}};
        auto result = compute_neighbors(pts, 10, 1.0);
        assert(result.size() == 2);
        assert(result[0].empty());
        assert(result[1].empty());
    }
    
    // Ties at cutoff: multiple points at same distance, limit truncates by index
    {
        std::vector<Point> pts = {{0,0,0}, {0.5,0,0}, {-0.5,0,0}};
        // All distances from point 0 are 0.5, ties broken by index: 1 then 2
        auto result = compute_neighbors(pts, 1, 1.0);
        assert(result.size() == 3);
        assert(result[0] == std::vector<size_t>{1});
        // Point 1 sees 0 and 2 both at distance 0.5 -> index 0 first
        assert(result[1] == std::vector<size_t>{0});
    }
    
    // 3D distances
    {
        std::vector<Point> pts = {{0,0,0}, {1,1,1}, {0.5,0.5,0.5}};
        auto result = compute_neighbors(pts, 10, 1.0);
        // Distance from (0,0,0) to (0.5,0.5,0.5) is sqrt(0.75) ≈ 0.866 < 1
        // Distance from (0,0,0) to (1,1,1) is sqrt(3) ≈ 1.732 > 1
        assert(result[0] == std::vector<size_t>{2});
        // (1,1,1) to (0.5,0.5,0.5) distance also sqrt(0.75) < 1
        assert(result[1] == std::vector<size_t>{2});
        // (0.5,0.5,0.5) has both others within radius
        assert(result[2] == std::vector<size_t>({0,1}));
    }
    
    return 0;
}
