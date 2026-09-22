Write a C++ function `bool bellmanFordShortestDistances(int source, int nodeCount, int edgeCount, const std::vector<std::tuple<int, int, long long>>& edges, std::vector<long long>& distances)` that computes the shortest distances from the given source node to all other nodes in a directed weighted graph. The graph has nodes numbered from 1 to `nodeCount`, and edges are given as tuples `(from, to, weight)`, where weights can be negative. The function must handle the possibility of unreachable nodes by setting their distance to a large sentinel value `LLONG_MAX/4` (effectively infinity). The function must also detect whether the graph contains a negative-weight cycle reachable from the source. If such a cycle exists, return `false`; otherwise return `true` and fill `distances` (indexed 1-based, with `distances[0]` unused) with the shortest distances. Your implementation must correctly skip unreachable nodes during relaxation to avoid overflow, and clamp distances to the sentinel value to prevent arithmetic overflow when adding a large negative weight. The algorithm must run in `O(nodeCount * edgeCount)` time and use `O(nodeCount)` auxiliary space.

The core algorithm is the Bellman-Ford shortest path algorithm, which relaxes all edges repeatedly. Initialize `distances` to a sentinel infinity value (e.g., `LLONG_MAX/4`) and set `distances[source] = 0`. Then perform exactly `nodeCount - 1` iterations, each time iterating over all edges. For each edge `(u, v, w)`, if `distances[u]` is not infinity (i.e., `u` is reachable), then attempt to relax: if `distances[u] + w < distances[v]`, update `distances[v]` to be the minimum of the sentinel and `distances[u] + w` to avoid overflow. Track whether any update occurred in an iteration; if no update occurs, early exit. After the `nodeCount - 1` relaxations, perform one more pass over all edges. If any edge can still relax a node (where `u` is reachable), then a negative cycle reachable from the source exists, so return `false`. Otherwise return `true`. Important edge cases: when `nodeCount == 0` or `1`, the loop should handle gracefully; when an edge has a huge negative weight that would overflow, clamping to the sentinel prevents undefined behavior; unreachable nodes remain at the sentinel. Time complexity is `O(nodeCount * edgeCount)` because we iterate over all edges up to `nodeCount - 1` times plus one final check. Space complexity is `O(nodeCount)` for the distances array.

#include <vector>
#include <tuple>
#include <limits>
#include <cstddef>

// Compute shortest distances from source using Bellman-Ford.
// Returns false if there is a negative cycle reachable from source, otherwise true.
bool bellmanFordShortestDistances(
    int source,
    int nodeCount,
    int edgeCount,
    const std::vector<std::tuple<int, int, long long>>& edges,
    std::vector<long long>& distances)
{
    const long long INF = std::numeric_limits<long long>::max() / 4;

    // Resize distances to 1-based indexing (index 0 unused).
    distances.assign(nodeCount + 1, INF);
    if (nodeCount == 0) return true;

    distances[source] = 0;

    // Relax all edges up to nodeCount-1 times.
    for (int iteration = 1; iteration < nodeCount; ++iteration) {
        bool updated = false;
        for (int i = 0; i < edgeCount; ++i) {
            int u = std::get<0>(edges[i]);
            int v = std::get<1>(edges[i]);
            long long w = std::get<2>(edges[i]);

            if (distances[u] == INF) continue;  // u not reachable

            long long candidate = distances[u] + w;
            if (candidate < distances[v]) {
                // Clamp to INF to prevent overflow.
                distances[v] = (candidate < INF) ? candidate : INF;
                updated = true;
            }
        }
        if (!updated) break;  // early exit
    }

    // Check for negative cycles reachable from source.
    for (int i = 0; i < edgeCount; ++i) {
        int u = std::get<0>(edges[i]);
        int v = std::get<1>(edges[i]);
        long long w = std::get<2>(edges[i]);

        if (distances[u] == INF) continue;

        if (distances[u] + w < distances[v]) {
            return false;  // negative cycle exists
        }
    }

    return true;
}

#include <cassert>
#include <vector>
#include <tuple>

// The solution function is declared elsewhere, but for testing we include it here.
// In a real test file, include the header or copy the function above.

int main() {
    // Test 1: Simple graph with positive edges.
    std::vector<std::tuple<int, int, long long>> edges1 = {
        {1, 2, 2}, {2, 3, 3}, {1, 3, 10}
    };
    std::vector<long long> dist1;
    bool ok1 = bellmanFordShortestDistances(1, 3, 3, edges1, dist1);
    assert(ok1 == true);
    assert(dist1[1] == 0);
    assert(dist1[2] == 2);
    assert(dist1[3] == 5);  // 1->2->3 = 2+3=5 < 10

    // Test 2: Graph with unreachable nodes.
    std::vector<std::tuple<int, int, long long>> edges2 = {
        {1, 2, 1}, {2, 3, 1}
    };
    std::vector<long long> dist2;
    bool ok2 = bellmanFordShortestDistances(1, 4, 2, edges2, dist2);
    assert(ok2 == true);
    assert(dist2[1] == 0);
    assert(dist2[2] == 1);
    assert(dist2[3] == 2);
    // Node 4 is unreachable, distance should be INF.
    long long INF = std::numeric_limits<long long>::max() / 4;
    assert(dist2[4] == INF);

    // Test 3: Negative edge but no negative cycle.
    std::vector<std::tuple<int, int, long long>> edges3 = {
        {1, 2, -1}, {2, 3, 2}, {1, 3, 5}
    };
    std::vector<long long> dist3;
    bool ok3 = bellmanFordShortestDistances(1, 3, 3, edges3, dist3);
    assert(ok3 == true);
    assert(dist3[1] == 0);
    assert(dist3[2] == -1);
    assert(dist3[3] == 1);  // -1+2=1 < 5

    // Test 4: Negative cycle reachable from source.
    std::vector<std::tuple<int, int, long long>> edges4 = {
        {1, 2, 1}, {2, 1, -2}, {2, 3, 0}
    };
    std::vector<long long> dist4;
    bool ok4 = bellmanFordShortestDistances(1, 3, 3, edges4, dist4);
    assert(ok4 == false);  // 1->2->1 gives cycle with sum -1

    // Test 5: Negative cycle not reachable from source.
    std::vector<std::tuple<int, int, long long>> edges5 = {
        {1, 2, 1}, {2, 3, 1}, {3, 2, -3}
    };
    std::vector<long long> dist5;
    bool ok5 = bellmanFordShortestDistances(1, 3, 3, edges5, dist5);
    assert(ok5 == true);  // cycle 2<->3 is not reachable from 1
    assert(dist5[1] == 0);
    assert(dist5[2] == 1);
    assert(dist5[3] == 2);

    // Test 6: Large negative weight that would overflow without clamping.
    std::vector<std::tuple<int, int, long long>> edges6 = {
        {1, 2, -1000000000000LL}, {2, 1, -1000000000000LL}
    };
    std::vector<long long> dist6;
    bool ok6 = bellmanFordShortestDistances(1, 2, 2, edges6, dist6);
    // This is a negative cycle, so return false.
    assert(ok6 == false);

    // Test 7: Single node graph.
    std::vector<std::tuple<int, int, long long>> edges7 = {};
    std::vector<long long> dist7;
    bool ok7 = bellmanFordShortestDistances(1, 1, 0, edges7, dist7);
    assert(ok7 == true);
    assert(dist7[1] == 0);

    // Test 8: Multiple edges between same nodes.
    std::vector<std::tuple<int, int, long long>> edges8 = {
        {1, 2, 5}, {1, 2, 3}, {2, 3, 1}
    };
    std::vector<long long> dist8;
    bool ok8 = bellmanFordShortestDistances(1, 3, 3, edges8, dist8);
    assert(ok8 == true);
    assert(dist8[2] == 3);
    assert(dist8[3] == 4);

    return 0;
}
