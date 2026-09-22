Write a standalone C++ function that, given a collection of 3D points representing a planar wall and a 3D normal vector, computes the average distance of each point to the plane defined by that normal passing through the centroid of the points, and returns the maximum of these absolute distances (i.e., the maximum perpendicular deviation of any point from the fitted plane). The input is a vector of 3D points (using `Eigen::Vector3d`) and a normal vector `Eigen::Vector3d` (assumed normalized). The function should be named `maxPlaneDeviation` and should return a `double` representing the largest absolute perpendicular distance from any point to the plane. Handle edge cases where the normal has zero length by returning a large sentinel value (e.g., `std::numeric_limits<double>::max()`). The plane is defined by the centroid of the points and the given normal; no fitting is required.

#include <cassert>
#include <cmath>
#include <vector>
#include <Eigen/Core>

// Assume the solution function is declared above.

int main() {
    // Points on the XY plane at z=1, normal along z, centroid at (0,0,1).
    std::vector<Eigen::Vector3d> points1 = {
        Eigen::Vector3d(1.0, 0.0, 1.0),
        Eigen::Vector3d(-1.0, 0.0, 1.0),
        Eigen::Vector3d(0.0, 1.0, 1.0),
        Eigen::Vector3d(0.0, -1.0, 1.0)
    };
    Eigen::Vector3d normal1(0.0, 0.0, 1.0);
    assert(std::abs(maxPlaneDeviation(points1, normal1) - 0.0) < 1e-6);

    // Points with one outlier at z=3, others at z=0. Centroid at z=0.75.
    // Distances: |0-0.75|=0.75, |3-0.75|=2.25 => max 2.25.
    std::vector<Eigen::Vector3d> points2 = {
        Eigen::Vector3d(0.0, 0.0, 0.0),
        Eigen::Vector3d(1.0, 0.0, 0.0),
        Eigen::Vector3d(0.0, 1.0, 0.0),
        Eigen::Vector3d(0.0, 0.0, 3.0)
    };
    Eigen::Vector3d normal2(0.0, 0.0, 1.0);
    assert(std::abs(maxPlaneDeviation(points2, normal2) - 2.25) < 1e-6);

    // Plane tilted: normal along x, points at x=2 and x=0, centroid at (1,0,0).
    // Distances: |2-1|=1, |0-1|=1 => max 1.
    std::vector<Eigen::Vector3d> points3 = {
        Eigen::Vector3d(2.0, 0.0, 0.0),
        Eigen::Vector3d(0.0, 1.0, 2.0)
    };
    Eigen::Vector3d normal3(1.0, 0.0, 0.0);
    assert(std::abs(maxPlaneDeviation(points3, normal3) - 1.0) < 1e-6);

    // Empty points: returns sentinel.
    std::vector<Eigen::Vector3d> empty;
    Eigen::Vector3d normal4(1.0, 0.0, 0.0);
    assert(maxPlaneDeviation(empty, normal4) == std::numeric_limits<double>::max());

    // Zero normal: returns sentinel.
    std::vector<Eigen::Vector3d> points4 = {Eigen::Vector3d(1,2,3)};
    Eigen::Vector3d zeroNormal(0.0, 0.0, 0.0);
    assert(maxPlaneDeviation(points4, zeroNormal) == std::numeric_limits<double>::max());

    // Non-normalized normal: should still work correctly because we normalize internally.
    std::vector<Eigen::Vector3d> points5 = {
        Eigen::Vector3d(0.0, 0.0, 0.0),
        Eigen::Vector3d(0.0, 0.0, 2.0)
    };
    Eigen::Vector3d scaledNormal(0.0, 0.0, 5.0);
    // centroid at (0,0,1), distances: |0-1|=1, |2-1|=1 => max 1.
    assert(std::abs(maxPlaneDeviation(points5, scaledNormal) - 1.0) < 1e-6);

    return 0;
}

#include <Eigen/Core>
#include <vector>
#include <limits>
#include <cmath>

/**
 * @brief Computes the maximum perpendicular distance of 3D points to a plane.
 * 
 * The plane is defined by the given normal vector and passes through the centroid
 * of the points. If the normal is zero or the point vector is empty, returns
 * std::numeric_limits<double>::max().
 * 
 * @param points Vector of 3D points (Eigen::Vector3d).
 * @param normal 3D normal vector (should be normalized, but any non-zero works).
 * @return double Maximum absolute distance from any point to the plane.
 */
double maxPlaneDeviation(const std::vector<Eigen::Vector3d>& points,
                         const Eigen::Vector3d& normal) {
    if (points.empty() || normal.norm() < 1e-12) {
        return std::numeric_limits<double>::max();
    }

    // Normalize the normal vector for accurate distances.
    Eigen::Vector3d n = normal.normalized();

    // Compute centroid.
    Eigen::Vector3d centroid = Eigen::Vector3d::Zero();
    for (const auto& p : points) {
        centroid += p;
    }
    centroid /= static_cast<double>(points.size());

    // Compute maximum absolute distance.
    double maxDist = 0.0;
    for (const auto& p : points) {
        double dist = std::abs(n.dot(p - centroid));
        if (dist > maxDist) {
            maxDist = dist;
        }
    }
    return maxDist;
}

// The solution computes the centroid of all points by averaging their coordinates. The plane equation is then defined by the normal `n` and the centroid `c`: the signed distance of a point `p` to the plane is `n.dot(p - c)`. The absolute value of that dot product is the perpendicular distance. We iterate over all points, compute this distance, and track the maximum. The centroid calculation is essential because the plane must pass through the centroid; using the centroid ensures the sum of signed distances is zero (balanced plane). Edge cases: (1) If the normal is the zero vector, the plane is undefined; we return a sentinel. (2) If the point vector is empty, there are no points; we also return the sentinel or 0? For the given task, we return sentinel for empty or zero normal to clearly signal an invalid input. Time complexity is O(n) for n points, space complexity O(1) beyond input storage. The function uses `Eigen::Vector3d` for points and normal; we must include `<Eigen/Core>` and `<limits>`. The normal should ideally be normalized, but the algorithm works with any non-zero normal; distances scale by the norm, but we assume a unit normal as per specification. For robustness, if the normal is not normalized, we could normalize it internally, but the task says "assumed normalized". We'll still handle non-normalized by normalizing inside to be safe.
