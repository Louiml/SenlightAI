Implement a C++ function `int minimumSpanningTreeWeight(int n, const std::vector<std::tuple<int, int, int>>& edges)` that takes the number of vertices `n` (labeled from 1 to `n`) and a list of undirected edges represented as tuples `(u, v, weight)`, and returns the total weight of the Minimum Spanning Tree (MST) using Prim's algorithm starting from vertex 1. The graph is guaranteed to be connected, edge weights are non-negative, and may contain duplicate edges (parallel edges) between the same pair of vertices. The function should compute and return the sum of weights of all edges in the MST.
// Prim's algorithm grows a tree one vertex at a time. Start from an arbitrary vertex (here vertex 1), maintain a vector `dist` where `dist[v]` is the minimum weight of an edge connecting `v` to the current tree, and a boolean `visited` array. Initially set all distances to infinity, except for vertex 1 (distance 0), then repeatedly select the unvisited vertex with the smallest `dist`, add its distance to the total, mark it visited, and relax all its outgoing edges by updating `dist[neighbor]` if the edge weight is smaller. Since the graph may have parallel edges, the smallest weight is naturally chosen by the relaxation. Complexity is \(O(n^2 + m)\) for the naïve implementation (scanning all vertices each iteration), and with a binary heap it becomes \(O((n+m)\log n)\). Since the problem statement is from a code snippet using an adjacency list and a linear scan, we can implement the \(O(n^2)\) version for clarity, which works well for moderate `n` (e.g., `n ≤ 2000`). Edge cases include a single vertex (returns 0), and duplicate edges where only the minimum should be considered. The graph is guaranteed connected, so no need to handle disconnected cases. Space complexity is \(O(n + m)\) for the adjacency list.
#include <vector>
#include <tuple>
#include <limits>
#include <algorithm>

// Returns the total weight of the Minimum Spanning Tree using Prim's algorithm.
// n: number of vertices (labels 1..n)
// edges: vector of tuples (u, v, weight) representing an undirected edge.
// Assumes the graph is connected and edge weights are non-negative.
int minimumSpanningTreeWeight(int n, const std::vector<std::tuple<int, int, int>>& edges) {
    // Build adjacency list: adj[vertex] = vector of pairs (neighbor, weight)
    std::vector<std::vector<std::pair<int, int>>> adj(n + 1);
    for (const auto& [u, v, w] : edges) {
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
    }

    const int INF = std::numeric_limits<int>::max();
    std::vector<int> dist(n + 1, INF);
    std::vector<bool> visited(n + 1, false);
    dist[1] = 0; // start from vertex 1

    int totalWeight = 0;

    for (int i = 1; i <= n; ++i) {
        // Find the unvisited vertex with the smallest distance
        int u = -1;
        int best = INF;
        for (int v = 1; v <= n; ++v) {
            if (!visited[v] && dist[v] < best) {
                best = dist[v];
                u = v;
            }
        }

        // In a connected graph, u should always be found for i <= n
        visited[u] = true;
        totalWeight += best;

        // Relax all edges from u
        for (const auto& [v, w] : adj[u]) {
            if (!visited[v] && w < dist[v]) {
                dist[v] = w;
            }
        }
    }

    return totalWeight;
}
#include <cassert>
#include <vector>
#include <tuple>

int main() {
    // Test 1: Simple triangle
    {
        std::vector<std::tuple<int, int, int>> edges = {{1,2,1},{2,3,2},{1,3,3}};
        assert(minimumSpanningTreeWeight(3, edges) == 3);
    }

    // Test 2: Single vertex
    {
        std::vector<std::tuple<int, int, int>> edges;
        assert(minimumSpanningTreeWeight(1, edges) == 0);
    }

    // Test 3: Duplicate edges - should pick the smaller one
    {
        std::vector<std::tuple<int, int, int>> edges = {{1,2,5},{2,1,3},{2,3,4},{3,2,1},{1,3,10}};
        // MST: use (1,2) with weight 3 and (2,3) with weight 1 => total 4
        assert(minimumSpanningTreeWeight(3, edges) == 4);
    }

    // Test 4: Larger graph (5 vertices, complete graph with chosen weights)
    {
        std::vector<std::tuple<int, int, int>> edges = {
            {1,2,2},{1,3,3},{1,4,6},{1,5,7},
            {2,3,4},{2,4,8},{2,5,9},
            {3,4,5},{3,5,10},
            {4,5,1}
        };
        // MST edges: (4,5)=1, (1,2)=2, (1,3)=3, (3,4)=5 => total 11
        assert(minimumSpanningTreeWeight(5, edges) == 11);
    }

    // Test 5: Star graph with center 1
    {
        std::vector<std::tuple<int, int, int>> edges = {{1,2,10},{1,3,20},{1,4,30}};
        assert(minimumSpanningTreeWeight(4, edges) == 60);
    }

    // Test 6: Line graph
    {
        std::vector<std::tuple<int, int, int>> edges = {{1,2,5},{2,3,6},{3,4,7}};
        assert(minimumSpanningTreeWeight(4, edges) == 18);
    }

    // Test 7: Two vertices with multiple weights
    {
        std::vector<std::tuple<int, int, int>> edges = {{1,2,10},{2,1,5},{1,2,20}};
        assert(minimumSpanningTreeWeight(2, edges) == 5);
    }

    // Test 8: All edges weight 1, complete graph on 4 vertices
    {
        std::vector<std::tuple<int, int, int>> edges = {
            {1,2,1},{1,3,1},{1,4,1},
            {2,3,1},{2,4,1},
            {3,4,1}
        };
        assert(minimumSpanningTreeWeight(4, edges) == 3);
    }

    // Test 9: Zero-weight edges
    {
        std::vector<std::tuple<int, int, int>> edges = {{1,2,0},{2,3,0},{1,3,5}};
        assert(minimumSpanningTreeWeight(3, edges) == 0);
    }

    // Test 10: Larger n=6 path with increasing weights
    {
        std::vector<std::tuple<int, int, int>> edges = {{1,2,1},{2,3,2},{3,4,4},{4,5,8},{5,6,16}};
        assert(minimumSpanningTreeWeight(6, edges) == 31);
    }

    return 0;
}
