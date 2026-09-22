// Given an undirected graph represented by a fixed-size adjacency list (`vector<int> adj[MAXN]`) and a global level array `lvl` initially set to `-1`, write a standalone C++ function `vector<Edge> findBridges(int n, const vector<vector<int>>& graph)` that takes the number of vertices `n` and an adjacency list `graph` (0-indexed) and returns a list of all edges `(u, v)` together with a boolean indicating whether that edge is a bridge in the graph. Use Tarjan's DFS-based algorithm with back-edge detection as demonstrated in the snippet. The graph is simple (no self-loops, at most one edge between any pair), but it may be disconnected. Your function must correctly mark an edge as a bridge if its removal increases the number of connected components. Handle parallel edges (if present) correctly, and ensure the output order is by increasing `u`, then increasing `v`. Do not use global mutable state; allocate local arrays inside the function.

// The solution is based on Tarjan's bridge-finding algorithm using DFS with discovery times (levels). We perform DFS from each unvisited node to handle disconnected graphs. For each vertex `u`, we set its level `lvl[u]` to the current depth. For each neighbor `v`:
// - If `v` is the parent and it's the first time we see the parent (to handle parallel edges), we skip it (using a `parentSeen` counter). In a simple graph without parallel edges, this is just `v != parent`.
// - If `v` is already visited (back edge), we treat it as a candidate low-link value `v` (its level).
// - Otherwise, recursively visit `v`; the returned value from recursion is the minimum level reachable from `v` via back edges or descendant bridges.
//
// After processing all neighbors of `u`, we compute `low` as the minimum level among all candidates. If `low` is at least `lvl[u]`, then the edge from `u` to its parent `p` is a bridge (because there is no back edge from the subtree to an ancestor above `u`). If `u` is the root (no parent), we don't record a bridge. Otherwise, we store the edge with the bridge flag.
//
// Edge cases: isolated vertices produce no edges. For the root, we must not mark any edge as a bridge based on `low`, but the standard condition still works because the parent is `-1`. Parallel edges: if two vertices `u` and `v` are connected by two parallel edges, neither edge is a bridge. To handle this, we treat the first occurrence of the parent as a skip, and the second occurrence as a back edge, which prevents a false bridge. Since the graph is simple by the problem statement, this is mostly a safety measure.
//
// Time complexity: O(V + E) because each vertex and edge is visited once. Space complexity: O(V) for recursion stack (in worst case) plus auxiliary arrays for `lvl` and `visited`, and O(E) to store the result.

#include <vector>
#include <algorithm>

struct Edge {
    int u, v;
    bool isBridge;
};

// Find all bridges in an undirected graph using Tarjan's algorithm.
// Graph is 0-indexed, may be disconnected. No self-loops, but may have parallel edges.
std::vector<Edge> findBridges(int n, const std::vector<std::vector<int>>& graph) {
    std::vector<int> lvl(n, -1);
    std::vector<Edge> result;
    int numEdges = 0;
    // For storing edges uniquely: we'll use a local recursive lambda.
    // We need to track parent occurrence count for parallel edge handling.

    // Recursive DFS function that returns the minimum level reachable from u
    // via back edges or descendants. 'parent' is the vertex we came from, -1 for root.
    // 'parentEdgeIndex' is used to skip the specific edge to parent (not just the vertex)
    // to correctly handle parallel edges. Since graph is simple, we can just skip one parent occurrence.
    std::function<int(int, int, int)> dfs = [&](int u, int parent, int level) -> int {
        lvl[u] = level;
        int low = level; // minimum level reachable from u
        bool parentSeen = false;

        for (int v : graph[u]) {
            if (v == parent && !parentSeen) {
                // First time we encounter the parent via the direct edge.
                parentSeen = true;
                continue;
            }
            int childLow = -1;
            if (lvl[v] != -1) {
                // Back edge to an ancestor or cross edge (in undirected, only ancestor).
                childLow = lvl[v];
            } else {
                // Tree edge: recurse.
                childLow = dfs(v, u, level + 1);
            }
            // childLow is the minimum level reachable from v's subtree.
            // For bridge detection on edge (u,v), we need to know if v's subtree can reach
            // above u. The returned 'low' from dfs(v) already accounts for that.
            // We track the minimum over all children.
            if (childLow != -1) {
                low = std::min(low, childLow);
            } else {
                // If no back edge, low remains as is.
            }
        }

        // After processing all neighbors, 'low' is the minimum level reachable from u's subtree.
        // If low > level (i.e., low == level because it can't be more), then the edge from parent to u
        // is a bridge. But we need to know if u's subtree can reach an ancestor above u.
        // In the original snippet, they used an additional check. Let's implement correctly:
        // The condition for edge (parent, u) to be a bridge is if there is no back edge from u's subtree
        // to an ancestor of u (including u itself). So we need to return the minimum level reachable.
        // If that minimum is strictly greater than level (i.e., equal to level but not lower), it's a bridge.
        // Actually, the standard Tarjan algorithm uses disc[u] and low[u] where low[u] = min(disc[u], disc[w]) for back edges, and low[u] = min(low[u], low[v]) for tree edges.
        // Bridge if low[v] > disc[u] for edge (u,v).
        // Here we have level as disc. So for each tree edge (parent, u), we need to check after DFS of u.
        // Let's refactor: we'll handle bridge recording during recursion.
        // To do that, we need to process tree edges after recursion.
        // We'll change the approach: use a separate visited array and compute low.
        // I'll rewrite cleanly.

        // Simpler approach: use standard Tarjan.
        // I'll re-implement properly.
    };

    // Let's implement a proper version.

    std::vector<int> disc(n, -1);
    int time = 0;
    std::vector<Edge> bridges;

    std::function<int(int, int)> tarjan = [&](int u, int parent) -> int {
        disc[u] = time++;
        int low = disc[u];
        bool parentSeen = false;

        for (int v : graph[u]) {
            if (v == parent && !parentSeen) {
                parentSeen = true; // skip the direct edge to parent once
                continue;
            }
            if (disc[v] == -1) {
                // tree edge
                int vLow = tarjan(v, u);
                low = std::min(low, vLow);
                if (vLow > disc[u]) {
                    // edge (u,v) is a bridge
                    Edge e;
                    e.u = std::min(u, v);
                    e.v = std::max(u, v);
                    e.isBridge = true;
                    bridges.push_back(e);
                } else {
                    // not a bridge, but still record for completeness? The task expects all edges.
                    // We'll record all edges later.
                }
            } else {
                // back edge to ancestor
                low = std::min(low, disc[v]);
            }
        }
        return low;
    };

    // To return all edges, we first collect all unique edges in the graph.
    std::vector<std::pair<int,int>> allEdges;
    for (int u = 0; u < n; ++u) {
        for (int v : graph[u]) {
            if (u < v) {
                allEdges.push_back({u, v});
            }
        }
    }
    // Sort edges by u then v
    std::sort(allEdges.begin(), allEdges.end());
    allEdges.erase(std::unique(allEdges.begin(), allEdges.end()), allEdges.end());

    // Run tarjan for all components
    std::vector<std::pair<int,int>> bridgeSet;
    for (int i = 0; i < n; ++i) {
        if (disc[i] == -1) {
            // We need to capture bridges via tarjan, but we are not storing them properly.
            // Let's use a modified tarjan that records bridges in a set.
            // I'll redo with a cleaner implementation.

        }
    }

    // --------------------------------------------
    // Clean implementation:
    // --------------------------------------------
    std::vector<int> levels(n, -1);
    std::vector<Edge> ans;
    int timer = 0;
    std::vector<std::pair<int,int>> foundBridges;

    std::function<int(int,int)> dfs2 = [&](int u, int parent) -> int {
        levels[u] = ++timer;
        int low = levels[u];
        bool parentSeen = false;

        for (int v : graph[u]) {
            if (v == parent && !parentSeen) {
                parentSeen = true;
                continue;
            }
            if (levels[v] != -1) {
                low = std::min(low, levels[v]);
            } else {
                int vLow = dfs2(v, u);
                low = std::min(low, vLow);
                if (vLow > levels[u]) {
                    foundBridges.push_back({std::min(u,v), std::max(u,v)});
                }
            }
        }
        return low;
    };

    for (int i = 0; i < n; ++i) {
        if (levels[i] == -1) {
            dfs2(i, -1);
        }
    }

    // Build result for all unique edges
    std::sort(foundBridges.begin(), foundBridges.end());
    for (auto& e : allEdges) {
        bool isBridge = std::binary_search(foundBridges.begin(), foundBridges.end(), e);
        ans.push_back({e.first, e.second, isBridge});
    }

    return ans;
}

#include <cassert>
#include <vector>

// Include the solution function here (from above) 

int main() {
    // Test 1: Simple line 0-1-2, all edges are bridges
    {
        std::vector<std::vector<int>> g = {{1}, {0,2}, {1}};
        auto bridges = findBridges(3, g);
        assert(bridges.size() == 2);
        assert(bridges[0].isBridge == true);
        assert(bridges[1].isBridge == true);
        assert(bridges[0].u == 0 && bridges[0].v == 1);
        assert(bridges[1].u == 1 && bridges[1].v == 2);
    }
    // Test 2: Triangle 0-1-2-0, no bridges
    {
        std::vector<std::vector<int>> g = {{1,2}, {0,2}, {0,1}};
        auto bridges = findBridges(3, g);
        assert(bridges.size() == 3);
        for (auto& e : bridges) assert(e.isBridge == false);
    }
    // Test 3: Disconnected graph: edge 0-1 bridge, isolated vertex 2
    {
        std::vector<std::vector<int>> g = {{1}, {0}, {}};
        auto bridges = findBridges(3, g);
        assert(bridges.size() == 1);
        assert(bridges[0].u == 0 && bridges[0].v == 1);
        assert(bridges[0].isBridge == true);
    }
    // Test 4: Two connected components with bridges
    {
        std::vector<std::vector<int>> g = {{1}, {0,2}, {1}, {4}, {3,5}, {4}};
        auto bridges = findBridges(6, g);
        // Edges: (0,1), (1,2), (3,4), (4,5) all bridges
        assert(bridges.size() == 4);
        for (auto& e : bridges) assert(e.isBridge == true);
    }
    // Test 5: Square with one diagonal (0-1,1-2,2-3,3-0,0-2). Bridges? None.
    {
        std::vector<std::vector<int>> g = {{1,3,2}, {0,2}, {1,3,0}, {0,2}};
        auto bridges = findBridges(4, g);
        assert(bridges.size() == 5);
        for (auto& e : bridges) assert(e.isBridge == false);
    }
    // Test 6: Single vertex no edges
    {
        std::vector<std::vector<int>> g = {{}};
        auto bridges = findBridges(1, g);
        assert(bridges.empty());
    }
    // Test 7: Two vertices with two parallel edges (graph is not simple by problem statement, but we handle parallel)
    {
        std::vector<std::vector<int>> g = {{1,1}, {0,0}};
        auto bridges = findBridges(2, g);
        // There are 2 edges, both should be non-bridges
        assert(bridges.size() == 2);
        for (auto& e : bridges) assert(e.isBridge == false);
    }
    // Test 8: Star with center 0 and leaves 1,2,3. All edges are bridges.
    {
        std::vector<std::vector<int>> g = {{1,2,3}, {0}, {0}, {0}};
        auto bridges = findBridges(4, g);
        assert(bridges.size() == 3);
        for (auto& e : bridges) assert(e.isBridge == true);
    }
    return 0;
}
