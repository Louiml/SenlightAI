/*
In a physics engine, an infinite static plane is often represented by a normal vector and a signed distance from the origin. Write a C++ function that, given a 3D plane defined by a normalized normal vector `n` (with components `nx, ny, nz`) and a scalar constant `d` (such that the plane is the set of points `p` satisfying `n·p = d`), and given an axis-aligned bounding box (AABB) specified by its minimum corner `minPt` and maximum corner `maxPt` (where each component of `minPt` is ≤ the corresponding component of `maxPt`), computes a triangle mesh that approximates the intersection of the plane with that AABB. The function should return a `std::vector<std::array<double,3>>` of vertices forming triangles (each consecutive triple of vertices is one triangle). The mesh should consist of exactly two triangles that together form a square patch lying exactly on the plane and centered at the projection of the AABB center onto the plane, with side length equal to the diagonal length of the AABB (i.e., the distance between `minPt` and `maxPt`). The two triangles must share a diagonal, and the vertices must be ordered so that the triangles have consistent orientation (e.g., counter‑clockwise when viewed from the side that the plane normal points toward). Assume the input normal is already normalized, and do not modify it. The function signature is:
```cpp
std::vector<std::array<double,3>> planePatchTriangles(
    const std::array<double,3>& n, double d,
    const std::array<double,3>& minPt, const std::array<double,3>& maxPt);
```
You may include necessary standard headers. Your implementation must be self‑contained (no external math library) and should not call any other helper functions unless you define them yourself.
*/
#include <array>
#include <vector>
#include <cmath>
#include <algorithm>

// Return two triangles forming a square patch on the plane defined by n·p = d.
// The square is centered at the projection of the AABB center onto the plane.
// The side length equals the AABB diagonal length.
std::vector<std::array<double,3>> planePatchTriangles(
    const std::array<double,3>& n, double d,
    const std::array<double,3>& minPt, const std::array<double,3>& maxPt) {
    // Compute AABB center and half-diagonal (radius).
    double cx = (minPt[0] + maxPt[0]) * 0.5;
    double cy = (minPt[1] + maxPt[1]) * 0.5;
    double cz = (minPt[2] + maxPt[2]) * 0.5;
    double dx = maxPt[0] - minPt[0];
    double dy = maxPt[1] - minPt[1];
    double dz = maxPt[2] - minPt[2];
    double radius = 0.5 * std::sqrt(dx*dx + dy*dy + dz*dz);

    // Project the center onto the plane along the normal.
    double dot_center = n[0]*cx + n[1]*cy + n[2]*cz;
    double t = dot_center - d;
    double px = cx - t * n[0];
    double py = cy - t * n[1];
    double pz = cz - t * n[2];

    // Build two orthonormal tangent vectors in the plane.
    // Choose the axis with the smallest absolute normal component for cross.
    std::array<double,3> tangent0, tangent1;
    double ax = std::abs(n[0]), ay = std::abs(n[1]), az = std::abs(n[2]);
    if (ax <= ay && ax <= az) {
        // Cross (1,0,0) with n.
        tangent0 = {0.0, -n[2], n[1]};
    } else if (ay <= ax && ay <= az) {
        // Cross (0,1,0) with n.
        tangent0 = {n[2], 0.0, -n[0]};
    } else {
        // Cross (0,0,1) with n.
        tangent0 = {-n[1], n[0], 0.0};
    }
    // Normalize tangent0.
    double len0 = std::sqrt(tangent0[0]*tangent0[0] + tangent0[1]*tangent0[1] + tangent0[2]*tangent0[2]);
    tangent0[0] /= len0; tangent0[1] /= len0; tangent0[2] /= len0;
    // tangent1 = n cross tangent0.
    tangent1 = {n[1]*tangent0[2] - n[2]*tangent0[1],
                n[2]*tangent0[0] - n[0]*tangent0[2],
                n[0]*tangent0[1] - n[1]*tangent0[0]};

    // Precompute scaled tangent vectors.
    double tx0 = tangent0[0] * radius, ty0 = tangent0[1] * radius, tz0 = tangent0[2] * radius;
    double tx1 = tangent1[0] * radius, ty1 = tangent1[1] * radius, tz1 = tangent1[2] * radius;

    // Four corners: center ± t0 ± t1.
    auto add = [&](double sx, double sy, double sz, std::vector<std::array<double,3>>& verts) {
        verts.push_back({px + sx*tx0 + sy*tx1,
                         py + sx*ty0 + sy*ty1,
                         pz + sx*tz0 + sy*tz1});
    };

    std::vector<std::array<double,3>> result;
    // First triangle: bottom-left ( -1, -1 ), bottom-right ( +1, -1 ), top-right ( +1, +1 ).
    add(-1.0, -1.0, 0.0, result);
    add( 1.0, -1.0, 0.0, result);
    add( 1.0,  1.0, 0.0, result);
    // Second triangle: bottom-left ( -1, -1 ), top-right ( +1, +1 ), top-left ( -1, +1 ).
    add(-1.0, -1.0, 0.0, result);
    add( 1.0,  1.0, 0.0, result);
    add(-1.0,  1.0, 0.0, result);
    return result;
}
#include <cassert>
#include <cmath>
#include <array>
#include <vector>

// Include the solution function here (or copy it above).

int main() {
    // Test 1: axis-aligned plane, normal along Z.
    {
        auto tri = planePatchTriangles({0,0,1}, 0.0, {-1,-1,-1}, {1,1,1});
        assert(tri.size() == 6);
        // All vertices must be on plane z=0.
        for (const auto& v : tri) {
            assert(std::abs(v[2]) < 1e-9);
        }
        // Diagonal length squared of AABB is (2^2+2^2+2^2)=12, radius=sqrt(12)/2.
        // The distance between any two opposite corners of the square should be 2*radius.
        double expected_side = std::sqrt(12.0);
        double dist01 = std::sqrt( (tri[0][0]-tri[1][0])*(tri[0][0]-tri[1][0]) +
                                   (tri[0][1]-tri[1][1])*(tri[0][1]-tri[1][1]) +
                                   (tri[0][2]-tri[1][2])*(tri[0][2]-tri[1][2]) );
        assert(std::abs(dist01 - expected_side) < 1e-9);
    }

    // Test 2: unit normal, plane constant not zero.
    {
        double inv = 1.0/std::sqrt(3.0);
        std::array<double,3> n = {inv, inv, inv};
        auto tri = planePatchTriangles(n, 5.0, {0,0,0}, {2,2,2});
        assert(tri.size() == 6);
        // Each vertex must satisfy n·v = 5.
        for (const auto& v : tri) {
            double dot = n[0]*v[0] + n[1]*v[1] + n[2]*v[2];
            assert(std::abs(dot - 5.0) < 1e-9);
        }
        // Check that the two triangles are non-degenerate and share a diagonal.
        // Compute area of first triangle (should be >0).
        auto cross = [](const std::array<double,3>& a, const std::array<double,3>& b,
                        const std::array<double,3>& c) -> double {
            double ux = b[0]-a[0], uy = b[1]-a[1], uz = b[2]-a[2];
            double vx = c[0]-a[0], vy = c[1]-a[1], vz = c[2]-a[2];
            double cx = uy*vz - uz*vy;
            double cy = uz*vx - ux*vz;
            double cz = ux*vy - uy*vx;
            return std::sqrt(cx*cx + cy*cy + cz*cz);
        };
        double area1 = 0.5 * cross(tri[0], tri[1], tri[2]);
        double area2 = 0.5 * cross(tri[3], tri[4], tri[5]);
        assert(area1 > 0.0 && area2 > 0.0);
        // The two triangles should share a diagonal; check vertex 0 equals vertex 3 and vertex 2 equals vertex 4.
        assert(tri[0] == tri[3]);
        assert(tri[2] == tri[4]);
    }

    // Test 3: degenerate AABB (single point).
    {
        auto tri = planePatchTriangles({1,0,0}, 3.0, {3,2,1}, {3,2,1});
        assert(tri.size() == 6);
        // All vertices identical to projection of point (3,2,1) onto plane x=3 -> (3,2,1).
        for (const auto& v : tri) {
            assert(v[0] == 3.0 && v[1] == 2.0 && v[2] == 1.0);
        }
    }

    // Test 4: normal along X, box not symmetric.
    {
        auto tri = planePatchTriangles({1,0,0}, 1.0, {0,0,0}, {2,4,6});
        assert(tri.size() == 6);
        // All x coordinates must be 1.
        for (const auto& v : tri) {
            assert(std::abs(v[0] - 1.0) < 1e-9);
        }
        // The side length should equal the diagonal of the box: sqrt(2^2+4^2+6^2).
        double diag = std::sqrt(4.0+16.0+36.0);
        // Distance between corners 0 and 1 is a side length.
        double side = std::sqrt( (tri[0][1]-tri[1][1])*(tri[0][1]-tri[1][1]) +
                                 (tri[0][2]-tri[1][2])*(tri[0][2]-tri[1][2]) );
        assert(std::abs(side - diag) < 1e-9);
    }

    // Test 5: negative normal.
    {
        auto tri = planePatchTriangles({0,-1,0}, -2.0, {-1,-5,-1}, {1,-3,1});
        assert(tri.size() == 6);
        for (const auto& v : tri) {
            assert(std::abs(v[1] - (-2.0)) < 1e-9); // plane y = -2.
        }
    }

    return 0;
}
// The core idea is to construct a square patch on the plane that is large enough to cover the AABB. The bounding box’s diagonal gives a worst‑case radius: any point inside the box is at most that distance from the center. So we take the center of the AABB, project it onto the plane along the plane normal (this gives the point on the plane closest to the box center). Then we build a square centered at that projected point, with side length equal to the AABB diagonal (so the half‑side is the radius). To define the square’s orientation, we need two perpendicular unit vectors lying in the plane (tangent directions). A robust way to find them: pick the axis with the smallest absolute component of the normal, cross it with the normal to get one tangent, then cross the normal with that tangent to get a second orthogonal tangent. This guarantees orthonormality. Then the four corners of the square are `center_project ± tangent0 * radius ± tangent1 * radius`. Finally split the square into two triangles sharing a diagonal. To ensure consistent orientation, order the vertices such that the triangle normals point in the plane normal direction. We can do this by choosing a vertex order: for the first triangle, use bottom‑left, bottom‑right, top‑right (relative to the tangent coordinates), and for the second triangle use bottom‑left, top‑right, top‑left. This yields both triangles with counter‑clockwise orientation when viewed from the normal side. Edge cases: the normal might be axis‑aligned (e.g., (1,0,0)), the selection of the least‑component axis still works because the cross product is never zero as long as we avoid the parallel axis. The AABB could be degenerate (min==max), then the diagonal radius is zero, and the square degenerates to a point, but the two triangles still have three identical vertices; this is acceptable. Complexity: O(1) time and O(1) auxiliary space, since we only produce six vertices (two triangles).
