// Write a C++ function named `sphereTriangleIntersectionPoint` that determines whether a 3D unit sphere centered at the origin intersects a triangle in 3D space, and if it does, returns the intersection point on the triangle's surface closest to the sphere's center. The function must accept three triangle vertices as `glm::vec3` parameters and a reference to a `glm::vec3` output parameter for the intersection point. It should return a `bool` (true if an intersection exists, false otherwise). The algorithm should treat the sphere as a solid (including its interior) and consider triangle edges and vertices as valid intersection surfaces. This must be implemented as a free function, not a method, and must not rely on GLM's built-in intersection functions — you must implement the geometric test manually.

#include <glm/vec3.hpp>
#include <cassert>
#include <cmath>

// Declaration of the function under test (assume it's in the same translation unit or included)
bool sphereTriangleIntersectionPoint(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, glm::vec3& intersectionPoint);

int main() {
    // Test 1: Triangle far away, no intersection
    glm::vec3 hit;
    bool result = sphereTriangleIntersectionPoint(glm::vec3(10, 0, 0), glm::vec3(10, 1, 0), glm::vec3(10, 0, 1), hit);
    assert(result == false);

    // Test 2: Triangle very close to origin, intersection at closest point
    result = sphereTriangleIntersectionPoint(glm::vec3(0.5f, 0, 0), glm::vec3(0.5f, 1, 0), glm::vec3(0.5f, 0, 1), hit);
    assert(result == true);
    // Closest point on triangle to origin should be (0.5, 0, 0)
    assert(fabs(hit.x - 0.5f) < 1e-4f && fabs(hit.y) < 1e-4f && fabs(hit.z) < 1e-4f);

    // Test 3: A triangle whose plane is tangent to sphere (distance exactly 1)
    result = sphereTriangleIntersectionPoint(glm::vec3(1, 0, 0), glm::vec3(1, 1, 0), glm::vec3(1, 0, 1), hit);
    assert(result == true);
    assert(fabs(hit.x - 1.0f) < 1e-4f && fabs(hit.y) < 1e-4f && fabs(hit.z) < 1e-4f);

    // Test 4: Triangle that contains the origin (sphere center inside triangle)
    result = sphereTriangleIntersectionPoint(glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), hit);
    assert(result == true);
    // Closest point is origin itself
    assert(fabs(hit.x) < 1e-4f && fabs(hit.y) < 1e-4f && fabs(hit.z) < 1e-4f);

    // Test 5: Edge is just at distance 1 from origin
    result = sphereTriangleIntersectionPoint(glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), glm::vec3(2, 1, 0), hit);
    assert(result == true);
    // The closest point on the edge from (0,1,0) to (1,0,0) to origin is (0.5,0.5,0) at distance sqrt(0.5) < 1
    assert(fabs(hit.x - 0.5f) < 1e-4f && fabs(hit.y - 0.5f) < 1e-4f && fabs(hit.z) < 1e-4f);

    // Test 6: Degenerate triangle (collinear points) slightly beyond sphere
    result = sphereTriangleIntersectionPoint(glm::vec3(2, 0, 0), glm::vec3(2, 1, 0), glm::vec3(2, 2, 0), hit);
    assert(result == false);

    // Test 7: Degenerate triangle collinear but one point near origin
    result = sphereTriangleIntersectionPoint(glm::vec3(-0.5f, 0, 0), glm::vec3(0.5f, 0, 0), glm::vec3(1.5f, 0, 0), hit);
    assert(result == true);
    // Closest point on segment to origin is (0,0,0)
    assert(fabs(hit.x) < 1e-4f && fabs(hit.y) < 1e-4f && fabs(hit.z) < 1e-4f);

    // Test 8: Triangle entire outside but very close (distance just above 1)
    result = sphereTriangleIntersectionPoint(glm::vec3(1.01f, 0, 0), glm::vec3(1.01f, 1, 0), glm::vec3(1.01f, 0, 1), hit);
    assert(result == false);
}

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <algorithm>

/**
 * Computes the closest point on a line segment (A-B) to point P.
 * Returns the closest point as a glm::vec3.
 */
glm::vec3 closestPointOnSegment(const glm::vec3& a, const glm::vec3& b, const glm::vec3& p) {
    glm::vec3 ab = b - a;
    float t = glm::dot(p - a, ab) / glm::dot(ab, ab);
    t = std::clamp(t, 0.0f, 1.0f);
    return a + t * ab;
}

/**
 * Computes the closest point on a triangle (v0, v1, v2) to point P.
 * Uses barycentric coordinates and edge clamping. Assumes non-degenerate triangle.
 */
glm::vec3 closestPointOnTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& p) {
    // Compute the normal of the triangle
    glm::vec3 e0 = v1 - v0;
    glm::vec3 e1 = v2 - v0;
    glm::vec3 normal = glm::cross(e0, e1);
    float area2 = glm::length(normal);
    if (area2 < 1e-8f) {
        // Degenerate triangle: fall back to closest point on longest edge
        float d01 = glm::distance2(v0, v1);
        float d12 = glm::distance2(v1, v2);
        float d20 = glm::distance2(v2, v0);
        if (d01 >= d12 && d01 >= d20) return closestPointOnSegment(v0, v1, p);
        if (d12 >= d20) return closestPointOnSegment(v1, v2, p);
        return closestPointOnSegment(v2, v0, p);
    }
    normal = glm::normalize(normal);

    // Project P onto the triangle's plane
    glm::vec3 proj = p - (glm::dot(normal, p - v0) * normal);

    // Compute barycentric coordinates of proj relative to triangle
    glm::vec3 n1 = glm::cross(v1 - proj, v2 - proj);
    glm::vec3 n2 = glm::cross(v2 - proj, v0 - proj);
    glm::vec3 n3 = glm::cross(v0 - proj, v1 - proj);
    float invArea2 = 1.0f / area2;
    float b0 = glm::dot(n1, normal) * invArea2;
    float b1 = glm::dot(n2, normal) * invArea2;
    float b2 = glm::dot(n3, normal) * invArea2;

    // Check if proj is inside triangle (all barycentric coordinates non-negative)
    if (b0 >= 0.0f && b1 >= 0.0f && b2 >= 0.0f) {
        return proj;
    }

    // Otherwise, find closest point on each edge and pick the one with minimal distance
    glm::vec3 closest = closestPointOnSegment(v0, v1, p);
    float minDist = glm::distance2(closest, p);
    glm::vec3 candidate = closestPointOnSegment(v1, v2, p);
    float d = glm::distance2(candidate, p);
    if (d < minDist) {
        minDist = d;
        closest = candidate;
    }
    candidate = closestPointOnSegment(v2, v0, p);
    d = glm::distance2(candidate, p);
    if (d < minDist) {
        closest = candidate;
    }
    return closest;
}

/**
 * Determines whether a unit sphere at the origin intersects the given triangle.
 * If there is an intersection, sets 'intersectionPoint' to the point on the triangle
 * closest to the sphere center (which is the origin, so distance <= 1).
 * Returns true on intersection, false otherwise.
 */
bool sphereTriangleIntersectionPoint(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, glm::vec3& intersectionPoint) {
    // Closest point on triangle to origin
    glm::vec3 closest = closestPointOnTriangle(v0, v1, v2, glm::vec3(0.0f));

    // Check distance from origin to that closest point
    float distSq = glm::dot(closest, closest);
    if (distSq <= 1.0f) {
        intersectionPoint = closest;
        return true;
    }
    return false;
}

// The core idea is to treat the sphere as a point (the origin) and the triangle as an expanded geometric region based on the Minkowski sum of the triangle with a unit sphere. However, a simpler and numerically stable approach used in collision detection is to compute the closest point on the triangle to the sphere's center, then check if that distance is ≤ 1. If it is, the sphere intersects the triangle. The closest point on a triangle to a given point `P` (here the origin) is found as follows: first compute the point's projection onto the triangle's plane, then determine if that projection lies inside the triangle using barycentric coordinates. If inside, the projection is the closest point. If outside, the closest point lies on one of the triangle's three edges; compute the closest point on each edge segment to `P` and take the minimum distance. The function computes distances squared (`length2`) to avoid sqrt until the final comparison. If the closest point is within distance 1 (inclusive), compute the actual intersection point (the closest point itself, since the sphere is solid and center at origin), store it in the output reference, and return true. Edge cases include when the triangle is degenerate (zero area); the closest point is then the closest point on the longest edge or a vertex. Also, if the sphere's center is exactly inside the triangle, the closest point is the origin itself, distance 0, so intersection is trivially true. Time complexity is O(1) since we perform a fixed number of geometric operations; space complexity is O(1) as only a few intermediate vectors are used.
