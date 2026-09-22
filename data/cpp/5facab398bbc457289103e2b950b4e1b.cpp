Write a C++ function `int countConnectedComponents(int n, const std::vector<std::pair<int,int>>& edges)` that takes the number of vertices `n` (vertices are labeled `0` to `n-1`) and a list of undirected edges, and returns the number of connected components in the undirected graph. The graph is guaranteed to have no self-loops or duplicate edges, but may be disconnected or have isolated vertices. Use an adjacency list representation internally (similar to the provided `UGraph` structure) and a depth-first search (DFS) or union-find algorithm. The function must be self-contained (no external graph library), handle `n=0` gracefully (return 0), and be `const`-correct where applicable.

#include <cassert>
#include <vector>
#include <utility>

int countConnectedComponents(int, const std::vector<std::pair<int,int>>&); // declaration

int main() {
    // Empty graph
    assert(countConnectedComponents(0, {}) == 0);

    // Isolated vertices
    assert(countConnectedComponents(5, {}) == 5);

    // Single edge connecting two vertices
    assert(countConnectedComponents(4, {{0,1}}) == 3);

    // Fully connected triangle plus isolated vertex
    assert(countConnectedComponents(4, {{0,1},{1,2},{2,0}}) == 2);

    // Two separate edges (two components plus isolated)
    assert(countConnectedComponents(6, {{0,1},{2,3}}) == 4);

    // Larger connected component
    assert(countConnectedComponents(7, {{0,1},{1,2},{2,3},{3,4},{4,5},{5,6}}) == 1);

    // Duplicate edges (should be handled gracefully, but spec says no duplicates)
    assert(countConnectedComponents(3, {{0,1},{0,1}}) == 2);

    // Invalid edge indices (should be ignored)
    assert(countConnectedComponents(3, {{0,5},{1,2}}) == 2);

    // Self-loop (should be ignored)
    assert(countConnectedComponents(3, {{1,1},{0,2}}) == 2);

    // Star graph
    assert(countConnectedComponents(5, {{0,1},{0,2},{0,3},{0,4}}) == 1);

    // All vertices in pairs
    assert(countConnectedComponents(6, {{0,1},{2,3},{4,5}}) == 3);
}

#include <vector>
#include <stack>

// Count connected components in an undirected graph with n vertices (0..n-1).
// Edges are given as pairs of endpoints. Returns 0 for n=0.
int countConnectedComponents(int n, const std::vector<std::pair<int,int>>& edges) {
    if (n <= 0) return 0;

    // Build adjacency list
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second;
        if (u >= 0 && u < n && v >= 0 && v < n && u != v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }

    std::vector<bool> visited(n, false);
    int components = 0;

    for (int start = 0; start < n; ++start) {
        if (visited[start]) continue;
        ++components;

        // Iterative DFS to avoid recursion depth issues
        std::stack<int> stack;
        stack.push(start);
        visited[start] = true;
        while (!stack.empty()) {
            int u = stack.top();
            stack.pop();
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    stack.push(v);
                }
            }
        }
    }

    return components;
}

// The problem is a classic graph connectivity task. Since the input is just a list of edges, we need to build an adjacency list (vector of vectors) for the graph. Then we perform a DFS traversal starting from each unvisited vertex. Each time we start a new DFS from an unvisited vertex, we have found one new connected component. We increment a counter and mark all vertices reachable from that start as visited. Edge cases include: `n=0` (return 0), `n>0` with no edges (each vertex is its own component, so return `n`), and graphs with multiple components of various sizes. Time complexity is O(n + E) since we visit each vertex and each edge once in the DFS. Space complexity is O(n + E) for the adjacency list plus O(n) for the visited array and recursion stack (or explicit stack). For large graphs, an iterative DFS could avoid recursion depth issues, but a recursive DFS is simpler and acceptable for typical test sizes.
