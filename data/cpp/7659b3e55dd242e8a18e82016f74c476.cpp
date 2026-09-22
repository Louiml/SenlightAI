Given an undirected graph with up to 1000 vertices and multiple edges, where each edge has both a distance and a cost, write a C++ function `pair<int,int> shortestPathWithMinCost(int n, int s, int t, const vector<tuple<int,int,int,int>>& edges)` that returns the shortest distance from `s` to `t`, and among all paths with that exact shortest distance, the minimum total cost. The graph may contain parallel edges (multiple edges between the same pair of vertices) and self-loops. If there is no path, return `{ -1, -1 }`. Distances and costs are positive integers. The function should be efficient for dense graphs up to 10000 edges and handle multiple test cases via repeated calls.
The problem is a variation of shortest path where the primary objective is distance, and the secondary objective is cost. This is solved using Dijkstra's algorithm with a customized priority queue that orders states first by total distance, then by total cost. We maintain two arrays: `dist[]` for the shortest distance to each node and `cost[]` for the minimum total cost among all paths that achieve that shortest distance. When relaxing an edge `(u,v)` from a state `(u, du, cu)`:
- If `du + edge.dis < dist[v]`, update both `dist[v]` and `cost[v]` to the new values.
- If `du + edge.dis == dist[v]` but `cu + edge.cost < cost[v]`, update `cost[v]` only.
We push new states into the priority queue whenever either update occurs. The initial state is `(s,0,0)`. Important edge cases: self-loops should not cause infinite loops (they are simply skipped because they never improve distance); parallel edges are handled naturally because each is considered separately. If `t` is unreachable, `dist[t]` remains at `INF`, so we return `{ -1, -1 }`. The priority queue must be a min-heap; using `std::priority_queue` with a custom comparator or using `std::greater` with a pair works. Time complexity is `O((V + E) log V)` due to Dijkstra, with `E` being the total number of edges (including parallels). Space complexity is `O(V + E)`.
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <utility>

// Returns {shortest_distance, minimum_cost} from s to t.
// If t is unreachable, returns {-1, -1}.
std::pair<int,int> shortestPathWithMinCost(
    int n, int s, int t,
    const std::vector<std::tuple<int,int,int,int>>& edges) {

    // Build adjacency list: for each vertex, list of (neighbor, distance, cost)
    std::vector<std::vector<std::tuple<int,int,int>>> adj(n + 1);
    for (const auto& [u, v, d, c] : edges) {
        adj[u].push_back({v, d, c});
        adj[v].push_back({u, d, c});
    }

    const int INF = INT_MAX;
    std::vector<int> dist(n + 1, INF);
    std::vector<int> cost(n + 1, INF);
    dist[s] = 0;
    cost[s] = 0;

    // Priority queue: (distance, cost, vertex)
    using State = std::tuple<int,int,int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    pq.push({0, 0, s});

    while (!pq.empty()) {
        auto [d_u, c_u, u] = pq.top();
        pq.pop();

        if (d_u > dist[u] || (d_u == dist[u] && c_u > cost[u])) continue;

        for (const auto& [v, w, c] : adj[u]) {
            int nd = d_u + w;
            int nc = c_u + c;
            if (nd < dist[v] || (nd == dist[v] && nc < cost[v])) {
                dist[v] = nd;
                cost[v] = nc;
                pq.push({nd, nc, v});
            }
        }
    }

    if (dist[t] == INF) return {-1, -1};
    return {dist[t], cost[t]};
}
#include <cassert>
#include <vector>
#include <tuple>
#include <utility>

// Declaration from solution
std::pair<int,int> shortestPathWithMinCost(
    int n, int s, int t,
    const std::vector<std::tuple<int,int,int,int>>& edges);

int main() {
    // Test 1: Simple graph
    {
        std::vector<std::tuple<int,int,int,int>> edges = {
            {1,2,1,5}, {2,3,2,1}, {1,3,4,1}
        };
        auto res = shortestPathWithMinCost(3, 1, 3, edges);
        assert(res == std::make_pair(3, 6)); // 1->2 (1,5) + 2->3 (2,1) = (3,6)
    }

    // Test 2: Parallel edges with different costs but same distance
    {
        std::vector<std::tuple<int,int,int,int>> edges = {
            {1,2,2,10}, {1,2,2,1}, {2,3,2,1}
        };
        auto res = shortestPathWithMinCost(3, 1, 3, edges);
        assert(res == std::make_pair(4, 2)); // Use edge with cost 1
    }

    // Test 3: Unreachable node
    {
        std::vector<std::tuple<int,int,int,int>> edges = {
            {1,2,1,1}
        };
        auto res = shortestPathWithMinCost(3, 1, 3, edges);
        assert(res == std::make_pair(-1, -1));
    }

    // Test 4: Self-loop and same start/end
    {
        std::vector<std::tuple<int,int,int,int>> edges = {
            {1,1,5,100}, {1,2,1,2}, {2,1,1,2}
        };
        auto res = shortestPathWithMinCost(2, 1, 1, edges);
        assert(res == std::make_pair(0, 0));
    }

    // Test 5: Longer path with lower cost tie-breaking
    {
        std::vector<std::tuple<int,int,int,int>> edges = {
            {1,2,2,10}, {2,4,2,10}, {1,3,2,1}, {3,4,2,1}, {1,4,5,1}
        };
        auto res = shortestPathWithMinCost(4, 1, 4, edges);
        // Shortest distance is 4 via 1-3-4 (cost 2) vs 1-2-4 (cost 20) vs direct (5)
        assert(res == std::make_pair(4, 2));
    }

    // Test 6: Multiple test cases - reuse function
    {
        std::vector<std::tuple<int,int,int,int>> edges1 = {{1,2,10,5}};
        auto res1 = shortestPathWithMinCost(2, 1, 2, edges1);
        assert(res1 == std::make_pair(10, 5));

        std::vector<std::tuple<int,int,int,int>> edges2 = {};
        auto res2 = shortestPathWithMinCost(1, 1, 1, edges2);
        assert(res2 == std::make_pair(0, 0)); // same node, no edges
    }

    // Test 7: Large graph with many edges
    {
        int n = 100;
        std::vector<std::tuple<int,int,int,int>> edges;
        for (int i = 1; i < n; ++i) {
            edges.push_back({i, i+1, 1, 1});
            edges.push_back({i+1, i, 2, 2}); // parallel edge, worse
        }
        auto res = shortestPathWithMinCost(n, 1, n, edges);
        // Shortest path is along the chain with distance 99, cost 99
        assert(res == std::make_pair(99, 99));
    }

    return 0;
}
