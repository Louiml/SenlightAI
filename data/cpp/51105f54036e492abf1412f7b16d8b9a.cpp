You are given an undirected connected graph with `V` vertices (numbered 1 to `V`) and `E` edges. Write a C++ function `findBridges` that takes the number of vertices `V`, the number of edges `E`, and a vector of pairs representing the edges, and returns a vector of pairs (each pair sorted ascending, e.g., `{u,v}` with `u < v`) containing all bridges (critical edges) in the graph. A bridge is an edge whose removal increases the number of connected components. The graph may contain multiple edges between the same pair of vertices and self-loops; such edges are never bridges. The function must handle disconnected graphs as well. The output vector should contain each bridge exactly once, sorted lexicographically (first by first vertex, then by second vertex). The order of edges in the output does not matter as long as it is deterministic and sorted.

// The solution uses Tarjan's bridge-finding algorithm based on Depth-First Search (DFS). For each connected component, we run DFS from an unvisited vertex. During DFS, we maintain three arrays: `tin` (discovery time), `tlow` (lowest discovery time reachable from the subtree), and `vis` (visited flag). For each edge from vertex `u` to vertex `v`, if `v` is the parent (the vertex from which `u` was discovered), we skip it (to avoid treating the tree edge back to the parent as a back edge). If `v` is not visited, we recursively DFS and after returning, update `tlow[u] = min(tlow[u], tlow[v])`. If `tlow[v] > tin[u]`, then edge `(u,v)` is a bridge because there is no back edge from the subtree rooted at `v` to an ancestor of `u`. If `v` is already visited and is not the parent, it's a back edge, so we update `tlow[u] = min(tlow[u], tin[v])` (note: in the original snippet they update with `tlow[v]` which works too because for visited vertices `tlow[v]` is at most `tin[v]`, but using `tin[v]` is standard). Important edge cases: self-loops are never bridges because removal does not disconnect the graph (they don't affect connectivity); multiple edges between the same vertices are also not bridges because removing one leaves the other connection. To handle multiple edges, we must avoid using a simple parent check that skips all edges to the parent; instead, we need to count the number of times we traverse to the parent. A common approach is to pass the edge index as the parent parameter instead of the vertex. We'll do that here: for each adjacency list entry, store the neighbor and the edge index (from the input list). When DFS encounters the edge back to the parent, it skips only if the edge index is the same as the one used to come in. Self-loops are processed but they will never satisfy the bridge condition because `tlow[child] > tin[parent]` requires a child vertex; self-loop has `v == u`, so `u` and `v` are same, and after visiting, the condition won't be true. We need to sort each output pair as `{min(u,v), max(u,v)}` and then sort the entire vector of bridges. Time complexity: O(V + E) for the DFS, plus O(B log B) for sorting bridges, where B is the number of bridges; overall O(V + E + B log B). Space complexity: O(V + E) for adjacency list, plus O(V) for arrays.

#include <vector>
#include <algorithm>
#include <utility>

// Find all bridges in an undirected graph.
// V: number of vertices (1-indexed), E: number of edges, edges: list of edges.
// Returns a sorted vector of bridge pairs {u, v} with u < v.
std::vector<std::pair<int, int>> findBridges(int V, int E,
    const std::vector<std::pair<int, int>>& edges) {
    // Build adjacency list: each element is {neighbor, edge_index}
    std::vector<std::vector<std::pair<int, int>>> adj(V + 1);
    for (int i = 0; i < E; ++i) {
        int u = edges[i].first;
        int v = edges[i].second;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }

    std::vector<int> tin(V + 1, -1);
    std::vector<int> tlow(V + 1, -1);
    std::vector<bool> vis(V + 1, false);
    int timer = 0;
    std::vector<std::pair<int, int>> bridges;

    // Recursive DFS using edge index as parent to handle multiple edges
    // and self-loops correctly.
    std::function<void(int, int)> dfs = [&](int u, int parentEdge) {
        vis[u] = true;
        tin[u] = tlow[u] = timer++;
        for (const auto& [v, edgeIdx] : adj[u]) {
            if (edgeIdx == parentEdge) continue;  // skip the edge we came from
            if (!vis[v]) {
                dfs(v, edgeIdx);
                tlow[u] = std::min(tlow[u], tlow[v]);
                if (tlow[v] > tin[u]) {
                    // Edge (u, v) is a bridge; store sorted pair
                    int a = u, b = v;
                    if (a > b) std::swap(a, b);
                    bridges.push_back({a, b});
                }
            } else {
                // Back edge (not parent); update low with discovery time of v
                tlow[u] = std::min(tlow[u], tin[v]);
            }
        }
    };

    // Run DFS on all connected components (graph may be disconnected)
    for (int i = 1; i <= V; ++i) {
        if (!vis[i]) {
            dfs(i, -1);  // -1 is not a valid edge index, so no skip
        }
    }

    // Sort the bridges lexicographically
    std::sort(bridges.begin(), bridges.end());
    return bridges;
}

#include <cassert>
#include <vector>
#include <utility>

// Include the solution function here (or include header)

int main() {
    // Test 1: Simple triangle (no bridges)
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,1}};
        auto res = findBridges(3, 3, edges);
        assert(res.empty());
    }
    // Test 2: Line graph - all edges are bridges
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4}};
        auto res = findBridges(4, 3, edges);
        assert(res.size() == 3);
        assert(res[0] == std::make_pair(1,2));
        assert(res[1] == std::make_pair(2,3));
        assert(res[2] == std::make_pair(3,4));
    }
    // Test 3: Disconnected graph with one bridge in each component
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{4,5}}; // 1-2-3? Actually 1-2 and 1-3 is a star, no bridge; 4-5 is bridge
        // Actually 1-2,1-3 star has no bridge; 4-5 is a bridge
        auto res = findBridges(5, 3, edges);
        assert(res.size() == 1);
        assert(res[0] == std::make_pair(4,5));
    }
    // Test 4: Self-loop and multiple edges - no bridges
    {
        std::vector<std::pair<int,int>> edges = {{1,1},{1,2},{1,2},{2,3},{2,3}};
        auto res = findBridges(3, 5, edges);
        // Edge 1-2 appears twice, so not a bridge; 2-3 appears twice, not a bridge
        assert(res.empty());
    }
    // Test 5: Single vertex with no edges - no bridges
    {
        std::vector<std::pair<int,int>> edges;
        auto res = findBridges(1, 0, edges);
        assert(res.empty());
    }
    // Test 6: Graph with a bridge and multiple edges elsewhere
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,5},{3,4}}; // 3-4 appears twice, so not a bridge; 1-2 is bridge, 2-3 is bridge, 4-5 is bridge
        auto res = findBridges(5, 5, edges);
        assert(res.size() == 3);
        assert(res[0] == std::make_pair(1,2));
        assert(res[1] == std::make_pair(2,3));
        assert(res[2] == std::make_pair(4,5));
    }
    // Test 7: Larger graph with cycle and a bridge attached
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,1},{3,4},{4,5}};
        auto res = findBridges(5, 5, edges);
        assert(res.size() == 2);
        assert(res[0] == std::make_pair(3,4));
        assert(res[1] == std::make_pair(4,5));
    }
    // Test 8: Input order of u,v in edge list is unsorted; output sorted
    {
        std::vector<std::pair<int,int>> edges = {{3,4},{1,2},{2,3},{4,5}};
        // Actually edges: 3-4,1-2,2-3,4-5 -> bridges: 1-2,2-3,3-4,4-5? Wait: 1-2 is bridge, 2-3 is bridge, 3-4 is bridge, 4-5 is bridge but 3-4 and 4-5 are part of a line, all are bridges
        auto res = findBridges(5, 4, edges);
        assert(res.size() == 4);
        assert(res == std::vector<std::pair<int,int>>({{1,2},{2,3},{3,4},{4,5}}));
    }
    return 0;
}
