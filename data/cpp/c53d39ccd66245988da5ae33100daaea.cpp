Write a standalone C++ function that determines whether a ray and a sphere intersect, and if so, returns the smallest positive or zero intersection parameter `t` that lies within a given valid interval `[min_t, max_t]`. The function must accept the ray origin (as a 3D vector), ray direction (as a 3D vector), the magnitude of the direction, the sphere center, the sphere radius, and the interval bounds. It should return `true` and set a reference parameter to the smallest valid `t` if an intersection exists; otherwise return `false`. You may use a simple 3D vector struct with `x`, `y`, `z` members and helper functions for dot product, subtraction, and normalization. Do not use any external libraries.

// The solution uses the quadratic equation derived from substituting the ray equation `P(t) = o + t*d` into the sphere equation `|P - c|² = r²`. Expanding gives `a = dot(d,d)`, `b = 2*dot(o-c, d)`, and `c = dot(o-c, o-c) - r²`. The discriminant `D = b² - 4ac` determines intersection: if `D < 0`, no real intersection. Otherwise, the two roots are `t1 = (-b - sqrt(D))/(2a)` and `t2 = (-b + sqrt(D))/(2a)`. We always have `t1 <= t2`. The function must consider only intersections with `t >= 0` (since a ray starts at `t=0` and goes forward) and within the given interval `[min_t, max_t]`. Edge cases: if `a` is zero (direction vector is zero), it's not a valid ray; return false. If the sphere contains the origin (i.e., `c = dot(o-c, o-c) < r²`), then one root is negative and the other positive; we should return the positive root if it's within the interval. If both roots are negative, no valid intersection. If `t1` is valid but `t2` is not, return `t1`; if `t1` is before `min_t` but `t2` is within, return `t2`. Complexity is O(1) time and O(1) space.

#include <cmath>
#include <limits>

// Minimal 3D vector struct for the task.
struct Vec3 {
    double x, y, z;
};

// Dot product.
double dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Subtract two vectors.
Vec3 operator-(const Vec3& a, const Vec3& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

// Scale vector by scalar.
Vec3 operator*(double s, const Vec3& v) {
    return {s * v.x, s * v.y, s * v.z};
}

// Add two vectors.
Vec3 operator+(const Vec3& a, const Vec3& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

// Normalize a vector in place (assumes non-zero length).
void normalize(Vec3& v) {
    double len = std::sqrt(dot(v, v));
    v.x /= len; v.y /= len; v.z /= len;
}

// Primary function: ray-sphere intersection with interval check.
// Returns true and sets 'out_t' to the smallest valid t >= 0 that lies in [min_t, max_t].
// If such t exists, also sets 'out_normal' to the outward normal at that point (normalized).
bool intersectRaySphere(const Vec3& rayOrigin, const Vec3& rayDir, double dirMagnitude,
                        const Vec3& sphereCenter, double sphereRadius,
                        double min_t, double max_t, double& out_t, Vec3& out_normal) {
    // Direction must be non-zero.
    if (dirMagnitude <= 0.0) {
        return false;
    }

    // Solve quadratic equation using direction vector directly.
    Vec3 oc = rayOrigin - sphereCenter;
    double a = dot(rayDir, rayDir);
    double b = 2.0 * dot(oc, rayDir);
    double c = dot(oc, oc) - sphereRadius * sphereRadius;
    double D = b * b - 4.0 * a * c;
    if (D < 0.0) {
        return false;
    }

    double sqrtD = std::sqrt(D);
    double t1 = (-b - sqrtD) / (2.0 * a);
    double t2 = (-b + sqrtD) / (2.0 * a);

    // Pick smallest t >= 0 that is within interval.
    double t = -1.0;
    if (t1 >= min_t && t1 <= max_t) {
        t = t1;
    } else if (t2 >= min_t && t2 <= max_t) {
        t = t2;
    }

    if (t < 0.0) {
        return false;
    }

    out_t = t;
    // Normal at intersection point: (P - center) / radius.
    Vec3 point = rayOrigin + t * rayDir;
    Vec3 normal = point - sphereCenter;
    normalize(normal);
    out_normal = normal;
    return true;
}

#include <cassert>
#include <cmath>

int main() {
    // Sphere at origin, radius 1.
    Vec3 center{0.0, 0.0, 0.0};
    double radius = 1.0;

    // Ray from (0,0,3) looking toward -Z direction (direction (0,0,-1), magnitude 1).
    Vec3 origin{0.0, 0.0, 3.0};
    Vec3 dir{0.0, 0.0, -1.0};
    double t = 0.0;
    Vec3 normal{0,0,0};
    // Intersection at z=1, distance t=2.
    assert(intersectRaySphere(origin, dir, 1.0, center, radius, 0.0, 10.0, t, normal));
    assert(fabs(t - 2.0) < 1e-9);
    assert(fabs(normal.x) < 1e-9 && fabs(normal.y) < 1e-9 && fabs(normal.z - 1.0) < 1e-9);

    // Ray from (0,0,0.5) along +Z, origin inside sphere, intersection at z=1, t=0.5.
    origin = {0.0, 0.0, 0.5};
    dir = {0.0, 0.0, 1.0};
    assert(intersectRaySphere(origin, dir, 1.0, center, radius, 0.0, 10.0, t, normal));
    assert(fabs(t - 0.5) < 1e-9);
    assert(fabs(normal.x) < 1e-9 && fabs(normal.y) < 1e-9 && fabs(normal.z - 1.0) < 1e-9);

    // Ray from (0,0,3) toward +Z (away) should not hit.
    dir = {0.0, 0.0, 1.0};
    assert(!intersectRaySphere(origin, dir, 1.0, center, radius, 0.0, 10.0, t, normal));

    // Ray from (0,0,-3) toward +Z, hits at z=-1, t=2.
    origin = {0.0, 0.0, -3.0};
    dir = {0.0, 0.0, 1.0};
    assert(intersectRaySphere(origin, dir, 1.0, center, radius, 0.0, 10.0, t, normal));
    assert(fabs(t - 2.0) < 1e-9);
    assert(fabs(normal.x) < 1e-9 && fabs(normal.y) < 1e-9 && fabs(normal.z + 1.0) < 1e-9);

    // Miss: ray from (0,0,3) toward +X direction.
    origin = {0.0, 0.0, 3.0};
    dir = {1.0, 0.0, 0.0};
    assert(!intersectRaySphere(origin, dir, 1.0, center, radius, 0.0, 10.0, t, normal));

    // Interval restriction: t must be >= 2.5, but actual t=2, so fail.
    origin = {0.0, 0.0, 3.0};
    dir = {0.0, 0.0, -1.0};
    assert(!intersectRaySphere(origin, dir, 1.0, center, radius, 2.5, 10.0, t, normal));

    // Interval that includes t=2 exactly at the lower bound.
    assert(intersectRaySphere(origin, dir, 1.0, center, radius, 2.0, 10.0, t, normal));
    assert(fabs(t - 2.0) < 1e-9);

    // Zero direction magnitude should fail.
    assert(!intersectRaySphere(origin, {0.0,0.0,0.0}, 0.0, center, radius, 0.0, 10.0, t, normal));

    // Edge case: ray tangent to sphere, discriminant zero, t1==t2.
    // Sphere radius 2, ray from (0,0,3) direction (0, 1, 0)? Actually need tangent.
    // Use sphere at (0,0,0), radius 2. Ray from (2,0,0) direction (0,1,0) misses? Let's do tangent: ray from (2,0,0) direction (-1,0,0) hits at t=4 (from x=2 to x=-2) but that's a full chord, not tangent. Tangent: ray from (2,0,0) direction (0,0,1)? That's not tangent. Tangent case: direction perpendicular to radius at the point of tangency. For sphere at origin, radius 2, point of tangency (2,0,0), ray direction (0,1,0) (perpendicular to radial direction). Then the line is x=2, y=t, z=0, distance from origin is always >=2, only touches at t=0. So ray origin (2,0,0), dir (0,1,0), t=0 is intersection if interval includes 0.
    Vec3 center2{0,0,0};
    double radius2 = 2.0;
    origin = {2.0, 0.0, 0.0};
    dir = {0.0, 1.0, 0.0};
    assert(intersectRaySphere(origin, dir, 1.0, center2, radius2, 0.0, 10.0, t, normal));
    assert(fabs(t - 0.0) < 1e-9);
    assert(fabs(normal.x - 1.0) < 1e-9 && fabs(normal.y) < 1e-9 && fabs(normal.z) < 1e-9);

    return 0;
}
