Write a C++ function `bool hasCycle(const std::vector<std::vector<int>>& adj)` that takes an undirected graph represented as an adjacency matrix (where `adj[i][j] == 1` means there is an edge between vertices `i` and `j`, and `0` means no edge) and returns `true` if the graph contains at least one cycle, and `false` otherwise. The graph has `adj.size()` vertices (assume it is square and non-empty). The graph may be disconnected, and may contain self-loops (an edge from a vertex to itself), which should be treated as a cycle. The function must not modify the input matrix. Use a recursive depth-first search (DFS) with a visited array to detect back edges, remembering the parent vertex to avoid incorrectly counting the edge back to the parent in an undirected tree. Ensure the function handles all edge cases correctly, including empty graphs, single-vertex graphs, disconnected components, and self-loops.
#include <cassert>
#include <vector>

// Include the solution function here (or link it).

int main() {
    // Empty graph
    std::vector<std::vector<int>> g0;
    assert(hasCycle(g0) == false);

    // Single vertex, no edges
    std::vector<std::vector<int>> g1 = {{0}};
    assert(hasCycle(g1) == false);

    // Single vertex with self-loop
    std::vector<std::vector<int>> g2 = {{1}};
    assert(hasCycle(g2) == true);

    // Two vertices, one edge (tree, no cycle)
    std::vector<std::vector<int>> g3 = {{0,1},{1,0}};
    assert(hasCycle(g3) == false);

    // Two vertices, two edges (parallel edges) - in a simple adjacency matrix this is still one edge, so no cycle, but for this test we simulate by setting both directions (which is already done). Actually we need a triangle or self-loop to get a cycle. So test a triangle:
    std::vector<std::vector<int>> g4 = {{0,1,1},{1,0,1},{1,1,0}};
    assert(hasCycle(g4) == true);

    // Disconnected: two separate edges
    std::vector<std::vector<int>> g5 = {{0,1,0,0},{1,0,0,0},{0,0,0,1},{0,0,1,0}};
    assert(hasCycle(g5) == false);

    // Disconnected: a cycle plus an isolated vertex
    std::vector<std::vector<int>> g6 = {{0,1,0,0},{1,0,1,0},{0,1,0,0},{0,0,0,0}};
    assert(hasCycle(g6) == true);

    // A larger tree (no cycle): line 0-1-2-3
    std::vector<std::vector<int>> g7 = {{0,1,0,0},{1,0,1,0},{0,1,0,1},{0,0,1,0}};
    assert(hasCycle(g7) == false);

    // A tree with self-loop on a leaf → cycle
    std::vector<std::vector<int>> g8 = {{0,1,0},{1,0,1},{0,1,1}};
    // Here vertex 2 has a self-loop, so cycle exists.
    assert(hasCycle(g8) == true);

    // Matrix with non-symmetric entries (directed edges) – still treated as undirected because we check both directions in the matrix, but a missing reverse edge would break undirected assumption. We'll test a symmetric case.

    // 4-vertex square (cycle)
    std::vector<std::vector<int>> g9 = {
        {0,1,0,1},
        {1,0,1,0},
        {0,1,0,1},
        {1,0,1,0}
    };
    assert(hasCycle(g9) == true);

    return 0;
}
#include <vector>

// Detects if an undirected graph (given as adjacency matrix) has a cycle.
// adj[i][j] == 1 means an edge between i and j, 0 means no edge.
// The graph is undirected (assumed symmetric). Self-loops count as cycles.
bool hasCycle(const std::vector<std::vector<int>>& adj) {
    int n = adj.size();
    std::vector<bool> visited(n, false);

    // Recursive DFS helper. Returns true if a cycle is found.
    std::function<bool(int, int)> dfs = [&](int src, int parent) -> bool {
        visited[src] = true;
        for (int i = 0; i < n; ++i) {
            if (adj[src][i] != 0) {
                if (!visited[i]) {
                    if (dfs(i, src)) {
                        return true;
                    }
                } else if (i != parent) {
                    return true; // back edge to a non-parent → cycle
                }
            }
        }
        return false;
    };

    // Check all components
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            if (dfs(i, -1)) {
                return true;
            }
        }
    }
    return false;
}
// The solution uses a standard DFS-based cycle detection for undirected graphs. For each unvisited vertex, start a DFS; during traversal, for every neighbor, if the neighbor is unvisited, recursively visit it with the current vertex as parent; if a neighbor is already visited and is not the parent, a cycle exists. A self-loop (edge to itself) is also a cycle because when the neighbor equals the source, the parent is different (unless the parent is itself, which never happens because the parent is the previous vertex), so the condition `i != parent` will be true, immediately detecting the cycle. The main algorithm runs in `O(V^2)` time because the adjacency matrix is fully scanned for each vertex, and `O(V)` auxiliary space for the visited array and recursion stack. Key edge cases: disconnected graphs (each component must be checked separately), empty graph (size 0, return false), a single vertex with no edges (false), a single vertex with a self-loop (true), and a multi-edge tree (false). The function uses `const` reference to avoid copying and applies `const` correctness to the visited array (though it must be mutable inside recursion, so it's a local non-const vector).
