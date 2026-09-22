// Write a C++ function `double shortestPathCost(const std::vector<std::tuple<int,int,double>>& edges, int numVertices, int source, int target)` that takes a list of undirected weighted edges (vertex IDs are non‑negative integers in `[0, numVertices-1]`), the total number of vertices, a source vertex, and a target vertex. The function must return the total weight of the shortest path from `source` to `target` using Dijkstra’s algorithm. If `source` and `target` are the same, return `0.0`. If no path exists, return `-1.0`. The graph may contain zero‑weight edges, but all edge weights must be non‑negative. The graph may be disconnected. The input list may contain duplicate edges; if multiple edges exist between the same pair of vertices, only the one with the smallest weight should be considered (i.e., an edge that is not the minimum weight between two vertices is ignored). The function must not modify the input list. Use a standard priority queue for efficiency.

// The solution builds a graph from the edge list, storing for each vertex a map from neighbor to the minimum edge weight seen so far (so duplicates are resolved). Then we run Dijkstra’s algorithm from `source` using a min‑heap of `(distance, vertex)` pairs. We initialize distances to `INF` (a large constant like `1e18`), set `dist[source]=0`, and push the source. While the heap is non‑empty, we pop the vertex with smallest tentative distance; if we reached the target, we return that distance. For each neighbor, if the new distance via the current vertex is smaller than the stored one, we update and push. Because edge weights are non‑negative and we use a priority queue, the first time we pop the target we have the shortest path. If the heap becomes empty without reaching the target, no path exists, so we return `-1.0`. Special care: if source==target we return 0 immediately without running the algorithm. Time complexity is `O((V+E) log V)` where `V` is the number of vertices and `E` is the number of distinct edges after deduplication. Space complexity is `O(V+E)`.

#include <vector>
#include <tuple>
#include <queue>
#include <limits>
#include <unordered_map>
#include <utility>
#include <cassert>

// Returns the cost of the shortest path from source to target in an undirected weighted graph.
// If source == target, returns 0.0. If there is no path, returns -1.0.
// Duplicated edges: only the minimum weight edge between any two vertices is considered.
double shortestPathCost(const std::vector<std::tuple<int,int,double>>& edges,
                        int numVertices, int source, int target) {
    if (source == target) return 0.0;
    assert(numVertices >= 0);
    assert(source >= 0 && source < numVertices);
    assert(target >= 0 && target < numVertices);

    // Build adjacency: for each vertex, a map from neighbor to minimum edge weight.
    std::vector<std::unordered_map<int, double>> adj(numVertices);
    for (const auto& [u, v, w] : edges) {
        assert(u >= 0 && u < numVertices);
        assert(v >= 0 && v < numVertices);
        assert(w >= 0.0);
        // Only keep the smallest weight for each undirected pair.
        auto it1 = adj[u].find(v);
        if (it1 == adj[u].end() || w < it1->second) adj[u][v] = w;
        auto it2 = adj[v].find(u);
        if (it2 == adj[v].end() || w < it2->second) adj[v][u] = w;
    }

    const double INF = std::numeric_limits<double>::infinity();
    std::vector<double> dist(numVertices, INF);
    dist[source] = 0.0;

    // Min‑heap of (distance, vertex).
    using Pair = std::pair<double, int>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;
    pq.push({0.0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue; // stale entry
        if (u == target) return d; // first time we pop target is the shortest distance

        for (const auto& [v, w] : adj[u]) {
            double nd = d + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.push({nd, v});
            }
        }
    }
    return -1.0; // target unreachable
}

#include <cassert>
#include <vector>
#include <tuple>

// The solution function is declared above, but for the test we include it here implicitly.

int main() {
    // Simple triangle: 0-1 (1), 0-2 (4), 1-2 (2)
    std::vector<std::tuple<int,int,double>> edges1 = {{0,1,1.0}, {0,2,4.0}, {1,2,2.0}};
    assert(shortestPathCost(edges1, 3, 0, 1) == 1.0);
    assert(shortestPathCost(edges1, 3, 0, 2) == 3.0); // via 1
    assert(shortestPathCost(edges1, 3, 1, 2) == 2.0);
    assert(shortestPathCost(edges1, 3, 0, 0) == 0.0);

    // Disconnected graph: 0-1, 2-3
    std::vector<std::tuple<int,int,double>> edges2 = {{0,1,5.0}, {2,3,7.0}};
    assert(shortestPathCost(edges2, 4, 0, 1) == 5.0);
    assert(shortestPathCost(edges2, 4, 0, 2) == -1.0);
    assert(shortestPathCost(edges2, 4, 2, 3) == 7.0);

    // Duplicate edges: 0-1 weight 2 and 5, keep 2
    std::vector<std::tuple<int,int,double>> edges3 = {{0,1,2.0}, {0,1,5.0}};
    assert(shortestPathCost(edges3, 2, 0, 1) == 2.0);

    // Zero weight path: 0-1 (0), 1-2 (0)
    std::vector<std::tuple<int,int,double>> edges4 = {{0,1,0.0}, {1,2,0.0}};
    assert(shortestPathCost(edges4, 3, 0, 2) == 0.0);

    // Larger test: chain 0-1-2-3, weights 1,2,3
    std::vector<std::tuple<int,int,double>> edges5 = {{0,1,1.0}, {1,2,2.0}, {2,3,3.0}};
    assert(shortestPathCost(edges5, 4, 0, 3) == 6.0);
    assert(shortestPathCost(edges5, 4, 1, 3) == 5.0);

    // Single vertex graph, no edges
    std::vector<std::tuple<int,int,double>> edges6;
    assert(shortestPathCost(edges6, 1, 0, 0) == 0.0);
    assert(shortestPathCost(edges6, 1, 0, 0) == 0.0);
}
