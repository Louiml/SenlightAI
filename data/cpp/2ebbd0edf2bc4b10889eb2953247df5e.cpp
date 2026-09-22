/*
Write a C++ function `long long minimumSpanningTreeWeight(int n, const std::vector<std::tuple<int,int,int>>& edges)` that takes the number of vertices `n` (vertices are numbered 1 through `n`), a vector of undirected edges where each tuple is `(u, v, weight)`, and returns the total weight of a minimum spanning tree (MST) using Kruskal's algorithm. If the graph is disconnected and no MST exists, return `-1`. The graph may have parallel edges and self-loops; ignore self-loops. All edge weights are non-negative integers up to 10^9, `n` can be up to 10,000, and the edge count can be up to 100,000. The function should be efficient and should not modify the input vector.
*/

#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>

// Returns total weight of MST, or -1 if graph is disconnected.
long long minimumSpanningTreeWeight(int n, const std::vector<std::tuple<int,int,int>>& edges) {
    // Union-find with path compression and union by rank
    std::vector<int> parent(n + 1);
    std::vector<int> rank(n + 1, 0);
    std::iota(parent.begin(), parent.end(), 0);

    auto find = [&](auto&& self, int x) -> int {
        if (parent[x] != x) {
            parent[x] = self(self, parent[x]); // path compression
        }
        return parent[x];
    };

    auto unite = [&](int x, int y) {
        int rx = find(find, x);
        int ry = find(find, y);
        if (rx == ry) return false;
        if (rank[rx] < rank[ry]) std::swap(rx, ry);
        parent[ry] = rx;
        if (rank[rx] == rank[ry]) ++rank[rx];
        return true;
    };

    // Sort edges by weight (first element of tuple)
    std::vector<std::tuple<int,int,int>> sortedEdges = edges;
    std::sort(sortedEdges.begin(), sortedEdges.end(),
        [](const auto& a, const auto& b) { return std::get<2>(a) < std::get<2>(b); });

    long long totalWeight = 0;
    int edgesUsed = 0;

    for (const auto& [u, v, w] : sortedEdges) {
        if (u == v) continue; // ignore self-loops
        if (unite(u, v)) {
            totalWeight += w;
            ++edgesUsed;
            if (edgesUsed == n - 1) break;
        }
    }

    return (edgesUsed == n - 1) ? totalWeight : -1;
}

#include <cassert>
#include <vector>
#include <tuple>

int main() {
    // Simple triangle: vertices 1-2-3 with weights 1,2,3 -> MST is 1+2=3
    std::vector<std::tuple<int,int,int>> e1 = {{1,2,1},{2,3,2},{1,3,3}};
    assert(minimumSpanningTreeWeight(3, e1) == 3);

    // Disconnected: two separate edges -> no MST
    std::vector<std::tuple<int,int,int>> e2 = {{1,2,5},{3,4,6}};
    assert(minimumSpanningTreeWeight(4, e2) == -1);

    // Single vertex, no edges -> MST weight 0 (0 edges used, n-1=0)
    std::vector<std::tuple<int,int,int>> e3;
    assert(minimumSpanningTreeWeight(1, e3) == 0);

    // Self-loops and parallel edges: vertices 1-2 have parallel edges 10 and 3, self-loop at 1 with 100, plus edge 2-3 with 1 -> MST = 3+1=4
    std::vector<std::tuple<int,int,int>> e4 = {{1,1,100},{1,2,10},{1,2,3},{2,3,1}};
    assert(minimumSpanningTreeWeight(3, e4) == 4);

    // Slightly larger: chain of 4 vertices with weights 5,4,3 and an extra edge linking ends with weight 2 -> MST is 2+3+4=9 (not including 5)
    std::vector<std::tuple<int,int,int>> e5 = {{1,2,5},{2,3,4},{3,4,3},{1,4,2}};
    assert(minimumSpanningTreeWeight(4, e5) == 9);

    // All edges negative? Not possible per spec, but ensure non-negative works with zeros
    std::vector<std::tuple<int,int,int>> e6 = {{1,2,0},{2,3,0},{1,3,0}};
    assert(minimumSpanningTreeWeight(3, e6) == 0);

    // Duplicate vertices disconnected from rest
    std::vector<std::tuple<int,int,int>> e7 = {{1,2,7},{2,2,7},{3,4,8}};
    assert(minimumSpanningTreeWeight(4, e7) == -1);

    // n=0? Not allowed per spec (n>=1), but handle gracefully: no vertices -> no edges needed, weight 0
    assert(minimumSpanningTreeWeight(0, e3) == 0);

    return 0;
}

// The solution uses Kruskal's algorithm with a union-find (disjoint set) data structure. Sort the edges by weight in non-decreasing order. Initialize a union-find structure where each vertex is its own parent. Iterate through sorted edges: for each edge `(u, v, w)`, if `u` and `v` are in different sets, union them and add `w` to the answer, increment a counter of selected edges. Stop when the counter equals `n-1` (since an MST on `n` vertices has exactly `n-1` edges). If we finish iterating without reaching `n-1` edges, the graph is disconnected → return `-1`. Edge cases: self-loops are skipped because they never connect different components; parallel edges are handled naturally because only the smallest edge that connects two components is used first after sorting. The union-find uses path compression and union by rank for near-constant amortized time. Complexity: O(E log E) for sorting plus O(E α(n)) for union-find operations, where α is the inverse Ackermann function. Space: O(n) for parent/rank arrays and O(E) for storing edges.
