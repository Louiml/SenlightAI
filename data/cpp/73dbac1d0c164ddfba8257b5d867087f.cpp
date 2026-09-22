// Write a C++ function `std::vector<uint32_t> weldVertices(const std::vector<std::array<float,3>>& inTriangles, float inVertexWeldDistance)` that takes a list of triangles (each triangle is an array of three vertices, each vertex is `(x,y,z)` as floats) and a weld distance threshold. The function must return, for each vertex in the input (where the i-th vertex is the i-th coordinate in the flat sequence of triangles in order), the index of the "representative" vertex it should be merged with. Two vertices should be merged if the squared Euclidean distance between them is less than or equal to `inVertexWeldDistance * inVertexWeldDistance`. The representative of a group must be the smallest index among all vertices in that group (i.e., vertices that are directly or transitively within the weld distance form a connected component, and the representative is the minimum index in that component). The output vector must have exactly the same number of elements as the total number of vertices (i.e., `inTriangles.size() * 3`), and for each index `i`, `output[i]` must be the minimum index in the connected component containing `i`. The function should handle empty input by returning an empty vector.
The problem requires grouping vertices into connected components where an edge exists between two vertices if their squared distance ≤ weld_dist². The representative for each component is its smallest index. This is a classic union-find (disjoint set) problem. We can iterate over all pairs of vertices (O(N²) where N = number of vertices) and union pairs that are within the distance threshold. To avoid excessive memory and time for large inputs, we can use a simple union-find with path compression and union by rank. However, the straightforward brute-force O(N²) comparison is acceptable for moderate N. Edge cases: if `inVertexWeldDistance` is negative, treat it as zero (or return identity mapping). For an empty input, return empty vector. For vertices that are not weldable to any other, their representative is themselves. The iterative union-find must handle transitive closure: if A is close to B and B is close to C, then A, B, C all share the same representative (minimum index among them). We need to union all found pairs and then compress paths to find the representative. After all unions, we perform path compression on every index to ensure `output[i]` is the root (minimum index). The time complexity is O(N²) for the pairwise checks plus near O(N α(N)) for union-find, total O(N²) in the worst case. Space complexity is O(N) for the parent array and the output.
#include <vector>
#include <array>
#include <cstdint>
#include <algorithm>
#include <cmath>

// Union-find with path compression and union by rank.
class UnionFind {
public:
    UnionFind(uint32_t n) : parent(n), rank(n, 0) {
        for (uint32_t i = 0; i < n; ++i) parent[i] = i;
    }

    uint32_t find(uint32_t x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(uint32_t a, uint32_t b) {
        uint32_t ra = find(a);
        uint32_t rb = find(b);
        if (ra == rb) return;
        // Union by rank (though we ultimately want the minimum index, not the rank)
        if (rank[ra] < rank[rb]) std::swap(ra, rb);
        parent[rb] = ra;
        if (rank[ra] == rank[rb]) ++rank[ra];
    }

private:
    std::vector<uint32_t> parent;
    std::vector<uint32_t> rank;
};

// Weld vertices based on a distance threshold.
// Each triangle is an array of three (x, y, z) floats.
// Returns a vector mapping each vertex index (in order of appearance) to the representative index.
std::vector<uint32_t> weldVertices(const std::vector<std::array<float,3>>& inTriangles, float inVertexWeldDistance) {
    const uint32_t numTriangles = static_cast<uint32_t>(inTriangles.size());
    const uint32_t numVertices = numTriangles * 3;

    if (numVertices == 0) return {};

    // Handle negative distance by treating as zero (only exact duplicates weld).
    const float weldDist = (inVertexWeldDistance < 0.0f) ? 0.0f : inVertexWeldDistance;
    const float weldDistSq = weldDist * weldDist;

    UnionFind uf(numVertices);

    // Compare every pair of vertices.
    // The vertex i is in triangle i/3, component (i%3).
    for (uint32_t i = 0; i < numVertices; ++i) {
        const auto& v1 = inTriangles[i / 3][i % 3];
        for (uint32_t j = i + 1; j < numVertices; ++j) {
            const auto& v2 = inTriangles[j / 3][j % 3];
            float dx = v1[0] - v2[0];
            float dy = v1[1] - v2[1];
            float dz = v1[2] - v2[2];
            if (dx*dx + dy*dy + dz*dz <= weldDistSq) {
                uf.unite(i, j);
            }
        }
    }

    // After all unions, we need the representative to be the minimum index in each component.
    // Since we used union by rank, the root may not be the minimum. We'll do a second pass to
    // find the minimum index in each component and then map everything to that.
    // First, find the minimum index for each root.
    std::vector<uint32_t> minIndex(numVertices, UINT32_MAX);
    for (uint32_t i = 0; i < numVertices; ++i) {
        uint32_t root = uf.find(i);
        if (i < minIndex[root]) minIndex[root] = i;
    }

    // Then assign the minimum as the representative for every vertex.
    std::vector<uint32_t> result(numVertices);
    for (uint32_t i = 0; i < numVertices; ++i) {
        result[i] = minIndex[uf.find(i)];
    }
    return result;
}
#include <cassert>
#include <vector>
#include <array>
#include <cstdint>

// Include the solution function here or link it.

int main() {
    // Test 1: Empty input
    {
        std::vector<std::array<float,3>> triangles;
        auto result = weldVertices(triangles, 0.1f);
        assert(result.empty());
    }

    // Test 2: Single triangle, no welding (all vertices far apart)
    {
        std::vector<std::array<float,3>> triangles = {{{0,0,0}, {10,0,0}, {0,10,0}}};
        auto result = weldVertices(triangles, 1.0f);
        assert(result.size() == 3);
        assert(result[0] == 0);
        assert(result[1] == 1);
        assert(result[2] == 2);
    }

    // Test 3: Two triangles sharing a vertex exactly (distance 0)
    {
        // Triangle 0: (0,0,0), (1,0,0), (0,1,0)
        // Triangle 1: (0,0,0), (2,0,0), (0,2,0)
        // Shared vertex index 0 (tri0) and index 3 (tri1) are identical.
        std::vector<std::array<float,3>> triangles = {
            {{0,0,0}, {1,0,0}, {0,1,0}},
            {{0,0,0}, {2,0,0}, {0,2,0}}
        };
        auto result = weldVertices(triangles, 0.0f);
        assert(result.size() == 6);
        // Vertex 0 and 3 are welded, representative is 0.
        assert(result[0] == 0);
        assert(result[3] == 0);
        // All others remain themselves.
        assert(result[1] == 1);
        assert(result[2] == 2);
        assert(result[4] == 4);
        assert(result[5] == 5);
    }

    // Test 4: Transitive closure: A close to B, B close to C, but A not close to C.
    // Use vertices along a line: 0 at 0, 1 at 2, 2 at 4 with weld distance 3 (squared 9).
    // Distances: 0-1=4 (≤9), 1-2=4 (≤9), 0-2=16 (>9). All should weld to 0.
    {
        std::vector<std::array<float,3>> triangles = {{{0,0,0}, {2,0,0}, {4,0,0}}};
        auto result = weldVertices(triangles, 3.0f);
        assert(result.size() == 3);
        assert(result[0] == 0);
        assert(result[1] == 0);
        assert(result[2] == 0);
    }

    // Test 5: Multiple triangles, connected component with minimum index not the first one encountered.
    {
        // Vertices: 0 at (100,0,0), 1 at (101,0,0), 2 at (102,0,0), all within distance 2.
        // Also vertex 3 at (200,0,0) far.
        std::vector<std::array<float,3>> triangles = {
            {{100,0,0}, {101,0,0}, {102,0,0}},
            {{200,0,0}, {300,0,0}, {400,0,0}}
        };
        auto result = weldVertices(triangles, 2.0f);
        // Indices: 0,1,2 weld together with representative 0.
        assert(result[0] == 0);
        assert(result[1] == 0);
        assert(result[2] == 0);
        // Others remain.
        assert(result[3] == 3);
        assert(result[4] == 4);
        assert(result[5] == 5);
    }

    // Test 6: Negative weld distance treated as zero (only exact duplicates).
    {
        std::vector<std::array<float,3>> triangles = {{{1,2,3}, {1,2,3}, {0,0,0}}};
        auto result = weldVertices(triangles, -1.0f);
        assert(result[0] == 0); // exact duplicate of 0 and 1, min is 0
        assert(result[1] == 0);
        assert(result[2] == 2);
    }

    // Test 7: All vertices identical, all weld to 0.
    {
        std::vector<std::array<float,3>> triangles = {
            {{5,5,5}, {5,5,5}, {5,5,5}},
            {{5,5,5}, {5,5,5}, {5,5,5}}
        };
        auto result = weldVertices(triangles, 0.5f);
        assert(result.size() == 6);
        for (size_t i = 0; i < result.size(); ++i) {
            assert(result[i] == 0);
        }
    }

    return 0;
}
