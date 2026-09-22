Write a C++ function `distanceWeightedAverage3D` that takes a vector of 3D points (each with x, y, z coordinates and an associated value), a query point (x, y, z), and a power parameter `p` (default 2.0), and returns the inverse distance weighted average of the values at the query point. Use the standard IDW formula: weight = 1 / (distance^p). If the query point exactly coincides with one or more data points (distance == 0), return the average of the values of all coincident points (ignoring all other points). If the input point list is empty, return `std::numeric_limits<double>::quiet_NaN()`. The function must be `const`-correct and use only standard library facilities. Ensure the function properly handles the case where all points are at the same location as the query (average of those values) and the general case with strictly positive distances.
The algorithm iterates over all provided points, computing the Euclidean distance from the query point to each point. For each point, we accumulate two quantities: the weighted sum of values (`weightedSum`) and the sum of weights (`weightSum`). The distance is computed using `std::sqrt(dx*dx + dy*dy + dz*dz)`. If any distance equals zero (within exact floating-point equality, as per the specification), we switch to handling the tie case: we collect the values of all points with exactly zero distance, compute their mean, and return that immediately, ignoring all nonzero-distance points. This handles the edge case where the query coincides with data points. If no points have zero distance, we compute `weightedSum / weightSum` as the result. If the input vector is empty, we return `NaN`. The function runs in O(N) time and uses O(1) extra space, where N is the number of points. It is important to note that floating-point equality comparison for distances uses exact `==` check; for typical data this works as expected when coordinates are exactly equal. The power parameter should be a positive real number; for p = 0, the function would divide by zero, but this is not expected in normal use—we simply apply the formula as given.
#include <vector>
#include <cmath>
#include <limits>
#include <cstddef>

struct Point3D {
    double x, y, z, value;
};

// Compute inverse distance weighted average of values at a query point.
double distanceWeightedAverage3D(const std::vector<Point3D>& points,
                                 double qx, double qy, double qz,
                                 double power = 2.0) {
    if (points.empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    double weightedSum = 0.0;
    double weightSum = 0.0;

    // First pass: accumulate weights and weighted values.
    for (const auto& p : points) {
        double dx = qx - p.x;
        double dy = qy - p.y;
        double dz = qz - p.z;
        double distSq = dx*dx + dy*dy + dz*dz;

        if (distSq == 0.0) {
            // Query point coincides with this point.
            // Collect all points at the same location and return their average.
            double sum = p.value;
            size_t count = 1;
            for (const auto& q : points) {
                // Skip the already included point (or compare by address/content).
                // Since we need all coincident points, we scan from the start.
                // To avoid double-counting the current one, we check identity.
                // A simpler approach: just accumulate all points with distSq==0.
                // But we've already added p.value, so we need to handle carefully.
                // Easiest: re-scan from beginning and sum all zero-distance points.
            }
            // Let's re-implement this section properly:
            double sumValues = 0.0;
            size_t countZero = 0;
            for (const auto& r : points) {
                double rdx = qx - r.x;
                double rdy = qy - r.y;
                double rdz = qz - r.z;
                if (rdx*rdx + rdy*rdy + rdz*rdz == 0.0) {
                    sumValues += r.value;
                    ++countZero;
                }
            }
            return (countZero > 0) ? (sumValues / static_cast<double>(countZero)) : 0.0;
        }

        double dist = std::sqrt(distSq);
        double w = 1.0 / std::pow(dist, power);
        weightedSum += w * p.value;
        weightSum += w;
    }

    if (weightSum == 0.0) {
        return std::numeric_limits<double>::quiet_NaN(); // All weights zero (shouldn't happen normally)
    }
    return weightedSum / weightSum;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <limits>

// The Point3D struct and function definition should be placed above.
// For brevity, assume they are included.

int main() {
    // Test 1: Basic IDW with two points.
    std::vector<Point3D> pts1 = {
        {0.0, 0.0, 0.0, 10.0},
        {2.0, 0.0, 0.0, 20.0}
    };
    // Query at (1,0,0): distances are 1 and 1, equal weights, average = 15.
    double result1 = distanceWeightedAverage3D(pts1, 1.0, 0.0, 0.0);
    assert(std::fabs(result1 - 15.0) < 1e-9);

    // Test 2: Query exactly at a point returns that point's value.
    std::vector<Point3D> pts2 = {
        {1.0, 2.0, 3.0, 42.0},
        {0.0, 0.0, 0.0, 7.0}
    };
    double result2 = distanceWeightedAverage3D(pts2, 1.0, 2.0, 3.0);
    assert(std::fabs(result2 - 42.0) < 1e-9);

    // Test 3: Multiple points at same location as query → average of their values.
    std::vector<Point3D> pts3 = {
        {0.0, 0.0, 0.0, 10.0},
        {0.0, 0.0, 0.0, 30.0},
        {5.0, 5.0, 5.0, 100.0}
    };
    double result3 = distanceWeightedAverage3D(pts3, 0.0, 0.0, 0.0);
    assert(std::fabs(result3 - 20.0) < 1e-9); // (10+30)/2 = 20

    // Test 4: Empty input returns NaN.
    std::vector<Point3D> pts4;
    double result4 = distanceWeightedAverage3D(pts4, 0.0, 0.0, 0.0);
    assert(std::isnan(result4));

    // Test 5: Power parameter effect – one point close, one far.
    std::vector<Point3D> pts5 = {
        {0.0, 0.0, 0.0, 1.0},
        {10.0, 0.0, 0.0, 100.0}
    };
    // Query at (1,0,0), distances: 1 and 9.
    // With power 2: weights 1 and 1/81 → sum weights ≈ 1.012345679, weighted sum = 1 + 100/81 ≈ 2.234567901
    // result ≈ 2.207... Let's compute precisely: (1 + 100/81) / (1 + 1/81) = (81+100)/81 / (82/81) = (181/81) * (81/82) = 181/82 ≈ 2.207317073...
    double result5 = distanceWeightedAverage3D(pts5, 1.0, 0.0, 0.0, 2.0);
    assert(std::fabs(result5 - (181.0/82.0)) < 1e-9);

    // Test 6: Power 1 (inverse distance) – simple check.
    // Same points as above, query at (1,0,0): distances 1 and 9 → weights 1 and 1/9 → weighted sum = 1 + 100/9 = 109/9 ≈ 12.111..., weight sum = 1 + 1/9 = 10/9 → result = 109/10 = 10.9
    double result6 = distanceWeightedAverage3D(pts5, 1.0, 0.0, 0.0, 1.0);
    assert(std::fabs(result6 - 10.9) < 1e-9);

    // Test 7: All points identical to query location → average of all their values.
    std::vector<Point3D> pts7 = {
        {2.0, 2.0, 2.0, 5.0},
        {2.0, 2.0, 2.0, 7.0},
        {2.0, 2.0, 2.0, 9.0}
    };
    double result7 = distanceWeightedAverage3D(pts7, 2.0, 2.0, 2.0);
    assert(std::fabs(result7 - 7.0) < 1e-9); // (5+7+9)/3 = 7

    return 0;
}
