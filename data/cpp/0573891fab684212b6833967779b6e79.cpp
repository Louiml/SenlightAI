Write a C++ function named `countVisibleTriangles` that takes a vector of triangles, where each triangle is represented as a `std::array<float, 9>` containing three 3D vertices (x,y,z per vertex, flattened in order), and a viewer position represented as a `std::array<float, 3>`. The function returns the number of triangles that are potentially visible from the viewer, meaning the triangle’s front face points toward the viewer. A triangle is considered front-facing if the dot product between its geometric normal (computed via cross product of two edge vectors, ensuring counter-clockwise winding) and the vector from the viewer to any point on the triangle (use the centroid) is positive. If the triangle is degenerate (zero area or two vertices coincident), it is ignored. The function must handle any number of triangles, return an integer, and be robust to floating-point precision by using a small epsilon tolerance (e.g., 1e-9) for both the area check and the dot product comparison.

#include <cassert>
#include <vector>
#include <array>

int countVisibleTriangles(
    const std::vector<std::array<float, 9>>& triangles,
    const std::array<float, 3>& viewer);

int main() {
    // Triangle facing +Z, viewer at origin looking toward +Z.
    // Triangle vertices: (0,0,1), (1,0,1), (0,1,1). Normal should be +Z.
    std::vector<std::array<float, 9>> tris1 = {
        {{0,0,1, 1,0,1, 0,1,1}}
    };
    std::array<float, 3> viewer1 = {0,0,0};
    assert(countVisibleTriangles(tris1, viewer1) == 1);

    // Triangle facing -Z, viewer at origin. Should be invisible.
    std::vector<std::array<float, 9>> tris2 = {
        {{0,0,-1, 0,1,-1, 1,0,-1}} // reversed winding to get normal -Z
    };
    std::array<float, 3> viewer2 = {0,0,0};
    assert(countVisibleTriangles(tris2, viewer2) == 0);

    // Degenerate triangle (collinear points) should be skipped.
    std::vector<std::array<float, 9>> tris3 = {
        {{0,0,0, 1,1,1, 2,2,2}},
        {{0,0,1, 1,0,1, 0,1,1}}
    };
    std::array<float, 3> viewer3 = {0,0,0};
    assert(countVisibleTriangles(tris3, viewer3) == 1);

    // Viewer inside a cube of front-facing triangles: all 6 faces visible.
    // For simplicity, use 6 triangles as a tetrahedron? Use a simple box with outward normals.
    std::vector<std::array<float, 9>> tris4 = {
        // Front face (z=1) facing +Z: vertices CCW when viewed from +Z
        {{0,0,1, 1,0,1, 0,1,1}},
        // Back face (z=-1) facing -Z: reversed winding
        {{0,0,-1, 0,1,-1, 1,0,-1}},
        // Right face (x=1) facing +X: CCW when viewed from +X
        {{1,0,0, 1,0,1, 1,1,0}},
        // Left face (x=0) facing -X: reversed
        {{0,0,0, 0,1,0, 0,0,1}},
        // Top face (y=1) facing +Y
        {{0,1,0, 0,1,1, 1,1,0}},
        // Bottom face (y=0) facing -Y
        {{0,0,0, 1,0,0, 0,0,1}}
    };
    std::array<float, 3> viewer4 = {0.5f, 0.5f, 0.5f};
    assert(countVisibleTriangles(tris4, viewer4) == 6);

    // Viewer exactly on triangle plane: dot ≈ 0, should not count.
    std::vector<std::array<float, 9>> tris5 = {
        {{0,0,0, 1,0,0, 0,1,0}} // triangle in z=0 plane, normal +Z
    };
    std::array<float, 3> viewer5 = {0,0,0}; // lies on plane, centroid (1/3,1/3,0)
    assert(countVisibleTriangles(tris5, viewer5) == 0);

    // Empty input
    std::vector<std::array<float, 9>> tris6;
    std::array<float, 3> viewer6 = {0,0,0};
    assert(countVisibleTriangles(tris6, viewer6) == 0);

    return 0;
}

#include <array>
#include <vector>

// Count front-facing triangles from a given viewer position.
// Each triangle is stored as {x0,y0,z0, x1,y1,z1, x2,y2,z2}.
// Viewer is {x,y,z}.
// Returns the number of triangles whose geometric normal (assumed outward)
// has a positive dot product with the vector from viewer to triangle centroid.
int countVisibleTriangles(
    const std::vector<std::array<float, 9>>& triangles,
    const std::array<float, 3>& viewer)
{
    const float epsilon = 1e-9f;
    int visible = 0;

    for (const auto& tri : triangles) {
        // Extract vertices
        float x0 = tri[0], y0 = tri[1], z0 = tri[2];
        float x1 = tri[3], y1 = tri[4], z1 = tri[5];
        float x2 = tri[6], y2 = tri[7], z2 = tri[8];

        // Edge vectors: e1 = v1 - v0, e2 = v2 - v0
        float ex1 = x1 - x0, ey1 = y1 - y0, ez1 = z1 - z0;
        float ex2 = x2 - x0, ey2 = y2 - y0, ez2 = z2 - z0;

        // Compute normal = cross(e1, e2)
        float nx = ey1 * ez2 - ez1 * ey2;
        float ny = ez1 * ex2 - ex1 * ez2;
        float nz = ex1 * ey2 - ey1 * ex2;

        // Check for degenerate triangle: normal length squared < epsilon^2
        float len_sq = nx * nx + ny * ny + nz * nz;
        if (len_sq < epsilon * epsilon) continue;

        // Centroid
        float cx = (x0 + x1 + x2) / 3.0f;
        float cy = (y0 + y1 + y2) / 3.0f;
        float cz = (z0 + z1 + z2) / 3.0f;

        // Vector from viewer to centroid
        float vx = cx - viewer[0];
        float vy = cy - viewer[1];
        float vz = cz - viewer[2];

        // Dot product normal · (viewer->centroid)
        float dot = nx * vx + ny * vy + nz * vz;

        if (dot > epsilon) ++visible;
    }

    return visible;
}

// The solution computes the geometric normal of each triangle by taking the cross product of two edge vectors: `(v1 - v0)` and `(v2 - v0)`. If the normal’s length squared is less than a small epsilon, the triangle is degenerate and skipped. Then compute the centroid as the average of the three vertices, and form the vector from the viewer to the centroid. Compute the dot product between the normal and this vector. A positive dot product (greater than epsilon) means the normal points toward the viewer, so the triangle is front-facing and counted. Edge cases include: degenerate triangles (zero area) being ignored; a dot product exactly zero meaning the triangle is edge-on (not counted); floating-point rounding causing near-zero dot products—use epsilon to avoid counting borderline cases. The algorithm runs in O(n) time where n is the number of triangles, and uses O(1) auxiliary space per triangle (no extra storage beyond local variables).
