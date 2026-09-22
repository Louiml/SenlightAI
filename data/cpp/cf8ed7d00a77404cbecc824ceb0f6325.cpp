Write a C++ function `int minimumSpanningTreeWeight(int n, const std::vector<std::tuple<int,int,int>>& edges)` that takes a positive integer `n` (the number of vertices, labeled from 1 to n) and a vector of undirected weighted edges (each edge is a tuple `(u, v, w)` where `u` and `v` are distinct vertex labels between 1 and n, and `w` is a non-negative integer weight). The function must compute the total weight of a minimum spanning tree (MST) of the undirected graph, or return `-1` if the graph is disconnected (i.e., no spanning tree exists). The input may contain multiple edges between the same pair of vertices and may be provided in any order. The graph is simple after merging parallel edges, and you may assume vertex labels are valid. Use Kruskal's algorithm with a disjoin‑set union (DSU) data structure for efficiency. Implement DSU with path compression and union by rank.

Kruskal's algorithm works by sorting all edges by weight in ascending order. We then iterate through the sorted list, and for each edge `(u,v,w)`, if `u` and `v` belong to different connected components (checked via DSU find), we add `w` to the total MST weight and merge the two components (union). This greedily picks the smallest‑weight edge that does not form a cycle, which is optimal for MST. After processing all edges, if the number of edges successfully added to the MST equals `n-1`, the graph is connected and we return the accumulated weight. Otherwise, we return `-1`. Important edge cases: (1) `n` may be 1, where the MST weight is 0 (no edges needed) and the graph is trivially connected; (2) if `n` is 1 and no edges exist, return 0; (3) if the graph has fewer than `n-1` edges after removing duplicates, it cannot be connected; (4) duplicate edges are naturally handled because DSU union ignores any edge that would create a cycle. Time complexity is `O(m log m)` for sorting plus `O(m α(n))` for DSU operations (where α is the inverse Ackermann function, practically constant), so overall `O(m log m)` for `m` edges. Space complexity is `O(n)` for DSU parent and rank arrays, plus `O(m)` for the edge list copy.

#include <vector>
#include <tuple>
#include <algorithm>

// Disjoint Set Union (Union-Find) with path compression and union by rank.
class DSU {
public:
    explicit DSU(int n) : parent(n + 1), rank(n + 1, 0) {
        for (int i = 1; i <= n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);  // path compression
        }
        return parent[x];
    }

    // Returns true if the two vertices were in different sets and union is performed.
    bool unite(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) return false;

        // union by rank
        if (rank[ra] < rank[rb]) {
            std::swap(ra, rb);
        }
        parent[rb] = ra;
        if (rank[ra] == rank[rb]) ++rank[ra];
        return true;
    }

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

// Computes the total weight of an MST using Kruskal's algorithm.
// Edges are given as (u, v, weight). Returns -1 if the graph is disconnected.
int minimumSpanningTreeWeight(int n, const std::vector<std::tuple<int, int, int>>& edges) {
    // Special case: a single vertex has an MST of weight 0.
    if (n <= 1) return 0;

    // Sort edges by weight.
    std::vector<std::tuple<int, int, int>> sorted_edges = edges;
    std::sort(sorted_edges.begin(), sorted_edges.end(),
              [](const auto& a, const auto& b) { return std::get<2>(a) < std::get<2>(b); });

    DSU dsu(n);
    int total_weight = 0;
    int edges_used = 0;

    for (const auto& [u, v, w] : sorted_edges) {
        if (dsu.unite(u, v)) {
            total_weight += w;
            ++edges_used;
            if (edges_used == n - 1) break;  // MST complete
        }
    }

    return (edges_used == n - 1) ? total_weight : -1;
}

#include <cassert>
#include <vector>
#include <tuple>

// declaration of the tested function
int minimumSpanningTreeWeight(int n, const std::vector<std::tuple<int, int, int>>& edges);

int main() {
    // Single vertex, no edges → weight 0
    assert(minimumSpanningTreeWeight(1, {}) == 0);

    // Simple connected graph with 3 vertices and 3 edges
    std::vector<std::tuple<int, int, int>> edges1 = {{1,2,1}, {2,3,2}, {1,3,3}};
    assert(minimumSpanningTreeWeight(3, edges1) == 3);  // choose 1-2 (1) + 2-3 (2)

    // Disconnected graph: vertices 1,2 isolated from 3,4
    std::vector<std::tuple<int, int, int>> edges2 = {{1,2,5}, {3,4,7}};
    assert(minimumSpanningTreeWeight(4, edges2) == -1);

    // Duplicate edges with different weights – smallest should be chosen
    std::vector<std::tuple<int, int, int>> edges3 = {{1,2,10}, {1,2,1}, {2,3,1}};
    assert(minimumSpanningTreeWeight(3, edges3) == 2);  // 1-2 (1) + 2-3 (1)

    // Graph with negative weights (allowed) – still works
    std::vector<std::tuple<int, int, int>> edges4 = {{1,2,-5}, {2,3,-2}, {1,3,3}};
    assert(minimumSpanningTreeWeight(3, edges4) == -7);  // -5 + -2

    // Complete graph on 4 vertices with various weights
    std::vector<std::tuple<int, int, int>> edges5 = {
        {1,2,4}, {1,3,1}, {1,4,3},
        {2,3,2}, {2,4,5},
        {3,4,6}
    };
    assert(minimumSpanningTreeWeight(4, edges5) == 7);  // edges: 1-3(1), 2-3(2), 1-4(3)

    // n=2 with one edge
    assert(minimumSpanningTreeWeight(2, {{1,2,8}}) == 8);

    // n=2 no edges → disconnected
    assert(minimumSpanningTreeWeight(2, {}) == -1);

    return 0;
}
