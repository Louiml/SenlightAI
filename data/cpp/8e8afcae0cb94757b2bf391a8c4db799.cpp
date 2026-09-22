Write a C++ function `std::vector<std::array<int, 3>> kruskalMST(int n, const std::vector<std::array<int, 3>>& edges)` that takes the number of vertices `n` (vertices are labeled `0` to `n-1`) and a list of undirected edges, where each edge is a triple `{source, destination, weight}`, and returns the edges of a Minimum Spanning Tree (MST) using Kruskal’s algorithm. The returned vector must contain exactly `n-1` edges sorted by weight (ascending), each as `{source, destination, weight}`; if multiple MSTs exist, any valid one is acceptable. Assume the graph is connected and the input edge list may contain duplicate edges or self-loops (which should be ignored). The function must not print anything; it must only return the MST edges.
// Kruskal’s algorithm builds the MST by sorting all edges by weight and then adding them one by one if they connect two different components. We need a union-find (disjoint-set) data structure with path compression and union by rank to efficiently track connected components. A self-loop (`source == destination`) can never be part of an MST, so skip them. Duplicate edges are harmless—the algorithm will skip any that create a cycle. We sort edges by weight, then iterate through them, checking `find` for both endpoints. If the roots differ, we add the edge to the result and perform a union. We stop once we have `n-1` edges (since the graph is connected). Edge cases: if `n` is 1 or 2, the MST has 0 or 1 edges respectively; if the input contains fewer than `n-1` valid edges, the result will be shorter (though the problem assumes connectivity). Time complexity is `O(E log E)` due to sorting (where `E` is the number of edges) plus near-constant time per union-find operation (inverse Ackermann). Space complexity is `O(n + V)` for the parent and rank arrays and the output vector.
#include <vector>
#include <array>
#include <algorithm>

// Union-Find with path compression and union by rank
class DSU {
public:
    std::vector<int> parent, rank;
    DSU(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    void unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return;
        if (rank[rx] < rank[ry]) std::swap(rx, ry);
        parent[ry] = rx;
        if (rank[rx] == rank[ry]) ++rank[rx];
    }
};

// Return MST edges (source, destination, weight) sorted by weight.
std::vector<std::array<int, 3>> kruskalMST(int n, const std::vector<std::array<int, 3>>& edges) {
    std::vector<std::array<int, 3>> valid_edges;
    for (const auto& e : edges) {
        if (e[0] != e[1]) valid_edges.push_back(e);
    }
    std::sort(valid_edges.begin(), valid_edges.end(),
              [](const auto& a, const auto& b) { return a[2] < b[2]; });

    DSU dsu(n);
    std::vector<std::array<int, 3>> mst;
    mst.reserve(n - 1);

    for (const auto& e : valid_edges) {
        if (mst.size() == static_cast<size_t>(n - 1)) break;
        int s = e[0], d = e[1];
        if (dsu.find(s) != dsu.find(d)) {
            dsu.unite(s, d);
            mst.push_back(e);
        }
    }
    return mst;
}
#include <cassert>
#include <vector>
#include <array>

// (The solution function is included from above; for testing, paste the solution here.)
// For brevity, the test assumes the function is already defined.

int main() {
    // Example 1: Triangle with equal weights
    {
        int n = 3;
        std::vector<std::array<int, 3>> edges = {{0,1,1}, {1,2,1}, {0,2,1}};
        auto mst = kruskalMST(n, edges);
        assert(mst.size() == 2);
        // Total weight should be 2
        int total = mst[0][2] + mst[1][2];
        assert(total == 2);
    }

    // Example 2: Classic 4-node graph; MST weight = 7
    {
        int n = 4;
        std::vector<std::array<int, 3>> edges = {
            {0,1,10}, {1,2,6}, {2,3,5}, {0,3,2}, {0,2,1}, {1,3,3}
        };
        auto mst = kruskalMST(n, edges);
        assert(mst.size() == 3);
        int total = 0;
        for (auto& e : mst) total += e[2];
        assert(total == 7);
        // Check sorted by weight
        for (size_t i = 1; i < mst.size(); ++i)
            assert(mst[i-1][2] <= mst[i][2]);
    }

    // Example 3: Self-loops and duplicates ignored
    {
        int n = 2;
        std::vector<std::array<int, 3>> edges = {
            {0,0,5}, {0,1,3}, {1,1,7}, {0,1,3}, {1,0,4}
        };
        auto mst = kruskalMST(n, edges);
        assert(mst.size() == 1);
        assert(mst[0][2] == 3);
    }

    // Example 4: Two vertices one edge
    {
        int n = 2;
        std::vector<std::array<int, 3>> edges = {{0,1,42}};
        auto mst = kruskalMST(n, edges);
        assert(mst.size() == 1);
        assert(mst[0][2] == 42);
    }

    // Example 5: Line graph (5 nodes)
    {
        int n = 5;
        std::vector<std::array<int, 3>> edges = {
            {0,1,2}, {1,2,3}, {2,3,1}, {3,4,4},
            {0,4,8}, {1,4,5}, {2,4,10}
        };
        auto mst = kruskalMST(n, edges);
        // Must include n-1 = 4 edges. Minimal total = 2+3+1+4? Wait: edges (2→3)=1, (0→1)=2, (1→2)=3, (3→4)=4 => total 10. But alternative: (0→1)=2,(1→2)=3,(2→3)=1,(3→4)=4 =10. Also (1→4)=5 would make more. So total must be 10.
        assert(mst.size() == 4);
        int total = 0;
        for (auto& e : mst) total += e[2];
        assert(total == 10);
    }

    // Example 6: Single vertex (no edges)
    {
        int n = 1;
        std::vector<std::array<int, 3>> edges = {};
        auto mst = kruskalMST(n, edges);
        assert(mst.empty());
    }
}
