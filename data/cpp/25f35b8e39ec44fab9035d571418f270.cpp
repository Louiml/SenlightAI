// Write a C++ function `bool circleVsAABB(const Vector3& circleCenter, float circleRadius, const Vector3& boxCenter, const Vector3& boxHalfExtents)` that determines whether a sphere (circle) intersects an axis-aligned bounding box (AABB). The box is defined by its center and half-extents along each axis. The function must return `true` if the sphere and box overlap or touch, and `false` otherwise. The `Vector3` type should be a simple struct with `x`, `y`, `z` public members. The function should handle edge cases where the sphere center is inside the box, exactly on the box boundary, or where the sphere only touches a box edge or corner. Provide a standalone implementation without a `main` function, and include necessary headers (`<cmath>` for `fabs`, `<algorithm>` for `std::clamp` or manual clamping).
#include <cassert>

int main() {
    // Sphere inside box
    assert(circleVsAABB(Vector3(0,0,0), 1.0f, Vector3(0,0,0), Vector3(2,2,2)) == true);
    // Sphere entirely outside
    assert(circleVsAABB(Vector3(10,0,0), 1.0f, Vector3(0,0,0), Vector3(2,2,2)) == false);
    // Sphere touching face exactly
    assert(circleVsAABB(Vector3(3,0,0), 1.0f, Vector3(0,0,0), Vector3(2,2,2)) == true);
    // Sphere center on face but radius zero (touching only)
    assert(circleVsAABB(Vector3(2,0,0), 0.0f, Vector3(0,0,0), Vector3(2,2,2)) == true);
    // Sphere near corner, exactly touching corner
    assert(circleVsAABB(Vector3(3,3,3), 1.7320508f, Vector3(0,0,0), Vector3(2,2,2)) == true);
    // Sphere near corner but not touching
    assert(circleVsAABB(Vector3(3,3,3), 1.0f, Vector3(0,0,0), Vector3(2,2,2)) == false);
    // Box is a flat plane (one zero half-extent)
    assert(circleVsAABB(Vector3(0,1,0), 0.5f, Vector3(0,0,0), Vector3(2,0,2)) == true);
    assert(circleVsAABB(Vector3(0,2,0), 0.5f, Vector3(0,0,0), Vector3(2,0,2)) == false);
    // Negative coordinates
    assert(circleVsAABB(Vector3(-3,0,0), 1.0f, Vector3(-2,0,0), Vector3(1,1,1)) == true);
    // Zero radius, center inside box
    assert(circleVsAABB(Vector3(0,0,0), 0.0f, Vector3(0,0,0), Vector3(1,1,1)) == true);
    return 0;
}
#include <cmath>
#include <algorithm>

struct Vector3 {
    float x, y, z;
    Vector3(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f) : x(x_), y(y_), z(z_) {}
};

// Determine if a sphere (circle) intersects an axis-aligned bounding box.
// The box is given by its center and half-extents along each axis.
bool circleVsAABB(const Vector3& circleCenter, float circleRadius,
                  const Vector3& boxCenter, const Vector3& boxHalfExtents) {
    // Compute the box minimum and maximum corners.
    Vector3 boxMin(boxCenter.x - boxHalfExtents.x,
                   boxCenter.y - boxHalfExtents.y,
                   boxCenter.z - boxHalfExtents.z);
    Vector3 boxMax(boxCenter.x + boxHalfExtents.x,
                   boxCenter.y + boxHalfExtents.y,
                   boxCenter.z + boxHalfExtents.z);

    // Clamp the circle center to the box's bounds to get the closest point on the box.
    float closestX = std::clamp(circleCenter.x, boxMin.x, boxMax.x);
    float closestY = std::clamp(circleCenter.y, boxMin.y, boxMax.y);
    float closestZ = std::clamp(circleCenter.z, boxMin.z, boxMax.z);

    // Compute squared distance between sphere center and closest point.
    float dx = circleCenter.x - closestX;
    float dy = circleCenter.y - closestY;
    float dz = circleCenter.z - closestZ;
    float distSq = dx * dx + dy * dy + dz * dz;

    // Intersection if squared distance is at most radius squared.
    return distSq <= circleRadius * circleRadius;
}
// The solution leverages the classic distance-based AABB-sphere collision test. First, find the closest point on the box (including its interior and surface) to the sphere center. For each axis, clamp the sphere’s center coordinate to the box’s [min, max] interval; if the center is less than the min, use min; if greater than max, use max; otherwise keep the coordinate unchanged. The clamped point is exactly the closest point on the box surface or interior to the sphere center. Then compute the squared Euclidean distance between the sphere center and this clamped point. If this squared distance is less than or equal to the sphere’s radius squared, the shapes intersect; otherwise, they do not. Edge cases handled naturally: if the center is inside the box, the distance is zero (since no coordinate changes), so it always intersects. If the center is exactly on a face, edge, or corner, the clamped point equals the center, giving zero distance, so it also intersects. This method is robust and does not require checking individual faces or edges. Time complexity is O(1) with constant number of arithmetic operations, and space complexity O(1). The solution uses floating-point comparisons; because exact equality is possible, using `<=` for comparison is correct. No epsilon is needed for typical game engines, but one could add if desired.
