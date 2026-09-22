// Write a standalone C++ function that computes the axis-aligned bounding box (AABB) of a set of 3D points, where each point is represented as a fixed-size vector type `Vec3` (with `double` components). The function must accept a `std::vector<Vec3>` and return a `BBox` struct containing `min` and `max` corners (both `Vec3`). The input is non-empty, may contain duplicate points, and coordinates can be negative, zero, or positive. The function must handle potential `NaN` or infinity values gracefully by ignoring them (i.e., they should not affect the bounds), and if all points contain invalid values, return an all-zero `BBox`. Provide a reference solution and test code using assertions that verify the correctness of the bounds.

#include <cassert>
#include <cmath>

// Vec3 and BBox definitions from solution are assumed included.
// Test the computeBoundingBox function.
int main() {
    // Basic case with positive and negative coordinates.
    std::vector<Vec3> pts1 = {{1.0, 2.0, 3.0}, {-1.0, -2.0, -3.0}, {0.5, 0.5, 0.5}};
    BBox b1 = computeBoundingBox(pts1);
    assert(b1.min.x == -1.0 && b1.min.y == -2.0 && b1.min.z == -3.0);
    assert(b1.max.x == 1.0 && b1.max.y == 2.0 && b1.max.z == 3.0);

    // Duplicate points and all same values.
    std::vector<Vec3> pts2 = {{5.0, 5.0, 5.0}, {5.0, 5.0, 5.0}, {5.0, 5.0, 5.0}};
    BBox b2 = computeBoundingBox(pts2);
    assert(b2.min.x == 5.0 && b2.min.y == 5.0 && b2.min.z == 5.0);
    assert(b2.max.x == 5.0 && b2.max.y == 5.0 && b2.max.z == 5.0);

    // Single point.
    std::vector<Vec3> pts3 = {{-7.5, 3.25, 0.0}};
    BBox b3 = computeBoundingBox(pts3);
    assert(b3.min.x == -7.5 && b3.min.y == 3.25 && b3.min.z == 0.0);
    assert(b3.max.x == -7.5 && b3.max.y == 3.25 && b3.max.z == 0.0);

    // Points with NaN and infinity should be ignored.
    std::vector<Vec3> pts4 = {{NAN, 1.0, 2.0}, {3.0, INFINITY, 4.0}, {1.0, 2.0, 3.0}};
    BBox b4 = computeBoundingBox(pts4);
    assert(b4.min.x == 1.0 && b4.min.y == 2.0 && b4.min.z == 3.0);
    assert(b4.max.x == 1.0 && b4.max.y == 2.0 && b4.max.z == 3.0);

    // All points invalid -> zero box.
    std::vector<Vec3> pts5 = {{NAN, NAN, NAN}, {INFINITY, INFINITY, INFINITY}};
    BBox b5 = computeBoundingBox(pts5);
    assert(b5.min.x == 0.0 && b5.min.y == 0.0 && b5.min.z == 0.0);
    assert(b5.max.x == 0.0 && b5.max.y == 0.0 && b5.max.z == 0.0);

    // Mixed order and negative infinity (should be ignored).
    std::vector<Vec3> pts6 = {{2.0, 1.0, -1.0}, {-3.0, -2.0, 4.0}, {0.5, 0.0, 0.0}, {-INFINITY, 0.0, 0.0}};
    BBox b6 = computeBoundingBox(pts6);
    assert(b6.min.x == -3.0 && b6.min.y == -2.0 && b6.min.z == -1.0);
    assert(b6.max.x == 2.0 && b6.max.y == 1.0 && b6.max.z == 4.0);

    return 0;
}

#include <vector>
#include <cmath>
#include <limits>

// Simple 3D vector type with double components.
struct Vec3 {
    double x, y, z;

    Vec3(double x_ = 0.0, double y_ = 0.0, double z_ = 0.0) : x(x_), y(y_), z(z_) {}

    bool isFinite() const {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
    }
};

// Bounding box structure storing minimum and maximum corners.
struct BBox {
    Vec3 min;
    Vec3 max;

    BBox() : min(), max() {}
};

// Compute the axis-aligned bounding box of a set of 3D points.
// Ignores points with NaN or infinite components. If no valid points exist, returns zero box.
BBox computeBoundingBox(const std::vector<Vec3>& points) {
    BBox result;

    // Find first valid point to initialize.
    bool initialized = false;
    for (const Vec3& p : points) {
        if (p.isFinite()) {
            result.min = p;
            result.max = p;
            initialized = true;
            break;
        }
    }

    // If no valid points, return zero box.
    if (!initialized) {
        return result;
    }

    // Update bounds with remaining valid points.
    for (const Vec3& p : points) {
        if (!p.isFinite()) {
            continue;
        }
        result.min.x = std::min(result.min.x, p.x);
        result.min.y = std::min(result.min.y, p.y);
        result.min.z = std::min(result.min.z, p.z);

        result.max.x = std::max(result.max.x, p.x);
        result.max.y = std::max(result.max.y, p.y);
        result.max.z = std::max(result.max.z, p.z);
    }

    return result;
}

// The solution is straightforward: initialize `min` and `max` to the first valid point (or to extreme values if first point is invalid). Then iterate through all points, skipping any with non-finite components (using `std::isfinite`), and update `min` and `max` component-wise using `std::min` and `std::max`. If no valid points exist, return a zero-initialized `BBox`. The time complexity is O(n) where n is the number of points, and space complexity is O(1) auxiliary. Edge cases include: negative coordinates (handled naturally), duplicates (no effect), empty input (but task guarantees non-empty; still handle defensively), and NaN/inf (must be filtered out). The `Vec3` type is a simple struct with `x`, `y`, `z` members and operators for comparison or direct access.
