Write a C++ function `int networkDelayTime(std::vector<std::vector<int>>& times, int n, int k)` that simulates a network of `n` nodes numbered from 1 to `n`. The vector `times` contains directed edges in the form `[u, v, w]`, meaning a signal sent from node `u` reaches node `v` in `w` milliseconds. A signal is sent from node `k`. The function must return the minimum time required for the signal to reach all `n` nodes, or `-1` if it is impossible for the signal to reach at least one node. Assume `1 <= k <= n`, edge weights are non-negative integers, nodes are 1-indexed, and there may be multiple edges or self-loops. The graph may be disconnected.
The problem is the classic single-source shortest path on a weighted directed graph, best solved with Dijkstra's algorithm since all edge weights are non-negative. Construct an adjacency list `G` where `G[u]` stores pairs `(v, w)` for each edge. Use a min-heap priority queue of `(current_distance, node)` pairs, initialized with `(0, k)` and a distance array `dist` initialized to a large value (e.g., `INT_MAX`) with `dist[k] = 0`. Pop the node with the smallest distance; if the popped distance is stale (greater than `dist[u]`), skip. For each neighbor `v`, if `dist[u] + w < dist[v]`, update `dist[v]` and push the new distance into the heap. After processing, check all nodes from 1 to `n`; if any `dist[i]` remains `INT_MAX`, return `-1`. Otherwise, the answer is the maximum value in `dist`. Edge cases: `n == 1` returns `0` immediately; multiple edges are handled naturally because Dijkstra updates only when strictly better; self-loops are ignored because they don't improve distance (equal weight never improves). Time complexity is `O((V+E) log V)` where `V = n` and `E = times.size()` due to heap operations. Space complexity is `O(V + E)` for the adjacency list and distance array, plus heap size up to `O(E)`.
#include <vector>
#include <queue>
#include <climits>
#include <utility>

// Returns the minimum time for a signal to reach all nodes from source k,
// or -1 if some node is unreachable.
int networkDelayTime(std::vector<std::vector<int>>& times, int n, int k) {
    // Build adjacency list: node -> list of (neighbor, weight)
    std::vector<std::vector<std::pair<int, int>>> graph(n + 1);
    for (const auto& edge : times) {
        graph[edge[0]].emplace_back(edge[1], edge[2]);
    }

    // Min-heap of (distance, node)
    using P = std::pair<int, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;

    std::vector<int> dist(n + 1, INT_MAX);
    dist[k] = 0;
    pq.push({0, k});

    while (!pq.empty()) {
        auto [current_dist, u] = pq.top();
        pq.pop();

        // Skip stale entries
        if (current_dist > dist[u]) continue;

        for (const auto& [v, weight] : graph[u]) {
            if (current_dist + weight < dist[v]) {
                dist[v] = current_dist + weight;
                pq.push({dist[v], v});
            }
        }
    }

    int max_time = 0;
    for (int i = 1; i <= n; ++i) {
        if (dist[i] == INT_MAX) return -1;
        max_time = std::max(max_time, dist[i]);
    }
    return max_time;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case
    std::vector<std::vector<int>> times1 = {{2,1,1},{2,3,1},{3,4,1}};
    assert(networkDelayTime(times1, 4, 2) == 2);

    // Unreachable node
    std::vector<std::vector<int>> times2 = {{1,2,1}};
    assert(networkDelayTime(times2, 3, 1) == -1);

    // Single node
    std::vector<std::vector<int>> times3 = {};
    assert(networkDelayTime(times3, 1, 1) == 0);

    // Multiple edges with different weights
    std::vector<std::vector<int>> times4 = {{1,2,5},{1,2,3},{2,3,2},{1,3,10}};
    assert(networkDelayTime(times4, 3, 1) == 5);

    // Self-loop ignored
    std::vector<std::vector<int>> times5 = {{1,1,1},{1,2,2}};
    assert(networkDelayTime(times5, 2, 1) == 2);

    // Disconnected graph
    std::vector<std::vector<int>> times6 = {{1,2,1},{3,4,1}};
    assert(networkDelayTime(times6, 4, 1) == -1);

    // Larger chain
    std::vector<std::vector<int>> times7 = {{1,2,1},{2,3,1},{3,4,1},{4,5,1}};
    assert(networkDelayTime(times7, 5, 1) == 4);

    // Zero-weight edges
    std::vector<std::vector<int>> times8 = {{1,2,0},{2,3,0}};
    assert(networkDelayTime(times8, 3, 1) == 0);

    // Cycle with negative? No, but ensure non-negative works
    std::vector<std::vector<int>> times9 = {{1,2,1},{2,1,1},{2,3,1}};
    assert(networkDelayTime(times9, 3, 1) == 2);

    return 0;
}
