// Given an undirected connected graph with V vertices (numbered 1 to V) and E edges, write a C++ function `findBridges(int V, const std::vector<std::pair<int,int>>& edges)` that returns a vector of pairs `(int a, int b)` representing all **bridges** (cut edges) in the graph. A bridge is an edge whose removal increases the number of connected components. The output pairs must satisfy `a < b` for each pair, must be sorted in lexicographical order (first by `a`, then by `b`), and each bridge must appear exactly once. The graph is connected, but may contain parallel edges (multiple edges between the same pair of vertices) — a parallel edge pair is **not** a bridge unless all parallel edges between those vertices are bridges (i.e., if there are two or more parallel edges between the same vertices, none of them is a bridge). The function should use Tarjan’s algorithm (DFS-based) to find bridges efficiently.

#include <cassert>
#include <vector>
#include <utility>

// Provided solution function (included here for completeness; assume it's available).
std::vector<std::pair<int,int>> findBridges(int V, const std::vector<std::pair<int,int>>& edges);

int main() {
    // Test 1: Simple triangle (no bridges)
    std::vector<std::pair<int,int>> e1 = {{1,2},{2,3},{3,1}};
    std::vector<std::pair<int,int>> b1 = findBridges(3, e1);
    assert(b1.empty());

    // Test 2: Line graph 1-2-3, bridges are both edges, sorted.
    std::vector<std::pair<int,int>> e2 = {{1,2},{2,3}};
    std::vector<std::pair<int,int>> b2 = findBridges(3, e2);
    assert(b2.size() == 2);
    assert(b2[0] == std::make_pair(1,2));
    assert(b2[1] == std::make_pair(2,3));

    // Test 3: Parallel edges between 1-2 plus edge 2-3. Bridge is only 2-3.
    std::vector<std::pair<int,int>> e3 = {{1,2},{1,2},{2,3}};
    std::vector<std::pair<int,int>> b3 = findBridges(3, e3);
    assert(b3.size() == 1);
    assert(b3[0] == std::make_pair(2,3));

    // Test 4: Star graph center 1 connected to 2,3,4. All edges are bridges.
    std::vector<std::pair<int,int>> e4 = {{1,2},{1,3},{1,4}};
    std::vector<std::pair<int,int>> b4 = findBridges(4, e4);
    assert(b4.size() == 3);
    assert(b4[0] == std::make_pair(1,2));
    assert(b4[1] == std::make_pair(1,3));
    assert(b4[2] == std::make_pair(1,4));

    // Test 5: Two cycles connected by a single edge (bridge): 1-2-3-1 and 4-5-4 (needs 3 vertices? Actually 4-5-6-4), plus bridge 3-4.
    std::vector<std::pair<int,int>> e5 = {{1,2},{2,3},{3,1},{3,4},{4,5},{5,6},{6,4}};
    std::vector<std::pair<int,int>> b5 = findBridges(6, e5);
    assert(b5.size() == 1);
    assert(b5[0] == std::make_pair(3,4));

    // Test 6: Larger graph with multiple bridges, unsorted input.
    std::vector<std::pair<int,int>> e6 = {{4,5},{1,2},{2,3},{3,4},{1,5},{5,6},{6,7}};
    // Graph: 1-2-3-4-5-1 forms a cycle; 5-6-7 is a tail, so bridges: 5-6 and 6-7.
    std::vector<std::pair<int,int>> b6 = findBridges(7, e6);
    assert(b6.size() == 2);
    assert(b6[0] == std::make_pair(5,6));
    assert(b6[1] == std::make_pair(6,7));

    return 0;
}

#include <vector>
#include <algorithm>

// Find all bridges in an undirected, connected graph.
// Edges are 1-indexed vertex pairs; returns bridges as (a,b) with a<b, sorted lexicographically.
std::vector<std::pair<int,int>> findBridges(int V, const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency list with edge indices to handle parallel edges.
    std::vector<std::vector<std::pair<int,int>>> adj(V + 1); // (neighbor, edge_index)
    for (int i = 0; i < (int)edges.size(); ++i) {
        int a = edges[i].first;
        int b = edges[i].second;
        adj[a].push_back({b, i});
        adj[b].push_back({a, i});
    }

    std::vector<int> disc(V + 1, 0);
    std::vector<int> low(V + 1, 0);
    std::vector<bool> visited(V + 1, false);
    int time = 0;
    std::vector<std::pair<int,int>> bridges;

    // Depth-first search from vertex u, with parent vertex p and edge index pEdge.
    // Lambda captures everything by reference.
    auto dfs = [&](int u, int p, int pEdge, auto&& dfs_ref) -> void {
        visited[u] = true;
        disc[u] = low[u] = ++time;

        for (const auto& [v, edgeIdx] : adj[u]) {
            if (edgeIdx == pEdge) continue; // Skip the edge we came from (to handle parallel edges correctly)
            if (!visited[v]) {
                dfs_ref(v, u, edgeIdx, dfs_ref);
                low[u] = std::min(low[u], low[v]);
                if (low[v] > disc[u]) {
                    // Bridge found: (u, v)
                    if (u < v) bridges.push_back({u, v});
                    else bridges.push_back({v, u});
                }
            } else {
                // Back edge to an ancestor or a parallel edge (different edge index)
                low[u] = std::min(low[u], disc[v]);
            }
        }
    };

    // Graph is connected, so start from vertex 1.
    dfs(1, 0, -1, dfs);

    // Sort bridges lexicographically.
    std::sort(bridges.begin(), bridges.end());

    return bridges;
}

// The solution uses a DFS traversal from vertex 1 (the graph is connected, so one traversal suffices). Maintain for each vertex `u` a discovery time `disc[u]` and the earliest discovery time reachable from `u` via back edges or tree edges (`low[u]`). During DFS, for each tree edge `(u, v)` where `v` is a child of `u`, if `low[v] > disc[u]`, then no back edge from the subtree rooted at `v` reaches an ancestor of `u`, so `(u, v)` is a bridge. Handle parallel edges by passing the parent edge index to DFS; if we encounter the same edge index (i.e., the reverse direction of the same edge), we skip it, but parallel edges (different indices) will be processed normally, correctly preventing a false bridge. For each bridge found, store the pair with smaller vertex first. After DFS, sort the vector of pairs lexicographically. Time complexity is O(V + E) for DFS plus O(B log B) for sorting B bridges (worst-case O(E log E)); space complexity is O(V + E) for adjacency list and recursion stack.
