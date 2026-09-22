// Given a 3D ray defined by an origin point and a direction vector, and a unit square plane lying in the xy-plane (z = 0) with side length 2 (extending from -1 to 1 in both x and y), write a C++ function `std::vector<RayIntersection> intersectPlane(const Ray& ray, const Material& mat)` that returns a vector containing the ray-plane intersection (if any) with the point, outward normal (normalized, pointing against the ray direction), distance from the ray origin, and the provided material. The ray direction may be unnormalized; treat it as a mathematical direction (normalize it internally). Ignore intersections with t ≤ epsilon (use `epsilon = 1e-6`). The normal should always face the ray (i.e., flipped so it points opposite to the ray direction). The returned vector is empty if the ray is parallel to the plane or misses the finite square. Input types are given as `Point` (Eigen-like 3D vector), `Direction` (same type), `Ray` (with `.point` and `.direction`), `Material` (copyable), and `RayIntersection` (with `.point`, `.normal`, `.distance`, `.material`). Use double precision. Do not use any external libraries beyond standard headers and simple struct definitions provided in your solution.

#include <cassert>
#include <cmath>

// Include the solution code here (or assume above definitions)

int main() {
    // Ray pointing straight at the center
    {
        Ray r(Point(0, 0, 5), Direction(0, 0, -1));
        auto hits = intersectPlane(r, Material(7));
        assert(hits.size() == 1);
        assert(std::fabs(hits[0].point.x - 0.0) < 1e-9);
        assert(std::fabs(hits[0].point.y - 0.0) < 1e-9);
        assert(std::fabs(hits[0].point.z - 0.0) < 1e-9);
        assert(std::fabs(hits[0].normal.x - 0.0) < 1e-9);
        assert(std::fabs(hits[0].normal.y - 0.0) < 1e-9);
        assert(std::fabs(hits[0].normal.z - 1.0) < 1e-9); // pointing against ray (up)
        assert(std::fabs(hits[0].distance - 5.0) < 1e-9);
        assert(hits[0].material.id == 7);
    }

    // Ray hitting the edge of the square
    {
        Ray r(Point(1.5, 0, 3), Direction(0, 0, -1));
        auto hits = intersectPlane(r, Material(2));
        // x=1.5 is outside bounds
        assert(hits.empty());
    }

    // Ray from below, normal should point down
    {
        Ray r(Point(0, 0, -3), Direction(0, 0, 1));
        auto hits = intersectPlane(r, Material(3));
        assert(hits.size() == 1);
        assert(std::fabs(hits[0].normal.z + 1.0) < 1e-9); // pointing down
        assert(std::fabs(hits[0].distance - 3.0) < 1e-9);
    }

    // Ray parallel to plane
    {
        Ray r(Point(0, 0, 2), Direction(1, 0, 0));
        auto hits = intersectPlane(r, Material(4));
        assert(hits.empty());
    }

    // Ray pointing away from plane (t negative)
    {
        Ray r(Point(0, 0, 5), Direction(0, 0, 1));
        auto hits = intersectPlane(r, Material(5));
        assert(hits.empty());
    }

    // Ray close to boundary but inside (epsilon tolerance)
    {
        Ray r(Point(0.9999999, 0, 2), Direction(0, 0, -1));
        auto hits = intersectPlane(r, Material(6));
        assert(hits.size() == 1);
    }

    // Unnormalized direction
    {
        Ray r(Point(0, 0, 10), Direction(0, 0, -2));
        auto hits = intersectPlane(r, Material(8));
        assert(hits.size() == 1);
        assert(std::fabs(hits[0].distance - 10.0) < 1e-9);
    }

    // Hit point at corner exactly
    {
        Ray r(Point(1, 1, 4), Direction(0, 0, -1));
        auto hits = intersectPlane(r, Material(9));
        assert(hits.size() == 1);
        assert(std::fabs(hits[0].point.x - 1.0) < 1e-9);
        assert(std::fabs(hits[0].point.y - 1.0) < 1e-9);
    }

    // Zero-length direction (degenerate)
    {
        Ray r(Point(0, 0, 1), Direction(0, 0, 0));
        auto hits = intersectPlane(r, Material(10));
        assert(hits.empty());
    }

    return 0;
}

#include <vector>
#include <cmath>
#include <cstdlib>

// Minimal types for the task
struct Point {
    double x, y, z;
    Point(double x=0, double y=0, double z=0) : x(x), y(y), z(z) {}
    double dot(const Point& o) const { return x*o.x + y*o.y + z*o.z; }
    Point operator-(const Point& o) const { return Point(x-o.x, y-o.y, z-o.z); }
    Point operator+(const Point& o) const { return Point(x+o.x, y+o.y, z+o.z); }
    Point operator*(double s) const { return Point(x*s, y*s, z*s); }
    double norm() const { return std::sqrt(x*x + y*y + z*z); }
};

using Direction = Point;

struct Ray {
    Point point;
    Direction direction;
    Ray(const Point& p, const Direction& d) : point(p), direction(d) {}
};

struct Material {
    // Dummy; could hold color, etc.
    int id;
    Material(int id=0) : id(id) {}
    bool operator==(const Material& o) const { return id == o.id; }
};

struct RayIntersection {
    Point point;
    Point normal;
    double distance;
    Material material;
};

// Return intersections of a ray with the unit square plane z=0, side length 2.
std::vector<RayIntersection> intersectPlane(const Ray& ray, const Material& mat) {
    const double epsilon = 1e-6;
    std::vector<RayIntersection> result;

    // Normalize ray direction
    double dlen = ray.direction.norm();
    if (dlen < epsilon) return result; // degenerate
    Direction d = ray.direction * (1.0 / dlen);
    Point p = ray.point;

    // Plane normal (z-axis)
    Point N(0, 0, 1);

    // Check if ray is not parallel to plane
    double denom = d.dot(N);
    if (std::fabs(denom) > epsilon) {
        double t = -(p.dot(N)) / denom;
        if (t > epsilon) {
            Point hitPoint = p + d * t;
            // Check bounds within square [-1,1] in x and y
            if (std::fabs(hitPoint.x) <= 1.0 + epsilon &&
                std::fabs(hitPoint.y) <= 1.0 + epsilon) {
                RayIntersection hit;
                hit.point = hitPoint;
                hit.normal = N;
                // Ensure normal points against ray direction
                if (hit.normal.dot(ray.direction) > 0) {
                    hit.normal = hit.normal * -1.0;
                }
                hit.normal = hit.normal * (1.0 / hit.normal.norm()); // normalize
                hit.distance = (hitPoint - ray.point).norm();
                hit.material = mat;
                result.push_back(hit);
            }
        }
    }

    return result;
}

// The solution computes the intersection of a ray with an infinite plane at z=0, then checks if the hit point lies within the square bounds |x| ≤ 1 and |y| ≤ 1. First normalize the ray direction to ensure correct t computation. The plane normal is N = (0,0,1). Compute t = -(p.dot(N)) / (d.dot(N)) where p is the ray origin and d is the normalized direction. If d(2) is near zero, the ray is parallel and we return empty. If t ≤ epsilon, the intersection is behind or at the origin, so discard. Otherwise compute the hit point = p + t*d, check bounds with epsilon tolerance. If within bounds, set the hit's point, compute the normal as (0,0,1), and if the normal dot the original (unnormalized) ray direction is positive, flip the normal to point against the ray. Normalize the normal (it's already unit length, but do it for safety). Set distance = (hit.point - ray.point).norm() (note: ray.point is original, not normalized p; but since we used normalized d for t, t is distance along normalized direction, so distance equals t directly, but we compute via norm for accuracy). Assign material. Edge cases: ray origin inside the plane (t=0) is ignored; ray parallel to plane with d(2)=0; ray pointing away from plane (t negative); hit point exactly on boundary (treat as inside due to epsilon). Time complexity O(1), space O(1).
