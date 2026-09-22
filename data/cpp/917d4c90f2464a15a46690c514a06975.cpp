// Write a C++ function that takes an array of undirected edges (each edge as a pair of integer vertices) and an integer specifying the number of vertices in the graph, then returns the adjacency list of the graph as a `std::vector<std::vector<int>>`. The graph vertices are numbered from 0 to `numVertices-1`. The function should build both directions for each undirected edge (i.e., if an edge (u,v) exists, both u and v appear in each other's adjacency lists), but the order of neighbors within each list does not matter. Edge cases include duplicate edges, self-loops (edge from a vertex to itself), and vertices that have no edges (they should appear as empty lists). The input may contain edges with vertex indices that are valid (less than `numVertices`). Return the adjacency list as a 2D vector.

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Empty edge list
    auto g1 = buildAdjacencyList({}, 3);
    assert(g1.size() == 3);
    assert(g1[0].empty() && g1[1].empty() && g1[2].empty());

    // Simple chain: 0-1, 1-2
    auto g2 = buildAdjacencyList({{0,1},{1,2}}, 3);
    assert(g2.size() == 3);
    assert(g2[0] == std::vector<int>({1}));
    assert(g2[1].size() == 2);
    assert(std::find(g2[1].begin(), g2[1].end(), 0) != g2[1].end());
    assert(std::find(g2[1].begin(), g2[1].end(), 2) != g2[1].end());
    assert(g2[2] == std::vector<int>({1}));

    // Duplicate edges and self-loop
    auto g3 = buildAdjacencyList({{0,0},{0,1},{0,1}}, 2);
    assert(g3.size() == 2);
    // vertex 0: self-loop adds 0, edge to 1 adds 1 twice
    assert(g3[0] == std::vector<int>({0,1,1}));
    // vertex 1: edge to 0 twice
    assert(g3[1] == std::vector<int>({0,0}));

    // Larger graph with isolated vertex
    auto g4 = buildAdjacencyList({{4,5},{5,4},{0,2}}, 6);
    assert(g4.size() == 6);
    assert(g4[0] == std::vector<int>({2}));
    assert(g4[2] == std::vector<int>({0}));
    assert(g4[4].size() == 1 && g4[4][0] == 5);
    assert(g4[5].size() == 1 && g4[5][0] == 4);
    assert(g4[1].empty() && g4[3].empty());

    // Single vertex with no edges
    auto g5 = buildAdjacencyList({}, 1);
    assert(g5.size() == 1);
    assert(g5[0].empty());
}

#include <vector>

// Build an undirected graph's adjacency list from a list of edges.
// edges: vector of pairs, each pair represents an edge (u,v).
// numVertices: total number of vertices (0 to numVertices-1).
// Returns a 2D vector where graph[u] contains all neighbors of vertex u.
std::vector<std::vector<int>> buildAdjacencyList(
    const std::vector<std::pair<int, int>>& edges,
    int numVertices
) {
    std::vector<std::vector<int>> graph(numVertices);
    for (const auto& edge : edges) {
        int src = edge.first;
        int dest = edge.second;
        graph[src].push_back(dest);  // edge from src to dest
        graph[dest].push_back(src);  // edge from dest to src (undirected)
    }
    return graph;
}

// The solution initializes a vector of `numVertices` empty integer vectors. For each edge in the input array, extract `src` and `dest`, then push `dest` into `graph[src]` and push `src` into `graph[dest]`. This handles undirected edges correctly. Duplicate edges simply add duplicate entries in the adjacency lists, which is acceptable. Self-loops (where `src == dest`) will push the same vertex twice into that vertex's list (once from the first push and once from the second), which is fine for an adjacency list representation. Vertices with no edges remain empty vectors as initialized. Edge cases include an empty edge array, which returns all empty lists, and a graph with single vertex and no edges. Time complexity is O(E) where E is the number of edges, since each edge does constant work (two push_back operations). Space complexity is O(V + E) for the adjacency lists, where V is the number of vertices.
