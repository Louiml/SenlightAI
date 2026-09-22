// Write a C++ function `int minimumSpanningTreeCost(int n, const std::vector<std::tuple<int,int,int>>& edges)` that takes a number of vertices `n` (vertices labeled `0` to `n-1`) and a list of undirected weighted edges as tuples `(u, v, weight)`, and returns the total weight of a minimum spanning tree (MST) using Kruskal’s algorithm. If the graph is disconnected (i.e., it is impossible to span all vertices), return `-1`. The graph may contain self-loops and parallel edges; ignore self-loops and handle parallel edges by choosing the smallest weight needed. The function must not modify the input vector and must be efficient for up to `n = 10^5` vertices and `10^5` edges.
#include <cassert>
#include <tuple>
#include <vector>

// The solution function is declared above; here we test it.

int main() {
    // Simple triangle: 3 vertices, MST cost = 1+2 = 3
    std::vector<std::tuple<int,int,int>> g1 = {
        {0,1,5}, {1,2,2}, {0,2,1}
    };
    assert(minimumSpanningTreeCost(3, g1) == 3);

    // Disconnected graph: vertex 2 isolated
    std::vector<std::tuple<int,int,int>> g2 = {
        {0,1,3}, {1,0,4}
    };
    assert(minimumSpanningTreeCost(3, g2) == -1);

    // Single vertex: no edges needed
    assert(minimumSpanningTreeCost(1, {}) == 0);

    // Parallel edges: choose smallest (1)
    std::vector<std::tuple<int,int,int>> g3 = {
        {0,1,10}, {0,1,1}, {0,1,5}
    };
    assert(minimumSpanningTreeCost(2, g3) == 1);

    // Self-loops ignored; line graph 0-1-2 with weights 2 and 3
    std::vector<std::tuple<int,int,int>> g4 = {
        {0,0,100}, {0,1,2}, {1,2,3}, {2,2,7}
    };
    assert(minimumSpanningTreeCost(3, g4) == 5);

    // Larger graph: 4 vertices, edges forming MST cost = 1+2+3 = 6
    std::vector<std::tuple<int,int,int>> g5 = {
        {0,1,1}, {1,2,2}, {2,3,3}, {0,3,10}, {1,3,8}
    };
    assert(minimumSpanningTreeCost(4, g5) == 6);

    // All vertices already connected but heavy edges: graph with 3 vertices, only edges 0-1 and 1-2
    std::vector<std::tuple<int,int,int>> g6 = {
        {0,1,42}, {1,2,7}
    };
    assert(minimumSpanningTreeCost(3, g6) == 49);

    // Empty edge list with 2 vertices -> disconnected
    assert(minimumSpanningTreeCost(2, {}) == -1);

    // Negative weights are allowed
    std::vector<std::tuple<int,int,int>> g7 = {
        {0,1,-5}, {1,2,-2}, {0,2,10}
    };
    assert(minimumSpanningTreeCost(3, g7) == -7);

    // Zero-weight edges and n large enough
    std::vector<std::tuple<int,int,int>> g8 = {
        {0,1,0}, {1,2,0}, {2,3,0}
    };
    assert(minimumSpanningTreeCost(4, g8) == 0);

    return 0;
}
#include <algorithm>
#include <numeric>
#include <tuple>
#include <vector>

// Returns the total weight of a minimum spanning tree using Kruskal's algorithm,
// or -1 if the graph is disconnected. Vertices are 0..n-1.
int minimumSpanningTreeCost(int n, const std::vector<std::tuple<int,int,int>>& edges) {
    if (n <= 1) return 0; // 0 or 1 vertex trivially has MST cost 0

    // Copy edges so we can sort without modifying the input
    std::vector<std::tuple<int,int,int>> sorted_edges = edges;
    // Sort by weight (third element of tuple)
    std::sort(sorted_edges.begin(), sorted_edges.end(),
              [](const auto& a, const auto& b) {
                  return std::get<2>(a) < std::get<2>(b);
              });

    // Union-find (disjoint set) with path compression and union by size
    std::vector<int> parent(n);
    std::vector<int> size(n, 1);
    std::iota(parent.begin(), parent.end(), 0);

    auto find_set = [&](int v) {
        int root = v;
        while (root != parent[root]) {
            root = parent[root];
        }
        // Path compression
        while (v != root) {
            int next = parent[v];
            parent[v] = root;
            v = next;
        }
        return root;
    };

    auto union_sets = [&](int a, int b) {
        a = find_set(a);
        b = find_set(b);
        if (a == b) return false;
        if (size[a] < size[b]) std::swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        return true;
    };

    int cost = 0;
    int components_merged = 0; // number of successful unions that connect components

    for (const auto& [u, v, w] : sorted_edges) {
        if (u == v) continue; // self-loop never helps
        if (find_set(u) != find_set(v)) {
            union_sets(u, v);
            cost += w;
            components_merged++;
            if (components_merged == n - 1) break; // all connected
        }
    }

    // If we didn't merge enough components, graph is disconnected
    if (components_merged != n - 1) return -1;
    return cost;
}
// The problem is solved using Kruskal’s algorithm, which relies on a union-find (disjoint-set union) data structure. First, sort all edges by weight in ascending order. Then initialize a union-find with `n` isolated components. Iterate over the sorted edges: for each edge `(u, v, w)`, if `u` and `v` belong to different components, unite them and add `w` to the total cost. Stop early if all vertices are in one component (i.e., when the number of successful unions reaches `n-1`). If after processing all edges fewer than `n-1` unions were performed, the graph is disconnected, so return `-1`. Self-loops (`u == v`) are ignored because they never help connect different components. Parallel edges are naturally handled because after a connection is made, the union-find makes further heavier parallel edges irrelevant. Edge cases include `n = 0` (return 0), `n = 1` (return 0, no edges needed), an empty edge list with `n > 1` (disconnected, return -1). Time complexity is `O(m log m)` for sorting plus `O(m α(n))` for union-find operations, where `α` is the inverse Ackermann function (practically constant). Space complexity is `O(n)` for the union-find arrays and `O(m)` for storing a copy of the edges if needed, though we can sort a copy if the input must remain unmodified.
