// Write a C++ function that takes a non‑negative integer `n` and an `std::vector<std::pair<int,int>>&` list of undirected edges (each edge connects two nodes numbered `0` through `n-1`), and returns a `std::vector<std::vector<int>>` adjacency list where index `i` contains all neighbours of node `i` in ascending order. If an edge is listed twice (parallel edge), it must appear twice in the adjacency list. The input is guaranteed to be valid (all node numbers are in range) and `n ≥ 1`. The function must be `const`‑correct and must not modify the input.
#include <cassert>
#include <vector>

int main() {
    // Example from the snippet (nodes 0..5, 7 edges)
    std::vector<std::pair<int,int>> edges1 = {
        {0,1}, {0,4}, {2,1}, {3,4}, {4,5}, {2,3}, {3,5}
    };
    auto adj1 = buildAdjacencyList(6, edges1);
    assert(adj1[0] == std::vector<int>({1,4}));
    assert(adj1[1] == std::vector<int>({0,2}));
    assert(adj1[2] == std::vector<int>({1,3}));
    assert(adj1[3] == std::vector<int>({2,4,5}));
    assert(adj1[4] == std::vector<int>({0,3,5}));
    assert(adj1[5] == std::vector<int>({3,4}));

    // Self-loop
    std::vector<std::pair<int,int>> edges2 = {{0,0}};
    auto adj2 = buildAdjacencyList(1, edges2);
    assert(adj2[0] == std::vector<int>({0,0}));

    // Duplicate edge and isolated node
    std::vector<std::pair<int,int>> edges3 = {{0,1}, {0,1}, {2,3}};
    auto adj3 = buildAdjacencyList(4, edges3);
    assert(adj3[0] == std::vector<int>({1,1}));
    assert(adj3[1] == std::vector<int>({0,0}));
    assert(adj3[2] == std::vector<int>({3}));
    assert(adj3[3] == std::vector<int>({2}));

    // No edges, multiple isolated nodes
    auto adj4 = buildAdjacencyList(3, {});
    assert(adj4[0].empty() && adj4[1].empty() && adj4[2].empty());

    // Larger random-ish case, order of neighbours must be sorted
    std::vector<std::pair<int,int>> edges5 = {{1,4}, {4,2}, {2,1}, {0,3}};
    auto adj5 = buildAdjacencyList(5, edges5);
    assert(adj5[0] == std::vector<int>({3}));
    assert(adj5[1] == std::vector<int>({2,4}));
    assert(adj5[2] == std::vector<int>({1,4}));
    assert(adj5[3] == std::vector<int>({0}));
    assert(adj5[4] == std::vector<int>({1,2}));
}
#include <vector>
#include <algorithm>

// Build an undirected adjacency list from a list of edges.
// Each neighbour list is sorted in ascending order.
std::vector<std::vector<int>> buildAdjacencyList(
    int n,
    const std::vector<std::pair<int, int>>& edges
) {
    std::vector<std::vector<int>> adj(n);
    for (const auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (auto& list : adj) {
        std::sort(list.begin(), list.end());
    }
    return adj;
}
// The task simply requires building an undirected graph adjacency list from a vector of pairs. For each edge `(u,v)`, push `v` into the list for `u` and push `u` into the list for `v`. After processing all edges, sort each individual list to ensure ascending order. Important edge cases: (1) self‑loops (`u == v`) – the node appears twice in its own list; (2) duplicate edges – they are appended as separate entries; (3) isolated nodes – their list remains empty. Time complexity is \(O(E + \sum_{i} d_i \log d_i)\) where \(d_i\) is the degree of node `i` (worst‑case \(O(E + E \log E)\) after sorting). Space complexity is \(O(V+E)\) for the returned list, ignoring the input vector.
