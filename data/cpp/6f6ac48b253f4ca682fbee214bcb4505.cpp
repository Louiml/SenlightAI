/*
Write a C++ function `double rayCylinderIntersection(const Ray& ray, const Cylinder& cylinder)` that computes the smallest positive parameter `t` along a parametric ray `P(t) = origin + t * direction` at which the ray intersects a finite right circular cylinder. The cylinder is defined by a base center point, a unit axis direction vector, a radius, a height, and two boolean flags indicating whether the top and bottom caps are present. The ray and cylinder are passed as simple structs containing the necessary geometric data. The function must consider intersections with the lateral surface (the infinite cylinder) and, if enabled, the two circular caps (perpendicular disks at the base and top). It must return the smallest non-negative `t` corresponding to a valid intersection point that lies on the actual finite cylinder (including caps if enabled), or `-1.0` if no such intersection exists. Handle degenerate cases gracefully, such as the ray being parallel to a cap's plane, the ray missing the lateral surface, or the ray starting inside the cylinder. The direction vector of the ray is assumed normalized, and the axis vector of the cylinder is also normalized. Implement the function using only standard C++ library features and vector/matrix helper functions that you define yourself (no external libraries).
*/

#include <cmath>
#include <limits>
#include <algorithm>

// Simple 3D vector and matrix helpers (minimal inline implementations)
struct Vec3 {
    double x, y, z;
    Vec3(double x=0, double y=0, double z=0) : x(x), y(y), z(z) {}
};

inline Vec3 operator+(const Vec3& a, const Vec3& b) { return Vec3(a.x+b.x, a.y+b.y, a.z+b.z); }
inline Vec3 operator-(const Vec3& a, const Vec3& b) { return Vec3(a.x-b.x, a.y-b.y, a.z-b.z); }
inline Vec3 operator*(double s, const Vec3& v) { return Vec3(s*v.x, s*v.y, s*v.z); }
inline double dot(const Vec3& a, const Vec3& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return Vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}
inline double norm(const Vec3& v) { return std::sqrt(dot(v,v)); }

// Project a vector onto the plane perpendicular to axis a (a is unit)
inline Vec3 projectPerp(const Vec3& v, const Vec3& a) {
    return v - dot(v,a) * a;
}

// Ray and Cylinder definitions
struct Ray {
    Vec3 origin;
    Vec3 direction; // assumed normalized
};

struct Cylinder {
    Vec3 baseCenter;   // Cb
    Vec3 axis;         // unit vector dc
    double radius;     // Rc
    double height;     // H
    bool hasBottomCap; // tampaInf
    bool hasTopCap;    // tampaSup
};

// Helper: solve quadratic ax^2 + bx + c = 0, return number of real solutions
// Store roots in t1 and t2 (if any)
int solveQuadratic(double a, double b, double c, double& t1, double& t2) {
    const double eps = 1e-12;
    if (std::abs(a) < eps) {
        // linear equation bx + c = 0
        if (std::abs(b) < eps) return 0;
        t1 = -c / b;
        return 1;
    }
    double disc = b*b - 4*a*c;
    if (disc < 0) return 0;
    if (std::abs(disc) < eps) {
        t1 = -b / (2*a);
        return 1;
    }
    double sqrtDisc = std::sqrt(disc);
    double tPlus = (-b + sqrtDisc) / (2*a);
    double tMinus = (-b - sqrtDisc) / (2*a);
    // order them so t1 <= t2
    if (tPlus <= tMinus) {
        t1 = tPlus; t2 = tMinus;
    } else {
        t1 = tMinus; t2 = tPlus;
    }
    return 2;
}

// Main function: returns smallest non-negative t for valid intersection, or -1.0
double rayCylinderIntersection(const Ray& ray, const Cylinder& cylinder) {
    const double eps = 1e-12;
    const double INF = std::numeric_limits<double>::infinity();
    double bestT = -1.0;

    // --- Lateral surface ---
    Vec3 w = ray.origin - cylinder.baseCenter;
    Vec3 m_d = projectPerp(ray.direction, cylinder.axis);
    Vec3 m_w = projectPerp(w, cylinder.axis);

    double a_coef = dot(m_d, m_d);
    double b_coef = 2.0 * dot(m_w, m_d);
    double c_coef = dot(m_w, m_w) - cylinder.radius * cylinder.radius;

    double t1, t2;
    int nRoots = solveQuadratic(a_coef, b_coef, c_coef, t1, t2);
    // Check each root (t1 <= t2 if two roots) for validity
    double candidates[2] = {t1, t2};
    int numCandidates = (nRoots >= 1 ? 1 : 0) + (nRoots == 2 ? 1 : 0);
    for (int i = 0; i < numCandidates; ++i) {
        double t = candidates[i];
        if (t > -eps) { // allow t >= 0 (small tolerance for zero)
            if (t < 0) t = 0;
            Vec3 point = ray.origin + t * ray.direction;
            double axisCoord = dot(point - cylinder.baseCenter, cylinder.axis);
            if (axisCoord >= -eps && axisCoord <= cylinder.height + eps) {
                if (bestT < 0 || t < bestT) bestT = t;
            }
        }
    }

    // --- Bottom cap (if enabled) ---
    if (cylinder.hasBottomCap) {
        double denom = dot(ray.direction, cylinder.axis);
        if (std::abs(denom) > eps) {
            double t = -dot(w, cylinder.axis) / denom;
            if (t > -eps) {
                if (t < 0) t = 0;
                Vec3 point = ray.origin + t * ray.direction;
                Vec3 proj = point - cylinder.baseCenter;
                double dist = norm(projectPerp(proj, cylinder.axis));
                if (dist <= cylinder.radius + eps) {
                    if (bestT < 0 || t < bestT) bestT = t;
                }
            }
        }
    }

    // --- Top cap (if enabled) ---
    if (cylinder.hasTopCap) {
        Vec3 topCenter = cylinder.baseCenter + cylinder.height * cylinder.axis;
        Vec3 wTop = ray.origin - topCenter;
        double denom = dot(ray.direction, cylinder.axis);
        if (std::abs(denom) > eps) {
            double t = -dot(wTop, cylinder.axis) / denom;
            if (t > -eps) {
                if (t < 0) t = 0;
                Vec3 point = ray.origin + t * ray.direction;
                Vec3 proj = point - cylinder.baseCenter;
                double dist = norm(projectPerp(proj, cylinder.axis));
                if (dist <= cylinder.radius + eps) {
                    if (bestT < 0 || t < bestT) bestT = t;
                }
            }
        }
    }

    return bestT;
}

#include <cassert>
#include <cmath>

int main() {
    // Cylinder: base at origin, axis along +Y, radius 1, height 2, both caps on
    Cylinder cyl;
    cyl.baseCenter = Vec3(0,0,0);
    cyl.axis = Vec3(0,1,0);
    cyl.radius = 1.0;
    cyl.height = 2.0;
    cyl.hasBottomCap = true;
    cyl.hasTopCap = true;

    // Ray along +X from (-5,0,0) hits lateral surface at t=4 (x=-1)
    Ray r1;
    r1.origin = Vec3(-5,0,0);
    r1.direction = Vec3(1,0,0);
    double t1 = rayCylinderIntersection(r1, cyl);
    assert(std::abs(t1 - 4.0) < 1e-9);

    // Ray from (0,-5,0) along +Y hits bottom cap at t=5 (y=0)
    Ray r2;
    r2.origin = Vec3(0,-5,0);
    r2.direction = Vec3(0,1,0);
    double t2 = rayCylinderIntersection(r2, cyl);
    assert(std::abs(t2 - 5.0) < 1e-9);

    // Ray from (0,5,0) along -Y hits top cap at t=3 (y=2)
    Ray r3;
    r3.origin = Vec3(0,5,0);
    r3.direction = Vec3(0,-1,0);
    double t3 = rayCylinderIntersection(r3, cyl);
    assert(std::abs(t3 - 3.0) < 1e-9);

    // Ray along +X from (5,0,0) hits lateral surface at t=4 (x=-1? actually x=1, so t=4)
    Ray r4;
    r4.origin = Vec3(5,0,0);
    r4.direction = Vec3(-1,0,0);
    double t4 = rayCylinderIntersection(r4, cyl);
    assert(std::abs(t4 - 4.0) < 1e-9);

    // Ray that misses entirely: from (0,0,-5) along +Z (parallel to axis? no, lateral but far away)
    Ray r5;
    r5.origin = Vec3(0,0,-5);
    r5.direction = Vec3(0,0,1);
    double t5 = rayCylinderIntersection(r5, cyl);
    assert(t5 == -1.0);

    // Ray through caps but misses disk: from (2,0,0) along -X? Actually it would hit lateral if within radius, but here start outside and direction horizontal at y=0, but x=2 > radius so no hit
    Ray r6;
    r6.origin = Vec3(2,0,0);
    r6.direction = Vec3(-1,0,0);
    double t6 = rayCylinderIntersection(r6, cyl);
    assert(t6 == -1.0);

    // Ray from inside cylinder along +Y: origin (0,0.5,0), hits top cap at t=1.5
    Ray r7;
    r7.origin = Vec3(0,0.5,0);
    r7.direction = Vec3(0,1,0);
    double t7 = rayCylinderIntersection(r7, cyl);
    assert(std::abs(t7 - 1.5) < 1e-9);

    // Cylinder without caps: ray through caps should not intersect
    Cylinder cylNoCaps = cyl;
    cylNoCaps.hasBottomCap = false;
    cylNoCaps.hasTopCap = false;
    Ray r8;
    r8.origin = Vec3(0,-5,0);
    r8.direction = Vec3(0,1,0);
    double t8 = rayCylinderIntersection(r8, cylNoCaps);
    assert(t8 == -1.0);

    // Ray starting exactly on the lateral surface at t=0 is accepted
    Ray r9;
    r9.origin = Vec3(1,0,0); // on surface
    r9.direction = Vec3(1,0,0);
    double t9 = rayCylinderIntersection(r9, cyl);
    assert(t9 >= 0.0 && t9 < 1e-9);

    return 0;
}

// To solve this, we treat the ray as a parametric line `P(t) = O + t * d` (where `O` is origin, `d` is unit direction). For the lateral surface, we project points onto the plane perpendicular to the cylinder axis. Define the projection matrix `M = I - a*a^T`, where `a` is the axis unit vector. A point lies on the infinite cylinder's lateral surface if the squared length of its projected vector from the base center, `(P - Cb)`, equals `R^2`. Substitute the ray into this equation: we get a quadratic in `t`: `a_coef * t^2 + b_coef * t + c_coef = 0`, where `a_coef = (M*d)·(M*d)`, `b_coef = 2*(M*(O-Cb))·(M*d)`, `c_coef = (M*(O-Cb))·(M*(O-Cb)) - R^2`. If `a_coef` is near zero, the ray is parallel to the axis and no lateral intersection unless it lies on the surface (then we treat as infinity or skip). Solve the quadratic; for each positive root `t`, compute the point and check that its projection along the axis lies between 0 and H (inclusive) from the base center. For caps, the bottom cap is a disk lying in the plane where `(P - Cb)·a = 0`, and the top cap is at `(P - Cb)·a = H`. For each enabled cap, intersect the ray with that plane: if `d·a` is near zero, skip (parallel); otherwise `t = -( (O-Cb)·a ) / (d·a)`. If `t > 0`, compute the point and check that the projected distance from the base center is ≤ R (i.e., inside the disk). After computing all candidate `t` values (lateral, bottom cap, top cap), select the smallest positive one that corresponds to a valid intersection. If none, return -1. Edge cases: if the ray starts exactly on the surface with `t=0`, we still consider `t>0` to avoid degenerate hits (or we accept `t=0` if desired; but the spec says smallest non-negative, so we allow 0 if it is a valid surface point, though typically we want positive to avoid self-intersection—we'll allow 0 for simplicity). We must also handle the case where the ray is completely inside the cylinder and hits a cap from inside—the function should return the exit intersection. Time complexity is O(1) since we only solve constant-size equations. Space complexity is O(1).
