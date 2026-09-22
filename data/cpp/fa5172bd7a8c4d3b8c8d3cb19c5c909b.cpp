Write a C++ function `int shortestPathCost(int n, const std::vector<std::vector<std::pair<int,int>>>& graph, int source, int target)` that computes the minimum total edge weight from `source` to `target` in a directed graph with `n` vertices (numbered 1 to n) and non-negative edge weights. The graph is given as an adjacency list where `graph[u]` contains pairs `{v, weight}` representing an edge from `u` to `v`. If there is no path, return -1. The graph may contain multiple edges between the same pair of vertices, and edge weights are non-negative integers up to 10^9. The function must handle the case where source equals target (return 0).

// The problem is a classic single-source shortest path on a graph with non-negative weights, best solved with Dijkstra's algorithm. Initialize a distance array `dist` of size n+1 with a large sentinel (e.g., 1e18), set `dist[source] = 0`, and use a min-heap priority queue storing `{negative_distance, vertex}` (or use a custom comparator) to pop the vertex with smallest tentative distance. For each popped vertex, relax all outgoing edges: if `dist[current] + weight < dist[neighbor]`, update `dist[neighbor]` and push the new distance into the queue. Continue until the queue is empty. Because all weights are non-negative, Dijkstra's guarantees correctness. Edge cases: if source == target, return 0 immediately. If the target remains with sentinel distance after the algorithm, there is no path, so return -1. Duplicate edges are naturally handled by relaxation (the minimum will be chosen). Time complexity is O((V+E) log V) where V = n, E = total number of edges, due to heap operations. Space complexity is O(V + E) for the adjacency list and distance array.

#include <vector>
#include <queue>
#include <limits>

// Computes the minimum cost path from source to target in a directed graph
// with non-negative edge weights. Returns -1 if no path exists.
// graph[u] is a vector of pairs {v, weight} meaning edge u -> v with that weight.
int shortestPathCost(int n, const std::vector<std::vector<std::pair<int,int>>>& graph, int source, int target) {
    if (source == target) return 0;

    const long long INF = std::numeric_limits<long long>::max() / 4;
    std::vector<long long> dist(n + 1, INF);
    dist[source] = 0;

    // Min-heap using negative distance trick (pair<distance, vertex>)
    using P = std::pair<long long, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d != dist[u]) continue; // stale entry

        for (const auto& [v, w] : graph[u]) {
            long long nd = dist[u] + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.push({nd, v});
            }
        }
    }

    return (dist[target] == INF) ? -1 : static_cast<int>(dist[target]);
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: simple path
    std::vector<std::vector<std::pair<int,int>>> g1(4);
    g1[1].push_back({2, 5});
    g1[2].push_back({3, 7});
    g1[1].push_back({3, 20});
    assert(shortestPathCost(3, g1, 1, 3) == 12);

    // Test 2: source equals target
    std::vector<std::vector<std::pair<int,int>>> g2(2);
    assert(shortestPathCost(1, g2, 1, 1) == 0);

    // Test 3: no path
    std::vector<std::vector<std::pair<int,int>>> g3(3);
    g3[1].push_back({2, 1});
    assert(shortestPathCost(3, g3, 1, 3) == -1);

    // Test 4: multiple edges, choose minimum
    std::vector<std::vector<std::pair<int,int>>> g4(2);
    g4[1].push_back({2, 10});
    g4[1].push_back({2, 3});
    g4[1].push_back({2, 7});
    assert(shortestPathCost(2, g4, 1, 2) == 3);

    // Test 5: larger graph with alternate paths
    std::vector<std::vector<std::pair<int,int>>> g5(5);
    g5[1].push_back({2, 4});
    g5[1].push_back({3, 2});
    g5[2].push_back({3, 1});
    g5[2].push_back({4, 5});
    g5[3].push_back({4, 8});
    g5[3].push_back({5, 10});
    g5[4].push_back({5, 2});
    // shortest 1->5 = 1->2->4->5 = 4+5+2=11, or 1->3->5=12, so 11
    assert(shortestPathCost(5, g5, 1, 5) == 11);

    // Test 6: zero-weight edges
    std::vector<std::vector<std::pair<int,int>>> g6(3);
    g6[1].push_back({2, 0});
    g6[2].push_back({3, 0});
    assert(shortestPathCost(3, g6, 1, 3) == 0);

    // Test 7: disconnected but target reachable via cycle
    std::vector<std::vector<std::pair<int,int>>> g7(4);
    g7[1].push_back({2, 1});
    g7[2].push_back({1, 1});
    g7[2].push_back({3, 2});
    assert(shortestPathCost(3, g7, 1, 3) == 3);

    // Test 8: large weights
    std::vector<std::vector<std::pair<int,int>>> g8(3);
    g8[1].push_back({2, 1000000000});
    g8[2].push_back({3, 1000000000});
    assert(shortestPathCost(3, g8, 1, 3) == 2000000000);

    return 0;
}
