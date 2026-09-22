Write a C++ function `int minimumSpanningTreeWeight(int V, const std::vector<std::tuple<int,int,int>>& edges)` that computes the total weight of a Minimum Spanning Tree (MST) for an undirected, connected, weighted graph with vertices labeled from 1 to V. The input is the number of vertices V and a list of edges, where each tuple is `{weight, u, v}` representing an undirected edge of that weight between vertices u and v. The graph is guaranteed to be connected, so an MST always exists. If multiple edges have the same weight or if vertices are 1-indexed, handle them normally. Return the sum of the weights of the edges in the MST. The weights can be negative or zero, but the sum fits within a 32-bit integer.
This is the classic Minimum Spanning Tree problem, solved efficiently using Kruskal's algorithm with a Disjoint Set Union (DSU) or Union-Find data structure. The approach sorts all edges by weight in ascending order (or uses a min-heap) and processes them one by one. For each edge, we check whether its two endpoints are already in the same connected component using `findParent`. If they are not, we add the edge's weight to the total answer and union the two components. If they are already connected, we skip the edge because adding it would create a cycle. The algorithm continues until we have processed all edges, and because the graph is connected, the result is the MST weight. Important edge cases: (1) vertices are 1-indexed, so the DSU array must be size V+1 and initialize parent[i]=i for i from 1 to V; (2) negative weights are allowed and should be added normally; (3) parallel edges of different weights may exist — the algorithm naturally picks the smallest weight edges first due to sorting; (4) the graph is guaranteed connected so no need to test for isolated vertices, but if the MST would not be formed (not possible here), the code would still return a partial sum. Time complexity is O(E log E) for sorting, and each DSU operation is nearly O(1) with path compression, so overall O(E log E). Space complexity is O(V + E) for the parent array and the edge list.
#include <vector>
#include <tuple>
#include <algorithm>

// Disjoint Set Union (DSU) helper structure for Kruskal's algorithm
class DSU {
    std::vector<int> parent;
public:
    explicit DSU(int n) {
        parent.resize(n + 1); // vertices are 1-indexed
        for (int i = 1; i <= n; ++i) {
            parent[i] = i;
        }
    }

    int findParent(int x) {
        if (parent[x] == x) return x;
        return parent[x] = findParent(parent[x]); // path compression
    }

    void unite(int x, int y) {
        int px = findParent(x);
        int py = findParent(y);
        if (px != py) {
            parent[px] = py;
        }
    }

    bool sameParent(int x, int y) {
        return findParent(x) == findParent(y);
    }
};

// Computes the total weight of the Minimum Spanning Tree using Kruskal's algorithm.
// V: number of vertices (1..V), edges: vector of (weight, u, v)
int minimumSpanningTreeWeight(int V, const std::vector<std::tuple<int,int,int>>& edges) {
    DSU dsu(V);
    // Copy edges and sort by weight descending (we'll process from smallest via reverse iteration)
    auto sortedEdges = edges;
    std::sort(sortedEdges.begin(), sortedEdges.end(), 
              [](const std::tuple<int,int,int>& a, const std::tuple<int,int,int>& b) {
                  return std::get<0>(a) < std::get<0>(b); // ascending weight
              });

    int totalWeight = 0;
    for (const auto& [weight, u, v] : sortedEdges) {
        if (!dsu.sameParent(u, v)) {
            dsu.unite(u, v);
            totalWeight += weight;
        }
    }
    return totalWeight;
}
#include <cassert>
#include <vector>
#include <tuple>

// Include the solution function here (or link it)

int main() {
    // Test 1: Simple triangle graph (MST picks two edges)
    {
        int V = 3;
        std::vector<std::tuple<int,int,int>> edges = {
            {1, 1, 2}, {2, 2, 3}, {3, 1, 3}
        };
        assert(minimumSpanningTreeWeight(V, edges) == 3);
    }

    // Test 2: Larger graph with unique weights
    {
        int V = 4;
        std::vector<std::tuple<int,int,int>> edges = {
            {10, 1, 2}, {6, 2, 3}, {5, 3, 4}, {15, 1, 4}, {8, 1, 3}
        };
        // MST: 5+6+8 = 19
        assert(minimumSpanningTreeWeight(V, edges) == 19);
    }

    // Test 3: Negative weight edges
    {
        int V = 3;
        std::vector<std::tuple<int,int,int>> edges = {
            {-5, 1, 2}, {-3, 2, 3}, {4, 1, 3}
        };
        // MST: -5 + (-3) = -8
        assert(minimumSpanningTreeWeight(V, edges) == -8);
    }

    // Test 4: Graph with parallel edges (choose smallest)
    {
        int V = 3;
        std::vector<std::tuple<int,int,int>> edges = {
            {5, 1, 2}, {3, 1, 2}, {2, 2, 3}, {6, 1, 3}
        };
        // MST: 3 (from 1-2) + 2 (from 2-3) = 5
        assert(minimumSpanningTreeWeight(V, edges) == 5);
    }

    // Test 5: Single edge graph (V=2, one edge)
    {
        int V = 2;
        std::vector<std::tuple<int,int,int>> edges = {{7, 1, 2}};
        assert(minimumSpanningTreeWeight(V, edges) == 7);
    }

    // Test 6: All zero weights
    {
        int V = 4;
        std::vector<std::tuple<int,int,int>> edges = {
            {0, 1, 2}, {0, 2, 3}, {0, 3, 4}, {0, 1, 4}
        };
        assert(minimumSpanningTreeWeight(V, edges) == 0);
    }

    // Test 7: Star graph
    {
        int V = 5;
        std::vector<std::tuple<int,int,int>> edges = {
            {1, 1, 2}, {1, 1, 3}, {1, 1, 4}, {1, 1, 5}, {100, 2, 3}, {100, 4, 5}
        };
        // MST: four edges of weight 1 = 4
        assert(minimumSpanningTreeWeight(V, edges) == 4);
    }

    // Test 8: Complete graph with equal weights, V=4, all edges weight 3
    {
        int V = 4;
        std::vector<std::tuple<int,int,int>> edges;
        for (int i = 1; i <= V; ++i)
            for (int j = i+1; j <= V; ++j)
                edges.emplace_back(3, i, j);
        // MST has V-1 = 3 edges, each weight 3 => total 9
        assert(minimumSpanningTreeWeight(V, edges) == 9);
    }

    return 0;
}
