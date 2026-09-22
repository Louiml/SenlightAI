Write a C++ function named `stepKMeans` that performs one single iteration of the K-means clustering algorithm on a given set of 3D points (with integer coordinates) and a fixed number of clusters `k`. The function must accept a vector of `Point` objects (each having mutable `clusterId`, and methods `distance`, `getFeature_int`, `setFeature`) and a vector of `Point` objects representing the current centroids. It should assign each point to the nearest centroid (updating its `clusterId` if changed), then recompute each centroid as the integer-rounded mean of the coordinates of all points assigned to that cluster. The function should return a boolean indicating whether any point's cluster assignment changed during this iteration. Handle the edge case where a cluster may end up with zero points: in that case, leave its centroid unchanged. The result must modify the `clusterId` of each input point in place and update the centroid coordinates in the centroids vector. Calcuate centroids by summing each feature as integers, then dividing by the cluster size, truncating toward zero (standard integer division). Assume `k` is positive and centroids has exactly `k` elements; the points vector may be empty.

The algorithm processes each point to find the nearest centroid by computing the Euclidean distance between the point and each centroid. The `distance` method of `Point` computes this. For each point, we track the minimum distance and the index of the closest centroid; if that index differs from the current `clusterId`, we update it and set a `change` flag. After all assignments, we compute sums of each coordinate for each cluster using a 2D vector or array of size `k x 3` and a count array for cluster sizes. For clusters with zero points, the sum remains zero and count is zero; we detect this and skip updating the centroid, preserving its previous value. Otherwise, we compute each centroid's coordinate as `static_cast<int>(sum / count)` which does integer division truncating toward zero (for positive sums, it's floor; for negative, it's truncation toward zero, matching typical C++ behavior). Time complexity is `O(n*k)` for the assignment step and `O(n*k)` for the sum step (the naive double loop over points and clusters per centroid also yields `O(n*k)`), so overall `O(n*k)` per iteration, where `n` is the number of points. Space complexity is `O(k)` for the sums and counts arrays. Edge cases include empty points (returns false, centroids unchanged), clusters with no assigned points (centroid unchanged), and clusters with a single point (centroid becomes that point's coordinates exactly).

#include <vector>
#include <limits>

// Assume Point is defined elsewhere with:
// - int clusterId (mutable)
// - double distance(const Point& other) const
// - int getFeature_int(int index) const // index 0..2 for x, y, z
// - void setFeature(int index, int value)

bool stepKMeans(std::vector<Point>& points, std::vector<Point>& centroids, int k) {
    bool change = false;

    // Assignment step
    for (auto& p : points) {
        double minDist = std::numeric_limits<double>::max();
        int nearest = 0;
        for (int j = 0; j < k; ++j) {
            double dist = p.distance(centroids[j]);
            if (dist < minDist) {
                minDist = dist;
                nearest = j;
            }
        }
        if (p.clusterId != nearest) {
            p.clusterId = nearest;
            change = true;
        }
    }

    // Compute sums and counts per cluster
    std::vector<std::vector<long long>> sums(k, std::vector<long long>(3, 0));
    std::vector<int> counts(k, 0);

    for (const auto& p : points) {
        int c = p.clusterId;
        if (c >= 0 && c < k) {
            counts[c]++;
            for (int dim = 0; dim < 3; ++dim) {
                sums[c][dim] += static_cast<long long>(p.getFeature_int(dim));
            }
        }
    }

    // Update centroids, only for non-empty clusters
    for (int i = 0; i < k; ++i) {
        if (counts[i] > 0) {
            for (int dim = 0; dim < 3; ++dim) {
                int newValue = static_cast<int>(sums[i][dim] / counts[i]);
                centroids[i].setFeature(dim, newValue);
            }
        }
    }

    return change;
}

#include <cassert>
#include <cmath>

// Minimal Point implementation for testing (simplified, but matches interface)
struct Point {
    int x, y, z;
    int clusterId;

    Point(int x_, int y_, int z_, int c = -1) : x(x_), y(y_), z(z_), clusterId(c) {}

    double distance(const Point& other) const {
        return std::sqrt((x - other.x)*(x - other.x) + 
                         (y - other.y)*(y - other.y) + 
                         (z - other.z)*(z - other.z));
    }

    int getFeature_int(int index) const {
        if (index == 0) return x;
        if (index == 1) return y;
        return z;
    }

    void setFeature(int index, int value) {
        if (index == 0) x = value;
        else if (index == 1) y = value;
        else z = value;
    }
};

int main() {
    // Test 1: Simple two clusters
    std::vector<Point> pts = {
        Point(0, 0, 0), Point(1, 0, 0), Point(9, 9, 9), Point(10, 10, 10)
    };
    std::vector<Point> centroids = { Point(0, 0, 0), Point(10, 10, 10) };
    bool changed = stepKMeans(pts, centroids, 2);
    assert(changed == true);
    // Points 0 and 1 should be in cluster 0; points 2 and 3 in cluster 1
    assert(pts[0].clusterId == 0);
    assert(pts[1].clusterId == 0);
    assert(pts[2].clusterId == 1);
    assert(pts[3].clusterId == 1);
    // New centroids are rounded means
    assert(centroids[0].x == 0 && centroids[0].y == 0 && centroids[0].z == 0);
    // Mean of (9,9,9) and (10,10,10) is (9,9,9) truncated toward zero (integer division 19/2=9)
    assert(centroids[1].x == 9 && centroids[1].y == 9 && centroids[1].z == 9);

    // Test 2: Second iteration with stable assignment
    changed = stepKMeans(pts, centroids, 2);
    assert(changed == false); // all remain in same cluster

    // Test 3: Empty points vector
    std::vector<Point> emptyPts;
    std::vector<Point> cen3 = { Point(1, 2, 3), Point(4, 5, 6) };
    assert(stepKMeans(emptyPts, cen3, 2) == false);
    assert(cen3[0].x == 1 && cen3[0].y == 2 && cen3[0].z == 3);
    assert(cen3[1].x == 4 && cen3[1].y == 5 && cen3[1].z == 6);

    // Test 4: Single point in a cluster and empty cluster
    std::vector<Point> pts2 = { Point(5, 5, 5) };
    std::vector<Point> cen4 = { Point(0, 0, 0), Point(100, 100, 100) };
    bool changed2 = stepKMeans(pts2, cen4, 2);
    assert(changed2 == true);
    assert(pts2[0].clusterId == 0);
    // centroid 0 becomes (5,5,5), centroid 1 unchanged (empty cluster)
    assert(cen4[0].x == 5 && cen4[0].y == 5 && cen4[0].z == 5);
    assert(cen4[1].x == 100 && cen4[1].y == 100 && cen4[1].z == 100);

    // Test 5: Negative coordinates and rounding toward zero
    std::vector<Point> pts3 = { Point(-3, -3, -3), Point(-2, -2, -2) };
    std::vector<Point> cen5 = { Point(0, 0, 0) };
    bool changed3 = stepKMeans(pts3, cen5, 1);
    assert(changed3 == true);
    assert(pts3[0].clusterId == 0 && pts3[1].clusterId == 0);
    // Sum = -5, count=2, -5/2 = -2 (truncation toward zero)
    assert(cen5[0].x == -2 && cen5[0].y == -2 && cen5[0].z == -2);

    return 0;
}
