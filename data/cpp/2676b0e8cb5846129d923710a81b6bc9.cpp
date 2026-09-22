/*
Write a C++ function `std::vector<long long> dijkstraShortestPaths(int source, const std::vector<std::vector<std::pair<int, long long>>>& graph)` that computes the shortest distances from a given source vertex to every other vertex in a weighted, directed, and possibly disconnected graph. The graph is provided as an adjacency list: `graph[i]` is a vector of `{neighbor, weight}` pairs, where the weight is a non-negative `long long`. The function must return a `std::vector<long long>` of size `graph.size()`, where the entry at index `i` is the shortest distance from `source` to `i`, or `-1` if vertex `i` is unreachable. The graph may contain up to 10^5 vertices and 10^6 edges, and edge weights will fit in a `long long`. The function should handle self-loops, parallel edges (multiple edges between the same pair), and zero-weight edges. You may assume the graph is not empty.
*/

#include <vector>
#include <queue>
#include <limits>

// Computes the shortest distances from a source vertex to all others in a
// directed, non-negative-weight graph. Returns -1 for unreachable vertices.
std::vector<long long> dijkstraShortestPaths(
    int source,
    const std::vector<std::vector<std::pair<int, long long>>>& graph) {
    
    const int n = static_cast<int>(graph.size());
    const long long INF = std::numeric_limits<long long>::max();
    std::vector<long long> dist(n, INF);
    dist[source] = 0;
    
    // Min-heap: {distance, vertex}
    using P = std::pair<long long, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0, source});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (d > dist[u]) continue;  // Stale entry
        
        for (const auto& [v, w] : graph[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    
    // Convert unreachable distances to -1
    for (int i = 0; i < n; ++i) {
        if (dist[i] == INF) {
            dist[i] = -1;
        }
    }
    return dist;
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: Simple graph from the prompt (0-based, 5 vertices)
    std::vector<std::vector<std::pair<int, long long>>> g1(5);
    g1[0] = {{1,10}, {3,5}};
    g1[1] = {{2,1}};
    g1[3] = {{1,3}, {2,9}, {4,2}};
    g1[4] = {{0,7}, {2,6}};
    std::vector<long long> r1 = dijkstraShortestPaths(0, g1);
    assert(r1 == std::vector<long long>({0, 8, 9, 5, 7}));

    // Test 2: Disconnected graph
    std::vector<std::vector<std::pair<int, long long>>> g2(4);
    g2[0] = {{1, 2}};
    g2[2] = {{3, 5}};
    std::vector<long long> r2 = dijkstraShortestPaths(0, g2);
    assert(r2 == std::vector<long long>({0, 2, -1, -1}));

    // Test 3: Zero-weight edges and self-loop
    std::vector<std::vector<std::pair<int, long long>>> g3(2);
    g3[0] = {{0, 0}, {1, 0}};  // self-loop and zero edge
    g3[1] = {{1, 0}};          // self-loop
    std::vector<long long> r3 = dijkstraShortestPaths(0, g3);
    assert(r3 == std::vector<long long>({0, 0}));

    // Test 4: Single vertex
    std::vector<std::vector<std::pair<int, long long>>> g4(1);
    std::vector<long long> r4 = dijkstraShortestPaths(0, g4);
    assert(r4 == std::vector<long long>({0}));

    // Test 5: Parallel edges – keep smaller weight
    std::vector<std::vector<std::pair<int, long long>>> g5(2);
    g5[0] = {{1, 5}, {1, 3}, {1, 10}};
    g5[1] = {};
    std::vector<long long> r5 = dijkstraShortestPaths(0, g5);
    assert(r5 == std::vector<long long>({0, 3}));

    // Test 6: Larger weights (long long)
    std::vector<std::vector<std::pair<int, long long>>> g6(3);
    g6[0] = {{1, 10000000000LL}, {2, 1}};
    g6[1] = {{2, 1}};
    std::vector<long long> r6 = dijkstraShortestPaths(0, g6);
    assert(r6 == std::vector<long long>({0, 10000000000LL, 1}));

    return 0;
}

// We use Dijkstra's algorithm because all edge weights are non-negative, which guarantees the greedy selection of the minimum-distance unprocessed node is correct. We initialize a distance vector of size `n` with a very large value (e.g., `LLONG_MAX`) and set the source's distance to 0. A min-heap priority queue stores pairs `{distance, vertex}`. While the heap is not empty, we pop the pair with the smallest distance. If that distance is greater than the current known distance (stale entry), we skip it. Otherwise, we relax all outgoing edges: for each neighbor `v` with weight `w`, if `dist[u] + w < dist[v]`, update `dist[v]` and push the new `{dist[v], v}`. After the loop, for each vertex, if `dist[i]` is still `LLONG_MAX`, replace it with `-1` to indicate unreachability. Edge cases include source having no outgoing edges (others remain `-1`), negative weights (not allowed per specification, so we don't check), and large weights that could overflow `int`, hence using `long long`. The time complexity is `O((V+E) log V)`, where `V` is the number of vertices and `E` is the total number of edges, due to each edge possibly causing a heap insertion. The space complexity is `O(V + E)` for the distance vector and the heap.
