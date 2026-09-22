Implement a C++ function `bool hasNegativeCycle(int vertexCount, const std::vector<std::vector<Edge>>& graph, int source)` that detects whether a directed weighted graph contains a negative cycle reachable from the given source vertex. The graph is represented as an adjacency list where each `Edge` has an integer index `idx` and weight `w`. The function should perform Bellman-Ford relaxation starting from `source`. If a relaxation occurs on the `vertexCount`-th full pass over all edges, a negative cycle is reachable and the function returns `true`; otherwise, it returns `false`. Vertices are numbered from `0` to `vertexCount - 1`. The graph may contain zero-weight or negative-weight edges, and some vertices might be unreachable from the source. The function must not modify the input graph. Use a large sentinel value (e.g., `1e9`) as infinity.
The solution uses the standard Bellman-Ford algorithm to compute shortest distances from the source. In each iteration (pass over all edges), we attempt to relax each edge `(u, v, w)` by checking if `dist[u] + w < dist[v]`. If any relaxation occurs, we mark `update = true`. After `vertexCount - 1` passes, the shortest paths without negative cycles are guaranteed to be found. If an additional complete pass still yields an update, then a negative cycle is reachable from the source, because distances can be improved indefinitely. Key edge cases: if the source has no outgoing paths, the algorithm correctly returns `false`; if there is a negative cycle but it is unreachable from the source, the function returns `false` as well; if there are multiple edges or self-loops, they are handled normally. Time complexity is O(V * E) where V is vertex count and E is total edge count across all adjacency lists. Space complexity is O(V) for the distance vector, plus the input graph storage (not counted).
#include <vector>
#include <algorithm>
#include <limits>

struct Edge {
    int idx;
    int w;
};

bool hasNegativeCycle(int vertexCount, const std::vector<std::vector<Edge>>& graph, int source) {
    const int INF = 1e9;
    std::vector<int> dist(vertexCount, INF);
    dist[source] = 0;
    
    // Relax edges up to vertexCount times; if update on the last pass, negative cycle
    for (int pass = 0; pass < vertexCount; ++pass) {
        bool updated = false;
        for (int u = 0; u < vertexCount; ++u) {
            if (dist[u] == INF) continue;
            for (const Edge& e : graph[u]) {
                if (dist[e.idx] > dist[u] + e.w) {
                    dist[e.idx] = dist[u] + e.w;
                    updated = true;
                }
            }
        }
        if (!updated) return false; // no more improvements, safe
        if (pass == vertexCount - 1) return true; // still updating after V-1 passes => negative cycle
    }
    return false;
}
#include <cassert>
#include <vector>
#include <iostream>

struct Edge {
    int idx;
    int w;
};

bool hasNegativeCycle(int vertexCount, const std::vector<std::vector<Edge>>& graph, int source);

int main() {
    // Test 1: Simple acyclic graph, no negative cycle
    {
        int n = 3;
        std::vector<std::vector<Edge>> g(n);
        g[0].push_back({1, 1});
        g[1].push_back({2, 2});
        assert(hasNegativeCycle(n, g, 0) == false);
    }
    
    // Test 2: Negative edge but no cycle
    {
        int n = 3;
        std::vector<std::vector<Edge>> g(n);
        g[0].push_back({1, -5});
        g[1].push_back({2, 2});
        assert(hasNegativeCycle(n, g, 0) == false);
    }
    
    // Test 3: Negative cycle reachable from source
    {
        int n = 3;
        std::vector<std::vector<Edge>> g(n);
        g[0].push_back({1, 1});
        g[1].push_back({2, -2});
        g[2].push_back({1, -1}); // 1->2->1 = -3 total, negative cycle
        assert(hasNegativeCycle(n, g, 0) == true);
    }
    
    // Test 4: Negative cycle not reachable from source
    {
        int n = 4;
        std::vector<std::vector<Edge>> g(n);
        g[0].push_back({1, 1});
        g[2].push_back({3, -2});
        g[3].push_back({2, -1}); // cycle 2<->3 negative but not reachable from 0
        assert(hasNegativeCycle(n, g, 0) == false);
    }
    
    // Test 5: Self-loop negative edge (shortest negative cycle)
    {
        int n = 2;
        std::vector<std::vector<Edge>> g(n);
        g[0].push_back({0, -1});
        g[0].push_back({1, 5});
        assert(hasNegativeCycle(n, g, 0) == true);
    }
    
    // Test 6: Single vertex, no edges
    {
        int n = 1;
        std::vector<std::vector<Edge>> g(n);
        assert(hasNegativeCycle(n, g, 0) == false);
    }
    
    // Test 7: Large positive weights, no negative cycle
    {
        int n = 5;
        std::vector<std::vector<Edge>> g(n);
        for (int i = 0; i < n - 1; ++i)
            g[i].push_back({i + 1, 10});
        assert(hasNegativeCycle(n, g, 0) == false);
    }
    
    // Test 8: Zero-weight cycle (not negative, should be false)
    {
        int n = 2;
        std::vector<std::vector<Edge>> g(n);
        g[0].push_back({1, 0});
        g[1].push_back({0, 0});
        assert(hasNegativeCycle(n, g, 0) == false);
    }
    
    // Test 9: Multiple edges between same vertices, one path to negative cycle
    {
        int n = 3;
        std::vector<std::vector<Edge>> g(n);
        g[0].push_back({1, 100});
        g[0].push_back({1, -1});
        g[1].push_back({2, 1});
        g[2].push_back({1, -2}); // negative cycle 1->2->1 with -1 total
        assert(hasNegativeCycle(n, g, 0) == true);
    }
    
    // Test 10: Disconnected graph, source isolated
    {
        int n = 4;
        std::vector<std::vector<Edge>> g(n);
        g[1].push_back({2, -2});
        g[2].push_back({1, -2});
        assert(hasNegativeCycle(n, g, 0) == false);
    }
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
