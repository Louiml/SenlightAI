// Write a standalone C++ function named `triangulatePolygon` that takes an array of `Vertex` points (where `Vertex` is a simple struct with `double x` and `double y` members) and the number of vertices `n`, and returns a `std::vector<Triangle>` representing a fan triangulation of a simple polygon whose vertices are given in counter-clockwise order starting from index 0. Each `Triangle` struct must contain three `Vertex` members named `a`, `b`, and `c`. The function should assume the polygon has at least 3 vertices, and produce `n-2` triangles, each formed by the first vertex and two consecutive vertices from the rest of the polygon (e.g., triangle 0 uses vertices 0, 1, 2; triangle 1 uses 0, 2, 3; etc.). The function must be `const`-correct, take the input as a pointer to `Vertex` and a size, and return the triangles by value (using `std::vector`). Do not modify the input. The function should handle any number of vertices ≥ 3, including large counts, and must not leak memory.

The solution replicates the classic "fan triangulation" of a polygon: given vertices in order, we fix the first vertex as the common apex and create one triangle for each consecutive edge that does not include the first vertex. Specifically, for `i` from 1 to `n-2`, the triangle connects vertex[0], vertex[i], and vertex[i+1]. This yields exactly `n-2` triangles, covering the polygon if it is convex (for a general simple polygon, this is a valid triangulation only if the polygon is star-shaped with respect to vertex 0, but the task is to implement the algorithm regardless). Edge cases: if `n < 3`, the function should return an empty vector (or we can assert in the test that n≥3, but the function itself should be robust and return empty for invalid input). The algorithm is straightforward: allocate a `std::vector<Triangle>` of size `n-2` (if n≥3), loop over the range, and assign each triangle's members directly. No dynamic memory management is needed beyond the vector's internal allocation, which is exception-safe. Time complexity is O(n) since we iterate once over the vertices; space complexity is O(n) for the output vector, plus O(1) auxiliary space.

#include <vector>
#include <cstddef>

// Vertex with two-dimensional coordinates.
struct Vertex {
    double x;
    double y;
};

// Triangle defined by three vertices.
struct Triangle {
    Vertex a;
    Vertex b;
    Vertex c;
};

/**
 * Triangulate a polygon using a fan from the first vertex.
 * The polygon is given by an array of vertices in order (e.g., counter-clockwise).
 * Produces (n-2) triangles if n >= 3, otherwise an empty vector.
 * The input array is not modified.
 */
std::vector<Triangle> triangulatePolygon(const Vertex* vertices, std::size_t n) {
    std::vector<Triangle> triangles;
    if (n < 3 || vertices == nullptr) {
        return triangles;
    }

    triangles.reserve(n - 2);
    for (std::size_t i = 1; i < n - 1; ++i) {
        Triangle tri;
        tri.a = vertices[0];
        tri.b = vertices[i];
        tri.c = vertices[i + 1];
        triangles.push_back(tri);
    }
    return triangles;
}

#include <cassert>
#include <cmath>

int main() {
    // Test 1: Square (4 vertices) – expect 2 triangles.
    Vertex square[4] = {{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}};
    auto tris = triangulatePolygon(square, 4);
    assert(tris.size() == 2);
    assert(tris[0].a.x == 0.0 && tris[0].a.y == 0.0);
    assert(tris[0].b.x == 1.0 && tris[0].b.y == 0.0);
    assert(tris[0].c.x == 1.0 && tris[0].c.y == 1.0);
    assert(tris[1].a.x == 0.0 && tris[1].a.y == 0.0);
    assert(tris[1].b.x == 1.0 && tris[1].b.y == 1.0);
    assert(tris[1].c.x == 0.0 && tris[1].c.y == 1.0);

    // Test 2: Triangle (3 vertices) – 1 triangle.
    Vertex tri_pts[3] = {{0.0, 0.0}, {2.0, 0.0}, {1.0, 2.0}};
    auto tri_res = triangulatePolygon(tri_pts, 3);
    assert(tri_res.size() == 1);
    assert(tri_res[0].a.x == 0.0 && tri_res[0].a.y == 0.0);
    assert(tri_res[0].b.x == 2.0 && tri_res[0].b.y == 0.0);
    assert(tri_res[0].c.x == 1.0 && tri_res[0].c.y == 2.0);

    // Test 3: Pentagon (5 vertices) – 3 triangles.
    Vertex pentagon[5] = {{0.0, 0.0}, {1.0, 0.0}, {2.0, 1.0}, {1.0, 2.0}, {0.0, 1.0}};
    auto pent_res = triangulatePolygon(pentagon, 5);
    assert(pent_res.size() == 3);
    // Verify each triangle shares vertex 0 as apex.
    for (const auto& t : pent_res) {
        assert(t.a.x == 0.0 && t.a.y == 0.0);
    }
    assert(pent_res[0].b.x == 1.0 && pent_res[0].b.y == 0.0);
    assert(pent_res[0].c.x == 2.0 && pent_res[0].c.y == 1.0);
    assert(pent_res[1].b.x == 2.0 && pent_res[1].b.y == 1.0);
    assert(pent_res[1].c.x == 1.0 && pent_res[1].c.y == 2.0);
    assert(pent_res[2].b.x == 1.0 && pent_res[2].b.y == 2.0);
    assert(pent_res[2].c.x == 0.0 && pent_res[2].c.y == 1.0);

    // Test 4: Edge case – n < 3 returns empty.
    Vertex two[2] = {{0.0, 0.0}, {1.0, 1.0}};
    auto small = triangulatePolygon(two, 2);
    assert(small.empty());

    // Test 5: Null pointer returns empty.
    auto null_res = triangulatePolygon(nullptr, 5);
    assert(null_res.empty());

    return 0;
}
