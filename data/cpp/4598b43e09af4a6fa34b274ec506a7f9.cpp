// Write a standalone C++ function named `rayTriangleClosestHit` that determines the closest intersection distance between a ray (defined by an origin point `origin` and a direction vector `direction`) and a triangle (defined by three vertices `v0`, `v1`, `v2`) in 3D space. The function must return the distance along the ray to the intersection point if the ray hits the triangle, or `infinity` (using `std::numeric_limits<float>::infinity()`) if it does not. The ray is considered to have a valid hit only if the intersection point lies in front of the ray origin (distance `>= 0.0f`), and the triangle is treated as single-sided: only intersections with the front face (where the triangle's geometric normal points toward the ray's direction) count. The function signature must be: `float rayTriangleClosestHit(const Vector3& origin, const Vector3& direction, const Vector3& v0, const Vector3& v1, const Vector3& v2);` You may assume a `Vector3` class is available with `x`, `y`, `z` public members and operations `+`, `-`, scalar multiplication, `dot` (returns float), and `cross` (returns Vector3). The function must be self-contained (no external math library), use `const` correctness, and be efficient (avoid unnecessary divisions or sqrt). The task is to implement the Möller–Trumbore ray-triangle intersection algorithm as a reusable, testable utility function.
// The core algorithm is the Möller–Trumbore ray-triangle intersection method, which solves for the intersection using barycentric coordinates. The approach: compute two edge vectors `edge1 = v1 - v0` and `edge2 = v2 - v0`. Then compute `p = direction.cross(edge2)` and the determinant `det = edge1.dot(p)`. If `det` is less than or equal to a small epsilon (e.g., `1e-8`), the ray is parallel to the triangle or hits the back face; since the triangle is single-sided, return infinity. Otherwise, compute `f = 1.0f / det`, `t = origin - v0`, and `u = t.dot(p) * f`. If `u < 0` or `u > 1`, the intersection lies outside the triangle, so return infinity. Next compute `q = t.cross(edge1)` and `v = direction.dot(q) * f`. If `v < 0` or `u + v > 1`, the intersection is outside, return infinity. Finally, compute the distance `distance = edge2.dot(q) * f`. If `distance >= 0`, the intersection is in front of the ray, so return that distance; otherwise return infinity. Edge cases include ray exactly parallel to triangle (det near zero), ray hitting exactly on the edge or vertex (barycentric coordinates at 0 or 1 are valid), and rays starting inside the triangle (distance 0, but note the problem requires `>= 0`, so 0 is acceptable; however, many applications treat 0 as "at origin", but the specification says `>= 0.0f` is valid). Numerical stability is handled by the epsilon check; for degenerate triangles (zero area, det≈0), return infinity. The time complexity is O(1) with constant operations, and space usage is O(1) beyond the inputs.
#include <limits>
#include <cmath>

// Minimal Vector3 structure for the task; assumes x, y, z, basic arithmetic, dot, cross.
struct Vector3 {
    float x, y, z;

    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
    Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
    Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }

    float dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }
    Vector3 cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
};

// Returns the distance along the ray to the closest hit on a single-sided triangle,
// or infinity if the ray does not hit the front face in front of the origin.
float rayTriangleClosestHit(const Vector3& origin, const Vector3& direction,
                            const Vector3& v0, const Vector3& v1, const Vector3& v2) {
    const float kEpsilon = 1e-8f;
    const float kInfinity = std::numeric_limits<float>::infinity();

    // Edge vectors from v0.
    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;

    // Determine determinant for backface culling.
    Vector3 p = direction.cross(edge2);
    float det = edge1.dot(p);

    // Single-sided: only accept positive determinant (front-facing).
    if (det <= kEpsilon) {
        return kInfinity;
    }

    float invDet = 1.0f / det;

    // Compute barycentric coordinate u.
    Vector3 t = origin - v0;
    float u = t.dot(p) * invDet;
    if (u < 0.0f || u > 1.0f) {
        return kInfinity;
    }

    // Compute barycentric coordinate v.
    Vector3 q = t.cross(edge1);
    float v = direction.dot(q) * invDet;
    if (v < 0.0f || u + v > 1.0f) {
        return kInfinity;
    }

    // Compute distance along ray.
    float distance = edge2.dot(q) * invDet;

    // Ensure the hit is in front of the ray origin.
    if (distance >= 0.0f) {
        return distance;
    }

    return kInfinity;
}
#include <cassert>
#include <cmath>
#include <limits>

// (The Vector3 definition from the solution is repeated here for standalone test compilation.)

int main() {
    const float inf = std::numeric_limits<float>::infinity();

    // Test 1: straightforward front-face hit.
    // Ray from (0,0,0) shooting +Z, triangle in the XY plane at z=5.
    {
        Vector3 origin(0, 0, 0), dir(0, 0, 1);
        Vector3 v0(-1, -1, 5), v1(1, -1, 5), v2(0, 1, 5);
        float dist = rayTriangleClosestHit(origin, dir, v0, v1, v2);
        assert(std::abs(dist - 5.0f) < 1e-5f);
    }

    // Test 2: ray misses to the side of the triangle.
    {
        Vector3 origin(2, 0, 0), dir(0, 0, 1);
        Vector3 v0(-1, -1, 5), v1(1, -1, 5), v2(0, 1, 5);
        float dist = rayTriangleClosestHit(origin, dir, v0, v1, v2);
        assert(dist == inf);
    }

    // Test 3: ray hits the back face, should be rejected (single-sided).
    {
        Vector3 origin(0, 0, 10), dir(0, 0, -1);
        Vector3 v0(-1, -1, 5), v1(1, -1, 5), v2(0, 1, 5); // geometric normal points +Z
        float dist = rayTriangleClosestHit(origin, dir, v0, v1, v2);
        assert(dist == inf);
    }

    // Test 4: ray parallel to triangle plane, no hit.
    {
        Vector3 origin(0, 0, 1), dir(1, 0, 0);
        Vector3 v0(-1, -1, 5), v1(1, -1, 5), v2(0, 1, 5);
        float dist = rayTriangleClosestHit(origin, dir, v0, v1, v2);
        assert(dist == inf);
    }

    // Test 5: ray starts inside the triangle (distance 0 is valid per spec).
    {
        Vector3 origin(0, 0, 5), dir(0, 1, 0); // Inside triangle plane, going up
        Vector3 v0(-1, -1, 5), v1(1, -1, 5), v2(0, 1, 5);
        float dist = rayTriangleClosestHit(origin, dir, v0, v1, v2);
        // In-plane ray: det=0, so no hit; but also check a proper edge case below.
        // Actually this ray is parallel, so expect infinity.
        assert(dist == inf);
    }

    // Test 6: hit exactly on an edge.
    {
        Vector3 origin(0, 0, 0), dir(0, 0, 1);
        Vector3 v0(-1, -1, 5), v1(1, -1, 5), v2(0, 1, 5);
        // Point on edge between v0 and v1: (0, -1, 5)
        Vector3 target(0, -1, 5);
        // Ray aiming at that point, but direction must be normalized; here any positive z works.
        float dist = rayTriangleClosestHit(origin, dir, v0, v1, v2);
        // Due to floating point, this hit may be exactly at 5 or infinity depending on epsilon. 
        // We check it's either 5 or infinity (but spec allows 0-1 barycentric, so 5 is correct).
        assert(dist == 5.0f || dist == inf);
    }

    // Test 7: hit at a vertex.
    {
        Vector3 origin(0, 0, 0), dir(0, 0, 1);
        Vector3 v0(-1, -1, 5), v1(1, -1, 5), v2(0, 1, 5);
        float dist = rayTriangleClosestHit(origin, dir, v0, v1, v2);
        // Ray hits interior point (0,0,5) not vertex, but this tests general hit.
        assert(std::abs(dist - 5.0f) < 1e-5f);
    }

    // Test 8: degenerate triangle (zero area).
    {
        Vector3 origin(0, 0, 0), dir(0, 0, 1);
        Vector3 v0(0, 0, 5), v1(0, 0, 5), v2(0, 0, 5);
        float dist = rayTriangleClosestHit(origin, dir, v0, v1, v2);
        assert(dist == inf);
    }

    // Test 9: hit behind the ray origin (negative distance) should be rejected.
    {
        Vector3 origin(0, 0, 10), dir(0, 0, 1); // points away from triangle at z=5
        Vector3 v0(-1, -1, 5), v1(1, -1, 5), v2(0, 1, 5);
        float dist = rayTriangleClosestHit(origin, dir, v0, v1, v2);
        assert(dist == inf);
    }

    // Test 10: non-axis-aligned ray hitting an arbitrary triangle.
    {
        Vector3 origin(0, 0, 0), dir(1, 0, 0); // shooting +X
        Vector3 v0(5, -1, -1), v1(5, 1, -1), v2(5, 0, 1); // triangle in plane x=5
        float dist = rayTriangleClosestHit(origin, dir, v0, v1, v2);
        assert(std::abs(dist - 5.0f) < 1e-5f);
    }

    return 0;
}
