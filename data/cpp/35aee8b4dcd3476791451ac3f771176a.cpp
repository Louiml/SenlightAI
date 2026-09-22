/*
Write a standalone C++ function that, given a directed weighted graph represented by its number of vertices `V`, number of edges `E`, and a vector of edges (each with source, destination, and weight), computes the shortest distances from vertex `0` to all other vertices using the Bellman-Ford algorithm. The function must return a vector of long long distances, where `dist[i]` is the shortest distance from vertex 0 to vertex i. If the graph contains a negative-weight cycle reachable from vertex 0, the function should throw a `std::runtime_error` with the message `"Negative cycle detected"`. Vertex indices are 0-based, and all weights are integers. The input will always have at least one vertex, and the graph may contain self-loops and parallel edges. Assume that vertex 0 is the source and that if a vertex is unreachable, its distance is `LLONG_MAX` (which is also the initial infinity value). Ensure the solution handles large absolute weights (up to 1e9) and up to 10^5 edges efficiently.
*/
#include <vector>
#include <limits>
#include <stdexcept>

struct Edge {
    int src;
    int dest;
    long long weight;
};

// Compute shortest distances from vertex 0 using Bellman-Ford.
// Throws std::runtime_error if a negative-weight cycle is reachable from vertex 0.
std::vector<long long> bellmanFordShortestPaths(int V, const std::vector<Edge>& edges) {
    const long long INF = std::numeric_limits<long long>::max() / 4; // Avoid overflow on add
    std::vector<long long> dist(V, INF);
    dist[0] = 0;

    // Relax all edges V-1 times
    for (int i = 1; i < V; ++i) {
        bool changed = false;
        for (const auto& e : edges) {
            if (dist[e.src] != INF && dist[e.src] + e.weight < dist[e.dest]) {
                dist[e.dest] = dist[e.src] + e.weight;
                changed = true;
            }
        }
        if (!changed) break; // Early exit if no improvement
    }

    // Check for negative-weight cycles
    for (const auto& e : edges) {
        if (dist[e.src] != INF && dist[e.src] + e.weight < dist[e.dest]) {
            throw std::runtime_error("Negative cycle detected");
        }
    }

    return dist;
}
#include <cassert>
#include <vector>
#include <stdexcept>
#include <limits>

bool hasNegativeCycle(int V, const std::vector<Edge>& edges) {
    try {
        bellmanFordShortestPaths(V, edges);
        return false;
    } catch (const std::runtime_error&) {
        return true;
    }
}

int main() {
    // Test 1: Simple positive graph
    {
        std::vector<Edge> edges = {{0,1,4},{0,2,1},{2,1,2},{1,3,1}};
        auto dist = bellmanFordShortestPaths(4, edges);
        assert(dist[0] == 0);
        assert(dist[1] == 3);
        assert(dist[2] == 1);
        assert(dist[3] == 4);
    }
    // Test 2: Negative edges but no cycle
    {
        std::vector<Edge> edges = {{0,1,-1},{0,2,4},{1,2,3},{1,3,2},{1,4,2},{3,2,5},{3,1,1},{4,3,-3}};
        auto dist = bellmanFordShortestPaths(5, edges);
        assert(dist[0] == 0);
        assert(dist[1] == -1);
        assert(dist[2] == 2);
        assert(dist[3] == -2);
        assert(dist[4] == 1);
    }
    // Test 3: Negative cycle reachable
    {
        std::vector<Edge> edges = {{0,1,1},{1,2,-1},{2,1,-1}};
        assert(hasNegativeCycle(3, edges) == true);
    }
    // Test 4: Unreachable vertex
    {
        std::vector<Edge> edges = {{0,1,2}};
        auto dist = bellmanFordShortestPaths(3, edges);
        assert(dist[0] == 0);
        assert(dist[1] == 2);
        assert(dist[2] == std::numeric_limits<long long>::max() / 4);
    }
    // Test 5: Self-loop positive
    {
        std::vector<Edge> edges = {{0,0,5},{0,1,1}};
        auto dist = bellmanFordShortestPaths(2, edges);
        assert(dist[0] == 0);
        assert(dist[1] == 1);
    }
    // Test 6: Self-loop negative (cycle)
    {
        std::vector<Edge> edges = {{0,0,-1},{0,1,1}};
        assert(hasNegativeCycle(2, edges) == true);
    }
    // Test 7: Single vertex
    {
        std::vector<Edge> edges = {};
        auto dist = bellmanFordShortestPaths(1, edges);
        assert(dist.size() == 1);
        assert(dist[0] == 0);
    }
    // Test 8: Large weights no overflow
    {
        std::vector<Edge> edges = {{0,1,1000000000},{1,2,-1000000000}};
        auto dist = bellmanFordShortestPaths(3, edges);
        assert(dist[1] == 1000000000);
        assert(dist[2] == 0);
    }
    // Test 9: Parallel edges with negative one
    {
        std::vector<Edge> edges = {{0,1,10},{0,1,-5},{1,2,2}};
        auto dist = bellmanFordShortestPaths(3, edges);
        assert(dist[1] == -5);
        assert(dist[2] == -3);
    }
    // Test 10: Disconnected component
    {
        std::vector<Edge> edges = {{0,1,1}};
        auto dist = bellmanFordShortestPaths(5, edges);
        assert(dist[0] == 0);
        assert(dist[1] == 1);
        for (int i = 2; i < 5; ++i) {
            assert(dist[i] == std::numeric_limits<long long>::max() / 4);
        }
    }
    return 0;
}
// The Bellman-Ford algorithm solves the single-source shortest path problem for graphs that may contain negative edge weights, and it also detects negative-weight cycles reachable from the source. The main idea is to iteratively relax all edges for `V-1` iterations, where `V` is the number of vertices. In each iteration, for each edge (u, v, w), if the current distance to `u` plus `w` is less than the current distance to `v`, we update the distance to `v`. After `V-1` iterations, all shortest paths are guaranteed to be found because any simple path has at most `V-1` edges. Then, to detect a negative-weight cycle, we perform one extra pass over all edges: if any relaxation is still possible, that edge is part of a negative cycle reachable from the source, so we throw an exception. Important edge cases include: unreachable vertices (distance remains `LLONG_MAX`), self-loops (which may or may not cause issues depending on sign), and very large weights that could overflow when adding, so we must check if `dist[u]` is not infinity before adding. The time complexity is `O(V*E)` and space complexity is `O(V)` for the distance array. This is optimal for general graphs with negative weights.
