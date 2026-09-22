/*
Given an undirected graph with `n` vertices (labeled from 1 to `n`) and `m` edges, write a C++ function `int countConnectedComponents(int n, const vector<pair<int,int>>& edges)` that returns the number of connected components in the graph. The graph may be disconnected, may contain isolated vertices, parallel edges, and self-loops. Vertices with no incident edges count as their own connected component. The function should use an iterative or recursive depth-first search (DFS) or breadth-first search (BFS) approach. Do not include a `main` function in your solution.
*/
#include <vector>
#include <functional>

// Count the number of connected components in an undirected graph.
// n: number of vertices (labels 1..n), edges: list of undirected edges.
int countConnectedComponents(int n, const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency list (1-indexed).
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& [a, b] : edges) {
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    std::vector<bool> visited(n + 1, false);

    // Recursive DFS lambda.
    std::function<void(int)> dfs = [&](int node) {
        visited[node] = true;
        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                dfs(neighbor);
            }
        }
    };

    int components = 0;
    for (int v = 1; v <= n; ++v) {
        if (!visited[v]) {
            ++components;
            dfs(v);
        }
    }
    return components;
}
#include <cassert>
#include <vector>

// Declaration of the solution function (would be included from above).
int countConnectedComponents(int n, const std::vector<std::pair<int,int>>& edges);

int main() {
    // Empty graph: all isolated vertices.
    assert(countConnectedComponents(5, {}) == 5);

    // Single edge connecting two vertices: 4 components total.
    assert(countConnectedComponents(4, {{1,2}}) == 3);

    // Connected graph: 1 component.
    assert(countConnectedComponents(3, {{1,2},{2,3},{1,3}}) == 1);

    // Two separate components.
    assert(countConnectedComponents(6, {{1,2},{3,4},{5,6}}) == 3);

    // Self-loop does not change component count.
    assert(countConnectedComponents(2, {{1,1},{2,2}}) == 2);

    // Parallel edges and self-loop in one component.
    assert(countConnectedComponents(3, {{1,2},{2,1},{2,2},{1,3}}) == 1);

    // Graph with one edge and isolated vertices.
    assert(countConnectedComponents(5, {{2,5}}) == 4);

    // Larger connected chain.
    assert(countConnectedComponents(5, {{1,2},{2,3},{3,4},{4,5}}) == 1);

    // Star graph with center 1 and leaves 2,3,4, plus isolated 5.
    assert(countConnectedComponents(5, {{1,2},{1,3},{1,4}}) == 2);
}
// The problem reduces to counting connected components in an undirected graph. The standard approach is to perform a graph traversal (DFS or BFS) starting from every unvisited vertex; each new traversal corresponds to exactly one connected component. Build an adjacency list from the edge list. The graph is undirected, so for each edge `(a,b)`, add `b` to `a`'s adjacency list and `a` to `b`'s list. Self-loops (where `a == b`) are handled naturally by adding the vertex to its own list; they do not affect component counting beyond normal traversal. Parallel edges are harmless because visited flags prevent redundant work. Isolated vertices (appearing in no edge) will never be visited during a traversal from another vertex, so they will each trigger a new DFS, correctly counting as separate components. The algorithm runs in `O(n + m)` time, because each vertex and edge is processed once. Space complexity is `O(n + m)` for the adjacency list plus `O(n)` for the visited array (or recursion stack depth in worst case for DFS).
