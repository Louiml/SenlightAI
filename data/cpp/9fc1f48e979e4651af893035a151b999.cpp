// Write a C++ function that computes the total weight of a minimum spanning tree (MST) of a small undirected graph using Kruskal's algorithm, given the graph as an adjacency list with `std::pair<int,int>` edges (where vertices are 0-based integers) and a `std::vector<double>` of edge weights in the same order as the edges. The function should take the number of vertices `n`, the vector of edges, and the vector of weights, and return the MST total weight as a `double`. The graph may be disconnected; in that case return the sum of weights of the MSTs of each connected component. If the graph has no edges, return 0.0. Edge weights are non-negative. Implement the function with signature `double mst_total_weight(int n, const std::vector<std::pair<int,int>>& edges, const std::vector<double>& weights)` and include a helper disjoint-set (union-find) structure to support efficient cycle detection and edge sorting.
// The solution applies Kruskal's algorithm: sort all edges by weight in ascending order, then process them in that order, adding an edge to the MST forest if it connects two distinct components. Use a disjoint-set union (DSU) with path compression and union by rank to track components. Initialize each vertex as its own component. For each edge (u,v) in sorted order, find the roots; if they differ, union them and add the weight to the total. Edge cases: an empty edge list yields total 0; disconnected graphs are handled naturally because edges within components are skipped and each component's MST weight is accumulated; duplicate edges are fine because the second one will connect already-connected vertices and be skipped; self-loops (u==v) are skipped because find(u)==find(v). Complexity: sorting O(E log E), DSU operations nearly O(alpha(V)) per edge, so overall O(E log E + V alpha(V)), which is dominated by sorting for large E. Space: O(V) for DSU and O(E) for the edge copy (if we sort by weight, we can sort an index array or pair weights with edges to avoid mutating input; we'll create a vector of triples or sort an index vector).
#include <vector>
#include <algorithm>
#include <numeric>

// Helper: Disjoint Set Union (Union-Find) with path compression and union by rank.
class DSU {
public:
    explicit DSU(int n) : parent(n), size(n, 1) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // path compression
        }
        return parent[x];
    }

    bool unite(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) return false; // already connected
        // union by size (rank)
        if (size[ra] < size[rb]) std::swap(ra, rb);
        parent[rb] = ra;
        size[ra] += size[rb];
        return true;
    }

private:
    std::vector<int> parent;
    std::vector<int> size;
};

// Compute total MST weight for an undirected graph using Kruskal's algorithm.
double mst_total_weight(int n, const std::vector<std::pair<int,int>>& edges,
                        const std::vector<double>& weights) {
    // Build a vector of (weight, edge_index) to keep edges sorted by weight without modifying input.
    std::vector<std::pair<double, int>> indexed_edges;
    indexed_edges.reserve(edges.size());
    for (int i = 0; i < static_cast<int>(edges.size()); ++i) {
        indexed_edges.emplace_back(weights[i], i);
    }
    std::sort(indexed_edges.begin(), indexed_edges.end());

    DSU dsu(n);
    double total = 0.0;

    for (const auto& [w, idx] : indexed_edges) {
        const auto& [u, v] = edges[idx];
        // Ignore self-loops; dsu.unite will also handle them.
        if (dsu.unite(u, v)) {
            total += w;
        }
    }

    return total;
}
#include <cassert>
#include <vector>
#include <utility>

// Declare the function (defined in solution above)
double mst_total_weight(int n, const std::vector<std::pair<int,int>>& edges,
                        const std::vector<double>& weights);

int main() {
    // Test 1: Simple triangle with weights 1,2,3 -> MST weight 1+2=3
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{0,2}};
        std::vector<double> weights = {1.0, 2.0, 3.0};
        assert(mst_total_weight(n, edges, weights) == 3.0);
    }

    // Test 2: Disconnected graph: two separate edges among 4 vertices -> sum = 4.5
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{0,1},{2,3}};
        std::vector<double> weights = {2.5, 2.0};
        assert(mst_total_weight(n, edges, weights) == 4.5);
    }

    // Test 3: No edges -> returns 0.0
    {
        int n = 5;
        std::vector<std::pair<int,int>> edges;
        std::vector<double> weights;
        assert(mst_total_weight(n, edges, weights) == 0.0);
    }

    // Test 4: Graph with a cycle and duplicate weights; MST picks two smallest edges
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,3},{3,0},{0,2}};
        std::vector<double> weights = {1.0, 1.0, 1.0, 10.0, 10.0};
        // MST uses edges (0,1),(1,2),(2,3) -> total = 3.0
        assert(mst_total_weight(n, edges, weights) == 3.0);
    }

    // Test 5: Self-loop only -> ignored, total = 0.0
    {
        int n = 2;
        std::vector<std::pair<int,int>> edges = {{0,0},{1,1}};
        std::vector<double> weights = {5.0, 7.0};
        assert(mst_total_weight(n, edges, weights) == 0.0);
    }

    // Test 6: A more complex tree (already a tree) -> sum of all weights
    {
        int n = 5;
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,3},{3,4}};
        std::vector<double> weights = {0.5, 1.5, 2.5, 3.5};
        assert(mst_total_weight(n, edges, weights) == 8.0);
    }

    // Test 7: Negative weights? Not required but ensure non-negative assumption; if allowed it should still work.
    // Use zeros to check tie-breaking.
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{0,2}};
        std::vector<double> weights = {0.0, 0.0, 5.0};
        assert(mst_total_weight(n, edges, weights) == 0.0);
    }

    return 0;
}
