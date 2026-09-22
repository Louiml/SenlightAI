/*
Given a set of four 3D points representing a tetrahedron, write a C++ function that recursively subdivides each triangular face into smaller triangles using the midpoint subdivision method (as in a Sierpinski tetrahedron) and returns the total number of vertices that would be generated if all faces were drawn at the given recursion depth. The function should take an integer `depth` (non-negative) and return an `int` representing the total count of distinct vertex positions required for rendering all triangles at that level. For `depth=0`, the tetrahedron is drawn as 4 triangles (the original faces) using only the 4 original vertices. For each subdivision step, every triangle is replaced by 4 smaller triangles (connecting midpoints of its edges), reusing shared midpoints across adjacent faces. The function must not actually render anything; it just computes the count.
*/

#include <cstdint>

// Return the total number of unique vertices needed to render a subdivided tetrahedron at given depth.
// depth >= 0. Uses 64-bit arithmetic internally to avoid overflow.
std::int64_t tetrahedronVertexCount(int depth) {
    if (depth < 0) depth = 0; // handle invalid input gracefully
    std::int64_t segments = std::int64_t(1) << depth; // 2^depth
    std::int64_t corners = 4;
    std::int64_t edgeMidpoints = 6 * (segments - 1);
    std::int64_t faceInteriors = 4 * ((segments - 1) * (segments - 2) / 2);
    return corners + edgeMidpoints + faceInteriors;
}

#include <cassert>
#include <cstdint>

int main() {
    // depth 0: just the original tetrahedron vertices
    assert(tetrahedronVertexCount(0) == 4);
    // depth 1: each face split into 4 triangles. New vertices: 6 edges * 1 midpoint each = 6, no interior. Total 4+6=10.
    assert(tetrahedronVertexCount(1) == 10);
    // depth 2: each edge has 3 interior points (2^2-1=3) *6 = 18; each face interior (3-1)*(3-2)/2 = 1, times 4 = 4. Total 4+18+4=26.
    assert(tetrahedronVertexCount(2) == 26);
    // depth 3: edges: 7 *6=42; face interiors: (7-1)*(7-2)/2=15, times 4=60. Total 4+42+60=106.
    assert(tetrahedronVertexCount(3) == 106);
    // depth 4: edges: 15*6=90; face interiors: (15-1)*(15-2)/2=91, *4=364. Total 4+90+364=458.
    assert(tetrahedronVertexCount(4) == 458);
    // depth 10: sanity check with formula, no overflow expected
    assert(tetrahedronVertexCount(10) == 4 + 6*(1023) + 4*(1023*1022/2)); // 4+6138+2093052=2099194
    // negative depth should be treated as 0
    assert(tetrahedronVertexCount(-5) == 4);
    return 0;
}

// The problem reduces to counting unique vertices in a subdivided tetrahedron. At depth `d`, each original triangular face is recursively subdivided `d` times. The subdivision of a triangle into 4 smaller triangles adds 3 new midpoints (one per edge), but these midpoints may be shared between adjacent triangles and between neighboring faces of the tetrahedron. Instead of simulating geometry, we can compute the total number of unique vertices analytically. The tetrahedron has 4 original corners. Each edge of the tetrahedron (there are 6 edges total) is subdivided into 2^d equal segments, so each edge contributes (2^d - 1) new distinct interior vertices. Additionally, each face (there are 4 faces) has interior points that are not on the tetrahedron edges; for a triangular face subdivided into a regular grid of side length 2^d, the number of strictly interior grid points (not on the boundary of the face) is (2^d - 1)(2^d - 2)/2. Since faces do not share interior points across faces, these are all distinct. So the total unique vertices = 4 (corners) + 6*(2^d - 1) (edge midpoints) + 4*(2^d - 1)*(2^d - 2)/2 (face interiors). For depth=0, this gives 4 + 0 + 0 = 4, which matches drawing the original tetrahedron. Edge cases: depth negative? treat as 0 or error; we'll assume non-negative per specification. Integer overflow: for depth up to maybe 10, 2^d ~ 1024, product ~ 1e6, safe in int; for larger depth use long long internally. Time complexity O(1) (just compute powers) and space O(1).
