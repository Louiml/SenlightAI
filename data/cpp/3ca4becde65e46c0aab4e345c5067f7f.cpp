// Write a self-contained C++ function `frustumCullsAxisAlignedBox` that, given a frustum defined by an origin, a forward axis, a horizontal half-angle, and distances to the near and far planes (in a coordinate system where the frustum apex is at the origin and the view direction is along +X), determines whether an axis-aligned bounding box (specified by a center and half-extents) is completely outside that frustum. The function must return `true` if the box is fully outside the frustum (i.e., culled) and `false` if the box might intersect or lie inside the frustum. The frustum is symmetric about the X-axis with a square cross-section (equal horizontal and vertical extents at any depth), so you may use a single half-angle parameter. Use a separating-axis strategy that tests only the four side planes of the frustum (near and far planes are not required for this task), handling the general case where the bounding box is axis-aligned in world space. You may assume the frustum's forward axis is normalized. Provide a robust numerical epsilon (e.g., `1e-6`) for comparisons to avoid precision artifacts.

// The key idea is to use the separating axis theorem: if any of the frustum's side planes separates the box completely (all box vertices on the outside of that plane), the box is culled. Since the frustum is symmetric and the box is axis-aligned, we can transform the frustum into the box's local space (or equivalently the box into frustum space) and test the four side plane equations efficiently.
//
// Because the box is axis-aligned, we work in the box's local coordinate system. The frustum origin and forward axis are transformed into that space. The four side planes of the frustum can be defined in terms of the forward direction and the half-angle: for a plane with normal `n`, the plane is `n · p ≤ 0` for points inside the frustum (with the apex at origin and opening toward +X). For such a plane, the maximum signed distance of the box (center `c`, half-extents `e`) along `n` is `value = dot(n, c) + sum_i |n[i]| * e[i]`. If this maximum is still less than `-epsilon`, all box vertices are on the outside of that plane, so the box is fully on the other side and can be culled. We must test all four side planes (left, right, up, down). Since the frustum is symmetric in Y and Z relative to the forward axis, the four plane normals are obtained by taking the forward direction and adding/subtracting appropriately scaled perpendicular directions, but an easier approach is to test the two axes perpendicular to the forward axis in the frustum's local frame. In the frustum's local coordinate frame (origin at frustum apex, forward = +X, horizontal = Y, vertical = Z), the four side planes have normals proportional to `( -dLeft, ±dFar, 0 )` and `( -dUp, 0, ±dFar )` where `dFar` is the far distance and `dLeft = dUp = tan(halfAngle) * dFar`. But a simpler and numerically robust way: compute the frustum's right and up vectors from the forward axis (using an arbitrary perpendicular basis), then define four plane normals as `right - forward*tanHalfAngle`, `-right - forward*tanHalfAngle`, `up - forward*tanHalfAngle`, `-up - forward*tanHalfAngle`. These normals point outward from the frustum sides. For a point `p` in world coordinates, the plane equation is `dot(n, p - origin) ≤ 0` for inside. For an AABB, we need the maximum of `dot(n, p - origin)` over the box. This maximum is `dot(n, c - origin) + sum_i |n_i| * e_i`. If this maximum is less than `-epsilon`, all vertices are outside the plane, so the box is culled by that plane. If any of the four side planes culls the box, return `true`; otherwise `false`. The main edge case is when the box is behind the frustum apex or very close to a plane; the epsilon helps avoid false positives due to floating-point errors. Time complexity is O(1) because only a constant number of operations (four plane tests) are performed, independent of any box size or frustum configuration. Space complexity is O(1).

#include <cmath>
#include <limits>

// Simple 3D vector and matrix types for clarity in this standalone task.
struct Vec3 {
    double x, y, z;
    Vec3(double x_ = 0.0, double y_ = 0.0, double z_ = 0.0) : x(x_), y(y_), z(z_) {}
};

inline Vec3 operator-(const Vec3& a, const Vec3& b) { return Vec3(a.x-b.x, a.y-b.y, a.z-b.z); }
inline Vec3 operator+(const Vec3& a, const Vec3& b) { return Vec3(a.x+b.x, a.y+b.y, a.z+b.z); }
inline Vec3 operator*(const Vec3& a, double s) { return Vec3(a.x*s, a.y*s, a.z*s); }
inline Vec3 operator/(const Vec3& a, double s) { return Vec3(a.x/s, a.y/s, a.z/s); }
inline double dot(const Vec3& a, const Vec3& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return Vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}
inline double length(const Vec3& v) { return std::sqrt(dot(v, v)); }
inline Vec3 normalize(const Vec3& v) { double len = length(v); return (len > 1e-12) ? v / len : Vec3(0,0,0); }
inline Vec3 abs(const Vec3& v) { return Vec3(std::fabs(v.x), std::fabs(v.y), std::fabs(v.z)); }

// Frustum parameters:
// origin: the apex of the frustum (in world space)
// forward: normalized direction of the frustum axis (from apex toward far plane)
// halfAngle: half of the total opening angle (in radians) for both X and Y directions
// nearDist: distance from apex to near plane (not used in this culling test)
// farDist: distance from apex to far plane (used for scaling the side planes)
//
// The bounding box is axis-aligned in world space:
// boxCenter: center of the box
// boxExtents: half-extents along X, Y, Z (non-negative)
//
// Returns true if the box is completely outside the frustum due to any of the four side planes.
// The near and far planes are intentionally ignored per the task specification.
bool frustumCullsAxisAlignedBox(
    const Vec3& origin,
    const Vec3& forward,
    double halfAngle,
    double /*nearDist*/,
    double farDist,
    const Vec3& boxCenter,
    const Vec3& boxExtents
) {
    const double epsilon = 1e-6;
    const double tanHalf = std::tan(halfAngle);

    // Build an orthonormal basis with forward as the X axis.
    // Choose up as an arbitrary vector not parallel to forward.
    Vec3 up;
    if (std::fabs(forward.x) < 0.9) {
        up = Vec3(0.0, 0.0, 1.0);
    } else {
        up = Vec3(0.0, 1.0, 0.0);
    }
    Vec3 right = normalize(cross(forward, up));
    up = normalize(cross(right, forward)); // ensure orthonormal

    // The four side plane normals (pointing outward from the frustum sides).
    // For a side plane, a point p satisfies dot(n, p - origin) <= 0 for inside.
    // We test if the entire box lies on the outside (positive) side of any plane.
    Vec3 normals[4];
    // right side: n = right - forward * tanHalf
    normals[0] = right - forward * tanHalf;
    // left side: n = -right - forward * tanHalf
    normals[1] = (right * -1.0) - forward * tanHalf;
    // up side: n = up - forward * tanHalf
    normals[2] = up - forward * tanHalf;
    // down side: n = -up - forward * tanHalf
    normals[3] = (up * -1.0) - forward * tanHalf;

    for (int i = 0; i < 4; ++i) {
        Vec3 n = normals[i];
        // Maximum of dot(n, p - origin) over the box is:
        // dot(n, boxCenter - origin) + sum |n_j| * boxExtents_j
        Vec3 centerOffset = boxCenter - origin;
        double maxSignedDistance = dot(n, centerOffset);
        Vec3 absN = abs(n);
        maxSignedDistance += absN.x * boxExtents.x + absN.y * boxExtents.y + absN.z * boxExtents.z;

        // If even the maximum signed distance is negative (with a small epsilon),
        // then all box vertices are on the outside side of this plane, so the box is culled.
        if (maxSignedDistance < -epsilon) {
            return true;
        }
    }
    return false;
}

#include <cassert>
#include <cmath>
#include <iostream>

// The Vec3 type and frustumCullsAxisAlignedBox function are assumed to be defined above.
// For brevity, they are not redefined here, but in a real project they would be included.

int main() {
    const double pi = 3.14159265358979323846;

    // Simple frustum: origin at (0,0,0), looking along +X with 45-degree half-angle (tan = 1).
    Vec3 origin(0, 0, 0);
    Vec3 forward(1, 0, 0);
    double halfAngle = pi / 4.0; // tan(45°) = 1
    double nearDist = 1.0;
    double farDist = 10.0;

    // Box directly in front, inside the frustum.
    assert(frustumCullsAxisAlignedBox(origin, forward, halfAngle, nearDist, farDist,
                                      Vec3(5, 0, 0), Vec3(0.5, 0.5, 0.5)) == false);

    // Box far to the right, outside.
    assert(frustumCullsAxisAlignedBox(origin, forward, halfAngle, nearDist, farDist,
                                      Vec3(5, 100, 0), Vec3(0.5, 0.5, 0.5)) == true);

    // Box far to the left, outside.
    assert(frustumCullsAxisAlignedBox(origin, forward, halfAngle, nearDist, farDist,
                                      Vec3(5, -100, 0), Vec3(0.5, 0.5, 0.5)) == true);

    // Box far above, outside.
    assert(frustumCullsAxisAlignedBox(origin, forward, halfAngle, nearDist, farDist,
                                      Vec3(5, 0, 100), Vec3(0.5, 0.5, 0.5)) == true);

    // Box far below, outside.
    assert(frustumCullsAxisAlignedBox(origin, forward, halfAngle, nearDist, farDist,
                                      Vec3(5, 0, -100), Vec3(0.5, 0.5, 0.5)) == true);

    // Box on the boundary: large box centered at origin (should not be culled because it straddles the apex).
    assert(frustumCullsAxisAlignedBox(origin, forward, halfAngle, nearDist, farDist,
                                      Vec3(0, 0, 0), Vec3(1, 1, 1)) == false);

    // Box near the right edge but still partially inside.
    assert(frustumCullsAxisAlignedBox(origin, forward, halfAngle, nearDist, farDist,
                                      Vec3(5, 4.5, 0), Vec3(1, 0.5, 0.5)) == false);

    // Box fully outside but very close to the edge (testing epsilon robustness).
    assert(frustumCullsAxisAlignedBox(origin, forward, halfAngle, nearDist, farDist,
                                      Vec3(5, 10.0 - 1e-8, 0), Vec3(0.1, 0.1, 0.1)) == true);

    // Frustum pointing up (forward = +Y).
    Vec3 forwardUp(0, 1, 0);
    assert(frustumCullsAxisAlignedBox(origin, forwardUp, halfAngle, nearDist, farDist,
                                      Vec3(5, 5, 0), Vec3(0.5, 0.5, 0.5)) == false);
    assert(frustumCullsAxisAlignedBox(origin, forwardUp, halfAngle, nearDist, farDist,
                                      Vec3(0, 5, 100), Vec3(0.5, 0.5, 0.5)) == true);

    // Frustum with a very narrow angle: tan(5°) ≈ 0.0875.
    double narrowAngle = 5.0 * pi / 180.0;
    assert(frustumCullsAxisAlignedBox(origin, forward, narrowAngle, nearDist, farDist,
                                      Vec3(5, 1, 0), Vec3(0.2, 0.2, 0.2)) == false);
    assert(frustumCullsAxisAlignedBox(origin, forward, narrowAngle, nearDist, farDist,
                                      Vec3(5, 5, 0), Vec3(0.2, 0.2, 0.2)) == true);

    std::cout << "All tests passed." << std::endl;
    return 0;
}
