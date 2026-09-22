Write a C++ function that, given a vector of 3D points (each represented as an array of three doubles, x, y, z) and a positive integer `k`, clusters the points into exactly `k` clusters using k-means clustering with random initialization (using a fixed seed for reproducibility) and returns a vector of cluster centroids, where each centroid is also represented as an array of three doubles. The function must handle the edge case where the number of points is less than `k` by returning an empty vector. The k-means algorithm should run for at most 100 iterations or until the centroids no longer change (i.e., the sum of squared differences between old and new centroids is below a small tolerance, e.g., 1e-10). Points are assumed to be unique, but duplicates are allowed in input; clustering should still work correctly.

#include <cassert>
#include <vector>
#include <array>
#include <cmath>

int main() {
    // Test 1: Basic clustering with well-separated points.
    std::vector<std::array<double, 3>> points = {
        {0.0, 0.0, 0.0},
        {0.1, 0.0, 0.0},
        {0.0, 0.1, 0.0},
        {10.0, 10.0, 10.0},
        {10.1, 10.0, 10.0},
        {10.0, 10.1, 10.0}
    };
    auto centroids = kMeansClustering(points, 2);
    assert(centroids.size() == 2);
    // Check that centroids are near expected cluster centers (0,0,0) and (10,10,10).
    bool foundA = false, foundB = false;
    for (auto& c : centroids) {
        double distA = sqrt(c[0]*c[0] + c[1]*c[1] + c[2]*c[2]);
        double distB = sqrt((c[0]-10)*(c[0]-10) + (c[1]-10)*(c[1]-10) + (c[2]-10)*(c[2]-10));
        if (distA < 1.0) foundA = true;
        if (distB < 1.0) foundB = true;
    }
    assert(foundA && foundB);

    // Test 2: Fewer points than k -> empty vector.
    std::vector<std::array<double, 3>> small = {{1.0, 2.0, 3.0}};
    auto emptyResult = kMeansClustering(small, 2);
    assert(emptyResult.empty());

    // Test 3: Single cluster (k=1) returns mean.
    std::vector<std::array<double, 3>> cluster1 = {
        {1.0, 2.0, 3.0},
        {3.0, 4.0, 5.0},
        {5.0, 6.0, 7.0}
    };
    auto oneCentroid = kMeansClustering(cluster1, 1);
    assert(oneCentroid.size() == 1);
    assert(fabs(oneCentroid[0][0] - 3.0) < 1e-9);
    assert(fabs(oneCentroid[0][1] - 4.0) < 1e-9);
    assert(fabs(oneCentroid[0][2] - 5.0) < 1e-9);

    // Test 4: Duplicate points still cluster properly.
    std::vector<std::array<double, 3>> dup = {
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        {5.0, 5.0, 5.0},
        {5.0, 5.0, 5.0}
    };
    auto dupCentroids = kMeansClustering(dup, 2);
    assert(dupCentroids.size() == 2);
    bool nearZero = false, nearFive = false;
    for (auto& c : dupCentroids) {
        double d0 = sqrt(c[0]*c[0] + c[1]*c[1] + c[2]*c[2]);
        double d5 = sqrt((c[0]-5)*(c[0]-5) + (c[1]-5)*(c[1]-5) + (c[2]-5)*(c[2]-5));
        if (d0 < 1e-6) nearZero = true;
        if (d5 < 1e-6) nearFive = true;
    }
    assert(nearZero && nearFive);

    // Test 5: k equals number of points -> each centroid is a point itself.
    std::vector<std::array<double, 3>> allPoints = {
        {0.0, 0.0, 0.0},
        {1.0, 1.0, 1.0},
        {2.0, 2.0, 2.0}
    };
    auto exactCentroids = kMeansClustering(allPoints, 3);
    assert(exactCentroids.size() == 3);
    // Check that all centroids are within tiny distance of original points.
    for (auto& c : exactCentroids) {
        bool matched = false;
        for (auto& p : allPoints) {
            double dx = c[0] - p[0];
            double dy = c[1] - p[1];
            double dz = c[2] - p[2];
            if (dx*dx + dy*dy + dz*dz < 1e-6) { matched = true; break; }
        }
        assert(matched);
    }

    return 0;
}

#include <vector>
#include <array>
#include <cmath>
#include <random>
#include <limits>
#include <algorithm>

// Perform k-means clustering on a set of 3D points.
// Returns a vector of k centroids (each as an array of three doubles).
// If the number of points is less than k, returns an empty vector.
std::vector<std::array<double, 3>> kMeansClustering(
    const std::vector<std::array<double, 3>>& points,
    int k) {
    
    const int n = static_cast<int>(points.size());
    if (n < k || k <= 0) {
        return {};
    }

    // Random generator with fixed seed for reproducibility.
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, n - 1);

    // Initialize centroids by choosing k distinct random points.
    std::vector<std::array<double, 3>> centroids(k);
    std::vector<int> chosen;
    chosen.reserve(k);
    while (static_cast<int>(chosen.size()) < k) {
        int idx = dist(rng);
        bool already = false;
        for (int c : chosen) {
            if (c == idx) { already = true; break; }
        }
        if (!already) {
            chosen.push_back(idx);
            centroids[chosen.size() - 1] = points[idx];
        }
    }

    // Vector to store cluster assignment for each point.
    std::vector<int> assignments(n, 0);

    const double tolerance = 1e-10;
    const int maxIterations = 100;

    for (int iter = 0; iter < maxIterations; ++iter) {
        // Assignment step: assign each point to nearest centroid.
        for (int i = 0; i < n; ++i) {
            double minDist = std::numeric_limits<double>::max();
            int bestCluster = 0;
            for (int c = 0; c < k; ++c) {
                double dx = points[i][0] - centroids[c][0];
                double dy = points[i][1] - centroids[c][1];
                double dz = points[i][2] - centroids[c][2];
                double distSq = dx*dx + dy*dy + dz*dz;
                if (distSq < minDist) {
                    minDist = distSq;
                    bestCluster = c;
                }
            }
            assignments[i] = bestCluster;
        }

        // Update step: compute new centroids as means.
        std::vector<std::array<double, 3>> newCentroids(k);
        std::vector<int> counts(k, 0);
        for (int i = 0; i < n; ++i) {
            int c = assignments[i];
            newCentroids[c][0] += points[i][0];
            newCentroids[c][1] += points[i][1];
            newCentroids[c][2] += points[i][2];
            counts[c]++;
        }

        // Handle empty clusters by reinitializing to a random point.
        for (int c = 0; c < k; ++c) {
            if (counts[c] == 0) {
                int idx = dist(rng);
                newCentroids[c] = points[idx];
                counts[c] = 1;
            } else {
                newCentroids[c][0] /= counts[c];
                newCentroids[c][1] /= counts[c];
                newCentroids[c][2] /= counts[c];
            }
        }

        // Compute sum of squared differences between old and new centroids.
        double diffSumSq = 0.0;
        for (int c = 0; c < k; ++c) {
            double dx = newCentroids[c][0] - centroids[c][0];
            double dy = newCentroids[c][1] - centroids[c][1];
            double dz = newCentroids[c][2] - centroids[c][2];
            diffSumSq += dx*dx + dy*dy + dz*dz;
        }

        centroids = newCentroids;

        // Check convergence.
        if (diffSumSq < tolerance) {
            break;
        }
    }

    return centroids;
}

// The solution uses the standard k-means algorithm. Initialize centroids by randomly selecting `k` distinct points from the input vector. To ensure reproducibility, use a fixed random seed (e.g., `std::mt19937` with seed 42) and select distinct indices without replacement. Repeat for up to 100 iterations: assign each point to the nearest centroid (using Euclidean distance), then recompute each centroid as the mean of all points assigned to it. If a cluster ends up empty (which can happen when `k` is close to the number of points or with poor initialization), reinitialize that centroid to a random point from the input. After each recomputation, check convergence by comparing the sum of squared Euclidean distances between old and new centroids; if below tolerance 1e-10, stop. Time complexity: \(O(I \cdot n \cdot k \cdot d)\) where \(I\) is the number of iterations (max 100), \(n\) is number of points, \(k\) is number of clusters, and \(d=3\) is dimensionality. Space complexity: \(O(n + k)\) for assignments and centroids.
