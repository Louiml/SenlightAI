/*
Write a C++ function `std::vector<int> shortestPathsFromSource(int N, const std::vector<std::vector<std::pair<int,int>>>& adj, int source)` that, given a directed weighted graph with `N` vertices (numbered 1 to `N`), an adjacency list where each entry is a list of `{neighbor, weight}` pairs (weights are non‑negative integers), and a starting vertex `source`, returns a vector of length `N+1` (indexing from 1) where each position contains the shortest distance from `source` to that vertex, or `-1` if the vertex is unreachable. The graph may contain parallel edges and self‑loops, and vertex labels are contiguous from 1 to `N`. Use Dijkstra’s algorithm with a priority queue (min‑heap). Ensure the function works correctly for the case where `source` itself has distance 0, and that distances can be up to \(2 \times 10^9\) (use a large sentinel like `INT_MAX` for “infinity”). The function must not modify the input graph.
*/
#include <vector>
#include <queue>
#include <limits>
#include <utility>

// Returns shortest distances from source to all vertices (1-indexed). -1 if unreachable.
std::vector<int> shortestPathsFromSource(
    int N,
    const std::vector<std::vector<std::pair<int, int>>>& adj,
    int source
) {
    const int INF = std::numeric_limits<int>::max() / 2; // safe large value
    std::vector<int> dist(N + 1, INF);
    dist[source] = 0;

    // Min-heap: store {distance, node}. Use greater to make it a min-priority queue.
    std::priority_queue<
        std::pair<int, int>,
        std::vector<std::pair<int, int>>,
        std::greater<std::pair<int, int>>
    > pq;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [currentDist, u] = pq.top();
        pq.pop();

        if (currentDist > dist[u]) continue; // stale entry

        for (const auto& [v, w] : adj[u]) {
            int newDist = currentDist + w;
            if (newDist < dist[v]) {
                dist[v] = newDist;
                pq.push({newDist, v});
            }
        }
    }

    // Replace INF with -1 for unreachable vertices.
    for (int i = 1; i <= N; ++i) {
        if (dist[i] == INF) dist[i] = -1;
    }
    return dist;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be declared above.
int main() {
    // Test 1: Simple graph from example
    // N=4, edges: (1,2,2), (1,3,5), (2,3,1), (3,4,3), source=1
    std::vector<std::vector<std::pair<int,int>>> adj1(5);
    adj1[1].push_back({2, 2});
    adj1[1].push_back({3, 5});
    adj1[2].push_back({3, 1});
    adj1[3].push_back({4, 3});
    std::vector<int> res1 = shortestPathsFromSource(4, adj1, 1);
    assert(res1[1] == 0);
    assert(res1[2] == 2);
    assert(res1[3] == 3); // via 1->2->3
    assert(res1[4] == 6); // via 1->2->3->4
    assert(res1.size() == 5);

    // Test 2: Unreachable vertex
    std::vector<std::vector<std::pair<int,int>>> adj2(3);
    adj2[1].push_back({2, 10});
    std::vector<int> res2 = shortestPathsFromSource(3, adj2, 1);
    assert(res2[1] == 0);
    assert(res2[2] == 10);
    assert(res2[3] == -1);

    // Test 3: Self-loop and parallel edges
    std::vector<std::vector<std::pair<int,int>>> adj3(3);
    adj3[1].push_back({1, 5}); // self-loop
    adj3[1].push_back({2, 7});
    adj3[1].push_back({2, 3}); // parallel edge
    adj3[2].push_back({2, 1}); // self-loop on 2
    std::vector<int> res3 = shortestPathsFromSource(2, adj3, 1);
    assert(res3[1] == 0);
    assert(res3[2] == 3); // min(7,3)

    // Test 4: Single vertex, no edges
    std::vector<std::vector<std::pair<int,int>>> adj4(2);
    std::vector<int> res4 = shortestPathsFromSource(1, adj4, 1);
    assert(res4[1] == 0);
    assert(res4.size() == 2);

    // Test 5: Larger distances
    std::vector<std::vector<std::pair<int,int>>> adj5(3);
    adj5[1].push_back({2, 1000000000});
    adj5[2].push_back({3, 1000000000});
    std::vector<int> res5 = shortestPathsFromSource(3, adj5, 1);
    assert(res5[1] == 0);
    assert(res5[2] == 1000000000);
    assert(res5[3] == 2000000000);

    // Test 6: Disconnected graph with multiple components
    std::vector<std::vector<std::pair<int,int>>> adj6(5);
    adj6[1].push_back({2, 1});
    adj6[3].push_back({4, 2});
    std::vector<int> res6 = shortestPathsFromSource(4, adj6, 1);
    assert(res6[1] == 0);
    assert(res6[2] == 1);
    assert(res6[3] == -1);
    assert(res6[4] == -1);

    // Test 7: Source has no outgoing edges but others do
    std::vector<std::vector<std::pair<int,int>>> adj7(3);
    adj7[2].push_back({3, 5});
    std::vector<int> res7 = shortestPathsFromSource(2, adj7, 2);
    assert(res7[1] == -1);
    assert(res7[2] == 0);
    assert(res7[3] == 5);

    // Test 8: All vertices reachable with zero-weight edges
    std::vector<std::vector<std::pair<int,int>>> adj8(4);
    adj8[1].push_back({2, 0});
    adj8[2].push_back({3, 0});
    adj8[3].push_back({4, 0});
    std::vector<int> res8 = shortestPathsFromSource(4, adj8, 1);
    assert(res8[1] == 0);
    assert(res8[2] == 0);
    assert(res8[3] == 0);
    assert(res8[4] == 0);

    // Test 9: Graph with cycles (non-negative weights)
    std::vector<std::vector<std::pair<int,int>>> adj9(4);
    adj9[1].push_back({2, 1});
    adj9[2].push_back({1, 1});
    adj9[2].push_back({3, 2});
    adj9[3].push_back({2, 0}); // cycle back
    std::vector<int> res9 = shortestPathsFromSource(3, adj9, 1);
    assert(res9[1] == 0);
    assert(res9[2] == 1);
    assert(res9[3] == 3);

    return 0;
}
// The problem is a classic single‑source shortest path on a weighted directed graph with non‑negative edge weights. Dijkstra’s algorithm is appropriate. Initialize a distance vector `dist` of size `N+1` with `INT_MAX` (or a large constant like `2e9`), set `dist[source] = 0`, and push `{0, source}` into a priority queue that stores pairs `{distance, node}` but in a min‑heap fashion (by negating the distance if using a default max‑heap). Repeatedly pop the node with the smallest tentative distance; if the popped distance is greater than the stored `dist[node]`, skip (this is the standard lazy deletion). For each neighbor `v` with edge weight `w`, if `dist[u] + w < dist[v]`, update `dist[v]` and push the new pair. After processing, replace any remaining `INT_MAX` values with `-1` to indicate unreachable. Edge cases: the graph may have zero or many edges; self‑loops are handled naturally because `dist[u] + w` will never be smaller than `dist[u]` when `w ≥ 0`; parallel edges are handled because the algorithm takes the minimum over all. Time complexity is \(O((V+E) \log V)\) due to the priority queue, and space complexity is \(O(V+E)\) for the adjacency list and distance array.
