// Write a C++ function that generates a point cloud of `n` random 3D points with coordinates uniformly distributed in the range `[0, 100)`, builds a `pcl::KdTreeFLANN<pcl::PointXYZ>` from that cloud, then performs a fixed number of nearest-neighbor searches for a given `K` value, and finally returns the average time (in seconds) taken for the index construction (tree building) and the average query time. The function should accept the number of points `n`, the number of neighbors `K`, and the number of repeated trials `numTrials` as parameters. It must use `std::clock()` for timing, include the necessary PCL headers and the standard library, and return a pair of doubles (construction time, query time) as averages over the trials. Ensure the code is self-contained, compiles with standard C++11, and handles any positive integers for `n`, `K`, and `numTrials` gracefully (e.g., if `K` is greater than `n`, still work without crashing by letting PCL return fewer neighbors).

// The solution involves generating random points within the specified range, building a PCL FLANN KD-tree, and timing both the construction and a sequence of queries. The main algorithm is straightforward: for each trial, we allocate a `pcl::PointCloud<pcl::PointXYZ>` of size `n`, fill it with random coordinates, and record the CPU time before and after calling `setInputCloud` to measure construction time. Then we generate a random query point, call `nearestKSearch` with the given `K`, and time that as well. We accumulate total times over all trials and divide by `numTrials` to get averages. Key edge cases: `n` and `K` must be positive, and if `K` exceeds `n`, PCL returns only `n` neighbors (since there are only `n` points), so we should not assume the output vectors are exactly of size `K`; we can simply ignore the results. Also, `numTrials` should be positive, and to avoid division by zero, we guard that. For timing, we use `std::clock()` which gives CPU time, and we convert to seconds by dividing by `CLOCKS_PER_SEC`. Complexity: building the tree is approximately `O(n log n)` time and `O(n)` space; each query is `O(log n)` average time. Total time for the function is `O(numTrials * (n log n + log n))`, which simplifies to `O(numTrials * n log n)`.

#include <pcl/point_cloud.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <vector>

// Return a pair (average construction time, average query time) in seconds.
// Generates a random point cloud of size n, builds a PCL FLANN KD-tree,
// then performs nearest K searches for numTrials trials using random query points.
std::pair<double, double> pclKDTreeBenchmark(int n, int K, int numTrials) {
    // Guard against invalid inputs: use at least 1 for each parameter.
    if (n <= 0) n = 1;
    if (K <= 0) K = 1;
    if (numTrials <= 0) numTrials = 1;

    // Use a fixed seed for reproducibility? No, we want random behavior; but we seed once.
    // If called multiple times, we might re-seed, but for consistency we'll seed with time.
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    double totalConstructionTime = 0.0;
    double totalQueryTime = 0.0;

    const double RANDOM_SCALE = 100.0;

    for (int trial = 0; trial < numTrials; ++trial) {
        // Generate point cloud with n random points.
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
        cloud->width = n;
        cloud->height = 1;
        cloud->points.resize(cloud->width * cloud->height);

        for (int i = 0; i < n; ++i) {
            (*cloud)[i].x = RANDOM_SCALE * std::rand() / (RAND_MAX + 1.0f);
            (*cloud)[i].y = RANDOM_SCALE * std::rand() / (RAND_MAX + 1.0f);
            (*cloud)[i].z = RANDOM_SCALE * std::rand() / (RAND_MAX + 1.0f);
        }

        // Build the KD-tree and time construction.
        pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;
        std::clock_t t0 = std::clock();
        kdtree.setInputCloud(cloud);
        std::clock_t t1 = std::clock();
        totalConstructionTime += static_cast<double>(t1 - t0) / CLOCKS_PER_SEC;

        // Generate a random query point.
        pcl::PointXYZ searchPoint;
        searchPoint.x = RANDOM_SCALE * std::rand() / (RAND_MAX + 1.0f);
        searchPoint.y = RANDOM_SCALE * std::rand() / (RAND_MAX + 1.0f);
        searchPoint.z = RANDOM_SCALE * std::rand() / (RAND_MAX + 1.0f);

        // Perform K nearest neighbor search and time it.
        std::vector<int> pointIdxNKNSearch(K);
        std::vector<float> pointNKNSquaredDistance(K);
        std::clock_t t2 = std::clock();
        kdtree.nearestKSearch(searchPoint, K, pointIdxNKNSearch, pointNKNSquaredDistance);
        std::clock_t t3 = std::clock();
        totalQueryTime += static_cast<double>(t3 - t2) / CLOCKS_PER_SEC;
    }

    return std::make_pair(totalConstructionTime / numTrials, totalQueryTime / numTrials);
}

#include <cassert>
#include <cmath>
#include <iostream>
#include <utility>

// Declaration of the function to test (assume it's provided in the solution above).
std::pair<double, double> pclKDTreeBenchmark(int n, int K, int numTrials);

int main() {
    // Basic sanity checks: time values should be non-negative and construction should not be negative.
    {
        auto result = pclKDTreeBenchmark(1000, 10, 5);
        assert(result.first >= 0.0);
        assert(result.second >= 0.0);
        // Construction time is typically larger than zero for 1000 points, but allow small rounding.
        assert(result.first > 0.0 || result.first == 0.0); // might be zero if too fast, but we just ensure non-negative
    }

    // Test with n=1, K=10 (K > n). Should not crash and times should be non-negative.
    {
        auto result = pclKDTreeBenchmark(1, 10, 3);
        assert(result.first >= 0.0);
        assert(result.second >= 0.0);
    }

    // Test with very small inputs and many trials to get stable averages.
    {
        auto result = pclKDTreeBenchmark(100, 5, 20);
        assert(result.first >= 0.0);
        assert(result.second >= 0.0);
        // Query time should be positive when K is positive and n>0.
        assert(result.second > 0.0);
    }

    // Test with larger n to ensure no crash and reasonable times.
    {
        auto result = pclKDTreeBenchmark(10000, 100, 2);
        assert(result.first >= 0.0);
        assert(result.second > 0.0);
    }

    // Test invalid inputs (n=0, K=0, numTrials=0) – function should still run without crashing.
    {
        auto result = pclKDTreeBenchmark(0, 0, 0);
        assert(result.first >= 0.0);
        assert(result.second >= 0.0);
    }

    // Test that average times are consistent: more trials should give similar averages (not exactly equal).
    {
        auto r1 = pclKDTreeBenchmark(500, 10, 1);
        auto r2 = pclKDTreeBenchmark(500, 10, 1);
        // Since random seeds change, we cannot expect equality; just ensure both are valid.
        assert(r1.first >= 0.0 && r1.second >= 0.0);
        assert(r2.first >= 0.0 && r2.second >= 0.0);
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}
