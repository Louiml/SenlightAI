// Given an undirected, connected graph with `N` nodes (numbered from 1 to N) and `M` weighted edges, write a C++ function `long long minSpanningTreeCost(int n, const std::vector<std::pair<int, std::pair<int, long long>>>& edges)` that returns the total weight of a minimum spanning tree (MST) using Prim’s algorithm. The graph may have parallel edges (multiple edges between the same pair of nodes) and edge weights can be negative, zero, or positive. The graph is guaranteed to be connected. The function should accept an integer `n` (number of nodes) and a vector where each element is `(u, (v, weight))` representing an undirected edge between nodes `u` and `v` (both 1-indexed) with the given weight. The function must build an adjacency list internally and apply Prim’s algorithm starting from node 1, returning the sum of weights of the MST. If the input is invalid (e.g., n=0 or no nodes), return 0.
Prim’s algorithm grows a tree one vertex at a time, always adding the cheapest edge that connects a vertex already in the tree to a vertex outside it. We initialize a min-heap (priority queue) with the pair `(0, 1)` to represent starting at node 1 with zero cost. We maintain a boolean `visited` array to track which nodes have been included. While the heap is not empty, we pop the smallest-weight edge. If the destination node is already visited, we skip it (this handles cycles). Otherwise, we add its weight to the total cost, mark it visited, and push all adjacent edges from that node that lead to unvisited nodes. Because the graph is connected, the loop will visit all nodes. For `V` nodes and `E` edges, building the adjacency list takes `O(E)`. The heap operations are `O(E log V)` in the worst case. Space complexity is `O(V + E)` for the adjacency list and visited array. Edge cases include parallel edges (the heap naturally picks the cheapest), negative weights (allowed, but Prim still works as long as the graph is connected), and zero-weight edges. Starting at node 1 is arbitrary but works for any connected graph. If the graph is not connected, the function would return a partial cost, but the problem guarantees connectivity.
#include <vector>
#include <queue>
#include <functional>
#include <utility>

// Returns the total weight of a Minimum Spanning Tree using Prim's algorithm.
// n: number of nodes (1-indexed)
// edges: vector of tuples (u, v, weight) where u and v are 1-indexed nodes.
// Graph is guaranteed to be connected.
long long minSpanningTreeCost(int n, const std::vector<std::pair<int, std::pair<int, long long>>>& edges) {
    if (n <= 0) return 0;

    // Build adjacency list: adj[node] = list of (weight, neighbor)
    std::vector<std::vector<std::pair<long long, int>>> adj(n + 1);
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second.first;
        long long w = e.second.second;
        adj[u].push_back({w, v});
        adj[v].push_back({w, u});
    }

    // Min-heap: (weight, node)
    std::priority_queue<std::pair<long long, int>, std::vector<std::pair<long long, int>>, std::greater<>> pq;
    std::vector<bool> visited(n + 1, false);

    long long totalCost = 0;
    pq.push({0, 1}); // start from node 1

    while (!pq.empty()) {
        auto [weight, node] = pq.top();
        pq.pop();

        if (visited[node]) continue; // already in tree
        visited[node] = true;
        totalCost += weight;

        for (const auto& [w, neighbor] : adj[node]) {
            if (!visited[neighbor]) {
                pq.push({w, neighbor});
            }
        }
    }

    return totalCost;
}
#include <cassert>
#include <vector>
#include <utility>

// Solution function declaration (as above) would be here.

int main() {
    // Test 1: Simple triangle
    std::vector<std::pair<int, std::pair<int, long long>>> edges1 = {
        {1, {2, 1}},
        {2, {3, 2}},
        {1, {3, 3}}
    };
    assert(minSpanningTreeCost(3, edges1) == 3); // edges 1-2 (1) + 2-3 (2)

    // Test 2: 4-node chain
    std::vector<std::pair<int, std::pair<int, long long>>> edges2 = {
        {1, {2, 5}},
        {2, {3, 3}},
        {3, {4, 4}},
        {1, {4, 10}}
    };
    assert(minSpanningTreeCost(4, edges2) == 12); // 5+3+4

    // Test 3: Negative weights
    std::vector<std::pair<int, std::pair<int, long long>>> edges3 = {
        {1, {2, -5}},
        {2, {3, 2}},
        {1, {3, -1}}
    };
    assert(minSpanningTreeCost(3, edges3) == -6); // choose -5 and -1

    // Test 4: Parallel edges (choose cheapest)
    std::vector<std::pair<int, std::pair<int, long long>>> edges4 = {
        {1, {2, 10}},
        {1, {2, 1}},
        {2, {3, 5}}
    };
    assert(minSpanningTreeCost(3, edges4) == 6); // 1 + 5

    // Test 5: Single node (no edges)
    std::vector<std::pair<int, std::pair<int, long long>>> edges5;
    assert(minSpanningTreeCost(1, edges5) == 0);

    // Test 6: Zero weight edges
    std::vector<std::pair<int, std::pair<int, long long>>> edges6 = {
        {1, {2, 0}},
        {2, {3, 0}},
        {1, {3, 5}}
    };
    assert(minSpanningTreeCost(3, edges6) == 0);

    // Test 7: Larger graph with multiple edges
    std::vector<std::pair<int, std::pair<int, long long>>> edges7 = {
        {1, {2, 4}},
        {1, {3, 1}},
        {2, {3, 2}},
        {2, {4, 3}},
        {3, {4, 5}},
        {4, {1, 8}}
    };
    assert(minSpanningTreeCost(4, edges7) == 6); // 1 + 2 + 3

    // Test 8: n=0 (invalid, should return 0)
    assert(minSpanningTreeCost(0, edges1) == 0);
}
