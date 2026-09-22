Write a C++ function `buildAdjacencyList` that takes a 2D vector of faces, where each face is a vector of vertex indices (positive integers, not necessarily starting at 0, and not necessarily contiguous), and returns an adjacency list as a `std::vector<std::vector<int>>`. The output adjacency list must be of size `maxVertexIndex + 1`, where `maxVertexIndex` is the maximum vertex index appearing in any face. For each vertex `v`, its list must contain all unique neighboring vertices (vertices connected directly by an edge). The edges are defined by consecutive vertices in each face, with the last and first vertices also forming an edge (closure). The neighbor lists must be sorted in ascending order. The input may contain degenerate faces (faces with fewer than 3 vertices, including empty faces) that should be ignored. The function must be robust to faces of varying lengths (e.g., triangles, quads, etc.). You may not use any external libraries beyond the standard C++ library.

The solution iterates over every face, and for each face iterates over each vertex index `s` at position `j`, determining the next vertex `d` as the vertex at position `(j+1) % face.size()`. For each such directed edge, the function adds `d` to the adjacency list of `s`, and `s` to the adjacency list of `d`. After processing all faces, each adjacency list may contain duplicates, so the function sorts each list and removes consecutive duplicates using `std::unique` and `erase`. The output vector is sized to `maxVertexIndex + 1`, computed by scanning all faces to find the maximum vertex index. Empty faces or faces with fewer than 2 vertices are skipped because the modulo operation would produce meaningless edges (for a face of size 1, `j+1` modulo 1 is 0, creating a self-loop, but since unique removal handles it, but we avoid it entirely because we skip faces with size < 2; for size 0 we skip entirely). The time complexity is `O(F * k + V * k log k)` where `F` is the number of faces, `k` is the average face size, and `V` is the number of vertices (max index + 1), due to sorting each adjacency list. In practice, the total number of edges is `O(F * k)`, so if we treat `E` as total directed edge insertions (which is ~2 per undirected edge), the time is `O(E log V)` because each adjacency list is sorted individually; worst‑case when a single vertex has many neighbors, sorting that list dominates but still `O(E log V)` overall. Space is `O(E + V)`.

#include <vector>
#include <algorithm>
#include <cstddef>

// Build an adjacency list from a collection of faces.
// Each face is a vector of vertex indices (may vary in length, may be degenerate).
// The returned list has size maxVertexIndex + 1; each inner list contains unique
// neighboring vertices sorted in ascending order.
std::vector<std::vector<int>> buildAdjacencyList(
    const std::vector<std::vector<int>>& faces) {
    
    // Find the maximum vertex index.
    int maxVertex = -1;
    for (const auto& face : faces) {
        for (int v : face) {
            if (v > maxVertex) maxVertex = v;
        }
    }
    
    // If no vertices exist, return an empty list.
    if (maxVertex < 0) return {};
    
    // Initialize adjacency list with the required size.
    std::vector<std::vector<int>> adj(maxVertex + 1);
    
    // Loop over each face, but skip degenerate faces (size < 2).
    for (const auto& face : faces) {
        std::size_t n = face.size();
        if (n < 2) continue;
        
        for (std::size_t j = 0; j < n; ++j) {
            int s = face[j];
            int d = face[(j + 1) % n];
            adj[s].push_back(d);
            adj[d].push_back(s);
        }
    }
    
    // Sort and remove duplicates in each adjacency list.
    for (auto& neighbors : adj) {
        std::sort(neighbors.begin(), neighbors.end());
        neighbors.erase(std::unique(neighbors.begin(), neighbors.end()), neighbors.end());
    }
    
    return adj;
}

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Simple triangle: vertices 0,1,2
    std::vector<std::vector<int>> faces1 = {{0,1,2}};
    auto adj1 = buildAdjacencyList(faces1);
    assert(adj1.size() == 3);
    assert(adj1[0] == std::vector<int>({1,2}));
    assert(adj1[1] == std::vector<int>({0,2}));
    assert(adj1[2] == std::vector<int>({0,1}));

    // Quad: vertices 0,1,2,3
    std::vector<std::vector<int>> faces2 = {{0,1,2,3}};
    auto adj2 = buildAdjacencyList(faces2);
    assert(adj2.size() == 4);
    assert(adj2[0] == std::vector<int>({1,3}));
    assert(adj2[1] == std::vector<int>({0,2}));
    assert(adj2[2] == std::vector<int>({1,3}));
    assert(adj2[3] == std::vector<int>({0,2}));

    // Two triangles sharing an edge: (0,1,2) and (1,3,2)
    std::vector<std::vector<int>> faces3 = {{0,1,2}, {1,3,2}};
    auto adj3 = buildAdjacencyList(faces3);
    assert(adj3.size() == 4);
    assert(adj3[0] == std::vector<int>({1,2}));
    assert(adj3[1] == std::vector<int>({0,2,3}));
    assert(adj3[2] == std::vector<int>({0,1,3}));
    assert(adj3[3] == std::vector<int>({1,2}));

    // Degenerate faces (empty, single vertex) are ignored
    std::vector<std::vector<int>> faces4 = {{}, {5}, {0,5}};
    auto adj4 = buildAdjacencyList(faces4);
    assert(adj4.size() == 6);
    assert(adj4[0] == std::vector<int>({5}));
    assert(adj4[5] == std::vector<int>({0}));
    for (int i = 1; i < 5; ++i) assert(adj4[i].empty());

    // Non-zero starting index, non-contiguous
    std::vector<std::vector<int>> faces5 = {{10,20,30}};
    auto adj5 = buildAdjacencyList(faces5);
    assert(adj5.size() == 31);
    assert(adj5[10] == std::vector<int>({20,30}));
    assert(adj5[20] == std::vector<int>({10,30}));
    assert(adj5[30] == std::vector<int>({10,20}));
    // All other vertices have empty lists
    for (int i = 0; i < 31; ++i) {
        if (i != 10 && i != 20 && i != 30) assert(adj5[i].empty());
    }

    // Fully closed polygon (triangle) yields each vertex connected to all others
    // Already covered; additional check for duplicate edges in same face
    std::vector<std::vector<int>> faces6 = {{0,0,1}}; // degenerate face with repeated vertex
    auto adj6 = buildAdjacencyList(faces6);
    assert(adj6.size() == 2);
    assert(adj6[0] == std::vector<int>({0,1})); // self-loop from 0-0 edge, but unique
    assert(adj6[1] == std::vector<int>({0}));

    // No faces at all
    std::vector<std::vector<int>> faces7 = {};
    auto adj7 = buildAdjacencyList(faces7);
    assert(adj7.empty());

    // Faces with varying lengths: triangle + quad
    std::vector<std::vector<int>> faces8 = {{0,1,2}, {1,3,4,2}};
    auto adj8 = buildAdjacencyList(faces8);
    assert(adj8.size() == 5);
    assert(adj8[0] == std::vector<int>({1,2}));
    assert(adj8[1] == std::vector<int>({0,2,3,4}));
    assert(adj8[2] == std::vector<int>({0,1,3,4}));
    assert(adj8[3] == std::vector<int>({1,2,4}));
    assert(adj8[4] == std::vector<int>({1,2,3}));

    return 0;
}
