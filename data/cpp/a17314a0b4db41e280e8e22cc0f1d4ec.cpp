// Write a C++ function that implements the classic k-means++ initialization algorithm on a set of 2D points. The function should take a vector of pairs (x, y coordinates) and an integer k (number of clusters), and return a vector of k indices representing the selected initial cluster centers. The first center must be chosen uniformly at random from all points. Each subsequent center must be chosen with probability proportional to the squared distance from the point to the nearest already-selected center. If k is greater than the number of points or k is zero or negative, the function should return an empty vector. The function should use a fixed random seed for deterministic testing, but the logic must handle any input correctly.
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>

// The solution function is assumed to be available in the same scope
// (include the solution code above).

int main() {
    // Test 1: Invalid k -> empty vector
    std::vector<std::pair<double, double>> pts1 = {{0.0, 0.0}, {1.0, 1.0}};
    assert(kmeansPlusPlusInit(pts1, 0).empty());
    assert(kmeansPlusPlusInit(pts1, 3).empty());
    assert(kmeansPlusPlusInit(pts1, -1).empty());

    // Test 2: k=1 returns a single valid index
    auto result2 = kmeansPlusPlusInit(pts1, 1);
    assert(result2.size() == 1);
    assert(result2[0] < pts1.size());

    // Test 3: k equals number of points -> all indices present
    auto result3 = kmeansPlusPlusInit(pts1, 2);
    assert(result3.size() == 2);
    std::vector<size_t> sorted = result3;
    std::sort(sorted.begin(), sorted.end());
    assert(sorted[0] == 0 && sorted[1] == 1);

    // Test 4: Larger dataset, deterministic seed test
    std::vector<std::pair<double, double>> pts4;
    for (int i = 0; i < 10; ++i) {
        pts4.push_back({static_cast<double>(i), static_cast<double>(i * i)});
    }
    auto result4 = kmeansPlusPlusInit(pts4, 3);
    assert(result4.size() == 3);
    for (size_t idx : result4) {
        assert(idx < pts4.size());
    }

    // Test 5: Duplicate points, still works
    std::vector<std::pair<double, double>> pts5 = {{1.0, 1.0}, {1.0, 1.0}, {5.0, 5.0}, {5.0, 5.0}};
    auto result5 = kmeansPlusPlusInit(pts5, 2);
    assert(result5.size() == 2);
    // At least one center should be at 1,1 and one at 5,5
    bool has01 = false, has23 = false;
    for (size_t idx : result5) {
        if (idx == 0 || idx == 1) has01 = true;
        if (idx == 2 || idx == 3) has23 = true;
    }
    assert(has01 && has23);

    // Test 6: Verify deterministic behavior (same seed -> same output)
    auto result6a = kmeansPlusPlusInit(pts4, 5);
    auto result6b = kmeansPlusPlusInit(pts4, 5);
    assert(result6a == result6b);

    // Test 7: Empty input -> empty output
    std::vector<std::pair<double, double>> pts7;
    assert(kmeansPlusPlusInit(pts7, 1).empty());

    return 0;
}
#include <vector>
#include <utility>
#include <random>
#include <limits>

/**
 * Performs k-means++ initialization on a set of 2D points.
 * @param points Vector of (x, y) coordinates.
 * @param k Number of cluster centers to select.
 * @return Vector of indices into points representing the initial centers.
 *         Empty vector if k is invalid (k <= 0 or k > points.size()).
 */
std::vector<size_t> kmeansPlusPlusInit(const std::vector<std::pair<double, double>>& points, int k) {
    const size_t n = points.size();
    if (k <= 0 || static_cast<size_t>(k) > n) {
        return {};
    }

    // Deterministic random generator for testability
    std::mt19937 gen(42); // Fixed seed for reproducible results
    std::uniform_real_distribution<double> dist01(0.0, 1.0);

    std::vector<size_t> centers;
    centers.reserve(k);

    // Step 1: Choose first center uniformly at random
    std::uniform_int_distribution<size_t> uniform_index(0, n - 1);
    centers.push_back(uniform_index(gen));

    // Distances to nearest selected center (squared Euclidean distance)
    std::vector<double> min_dist(n, std::numeric_limits<double>::max());

    // Update distances for the first center
    for (size_t i = 0; i < n; ++i) {
        double dx = points[i].first - points[centers[0]].first;
        double dy = points[i].second - points[centers[0]].second;
        min_dist[i] = dx * dx + dy * dy;
    }

    // Select remaining k-1 centers
    for (int c = 1; c < k; ++c) {
        // Compute total sum of squared distances
        double total = 0.0;
        for (double d : min_dist) {
            total += d;
        }

        // If total is zero (all points are already centers or duplicates), pick randomly
        if (total == 0.0) {
            centers.push_back(uniform_index(gen));
            // Update min_dist for this new center
            size_t new_center = centers.back();
            for (size_t i = 0; i < n; ++i) {
                double dx = points[i].first - points[new_center].first;
                double dy = points[i].second - points[new_center].second;
                double d2 = dx * dx + dy * dy;
                if (d2 < min_dist[i]) {
                    min_dist[i] = d2;
                }
            }
            continue;
        }

        // Generate a random value in [0, total)
        double r = dist01(gen) * total;
        size_t idx = 0;
        double cumulative = 0.0;
        for (size_t i = 0; i < n; ++i) {
            cumulative += min_dist[i];
            if (cumulative >= r) {
                idx = i;
                break;
            }
        }

        centers.push_back(idx);

        // Update min_dist for all points with the new center
        for (size_t i = 0; i < n; ++i) {
            double dx = points[i].first - points[idx].first;
            double dy = points[i].second - points[idx].second;
            double d2 = dx * dx + dy * dy;
            if (d2 < min_dist[i]) {
                min_dist[i] = d2;
            }
        }
    }

    return centers;
}
// The solution follows the standard k-means++ initialization procedure. First, we validate that k is between 1 and the number of points, otherwise return an empty vector. We then use a deterministic random number generator (e.g., `std::mt19937` with a fixed seed) to ensure testability. We select the first center index uniformly at random from 0 to N-1. For each subsequent center, we maintain a vector `dist` where `dist[i]` is the squared distance from point i to the nearest already-selected center. After each new center is chosen, we update `dist` for all points by computing the squared distance to the new center and taking the minimum with the current value. To select the next center, we compute the total sum of all `dist` values, generate a random number in [0, total), then iterate through the points subtracting their `dist` from the random value; when the value becomes negative, that point is chosen. This ensures probability proportional to `dist[i]`. Edge cases include k=1 (only one center chosen), duplicate points (works fine since squared distances may be zero), and the possibility of selecting the same point twice (which is avoided because if a point is already a center, its distance is zero, so it has zero probability unless all remaining distances are zero; in that degenerate case the algorithm may pick it, but that's acceptable as it still yields a valid initialization). Time complexity is O(k·N) for computing and updating distances and selecting centers. Space complexity is O(N) for the distance vector.
