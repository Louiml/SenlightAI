// Write a C++ function `std::vector<Edge> kruskalMSTEdges(int n, const std::vector<Edge>& edges)` that, given the number of vertices `n` (0-indexed) and a list of undirected edges (each with `from`, `to`, and `weight`), returns the edges of the Minimum Spanning Tree (MST) using Kruskal's algorithm. The input graph is connected and may contain multiple edges between the same pair of vertices with different weights, and all edge weights are positive. The output should be a vector of `Edge` objects in the order they were selected by Kruskal's algorithm (i.e., sorted by weight, and for equal weights, the order in the input). Include the `Edge` struct and a helper function `findSet` with path compression.

#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple triangle graph with fixed edge order
    {
        int n = 3;
        std::vector<Edge> edges = {Edge(0,1,5), Edge(1,2,3), Edge(0,2,1)};
        std::vector<Edge> mst = kruskalMSTEdges(n, edges);
        assert(mst.size() == 2);
        assert(mst[0].weight == 1 && mst[1].weight == 3);
    }

    // Test 2: Graph with equal weights, preserve input order
    {
        int n = 4;
        std::vector<Edge> edges = {Edge(0,1,2), Edge(1,2,2), Edge(2,3,2), Edge(0,3,2)};
        std::vector<Edge> mst = kruskalMSTEdges(n, edges);
        assert(mst.size() == 3);
        assert(mst[0].from == 0 && mst[0].to == 1);
        assert(mst[1].from == 1 && mst[1].to == 2);
        assert(mst[2].from == 2 && mst[2].to == 3);
    }

    // Test 3: Duplicate edges; the algorithm picks lighter ones
    {
        int n = 3;
        std::vector<Edge> edges = {Edge(0,1,10), Edge(0,1,1), Edge(1,2,2), Edge(0,2,5)};
        std::vector<Edge> mst = kruskalMSTEdges(n, edges);
        assert(mst.size() == 2);
        assert(mst[0].weight == 1 && mst[1].weight == 2);
    }

    // Test 4: Single vertex graph (n=1) has no edges
    {
        int n = 1;
        std::vector<Edge> edges;
        std::vector<Edge> mst = kruskalMSTEdges(n, edges);
        assert(mst.empty());
    }

    // Test 5: Larger graph, expected MST total weight 37 (example from typical MST problem)
    {
        int n = 6;
        std::vector<Edge> edges = {
            Edge(0,1,4), Edge(0,2,3), Edge(1,2,1), Edge(1,3,2),
            Edge(2,3,4), Edge(3,4,2), Edge(4,5,6), Edge(3,5,7)
        };
        std::vector<Edge> mst = kruskalMSTEdges(n, edges);
        assert(mst.size() == 5);
        int sum = 0;
        for (const Edge& e : mst) sum += e.weight;
        assert(sum == 16);
    }

    return 0;
}

#include <vector>
#include <algorithm>

struct Edge {
    int from;
    int to;
    int weight;
    Edge(int u, int v, int w) : from(u), to(v), weight(w) {}
};

// Find with path compression
int findSet(std::vector<int>& parent, int i) {
    if (parent[i] != i) {
        parent[i] = findSet(parent, parent[i]);
    }
    return parent[i];
}

// Union by rank
void unionSet(std::vector<int>& parent, std::vector<int>& rank, int x, int y) {
    int xroot = findSet(parent, x);
    int yroot = findSet(parent, y);
    if (rank[xroot] < rank[yroot]) {
        parent[xroot] = yroot;
    } else if (rank[xroot] > rank[yroot]) {
        parent[yroot] = xroot;
    } else {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}

// Returns the MST edges using Kruskal's algorithm
std::vector<Edge> kruskalMSTEdges(int n, const std::vector<Edge>& edges) {
    std::vector<Edge> sortedEdges = edges;  // copy
    // Stable sort to preserve original order for equal weights
    std::stable_sort(sortedEdges.begin(), sortedEdges.end(),
                     [](const Edge& a, const Edge& b) { return a.weight < b.weight; });

    std::vector<int> parent(n);
    std::vector<int> rank(n, 0);
    for (int i = 0; i < n; ++i) parent[i] = i;

    std::vector<Edge> mst;
    for (const Edge& e : sortedEdges) {
        int u = findSet(parent, e.from);
        int v = findSet(parent, e.to);
        if (u != v) {
            mst.push_back(e);
            unionSet(parent, rank, u, v);
            if (mst.size() == static_cast<size_t>(n - 1)) break; // MST complete
        }
    }
    return mst;
}

// The solution uses Kruskal's algorithm: sort all edges by weight (stable sort to preserve input order for equal weights), then iterate through edges and add an edge to the MST only if its endpoints belong to different sets. Disjoint-set (union-find) with path compression and union by rank is used to efficiently track connectivity. The algorithm terminates after selecting `n-1` edges (since the graph is connected). Edge cases: if the graph is not connected, the function would return fewer than `n-1` edges; handle by returning whatever is selected. Duplicate edges between the same vertices are allowed; the algorithm picks the lightest ones that do not create cycles. Time complexity: `O(E log E)` for sorting (with `E` edges) plus `O(E α(V))` for union-find operations (α is inverse Ackermann). Space complexity: `O(V)` for the subset array and `O(E)` for the sorted edge list (if we copy).
