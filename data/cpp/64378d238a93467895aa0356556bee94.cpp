Write a C++ function named `hasCycle` that takes the number of nodes `n` and a vector of undirected edges (each edge represented as a pair of integers `(u, v)`, where `0 <= u, v < n`), and returns `true` if the graph contains at least one cycle, and `false` otherwise. The graph is simple (no self-loops, no duplicate edges). Your function must use the Disjoint Set Union (DSU) with union by size and path compression to detect cycles efficiently. Do not worry about whether the graph is connected; handle all components independently. The function signature should be `bool hasCycle(int n, const std::vector<std::pair<int, int>>& edges)`. You are free to use internal helper functions or a local struct/class for the DSU.

#include <cassert>
#include <vector>
#include <utility>

// Include the solution code here (the DSU class and hasCycle function).

int main() {
    // No edges: no cycle.
    assert(hasCycle(3, {}) == false);

    // Single node, no edges.
    assert(hasCycle(1, {}) == false);

    // Two nodes, one edge: no cycle.
    assert(hasCycle(2, {{0, 1}}) == false);

    // Three nodes forming a simple path: no cycle.
    assert(hasCycle(3, {{0, 1}, {1, 2}}) == false);

    // Triangle: cycle exists.
    assert(hasCycle(3, {{0, 1}, {1, 2}, {2, 0}}) == true);

    // Connected cyclic graph with extra branches.
    assert(hasCycle(5, {{0, 1}, {1, 2}, {2, 0}, {3, 4}}) == true);

    // Two separate components, one component has a cycle.
    assert(hasCycle(5, {{0, 1}, {1, 2}, {2, 0}, {3, 4}}) == true);

    // Large n with only a single edge: no cycle.
    assert(hasCycle(100000, {{0, 99999}}) == false);

    // Multiple edges that eventually form a cycle deep in the list.
    assert(hasCycle(4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}}) == true);

    // Disconnected with no cycles.
    assert(hasCycle(6, {{0, 1}, {2, 3}, {4, 5}}) == false);

    return 0;
}

#include <vector>
#include <utility>

// Disjoint Set Union (Union-Find) with path compression and union by size.
class DSU {
    std::vector<int> parent;
    std::vector<int> size;

public:
    explicit DSU(int n) : parent(n, -1), size(n, 1) {}

    int find(int x) {
        if (parent[x] == -1)
            return x;
        return parent[x] = find(parent[x]);
    }

    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY)
            return;
        if (size[rootX] < size[rootY]) {
            parent[rootX] = rootY;
            size[rootY] += size[rootX];
        } else {
            parent[rootY] = rootX;
            size[rootX] += size[rootY];
        }
    }
};

// Returns true if the undirected graph contains at least one cycle, false otherwise.
bool hasCycle(int n, const std::vector<std::pair<int, int>>& edges) {
    DSU dsu(n);
    for (const auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        int leaderU = dsu.find(u);
        int leaderV = dsu.find(v);
        if (leaderU == leaderV)
            return true;
        dsu.unite(leaderU, leaderV);
    }
    return false;
}

// The core idea is to process each edge sequentially. Initially, each node is its own component. For each edge `(u, v)`, we find the leaders (representatives) of the components containing `u` and `v` using path compression to flatten the tree. If the two leaders are the same, then `u` and `v` are already in the same connected component, meaning adding this edge would form a cycle; we return `true` immediately. If the leaders are different, we union the two components by attaching the smaller tree's root to the larger tree's root (union by size) to keep the DSU trees shallow. Since we return as soon as a cycle is detected, we only need to process up to the first cycle-causing edge. If no cycle is found after processing all edges, return `false`. Edge cases: an empty edge list or `n <= 1` always returns `false` because no cycle can exist. Self-loops are not present per constraints, but if they were, the same logic would catch them since `u == v` would give the same leader. Time complexity is O(α(n) * E) where α is the inverse Ackermann function (practically constant) and E is the number of edges; space complexity is O(n) for the parent and size arrays.
