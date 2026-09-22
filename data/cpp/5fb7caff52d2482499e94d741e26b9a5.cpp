/*
Write a C++ function `findMinimumSpanningWeight` that takes an undirected graph represented as an adjacency list with edge weights, along with a real-valued vector `x` of length equal to the number of edges in the graph, and computes the weight of a minimum spanning tree (MST) using a **modified Kruskal's algorithm**. The modification is: edge ordering is determined by a composite key `alpha * x[e] - beta * w[e]`, where `w[e]` is the original edge weight, and `alpha` and `beta` are non-negative constants. Edges with smaller composite key are considered first. Your function should return the total original edge weight `w` of the resulting tree. If the graph is disconnected, return `-1.0`. The graph has `n` nodes (numbered `0` to `n-1`) and `m` edges. The input gives `n`, `alpha`, `beta`, and the list of edges each as `(u, v, w)`; `x` is a vector of length `m` corresponding to the edges in the given order. Assume `alpha > 0` and `beta >= 0`. Use exact double comparisons (no epsilon tolerance) for sorting and for the final sum. The function signature is: `double findMinimumSpanningWeight(int n, double alpha, double beta, const std::vector<std::tuple<int,int,double>>& edges, const std::vector<double>& x)`.
*/

#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>

class DisjointSet {
public:
    explicit DisjointSet(int n) : parent(n), rank(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    bool unite(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) return false;
        if (rank[ra] < rank[rb]) {
            parent[ra] = rb;
        } else if (rank[ra] > rank[rb]) {
            parent[rb] = ra;
        } else {
            parent[rb] = ra;
            ++rank[ra];
        }
        return true;
    }
private:
    std::vector<int> parent;
    std::vector<int> rank;
};

// Compute the total original edge weight of a minimum spanning tree
// using modified Kruskal's algorithm with composite edge ordering:
// key = alpha * x[e] - beta * w[e]. Returns -1.0 if graph is disconnected.
double findMinimumSpanningWeight(
    int n,
    double alpha,
    double beta,
    const std::vector<std::tuple<int,int,double>>& edges,
    const std::vector<double>& x) 
{
    const int m = static_cast<int>(edges.size());
    if (n == 0) return 0.0;
    if (m == 0) return (n == 1) ? 0.0 : -1.0;
    if (x.size() != static_cast<size_t>(m)) return -1.0;

    // Build indices and composite keys
    std::vector<int> order(m);
    std::iota(order.begin(), order.end(), 0);
    std::vector<double> key(m);
    for (int i = 0; i < m; ++i) {
        double w = std::get<2>(edges[i]);
        key[i] = alpha * x[i] - beta * w;
    }

    // Sort edges by composite key ascending
    std::sort(order.begin(), order.end(),
              [&key](int a, int b) { return key[a] < key[b]; });

    DisjointSet dsu(n);
    double totalWeight = 0.0;
    int edgesUsed = 0;

    for (int idx : order) {
        int u = std::get<0>(edges[idx]);
        int v = std::get<1>(edges[idx]);
        double w = std::get<2>(edges[idx]);
        if (dsu.unite(u, v)) {
            totalWeight += w;
            ++edgesUsed;
            if (edgesUsed == n - 1) break;
        }
    }

    if (edgesUsed != n - 1) return -1.0;
    return totalWeight;
}

#include <cassert>
#include <vector>
#include <tuple>

int main() {
    // Example 1: Simple triangle, all x=0, alpha=1, beta=1.
    // Edges: (0,1,1), (1,2,2), (0,2,3). Keys: -1, -2, -3 => order: edge2, edge1, edge0
    // Select edge2 (0,2,3) and edge1 (1,2,2) => total 5.0
    {
        int n = 3;
        double alpha = 1.0, beta = 1.0;
        std::vector<std::tuple<int,int,double>> edges = {{0,1,1.0}, {1,2,2.0}, {0,2,3.0}};
        std::vector<double> x = {0.0, 0.0, 0.0};
        double result = findMinimumSpanningWeight(n, alpha, beta, edges, x);
        assert(result == 5.0);
    }

    // Example 2: Same triangle but x prefers the lightest edge first.
    // x = {0.9, 0.1, 0.1}, alpha=1, beta=0. Keys: 0.9, 0.1, 0.1 => order: edge1, edge2, edge0
    // Select edge1 (1,2,2) and edge2 (0,2,3) => total 5.0
    {
        int n = 3;
        double alpha = 1.0, beta = 0.0;
        std::vector<std::tuple<int,int,double>> edges = {{0,1,1.0}, {1,2,2.0}, {0,2,3.0}};
        std::vector<double> x = {0.9, 0.1, 0.1};
        double result = findMinimumSpanningWeight(n, alpha, beta, edges, x);
        assert(result == 5.0);
    }

    // Example 3: Disconnected graph
    {
        int n = 4;
        double alpha = 1.0, beta = 1.0;
        std::vector<std::tuple<int,int,double>> edges = {{0,1,1.0}, {2,3,2.0}};
        std::vector<double> x = {0.5, 0.5};
        double result = findMinimumSpanningWeight(n, alpha, beta, edges, x);
        assert(result == -1.0);
    }

    // Example 4: Single node, no edges
    {
        int n = 1;
        double alpha = 1.0, beta = 1.0;
        std::vector<std::tuple<int,int,double>> edges;
        std::vector<double> x;
        double result = findMinimumSpanningWeight(n, alpha, beta, edges, x);
        assert(result == 0.0);
    }

    // Example 5: Two nodes one edge
    {
        int n = 2;
        double alpha = 1.0, beta = 0.0;
        std::vector<std::tuple<int,int,double>> edges = {{0,1,7.5}};
        std::vector<double> x = {1.0};
        double result = findMinimumSpanningWeight(n, alpha, beta, edges, x);
        assert(result == 7.5);
    }

    // Example 6: Cycle where modified order picks a heavier tree.
    // 4 nodes square: edges: (0,1,1), (1,2,2), (2,3,3), (0,3,4), (0,2,10).
    // Set x={0,0,0,0,1} alpha=1 beta=0 => keys:0,0,0,0,1 => order: first four edges.
    // MST from first four: edges (0,1,1), (1,2,2), (2,3,3) -> total 6.0 (diagonal not selected because last)
    {
        int n = 4;
        double alpha = 1.0, beta = 0.0;
        std::vector<std::tuple<int,int,double>> edges = {{0,1,1.0}, {1,2,2.0}, {2,3,3.0}, {0,3,4.0}, {0,2,10.0}};
        std::vector<double> x = {0.0, 0.0, 0.0, 0.0, 1.0};
        double result = findMinimumSpanningWeight(n, alpha, beta, edges, x);
        assert(result == 6.0);
    }
}

// The solution follows a standard Kruskal's algorithm but with a custom edge ordering. First, build a list of edge indices `0` to `m-1`. For each edge `i`, compute a composite key `key_i = alpha * x[i] - beta * w_i`. Sort the indices by these keys in ascending order. Then iterate through the sorted indices, and for each edge `(u, v, w)`, if `u` and `v` are in different connected components (using a disjoint-set union / union-find data structure), add `w` to the total weight and union the components. Count how many edges are added; if the count reaches `n-1`, we have a spanning tree and can stop early. If after processing all edges the count is less than `n-1`, the graph is disconnected and return `-1.0`. Important edge cases: when `m == 0` and `n == 1`, the empty tree has weight 0 (return 0.0); when `n == 0`, return 0.0. Sorting must be stable with respect to equal keys — using `std::sort` with a comparator that returns `key[a] < key[b]` is fine, since equal keys any order is acceptable (the result is the same total weight). The time complexity is dominated by sorting: `O(m log m)`, and each union-find operation is nearly constant (inverse Ackermann), so overall `O(m log m + m α(n))`, which simplifies to `O(m log m)` in practice. Space complexity is `O(n + m)` for the union-find arrays and the keys/indices.
