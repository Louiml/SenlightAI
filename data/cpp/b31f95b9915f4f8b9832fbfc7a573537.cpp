// Given an undirected weighted graph with `n` vertices (numbered 1 to `n`) and `m` edges, write a C++ function `long long countShortestPaths(int n, const std::vector<std::tuple<int,int,int>>& edges)` that returns the number of shortest paths from vertex 1 to vertex 2. The graph is connected, edge weights are positive, and no two vertices are connected by more than one edge. The result may be large, so return it as `long long`. Note: the original code's logic computes the number of ways to reach each vertex via strictly increasing shortest distances from a starting vertex (2), but in our task, we count shortest paths from vertex 1 to vertex 2. Also, the original code incorrectly counts paths by adding counts along edges where the neighbor's distance is greater than current, but for counting shortest paths correctly, we must only relax edges that lie on a shortest path from the source. We'll follow the classic Dijkstra plus counting approach: first compute shortest distances from vertex 1 using Dijkstra, then perform a topological order of vertices sorted by distance and accumulate counts. The function should return the number of distinct shortest paths from 1 to 2. Assume `n >= 2`, `m >= 1`, and all edge weights are positive integers.
The task is to count the number of shortest paths from a fixed source (vertex 1) to a fixed target (vertex 2) in an undirected weighted graph with positive weights. The correct approach is: (1) Run Dijkstra's algorithm from vertex 1 to compute the shortest distance `dist[v]` for every vertex. (2) Then, we count the number of paths. To do this, we iterate vertices in non-decreasing order of `dist` (since all edge weights are positive, this is a valid topological order for the shortest-path DAG). For each vertex `u` in that order, for each neighbor `v` such that `dist[u] + weight(u,v) == dist[v]`, we add `ways[u]` to `ways[v]`. Initialize `ways[1] = 1`. Finally, return `ways[2]`. Edge cases: the source and target may be directly connected; there may be multiple shortest paths due to equal-weight alternatives; the answer can be large, hence `long long` is used. Complexity: Dijkstra with a priority queue takes O((n+m) log n) time; the counting phase takes O(n + m) time. Space is O(n + m). We also need to handle the graph input as a vector of tuples; convert to adjacency lists for efficiency.
#include <vector>
#include <queue>
#include <tuple>
#include <limits>
#include <algorithm>

// Count the number of shortest paths from vertex 1 to vertex 2
// in an undirected weighted graph with n vertices (1..n) and edges as tuples (u, v, weight).
long long countShortestPaths(int n, const std::vector<std::tuple<int,int,int>>& edges) {
    // Build adjacency list: for each vertex, list of (neighbor, weight)
    std::vector<std::vector<std::pair<int,int>>> adj(n + 1);
    for (const auto& [u, v, w] : edges) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    const long long INF = std::numeric_limits<long long>::max() / 4;

    // Dijkstra from vertex 1
    std::vector<long long> dist(n + 1, INF);
    dist[1] = 0;
    std::priority_queue<std::pair<long long,int>, std::vector<std::pair<long long,int>>, std::greater<>> pq;
    pq.push({0, 1});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue; // stale entry
        for (const auto& [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    // If target unreachable (should not happen per problem statement), return 0
    if (dist[2] == INF) return 0;

    // Order vertices by distance (ascending)
    std::vector<int> order(n);
    for (int i = 0; i < n; ++i) order[i] = i + 1;
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        if (dist[a] != dist[b]) return dist[a] < dist[b];
        return a < b;
    });

    // Count ways in topological order of shortest-path DAG
    std::vector<long long> ways(n + 1, 0);
    ways[1] = 1;
    for (int u : order) {
        if (ways[u] == 0) continue; // not reachable or no paths? Actually 1 has ways>0
        for (const auto& [v, w] : adj[u]) {
            if (dist[u] + w == dist[v]) {
                ways[v] += ways[u];
            }
        }
    }

    return ways[2];
}
#include <cassert>
#include <vector>
#include <tuple>

// Include the solution function here (or via header)

int main() {
    // Simple case: direct edge
    {
        int n = 2;
        std::vector<std::tuple<int,int,int>> edges = {{1,2,5}};
        assert(countShortestPaths(n, edges) == 1);
    }
    // Two equal shortest paths via different intermediates
    {
        int n = 4;
        std::vector<std::tuple<int,int,int>> edges = {
            {1,2,1}, {1,3,1}, {2,4,1}, {3,4,1}
        };
        assert(countShortestPaths(n, edges) == 2);
    }
    // Triangle: path 1-2 direct vs 1-3-2 with equal cost
    {
        int n = 3;
        std::vector<std::tuple<int,int,int>> edges = {
            {1,2,2}, {1,3,1}, {3,2,1}
        };
        assert(countShortestPaths(n, edges) == 2);
    }
    // Longer graph with multiple shortest paths
    {
        int n = 5;
        std::vector<std::tuple<int,int,int>> edges = {
            {1,2,1}, {2,3,1}, {3,4,1}, {4,5,1}, // path 1-2-3-4-5 cost 4
            {1,3,2}, {3,5,2},                   // path 1-3-5 cost 4
            {2,4,2}                             // alternative? gives cost 3? no
        };
        // Let's compute: edges: 1-2(1),2-3(1),3-4(1),4-5(1) => 1 to 5 cost 4
        // 1-3(2),3-5(2) => cost 4, so that's another path
        // Total shortest paths from 1 to 5? Also 1-2-3-5? 1-2(1)+2-3(1)+3-5(2)=4, that's another.
        // Also 1-2-4-5? 1-2(1)+2-4(2)+4-5(1)=4, another.
        // Let's count: 
        // Paths cost 4: 
        // 1-2-3-4-5
        // 1-3-5
        // 1-2-3-5
        // 1-2-4-5
        // Also 1-3-2-4-5? cost 1+1+2+1=5 no.
        assert(countShortestPaths(n, edges) == 4);
    }
    // No path (ensure 0 if unreachable, though problem says connected)
    {
        int n = 3;
        std::vector<std::tuple<int,int,int>> edges = {{1,3,1}};
        assert(countShortestPaths(n, edges) == 0);
    }
    return 0;
}
