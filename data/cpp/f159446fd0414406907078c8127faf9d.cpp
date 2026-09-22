Write a C++ function named `findClosestIntersection` that takes a vector of geometric objects (represented by a minimal interface) and a ray (given by an origin point and a direction vector), and returns the parameter `t` of the closest intersection along the ray, or `-1.0` if no intersection exists. Each object provides an `intersectRay` method that, given the ray origin and direction, returns the smallest positive `t` for which the ray hits the object, or `-1.0` if no hit. The ray is defined in 3D using simple structs for `Vec3` and `Ray`. The function must handle objects that may have no intersection, multiple intersections, and negative `t` (behind the origin) which should be ignored. The input vector is non-empty. Use only the standard library; no external ray-tracing code.
#include <cassert>
#include <vector>

int main() {
    // Sphere along z-axis: center (0,0,5), radius 1
    Sphere s1(Vec3(0,0,5), 1.0);
    // Sphere behind origin: center (0,0,-3), radius 1 → no positive hit from origin
    Sphere s2(Vec3(0,0,-3), 1.0);
    // Sphere far ahead, radius 1
    Sphere s3(Vec3(0,0,10), 1.0);
    // Sphere to the side at origin, radius 2 → ray along z doesn't hit it
    Sphere s4(Vec3(3,0,0), 1.0);

    std::vector<Intersectable*> objects = {&s1, &s2, &s3, &s4};

    Ray ray(Vec3(0,0,0), Vec3(0,0,1)); // pointing +z

    double closest = findClosestIntersection(objects, ray);
    // Expected closest hit is s1 at t=4 (since ray origin at z=0, center at 5, radius 1 => t=4)
    assert(closest > 3.999 && closest < 4.001);

    // Ray pointing -z: should hit s2 at t=2 (center at -3, radius 1, origin at 0 => t=2)
    Ray rayNeg(Vec3(0,0,0), Vec3(0,0,-1));
    double closestNeg = findClosestIntersection(objects, rayNeg);
    assert(closestNeg > 1.999 && closestNeg < 2.001);

    // Ray pointing +x: only s4 is at x=3, radius 1 → closest t=2
    Ray rayX(Vec3(0,0,0), Vec3(1,0,0));
    double closestX = findClosestIntersection(objects, rayX);
    assert(closestX > 1.999 && closestX < 2.001);

    // Ray pointing up: no sphere on y-axis → should return -1
    Ray rayY(Vec3(0,0,0), Vec3(0,1,0));
    assert(findClosestIntersection(objects, rayY) == -1.0);

    // Empty objects vector should return -1
    std::vector<Intersectable*> empty;
    assert(findClosestIntersection(empty, ray) == -1.0);
}
#include <vector>
#include <limits>
#include <algorithm>

struct Vec3 {
    double x, y, z;
    Vec3(double x = 0.0, double y = 0.0, double z = 0.0) : x(x), y(y), z(z) {}
};

struct Ray {
    Vec3 origin;
    Vec3 direction;
    Ray(const Vec3& o, const Vec3& d) : origin(o), direction(d) {}
};

// Minimal abstract base class for geometric objects.
struct Intersectable {
    virtual ~Intersectable() = default;
    // Returns the smallest positive t for intersection, or -1.0 if no hit.
    virtual double intersectRay(const Vec3& origin, const Vec3& direction) const = 0;
};

// Example concrete object for testing: a sphere.
struct Sphere : public Intersectable {
    Vec3 center;
    double radius;
    Sphere(const Vec3& c, double r) : center(c), radius(r) {}
    double intersectRay(const Vec3& origin, const Vec3& direction) const override {
        Vec3 oc = {origin.x - center.x, origin.y - center.y, origin.z - center.z};
        double a = direction.x*direction.x + direction.y*direction.y + direction.z*direction.z;
        double b = 2.0 * (oc.x*direction.x + oc.y*direction.y + oc.z*direction.z);
        double c = oc.x*oc.x + oc.y*oc.y + oc.z*oc.z - radius*radius;
        double disc = b*b - 4.0*a*c;
        if (disc < 0.0) return -1.0;
        double sqrtDisc = std::sqrt(disc);
        double t1 = (-b - sqrtDisc) / (2.0 * a);
        double t2 = (-b + sqrtDisc) / (2.0 * a);
        double t = (t1 > 1e-9) ? t1 : t2;
        return (t > 1e-9) ? t : -1.0;
    }
};

// Returns the smallest positive t for any hit among objects, or -1.0 if none.
double findClosestIntersection(const std::vector<Intersectable*>& objects, const Ray& ray) {
    double bestT = std::numeric_limits<double>::max();
    for (const Intersectable* obj : objects) {
        if (obj == nullptr) continue;
        double t = obj->intersectRay(ray.origin, ray.direction);
        if (t > 1e-9 && t < bestT) {
            bestT = t;
        }
    }
    return (bestT == std::numeric_limits<double>::max()) ? -1.0 : bestT;
}
// The solution needs to iterate over all objects in the input vector, call each object's `intersectRay` method with the ray parameters, and keep track of the smallest positive `t` encountered. Initialize `bestT` to a large sentinel value (e.g., `std::numeric_limits<double>::max()`). For each object, get `t = obj.intersectRay(origin, direction)`. If `t` is greater than or equal to 0 (or a small epsilon like 1e-9 to avoid numerical noise) and less than `bestT`, update `bestT`. After processing all objects, if `bestT` remains the sentinel value, return `-1.0` to indicate no hit; otherwise return `bestT`. Important edge cases include: objects that return `-1.0` for no hit, objects that return `0.0` (touching the origin, which may or may not count; here we treat `t >= epsilon` as valid to avoid degenerate self-intersections), and multiple objects with the same `t` (any is fine). Time complexity is O(N) where N is the number of objects (each intersection test is assumed O(1)). Space complexity is O(1) auxiliary, excluding input storage. The function should be `const`-correct: take the vector by `const` reference and the ray by `const` reference, and the function itself should be `const`-qualified if appropriate for a class, but as a free function it can simply take const references.
