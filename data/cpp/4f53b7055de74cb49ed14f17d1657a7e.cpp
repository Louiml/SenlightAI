Write a C++ function `bool areParallel(const std::vector<std::pair<double,double>>& points)` that accepts four 2D points in the order `p0, p1, p2, p3` and returns `true` if the line segment from `p0` to `p1` is parallel to the line segment from `p2` to `p3`, and `false` otherwise. Two segments are considered parallel if the absolute value of the cross product of their direction vectors is less than or equal to `1e-6`. The input vector always contains exactly four points; each point is a pair of `double` coordinates. The function must handle cases where segment length is zero (i.e., the two endpoints are identical) — in such a case, the segment has no defined direction, and the function should return `true` only if the other segment also has zero length; otherwise `false`. Your implementation must not use global variables, must be thread-safe, and must avoid any floating-point equality comparisons other than the epsilon check.
#include <cassert>
#include <vector>
#include <utility>
#include <cmath>

// The solution function is declared above (assume it is included).

int main() {
    // Case 1: Basic parallel horizontal lines.
    std::vector<std::pair<double,double>> p1 = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}, {1.0, 1.0}};
    assert(areParallel(p1) == true);

    // Case 2: Basic perpendicular lines (should be false).
    std::vector<std::pair<double,double>> p2 = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 0.0}, {0.0, 1.0}};
    assert(areParallel(p2) == false);

    // Case 3: Same direction but different lengths (parallel).
    std::vector<std::pair<double,double>> p3 = {{-1.0, -1.0}, {0.0, 0.0}, {2.0, 2.0}, {5.0, 5.0}};
    assert(areParallel(p3) == true);

    // Case 4: Opposite direction (still parallel).
    std::vector<std::pair<double,double>> p4 = {{0.0, 0.0}, {2.0, 2.0}, {10.0, 10.0}, {5.0, 5.0}};
    assert(areParallel(p4) == true);

    // Case 5: Nearly parallel within tolerance.
    std::vector<std::pair<double,double>> p5 = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 0.0}, {1.0, 1e-7}};
    assert(areParallel(p5) == true);

    // Case 6: Both degenerate (zero length) segments.
    std::vector<std::pair<double,double>> p6 = {{1.0, 2.0}, {1.0, 2.0}, {3.0, 4.0}, {3.0, 4.0}};
    assert(areParallel(p6) == true);

    // Case 7: One degenerate, one non-degenerate.
    std::vector<std::pair<double,double>> p7 = {{1.0, 2.0}, {1.0, 2.0}, {0.0, 0.0}, {1.0, 1.0}};
    assert(areParallel(p7) == false);

    // Case 8: Vertical lines are parallel.
    std::vector<std::pair<double,double>> p8 = {{2.0, -3.0}, {2.0, 5.0}, {-1.0, 0.0}, {-1.0, 10.0}};
    assert(areParallel(p8) == true);

    // Case 9: Slightly non-parallel (cross product > epsilon).
    std::vector<std::pair<double,double>> p9 = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 0.0}, {1.0, 0.001}};
    assert(areParallel(p9) == false);

    // Case 10: Ensure exact crossing at a point is correctly identified as non-parallel.
    std::vector<std::pair<double,double>> p10 = {{0.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}, {1.0, 0.0}};
    assert(areParallel(p10) == false);

    return 0;
}
#include <vector>
#include <utility>
#include <cmath>

// Returns true if the segment from a to b is parallel to the segment from c to d.
// The tolerance for parallel is an absolute cross product <= 1e-6.
bool areParallel(const std::vector<std::pair<double,double>>& points) {
    // Extract the four points in order: points[0]=p0, points[1]=p1, points[2]=p2, points[3]=p3.
    const auto& p0 = points[0];
    const auto& p1 = points[1];
    const auto& p2 = points[2];
    const auto& p3 = points[3];

    // Direction vectors for the two segments.
    const double dx1 = p1.first - p0.first;
    const double dy1 = p1.second - p0.second;
    const double dx2 = p3.first - p2.first;
    const double dy2 = p3.second - p2.second;

    // Check if both segments are degenerate (zero length).
    const bool zero1 = (dx1 == 0.0 && dy1 == 0.0);
    const bool zero2 = (dx2 == 0.0 && dy2 == 0.0);
    if (zero1 && zero2) {
        return true;  // Both are degenerate, trivially parallel.
    }
    if (zero1 || zero2) {
        return false; // Only one degenerate, cannot be parallel.
    }

    // Cross product: dx1 * dy2 - dy1 * dx2.
    const double crossProd = dx1 * dy2 - dy1 * dx2;
    return std::abs(crossProd) <= 1e-6;
}
// The solution computes the direction vector for each segment as the difference between its two endpoints: `v1 = p1 - p0` and `v2 = p3 - p2`. The cross product in 2D is defined as `v1.x * v2.y - v1.y * v2.x`. Two segments are parallel if this cross product is close to zero within an absolute tolerance of `1e-6`. However, special care is needed when either direction vector is the zero vector (i.e., the segment has zero length). If both are zero vectors, the segments are trivially parallel (both degenerate); if only one is zero, they are not parallel because a degenerate segment cannot be assigned a direction. The algorithm is straightforward: compute both vectors, check if they are both zero, then compute the cross product and compare its absolute value with `1e-6`. Time complexity is O(1) since only four points are processed; space complexity is O(1) as we only store a few local doubles. The main edge case is the zero-length segment, which is explicitly handled before the epsilon check.
