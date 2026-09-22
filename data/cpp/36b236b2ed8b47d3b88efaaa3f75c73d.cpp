Write a C++ function `std::pair<btVector3, btVector3> projectConvexShape(const btTransform& trans, const btVector3& dir, const btVector3& localSupportFn(const btVector3&))` that, given a convex shape's transform, a direction `dir`, and a callback `localSupportFn` returning the support point of the shape in local coordinates along a local direction, computes the projection interval of the shape onto `dir` in world space. The function should return the two extreme world-space points: the point with minimal projection and the point with maximal projection. The direction `dir` is assumed to be a unit-length vector (normalized). If the shape's support function is symmetric (e.g., a sphere), the two returned points may be the same if the projection interval collapses to a single point. The function must correctly handle the case where the support function returns points such that the minimal projection value is greater than the maximal value (e.g., due to numerical error), in which case the two points should be swapped to ensure the first returned point is the minimum and the second is the maximum. The input transform `trans` includes both a rotation basis and a translation origin; the support point is computed in local coordinates, transformed to world, and then projected onto `dir`. The function should use `const` appropriately and not modify the shape or callback.

// The algorithm is straightforward: first compute the local direction by transforming `dir` into the shape's local coordinate frame. Since `dir` is a world-space direction, the local direction is `dir * trans.getBasis()` (i.e., the transpose of the rotation matrix multiplied by `dir`). Then call the provided support function twice: once with the local direction to get the maximum support point and once with its negation to get the minimum support point. Transform both support points to world space using the transform (rotation plus translation). Compute the projection of the world-space points onto `dir` using the dot product. The point with the smaller dot product is the minimum, the one with the larger is the maximum. However, if the dot product of the "min" candidate is greater than that of the "max" candidate (due to asymmetry or numerical issues), swap the two points so the result is ordered correctly. Edge case: if the support function returns a degenerate shape (e.g., all points identical), both projections equal and no swap is needed. Time complexity is O(1) as only a constant number of vector operations and two support calls are made; space complexity is O(1) beyond the inputs. The function is generic and does not depend on the actual shape type, making it reusable.

#include <utility> // for std::pair

// A simple 3D vector class with required operations. In practice, use btVector3.
struct btVector3 {
    double x, y, z;
    btVector3(double x_ = 0, double y_ = 0, double z_ = 0) : x(x_), y(y_), z(z_) {}
    btVector3 operator*(double s) const { return btVector3(x*s, y*s, z*s); }
    btVector3 operator+(const btVector3& o) const { return btVector3(x+o.x, y+o.y, z+o.z); }
    double dot(const btVector3& o) const { return x*o.x + y*o.y + z*o.z; }
};

// A minimal transform: rotation basis (as 3x3) and origin.
struct btTransform {
    btVector3 basis[3]; // rows? For simplicity, treat as three column vectors? We'll assume basis[0], [1], [2] are the columns.
    btVector3 origin;
    btVector3 operator*(const btVector3& v) const {
        // Apply rotation: v' = R * v, where R columns are basis[0], basis[1], basis[2].
        btVector3 r;
        r.x = basis[0].x * v.x + basis[1].x * v.y + basis[2].x * v.z;
        r.y = basis[0].y * v.x + basis[1].y * v.y + basis[2].y * v.z;
        r.z = basis[0].z * v.x + basis[1].z * v.y + basis[2].z * v.z;
        return r + origin;
    }
    btVector3 getBasis() const { return btVector3(); } // Not used; we define multiply as operator*.
};

// The solution function: project a convex shape onto a direction.
// localSupportFn: given a local direction (unit vector), returns the support point in local coordinates.
std::pair<btVector3, btVector3> projectConvexShape(
    const btTransform& trans, 
    const btVector3& dir, 
    btVector3 (*localSupportFn)(const btVector3& localDir))
{
    // Transform direction to local frame: since R is a rotation, localDir = R^T * dir.
    // With our btTransform, we need to compute R^T * dir. 
    // For simplicity, we assume basis columns are orthonormal; we compute using dot products.
    btVector3 localDir(
        dir.dot(trans.basis[0]),
        dir.dot(trans.basis[1]),
        dir.dot(trans.basis[2])
    );

    // Get support points in local coordinates.
    btVector3 localMax = localSupportFn(localDir);
    btVector3 localMin = localSupportFn(localDir * -1.0);

    // Transform to world.
    btVector3 worldMax = trans * localMax;
    btVector3 worldMin = trans * localMin;

    // Compute projected values.
    double projMax = worldMax.dot(dir);
    double projMin = worldMin.dot(dir);

    // Ensure ordering: first is min, second is max.
    if (projMin > projMax) {
        std::swap(worldMin, worldMax);
    }

    return {worldMin, worldMax};
}

#include <cassert>
#include <cmath>

// Minimal support function for a sphere of radius 1 centered at origin.
btVector3 sphereSupport(const btVector3& dir) {
    double len = std::sqrt(dir.x*dir.x + dir.y*dir.y + dir.z*dir.z);
    if (len < 1e-9) return btVector3(0,0,0);
    return dir * (1.0 / len); // unit vector
}

// Support function for a point cloud (e.g., a triangle) - simple convex hull.
btVector3 triangleSupport(const btVector3& dir) {
    // Triangle vertices in local coords.
    static const btVector3 verts[3] = {
        btVector3(1,0,0), btVector3(0,1,0), btVector3(0,0,1)
    };
    double maxDot = -1e9;
    btVector3 best = verts[0];
    for (int i = 0; i < 3; ++i) {
        double d = verts[i].dot(dir);
        if (d > maxDot) { maxDot = d; best = verts[i]; }
    }
    return best;
}

int main() {
    // Identity transform.
    btTransform id;
    id.basis[0] = btVector3(1,0,0);
    id.basis[1] = btVector3(0,1,0);
    id.basis[2] = btVector3(0,0,1);
    id.origin = btVector3(0,0,0);

    // Test sphere radius 1, project along X axis.
    auto res = projectConvexShape(id, btVector3(1,0,0), sphereSupport);
    assert(fabs(res.first.x - (-1.0)) < 1e-6); // min at -1 in x
    assert(fabs(res.second.x - 1.0) < 1e-6);   // max at +1 in x
    assert(fabs(res.first.y - 0.0) < 1e-6 && fabs(res.first.z - 0.0) < 1e-6);
    assert(fabs(res.second.y - 0.0) < 1e-6 && fabs(res.second.z - 0.0) < 1e-6);

    // Project along Y axis, should get min at (0,-1,0) and max at (0,1,0).
    res = projectConvexShape(id, btVector3(0,1,0), sphereSupport);
    assert(fabs(res.first.y - (-1.0)) < 1e-6);
    assert(fabs(res.second.y - 1.0) < 1e-6);

    // Translate sphere by (2,3,4), project along X.
    id.origin = btVector3(2,3,4);
    res = projectConvexShape(id, btVector3(1,0,0), sphereSupport);
    assert(fabs(res.first.x - 1.0) < 1e-6); // 2-1=1
    assert(fabs(res.second.x - 3.0) < 1e-6); // 2+1=3

    // Test triangle shape (vertices (1,0,0),(0,1,0),(0,0,1)), identity transform.
    id.origin = btVector3(0,0,0);
    res = projectConvexShape(id, btVector3(1,0,0), triangleSupport);
    // Min point: vertex with smallest x: (0,1,0) or (0,0,1) both have x=0, dot=0. Max: (1,0,0) dot=1.
    assert(fabs(res.first.x - 0.0) < 1e-6);
    assert(fabs(res.second.x - 1.0) < 1e-6);
    assert(fabs(res.second.y - 0.0) < 1e-6 && fabs(res.second.z - 0.0) < 1e-6);

    // Test with negative direction.
    res = projectConvexShape(id, btVector3(-1,0,0), triangleSupport);
    // Min is now max? Actually support along -x gives vertex with largest x? No, support along -x gives vertex with smallest x, which is (0,1,0) or (0,0,1). The projection min is -1 (from (1,0,0) dot -1 = -1), max is 0.
    // Our function should return first = min = (1,0,0) with dot = -1, second = max = (0,1,0) with dot = 0.
    assert(fabs(res.first.x - 1.0) < 1e-6);
    assert(fabs(res.second.x - 0.0) < 1e-6);

    // Test with rotation: rotate 90 degrees around Z axis, so X becomes Y.
    btTransform rot;
    rot.basis[0] = btVector3(0,1,0);
    rot.basis[1] = btVector3(-1,0,0);
    rot.basis[2] = btVector3(0,0,1);
    rot.origin = btVector3(0,0,0);
    res = projectConvexShape(rot, btVector3(1,0,0), sphereSupport);
    // Sphere is symmetric, still should give min/max along Y now.
    assert(fabs(res.first.y - (-1.0)) < 1e-6);
    assert(fabs(res.second.y - 1.0) < 1e-6);

    return 0;
}
