// Write a C++ function `std::vector<int> shortestDistancesFromSource(int source, const std::vector<Edge>& edges, int numVertices)` that implements the Bellman-Ford algorithm to find the shortest distances from a given source vertex to all other vertices in a directed graph with possibly negative edge weights. The graph is represented by a vector of `Edge` objects, where each `Edge` has integer fields `begin`, `end`, and `weight` (vertices are 0-indexed). If a vertex is unreachable from the source, the function must return a large sentinel value `30000` for that vertex instead of `INT_MAX`. The function should not modify the input edges. Assume the graph has no negative-weight cycles. The function must produce the same output format as the original code: distances for vertices 0 through `numVertices-1`, with unreachable vertices represented by `30000`.
// The core is the classic Bellman-Ford algorithm, which relaxes all edges `numVertices - 1` times. Initialize a distance vector of size `numVertices` with a large sentinel (e.g., `INT_MAX`) and set `distance[source] = 0`. Each relaxation step checks if `distance[begin] != INT_MAX` and if `distance[end] > distance[begin] + weight`, then updates `distance[end]`. After the loop, traverse the distance vector: if a distance remains `INT_MAX`, replace it with `30000`. Edge cases: source vertex itself always has distance 0; a graph with zero edges means all other vertices are unreachable; if the source is out of bounds, the function should still work gracefully (though typically input is valid). Since negative cycles are assumed absent, we do not need a final check for them. Time complexity is O(V * E), space complexity is O(V) for distances (plus the input storage already exists).
#include <vector>
#include <climits>
#include <algorithm>

// Represents a directed edge in the graph.
struct Edge {
    int begin;
    int end;
    int weight;
};

// Computes shortest distances from source to all vertices using Bellman-Ford.
// Unreachable vertices are represented by a sentinel value 30000.
std::vector<int> shortestDistancesFromSource(int source, const std::vector<Edge>& edges, int numVertices) {
    const int SENTINEL = 30000;
    std::vector<int> distance(numVertices, INT_MAX);
    distance[source] = 0;

    // Relax all edges V-1 times.
    for (int i = 0; i < numVertices - 1; ++i) {
        bool updated = false;
        for (const auto& edge : edges) {
            if (distance[edge.begin] != INT_MAX &&
                distance[edge.end] > distance[edge.begin] + edge.weight) {
                distance[edge.end] = distance[edge.begin] + edge.weight;
                updated = true;
            }
        }
        if (!updated) break; // Early exit if no changes.
    }

    // Replace unreachable distances with sentinel.
    std::transform(distance.begin(), distance.end(), distance.begin(),
                   [](int d) { return d == INT_MAX ? SENTINEL : d; });
    return distance;
}
#include <cassert>
#include <vector>

// The Edge struct and solution function are assumed to be declared above.

int main() {
    // Test 1: No edges, source 0, 3 vertices.
    {
        std::vector<Edge> edges;
        auto dist = shortestDistancesFromSource(0, edges, 3);
        assert(dist == std::vector<int>({0, 30000, 30000}));
    }

    // Test 2: Simple path with positive weights.
    {
        std::vector<Edge> edges = {{0, 1, 2}, {1, 2, 3}};
        auto dist = shortestDistancesFromSource(0, edges, 3);
        assert(dist == std::vector<int>({0, 2, 5}));
    }

    // Test 3: Negative edge weights but no negative cycle.
    {
        std::vector<Edge> edges = {{0, 1, -1}, {1, 2, -2}, {0, 2, 10}};
        auto dist = shortestDistancesFromSource(0, edges, 3);
        assert(dist == std::vector<int>({0, -1, -3}));
    }

    // Test 4: Disconnected component.
    {
        std::vector<Edge> edges = {{0, 1, 5}, {2, 3, 7}};
        auto dist = shortestDistancesFromSource(0, edges, 4);
        assert(dist == std::vector<int>({0, 5, 30000, 30000}));
    }

    // Test 5: Source is not zero, and multiple paths to same vertex.
    {
        std::vector<Edge> edges = {{0, 2, 4}, {1, 0, 1}, {1, 2, 3}, {2, 3, 2}};
        auto dist = shortestDistancesFromSource(1, edges, 4);
        // From 1: to 0 = 1, to 2 = min(1+4=5, 3) = 3, to 3 = 3+2=5
        assert(dist == std::vector<int>({1, 0, 3, 5}));
    }

    // Test 6: Self-loop with positive weight (ignored) and unreachable.
    {
        std::vector<Edge> edges = {{2, 2, 10}};
        auto dist = shortestDistancesFromSource(1, edges, 3);
        assert(dist == std::vector<int>({30000, 0, 30000}));
    }

    return 0;
}
