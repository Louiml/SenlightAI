Write a C++ function `vector<int> shortestPathThroughGivenEdge(int n, vector<tuple<int,int,int>>& edges, int start, int target1, int target2, vector<int>& destinations)` that, given an undirected weighted graph with `n` vertices (numbered 1 to n), returns the list of destination nodes (from the given `destinations` vector) for which there exists a shortest path from `start` to that destination that passes through the edge between `target1` and `target2` (that edge is guaranteed to exist and is unique between those two vertices). The returned list must be sorted in ascending order. If no such destination exists, return an empty vector. Edge weights are positive integers.
// The solution uses Dijkstra's algorithm multiple times. First, run Dijkstra from `start` to compute the shortest distances from the source to all nodes. Then, run Dijkstra from `target1` and from `target2` separately. For each destination `v`, a shortest path from `start` to `v` that passes through the given edge exists if either `dist_start[target1] + edge_weight + dist_target2[v] == dist_start[v]` or `dist_start[target2] + edge_weight + dist_target1[v] == dist_start[v]`. The first condition corresponds to going from start to target1, crossing the edge, then going to v from target2; the second condition is symmetric. If the equality holds, then the path is indeed a shortest path because the total cost equals the known shortest distance from start to v. If multiple shortest paths exist but only some use the given edge, we still include the destination because the problem asks for "a shortest path" that uses the edge, not necessarily that all shortest paths use it. Edge cases: if the graph is disconnected, some distances remain infinity; the equality will never hold. If the destination is the start itself, the trivial path of length 0 does not use the edge, so we must ensure the equality condition using the edge is strictly checked; for a destination equal to start, the condition will typically yield a larger distance than 0, so it won't be included unless the edge has weight 0 (which isn't allowed). Time complexity: three Dijkstra runs, each O((n+m) log n) with priority queue, so overall O((n+m) log n) per test, plus O(t) for checking destinations. Space complexity: O(n+m) for adjacency list and distance arrays.
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <limits>

// Returns destinations reachable via a shortest path that includes the edge target1-target2.
std::vector<int> shortestPathThroughGivenEdge(
    int n,
    const std::vector<std::tuple<int,int,int>>& edges,
    int start,
    int target1,
    int target2,
    const std::vector<int>& destinations) {
    
    // Build adjacency list: node -> (neighbor, weight)
    std::vector<std::vector<std::pair<int,int>>> graph(n + 1);
    int edgeWeight = 0;
    for (const auto& [a, b, w] : edges) {
        graph[a].emplace_back(b, w);
        graph[b].emplace_back(a, w);
        if ((a == target1 && b == target2) || (a == target2 && b == target1)) {
            edgeWeight = w;
        }
    }
    
    const long long INF = std::numeric_limits<long long>::max() / 2;
    
    // Dijkstra from a given source, returns distance vector
    auto dijkstra = [&](int src) {
        std::vector<long long> dist(n + 1, INF);
        dist[src] = 0;
        using P = std::pair<long long, int>;
        std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
        pq.emplace(0, src);
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d != dist[u]) continue;
            for (const auto& [v, w] : graph[u]) {
                if (dist[v] > d + w) {
                    dist[v] = d + w;
                    pq.emplace(dist[v], v);
                }
            }
        }
        return dist;
    };
    
    auto dist_start = dijkstra(start);
    auto dist_t1 = dijkstra(target1);
    auto dist_t2 = dijkstra(target2);
    
    std::vector<int> result;
    for (int v : destinations) {
        // Path via target1 -> target2 -> v
        if (dist_start[target1] + edgeWeight + dist_t2[v] == dist_start[v]) {
            result.push_back(v);
        } else if (dist_start[target2] + edgeWeight + dist_t1[v] == dist_start[v]) {
            result.push_back(v);
        }
    }
    
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}
#include <cassert>
#include <vector>
#include <tuple>

int main() {
    // Test 1: Simple graph where edge g-h is on the shortest path
    int n = 4;
    std::vector<std::tuple<int,int,int>> edges = {
        {1, 2, 1}, {2, 3, 2}, {3, 4, 1}, {1, 3, 5}
    };
    int start = 1, t1 = 2, t2 = 3;
    std::vector<int> dests = {4};
    std::vector<int> expected = {4};
    assert(shortestPathThroughGivenEdge(n, edges, start, t1, t2, dests) == expected);

    // Test 2: Edge not on the shortest path
    edges = {
        {1, 2, 1}, {2, 3, 1}, {1, 4, 1}, {4, 3, 1}
    };
    n = 4;
    start = 1; t1 = 1; t2 = 4;
    dests = {3};
    expected = {};
    assert(shortestPathThroughGivenEdge(n, edges, start, t1, t2, dests) == expected);

    // Test 3: Multiple destinations, some filtered
    n = 5;
    edges = {
        {1, 2, 1}, {2, 5, 1}, {5, 4, 1},
        {1, 3, 1}, {3, 4, 1}
    };
    start = 1; t1 = 2; t2 = 5;
    dests = {4, 5, 3};
    expected = {4, 5};  // 3 is reachable only via 1-3-4, not via edge 2-5
    assert(shortestPathThroughGivenEdge(n, edges, start, t1, t2, dests) == expected);

    // Test 4: Destination is start itself, edge not used
    n = 3;
    edges = {{1, 2, 1}, {2, 3, 1}};
    start = 1; t1 = 1; t2 = 2;
    dests = {1};
    expected = {};
    assert(shortestPathThroughGivenEdge(n, edges, start, t1, t2, dests) == expected);

    // Test 5: Edge is the only way, no alternate path
    n = 3;
    edges = {{1, 2, 5}, {2, 3, 5}};
    start = 1; t1 = 1; t2 = 2;
    dests = {3};
    expected = {3};
    assert(shortestPathThroughGivenEdge(n, edges, start, t1, t2, dests) == expected);

    // Test 6: Graph with equal shortest paths, one via edge, one not
    n = 4;
    edges = {
        {1, 2, 1}, {2, 4, 1},  // path via edge
        {1, 3, 1}, {3, 4, 1}   // alternate path
    };
    start = 1; t1 = 1; t2 = 2;
    dests = {4};
    expected = {4};  // Because at least one shortest path uses the edge
    assert(shortestPathThroughGivenEdge(n, edges, start, t1, t2, dests) == expected);

    // Test 7: Disconnected graph
    n = 5;
    edges = {{1, 2, 1}};
    start = 1; t1 = 1; t2 = 2;
    dests = {3};
    expected = {};
    assert(shortestPathThroughGivenEdge(n, edges, start, t1, t2, dests) == expected);

    // Test 8: Duplicate destinations in input
    n = 3;
    edges = {{1, 2, 1}, {2, 3, 1}};
    start = 1; t1 = 1; t2 = 2;
    dests = {3, 3};
    expected = {3};
    assert(shortestPathThroughGivenEdge(n, edges, start, t1, t2, dests) == expected);

    // Test 9: Large weights
    n = 3;
    edges = {{1, 2, 1000000000}, {2, 3, 1000000000}};
    start = 1; t1 = 1; t2 = 2;
    dests = {3};
    expected = {3};
    assert(shortestPathThroughGivenEdge(n, edges, start, t1, t2, dests) == expected);

    // Test 10: Directed-like but undirected symmetric
    n = 4;
    edges = {{1, 2, 3}, {2, 4, 4}, {1, 3, 2}, {3, 4, 1}};
    start = 1; t1 = 2; t2 = 4;
    dests = {4, 3};
    // Shortest to 4 is 1-3-4 (cost 3), not using edge 2-4
    // Shortest to 3 is 1-3 (cost 2), not using edge
    expected = {};
    assert(shortestPathThroughGivenEdge(n, edges, start, t1, t2, dests) == expected);

    return 0;
}
