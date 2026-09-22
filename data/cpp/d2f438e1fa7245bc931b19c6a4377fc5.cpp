// Given a convex polyhedron represented by a set of oriented planes (each defined by a unit normal and a signed distance from the origin, with the half-space being the side the normal points into), write a C++ function that finds the index of the plane whose normal is most opposite to a given direction vector, considering only planes that are "near" a specified query point. Specifically, implement `int32 FindMostOpposingNearPlane(const std::vector<Plane>& planes, const Vec3& position, const Vec3& dir, float searchDist)`, where `Plane` is a simple struct with `Vec3 normal` and `float distance` (signed distance from origin such that a point `p` is on the plane if `dot(normal, p) - distance == 0`). The function ignores planes whose absolute signed distance from `position` is greater than or equal to `searchDist`; among the remaining candidates, it returns the index of the plane with the smallest dot product between its normal and `dir`. If no plane satisfies the distance condition, return `-1`. Assume `dir` is normalized, but handle any length gracefully. You may assume `planes` is non-empty, and the normals are unit length.
The algorithm iterates over all planes once, computing the signed distance of the query point to each plane using `dot(normal, position) - distance`. For each plane where the absolute value of this distance is strictly less than `searchDist`, we compute the dot product between the plane's normal and the direction vector `dir`. We track the minimum dot product and the corresponding index, initializing the best index to `-1` and the best dot to `+infinity`. At the end, return the best index (which will be `-1` if no plane matched). Edge cases: if `searchDist` is zero or negative, no plane will satisfy because we require strict `< searchDist`, so the function correctly returns `-1`. If multiple planes have the same dot product, the first one encountered is returned (since we use strict `<` when updating). The direction vector does not need to be normalized because the comparison is affine — scaling `dir` scales all dot products by the same positive factor, preserving the ordering. Time complexity is O(N) where N is the number of planes; space usage is O(1) auxiliary.
#include <vector>
#include <limits>

struct Vec3 {
    float x, y, z;
};

struct Plane {
    Vec3 normal;   // unit normal, oriented outward
    float distance; // signed distance from origin: dot(normal, p) == distance for points on the plane
};

float dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float signedDistance(const Plane& p, const Vec3& point) {
    return dot(p.normal, point) - p.distance;
}

// Returns the index of the plane whose normal is most opposite to `dir` among
// planes within `searchDist` of `position`. Returns -1 if no plane qualifies.
int findMostOpposingNearPlane(const std::vector<Plane>& planes, const Vec3& position, const Vec3& dir, float searchDist) {
    int bestIndex = -1;
    float bestDot = std::numeric_limits<float>::max();

    for (int i = 0; i < static_cast<int>(planes.size()); ++i) {
        const float dist = signedDistance(planes[i], position);
        if (std::abs(dist) < searchDist) {
            const float d = dot(planes[i].normal, dir);
            if (d < bestDot) {
                bestDot = d;
                bestIndex = i;
            }
        }
    }

    return bestIndex;
}
#include <cassert>
#include <cmath>

int main() {
    // Simple cube centered at origin, planes at x=±1, y=±1, z=±1
    std::vector<Plane> cube = {
        {{1,0,0}, 1},   // +x
        {{-1,0,0}, 1},  // -x
        {{0,1,0}, 1},   // +y
        {{0,-1,0}, 1},  // -y
        {{0,0,1}, 1},   // +z
        {{0,0,-1}, 1}   // -z
    };

    // Query point inside the cube, direction pointing +x
    // All planes are within distance 1, but most opposing to +x is -x plane (index 1)
    Vec3 pos = {0,0,0};
    Vec3 dir = {1,0,0};
    assert(findMostOpposingNearPlane(cube, pos, dir, 2.0f) == 1);

    // Direction +z: most opposing is -z (index 5)
    dir = {0,0,1};
    assert(findMostOpposingNearPlane(cube, pos, dir, 2.0f) == 5);

    // SearchDist small enough to exclude far planes: only +x plane near point (1,0,0)
    Vec3 posNearX = {0.9f, 0, 0};
    dir = {-1,0,0}; // Should pick +x plane (index 0) since it's the only one within 0.2 distance
    assert(findMostOpposingNearPlane(cube, posNearX, dir, 0.2f) == 0);

    // No plane within searchDist: return -1
    dir = {1,0,0};
    assert(findMostOpposingNearPlane(cube, pos, dir, 0.1f) == -1);

    // Edge: searchDist exactly equal to distance does not count (strict <)
    assert(findMostOpposingNearPlane(cube, pos, dir, 1.0f) == -1);

    // Non-normalized dir: same result because scaling doesn't change ordering
    dir = {2,0,0};
    assert(findMostOpposingNearPlane(cube, pos, dir, 2.0f) == 1);

    // Degenerate: zero searchDist returns -1
    assert(findMostOpposingNearPlane(cube, pos, {1,0,0}, 0.0f) == -1);

    // Planes with duplicate normals: first is chosen when equal
    std::vector<Plane> custom = {{{1,0,0}, 1}, {{1,0,0}, 0.5f}, {{-1,0,0}, 1}};
    // Both +x planes are near position (0,0,0), have same dot with -x direction? Actually direction +x: both have dot 1, direction -x: both -1, so first returns index 0
    assert(findMostOpposingNearPlane(custom, {0,0,0}, {-1,0,0}, 2.0f) == 0);

    return 0;
}
