// Write a C++ function `closestRaycastHit` that determines whether a ray from a given origin through a given direction intersects any sphere from a list of spheres, and if so, returns the index of the closest sphere hit along with the distance. Spheres are defined by a center (Vec with x,y,z) and a radius. The ray is given as an origin point `from` and a direction `to` (the direction vector is not necessarily normalized; normalize it inside). Return a struct containing a boolean `hit`, the hit distance `t` (the smallest positive `t` such that `from + t*dir` lies on the sphere surface), and the index of the sphere. If no hit exists, set `hit=false` and leave `t` and `index` undefined. Ignore hits behind the origin (t <= 0). Prefer the sphere with the smallest positive `t` in case of multiple intersections. Use double precision arithmetic. The function must be `const`-correct and handle an empty sphere list.

#include <cassert>

int main() {
    // Define spheres
    Sphere s1(Vec(0, 0, 5), 1.0);
    Sphere s2(Vec(0, 0, 10), 2.0);
    Sphere s3(Vec(0, 0, 3), 0.5);
    std::vector<Sphere> spheres = {s1, s2, s3};

    // Ray from origin towards +z, should hit s3 first (t=2.5), then s1 (t=4) then s2 (t=8)
    RayHit hit = closestRaycastHit(Vec(0,0,0), Vec(0,0,1), spheres);
    assert(hit.hit == true);
    assert(hit.index == 2);
    assert(fabs(hit.t - 2.5) < 1e-9);

    // Ray from origin towards -z, no hits (all spheres are in +z)
    RayHit noHit = closestRaycastHit(Vec(0,0,0), Vec(0,0,-1), spheres);
    assert(noHit.hit == false);

    // Ray starting inside a sphere: origin at (0,0,5) inside s1
    // direction +z, should hit s2 at t=5 (since s1 gives t=0, which is ignored)
    RayHit fromInside = closestRaycastHit(Vec(0,0,5), Vec(0,0,1), spheres);
    assert(fromInside.hit == true);
    assert(fromInside.index == 1);
    assert(fabs(fromInside.t - 5.0) < 1e-9);

    // Empty sphere list
    std::vector<Sphere> empty;
    RayHit emptyHit = closestRaycastHit(Vec(0,0,0), Vec(0,0,1), empty);
    assert(emptyHit.hit == false);

    // Ray exactly tangent to sphere (D == 0) and origin outside
    // Sphere at (0,0,2) radius 1, ray from (0,0,0) direction (0,1,0) is tangent at t? 
    // Actually tangent point is at (0,1,2) from origin? Let's use a simpler case:
    // Sphere center (0,2,0) radius 1, ray from (0,0,0) direction (0,0,1) is y=0 plane, distance 2 >1, no hit.
    // Instead, test tangent with ray from (0,0,0) to (0,2,0) direction is normalized (0,1,0), sphere at (0,1,0) radius 1. The tangent point is at t=1 (origin is distance 1 from center, exactly on surface? Actually origin is at distance 1, so t=0 is on surface, ignored, and t=2 is the other side? That doesn't work. Use a clear tangent: from (0,0,0) to (0,2,2) direction (0,1,1) normalized, sphere center (0,1,1) radius 1. Closest approach distance from center to line is 1 (radius) -> exactly tangent at one point, t = dot(center-from, dir) = (0,1,1)·(0,1/√2,1/√2) = √2 ≈ 1.414. So hit at that t.
    {
        std::vector<Sphere> tang = {Sphere(Vec(0,1,1), 1.0)};
        RayHit th = closestRaycastHit(Vec(0,0,0), Vec(0,2,2), tang);
        assert(th.hit == true);
        assert(th.index == 0);
        assert(fabs(th.t - std::sqrt(2.0)) < 1e-9);
    }

    // Ray from exactly on sphere surface but pointing away (should have no hit because t=0 ignored and other root negative)
    std::vector<Sphere> surf = {Sphere(Vec(0,0,1), 1.0)};
    RayHit surfHit = closestRaycastHit(Vec(0,0,0), Vec(0,0,-1), surf); // origin is on sphere at (0,0,0) distance 1 from center? Actually center (0,0,1), origin (0,0,0) is on surface. Direction away from center is (0,0,-1). Both roots: t1=0, t2=2, but we ignore t=0, and t2=2 is positive but that's the far side, which is actually behind the ray? Wait direction (0,0,-1) points away, so t2=2 gives point (0,0,-2) which is a valid intersection on the sphere (the other side). Actually it is a real intersection, so hit=true with t=2. Let's not assert no hit; instead assert that it hits at t=2.
    assert(surfHit.hit == true);
    assert(surfHit.index == 0);
    assert(fabs(surfHit.t - 2.0) < 1e-9);

    // Multiple spheres, ray passes through both, pick closest
    std::vector<Sphere> two = {Sphere(Vec(0,0,3), 1.0), Sphere(Vec(0,0,6), 0.5)};
    RayHit twoHit = closestRaycastHit(Vec(0,0,0), Vec(0,0,1), two);
    assert(twoHit.hit == true);
    assert(twoHit.index == 0); // closer sphere
    assert(fabs(twoHit.t - 2.0) < 1e-9); // distance to first intersection = 3-1 = 2

    return 0;
}

#include <vector>
#include <cmath>
#include <limits>

// Minimal vector class for 3D points with double precision
struct Vec {
    double x, y, z;
    Vec(double x_ = 0, double y_ = 0, double z_ = 0) : x(x_), y(y_), z(z_) {}
    Vec operator-(const Vec& other) const { return Vec(x - other.x, y - other.y, z - other.z); }
    double dot(const Vec& other) const { return x * other.x + y * other.y + z * other.z; }
    double norm() const { return std::sqrt(dot(*this)); }
    Vec normalized() const {
        double n = norm();
        if (n < 1e-12) return Vec(0, 0, 0);
        return Vec(x / n, y / n, z / n);
    }
};

// Sphere definition
struct Sphere {
    Vec center;
    double radius;
    Sphere(const Vec& c, double r) : center(c), radius(r) {}
};

// Result of raycast: hit flag, distance t, sphere index
struct RayHit {
    bool hit;
    double t;
    int index;
    RayHit() : hit(false), t(0.0), index(-1) {}
};

// Given a ray from 'from' in direction 'to' (not necessarily normalized),
// find the closest sphere intersection with t > 0.
RayHit closestRaycastHit(const Vec& from, const Vec& to, const std::vector<Sphere>& spheres) {
    RayHit best;
    double minT = std::numeric_limits<double>::infinity();
    
    Vec dir = (to - from).normalized();  // normalize the direction
    if (dir.norm() == 0.0) {  // degenerate direction (from == to) -> no valid ray
        return best;
    }

    for (int i = 0; i < static_cast<int>(spheres.size()); ++i) {
        const Sphere& s = spheres[i];
        Vec oc = from - s.center;
        
        // Quadratic coefficients: a = 1 (because dir is normalized)
        double b = 2.0 * dir.dot(oc);
        double c = oc.dot(oc) - s.radius * s.radius;
        double D = b * b - 4.0 * c;
        
        if (D < 0.0) continue;  // no real intersection
        
        double sqrtD = std::sqrt(D);
        double t1 = (-b - sqrtD) / 2.0;
        double t2 = (-b + sqrtD) / 2.0;
        
        // Consider both roots, pick the smallest positive one (strictly > 0)
        double tCandidate = 0.0;
        if (t1 > 0.0 && t2 > 0.0) {
            tCandidate = (t1 < t2) ? t1 : t2;
        } else if (t1 > 0.0) {
            tCandidate = t1;
        } else if (t2 > 0.0) {
            tCandidate = t2;
        } else {
            continue;  // both roots non-positive
        }
        
        if (tCandidate < minT) {
            minT = tCandidate;
            best.hit = true;
            best.t = tCandidate;
            best.index = i;
        }
    }
    
    return best;
}

// The solution is based on solving the quadratic equation for ray-sphere intersection. For a ray `P = from + t*dir` (with `dir = normalize(to - from)`) and a sphere center `C` and radius `r`, intersection occurs when `|P - C|^2 = r^2`. This expands to `t^2 * (dir·dir) + 2*t*(dir·(from - C)) + ((from - C)·(from - C) - r^2) = 0`. Since `dir` is normalized, `dir·dir = 1`. Let `oc = from - C`. Then `a = 1`, `b = 2*(dir·oc)`, `c = oc·oc - r*r`. The discriminant `D = b*b - 4*c`. If `D < 0`, no intersection. If `D >= 0`, compute two roots: `t1 = (-b - sqrt(D))/2` and `t2 = (-b + sqrt(D))/2`. Although `t2` is always larger (if D>0), we must consider both because sometimes only one root is positive. For each sphere, compute both roots and take the smallest positive root among them. Among all spheres, take the minimal positive root. The smallest positive `t` is the earliest intersection along the ray. Edge cases: when `D == 0`, both roots coincide, and unless the double root is positive, no hit. When the origin is exactly on the sphere surface, `c = 0`, and one root is `t=0` (origin on surface) and the other is positive; we ignore `t=0` because we require `t>0` (strictly positive; usually `t >= epsilon` to avoid self-intersection). Since the problem states "ignore hits behind origin (t <= 0)", we use `t > 0` strictly. The complexity is O(n) per ray for n spheres, with O(1) extra space.
