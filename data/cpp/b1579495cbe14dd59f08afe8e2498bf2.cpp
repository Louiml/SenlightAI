// Write a C++ function `vector<int> findArticulationPoints(int n, const vector<vector<int>>& adj)` that returns the indices of all articulation points (cut vertices) in an undirected graph with `n` vertices labeled `0` to `n-1`. The graph is represented by an adjacency list `adj` where `adj[u]` contains all vertices adjacent to `u`. The graph may be disconnected, may contain self-loops or multiple edges between the same pair of vertices, and may have any number of vertices from 1 to 10^5. An articulation point is a vertex whose removal increases the number of connected components of the graph. Return the articulation points in increasing order of index. For example, in a simple cycle of 3 vertices, no vertex is an articulation point; in a tree with 2 or more vertices, every vertex with degree > 1 or the root with at least 2 children is an articulation point. The function must handle disconnected graphs by independently processing each connected component. You may assume the graph is undirected (the adjacency list is symmetric, possibly with duplicates), and you should not modify the input adjacency list.

#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple graph with no articulation points (cycle of 3 vertices)
    {
        int n = 3;
        std::vector<std::vector<int>> adj = {{1,2}, {0,2}, {0,1}};
        std::vector<int> result = findArticulationPoints(n, adj);
        assert(result.empty());
    }

    // Test 2: Tree with 4 vertices (1-2, 2-3, 2-4). Articulation point is vertex 2.
    {
        int n = 4;
        std::vector<std::vector<int>> adj = {{1}, {0,2,3}, {1}, {1}};
        std::vector<int> result = findArticulationPoints(n, adj);
        assert(result == std::vector<int>({1}));
    }

    // Test 3: Disconnected graph: two separate edges (0-1) and (2-3). No articulation points.
    {
        int n = 4;
        std::vector<std::vector<int>> adj = {{1}, {0}, {3}, {2}};
        std::vector<int> result = findArticulationPoints(n, adj);
        assert(result.empty());
    }

    // Test 4: Star graph with center vertex 0 and leaves 1,2,3. Center is articulation.
    {
        int n = 4;
        std::vector<std::vector<int>> adj = {{1,2,3}, {0}, {0}, {0}};
        std::vector<int> result = findArticulationPoints(n, adj);
        assert(result == std::vector<int>({0}));
    }

    // Test 5: Single vertex graph. No articulation points.
    {
        int n = 1;
        std::vector<std::vector<int>> adj = {{}};
        std::vector<int> result = findArticulationPoints(n, adj);
        assert(result.empty());
    }

    // Test 6: Graph with self-loop and multiple edges: vertices 0 and 1 connected by two edges, plus self-loop on 0.
    // No articulation points because removing either vertex leaves the other isolated as a single component (component count unchanged).
    {
        int n = 2;
        std::vector<std::vector<int>> adj = {{1,1,0}, {0,0}};  // vertex 0 has edges to 1 (twice) and self, vertex 1 has edges to 0 (twice)
        std::vector<int> result = findArticulationPoints(n, adj);
        assert(result.empty());
    }

    // Test 7: Graph where root is not an articulation point but internal vertex is.
    // Path 0-1-2-3. Articulation points are 1 and 2.
    {
        int n = 4;
        std::vector<std::vector<int>> adj = {{1}, {0,2}, {1,3}, {2}};
        std::vector<int> result = findArticulationPoints(n, adj);
        assert(result == std::vector<int>({1,2}));
    }

    // Test 8: Graph with a bridge but no articulation points if bridge connects two cycles? Actually bridge means endpoints are cuts unless endpoint is leaf? 
    // Example: two triangles connected by a single edge between vertices 0 and 3. Articulation points are 0 and 3.
    {
        int n = 6;
        std::vector<std::vector<int>> adj = {{1,2,3}, {0,2}, {0,1}, {0,4,5}, {3,5}, {3,4}};
        std::vector<int> result = findArticulationPoints(n, adj);
        assert(result == std::vector<int>({0,3}));
    }

    // Test 9: Graph with single edge between 0 and 1. No articulation points.
    {
        int n = 2;
        std::vector<std::vector<int>> adj = {{1}, {0}};
        std::vector<int> result = findArticulationPoints(n, adj);
        assert(result.empty());
    }

    // Test 10: Disconnected components where one component is a star and another is a cycle.
    // Star component: vertices 0(center),1,2. Cycle component: vertices 3,4,5.
    // Articulation point is 0.
    {
        int n = 6;
        std::vector<std::vector<int>> adj = {{1,2}, {0}, {0}, {4,5}, {3,5}, {3,4}};
        std::vector<int> result = findArticulationPoints(n, adj);
        assert(result == std::vector<int>({0}));
    }

    return 0;
}

#include <vector>
#include <functional>
#include <algorithm>

// Returns indices of all articulation points (cut vertices) in an undirected graph.
// Graph is given as adjacency list 'adj' of size n. Vertices are 0..n-1.
// Handles disconnected graphs, self-loops, and multiple edges.
std::vector<int> findArticulationPoints(int n, const std::vector<std::vector<int>>& adj) {
    int timer = 0;
    std::vector<int> discovery(n, -1);   // -1 means unvisited
    std::vector<int> low(n, 0);
    std::vector<int> isCut(n, 0);

    // Depth-first search from vertex u with parent p. p = -1 for root.
    std::function<void(int, int)> dfs = [&](int u, int parent) {
        discovery[u] = low[u] = timer++;
        int childCount = 0;
        for (int v : adj[u]) {
            if (discovery[v] == -1) {
                // v is a child in DFS tree
                dfs(v, u);
                low[u] = std::min(low[u], low[v]);
                // If v cannot reach an ancestor of u (other than via u), then u is a cut vertex
                if (low[v] >= discovery[u] && parent != -1) {
                    isCut[u] = 1;
                }
                childCount++;
            } else if (v != parent) {
                // Back edge to an ancestor (or a parallel edge, handled by ignoring parent)
                low[u] = std::min(low[u], discovery[v]);
            }
        }
        // Root vertex of a DFS tree is a cut vertex if it has more than one child
        if (parent == -1 && childCount > 1) {
            isCut[u] = 1;
        }
    };

    // Process all connected components
    for (int i = 0; i < n; ++i) {
        if (discovery[i] == -1) {
            dfs(i, -1);
        }
    }

    // Collect and return articulation points in increasing order
    std::vector<int> result;
    for (int i = 0; i < n; ++i) {
        if (isCut[i]) {
            result.push_back(i);
        }
    }
    return result;
}

// The solution uses a depth-first search (DFS) with discovery time (`dfn`) and low-link values (`low`). For each vertex `u`, `dfn[u]` records the order in which it is first visited, and `low[u]` is the smallest discovery time reachable from `u` via the DFS tree edges and at most one back edge. The algorithm computes these values via a recursive DFS that also tracks the parent vertex `p`. For a non-root vertex `u`, `u` is an articulation point if there exists a child `v` in the DFS tree such that `low[v] >= dfn[u]`, meaning that the subtree rooted at `v` cannot reach any ancestor of `u` without passing through `u`. For the root of a DFS tree, it is an articulation point if it has more than one child in the DFS tree (because removing the root disconnects those children's subtrees from each other). The DFS must be started from every unvisited vertex to handle disconnected graphs. Self-loops and multiple edges are naturally handled because the DFS only considers unvisited neighbors for tree edges, and when visiting an already-visited neighbor that is not the parent, it considers a back edge and updates `low[u]` with `dfn[v]`. The algorithm runs in O(n + m) time and O(n) auxiliary space for the arrays, where `m` is the total number of edges (including duplicates). Edge cases include: a graph with a single vertex (no articulation points), a graph with two vertices connected by one or more edges (neither vertex is an articulation point because removing one leaves the other isolated as a single component, but the number of components increases from 1 to 2 only if the remaining vertex is removed; actually for two vertices connected by an edge, removing either vertex leaves the other isolated, so the number of components goes from 1 to 2? No: removing a vertex that is a leaf in a tree of size 2 removes that vertex, leaving one vertex, which is still one component? Let's check: original graph has one component. Remove vertex 0, the graph has only vertex 1, which is one component. The number of components does not increase, so vertex 0 is not an articulation point. Similarly for vertex 1. So for n=2 with an edge, no articulation points. For n=2 with no edge, the graph has two components, removing either vertex leaves one component, so the number of components decreases, not increases, so none are articulation points. For n=1, no articulation points. All these are handled correctly by the algorithm.
