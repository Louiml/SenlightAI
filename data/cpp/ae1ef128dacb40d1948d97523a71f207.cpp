/*
Given an undirected connected graph with `n` vertices (numbered 1 to `n`) and `m` weighted edges, write a C++ function `long long minimumSpanningTreeWeight(int n, const std::vector<std::array<int,3>>& edges)` that returns the total weight of a minimum spanning tree (MST). Each edge in the input is given as `{weight, u, v}` where `u` and `v` are endpoint vertices and `weight` is a positive integer. The graph is guaranteed to be connected, so an MST always exists. The function must handle the case where `n` can be as large as 300,000 and `m` up to `3e5`, with weights up to `1e9`. Note that the input graph may contain multiple edges between the same pair of vertices (parallel edges) and self-loops; the function must correctly process them (self-loops should never be part of an MST).

##
*/
#include<vector>
#include<array>
#include<algorithm>

struct DSU {
    std::vector<int> parent, rank, size;
    int components;
    
    DSU(int n) : parent(n+1), rank(n+1, 0), size(n+1, 1), components(n) {
        for (int i = 1; i <= n; ++i) parent[i] = i;
    }
    
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    
    bool same(int a, int b) {
        return find(a) == find(b);
    }
    
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (rank[a] < rank[b]) std::swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        if (rank[a] == rank[b]) rank[a]++;
        --components;
        return true;
    }
    
    int getSize(int x) {
        return size[find(x)];
    }
    
    int countComponents() const {
        return components;
    }
};

long long minimumSpanningTreeWeight(int n, const std::vector<std::array<int,3>>& edges) {
    std::vector<std::array<int,3>> sortedEdges = edges;
    std::sort(sortedEdges.begin(), sortedEdges.end(), 
              [](const std::array<int,3>& a, const std::array<int,3>& b) {
                  return a[0] < b[0];
              });
    
    DSU dsu(n);
    long long totalWeight = 0;
    for (const auto& edge : sortedEdges) {
        int weight = edge[0];
        int u = edge[1];
        int v = edge[2];
        if (dsu.unite(u, v)) {
            totalWeight += weight;
        }
    }
    return totalWeight;
}

##
#include<assert.h>
#include<vector>
#include<array>

// Assume the solution function is already declared above.

int main() {
    // Basic test: 3 vertices, triangle with weights 1,2,3 -> MST = 1+2 = 3
    {
        int n = 3;
        std::vector<std::array<int,3>> edges = {{1,1,2}, {2,2,3}, {3,1,3}};
        assert(minimumSpanningTreeWeight(n, edges) == 3);
    }
    
    // Test with parallel edges: pick smallest
    {
        int n = 2;
        std::vector<std::array<int,3>> edges = {{5,1,2}, {3,1,2}, {7,1,2}};
        assert(minimumSpanningTreeWeight(n, edges) == 3);
    }
    
    // Test with self-loops (should be ignored)
    {
        int n = 2;
        std::vector<std::array<int,3>> edges = {{1,1,1}, {5,1,2}, {2,2,2}};
        assert(minimumSpanningTreeWeight(n, edges) == 5);
    }
    
    // Test with 4 vertices chain: 1-2 (10), 2-3 (20), 3-4 (30), plus an extra edge 1-4 (100)
    {
        int n = 4;
        std::vector<std::array<int,3>> edges = {{10,1,2}, {20,2,3}, {30,3,4}, {100,1,4}};
        assert(minimumSpanningTreeWeight(n, edges) == 60);
    }
    
    // Test with larger weights and negative not allowed
    {
        int n = 5;
        std::vector<std::array<int,3>> edges = {{1000000000,1,2}, {1,2,3}, {2,3,4}, {3,4,5}, {4,5,1}};
        // MST: 1 (2-3), 2 (3-4), 3 (4-5), 4 (5-1) → total 10
        assert(minimumSpanningTreeWeight(n, edges) == 10);
    }
    
    // Single vertex graph with self-loop
    {
        int n = 1;
        std::vector<std::array<int,3>> edges = {{7,1,1}};
        assert(minimumSpanningTreeWeight(n, edges) == 0);
    }
    
    // Two vertices single edge
    {
        int n = 2;
        std::vector<std::array<int,3>> edges = {{42,1,2}};
        assert(minimumSpanningTreeWeight(n, edges) == 42);
    }
    
    // Complex: 6 vertices, many edges, expect MST = 1+2+3+4+5 = 15
    {
        int n = 6;
        std::vector<std::array<int,3>> edges = {
            {1,1,2}, {2,2,3}, {3,3,4}, {4,4,5}, {5,5,6},
            {100,1,6}, {99,2,6}, {98,3,6}, {97,4,6}, {96,5,1}
        };
        assert(minimumSpanningTreeWeight(n, edges) == 15);
    }
    
    // Graph with only self-loops (connected? not really but handle gracefully)
    {
        int n = 3;
        std::vector<std::array<int,3>> edges = {{1,1,1}, {2,2,2}, {3,3,3}};
        // Not connected, but assume input is connected; still function returns 0 for no valid edges
        assert(minimumSpanningTreeWeight(n, edges) == 0);
    }
    
    // Duplicate edges with identical weight
    {
        int n = 3;
        std::vector<std::array<int,3>> edges = {{5,1,2}, {5,2,3}, {5,1,3}};
        assert(minimumSpanningTreeWeight(n, edges) == 10);
    }
    
    return 0;
}
// This is a classic Minimum Spanning Tree problem solved using Kruskal's algorithm with a Disjoint Set Union (DSU) data structure. First, sort all edges by their weight in non-decreasing order. Then, initialize a DSU with `n` vertices. Iterate through the sorted edges: for each edge `{w, u, v}`, check if `u` and `v` are in different connected components using DSU's `find` operation. If they are different, add the edge's weight `w` to the total answer and merge the two components. Continue until all edges are processed. Since the graph is connected, the MST will have exactly `n-1` edges, and the algorithm naturally accumulates the minimum total weight.
//
// Edge cases: self-loops (u == v) are automatically skipped because `same(u,v)` returns true. Parallel edges are handled correctly because only the smallest weight edge that connects two different components will be chosen first (after sorting). The graph being connected guarantees an MST exists; no need to check for an incomplete tree. Complexity: sorting takes `O(m log m)` time, each DSU operation is nearly `O(α(n))` (inverse Ackermann, essentially constant), so the overall time is `O(m log m)` and space is `O(n + m)`.
//
// ##
