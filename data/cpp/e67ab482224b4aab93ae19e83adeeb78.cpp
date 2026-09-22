Given a list of 3D triangles, each defined by three vertices, and a query axis-aligned bounding box (AABB), write a C++ function that returns the indices of all triangles whose axis-aligned bounding box (computed from the triangle’s vertices) overlaps with the query AABB. Overlap is inclusive: touching at edges or corners counts as overlapping. The input is a vector of triangles (each triangle is a `std::array<Vector3, 3>`), a query AABB defined by minimum and maximum corners, and the output is a vector of indices in the original order. You may define a simple `Vector3` struct with `x`, `y`, `z` and a `AABB` struct with `min` and `max` fields. The function must be efficient enough to handle up to 100,000 triangles and many queries, so you should precompute per‑triangle AABBs once per call. The function signature is: `std::vector<int> findOverlappingTriangles(const std::vector<std::array<Vector3,3>>& triangles, const AABB& queryAabb)`.

// The core idea is to avoid testing each triangle’s vertices against the query AABB directly (which would be O(1) per triangle but still require reading all vertices for each query). Instead, precompute the AABB for every triangle once, in O(n) time, and store them in a parallel vector. For a given query, iterate through all triangles, but for each triangle test only its precomputed min and max corners against the query AABB using a standard overlap test: two AABBs overlap if and only if `min1.x <= max2.x && min2.x <= max1.x` for each of the x, y, z axes. This inclusive test handles touching cases correctly. The precomputed AABBs are computed by scanning each triangle’s three vertices to find per‑axis minimum and maximum values, which takes O(1) per triangle (constant number of vertices). The overall time complexity per query is O(n), and precomputation is O(n) once per call; space is O(n) for the stored AABBs. Edge cases include degenerate triangles (all vertices equal or collinear) – the AABB is still valid (zero extent in one or more axes) and the overlap test works naturally. Also, triangles with very large coordinates may lead to floating‑point issues, but using `double` for coordinates and a tolerance-free comparison is acceptable for typical test data.

#include <vector>
#include <array>
#include <algorithm>
#include <cstddef>

// Simple 3D vector struct.
struct Vector3 {
    double x, y, z;
};

// Axis-aligned bounding box defined by its minimum and maximum corners.
struct AABB {
    Vector3 min;
    Vector3 max;
};

// Precompute the AABB for a single triangle.
AABB computeTriangleAabb(const std::array<Vector3, 3>& tri) {
    AABB box;
    box.min = { std::min({tri[0].x, tri[1].x, tri[2].x}),
                std::min({tri[0].y, tri[1].y, tri[2].y}),
                std::min({tri[0].z, tri[1].z, tri[2].z}) };
    box.max = { std::max({tri[0].x, tri[1].x, tri[2].x}),
                std::max({tri[0].y, tri[1].y, tri[2].y}),
                std::max({tri[0].z, tri[1].z, tri[2].z}) };
    return box;
}

// Return the indices of all triangles whose AABB overlaps (inclusive) with the query AABB.
std::vector<int> findOverlappingTriangles(const std::vector<std::array<Vector3, 3>>& triangles,
                                          const AABB& queryAabb) {
    // Precompute all triangle AABBs in O(n).
    std::vector<AABB> triangleBoxes;
    triangleBoxes.reserve(triangles.size());
    for (const auto& tri : triangles) {
        triangleBoxes.push_back(computeTriangleAabb(tri));
    }

    std::vector<int> result;
    for (std::size_t i = 0; i < triangles.size(); ++i) {
        const AABB& box = triangleBoxes[i];
        // Inclusive overlap test: no separation on any axis.
        bool overlaps = (box.min.x <= queryAabb.max.x && queryAabb.min.x <= box.max.x) &&
                        (box.min.y <= queryAabb.max.y && queryAabb.min.y <= box.max.y) &&
                        (box.min.z <= queryAabb.max.z && queryAabb.min.z <= box.max.z);
        if (overlaps) {
            result.push_back(static_cast<int>(i));
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <array>

// (Assume Vector3, AABB, and findOverlappingTriangles are declared as above.)

int main() {
    // Test 1: Simple case with two triangles, one inside query, one outside.
    std::vector<std::array<Vector3, 3>> triangles = {
        { Vector3{0,0,0}, Vector3{1,0,0}, Vector3{0,1,0} },   // triangle 0: XY plane
        { Vector3{5,5,5}, Vector3{6,5,5}, Vector3{5,6,5} }    // triangle 1: far away
    };
    AABB query = { {0,0,0}, {2,2,2} };
    std::vector<int> result = findOverlappingTriangles(triangles, query);
    assert(result.size() == 1);
    assert(result[0] == 0);

    // Test 2: Query box exactly matches a triangle's AABB (touching all faces).
    triangles = { { Vector3{0,0,0}, Vector3{2,0,0}, Vector3{0,2,0} } };
    query = { {0,0,0}, {2,2,0} };  // z min = max = 0, triangle lies in z=0 plane
    result = findOverlappingTriangles(triangles, query);
    assert(result.size() == 1);
    assert(result[0] == 0);

    // Test 3: Query box is empty (degenerate, min > max on an axis) – no triangle should overlap.
    query = { {1,1,1}, {0,1,1} };  // min.x > max.x
    result = findOverlappingTriangles(triangles, query);
    assert(result.empty());

    // Test 4: Triangle with negative coordinates and query box covering negative range.
    triangles = { { Vector3{-3,-3,-3}, Vector3{-1,-3,-3}, Vector3{-3,-1,-3} } };
    query = { {-4,-4,-4}, {-2,-2,-2} };
    result = findOverlappingTriangles(triangles, query);
    assert(result.size() == 1);
    assert(result[0] == 0);

    // Test 5: No triangles – empty input.
    triangles.clear();
    query = { {0,0,0}, {1,1,1} };
    result = findOverlappingTriangles(triangles, query);
    assert(result.empty());

    // Test 6: Multiple triangles where only some overlap.
    triangles = {
        { Vector3{0,0,0}, Vector3{1,0,0}, Vector3{0,1,0} },      // inside
        { Vector3{10,10,10}, Vector3{11,10,10}, Vector3{10,11,10} }, // far
        { Vector3{0.5,0.5,0.5}, Vector3{1.5,0.5,0.5}, Vector3{0.5,1.5,0.5} } // inside
    };
    query = { {0,0,0}, {2,2,2} };
    result = findOverlappingTriangles(triangles, query);
    assert(result.size() == 2);
    assert(result[0] == 0);
    assert(result[1] == 2);

    // Test 7: Triangle that just touches the query box at a corner (inclusive overlap).
    triangles = { { Vector3{2,2,2}, Vector3{3,2,2}, Vector3{2,3,2} } };
    query = { {0,0,0}, {2,2,2} };
    result = findOverlappingTriangles(triangles, query);
    assert(result.size() == 1);
    assert(result[0] == 0);

    return 0;
}
