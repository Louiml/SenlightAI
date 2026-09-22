Write a C++ function `int countRedundantEdges(int n, const std::vector<std::pair<int,int>>& edges)` that counts the number of redundant edges in an undirected graph. An edge is redundant if it connects two vertices that are already in the same connected component. The graph has vertices labeled from `0` to `n-1`, and the input is a list of undirected edges (each pair `{u,v}` with `0 <= u,v < n`). The function must use a Disjoint Set Union (Union-Find) data structure with path compression and union by rank (or level). The function should return how many edges, when processed in the given order, connect two vertices already in the same component, i.e., would create a cycle if added to a forest formed by the previously accepted edges.
// The problem reduces to detecting cycles while adding edges to a forest using DSU. Initialize `n` parent pointers to `-1` (or self) and ranks (or levels) to 0. For each edge `{u,v}`, perform `find` on both endpoints to get their component leaders. If the leaders are equal, the edge is redundant and increment a counter; otherwise, perform a union operation that attaches the tree with smaller rank under the root of the larger rank, and if ranks are equal, choose one root arbitrarily and increment its rank. Path compression during `find` flattens the tree, making future operations almost constant. Edge cases include self-loops (where `u == v`), which are always redundant because a vertex is already in its own component; isolated vertices not in any edge are irrelevant. Duplicate edges are handled naturally: the first occurrence of a connection is accepted, and any subsequent duplicate is redundant. Time complexity is O(α(n)) amortized per `find` or `union` operation, where α is the inverse Ackermann function, so overall O(m α(n)) for m edges, and space complexity is O(n) for the parent and rank arrays.
#include <vector>
#include <utility>

// Count redundant edges in an undirected graph using Disjoint Set Union.
// Vertices are 0..n-1. An edge is redundant if its endpoints are already connected.
int countRedundantEdges(int n, const std::vector<std::pair<int, int>>& edges) {
    // Parent array; -1 means the node is its own root.
    std::vector<int> parent(n, -1);
    // Rank (level) of each root; used for union by rank.
    std::vector<int> rank(n, 0);
    
    // Recursive find with path compression.
    // Returns the root of the set containing x.
    auto find = [&](auto&& self, int x) -> int {
        if (parent[x] == -1) return x;
        int root = self(self, parent[x]);
        parent[x] = root; // path compression
        return root;
    };
    
    // Union two sets by rank. Assumes a and b are valid vertices.
    auto unite = [&](int a, int b) {
        int rootA = find(find, a);
        int rootB = find(find, b);
        if (rootA == rootB) return; // already connected
        if (rank[rootA] < rank[rootB]) {
            parent[rootA] = rootB;
        } else if (rank[rootA] > rank[rootB]) {
            parent[rootB] = rootA;
        } else {
            parent[rootB] = rootA;
            ++rank[rootA];
        }
    };
    
    int redundant = 0;
    for (const auto& [u, v] : edges) {
        int rootU = find(find, u);
        int rootV = find(find, v);
        if (rootU == rootV) {
            ++redundant;
        } else {
            unite(u, v);
        }
    }
    return redundant;
}
#include <cassert>
#include <vector>
#include <utility>

int countRedundantEdges(int n, const std::vector<std::pair<int, int>>& edges);

int main() {
    // Empty graph
    assert(countRedundantEdges(3, {}) == 0);
    
    // Single edge, no cycle
    assert(countRedundantEdges(2, {{0, 1}}) == 0);
    
    // Two edges forming a cycle (triangle with 3 vertices, but only one extra)
    assert(countRedundantEdges(3, {{0, 1}, {1, 2}, {2, 0}}) == 1);
    
    // Self-loop is always redundant
    assert(countRedundantEdges(4, {{0, 0}}) == 1);
    
    // Duplicate edge is redundant on second occurrence
    assert(countRedundantEdges(4, {{0, 1}, {0, 1}}) == 1);
    
    // Mixed case: a tree plus one extra edge between two already connected vertices
    assert(countRedundantEdges(5, {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}}) == 1);
    
    // Two separate components, no redundant edges in a forest
    assert(countRedundantEdges(6, {{0, 1}, {2, 3}, {4, 5}}) == 0);
    
    // Multiple redundant edges in a fully connected component of 4 vertices
    // Minimal spanning tree has 3 edges, so redundant = total - (n-1) for one component
    assert(countRedundantEdges(4, {{0,1},{1,2},{2,3},{3,0},{0,2}}) == 2);
    
    // Edge connecting two vertices from different components after some unions
    assert(countRedundantEdges(6, {{0,1},{2,3},{1,2},{3,4},{0,4}}) == 1);
    
    // Large-ish stress check: cyclic chain plus self-loop
    assert(countRedundantEdges(100, [&](){
        std::vector<std::pair<int,int>> e;
        for (int i = 0; i < 99; ++i) e.push_back({i, i+1});
        e.push_back({99, 0});
        e.push_back({50, 50});
        return e;
    }()) == 2);
    
    return 0;
}
