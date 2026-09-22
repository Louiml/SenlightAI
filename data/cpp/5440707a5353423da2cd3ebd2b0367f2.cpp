Write a C++ function `rayIntersectsAABB` that determines whether a ray (defined by an origin point and a normalized direction vector) intersects an axis-aligned bounding box (defined by its minimum and maximum corner points). The function should use the slab method: for each of the three coordinate axes, compute the interval of ray parameter `t` values where the ray is inside the box along that axis, then intersect the three intervals. The function must handle rays that are parallel to an axis (direction component is zero) by treating the interval as unbounded if the origin is within the slab, or as empty if the origin is outside. Additionally, the function must correctly reject intersections that occur behind the ray origin (negative `t`), and must return a boolean result along with the smallest positive `t` value at which the intersection occurs (if any). The box is assumed to be valid (`min` ≤ `max` on each axis), but the ray direction may not be normalized, so the computed `t` values will be scaled accordingly; the function should work correctly regardless of normalization.

#include <cassert>
#include <cmath>
#include <iostream>

// The Vec3 and rayIntersectsAABB definitions are assumed to be included from above.

int main() {
    // Box from (0,0,0) to (1,1,1)
    Vec3 boxMin{0, 0, 0};
    Vec3 boxMax{1, 1, 1};

    // 1. Ray from outside, hitting the box
    Vec3 origin1{-1, 0.5, 0.5};
    Vec3 dir1{1, 0, 0};
    double dist1;
    assert(rayIntersectsAABB(origin1, dir1, boxMin, boxMax, dist1));
    assert(std::abs(dist1 - 1.0) < 1e-9);

    // 2. Ray from inside, direction away
    Vec3 origin2{0.5, 0.5, 0.5};
    Vec3 dir2{1, 0, 0};
    double dist2;
    assert(rayIntersectsAABB(origin2, dir2, boxMin, boxMax, dist2));
    assert(std::abs(dist2 - 0.0) < 1e-9);

    // 3. Ray missing the box
    Vec3 origin3{-1, -1, -1};
    Vec3 dir3{0, 1, 0};
    double dist3;
    assert(!rayIntersectsAABB(origin3, dir3, boxMin, boxMax, dist3));

    // 4. Ray parallel to y-axis, origin outside slab on y → no hit
    Vec3 origin4{1.5, 0.5, 0.5};
    Vec3 dir4{0, 1, 0};
    double dist4;
    assert(!rayIntersectsAABB(origin4, dir4, boxMin, boxMax, dist4));

    // 5. Ray hitting the box from an angle, with non-normalized direction
    Vec3 origin5{0, 0, 0};
    Vec3 dir5{2, 2, 2}; // length 2√3, not normalized
    double dist5;
    assert(rayIntersectsAABB(origin5, dir5, boxMin, boxMax, dist5));
    // The intersection point is where the ray first exits the box along, e.g., x=1
    // t such that 2t = 1 → t = 0.5
    assert(std::abs(dist5 - 0.5) < 1e-9);

    // 6. Ray pointing away from the box (negative direction)
    Vec3 origin6{-1, 0.5, 0.5};
    Vec3 dir6{-1, 0, 0};
    double dist6;
    assert(!rayIntersectsAABB(origin6, dir6, boxMin, boxMax, dist6));

    // 7. Ray exactly along an edge (touching corner)
    Vec3 origin7{1, 1, 1};
    Vec3 dir7{1, 1, 1};
    double dist7;
    assert(rayIntersectsAABB(origin7, dir7, boxMin, boxMax, dist7));
    // Origin is on the corner, direction away → intersection at t=0
    assert(std::abs(dist7 - 0.0) < 1e-9);

    // 8. Zero direction vector should not crash (treat as no slab constraint, but origin outside → false)
    Vec3 origin8{-1, 0, 0};
    Vec3 dir8{0, 0, 0};
    double dist8;
    assert(!rayIntersectsAABB(origin8, dir8, boxMin, boxMax, dist8));

    // 9. Large box, ray from far away
    Vec3 bigMin{-1000, -1000, -1000};
    Vec3 bigMax{1000, 1000, 1000};
    Vec3 origin9{2000, 0, 0};
    Vec3 dir9{-1, 0, 0};
    double dist9;
    assert(rayIntersectsAABB(origin9, dir9, bigMin, bigMax, dist9));
    assert(std::abs(dist9 - 1000.0) < 1e-9);

    // 10. Ray that passes through box but starts beyond it (behind)
    Vec3 origin10{2, 0.5, 0.5};
    Vec3 dir10{1, 0, 0}; // points away from box, which is behind
    double dist10;
    assert(!rayIntersectsAABB(origin10, dir10, boxMin, boxMax, dist10));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <algorithm>
#include <cmath>
#include <limits>

// Simple 3D vector struct for this task
struct Vec3 {
    double x, y, z;
};

// Return true if ray (origin, direction) intersects AABB (min, max).
// If true, sets hitDist to the smallest non-negative t of intersection.
// Works correctly even if direction is not normalized.
bool rayIntersectsAABB(const Vec3& origin, const Vec3& direction,
                       const Vec3& boxMin, const Vec3& boxMax,
                       double& hitDist) {
    double tNear = -std::numeric_limits<double>::infinity();
    double tFar  =  std::numeric_limits<double>::infinity();

    // Process each axis independently
    for (int axis = 0; axis < 3; ++axis) {
        double o, d, minVal, maxVal;
        if (axis == 0) { o = origin.x; d = direction.x; minVal = boxMin.x; maxVal = boxMax.x; }
        else if (axis == 1) { o = origin.y; d = direction.y; minVal = boxMin.y; maxVal = boxMax.y; }
        else { o = origin.z; d = direction.z; minVal = boxMin.z; maxVal = boxMax.z; }

        if (std::abs(d) < 1e-12) {
            // Ray is parallel to this axis
            if (o < minVal || o > maxVal) {
                return false; // Origin outside slab, no intersection
            }
            // Otherwise, no constraint on t for this axis
        } else {
            double t1 = (minVal - o) / d;
            double t2 = (maxVal - o) / d;
            if (t1 > t2) std::swap(t1, t2);
            tNear = std::max(tNear, t1);
            tFar  = std::min(tFar,  t2);
            if (tNear > tFar) return false; // Early exit if interval empty
        }
    }

    // Check if the intersection is in front of the ray (non-negative t)
    if (tFar < 0.0) return false;

    // The intersection distance is tNear, but clamp to 0 if origin inside box
    hitDist = (tNear < 0.0) ? 0.0 : tNear;
    return true;
}

// The solution uses the standard slab method for ray-AABB intersection. For each axis `i` in {x, y, z}, we compute `t1 = (min[i] - origin[i]) / direction[i]` and `t2 = (max[i] - origin[i]) / direction[i]` (if `direction[i]` is nonzero). If `direction[i]` is zero, then the ray is parallel to that axis: if the origin coordinate is outside the [min, max] interval on that axis, there is no intersection; otherwise that axis imposes no constraint on `t`. For nonzero direction components, we swap `t1` and `t2` if `t1 > t2`, then maintain an overall interval `[tNear, tFar]` by taking `tNear = max(tNear, t1)` and `tFar = min(tFar, t2)`. After processing all three axes, if `tNear <= tFar` and `tFar >= 0`, the ray hits the box. The smallest positive intersection distance is `tNear` (clamped to 0 if the origin is inside the box, which is a valid intersection at `t=0`). Edge cases: (1) zero direction component with origin outside slab → return false immediately; (2) division by zero avoided by branching; (3) negative `t` values are excluded because we require `tFar >= 0`; (4) if the origin is inside the box, `tNear` will be ≤ 0, but `tFar` ≥ 0, so the intersection is valid at `t=0`. Time complexity is O(1) constant time, and space complexity is O(1) auxiliary storage.
