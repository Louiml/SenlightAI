// Write a C++ function `double closestPointPairDistance(const std::vector<Eigen::Vector2d>& redPoints, const std::vector<Eigen::Vector2d>& bluePoints)` that computes the squared Euclidean distance between the closest pair of points, where one point is taken from `redPoints` and the other from `bluePoints`. The function must use the Eigen BVH (Bounding Volume Hierarchy) library to accelerate the search, and it must return the exact minimum squared distance as a `double`. The input vectors are non-empty, and the function should not modify them. To compute the distance during BVH traversal, define a custom minimizer functor (similar to the code snippet) that counts the number of distance evaluations, but the function itself does not need to expose the count—only return the minimum squared distance. Ensure that the implementation works with Eigen's aligned allocator for point vectors, as `Vector2d` requires alignment for SIMD.

#include <cassert>
#include <cmath>
#include <Eigen/StdVector>
#include <vector>

// Declaration of the function under test (provided from solution)
double closestPointPairDistance(
    const std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>>& redPoints,
    const std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>>& bluePoints);

int main() {
    // Test 1: Simple known distance
    {
        std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>> red = {Eigen::Vector2d(0.0, 0.0)};
        std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>> blue = {Eigen::Vector2d(3.0, 4.0)};
        assert(std::abs(closestPointPairDistance(red, blue) - 25.0) < 1e-12); // 3^2 + 4^2
    }

    // Test 2: Closest pair not at first positions
    {
        std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>> red = {
            Eigen::Vector2d(0.0, 0.0), Eigen::Vector2d(10.0, 10.0)
        };
        std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>> blue = {
            Eigen::Vector2d(5.0, 5.0), Eigen::Vector2d(1.0, 0.0)
        };
        // Closest: (0,0) to (1,0) => distance^2 = 1
        assert(std::abs(closestPointPairDistance(red, blue) - 1.0) < 1e-12);
    }

    // Test 3: Identical points
    {
        std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>> red = {
            Eigen::Vector2d(2.0, -1.0), Eigen::Vector2d(0.0, 0.0)
        };
        std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>> blue = {
            Eigen::Vector2d(0.0, 0.0), Eigen::Vector2d(100.0, 100.0)
        };
        // Closest: (0,0) to (0,0) => distance^2 = 0
        assert(closestPointPairDistance(red, blue) == 0.0);
    }

    // Test 4: Negative coordinates
    {
        std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>> red = {
            Eigen::Vector2d(-1.0, -1.0), Eigen::Vector2d(5.0, 5.0)
        };
        std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>> blue = {
            Eigen::Vector2d(-2.0, -2.0), Eigen::Vector2d(0.0, 0.0)
        };
        // Closest: (-1,-1) to (-2,-2) => distance^2 = 2
        assert(std::abs(closestPointPairDistance(red, blue) - 2.0) < 1e-12);
    }

    // Test 5: Single point in each vector
    {
        std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>> red = {Eigen::Vector2d(1.0, 2.0)};
        std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>> blue = {Eigen::Vector2d(1.0, 3.0)};
        assert(std::abs(closestPointPairDistance(red, blue) - 1.0) < 1e-12);
    }

    // Test 6: Larger sets with a known close pair
    {
        std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>> red;
        std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>> blue;
        for (int i = 0; i < 50; ++i) {
            red.push_back(Eigen::Vector2d(i * 1.0, i * 2.0));
            blue.push_back(Eigen::Vector2d(i * 1.0 + 10.0, i * 2.0 + 10.0));
        }
        // Closest pair is (0,0) and (10,10) => distance^2 = 200
        assert(std::abs(closestPointPairDistance(red, blue) - 200.0) < 1e-10);
    }

    return 0;
}

#include <Eigen/StdVector>
#include <unsupported/Eigen/BVH>
#include <vector>
#include <limits>

// Custom minimizer for closest point pair between two sets of points.
// Computes squared distances between points and bounding boxes.
struct PointPointMinimizer {
    typedef double Scalar;

    double minimumOnVolumeVolume(const Eigen::AlignedBox<double, 2>& r1, const Eigen::AlignedBox<double, 2>& r2) {
        return r1.squaredExteriorDistance(r2);
    }
    double minimumOnVolumeObject(const Eigen::AlignedBox<double, 2>& r, const Eigen::Vector2d& v) {
        return r.squaredExteriorDistance(v);
    }
    double minimumOnObjectVolume(const Eigen::Vector2d& v, const Eigen::AlignedBox<double, 2>& r) {
        return r.squaredExteriorDistance(v);
    }
    double minimumOnObjectObject(const Eigen::Vector2d& v1, const Eigen::Vector2d& v2) {
        return (v1 - v2).squaredNorm();
    }
};

// Compute the squared Euclidean distance between the closest pair of points,
// one from redPoints and one from bluePoints, using BVH acceleration.
double closestPointPairDistance(
    const std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>>& redPoints,
    const std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>>& bluePoints)
{
    // Build BVH trees for both point sets
    Eigen::KdBVH<double, 2, Eigen::Vector2d> redTree(redPoints.begin(), redPoints.end());
    Eigen::KdBVH<double, 2, Eigen::Vector2d> blueTree(bluePoints.begin(), bluePoints.end());

    // Use the minimizer to find the closest pair
    PointPointMinimizer minimizer;
    return Eigen::BVMinimize(redTree, blueTree, minimizer);
}

// The solution leverages Eigen's `KdBVH` (kd-tree-based BVH) to organize both point sets into spatial hierarchies. First, define a functor `PointPointMinimizer` that implements the required methods for BVH traversal: `minimumOnVolumeVolume`, `minimumOnVolumeObject`, `minimumOnObjectVolume`, and `minimumOnObjectObject`. These methods return squared distances: for two bounding boxes, use `squaredExteriorDistance`; for a box and a point, use `squaredExteriorDistance` on the box; for two points, use `(v1 - v2).squaredNorm()`. To construct the BVH, use `KdBVH<double, 2, Vector2d>` and pass iterators from the vectors. Then call `BVMinimize(redTree, blueTree, minimizer)`, which returns the minimal squared distance. Edge cases: both vectors must be non-empty; if one vector has only one point, the BVH still works correctly. The minimizer functor must be default-constructible and define `Scalar` as `double`. The time complexity is approximately O((N+M) log N log M) in practice due to BVH pruning, versus O(N*M) for brute force. Space complexity is O(N+M) for the trees.
