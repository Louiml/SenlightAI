Write a C++ function `minimumSpanningTreeInfo` that takes a number of vertices `n`, a number of edges `m`, and a vector of edges (each edge having endpoints `u`, `v`, and weight `w`), and returns a `struct Result` containing two fields: `bool possible` (whether a minimum spanning tree (MST) can be formed, i.e., the graph is connected) and `vector<int> excludedWeights` (the weights of all edges that are not selected in the MST, in the same order as the input edges after sorting by weight, but only if the MST exists; if no MST exists, this vector should be empty). The graph is undirected and edges may have duplicate weights. The function must be self-contained and not rely on global variables. If multiple MSTs exist, any valid MST is acceptable; the weights of edges not in that MST should be reported. The edges are given with 0-based vertex indices.
#include <cassert>
#include <vector>

// The solution function is declared here (already defined above).
// Declare it again for clarity.
Result minimumSpanningTreeInfo(int n, int m, const std::vector<int>& edgeU,
                               const std::vector<int>& edgeV,
                               const std::vector<int>& edgeW);

int main() {
    // Test 1: Simple connected graph with 3 vertices and 3 edges.
    {
        std::vector<int> u = {0, 1, 2};
        std::vector<int> v = {1, 2, 0};
        std::vector<int> w = {1, 2, 3};
        Result r = minimumSpanningTreeInfo(3, 3, u, v, w);
        assert(r.possible == true);
        assert(r.excludedWeights == std::vector<int>({3})); // the heaviest edge
    }

    // Test 2: Disconnected graph.
    {
        std::vector<int> u = {0, 1};
        std::vector<int> v = {1, 2};
        std::vector<int> w = {1, 1};
        // Vertex 3 isolated.
        Result r = minimumSpanningTreeInfo(4, 2, u, v, w);
        assert(r.possible == false);
        assert(r.excludedWeights.empty());
    }

    // Test 3: Duplicate weights and multiple valid MSTs.
    {
        std::vector<int> u = {0, 1, 2, 3};
        std::vector<int> v = {1, 2, 3, 0};
        std::vector<int> w = {1, 1, 1, 1};
        Result r = minimumSpanningTreeInfo(4, 4, u, v, w);
        assert(r.possible == true);
        assert(r.excludedWeights.size() == 1); // exactly one edge excluded
        assert(r.excludedWeights[0] == 1);
    }

    // Test 4: Single vertex with no edges.
    {
        std::vector<int> u, v, w;
        Result r = minimumSpanningTreeInfo(1, 0, u, v, w);
        assert(r.possible == true);
        assert(r.excludedWeights.empty());
    }

    // Test 5: Two vertices with two parallel edges.
    {
        std::vector<int> u = {0, 0};
        std::vector<int> v = {1, 1};
        std::vector<int> w = {5, 3};
        Result r = minimumSpanningTreeInfo(2, 2, u, v, w);
        assert(r.possible == true);
        assert(r.excludedWeights == std::vector<int>({5})); // heavier parallel edge excluded
    }

    // Test 6: Self-loop and another edge.
    {
        std::vector<int> u = {0, 0};
        std::vector<int> v = {0, 1};
        std::vector<int> w = {7, 2};
        Result r = minimumSpanningTreeInfo(2, 2, u, v, w);
        assert(r.possible == true);
        assert(r.excludedWeights == std::vector<int>({7})); // self-loop excluded
    }

    // Test 7: Larger graph with non-trivial excluded set.
    {
        // 5 vertices, edges: (0,1)2, (1,2)3, (2,3)5, (3,4)7, (0,4)1, (1,4)4
        std::vector<int> u = {0, 1, 2, 3, 0, 1};
        std::vector<int> v = {1, 2, 3, 4, 4, 4};
        std::vector<int> w = {2, 3, 5, 7, 1, 4};
        Result r = minimumSpanningTreeInfo(5, 6, u, v, w);
        assert(r.possible == true);
        // MST includes edges 0-4(1), 0-1(2), 1-2(3), 2-3(5); excludes 3-4(7) and 1-4(4)
        // Sorted by weight: 1(edge4), 2(edge0), 3(edge1), 4(edge5), 5(edge2), 7(edge3)
        // Selected: edges 4,0,1,2 (weights 1,2,3,5). Excluded: weights 4,7 (in sorted order)
        assert(r.excludedWeights == std::vector<int>({4, 7}));
    }

    // Test 8: Graph with n=0 (empty).
    {
        std::vector<int> u, v, w;
        Result r = minimumSpanningTreeInfo(0, 0, u, v, w);
        assert(r.possible == true);
        assert(r.excludedWeights.empty());
    }

    return 0;
}
#include <vector>
#include <algorithm>

// Result structure: possible indicates whether an MST exists, and excludedWeights
// contains weights of edges not in the MST (in sorted edge order) if possible.
struct Result {
    bool possible;
    std::vector<int> excludedWeights;
};

// Disjoint-set (union-find) helpers using path compression and union by rank.
namespace {
    int findRoot(std::vector<int>& parent, int x) {
        if (parent[x] != x) {
            parent[x] = findRoot(parent, parent[x]);
        }
        return parent[x];
    }

    void unionSets(std::vector<int>& parent, std::vector<int>& rank, int a, int b) {
        a = findRoot(parent, a);
        b = findRoot(parent, b);
        if (a == b) return;
        if (rank[a] < rank[b]) {
            std::swap(a, b);
        }
        parent[b] = a;
        if (rank[a] == rank[b]) {
            ++rank[a];
        }
    }
}

// Compute the MST and return which edges are excluded.
Result minimumSpanningTreeInfo(int n, int m, const std::vector<int>& edgeU,
                               const std::vector<int>& edgeV,
                               const std::vector<int>& edgeW) {
    Result res;
    res.possible = false;
    res.excludedWeights.clear();

    // Handle trivial cases: empty or single-vertex graph.
    if (n <= 1) {
        res.possible = true;
        // All edges excluded (if any).
        res.excludedWeights = edgeW;
        return res;
    }

    // Create sorted list of edges by weight.
    struct Edge {
        int u, v, w;
    };
    std::vector<Edge> edges;
    edges.reserve(m);
    for (int i = 0; i < m; ++i) {
        edges.push_back({edgeU[i], edgeV[i], edgeW[i]});
    }
    std::sort(edges.begin(), edges.end(),
              [](const Edge& a, const Edge& b) { return a.w < b.w; });

    // Initialize disjoint-set.
    std::vector<int> parent(n);
    std::vector<int> rank(n, 0);
    for (int i = 0; i < n; ++i) parent[i] = i;

    std::vector<bool> selected(m, false); // mark selected edges in sorted order
    int edgesUsed = 0;
    for (int i = 0; i < m && edgesUsed < n - 1; ++i) {
        int ru = findRoot(parent, edges[i].u);
        int rv = findRoot(parent, edges[i].v);
        if (ru != rv) {
            unionSets(parent, rank, ru, rv);
            selected[i] = true;
            ++edgesUsed;
        }
    }

    // If not enough edges, no MST.
    if (edgesUsed != n - 1) {
        return res; // possible remains false
    }

    // Build excluded weights in sorted order.
    res.possible = true;
    for (int i = 0; i < m; ++i) {
        if (!selected[i]) {
            res.excludedWeights.push_back(edges[i].w);
        }
    }
    return res;
}
// The problem is a classic minimum spanning tree (MST) construction using Kruskal's algorithm. First, sort all edges by weight in ascending order. Then, use a disjoint-set (union-find) data structure to process edges in sorted order. For each edge, if its endpoints are in different components, include it in the MST (mark it as selected), union the components, and accumulate the total weight. Stop early once `n-1` edges have been selected. If after processing all edges, fewer than `n-1` edges were selected, the graph is disconnected and no MST exists; return `possible = false` and an empty vector. Otherwise, collect the weights of all edges not selected (i.e., those not used in the MST) in the order they appear in the sorted edge list. Edge cases to consider: (1) `n = 0` or `n = 1`: an MST trivially exists with total weight 0 and all edges excluded; handle by always returning `possible=true` for `n <= 1` (since a single vertex is connected). (2) Duplicate weights: Kruskal's algorithm naturally handles them. (3) Self-loops and multiple edges: the disjoint-set checks prevent cycles, so they are fine. Time complexity is O(m log m) for sorting, plus O(m α(n)) for union-find operations, where α(n) is the inverse Ackermann function (nearly constant). Space complexity is O(n + m) for the stored edges and disjoint-set parent array.
