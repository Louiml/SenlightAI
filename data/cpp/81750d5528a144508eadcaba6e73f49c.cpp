// Given an array of 3D points (represented as x, y, z coordinates), write a C++ function that counts how many points are within a given horizontal distance threshold (ignoring the y-coordinate) from a target point. The function should return an integer. Use a simple struct `Point3` with `double x, y, z` members. This mirrors the logic of checking which players are within a radius of an enemy's position in the provided code, but simplified to a pure computational task. The horizontal distance is computed as `sqrt(dx*dx + dz*dz)` where `dx = point.x - target.x` and `dz = point.z - target.z`. Only count points where this distance is strictly less than the given threshold.
#include <cassert>

int main() {
    Point3 target = {0.0, 0.0, 0.0};
    std::vector<Point3> points = {
        {1.0, 0.0, 0.0},   // horizontal dist = 1.0
        {0.0, 0.0, 2.0},   // horizontal dist = 2.0
        {-1.0, 5.0, -1.0}, // horizontal dist = sqrt(2) ~ 1.414
        {3.0, 0.0, 4.0}    // horizontal dist = 5.0
    };

    assert(countPointsInRadius(points, target, 1.5) == 2);
    assert(countPointsInRadius(points, target, 2.0) == 3);
    assert(countPointsInRadius(points, target, 5.0) == 3);
    assert(countPointsInRadius(points, target, 0.0) == 0);
    assert(countPointsInRadius({}, target, 10.0) == 0);

    Point3 target2 = {1.0, 0.0, 1.0};
    std::vector<Point3> points2 = {
        {1.0, 0.0, 1.0},   // dist = 0
        {1.0, 0.0, 2.0},   // dist = 1
        {0.0, 0.0, 1.0}    // dist = 1
    };
    assert(countPointsInRadius(points2, target2, 1.0) == 1);
    assert(countPointsInRadius(points2, target2, 0.5) == 1);
}
#include <vector>
#include <cmath>

struct Point3 {
    double x, y, z;
};

// Count points whose horizontal (XZ) distance from target is less than threshold.
int countPointsInRadius(const std::vector<Point3>& points, const Point3& target, double threshold) {
    int count = 0;
    double thresholdSq = threshold * threshold;

    for (const auto& p : points) {
        double dx = p.x - target.x;
        double dz = p.z - target.z;
        double distSq = dx * dx + dz * dz;
        if (distSq < thresholdSq) {
            ++count;
        }
    }

    return count;
}
// The solution involves iterating through every point in the input array and computing the squared horizontal distance to the target. Since the threshold comparison only needs the distance, we can avoid the costly square root by comparing the squared distance against `threshold * threshold`. This is safe because both distances are non-negative. Edge cases include an empty array (return 0), a threshold of zero (only points exactly at the same x,z as the target count, but since comparison is strict `<`, none count unless the distance is 0, which occurs only if dx=0 and dz=0), and points far away. The time complexity is O(n) where n is the number of points, and space complexity is O(1) beyond the input storage.
