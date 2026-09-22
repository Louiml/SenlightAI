// Write a C++ function `std::vector<int> shortestDistancesFromSource(const std::vector<std::vector<std::pair<int,int>>>& graph, int source)` that takes an adjacency list representing a **directed graph with non-negative edge weights** (where each pair is `{neighbor, weight}`) and a source vertex index, and returns a vector of shortest distances from the source to every vertex. If a vertex is unreachable, its distance should be `INT_MAX`. The graph may have up to 10,000 vertices and 100,000 edges, and weights are positive integers. The function must handle a graph with a single vertex (returning `{0}`), self-loops, parallel edges, and unreachable components. Use Dijkstra's algorithm with a priority queue. Do not modify the input graph; ensure the function is `const`-correct (takes the graph by const reference).
// The standard approach is **Dijkstra's algorithm** because all edge weights are non-negative, and we need shortest paths from a fixed source. Initialize a distance vector of size `graph.size()` with `INT_MAX`, set `dist[source] = 0`, and use a min-priority queue (simulated by storing negative distances) of pairs `{distance, vertex}`. Push the source. Then repeatedly extract the vertex with the smallest tentative distance; if the popped distance is greater than the current known distance, skip it (lazy deletion). For each neighbor `(v, weight)` of the current vertex `u`, compute `alt = dist[u] + weight`; if `alt < dist[v]`, update `dist[v]` and push `{ -alt, v }`. Continue until the queue is empty. Edge cases: If the source index is out of bounds, return an empty vector (or handle gracefully). Self-loops are naturally handled because `alt >= dist[u]` anyway, so they are ignored. Parallel edges are handled because the relaxation updates the best. Unreachable vertices remain `INT_MAX`. With a priority queue, each vertex may be pushed multiple times, but each edge is relaxed once per successful update, leading to `O((V + E) log V)` time if using a binary heap, and `O(V)` auxiliary space for the distance vector (plus the priority queue, which in the worst case can hold `O(E)` entries). The use of negative distances in the priority queue turns the max‑heap into a min‑heap.
#include <vector>
#include <queue>
#include <climits>
#include <utility>

// Returns the shortest distances from 'source' to every vertex using Dijkstra's algorithm.
// The graph is given as an adjacency list: graph[u] = { {v, weight}, ... }.
// Unreachable vertices get INT_MAX. Assumes non-negative edge weights.
std::vector<int> shortestDistancesFromSource(
    const std::vector<std::vector<std::pair<int, int>>>& graph,
    int source)
{
    const int n = static_cast<int>(graph.size());
    if (source < 0 || source >= n) {
        return {};  // Invalid source
    }

    std::vector<int> dist(n, INT_MAX);
    dist[source] = 0;

    // Min-heap simulated by storing negative distances.
    // Pair: { -distance, vertex }
    std::priority_queue<std::pair<int, int>> pq;
    pq.push({0, source});  // -0 = 0

    while (!pq.empty()) {
        int u = pq.top().second;
        int current_neg_dist = pq.top().first;
        pq.pop();

        int current_dist = -current_neg_dist;
        // Skip outdated entries (lazy deletion)
        if (current_dist > dist[u]) {
            continue;
        }

        for (const auto& [v, weight] : graph[u]) {
            int alt = dist[u] + weight;
            if (alt < dist[v]) {
                dist[v] = alt;
                pq.push({-alt, v});
            }
        }
    }

    return dist;
}
#include <cassert>
#include <vector>
#include <utility>
#include <climits>

// The solution function is declared above; include it here for the test.
// (Normally the declaration would be in a header; this is for standalone testing.)

int main() {
    // Test 1: Simple three-node path 0->1 (weight 2), 1->2 (weight 3)
    {
        std::vector<std::vector<std::pair<int,int>>> g(3);
        g[0].push_back({1, 2});
        g[1].push_back({2, 3});
        auto d = shortestDistancesFromSource(g, 0);
        assert(d.size() == 3);
        assert(d[0] == 0);
        assert(d[1] == 2);
        assert(d[2] == 5);
    }

    // Test 2: Single node
    {
        std::vector<std::vector<std::pair<int,int>>> g(1);
        auto d = shortestDistancesFromSource(g, 0);
        assert(d.size() == 1);
        assert(d[0] == 0);
    }

    // Test 3: Unreachable node
    {
        std::vector<std::vector<std::pair<int,int>>> g(3);
        g[0].push_back({1, 1});
        // Node 2 unreachable
        auto d = shortestDistancesFromSource(g, 0);
        assert(d[0] == 0);
        assert(d[1] == 1);
        assert(d[2] == INT_MAX);
    }

    // Test 4: Parallel edges and self-loop
    {
        std::vector<std::vector<std::pair<int,int>>> g(2);
        g[0].push_back({0, 10}); // self-loop, ignored
        g[0].push_back({1, 5});
        g[0].push_back({1, 3});  // parallel edge with smaller weight
        auto d = shortestDistancesFromSource(g, 0);
        assert(d[0] == 0);
        assert(d[1] == 3);
    }

    // Test 5: Cyclic graph with non-negative weights, no negative cycles (Dijkstra works)
    {
        std::vector<std::vector<std::pair<int,int>>> g(4);
        g[0].push_back({1, 1});
        g[1].push_back({2, 1});
        g[2].push_back({0, 1});
        g[2].push_back({3, 1});
        auto d = shortestDistancesFromSource(g, 0);
        assert(d[0] == 0);
        assert(d[1] == 1);
        assert(d[2] == 2);
        assert(d[3] == 3);
    }

    // Test 6: Multiple updates (source not zero)
    {
        std::vector<std::vector<std::pair<int,int>>> g(4);
        g[0].push_back({1, 2});
        g[1].push_back({2, 3});
        g[2].push_back({3, 1});
        auto d = shortestDistancesFromSource(g, 1);
        assert(d[0] == INT_MAX); // unreachable from 1
        assert(d[1] == 0);
        assert(d[2] == 3);
        assert(d[3] == 4);
    }

    // Test 7: Invalid source returns empty
    {
        std::vector<std::vector<std::pair<int,int>>> g(2);
        auto d = shortestDistancesFromSource(g, 5);
        assert(d.empty());
    }

    // Test 8: Larger chain with heavy updates
    {
        std::vector<std::vector<std::pair<int,int>>> g(4);
        g[0].push_back({1, 100});
        g[0].push_back({2, 1});
        g[2].push_back({1, 1});
        g[1].push_back({3, 1});
        auto d = shortestDistancesFromSource(g, 0);
        assert(d[0] == 0);
        assert(d[1] == 2); // via 2
        assert(d[2] == 1);
        assert(d[3] == 3);
    }

    // Test 9: Zero-weight edges (allowed since non-negative)
    {
        std::vector<std::vector<std::pair<int,int>>> g(3);
        g[0].push_back({1, 0});
        g[1].push_back({2, 0});
        auto d = shortestDistancesFromSource(g, 0);
        assert(d[0] == 0);
        assert(d[1] == 0);
        assert(d[2] == 0);
    }

    // Test 10: Large weights not overflowing int (using max int)
    {
        std::vector<std::vector<std::pair<int,int>>> g(2);
        g[0].push_back({1, INT_MAX - 1});
        auto d = shortestDistancesFromSource(g, 0);
        assert(d[0] == 0);
        assert(d[1] == INT_MAX - 1);
    }

    return 0;
}
