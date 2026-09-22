// Implement a C++ function that simulates a simple public transportation network. The network has `n` stations (numbered 1 to n) and `m` directed connections between them, where each connection has a positive integer travel time. Given a starting station `src` and a destination station `dst`, your function must return the shortest possible travel time from `src` to `dst`, along with the sequence of stations visited (including start and end) that achieves this minimum. If no path exists, return a pair where the travel time is set to a sentinel value like `-1` and the path is empty. The graph is directed (edges go one way), may contain cycles, and edge weights are positive integers. Input will be provided as: first an integer `n`, then an integer `m`, then `m` lines each with three integers `u, v, w` meaning a directed edge from station `u` to station `v` with travel time `w`. Finally, two integers `src` and `dst` are given. The function signature should be `std::pair<int, std::vector<int>> shortestRoute(int n, const std::vector<std::tuple<int,int,int>>& edges, int src, int dst)`. All stations are 1-indexed in the input, but you may convert internally to 0-indexed.
// The problem is the classic single-source shortest path in a directed weighted graph with non-negative weights, which is optimally solved using Dijkstra's algorithm. We convert the 1-indexed station numbers to 0-indexed internally. Build an adjacency list mapping each station to pairs `(neighbor, weight)`. Initialize a distance array with a large sentinel (like `INF = 1e9`) and set distance to `src` to 0. Use a `priority_queue` of `(distance, node)` pairs, ordered by distance (min-heap). While the priority queue is not empty, pop the nearest unprocessed node. If the popped node's distance is outdated (greater than the stored distance), skip it. Otherwise, for each outgoing edge to neighbor `v` with weight `w`, if `dist[current] + w < dist[v]`, update `dist[v]` and record the predecessor `prev[v] = current`, then push `(dist[v], v)` into the queue. After the algorithm finishes, if `dist[dst]` is still `INF`, no path exists, return `{-1, {}}`. Otherwise, reconstruct the path by backtracking using `prev` from `dst` to `src`, then reverse the sequence and convert back to 1-indexed. Edge cases: `src == dst` → distance is 0, path contains just that station. Multiple edges or self-loops do not break the algorithm since Dijkstra handles them naturally. The time complexity is `O((n + m) log n)` because each edge is relaxed at most once and each heap operation costs `O(log n)`. Space complexity is `O(n + m)` for the adjacency list, distance, predecessor, and the priority queue.
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <limits>

// Return the shortest travel time from src to dst and the path (1-indexed).
// If no path, returns {-1, {}}.
std::pair<int, std::vector<int>> shortestRoute(
    int n,
    const std::vector<std::tuple<int,int,int>>& edges,
    int src,
    int dst)
{
    const int INF = std::numeric_limits<int>::max();

    // Build adjacency list (0-indexed).
    std::vector<std::vector<std::pair<int,int>>> adj(n);
    for (const auto& [u, v, w] : edges) {
        // Convert from 1-indexed to 0-indexed.
        adj[u - 1].emplace_back(v - 1, w);
    }

    int src0 = src - 1;
    int dst0 = dst - 1;

    // Dijkstra's algorithm.
    std::vector<int> dist(n, INF);
    std::vector<int> prev(n, -1);
    dist[src0] = 0;

    using P = std::pair<int,int>; // (distance, node)
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.emplace(0, src0);

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue; // outdated entry
        if (u == dst0) break; // early exit
        for (const auto& [v, w] : adj[u]) {
            int nd = d + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                prev[v] = u;
                pq.emplace(nd, v);
            }
        }
    }

    if (dist[dst0] == INF) {
        return {-1, {}};
    }

    // Reconstruct path.
    std::vector<int> path;
    for (int cur = dst0; cur != -1; cur = prev[cur]) {
        path.push_back(cur + 1); // convert back to 1-indexed
    }
    std::reverse(path.begin(), path.end());

    return {dist[dst0], path};
}
#include <cassert>
#include <vector>
#include <tuple>
#include <iostream>

// Include the solution function here.

int main() {
    // Test 1: simple path 1 -> 2 -> 4 with weights 3 and 2, and direct 1 -> 4 weight 10.
    {
        std::vector<std::tuple<int,int,int>> edges = {
            {1,2,3}, {2,4,2}, {1,4,10}
        };
        auto [dist, path] = shortestRoute(4, edges, 1, 4);
        assert(dist == 5);
        assert(path == std::vector<int>({1,2,4}));
    }

    // Test 2: start equals destination.
    {
        std::vector<std::tuple<int,int,int>> edges = {
            {1,2,1}
        };
        auto [dist, path] = shortestRoute(2, edges, 1, 1);
        assert(dist == 0);
        assert(path == std::vector<int>({1}));
    }

    // Test 3: no path.
    {
        std::vector<std::tuple<int,int,int>> edges = {
            {1,2,5}
        };
        auto [dist, path] = shortestRoute(3, edges, 1, 3);
        assert(dist == -1);
        assert(path.empty());
    }

    // Test 4: multiple paths, cycles, and self-loop.
    {
        std::vector<std::tuple<int,int,int>> edges = {
            {1,2,1}, {2,3,1}, {3,2,1}, {1,3,10}, {2,2,5}, {3,4,1}
        };
        auto [dist, path] = shortestRoute(4, edges, 1, 4);
        assert(dist == 3);
        assert(path == std::vector<int>({1,2,3,4}));
    }

    // Test 5: negative? Not allowed, but large weights and many nodes.
    {
        std::vector<std::tuple<int,int,int>> edges;
        int n = 1000;
        for (int i = 1; i < n; ++i) {
            edges.emplace_back(i, i+1, 1000000);
        }
        auto [dist, path] = shortestRoute(n, edges, 1, n);
        assert(dist == 999000000);
        assert(path.size() == n);
        assert(path.front() == 1);
        assert(path.back() == n);
    }

    // Test 6: graph with disconnected but alternative route.
    {
        std::vector<std::tuple<int,int,int>> edges = {
            {1,2,4}, {2,3,3}, {1,3,8}, {3,5,1}, {5,4,2}, {1,4,100}
        };
        auto [dist, path] = shortestRoute(5, edges, 1, 4);
        assert(dist == 14);
        assert(path == std::vector<int>({1,3,5,4}));
    }

    // Test 7: self-loop only and no way out.
    {
        std::vector<std::tuple<int,int,int>> edges = {
            {1,1,2}
        };
        auto [dist, path] = shortestRoute(2, edges, 1, 2);
        assert(dist == -1);
        assert(path.empty());
    }

    std::cout << "All tests passed.\n";
    return 0;
}
