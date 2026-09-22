Write a C++ function `int minimumSpanningTreeWeight(int n, const std::vector<std::tuple<int,int,int>>& edges)` that, given a number of vertices `n` and an undirected weighted graph represented by a vector of tuples `(u, v, weight)` (with 0-based vertex indices), returns the total weight of a Minimum Spanning Tree (MST) using Kruskal's algorithm. The graph is connected, edge weights are positive integers, and there may be multiple edges between the same pair of vertices. The function must return the sum of the weights of the edges in the MST. If the graph is not connected, return -1 (though the input is guaranteed connected). Use a disjoint set union (DSU) data structure for efficiency. Avoid using global arrays; keep all state local to the function.

The solution uses Kruskal's algorithm, which sorts all edges by weight in ascending order and adds each edge to the MST if it connects two different components. To track components, we use a Disjoint Set Union (DSU) with path compression and union by rank. Initially, each vertex is its own component. We iterate through the sorted edges; for each edge `(u, v, w)`, we find the roots of `u` and `v`. If they are different, we union them, add `w` to the total sum, and increment an edge counter. We stop early once we have added exactly `n-1` edges, because that forms a spanning tree. If we finish the loop without reaching `n-1` edges, the graph is disconnected. Edge cases include parallel edges (handled naturally since we only add an edge when it connects disjoint components), self-loops (which we ignore because they connect a vertex to itself, so find returns the same root), and very large `n` where recursion depth is avoided by using iterative DSU operations. Time complexity is `O(E log E)` due to sorting, and each DSU operation is nearly `O(1)` (amortized inverse Ackermann). Space complexity is `O(n)` for the DSU parent and rank arrays plus `O(E)` for storing the sorted edge list.

#include <vector>
#include <tuple>
#include <algorithm>

class DSU {
    std::vector<int> parent, rank;
public:
    explicit DSU(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return false;
        if (rank[rx] < rank[ry]) std::swap(rx, ry);
        parent[ry] = rx;
        if (rank[rx] == rank[ry]) ++rank[rx];
        return true;
    }
};

// Return total weight of MST using Kruskal's algorithm, -1 if disconnected
int minimumSpanningTreeWeight(int n, const std::vector<std::tuple<int,int,int>>& edges) {
    std::vector<std::tuple<int,int,int>> sortedEdges = edges;
    std::sort(sortedEdges.begin(), sortedEdges.end(),
              [](const auto& a, const auto& b) { return std::get<2>(a) < std::get<2>(b); });

    DSU dsu(n);
    int totalWeight = 0;
    int edgesUsed = 0;

    for (const auto& [u, v, w] : sortedEdges) {
        if (dsu.unite(u, v)) {
            totalWeight += w;
            ++edgesUsed;
            if (edgesUsed == n - 1) break;
        }
    }

    return edgesUsed == n - 1 ? totalWeight : -1;
}

#include <cassert>
#include <vector>
#include <tuple>

int main() {
    // Simple triangle: vertices 0,1,2 with edges 0-1 (1), 1-2 (2), 0-2 (3)
    std::vector<std::tuple<int,int,int>> e1 = {{0,1,1},{1,2,2},{0,2,3}};
    assert(minimumSpanningTreeWeight(3, e1) == 3);

    // Star graph centered at 0 with 3 leaves, weights 5,6,7 -> MST sum = 18
    std::vector<std::tuple<int,int,int>> e2 = {{0,1,5},{0,2,6},{0,3,7},{1,2,100},{2,3,100}};
    assert(minimumSpanningTreeWeight(4, e2) == 18);

    // Disconnected graph -> should return -1
    std::vector<std::tuple<int,int,int>> e3 = {{0,1,1},{2,3,2}};
    assert(minimumSpanningTreeWeight(4, e3) == -1);

    // Self-loop and parallel edges: vertices 0,1 with two edges weight 10 and 20, self-loop weight 1
    std::vector<std::tuple<int,int,int>> e4 = {{0,0,1},{0,1,10},{0,1,20}};
    assert(minimumSpanningTreeWeight(2, e4) == 10);

    // Larger connected graph with 5 vertices and many edges
    std::vector<std::tuple<int,int,int>> e5 = {{0,1,4},{0,2,3},{1,2,1},{1,3,2},{2,3,4},{3,4,2},{2,4,5}};
    assert(minimumSpanningTreeWeight(5, e5) == 8);

    // Single vertex, no edges -> MST weight 0
    std::vector<std::tuple<int,int,int>> e6 = {};
    assert(minimumSpanningTreeWeight(1, e6) == 0);

    // Two vertices, one edge
    std::vector<std::tuple<int,int,int>> e7 = {{0,1,42}};
    assert(minimumSpanningTreeWeight(2, e7) == 42);

    // All edges same weight, tree must pick 3 edges for 4 vertices
    std::vector<std::tuple<int,int,int>> e8 = {{0,1,7},{1,2,7},{2,3,7},{0,3,7}};
    assert(minimumSpanningTreeWeight(4, e8) == 21);

    // Negative weights not allowed by spec, but just in case; here positive only
    std::vector<std::tuple<int,int,int>> e9 = {{0,1,1},{0,2,1},{1,2,1},{2,3,1}};
    assert(minimumSpanningTreeWeight(4, e9) == 3);

    return 0;
}
