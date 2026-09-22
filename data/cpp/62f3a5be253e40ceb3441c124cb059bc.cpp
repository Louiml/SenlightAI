/*
Write a C++ function named `firstIntersectionT` that takes an axis-aligned 3D bounding box (represented by two `Point3D` corners: minimum and maximum) and a ray (represented by an origin `Point3D` and a direction `Point3D`), and returns the smallest positive `t` value (along the ray parameterization `origin + t * direction`) at which the ray hits the box, or `-1.0` if there is no intersection. The box is defined with its two opposite corners, where `p[0]` has all coordinates less than or equal to `p[1]`. The ray direction is not normalized, so `t` is measured in units consistent with the direction vector. Handle cases where the ray is parallel to a box face (i.e., direction component is zero) by treating the ray as intersecting only if the origin coordinate is already within that face's slab. Ignore intersections behind the ray origin (negative `t` values). Since the box is axis-aligned, use the slab method: compute candidates for `t` at each pair of parallel faces, take the maximum of the three "near" values and the minimum of the three "far" values; if the near max is less than the far min and the maximum near value is non-negative, that maximum near value is the entry point. Return `-1` otherwise. Include necessary headers and use `const` references where appropriate. The function should not modify the input data.
*/

#include <algorithm>
#include <limits>

// Simple 3D point type with array access
struct Point3D {
    double p[3];
    double& operator[](int i) { return p[i]; }
    const double& operator[](int i) const { return p[i]; }
};

// Ray definition: origin and direction
struct Ray3D {
    Point3D origin;
    Point3D direction;
};

// Axis-aligned bounding box defined by two corners: p[0] = min, p[1] = max
struct BoundingBox3D {
    Point3D p[2];
};

// Return the smallest non-negative t such that ray hits the box, or -1 if no hit
double firstIntersectionT(const BoundingBox3D& box, const Ray3D& ray) {
    const double epsilon = 1e-6;
    double t_enter = 0.0;
    double t_exit = std::numeric_limits<double>::max();

    for (int axis = 0; axis < 3; ++axis) {
        double origin = ray.origin[axis];
        double direction = ray.direction[axis];
        double min_bound = box.p[0][axis];
        double max_bound = box.p[1][axis];

        if (std::abs(direction) < epsilon) {
            // Ray is parallel to this axis's slab
            if (origin < min_bound - epsilon || origin > max_bound + epsilon) {
                return -1; // Outside the slab, no intersection
            }
            // Otherwise, this axis does not constrain t
        } else {
            double t1 = (min_bound - origin) / direction;
            double t2 = (max_bound - origin) / direction;
            double near_t = std::min(t1, t2);
            double far_t = std::max(t1, t2);

            t_enter = std::max(t_enter, near_t);
            t_exit = std::min(t_exit, far_t);

            // Early exit if the intervals are disjoint or the intersection is behind the ray
            if (t_enter > t_exit || t_exit < 0.0) {
                return -1;
            }
        }
    }

    // Final check: entry point must be non-negative and before exit point
    if (t_enter <= t_exit && t_enter >= 0.0) {
        return t_enter;
    }
    return -1;
}

int main() {
    // Box from (0,0,0) to (2,2,2)
    BoundingBox3D box;
    box.p[0][0] = 0.0; box.p[0][1] = 0.0; box.p[0][2] = 0.0;
    box.p[1][0] = 2.0; box.p[1][1] = 2.0; box.p[1][2] = 2.0;

    Ray3D ray;
    ray.origin[0] = -1.0; ray.origin[1] = 0.5; ray.origin[2] = 0.5;
    ray.direction[0] = 1.0; ray.direction[1] = 0.0; ray.direction[2] = 0.0;
    assert(firstIntersectionT(box, ray) == 1.0);

    ray.origin[0] = 3.0; ray.origin[1] = 1.0; ray.origin[2] = 1.0;
    ray.direction[0] = -1.0; ray.direction[1] = 0.0; ray.direction[2] = 0.0;
    assert(firstIntersectionT(box, ray) == 1.0);

    ray.origin[0] = -1.0; ray.origin[1] = 0.0; ray.origin[2] = 0.0;
    ray.direction[0] = 1.0; ray.direction[1] = 1.0; ray.direction[2] = 1.0;
    assert(firstIntersectionT(box, ray) == 1.0);

    ray.origin[0] = -1.0; ray.origin[1] = 3.0; ray.origin[2] = 1.0;
    ray.direction[0] = 1.0; ray.direction[1] = 0.0; ray.direction[2] = 0.0;
    assert(firstIntersectionT(box, ray) == -1.0); // outside y slab

    ray.origin[0] = 1.0; ray.origin[1] = 1.0; ray.origin[2] = 1.0;
    ray.direction[0] = 0.0; ray.direction[1] = 0.0; ray.direction[2] = 0.0;
    assert(firstIntersectionT(box, ray) == 0.0); // origin inside, zero direction

    ray.origin[0] = 3.0; ray.origin[1] = 1.0; ray.origin[2] = 1.0;
    ray.direction[0] = 1.0; ray.direction[1] = 0.0; ray.direction[2] = 0.0;
    assert(firstIntersectionT(box, ray) == -1.0); // ray goes away from box

    ray.origin[0] = -1.0; ray.origin[1] = 0.5; ray.origin[2] = 0.5;
    ray.direction[0] = 2.0; ray.direction[1] = 0.0; ray.direction[2] = 0.0;
    assert(firstIntersectionT(box, ray) == 0.5); // faster direction

    return 0;
}

// The slab method is the standard approach for ray–axis-aligned bounding box intersection. For each coordinate axis `x, y, z`, we compute two candidate `t` values: `t_min_coord = (box_min[coord] - origin[coord]) / direction[coord]` and `t_max_coord = (box_max[coord] - origin[coord]) / direction[coord]`, assuming the direction component is non-zero. If the direction component is zero, then the ray is parallel to that slab; it only intersects if the origin's coordinate is within the slab interval (including a small epsilon tolerance), otherwise no intersection. For each axis, the smaller of the two candidate `t` values becomes the "near" boundary and the larger becomes the "far" boundary because the ray can enter through either face depending on direction sign. The ray enters the box only after it has passed the maximum of all three near boundaries (`t_enter`) and exits before the minimum of all three far boundaries (`t_exit`). If `t_enter <= t_exit` and `t_enter` is non-negative (since we ignore intersections behind the ray origin), the ray hits the box at `t_enter`. Edge cases: when a direction component is zero, we must skip that axis's near/far computation because division by zero is invalid; instead check if the origin's coordinate lies within that slab (allowing a small epsilon). If at least one direction component is zero and the origin is outside the slab for that axis, return `-1`. Also, if all direction components are zero, treat as no intersection unless the origin is inside the box (but typically rays have non-zero direction). The algorithm processes each axis once, so time complexity is O(1) since the box has only three axes; space complexity is O(1) as we only store a few scalar variables. The error epsilon (e.g., `1e-6`) compensates for floating-point inaccuracies when checking bounds.
