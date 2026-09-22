// Write a standalone C++ function that computes the degree centrality of every vertex in an undirected graph with a fixed number of vertices numbered from 0 to n-1. The function should take as input a vector of pairs representing undirected edges (each pair contains two vertex indices) and the number of vertices n. It should return a vector of unsigned integers where the i-th element is the degree centrality of vertex i (i.e., the number of edges incident to that vertex). The graph is undirected, so each edge contributes 1 to both endpoints. The input may contain duplicate edges, self-loops (where both vertices in the pair are equal), and the vertices may appear in any order within each pair. Edge cases include n=0, empty edge list, and vertices with degree 0. The function must handle all such cases correctly and efficiently.

The solution is straightforward: initialize a vector of size n with all zeros. Iterate over every edge pair, and for each pair (u,v), increment the centrality of u once and the centrality of v once. If it is a self-loop (u == v), the edge still contributes 1 to that vertex (not 2), because each edge incident to a vertex is counted exactly once even if both endpoints are the same. Duplicate edges simply increment counts multiple times, which is correct because the definition of degree in a multigraph counts each parallel edge separately. Time complexity is O(E) where E is the number of edge pairs, and space complexity is O(n) for the output vector plus O(E) if the input vector is copied, but the function can take a const reference to avoid copying the edges. No special handling is needed for isolated vertices—they naturally remain at 0.

#include <vector>
#include <utility>

// Computes degree centrality for each vertex in an undirected multigraph.
// vertices: number of vertices (0..vertices-1).
// edges: vector of pairs {u,v} representing undirected edges.
// Returns a vector of size vertices where result[i] = degree of vertex i.
std::vector<unsigned> degree_centrality(
    unsigned vertices,
    const std::vector<std::pair<unsigned, unsigned>>& edges)
{
    // Initialize all degrees to zero.
    std::vector<unsigned> degrees(vertices, 0);
    
    // Process each edge.
    for (const auto& edge : edges) {
        // Undirected edge: increment both endpoints.
        // For a self-loop (u==v), we increment once (not twice),
        // because the edge is incident to that vertex only once.
        degrees[edge.first] += 1;
        degrees[edge.second] += 1;
    }
    
    return degrees;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be declared above.

int main() {
    // Test 1: Simple triangle graph (0-1, 1-2, 0-2)
    std::vector<std::pair<unsigned, unsigned>> edges1 = {{0,1},{1,2},{0,2}};
    auto d1 = degree_centrality(3, edges1);
    assert(d1.size() == 3);
    assert(d1[0] == 2 && d1[1] == 2 && d1[2] == 2);

    // Test 2: Empty graph with n=5
    std::vector<std::pair<unsigned, unsigned>> edges2;
    auto d2 = degree_centrality(5, edges2);
    assert(d2.size() == 5);
    for (unsigned val : d2) assert(val == 0);

    // Test 3: Single edge between 0 and 1, isolated vertex 2
    std::vector<std::pair<unsigned, unsigned>> edges3 = {{0,1}};
    auto d3 = degree_centrality(3, edges3);
    assert(d3[0] == 1 && d3[1] == 1 && d3[2] == 0);

    // Test 4: Duplicate edges (multi-edge) and vertices order reversed
    std::vector<std::pair<unsigned, unsigned>> edges4 = {{1,0},{0,1},{1,0}};
    auto d4 = degree_centrality(2, edges4);
    assert(d4[0] == 3 && d4[1] == 3);

    // Test 5: Self-loop at vertex 0 plus an edge 0-1
    std::vector<std::pair<unsigned, unsigned>> edges5 = {{0,0},{0,1}};
    auto d5 = degree_centrality(2, edges5);
    assert(d5[0] == 2 && d5[1] == 1); // self-loop contributes 1 to vertex 0

    // Test 6: n=0 (no vertices)
    std::vector<std::pair<unsigned, unsigned>> edges6 = {{0,0}}; // invalid but function should not crash
    auto d6 = degree_centrality(0, edges6);
    assert(d6.empty());

    // Test 7: All vertices connected to one hub (star graph)
    std::vector<std::pair<unsigned, unsigned>> edges7 = {{0,1},{0,2},{0,3}};
    auto d7 = degree_centrality(4, edges7);
    assert(d7[0] == 3);
    assert(d7[1] == 1 && d7[2] == 1 && d7[3] == 1);

    return 0;
}
