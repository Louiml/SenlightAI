// Write a C++ function `bool hasNegativeCycle(int N, const std::vector<std::vector<Edge>>& graph)` that determines whether a directed graph with `N` vertices (numbered 1 through `N`) contains a negative weight cycle reachable from any vertex, given an adjacency list where each `Edge` has fields `to` and `weight`. The graph may contain multiple edges and self-loops. The function should handle up to N = 500 vertices and return `true` if and only if there exists at least one cycle whose total weight is negative. This is a general version of the classic "wormholes" problem where roads are undirected (positive weights) but wormholes are directed (negative weights), and the task is to detect whether time travel is possible.
The problem is a direct application of the Bellman-Ford algorithm for detecting negative weight cycles in a graph. The key idea is to initialize all distances to infinity (or a large value) except for a source, then relax all edges `N-1` times; after that, if any edge can still be relaxed, a negative cycle exists. However, since we need to detect a negative cycle reachable from *any* vertex (not a fixed source), we can initialize all distances to 0. This works because a negative cycle will cause distances to keep decreasing indefinitely regardless of the starting point: if a negative cycle exists, after `N` iterations of relaxing all edges, at least one distance will still improve, which indicates the presence of the cycle. The algorithm runs in O(N * E) time where E is the total number of edges, and O(N) auxiliary space for the distance array. Edge cases include disconnected graphs (the algorithm still works because initialization to 0 makes all vertices "reachable"), multiple edges between the same vertices (they are all processed), self-loops with negative weight (which immediately produce a negative cycle), and graphs with no negative cycles at all. The implementation must be careful with integer overflow: since the problem constraints are not specified, but from the original snippet N ≤ 500 and weights likely fit in 32-bit ints, we use a large sentinel value like `1e9` for "infinity" and check for relaxation using long long to avoid overflow when adding weights to distances.
#include <vector>
#include <limits>

struct Edge {
    int to;
    int weight;
};

// Determines if a directed graph contains a negative weight cycle.
// Vertices are numbered 1..N. The graph is given as an adjacency list.
bool hasNegativeCycle(int N, const std::vector<std::vector<Edge>>& graph) {
    // Initialize all distances to 0. This allows negative cycles
    // reachable from any vertex to be detected because any negative cycle
    // will cause distances to keep decreasing after N-1 relaxations.
    std::vector<int> dist(N + 1, 0);
    const int INF = 1e9; // Large value for sentinel, but not used here.

    // Relax all edges N times. If the Nth relaxation still improves a distance,
    // a negative cycle exists.
    for (int i = 0; i < N; ++i) {
        bool updated = false;
        for (int u = 1; u <= N; ++u) {
            for (const auto& e : graph[u]) {
                // Use long long to avoid overflow.
                long long new_dist = (long long)dist[u] + e.weight;
                if (new_dist < dist[e.to]) {
                    dist[e.to] = (int)new_dist;
                    updated = true;
                    // If we are on the Nth iteration and an update occurs,
                    // a negative cycle is present.
                    if (i == N - 1) {
                        return true;
                    }
                }
            }
        }
        // Optional early exit: if no updates in a full pass, no negative cycle.
        if (!updated) {
            return false;
        }
    }
    return false;
}
#include <cassert>
#include <vector>

// Edge and function declaration (assumed from solution).
struct Edge { int to; int weight; };
bool hasNegativeCycle(int N, const std::vector<std::vector<Edge>>& graph);

int main() {
    // Test 1: No edges -> no negative cycle.
    {
        int N = 3;
        std::vector<std::vector<Edge>> g(N + 1);
        assert(!hasNegativeCycle(N, g));
    }

    // Test 2: Simple positive cycle (1->2->3->1 with weights 1,1,1) -> no negative cycle.
    {
        int N = 3;
        std::vector<std::vector<Edge>> g(N + 1);
        g[1].push_back({2, 1});
        g[2].push_back({3, 1});
        g[3].push_back({1, 1});
        assert(!hasNegativeCycle(N, g));
    }

    // Test 3: Negative cycle (1->2 with -5, 2->1 with 2) total -3.
    {
        int N = 2;
        std::vector<std::vector<Edge>> g(N + 1);
        g[1].push_back({2, -5});
        g[2].push_back({1, 2});
        assert(hasNegativeCycle(N, g));
    }

    // Test 4: Self-loop with negative weight.
    {
        int N = 1;
        std::vector<std::vector<Edge>> g(N + 1);
        g[1].push_back({1, -1});
        assert(hasNegativeCycle(N, g));
    }

    // Test 5: Graph with no negative cycle but there is a negative edge (1->2 -3, 2->3 4) not forming a cycle.
    {
        int N = 3;
        std::vector<std::vector<Edge>> g(N + 1);
        g[1].push_back({2, -3});
        g[2].push_back({3, 4});
        assert(!hasNegativeCycle(N, g));
    }

    // Test 6: Multiple edges and a negative cycle reachable only from a specific vertex.
    {
        int N = 4;
        std::vector<std::vector<Edge>> g(N + 1);
        g[1].push_back({2, 10}); // positive
        g[2].push_back({3, -1});
        g[3].push_back({2, -1}); // negative cycle 2<->3 total -2
        g[4].push_back({1, 0});  // vertex 4 connects to the cycle
        assert(hasNegativeCycle(N, g));
    }

    // Test 7: Large graph without negative cycle (e.g., all weights positive).
    {
        int N = 5;
        std::vector<std::vector<Edge>> g(N + 1);
        for (int i = 1; i < N; ++i) {
            g[i].push_back({i+1, i});
        }
        assert(!hasNegativeCycle(N, g));
    }

    // Test 8: Negative cycle that requires many iterations to detect.
    {
        int N = 4;
        std::vector<std::vector<Edge>> g(N + 1);
        g[1].push_back({2, 1});
        g[2].push_back({3, 1});
        g[3].push_back({4, 1});
        g[4].push_back({1, -10}); // cycle total -7
        assert(hasNegativeCycle(N, g));
    }

    // Test 9: Disconnected components with positive cycles only -> no negative cycle.
    {
        int N = 6;
        std::vector<std::vector<Edge>> g(N + 1);
        g[1].push_back({2, 1});
        g[2].push_back({1, 1}); // positive cycle
        g[3].push_back({4, 2});
        g[4].push_back({3, 2}); // positive cycle
        g[5].push_back({6, 1});
        g[6].push_back({5, 1}); // positive cycle
        assert(!hasNegativeCycle(N, g));
    }

    // Test 10: Graph with zero-weight cycle (not negative) -> no negative cycle.
    {
        int N = 3;
        std::vector<std::vector<Edge>> g(N + 1);
        g[1].push_back({2, 0});
        g[2].push_back({3, 0});
        g[3].push_back({1, 0});
        assert(!hasNegativeCycle(N, g));
    }

    return 0;
}
