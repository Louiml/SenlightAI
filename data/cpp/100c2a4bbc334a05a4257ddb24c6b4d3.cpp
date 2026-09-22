Write a C++ function `vector<int> computeShortestPaths(const vector<Edge>& edges, int V, int source)` that implements the Bellman-Ford algorithm to find the shortest distances from a given source vertex to all other vertices in a weighted directed graph. The graph may contain edges with negative weights, but no negative-weight cycles reachable from the source. The function must return a `vector<int>` where the element at index `i` is the shortest distance from `source` to vertex `i`, or `INT_MAX` if vertex `i` is unreachable. If the graph contains a negative-weight cycle reachable from the source, the function should return an empty vector `{}` to signal the presence of such a cycle. You may assume the `Edge` struct is defined as `struct Edge { int src, dest, weight; };` and that vertex indices are in the range `[0, V-1]`. Handle the case where the source is a valid vertex (already guaranteed) and where `V` is positive.

The Bellman-Ford algorithm relaxes all edges exactly `V-1` times. In each relaxation pass, for each edge `(u, v, w)`, if the current distance to `u` is finite and `dist[u] + w` is smaller than the current distance to `v`, we update `dist[v]`. This works because the longest simple path in a graph with `V` vertices has at most `V-1` edges, so after `V-1` passes, the shortest distances are finalized (provided no negative-weight cycle is reachable). After the passes, we perform one final relaxation check over all edges. If any edge can still be relaxed, a negative-weight cycle exists, and we return an empty vector. Edge cases include: vertices unreachable from the source (remain at `INT_MAX`), source distance is zero, and edges with negative weights that do not form a cycle. The time complexity is `O(V * E)` and the auxiliary space complexity is `O(V)` for the distance vector plus `O(E)` for storing the edges (which are given as input).

#include <vector>
#include <climits>
#include <algorithm>

struct Edge {
    int src, dest, weight;
};

// Computes shortest distances from source using Bellman-Ford.
// Returns empty vector if a negative-weight cycle is reachable from source.
std::vector<int> computeShortestPaths(const std::vector<Edge>& edges, int V, int source) {
    std::vector<int> dist(V, INT_MAX);
    dist[source] = 0;

    // Relax all edges V-1 times
    for (int i = 0; i < V - 1; ++i) {
        bool updated = false;
        for (const auto& edge : edges) {
            if (dist[edge.src] != INT_MAX && 
                dist[edge.src] + edge.weight < dist[edge.dest]) {
                dist[edge.dest] = dist[edge.src] + edge.weight;
                updated = true;
            }
        }
        // Early exit if no update occurred
        if (!updated) break;
    }

    // Check for negative-weight cycle
    for (const auto& edge : edges) {
        if (dist[edge.src] != INT_MAX && 
            dist[edge.src] + edge.weight < dist[edge.dest]) {
            return {};  // indicates negative cycle
        }
    }

    return dist;
}

#include <cassert>
#include <iostream>
#include <vector>
#include <climits>

// assume the Edge struct and computeShortestPaths are defined above

int main() {
    // Test 1: Standard example from the prompt (no negative cycle)
    std::vector<Edge> edges1 = {
        {0, 1, -1}, {0, 2, 4}, {1, 2, 3}, {1, 3, 2},
        {1, 4, 2}, {3, 1, 1}, {3, 2, 5}, {4, 3, -3}
    };
    std::vector<int> result1 = computeShortestPaths(edges1, 5, 0);
    std::vector<int> expected1 = {0, -1, 2, -2, 1};
    assert(result1 == expected1);

    // Test 2: Graph with negative weight cycle reachable from source
    std::vector<Edge> edges2 = {
        {0, 1, 1}, {1, 2, -1}, {2, 1, -1}, {2, 3, 2}
    };
    std::vector<int> result2 = computeShortestPaths(edges2, 4, 0);
    assert(result2.empty());

    // Test 3: Unreachable vertices
    std::vector<Edge> edges3 = {
        {0, 1, 5}, {2, 3, -4}
    };
    std::vector<int> result3 = computeShortestPaths(edges3, 4, 0);
    std::vector<int> expected3 = {0, 5, INT_MAX, INT_MAX};
    assert(result3 == expected3);

    // Test 4: Single vertex graph
    std::vector<Edge> edges4;
    std::vector<int> result4 = computeShortestPaths(edges4, 1, 0);
    assert(result4 == std::vector<int>{0});

    // Test 5: Simple negative edge but no cycle
    std::vector<Edge> edges5 = {{0, 1, -10}, {1, 2, 5}};
    std::vector<int> result5 = computeShortestPaths(edges5, 3, 0);
    std::vector<int> expected5 = {0, -10, -5};
    assert(result5 == expected5);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
