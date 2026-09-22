Write a standalone C++ function named `computeBoundingSphere` that takes an array of 3D points (represented as a `std::vector` of a simple struct with `double x, y, z`) and returns a `BoundingSphere` struct containing the center point and radius of the tight axis-aligned bounding sphere for that point set (i.e., a sphere whose center is the midpoint of the axis-aligned bounding box of the points, and whose radius is the maximum distance from that center to any input point). The input vector will always contain at least one point. Your function must be correct for negative coordinates, duplicate points, and points that are identical; it must return a radius of `0.0` when all points coincide. You may not use any external math libraries—implement the Euclidean distance calculation yourself.
// The approach mirrors the given snippet’s `FromPoints` method: first compute the minimum and maximum coordinates along each axis (x, y, z) across all points to obtain an axis-aligned bounding box. The sphere center is the midpoint of that box: `center = (mins + maxs) / 2.0`. Then iterate through all points, computing the squared Euclidean distance from the center to each point, and keep track of the maximum squared distance. Finally, take the square root to get the radius. This guarantees that every point lies inside or on the sphere, but it is not necessarily the minimal enclosing sphere in the general case—it is a tight sphere for the axis-aligned bounding box, which is exactly what the original code does. Edge cases: a single point results in center equal to that point and radius `0.0`; duplicate points do not change the result; negative coordinates are handled naturally by the min/max logic. Time complexity is two passes over the points: one for the bounding box, one for the radius, so `O(n)` time, where `n` is the number of points. Space complexity is `O(1)` extra, aside from the returned struct.
#include <vector>
#include <cmath>
#include <algorithm>

struct Point3D {
    double x, y, z;
};

struct BoundingSphere {
    double centerX, centerY, centerZ;
    double radius;
};

// Compute a tight axis-aligned bounding sphere for a set of 3D points.
// The sphere's center is the midpoint of the axis-aligned bounding box,
// and the radius is the maximum distance from that center to any point.
BoundingSphere computeBoundingSphere(const std::vector<Point3D>& points) {
    if (points.empty()) {
        return {0.0, 0.0, 0.0, 0.0};
    }

    // Step 1: Compute axis-aligned bounding box (min and max coordinates)
    double minX = points[0].x, maxX = points[0].x;
    double minY = points[0].y, maxY = points[0].y;
    double minZ = points[0].z, maxZ = points[0].z;

    for (const Point3D& p : points) {
        minX = std::min(minX, p.x);
        maxX = std::max(maxX, p.x);
        minY = std::min(minY, p.y);
        maxY = std::max(maxY, p.y);
        minZ = std::min(minZ, p.z);
        maxZ = std::max(maxZ, p.z);
    }

    // Center is midpoint of the bounding box
    double centerX = (minX + maxX) / 2.0;
    double centerY = (minY + maxY) / 2.0;
    double centerZ = (minZ + maxZ) / 2.0;

    // Step 2: Find maximum squared distance from center to any point
    double maxRadiusSqr = 0.0;
    for (const Point3D& p : points) {
        double dx = p.x - centerX;
        double dy = p.y - centerY;
        double dz = p.z - centerZ;
        double distSqr = dx * dx + dy * dy + dz * dz;
        maxRadiusSqr = std::max(maxRadiusSqr, distSqr);
    }

    BoundingSphere result;
    result.centerX = centerX;
    result.centerY = centerY;
    result.centerZ = centerZ;
    result.radius = std::sqrt(maxRadiusSqr);
    return result;
}
#include <cassert>
#include <cmath>

// Helper for approximate equality
bool nearlyEqual(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) < eps;
}

int main() {
    // Single point at origin
    {
        std::vector<Point3D> pts = {{0.0, 0.0, 0.0}};
        BoundingSphere s = computeBoundingSphere(pts);
        assert(nearlyEqual(s.centerX, 0.0) && nearlyEqual(s.centerY, 0.0) && nearlyEqual(s.centerZ, 0.0));
        assert(nearlyEqual(s.radius, 0.0));
    }

    // Two points symmetric around origin
    {
        std::vector<Point3D> pts = {{-1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}};
        BoundingSphere s = computeBoundingSphere(pts);
        assert(nearlyEqual(s.centerX, 0.0) && nearlyEqual(s.centerY, 0.0) && nearlyEqual(s.centerZ, 0.0));
        assert(nearlyEqual(s.radius, 1.0));
    }

    // Cube corners from (0,0,0) to (2,2,2)
    {
        std::vector<Point3D> pts = {
            {0,0,0}, {2,0,0}, {0,2,0}, {0,0,2},
            {2,2,0}, {2,0,2}, {0,2,2}, {2,2,2}
        };
        BoundingSphere s = computeBoundingSphere(pts);
        assert(nearlyEqual(s.centerX, 1.0) && nearlyEqual(s.centerY, 1.0) && nearlyEqual(s.centerZ, 1.0));
        assert(nearlyEqual(s.radius, std::sqrt(3.0)));
    }

    // Negative coordinates and asymmetric points
    {
        std::vector<Point3D> pts = {{-5.0, -3.0, -1.0}, {-5.0, -3.0, -1.0}, {2.0, 4.0, 6.0}};
        BoundingSphere s = computeBoundingSphere(pts);
        assert(nearlyEqual(s.centerX, -1.5)); // (-5+2)/2 = -1.5
        assert(nearlyEqual(s.centerY, 0.5));  // (-3+4)/2 = 0.5
        assert(nearlyEqual(s.centerZ, 2.5));  // (-1+6)/2 = 2.5
        // Distance to (-5,-3,-1): sqrt(3.5^2 + 3.5^2 + 3.5^2) = 3.5*sqrt(3)
        // Distance to (2,4,6): same value
        assert(nearlyEqual(s.radius, 3.5 * std::sqrt(3.0)));
    }

    // All identical points
    {
        std::vector<Point3D> pts = {{7.0, 8.0, 9.0}, {7.0, 8.0, 9.0}, {7.0, 8.0, 9.0}};
        BoundingSphere s = computeBoundingSphere(pts);
        assert(nearlyEqual(s.centerX, 7.0) && nearlyEqual(s.centerY, 8.0) && nearlyEqual(s.centerZ, 9.0));
        assert(nearlyEqual(s.radius, 0.0));
    }

    return 0;
}
