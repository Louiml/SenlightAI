// Implement a C++ function that checks whether a given undirected graph is biconnected. Write a free function `bool isBiconnected(const std::vector<std::vector<int>>& adj)` that takes an adjacency list representing an undirected graph (vertices numbered 0..n-1, with each edge listed in both directions) and returns `true` if the graph is biconnected. A graph is biconnected if it is connected and remains connected after removing any single vertex (i.e., it has no articulation points). Handle edge cases: an empty graph (n=0) is not biconnected; a graph with a single vertex (n=1) is considered biconnected; a graph with two vertices and one edge is biconnected; isolated vertices make the graph disconnected (not biconnected). You must not modify the input.
// The solution uses Tarjan's algorithm for finding articulation points in an undirected graph. We perform a DFS while keeping track of discovery time (`tin`) and the lowest reachable discovery time (`low`) for each vertex. During DFS, we count the number of children in the DFS tree for each vertex. A vertex `u` is an articulation point if:
// - `u` is the root of the DFS tree and has more than one child.
// - `u` is not the root and there exists a child `v` such that `low[v] >= tin[u]` (meaning no back edge from `v`'s subtree reaches an ancestor of `u`).
//
// If any articulation point exists, or if the graph is disconnected (DFS doesn't visit all vertices from one start), then the graph is not biconnected. For n=1, return `true` because removing the single vertex leaves an empty graph (trivially connected). For n=0, return `false` as there is no meaningful connectivity. For n=2 with an edge, there are no articulation points, and the graph is connected, so it is biconnected. Time complexity is O(n + m) where m is the number of edges. Space complexity is O(n) for recursion stack and auxiliary arrays.
#include <vector>
#include <algorithm>

// Returns true if the undirected graph (adjacency list, 0-indexed) is biconnected.
// Biconnected means connected and no articulation points (removing any single vertex
// keeps the graph connected). Empty graph (n=0) is not biconnected.
// Single vertex (n=1) is biconnected.
bool isBiconnected(const std::vector<std::vector<int>>& adj) {
    int n = (int)adj.size();
    if (n == 0) return false;
    if (n == 1) return true;

    std::vector<int> tin(n, -1);
    std::vector<int> low(n, -1);
    int timer = 0;
    bool hasArticulation = false;
    int rootChildren = 0;

    // Recursive DFS to compute tin and low, and detect articulation points.
    // `parent` is the DFS parent; for root parent = -1.
    std::function<void(int, int)> dfs = [&](int u, int parent) {
        tin[u] = low[u] = timer++;
        int children = 0;
        for (int v : adj[u]) {
            if (v == parent) continue;
            if (tin[v] != -1) {
                // Back edge (u,v) where v is an ancestor of u.
                low[u] = std::min(low[u], tin[v]);
            } else {
                // Forward edge to an unvisited vertex.
                dfs(v, u);
                low[u] = std::min(low[u], low[v]);
                if (low[v] >= tin[u] && parent != -1) {
                    hasArticulation = true; // u is an articulation point (non-root)
                }
                children++;
            }
        }
        if (parent == -1 && children > 1) {
            hasArticulation = true; // root is articulation point
        }
    };

    // Start DFS from vertex 0. If the graph is disconnected, DFS won't visit all vertices.
    dfs(0, -1);
    for (int i = 0; i < n; ++i) {
        if (tin[i] == -1) return false; // disconnected
    }

    return !hasArticulation;
}
#include <cassert>
#include <vector>

// Include the solution function here (or via a header).

int main() {
    // Single vertex
    std::vector<std::vector<int>> g1 = {{}};
    assert(isBiconnected(g1) == true);

    // Empty graph
    std::vector<std::vector<int>> g0 = {};
    assert(isBiconnected(g0) == false);

    // Two vertices one edge (biconnected)
    std::vector<std::vector<int>> g2 = {{1}, {0}};
    assert(isBiconnected(g2) == true);

    // Path of 3 vertices (not biconnected; middle is articulation)
    std::vector<std::vector<int>> g3 = {{1}, {0,2}, {1}};
    assert(isBiconnected(g3) == false);

    // Triangle (biconnected)
    std::vector<std::vector<int>> g4 = {{1,2}, {0,2}, {0,1}};
    assert(isBiconnected(g4) == true);

    // Disconnected graph (not biconnected)
    std::vector<std::vector<int>> g5 = {{1}, {0}, {}};
    assert(isBiconnected(g5) == false);

    // Square with a diagonal? Actually a square cycle is biconnected.
    std::vector<std::vector<int>> g6 = {{1,3}, {0,2}, {1,3}, {0,2}};
    assert(isBiconnected(g6) == true);

    // Square with a tail (not biconnected)
    std::vector<std::vector<int>> g7 = {{1,3}, {0,2}, {1,3}, {0,2,4}, {3}};
    assert(isBiconnected(g7) == false);

    // Two vertices no edge (disconnected)
    std::vector<std::vector<int>> g8 = {{}, {}};
    assert(isBiconnected(g8) == false);

    // Star graph with 4 leaves (center is articulation)
    std::vector<std::vector<int>> g9 = {{1,2,3}, {0}, {0}, {0}};
    assert(isBiconnected(g9) == false);

    return 0;
}
