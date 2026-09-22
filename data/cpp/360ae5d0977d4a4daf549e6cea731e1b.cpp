Given an undirected graph with `n` vertices and `m` edges, where each edge `(u, v)` has an integer weight `w` (the same weight applies in both directions), write a standalone C++ function `std::vector<long long> assignDistances(int n, const std::vector<std::tuple<int, int, long long>>& edges)` that assigns a `long long` value to every vertex such that for each edge `(u, v, w)`, the difference `dist[u] - dist[v]` equals `w` (and equivalently `dist[v] - dist[u]` equals `-w`). If multiple valid assignments exist, any is acceptable; the graph is guaranteed to be consistent (no contradiction). Vertices are 0-indexed. The function should return a vector of size `n` containing the assigned values. The graph may be disconnected and may contain self-loops or parallel edges (parallel edges with different weights would create a contradiction, so assume consistent input). The weights can be negative, zero, or large positive values.
#include <cassert>
#include <tuple>
#include <vector>

// The solution function is assumed to be declared above.

int main() {
    // Simple disconnected components
    {
        std::vector<std::tuple<int, int, long long>> edges = {
            {0, 1, 5}, {1, 2, -3}
        };
        auto d = assignDistances(4, edges);
        assert(d.size() == 4);
        // Check edge conditions
        assert(d[0] - d[1] == 5);
        assert(d[1] - d[2] == -3);
        // Vertex 3 is isolated, default 0
        assert(d[3] == 0);
    }

    // Negative and zero weights
    {
        std::vector<std::tuple<int, int, long long>> edges = {
            {0, 1, -4}, {1, 2, 0}, {2, 3, 7}
        };
        auto d = assignDistances(4, edges);
        assert(d[0] - d[1] == -4);
        assert(d[1] - d[2] == 0);
        assert(d[2] - d[3] == 7);
    }

    // Self-loop with zero weight (consistent)
    {
        std::vector<std::tuple<int, int, long long>> edges = {
            {0, 0, 0}, {0, 1, 2}
        };
        auto d = assignDistances(2, edges);
        assert(d[0] - d[0] == 0); // trivially true
        assert(d[0] - d[1] == 2);
    }

    // Larger component with multiple branches
    {
        std::vector<std::tuple<int, int, long long>> edges = {
            {0, 1, 3}, {1, 2, -1}, {0, 3, 8}, {3, 4, -6}, {2, 4, 10}
        };
        auto d = assignDistances(5, edges);
        assert(d[0] - d[1] == 3);
        assert(d[1] - d[2] == -1);
        assert(d[0] - d[3] == 8);
        assert(d[3] - d[4] == -6);
        assert(d[2] - d[4] == 10);
    }

    // Parallel edges with same weight (consistent)
    {
        std::vector<std::tuple<int, int, long long>> edges = {
            {0, 1, 2}, {1, 0, 2}
        };
        auto d = assignDistances(2, edges);
        assert(d[0] - d[1] == 2);
    }

    // Single vertex with no edges
    {
        auto d = assignDistances(1, {});
        assert(d.size() == 1);
        assert(d[0] == 0);
    }

    return 0;
}
#include <vector>
#include <queue>

// Assigns values to vertices such that for every edge (u,v,w), dist[u] - dist[v] == w.
// Uses BFS on each connected component, starting with 0 at an unvisited vertex.
std::vector<long long> assignDistances(int n, const std::vector<std::tuple<int, int, long long>>& edges) {
    // Build adjacency list: for edge (u,v,w), dist[v] = dist[u] - w, dist[u] = dist[v] + w
    std::vector<std::vector<std::pair<int, long long>>> adj(n);
    for (const auto& [u, v, w] : edges) {
        adj[u].emplace_back(v, -w); // dist[v] = dist[u] + (-w)
        adj[v].emplace_back(u,  w); // dist[u] = dist[v] + w
    }

    std::vector<long long> dist(n, 0);
    std::vector<bool> visited(n, false);

    for (int start = 0; start < n; ++start) {
        if (visited[start]) continue;
        visited[start] = true;
        dist[start] = 0; // arbitrary root value

        std::queue<int> q;
        q.push(start);
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            for (const auto& [nei, delta] : adj[cur]) {
                if (!visited[nei]) {
                    visited[nei] = true;
                    dist[nei] = dist[cur] + delta;
                    q.push(nei);
                }
            }
        }
    }
    return dist;
}
// The problem reduces to finding potential differences along each connected component. Since each edge gives a linear relation between two vertices' assigned values, we can pick an arbitrary root per connected component, set its distance to 0, and perform a DFS/BFS to propagate values: for an edge `(u,v,w)`, if `dist[u]` is already known, set `dist[v] = dist[u] - w` (because `dist[u] - dist[v] = w` → `dist[v] = dist[u] - w`). Equivalently, if we store edges as directed with `dist[v] = dist[u] - w` for one direction and `dist[u] = dist[v] + w` for the reverse, we can just traverse all edges. The provided snippet uses `g[u].emplace_back(v, w)` and `g[v].emplace_back(u, -w)` and then sets `dis[u] = dis[v] + w` from the perspective of moving from `v` to `u`. In our reference solution, we’ll build an adjacency list with edges `(neighbor, delta)` meaning `dist[neighbor] = dist[current] + delta`. For an input edge `(u,v,w)` satisfying `dist[u]-dist[v]=w`, we add to `u`’s list `(v, -w)` (since `dist[v] = dist[u] - w`), and to `v`’s list `(u, +w)` (since `dist[u] = dist[v] + w`). Then we BFS/DFS each unvisited vertex, setting its distance to 0, and propagate. Edge cases: disconnected components require iterating all vertices; self-loops (u==v) with w must satisfy `0=w` — if `w != 0` the input would be inconsistent (but problem guarantees consistency, so self-loop must have weight 0, and setting it is fine). Parallel edges with same weight are fine; inconsistent parallel edges cause contradiction but are not given. Time complexity is O(n+m) using adjacency list. Space complexity is O(n+m).
