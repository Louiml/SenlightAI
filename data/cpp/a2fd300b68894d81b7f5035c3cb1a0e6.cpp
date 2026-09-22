Write a standalone C++ function that determines whether a sphere and a triangle in 3D space are in contact, given the sphere's center and radius, and three triangle vertices. The function must return `true` if the distance from the sphere center to the closest point on the triangle is less than the sphere radius plus a given contact breaking threshold, and `false` otherwise. The triangle may be degenerate (zero‑area), and the sphere center may be anywhere in space, including exactly on the triangle plane or inside the triangle. The function must be robust to floating‑point edge cases such as near‑zero distances and degenerate edges.
#include <cassert>
#include <cmath>

int main() {
    // Basic: sphere far away
    Vec3 v0{0,0,0}, v1{1,0,0}, v2{0,1,0};
    assert(!sphereTriangleContact({0,0,10}, 1.0, v0,v1,v2, 0.0));
    
    // Sphere touching triangle plane from above (distance = 1, radius = 1)
    assert(sphereTriangleContact({0.5,0.5,1.0}, 1.0, v0,v1,v2, 0.0));
    
    // Sphere slightly below threshold
    assert(!sphereTriangleContact({0.5,0.5,2.0}, 1.0, v0,v1,v2, 0.5));
    assert(sphereTriangleContact({0.5,0.5,2.0}, 1.0, v0,v1,v2, 1.0));
    
    // Sphere center exactly on the triangle plane but outside edges: use threshold
    assert(!sphereTriangleContact({2.0,2.0,0.0}, 0.1, v0,v1,v2, 0.0));
    assert(sphereTriangleContact({2.0,2.0,0.0}, 0.1, v0,v1,v2, 2.0));
    
    // Sphere center inside triangle (distance zero)
    assert(sphereTriangleContact({0.2,0.2,0.0}, 0.5, v0,v1,v2, 0.0));
    
    // Degenerate triangle (collinear points)
    Vec3 d0{0,0,0}, d1{1,0,0}, d2{2,0,0};
    assert(sphereTriangleContact({0.5,0.0,0.0}, 0.5, d0,d1,d2, 0.0));
    assert(!sphereTriangleContact({0.5,0.0,1.0}, 0.5, d0,d1,d2, 0.0));
    assert(sphereTriangleContact({0.5,0.0,1.0}, 0.6, d0,d1,d2, 0.0));
    
    // Degenerate triangle (all same point)
    Vec3 p0{1,2,3}, p1{1,2,3}, p2{1,2,3};
    assert(sphereTriangleContact({1,2,3}, 1.0, p0,p1,p2, 0.0));
    assert(!sphereTriangleContact({1,2,4}, 0.5, p0,p1,p2, 0.0));
    
    // Sphere with radius zero, exactly on vertex
    assert(sphereTriangleContact({0,0,0}, 0.0, v0,v1,v2, 0.0));
    assert(!sphereTriangleContact({0.01,0.01,0.01}, 0.0, v0,v1,v2, 0.0));

    return 0;
}
#include <cmath>
#include <array>
#include <algorithm>

// Minimal 3D vector struct for this task (could be replaced by an external library).
struct Vec3 {
    double x, y, z;

    Vec3 operator-(const Vec3& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }
    Vec3 operator+(const Vec3& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }
    Vec3 operator*(double s) const {
        return {x * s, y * s, z * s};
    }
    Vec3 cross(const Vec3& other) const {
        return {y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x};
    }
    double dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    double lengthSq() const {
        return dot(*this);
    }
    double length() const {
        return std::sqrt(lengthSq());
    }
    void normalize() {
        double len = length();
        if (len > 1e-12) {
            x /= len; y /= len; z /= len;
        }
    }
};

// Compute squared distance from point p to segment from a to b, and store closest point in nearest.
static double segmentSqrDistance(const Vec3& a, const Vec3& b, const Vec3& p, Vec3& nearest) {
    Vec3 ab = b - a;
    Vec3 ap = p - a;
    double t = ap.dot(ab);
    if (t <= 0.0) {
        nearest = a;
        return ap.lengthSq();
    }
    double lenSq = ab.lengthSq();
    if (t >= lenSq) {
        nearest = b;
        return (p - b).lengthSq();
    }
    t /= lenSq;
    nearest = a + ab * t;
    return (p - nearest).lengthSq();
}

// Determine if sphere (center, radius) is in contact with triangle (v0,v1,v2) within threshold.
bool sphereTriangleContact(const Vec3& sphereCenter, double radius,
                           const Vec3& v0, const Vec3& v1, const Vec3& v2,
                           double contactBreakingThreshold = 0.0) {
    const double radiusWithThreshold = radius + contactBreakingThreshold;
    const double radiusSq = radiusWithThreshold * radiusWithThreshold;

    // Compute triangle normal
    Vec3 normal = (v1 - v0).cross(v2 - v0);
    const double normalLenSq = normal.lengthSq();
    const double eps = 1e-12;

    Vec3 closest;

    if (normalLenSq > eps * eps) {
        // Non-degenerate triangle
        normal.normalize();

        // Project sphere center onto plane
        Vec3 p0ToCenter = sphereCenter - v0;
        double distanceFromPlane = p0ToCenter.dot(normal);
        Vec3 projection = sphereCenter - normal * distanceFromPlane;

        // Point-in-triangle test using edge cross products (same as supplied code)
        Vec3 edge1 = v1 - v0;
        Vec3 edge2 = v2 - v1;
        Vec3 edge3 = v0 - v2;

        Vec3 p1 = projection - v0;
        Vec3 p2 = projection - v1;
        Vec3 p3 = projection - v2;

        Vec3 e1n = edge1.cross(normal);
        Vec3 e2n = edge2.cross(normal);
        Vec3 e3n = edge3.cross(normal);

        double r1 = e1n.dot(p1);
        double r2 = e2n.dot(p2);
        double r3 = e3n.dot(p3);

        bool inside = (r1 > 0 && r2 > 0 && r3 > 0) || (r1 <= 0 && r2 <= 0 && r3 <= 0);

        if (inside) {
            closest = projection;
        } else {
            // Find closest point on each edge, take the min distance
            Vec3 nearestEdge;
            double bestDistSq = -1.0;
            for (const auto& edge : {std::pair{v0,v1}, std::pair{v1,v2}, std::pair{v2,v0}}) {
                double d = segmentSqrDistance(edge.first, edge.second, sphereCenter, nearestEdge);
                if (bestDistSq < 0 || d < bestDistSq) {
                    bestDistSq = d;
                    closest = nearestEdge;
                }
            }
        }
    } else {
        // Degenerate triangle: treat as collection of edges (or a point if all vertices equal)
        Vec3 nearestEdge;
        double bestDistSq = -1.0;
        for (const auto& edge : {std::pair{v0,v1}, std::pair{v1,v2}, std::pair{v2,v0}}) {
            double d = segmentSqrDistance(edge.first, edge.second, sphereCenter, nearestEdge);
            if (bestDistSq < 0 || d < bestDistSq) {
                bestDistSq = d;
                closest = nearestEdge;
            }
        }
    }

    // Check distance from sphere center to closest point
    double distSq = (sphereCenter - closest).lengthSq();
    return distSq <= radiusSq + 1e-9;  // small epsilon to handle rounding
}
// The solution computes the closest point on a triangle to the sphere center, then checks whether that distance is within the threshold. The main algorithm has three stages:  
// 1. **Project onto triangle plane**: Compute the triangle normal via the cross product of two edges. If the normal is (nearly) zero, the triangle is degenerate; in that case, treat it as a line segment or a point (which is handled by the edge/vertex checks in stage 2). Otherwise normalize the normal and project the sphere center onto the plane.  
// 2. **Check if projection is inside the triangle**: Use a point‑in‑triangle test based on edge‑normal cross products (similar to the provided `pointInTriangle`). If inside, the closest point is the projection.  
// 3. **If projection is outside**: Find the closest point on each of the three edges using a segment‑point distance formula, and take the minimum distance point.  
// Then compute the squared distance from the sphere center to the closest point and compare against `(radius + threshold)^2`. If the distance is zero (center exactly on the triangle), still return true if the sphere radius is non‑negative (it always is). Edge cases include degenerate triangles (zero area) where the normal is undefined; the algorithm then simply evaluates all three edges and picks the closest point, which correctly handles a triangle collapsed to a line or a point. Also, if the sphere center lies exactly inside the triangle, the distance is zero and contact is immediate if radius + threshold > 0. Time complexity is O(1) constant work, space complexity is O(1).
