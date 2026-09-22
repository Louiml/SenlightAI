Write a C++ function that, given a triangle mesh represented by arrays of vertex positions and triangle indices, determines whether each triangle edge is a boundary edge (belonging to only one triangle) or a shared edge, and returns a vector of 8-bit flags per triangle where bits 0-2 indicate whether edges 0, 1, and 2 are boundary edges (1 if boundary, 0 if shared), and bits 3-5 indicate whether the corresponding opposite vertices (vertex opposite edge 0 is vertex 2, opposite edge 1 is vertex 0, opposite edge 2 is vertex 1) are incident to any boundary edge (1 if yes, 0 if no). The function signature is `std::vector<uint8_t> computeEdgeBoundaryFlags(const std::vector<std::array<double,3>>& vertices, const std::vector<std::array<int,3>>& triangles)`. The mesh is assumed to be a closed manifold (each edge shared by at most two triangles) but may have boundary edges; triangles are consistently oriented (all CCW or all CW). The function must be self-contained, use only standard library, and run in O(V + T log T) time where V is vertex count and T is triangle count.
The core challenge is to efficiently classify edges as boundary or shared, and then determine which vertices are incident to boundary edges. The approach is as follows: First, generate an edge record for each of the three edges of each triangle, storing the sorted pair of vertex indices (to canonicalize edges regardless of triangle orientation), the triangle index, and a flag indicating which edge in the triangle (edge 0, 1, or 2). Sort all edge records by the sorted vertex pair. After sorting, edges that appear twice (two adjacent triangles) can be identified by consecutive records with the same vertex pair. A single occurrence indicates a boundary edge. For each edge record, set a bit in the triangle's flag for that edge if the edge is a boundary edge, and also mark the two vertices of that boundary edge as "boundary-incident" in a per-vertex boolean array. Finally, for each triangle, after processing all its three edges, set bits 3-5 based on whether the three vertices (opposite vertices: vertex 2 for edge 0, vertex 0 for edge 1, vertex 1 for edge 2) are marked as boundary-incident. Important edge cases: The same vertex pair might appear more than twice if the mesh is non-manifold; the problem guarantees manifold, but the algorithm gracefully handles duplicates by only marking as boundary when exactly one occurrence is found (since consecutive sorted records with same pair: if exactly one, boundary; if two, shared; if more than two, treat as shared to avoid false positives). Also, edges may have vertex indices swapped in the triangle; normalization (sorting indices) is essential. Time complexity: generating records O(T), sorting O(T log T), scanning sorted list O(T), marking vertices O(T), final triangle pass O(T), so total O(T log T) dominated by sort. Space complexity O(T) for records and O(V) for vertex flags.
#include <vector>
#include <array>
#include <cstdint>
#include <algorithm>

// Compute per-triangle edge boundary and boundary-vertex flags.
// Returns a vector of 8-bit flags per triangle:
// bits 0-2: edge 0,1,2 is boundary (1) or shared (0)
// bits 3-5: vertex opposite edge 0 (v2), edge 1 (v0), edge 2 (v1) is incident to any boundary edge (1) or not (0)
std::vector<uint8_t> computeEdgeBoundaryFlags(
    const std::vector<std::array<double, 3>>& vertices,
    const std::vector<std::array<int, 3>>& triangles)
{
    const size_t numTri = triangles.size();
    if (numTri == 0) return {};

    // Record for each directed triangle edge, later sorted by canonical (v0,v1) pair.
    struct EdgeRecord {
        int v0; // lower vertex index
        int v1; // higher vertex index
        int triIdx;
        uint8_t edgeBit; // 0x01 for edge 0, 0x02 for edge 1, 0x04 for edge 2
    };
    std::vector<EdgeRecord> records;
    records.reserve(numTri * 3);
    for (size_t t = 0; t < numTri; ++t) {
        int idx[3] = {triangles[t][0], triangles[t][1], triangles[t][2]};
        // For each of the three edges: (idx0,idx1), (idx1,idx2), (idx2,idx0)
        for (int e = 0; e < 3; ++e) {
            int a = idx[e];
            int b = idx[(e + 1) % 3];
            int vlo = std::min(a, b);
            int vhi = std::max(a, b);
            uint8_t bit = (uint8_t)(1 << e);
            records.push_back({vlo, vhi, static_cast<int>(t), bit});
        }
    }

    std::sort(records.begin(), records.end(),
              [](const EdgeRecord& r1, const EdgeRecord& r2) {
                  if (r1.v0 != r2.v0) return r1.v0 < r2.v0;
                  return r1.v1 < r2.v1;
              });

    std::vector<uint8_t> edgeFlags(numTri, 0);       // bits 0-2 for boundary edges
    std::vector<bool> vertexBoundary(vertices.size(), false); // marks vertices incident to boundary edges

    size_t i = 0;
    while (i < records.size()) {
        size_t j = i;
        // Advance to the next record with different (v0,v1)
        while (j < records.size() &&
               records[j].v0 == records[i].v0 &&
               records[j].v1 == records[i].v1) {
            ++j;
        }
        size_t count = j - i;
        if (count == 1) {
            // Boundary edge
            edgeFlags[records[i].triIdx] |= records[i].edgeBit;
            vertexBoundary[records[i].v0] = true;
            vertexBoundary[records[i].v1] = true;
        }
        // if count >= 2, internal edge; count >2 assumed non-manifold but treated as shared
        i = j;
    }

    // For each triangle, compute opposite vertex boundary flags
    std::vector<uint8_t> result(numTri);
    for (size_t t = 0; t < numTri; ++t) {
        int idx[3] = {triangles[t][0], triangles[t][1], triangles[t][2]};
        uint8_t flags = edgeFlags[t];
        if (vertexBoundary[idx[2]]) flags |= 0x08; // opposite edge 0 is vertex 2
        if (vertexBoundary[idx[0]]) flags |= 0x10; // opposite edge 1 is vertex 0
        if (vertexBoundary[idx[1]]) flags |= 0x20; // opposite edge 2 is vertex 1
        result[t] = flags;
    }

    return result;
}
#include <cassert>
#include <vector>
#include <array>
#include <cstdint>

int main() {
    // Single triangle: all three edges are boundary, all three vertices are boundary-incident.
    std::vector<std::array<double,3>> verts1 = {{{0,0,0},{1,0,0},{0,1,0}}};
    std::vector<std::array<int,3>> tris1 = {{{0,1,2}}};
    auto flags1 = computeEdgeBoundaryFlags(verts1, tris1);
    assert(flags1.size() == 1);
    // edges 0,1,2 boundary and all opposite vertices boundary-incident
    assert(flags1[0] == 0x3F); // bits 0-5 all set

    // Two triangles forming a square with diagonal: edges (0,1),(1,2),(2,0) are boundary
    // and (0,2) is shared? Wait: triangles (0,1,2) and (2,3,0) with vertices square corners.
    std::vector<std::array<double,3>> verts2 = {{{0,0,0},{1,0,0},{1,1,0},{0,1,0}}};
    std::vector<std::array<int,3>> tris2 = {{{0,1,2},{2,3,0}}};
    auto flags2 = computeEdgeBoundaryFlags(verts2, tris2);
    assert(flags2.size() == 2);
    // Triangle 0 edges: (0,1) boundary, (1,2) boundary, (2,0) shared. Opposite vertices: v2 (2) boundary-incident? v2 is vertex 2, yes boundary edges (1,2) incident; v0 (0) boundary incident; v1 (1) boundary incident.
    // Triangle 1 edges: (2,3) boundary? (2,3) is boundary, (3,0) boundary, (0,2) shared. Opposite vertices: v0 (2) boundary, v1 (0) boundary, v2 (3) boundary?
    // Let's compute manually:
    // Shared edge is (0,2) because both triangles have vertices 0 and 2 (order swapped).
    // Triangle 0 edges: (0,1) boundary, (1,2) boundary, (2,0) shared (so edge bit 0x04 not set).
    // Triangle 1 edges: (2,3) boundary, (3,0) boundary, (0,2) shared (edge bit 0x04 not set).
    // All vertices 0,1,2,3 are boundary-incident because each is on some boundary edge.
    assert((flags2[0] & 0x07) == 0x03); // edges 0 and 1 boundary, edge 2 not
    assert((flags2[0] & 0x38) == 0x38); // all three opposite vertices boundary-incident
    assert((flags2[1] & 0x07) == 0x03); // edges 0 and 1 boundary, edge 2 not
    assert((flags2[1] & 0x38) == 0x38);

    // Four triangles forming a tetrahedron (closed) - no boundary edges, no boundary vertices.
    std::vector<std::array<double,3>> verts3 = {{{0,0,0},{1,0,0},{0,1,0},{0,0,1}}};
    std::vector<std::array<int,3>> tris3 = {{{0,2,1},{0,1,3},{0,3,2},{1,2,3}}};
    auto flags3 = computeEdgeBoundaryFlags(verts3, tris3);
    assert(flags3.size() == 4);
    for (uint8_t f : flags3) assert(f == 0); // all zero

    // Two disconnected triangles: all edges boundary.
    std::vector<std::array<double,3>> verts4 = {{{0,0,0},{1,0,0},{0,1,0},{0,0,1},{1,0,1},{0,1,1}}};
    std::vector<std::array<int,3>> tris4 = {{{0,1,2},{3,4,5}}};
    auto flags4 = computeEdgeBoundaryFlags(verts4, tris4);
    assert(flags4.size() == 2);
    assert(flags4[0] == 0x3F);
    assert(flags4[1] == 0x3F);

    // Mesh with an internal edge shared by two triangles and one boundary vertex not on any boundary edge?
    // Build a strip: triangles (0,1,2) and (0,2,3) share edge (0,2). Vertex 0 and 2 are shared, vertex 1 and 3 are only in each triangle, but edges (0,1),(1,2),(2,3),(3,0) are boundary? Actually vertices 1 and 3 might be boundary.
    std::vector<std::array<double,3>> verts5 = {{{0,0,0},{1,0,0},{1,1,0},{0,1,0}}};
    std::vector<std::array<int,3>> tris5 = {{{0,1,2},{0,2,3}}};
    auto flags5 = computeEdgeBoundaryFlags(verts5, tris5);
    assert(flags5.size() == 2);
    // Shared edge (0,2) so both triangles have edge 2? Let's see triangle0 edges: (0,1),(1,2),(2,0) -> edge 2 is (2,0) shared. triangle1 edges: (0,2),(2,3),(3,0) -> edge 0 is (0,2) shared.
    // Triangle0 boundary edges: (0,1) and (1,2). Triangle1 boundary edges: (2,3) and (3,0).
    assert((flags5[0] & 0x07) == 0x03); // edges 0 and 1
    assert((flags5[1] & 0x07) == 0x06); // edges 1 and 2
    // Which vertices are boundary? 0,1,2,3 all boundary? Vertex 0 is on boundary edges (0,1) and (3,0); vertex1 on (0,1),(1,2); vertex2 on (1,2),(2,3); vertex3 on (2,3),(3,0). So all boundary.
    assert((flags5[0] & 0x38) == 0x38);
    assert((flags5[1] & 0x38) == 0x38);

    // Empty mesh
    auto empty = computeEdgeBoundaryFlags({}, {});
    assert(empty.empty());

    return 0;
}
