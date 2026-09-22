Write a C++ function `std::vector<int> shortestPathWithEdges(int n, const std::vector<std::tuple<int,int,int>>& edges, int source, int target)` that returns the shortest path (as a list of vertices from `source` to `target`, inclusive) in an undirected, weighted, connected graph. The graph has `n` vertices (numbered `1` to `n`), and `edges` is a list of tuples `(u, v, weight)` where `weight` is a positive integer. If multiple shortest paths exist, return any one of them. The function must assume that a path always exists between `source` and `target`, and the input graph is connected (no need to handle the disconnected case). Return an empty vector if `source == target`? No, in that case return a vector with just `source` (the path of length 0). The function should not print anything; it should only return the vector of vertex numbers. The weights are such that the shortest path distance fits into a 32-bit signed integer.

The solution uses Dijkstra's algorithm because all edge weights are positive. We maintain a distance array `dist` initialized to a large value (e.g., `INT_MAX/2`), and a predecessor array `parent` to reconstruct the path. We use a priority queue (min-heap) of pairs `(distance, vertex)`. Start by setting `dist[source]=0` and pushing `(0, source)`. While the priority queue is not empty, pop the pair with smallest distance; if the popped distance differs from `dist[current]`, we skip (lazy deletion). Otherwise, for each neighbor `(neighbor, weight)` of `current`, if `dist[current] + weight < dist[neighbor]`, update `dist[neighbor]` and `parent[neighbor] = current`, then push `(dist[neighbor], neighbor)`. After Dijkstra completes, reconstruct the path from `target` backwards using `parent`, then reverse it. Edge case: if `source == target`, the loop is unnecessary; just return `{source}`. The graph is given as undirected, so we add both directions. Time complexity: O((n + m) log n) due to the priority queue, where `m` is the number of edges. Space complexity: O(n + m) for adjacency list and distance/parent arrays. All weights are positive, so Dijkstra works correctly; we do not need to handle negative weights. The graph is connected, so we never encounter unreachable vertices.

#include <vector>
#include <queue>
#include <tuple>
#include <limits>
#include <algorithm>
#include <utility>

// Returns the shortest path from 'source' to 'target' in an undirected weighted graph.
// Graph has 'n' vertices (1..n). Edges are tuples (u, v, weight). Assumes a path exists.
std::vector<int> shortestPathWithEdges(
    int n,
    const std::vector<std::tuple<int, int, int>>& edges,
    int source,
    int target)
{
    // Empty path if target equals source? Actually path length 0: just source.
    if (source == target) {
        return std::vector<int>{source};
    }

    // Build adjacency list: for each vertex, list of (neighbor, weight)
    std::vector<std::vector<std::pair<int, int>>> adj(n + 1);
    for (const auto& [u, v, w] : edges) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    const int INF = std::numeric_limits<int>::max() / 2;
    std::vector<int> dist(n + 1, INF);
    std::vector<int> parent(n + 1, -1);
    dist[source] = 0;

    // Min-heap of (distance, vertex)
    using P = std::pair<int, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // Skip outdated entries
        if (d != dist[u]) continue;

        // Early exit: we can stop when we pop the target, but not mandatory.
        for (const auto& [v, w] : adj[u]) {
            int newDist = dist[u] + w;
            if (newDist < dist[v]) {
                dist[v] = newDist;
                parent[v] = u;
                pq.push({newDist, v});
            }
        }
    }

    // Reconstruct path from target to source
    std::vector<int> path;
    for (int cur = target; cur != -1; cur = parent[cur]) {
        path.push_back(cur);
        if (cur == source) break; // safety
    }
    std::reverse(path.begin(), path.end());
    return path;
}

#include <cassert>
#include <vector>
#include <tuple>

// The solution function is assumed to be defined above (or included).
int main() {
    // Test 1: simple triangle:  1-2 (weight 1), 2-3 (weight 1), 1-3 (weight 10)
    {
        std::vector<std::tuple<int,int,int>> edges = {{1,2,1}, {2,3,1}, {1,3,10}};
        auto path = shortestPathWithEdges(3, edges, 1, 3);
        assert(path == std::vector<int>({1,2,3})); // shortest path is 1-2-3 weight 2
    }

    // Test 2: direct edge only
    {
        std::vector<std::tuple<int,int,int>> edges = {{1,4,5}};
        auto path = shortestPathWithEdges(4, edges, 1, 4);
        assert(path == std::vector<int>({1,4}));
    }

    // Test 3: source equals target
    {
        std::vector<std::tuple<int,int,int>> edges = {{1,2,10}};
        auto path = shortestPathWithEdges(2, edges, 2, 2);
        assert(path == std::vector<int>({2}));
    }

    // Test 4: multiple equal paths – any valid path accepted, check distance? We check path length and endpoints.
    {
        std::vector<std::tuple<int,int,int>> edges = {{1,2,5},{1,3,5},{2,4,5},{3,4,5}};
        auto path = shortestPathWithEdges(4, edges, 1, 4);
        // Two possible shortest paths: 1-2-4 or 1-3-4, both length 3 vertices.
        assert(path.front() == 1 && path.back() == 4);
        assert(path.size() == 3);
        // Check that consecutive vertices are connected with correct weight sum = 10
        // But we don't have a direct map; just check adjacency from given edges.
        // Find which internal vertex (2 or 3) is present.
        if (path == std::vector<int>({1,2,4})) {
            // ok
        } else if (path == std::vector<int>({1,3,4})) {
            // ok
        } else {
            assert(false);
        }
    }

    // Test 5: larger graph with a clear shortest path
    {
        std::vector<std::tuple<int,int,int>> edges = {
            {1,2,2},{1,3,4},{2,3,1},{2,4,7},{3,4,3},{3,5,6},{4,5,1}
        };
        auto path = shortestPathWithEdges(5, edges, 1, 5);
        // Shortest: 1-2-3-4-5 total 2+1+3+1=7 (or 1-3-4-5 total 4+3+1=8, so 1-2-3-4-5 is shorter)
        assert(path.front() == 1 && path.back() == 5);
        // Could be 1,2,3,4,5
        assert(path == std::vector<int>({1,2,3,4,5}));
    }

    // Test 6: line graph with increasing weights
    {
        std::vector<std::tuple<int,int,int>> edges = {{1,2,10},{2,3,10},{3,4,10},{4,5,10}};
        auto path = shortestPathWithEdges(5, edges, 1, 5);
        assert(path == std::vector<int>({1,2,3,4,5}));
    }

    // Test 7: heavy weights and large n (no overflow)
    {
        std::vector<std::tuple<int,int,int>> edges;
        int n = 50;
        for (int i = 1; i < n; ++i) {
            edges.push_back({i, i+1, 1000});
        }
        auto path = shortestPathWithEdges(n, edges, 1, n);
        assert(path.size() == n);
        for (int i = 0; i < n; ++i) assert(path[i] == i + 1);
    }

    // Test 8: star graph with center 1, leaves 2-10, edges weight 1
    {
        std::vector<std::tuple<int,int,int>> edges;
        for (int leaf = 2; leaf <= 10; ++leaf) {
            edges.push_back({1, leaf, 1});
        }
        auto path = shortestPathWithEdges(10, edges, 3, 7);
        // Path must be 3-1-7
        assert(path == std::vector<int>({3,1,7}));
    }

    // Test 9: all edges with weight 1, random connectivity (triangle plus a tail)
    {
        std::vector<std::tuple<int,int,int>> edges = {{1,2,1},{2,3,1},{3,1,1},{3,4,1}};
        auto path = shortestPathWithEdges(4, edges, 4, 2);
        // Shortest: 4-3-2 (weight 2) or 4-3-1-2 (weight 3) so 4-3-2 is unique.
        assert(path == std::vector<int>({4,3,2}));
    }

    // Test 10: duplicate edges with different weights (choose the smaller)
    {
        std::vector<std::tuple<int,int,int>> edges = {{1,2,5},{1,2,1},{2,3,1}};
        auto path = shortestPathWithEdges(3, edges, 1, 3);
        // The edge 1-2 with weight 1 should be used, path 1-2-3 total 2
        assert(path == std::vector<int>({1,2,3}));
    }

    return 0;
}
