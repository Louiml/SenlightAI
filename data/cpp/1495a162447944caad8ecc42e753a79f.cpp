// Create a C++ function that computes a simplified local geometric descriptor for each point in a point cloud. Given two input point clouds (a source and a target) as vectors of 3D points, implement a function `computeLocalDescriptors` that returns a vector of descriptor vectors (each descriptor being a fixed-size `std::vector<double>` of length 3) capturing the local surface variation around each point. The descriptor for a point should be based on its `k` nearest neighbors (including itself) within the same cloud, and consist of: (1) the mean height (z-coordinate) of the neighbors, (2) the standard deviation of the z-coordinates of the neighbors, and (3) the roughness defined as the mean absolute deviation of the neighbors' z-coordinates from the local plane fitted through them (approximated by the mean z-deviation from the mean). The function must handle null or empty input gracefully by returning an empty vector, must assume a fixed `k = 5` (if fewer than 5 points exist, use all available points), and must not modify the input clouds. The function should be const-correct and use only standard library containers and algorithms.

// The solution approach involves, for each point in the cloud, finding the `k` nearest neighbors by Euclidean distance. Since no external libraries like PCL are allowed, we need a simple brute-force search: for each query point, compute distances to every other point, then select the smallest `k` distances and their corresponding indices. To avoid O(n²) memory, we can use a priority queue (max-heap) of size at most `k` storing pairs of (distance, neighbor index). After collecting the neighbors, we compute the descriptive statistics: mean of z, standard deviation of z (population standard deviation, using `sqrt(mean((z - mean_z)^2))`), and roughness as the mean absolute deviation from the mean z (i.e., `mean(abs(z - mean_z))`). Edge cases: (1) empty input → return empty vector; (2) cloud size smaller than `k` → use all points; (3) duplicate points at the same location are allowed; (4) if cloud has exactly one point, the descriptor will have mean = that point's z, stddev = 0, roughness = 0. Time complexity: For each of `n` points, we compute distances to all `n` points → O(n²) total distance computations, and for each point we maintain a heap of size `k` → O(n * n * log k) worst-case, which simplifies to O(n² log k) (with k constant, effectively O(n²)). Space complexity: O(k) per query points (heap) plus O(n * 3) for the output, so O(n) additional space besides inputs.

#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <utility>

// Compute a 3-dimensional local descriptor for each point in a point cloud.
// Each point is a 3D coordinate (x, y, z). The descriptor uses k (default 5) nearest neighbors.
// The descriptor is [mean_z, stddev_z, roughness] where roughness is mean absolute deviation from mean z.
std::vector<std::vector<double>> computeLocalDescriptors(
    const std::vector<std::array<double, 3>>& cloud,
    int k = 5)
{
    const int n = static_cast<int>(cloud.size());
    if (n == 0) return {};

    // If k is larger than the cloud, use all points.
    int effective_k = std::min(k, n);

    std::vector<std::vector<double>> descriptors(n, std::vector<double>(3, 0.0));

    for (int i = 0; i < n; ++i) {
        // Use a max-heap to keep the smallest effective_k distances.
        // Heap stores pairs (distance, index), where the top has the largest distance.
        std::priority_queue<std::pair<double, int>> heap;

        for (int j = 0; j < n; ++j) {
            double dx = cloud[i][0] - cloud[j][0];
            double dy = cloud[i][1] - cloud[j][1];
            double dz = cloud[i][2] - cloud[j][2];
            double dist = std::sqrt(dx*dx + dy*dy + dz*dz);

            if (static_cast<int>(heap.size()) < effective_k) {
                heap.push({dist, j});
            } else if (dist < heap.top().first) {
                heap.pop();
                heap.push({dist, j});
            }
        }

        // Collect the neighbor indices (including self, since distance 0 is always included).
        std::vector<int> neighbors;
        neighbors.reserve(effective_k);
        while (!heap.empty()) {
            neighbors.push_back(heap.top().second);
            heap.pop();
        }

        // Compute statistics on z-coordinates of neighbors.
        double sum_z = 0.0;
        for (int idx : neighbors) {
            sum_z += cloud[idx][2];
        }
        double mean_z = sum_z / static_cast<double>(neighbors.size());

        double sum_sq_diff = 0.0;
        double sum_abs_diff = 0.0;
        for (int idx : neighbors) {
            double diff = cloud[idx][2] - mean_z;
            sum_sq_diff += diff * diff;
            sum_abs_diff += std::abs(diff);
        }
        double variance = sum_sq_diff / static_cast<double>(neighbors.size());
        double stddev_z = std::sqrt(variance);
        double roughness = sum_abs_diff / static_cast<double>(neighbors.size());

        descriptors[i][0] = mean_z;
        descriptors[i][1] = stddev_z;
        descriptors[i][2] = roughness;
    }

    return descriptors;
}

#include <cassert>
#include <array>
#include <vector>
#include <cmath>

// The solution function is assumed to be defined above.
// Include the solution code here or link appropriately.

int main() {
    // Test 1: Empty cloud returns empty descriptor vector.
    {
        std::vector<std::array<double, 3>> cloud;
        auto desc = computeLocalDescriptors(cloud);
        assert(desc.empty());
    }

    // Test 2: Single point: mean = z, stddev = 0, roughness = 0.
    {
        std::vector<std::array<double, 3>> cloud = {{{0.0, 0.0, 5.0}}};
        auto desc = computeLocalDescriptors(cloud);
        assert(desc.size() == 1);
        assert(desc[0][0] == 5.0);
        assert(desc[0][1] == 0.0);
        assert(desc[0][2] == 0.0);
    }

    // Test 3: Two points with different z: k=5 uses both.
    {
        std::vector<std::array<double, 3>> cloud = {{{0.0, 0.0, 1.0}, {1.0, 0.0, 3.0}}};
        auto desc = computeLocalDescriptors(cloud);
        // For point 0: mean = 2.0, stddev = 1.0, roughness = 1.0
        assert(desc.size() == 2);
        assert(std::abs(desc[0][0] - 2.0) < 1e-9);
        assert(std::abs(desc[0][1] - 1.0) < 1e-9);
        assert(std::abs(desc[0][2] - 1.0) < 1e-9);
        // For point 1: same values
        assert(std::abs(desc[1][0] - 2.0) < 1e-9);
    }

    // Test 4: Four points where nearest neighbors are clear.
    {
        std::vector<std::array<double, 3>> cloud = {
            {0.0, 0.0, 0.0},
            {0.0, 0.1, 2.0},
            {0.0, 0.2, 4.0},
            {100.0, 0.0, 10.0} // far away
        };
        auto desc = computeLocalDescriptors(cloud, 3); // k=3, but with far point, nearest 3 are first three.
        // For point 0: neighbors are 0,1,2 => z's are 0,2,4 => mean=2, stddev = sqrt((4+0+4)/3) ≈ 1.63299, roughness = (2+0+2)/3 ≈ 1.33333
        assert(desc.size() == 4);
        assert(std::abs(desc[0][0] - 2.0) < 1e-6);
        assert(std::abs(desc[0][1] - std::sqrt(8.0/3.0)) < 1e-6);
        assert(std::abs(desc[0][2] - (4.0/3.0)) < 1e-6);
    }

    // Test 5: Larger k than cloud size uses all points.
    {
        std::vector<std::array<double, 3>> cloud = {
            {0.0, 0.0, 0.0},
            {0.0, 0.0, 1.0},
            {0.0, 0.0, 2.0}
        };
        auto desc = computeLocalDescriptors(cloud, 100); // k=100, but only 3 points
        assert(desc.size() == 3);
        // All points have same descriptor: mean=1, stddev≈0.816496, roughness≈0.666666
        for (auto& d : desc) {
            assert(std::abs(d[0] - 1.0) < 1e-9);
            assert(std::abs(d[1] - std::sqrt(2.0/3.0)) < 1e-9);
            assert(std::abs(d[2] - (2.0/3.0)) < 1e-9);
        }
    }

    return 0;
}
