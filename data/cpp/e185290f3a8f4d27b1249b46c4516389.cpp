// Write a standalone C++ function `multiSourceDijkstra` that takes a graph represented by an adjacency list (as `std::vector<std::vector<std::pair<int, double>>>` where each entry is `(neighbor, weight)`), a list of source nodes (`std::vector<int>`), and a maximum distance cutoff (`double`). The function must return a `std::vector<double>` of length equal to the number of nodes (assume node IDs are from 0 to n-1), where each entry is the shortest distance from any source to that node, or `std::numeric_limits<double>::max()` if unreachable within the cutoff or if the node is isolated from all sources. If a node is a source, its distance is 0. The graph is undirected, weights are non-negative, and there may be multiple edges between the same pair. You must use a priority queue (min-heap) for Dijkstra-style propagation, and ensure nodes are not re-processed after finalization, but allow distance improvements (relaxations) as needed. Edge case: if the source list is empty, return a vector filled with `max()`. Handle self-loops (a node connected to itself) by ignoring them (they never improve distance). The function must be `const`-correct and not modify the input graph.

#include <cassert>
#include <cmath>
#include <vector>
#include <utility>
#include <limits>

// (The solution function is placed here for completeness in the test.)
std::vector<double> multiSourceDijkstra(
    const std::vector<std::vector<std::pair<int, double>>>& graph,
    const std::vector<int>& sources,
    double maxDistance) {
    const std::size_t n = graph.size();
    const double INF = std::numeric_limits<double>::max();
    std::vector<double> dist(n, INF);
    if (sources.empty()) return dist;
    using P = std::pair<double, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    for (int s : sources) {
        if (s >= 0 && s < static_cast<int>(n)) {
            if (dist[s] > 0.0) {
                dist[s] = 0.0;
                pq.emplace(0.0, s);
            }
        }
    }
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;
        for (const auto& [v, weight] : graph[u]) {
            if (u == v) continue;
            if (weight < 0.0) continue;
            if (weight >= maxDistance) continue;
            double newDist = d + weight;
            if (newDist > maxDistance) continue;
            if (newDist < dist[v]) {
                dist[v] = newDist;
                pq.emplace(newDist, v);
            }
        }
    }
    return dist;
}

int main() {
    // Simple linear graph: 0-1 (2.0), 1-2 (3.0), 2-3 (1.0)
    std::vector<std::vector<std::pair<int, double>>> g1(4);
    g1[0].push_back({1, 2.0});
    g1[1].push_back({0, 2.0});
    g1[1].push_back({2, 3.0});
    g1[2].push_back({1, 3.0});
    g1[2].push_back({3, 1.0});
    g1[3].push_back({2, 1.0});

    auto res1 = multiSourceDijkstra(g1, {0}, 100.0);
    assert(res1[0] == 0.0);
    assert(res1[1] == 2.0);
    assert(res1[2] == 5.0);
    assert(res1[3] == 6.0);

    // Multiple sources: sources 0 and 2, all nodes reachable
    auto res2 = multiSourceDijkstra(g1, {0, 2}, 100.0);
    assert(res2[0] == 0.0);
    assert(res2[1] == 2.0);
    assert(res2[2] == 0.0);
    assert(res2[3] == 1.0); // from source 2 via 2->3

    // Cutoff restricts reachability
    auto res3 = multiSourceDijkstra(g1, {0}, 4.0);
    assert(res3[0] == 0.0);
    assert(res3[1] == 2.0);
    // Node 2 requires distance 5.0 > 4.0, so remains INF
    assert(res3[2] == std::numeric_limits<double>::max());
    assert(res3[3] == std::numeric_limits<double>::max());

    // Empty sources
    auto res4 = multiSourceDijkstra(g1, {}, 100.0);
    assert(res4[0] == std::numeric_limits<double>::max());
    assert(res4[3] == std::numeric_limits<double>::max());

    // Self-loop and multiple edges: node 0 has self-loop weight 10, and parallel edge to 1 with weight 1 and weight 5
    std::vector<std::vector<std::pair<int, double>>> g2(2);
    g2[0].push_back({0, 10.0}); // self-loop ignored
    g2[0].push_back({1, 1.0});
    g2[0].push_back({1, 5.0});
    g2[1].push_back({0, 1.0});
    g2[1].push_back({0, 5.0});

    auto res5 = multiSourceDijkstra(g2, {0}, 100.0);
    assert(res5[0] == 0.0);
    assert(res5[1] == 1.0); // minimum weight edge taken

    // Large cutoff allows all, negative values not present
    auto res6 = multiSourceDijkstra(g2, {1}, 10.0);
    assert(res6[0] == 1.0);
    assert(res6[1] == 0.0);

    // Disconnected graph: node 3 is isolated
    std::vector<std::vector<std::pair<int, double>>> g3(4);
    g3[0].push_back({1, 2.0});
    g3[1].push_back({0, 2.0});
    // nodes 2 and 3 have no edges
    auto res7 = multiSourceDijkstra(g3, {0}, 100.0);
    assert(res7[0] == 0.0);
    assert(res7[1] == 2.0);
    assert(res7[2] == std::numeric_limits<double>::max());
    assert(res7[3] == std::numeric_limits<double>::max());

    return 0;
}

#include <vector>
#include <queue>
#include <limits>
#include <utility>
#include <cstddef>

// Multi-source Dijkstra with a maximum distance cutoff.
// Graph: adjacency list where graph[u] contains (v, weight).
// Sources: list of node IDs.
// Returns vector of shortest distances from any source, or max() if unreachable beyond cutoff.
std::vector<double> multiSourceDijkstra(
    const std::vector<std::vector<std::pair<int, double>>>& graph,
    const std::vector<int>& sources,
    double maxDistance) {

    const std::size_t n = graph.size();
    const double INF = std::numeric_limits<double>::max();
    std::vector<double> dist(n, INF);

    if (sources.empty()) {
        return dist; // all remain INF
    }

    // Min-heap: (distance, node)
    using P = std::pair<double, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;

    for (int s : sources) {
        if (s >= 0 && s < static_cast<int>(n)) {
            if (dist[s] > 0.0) {
                dist[s] = 0.0;
                pq.emplace(0.0, s);
            }
        }
    }

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // Skip stale entries
        if (d > dist[u]) {
            continue;
        }

        // Distances are final for u at this point (Dijkstra property)
        for (const auto& [v, weight] : graph[u]) {
            if (u == v) continue; // ignore self-loops
            if (weight < 0.0) continue; // defensive, though input should be non-negative
            if (weight >= maxDistance) continue; // edge cost alone too large

            double newDist = d + weight;
            // Prune if exceeding cutoff
            if (newDist > maxDistance) continue;
            if (newDist < dist[v]) {
                dist[v] = newDist;
                pq.emplace(newDist, v);
            }
        }
    }

    return dist;
}

// This is a multi-source shortest path problem on a weighted, undirected graph. The standard approach is a Dijkstra-like algorithm initialized with all sources in the priority queue with distance 0. We maintain a `dist` array initialized to `INF` (`std::numeric_limits<double>::max()`). For each source, set `dist[source]=0` and push `(0, source)` into a min-heap. Then while the heap is not empty, pop the smallest `(d, u)`. If `d > dist[u]`, skip (stale entry). Otherwise, mark `u` as finalized (optional but can speed up), and iterate over all edges `(v, w)` from `u`. If `w < 0` (should never happen for valid input, but we can guard) or `dist[u] + w >= cutoff` (we can use `>` or `>=`; the problem says "within the cutoff" so distances exactly equal to cutoff are allowed, so use `>` for pruning) then skip. If `dist[u] + w < dist[v]`, update `dist[v]` and push `(dist[v], v)`. Since we use a min-heap and all weights are non-negative, the first time we pop a node with a certain distance, that distance is final (classic Dijkstra correctness). However, because we allow multiple sources, the algorithm remains correct. After processing all reachable nodes, distances that remain `INF` are unreachable or beyond cutoff. If the source list is empty, we never push anything, so all `dist` remain `INF`, which matches the specification. Time complexity: `O((V+E) log V)` where `V` is number of nodes and `E` is total number of undirected edges (each undirected edge appears twice in adjacency list). Space: `O(V+E)` for adjacency list plus `O(V)` for dist and heap.
