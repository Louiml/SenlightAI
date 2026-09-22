Write a C++ function `bool hasCycleUndirected(int n, const std::vector<std::pair<int,int>>& edges)` that takes the number of vertices `n` (vertices numbered `0` to `n-1`) and a vector of undirected edges, and returns `true` if the graph contains at least one cycle, and `false` otherwise. The graph may be disconnected, may contain self-loops (an edge from a vertex to itself), and may contain multiple edges between the same pair of vertices. Use a breadth-first search (BFS) traversal (not DFS) to detect cycles, and assume that vertices without edges are isolated. The function must handle an empty edge list and graphs with up to `n = 10^5` vertices and `10^5` edges efficiently. Return `true` for any self-loop (since it creates a cycle) and also for parallel edges (since they form a cycle of length 2).
#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be defined above.
// Include the solution code here for completeness in a single file.

int main() {
    // No edges
    assert(!hasCycleUndirected(5, {}));

    // Single isolated self-loop
    assert(hasCycleUndirected(3, {{0,0}}));

    // Simple tree (no cycle)
    assert(!hasCycleUndirected(4, {{0,1},{1,2},{2,3}}));

    // Triangle cycle
    assert(hasCycleUndirected(3, {{0,1},{1,2},{2,0}}));

    // Parallel edges - cycle of length 2
    assert(hasCycleUndirected(2, {{0,1},{0,1}}));

    // Disconnected components: one cycle, one tree
    assert(hasCycleUndirected(6, {{0,1},{1,2},{2,0},{3,4}}));

    // Larger graph with no cycle (tree with 7 nodes)
    assert(!hasCycleUndirected(7, {{0,1},{0,2},{0,3},{1,4},{1,5},{3,6}}));

    // Graph where cycle appears after connecting two components
    assert(hasCycleUndirected(5, {{0,1},{1,2},{2,0},{3,4},{2,3}}));

    // Self-loop on non-root vertex
    assert(hasCycleUndirected(4, {{0,1},{1,1},{1,2}}));

    // Single vertex, no edges
    assert(!hasCycleUndirected(1, {}));

    return 0;
}
#include <vector>
#include <queue>
#include <utility>

// Check if an undirected graph has a cycle using BFS.
bool hasCycleUndirected(int n, const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    std::vector<bool> visited(n, false);
    // Queue stores pairs of (vertex, parent)
    std::queue<std::pair<int,int>> q;

    for (int start = 0; start < n; ++start) {
        if (visited[start]) continue;

        // Initialize BFS for this component
        visited[start] = true;
        q.push({start, -1}); // parent of root is -1

        while (!q.empty()) {
            auto [u, parent] = q.front();
            q.pop();

            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push({v, u});
                } else if (v != parent) {
                    // visited and not the parent => cycle
                    return true;
                }
            }
        }
    }

    return false;
}
// The standard BFS cycle detection for undirected graphs works by visiting each component and, when exploring neighbors, checking if a neighbor is already visited but is not the parent of the current node. Since the graph is undirected, we must avoid treating the immediate parent (the vertex we came from) as a cycle. The algorithm uses a BFS queue, storing each node along with its parent. For each adjacent vertex `v` of the current node `u`, if `v` is unvisited, mark it visited and enqueue it with parent `u`. If `v` is already visited and `v != parent[u]`, a cycle exists. For self-loops, the neighbor is the vertex itself, and since `v == u` and `u != parent[u]` (except when `u` is the root, where we set parent to `-1`), this will correctly return `true`. For parallel edges, when the second parallel edge is processed, the neighbor is already visited and is not the parent, so it returns `true`. The graph is traversed component by component; we iterate over all vertices, and for any unvisited vertex, we start a BFS. The time complexity is O(n + e) because each vertex and edge is processed once. The space complexity is O(n + e) for the adjacency list, visited array, and BFS queue.
