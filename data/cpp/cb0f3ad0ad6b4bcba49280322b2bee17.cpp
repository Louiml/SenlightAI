/*
Write a C++ function `vector<int> shortestPathCosts(int n, const vector<tuple<int,int,int>>& edges, int start, int target)` that takes the number of vertices `n`, a list of undirected weighted edges (each tuple contains `u`, `v`, and weight `w`), a starting vertex, and a target vertex. The function should return a vector containing the shortest distance from `start` to every vertex from `0` to `n-1` (in that order). If a vertex is unreachable, its distance should be `-1`. Additionally, the function must ensure that Dijkstra's algorithm is used correctly even with zero-weight edges and self-loops. The graph is 0-indexed and connected components may be isolated.
*/
#include <vector>
#include <queue>
#include <tuple>
#include <limits>
#include <algorithm>

// Compute shortest distances from start to all vertices using Dijkstra.
// Returns a vector where index i holds the shortest distance from start to i,
// or -1 if vertex i is unreachable.
std::vector<int> shortestPathCosts(int n, const std::vector<std::tuple<int,int,int>>& edges, int start, int target) {
    // Build adjacency list: for each vertex, store pairs (neighbor, weight)
    std::vector<std::vector<std::pair<int,int>>> adj(n);
    for (const auto& [u, v, w] : edges) {
        // Undirected: add both directions
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    // Initialize distances with a large sentinel value
    const int INF = std::numeric_limits<int>::max();
    std::vector<int> dist(n, INF);
    dist[start] = 0;

    // Min-heap priority queue storing (distance, vertex)
    using P = std::pair<int,int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0, start});

    // Process until the heap is empty
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // If this distance is stale, skip
        if (d != dist[u]) continue;

        // Explore neighbors (no visited array needed because we skip stale entries)
        for (const auto& [v, w] : adj[u]) {
            int newDist = d + w;
            if (newDist < dist[v]) {
                dist[v] = newDist;
                pq.push({newDist, v});
            }
        }
    }

    // Replace INF with -1 to indicate unreachable, target is unused for distance computation
    // but we keep it as parameter for potential path reconstruction (not needed here).
    // We simply return all distances.
    for (int& d : dist) {
        if (d == INF) d = -1;
    }

    return dist;
}
#include <cassert>
#include <vector>
#include <tuple>

int main() {
    // Test 1: Simple triangle
    {
        std::vector<std::tuple<int,int,int>> edges = {{0,1,2}, {1,2,3}, {0,2,5}};
        std::vector<int> result = shortestPathCosts(3, edges, 0, 2);
        assert(result.size() == 3);
        assert(result[0] == 0);
        assert(result[1] == 2);
        assert(result[2] == 5); // direct is 5, via 1 is 5, tie, but 5 is shortest
    }

    // Test 2: Unreachable vertex
    {
        std::vector<std::tuple<int,int,int>> edges = {{0,1,1}};
        std::vector<int> result = shortestPathCosts(3, edges, 0, 1);
        assert(result[0] == 0);
        assert(result[1] == 1);
        assert(result[2] == -1);
    }

    // Test 3: Zero-weight edges and self-loop
    {
        // self-loop at 0, zero edge 0-1, edge 1-2 weight 5
        std::vector<std::tuple<int,int,int>> edges = {{0,0,1}, {0,1,0}, {1,2,5}};
        std::vector<int> result = shortestPathCosts(3, edges, 0, 2);
        assert(result[0] == 0);
        assert(result[1] == 0); // zero-weight edge
        assert(result[2] == 5);
    }

    // Test 4: Negative weights are not allowed, but zero weights are fine
    // This tests a linear path with increasing weights
    {
        std::vector<std::tuple<int,int,int>> edges = {{0,1,4}, {1,2,3}, {2,3,2}};
        std::vector<int> result = shortestPathCosts(4, edges, 0, 3);
        assert(result[0] == 0);
        assert(result[1] == 4);
        assert(result[2] == 7);
        assert(result[3] == 9);
    }

    // Test 5: Multiple edges between same nodes (parallel edges)
    {
        std::vector<std::tuple<int,int,int>> edges = {{0,1,10}, {0,1,5}};
        std::vector<int> result = shortestPathCosts(2, edges, 0, 1);
        assert(result[0] == 0);
        assert(result[1] == 5);
    }

    // Test 6: Single vertex
    {
        std::vector<std::tuple<int,int,int>> edges = {};
        std::vector<int> result = shortestPathCosts(1, edges, 0, 0);
        assert(result.size() == 1);
        assert(result[0] == 0);
    }

    // Test 7: Disconnected components
    {
        std::vector<std::tuple<int,int,int>> edges = {{0,1,2}, {2,3,4}};
        std::vector<int> result = shortestPathCosts(4, edges, 0, 3);
        assert(result[0] == 0);
        assert(result[1] == 2);
        assert(result[2] == -1);
        assert(result[3] == -1);
    }

    // Test 8: Larger graph with branching
    {
        std::vector<std::tuple<int,int,int>> edges = {
            {0,1,1}, {0,2,4}, {1,2,2}, {1,3,6}, {2,3,3}
        };
        std::vector<int> result = shortestPathCosts(4, edges, 0, 3);
        // Path 0-1-2-3 cost 1+2+3=6, 0-2-3 cost 4+3=7, 0-1-3 cost 1+6=7
        assert(result[0] == 0);
        assert(result[1] == 1);
        assert(result[2] == 3); // 0-1-2 cost 1+2=3 better than direct 4
        assert(result[3] == 6);
    }

    // Test 9: Large distance but within int range
    {
        std::vector<std::tuple<int,int,int>> edges = {{0,1,1000000}, {1,2,1000000}};
        std::vector<int> result = shortestPathCosts(3, edges, 0, 2);
        assert(result[2] == 2000000);
    }

    // Test 10: Star graph with center
    {
        std::vector<std::tuple<int,int,int>> edges = {
            {0,1,8}, {0,2,3}, {0,3,6}, {2,1,1}, {2,3,2}
        };
        std::vector<int> result = shortestPathCosts(4, edges, 0, 3);
        // To 1: 0-2-1 = 3+1=4, to 2: 3, to 3: 0-2-3 = 3+2=5 or direct 6, so 5
        assert(result[0] == 0);
        assert(result[1] == 4);
        assert(result[2] == 3);
        assert(result[3] == 5);
    }

    return 0;
}
// The solution applies Dijkstra's algorithm with a min-heap priority queue. The graph is represented as an adjacency list where each neighbor is stored with its edge weight. The algorithm starts by setting all distances to infinity (or a large sentinel), then sets the start vertex's distance to zero. The priority queue always pops the vertex with the smallest tentative distance. When a vertex is popped, it is marked as visited (processed), so each vertex is finalized only once; this ensures correctness even with zero-weight edges because zero-weight edges do not cause infinite loops since visited vertices are skipped. For each neighbor, if the new tentative distance (current distance + edge weight) is smaller than the recorded distance, update it and push the new distance into the queue. Self-loops and parallel edges are handled naturally: they either improve or are ignored if not beneficial. After the algorithm finishes, unreachable vertices retain the sentinel value, so we convert them to `-1` in the final result. The time complexity is `O((V+E) log V)` due to the heap operations, and the auxiliary space is `O(V+E)` for the adjacency list plus `O(V)` for distances and the heap.
