// Write a C++ function named `countComponentsAfterUnion` that takes an integer `n` representing the total number of nodes (labeled from 1 to `n`) and a vector of integer pairs `edges`, where each pair `(u, v)` indicates an undirected connection between nodes `u` and `v`. The function should apply union-by-size (as defined in the provided DisjointSet class) to process all edges in order, and then return the number of disjoint connected components in the graph after all unions are performed. For example, if `n = 7` and edges are `{{1,2},{2,3},{4,5},{6,7},{5,6},{3,7}}`, then after all unions, nodes 1,2,3,7 are connected and nodes 4,5,6 are connected, resulting in 2 components. The function must handle cases with no edges (all nodes isolated), self-loops (u == v), duplicate edges, and nodes that never appear in any edge. Use 1-based indexing and assume `n >= 1`.
// The problem reduces to counting connected components in an undirected graph using a disjoint-set (union-find) data structure with union by size and path compression. Start by initializing a DisjointSet instance with size `n`. For each edge `(u, v)`, call `unionBySize(u, v)` which merges the sets containing these nodes if they are different. After processing all edges, the number of components is simply the number of distinct parent roots among all nodes 1 through `n`. Since union-by-size attaches the smaller tree under the larger tree, and path compression keeps trees shallow, each union and find is nearly constant time on average (inverse Ackermann). To count components, iterate over all nodes, call `findParent(i)` for each, and insert into a set (or count unique roots using an array of booleans). Alternatively, you can start with `components = n` and decrement on each successful union (when `findParent(u) != findParent(v)`), which is more efficient and simpler. Edge cases: self-loops do nothing (already same set), duplicate edges also do nothing after first union, isolated nodes remain separate. Time complexity is O(n + m * α(n)) where m is the number of edges and α is the inverse Ackermann function (practically constant). Space complexity is O(n) for the DisjointSet arrays.
#include <vector>

// DisjointSet class as provided but kept internal to the solution.
class DisjointSet {
public:
    std::vector<int> rank, parent, size;
    DisjointSet(int n) {
        rank.resize(n + 1, 0);
        parent.resize(n + 1);
        size.resize(n + 1, 1);
        for (int i = 0; i <= n; ++i) parent[i] = i;
    }
    int findParent(int ele) {
        if (ele == parent[ele]) return ele;
        return parent[ele] = findParent(parent[ele]);
    }
    void unionBySize(int u, int v) {
        int ulp_u = findParent(u);
        int ulp_v = findParent(v);
        if (ulp_u == ulp_v) return;
        if (size[ulp_u] < size[ulp_v]) {
            parent[ulp_u] = ulp_v;
            size[ulp_v] += size[ulp_u];
        } else {
            parent[ulp_v] = ulp_u;
            size[ulp_u] += size[ulp_v];
        }
    }
};

// Returns the number of connected components after applying union-by-size on all edges.
int countComponentsAfterUnion(int n, const std::vector<std::pair<int, int>>& edges) {
    DisjointSet ds(n);
    int components = n;
    for (const auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        if (ds.findParent(u) != ds.findParent(v)) {
            ds.unionBySize(u, v);
            --components;
        }
    }
    return components;
}
#include <cassert>
#include <vector>
int countComponentsAfterUnion(int n, const std::vector<std::pair<int, int>>& edges);

int main() {
    // Example from the prompt: n=7, edges create two components.
    assert(countComponentsAfterUnion(7, {{1,2},{2,3},{4,5},{6,7},{5,6},{3,7}}) == 2);

    // No edges: each node is its own component.
    assert(countComponentsAfterUnion(5, {}) == 5);

    // Single edge connecting two nodes: 3 nodes total, one edge.
    assert(countComponentsAfterUnion(3, {{1,2}}) == 2);

    // Self-loop does nothing.
    assert(countComponentsAfterUnion(4, {{2,2}}) == 4);

    // Duplicate edges do not change the component count.
    assert(countComponentsAfterUnion(4, {{1,2},{2,1},{1,2}}) == 3);

    // Chain of all nodes: fully connected graph -> one component.
    assert(countComponentsAfterUnion(6, {{1,2},{2,3},{3,4},{4,5},{5,6}}) == 1);

    // Nodes that never appear in edges remain isolated.
    assert(countComponentsAfterUnion(10, {{2,3},{3,4}}) == 8);

    // Large n with no edges.
    assert(countComponentsAfterUnion(1000, {}) == 1000);
}
