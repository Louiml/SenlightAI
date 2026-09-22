Implement a C++ function `int disjointSetComponents(int n, const std::vector<std::pair<int, int>>& edges)` that, given a number of labeled nodes `0` through `n-1` and a list of undirected edges, returns the number of connected components in the resulting graph. Use a disjoint-set (union-find) data structure with path compression and union by rank to efficiently process the edges and count distinct components. The function must handle graphs with zero edges, self-loops, duplicate edges, and a large number of nodes/edges (up to 10^5) efficiently.

#include <cassert>
#include <vector>
#include <utility>

// Include the solution function here (or link appropriately)

int main() {
    // No edges -> all nodes isolated
    assert(disjointSetComponents(5, {}) == 5);

    // Single edge connecting two nodes
    assert(disjointSetComponents(4, {{0, 1}}) == 3);

    // Full connected graph on 3 nodes
    assert(disjointSetComponents(3, {{0, 1}, {1, 2}, {0, 2}}) == 1);

    // Duplicate edges and self-loops do not change count
    assert(disjointSetComponents(3, {{0, 0}, {1, 2}, {1, 2}, {2, 1}}) == 2);

    // Multiple components
    assert(disjointSetComponents(6, {{0, 1}, {1, 2}, {3, 4}}) == 3);

    // Large n with a single edge
    assert(disjointSetComponents(100000, {{0, 99999}}) == 99999);

    // Edges forming a cycle
    assert(disjointSetComponents(4, {{0, 1}, {1, 2}, {2, 0}, {3, 3}}) == 2);

    // All nodes in one line
    assert(disjointSetComponents(5, {{0, 1}, {1, 2}, {2, 3}, {3, 4}}) == 1);

    // Empty graph with zero nodes
    assert(disjointSetComponents(0, {}) == 0);

    // Disconnected with many edges on one component
    assert(disjointSetComponents(5, {{0, 1}, {1, 2}, {2, 0}, {3, 4}}) == 2);

    return 0;
}

#include <vector>

// Count connected components in an undirected graph using disjoint-set with path compression and union by rank.
int disjointSetComponents(int n, const std::vector<std::pair<int, int>>& edges) {
    std::vector<int> parent(n);
    std::vector<int> rank(n, 0);
    for (int i = 0; i < n; ++i) parent[i] = i;

    // Find with path compression
    auto find = [&](int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    };

    int components = n;
    for (auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        int root_u = find(u);
        int root_v = find(v);
        if (root_u == root_v) continue; // self-loop or already connected
        // Union by rank
        if (rank[root_u] < rank[root_v]) {
            parent[root_u] = root_v;
        } else if (rank[root_u] > rank[root_v]) {
            parent[root_v] = root_u;
        } else {
            parent[root_v] = root_u;
            rank[root_u]++;
        }
        components--;
    }
    return components;
}

// The solution uses a disjoint-set data structure to group nodes into connected components. Initialize each node as its own parent with rank 0. For each edge `(u, v)`, perform union operations: find the roots of both nodes; if they are different, merge the smaller-rank tree under the larger-rank tree, incrementing rank on ties. After processing all edges, count the number of distinct roots among all nodes (i.e., nodes where `parent[i] == i`). Path compression in `find` flattens the tree structure, reducing future operations; union by rank keeps trees shallow. Edge cases: self-loops and duplicate edges do not change the component count, so they are safely ignored after union (the `root1 == root2` check returns early). The number of components is `n - (number of successful unions)`. Time complexity: O((n + m) α(n)) where α is the inverse Ackermann function (nearly constant), and space complexity O(n) for the parent and rank arrays. For worst-case input with no edges, all nodes are isolated, yielding `n` components.
