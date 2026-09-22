// Write a C++ function `std::vector<int> findSmallestSetOfVertices(int n, const std::vector<std::vector<int>>& edges)` that, given a directed acyclic graph (DAG) with vertices numbered `0` to `n-1`, returns the smallest set of vertices from which all vertices in the graph are reachable. The graph is guaranteed to have no cycles. The input `edges` is a list of directed edges `[from, to]`, where `0 <= from, to < n`. Your function must return the vertices in increasing numeric order. If the graph has multiple such smallest sets (which is impossible for a DAG—the set is unique), return any valid one. You may assume `n >= 1` and that every vertex appears in at least one edge (i.e., there are no isolated vertices with no edges), though your solution should still handle isolated vertices gracefully by including them in the result (since they have no incoming edges, they must be included). The function must not use any global state and must not modify the input edges.
#include <cassert>
#include <vector>

// Function declaration (from the solution above)
std::vector<int> findSmallestSetOfVertices(int n, const std::vector<std::vector<int>>& edges);

int main() {
    // Test 1: Simple DAG with one obvious source.
    {
        int n = 3;
        std::vector<std::vector<int>> edges = {{0, 1}, {1, 2}};
        std::vector<int> expected = {0};
        assert(findSmallestSetOfVertices(n, edges) == expected);
    }

    // Test 2: Multiple sources.
    {
        int n = 5;
        std::vector<std::vector<int>> edges = {{0, 1}, {2, 1}, {3, 1}, {1, 4}, {2, 4}};
        std::vector<int> expected = {0, 2, 3};
        assert(findSmallestSetOfVertices(n, edges) == expected);
    }

    // Test 3: Isolated vertex (no edges at all) must be included.
    {
        int n = 3;
        std::vector<std::vector<int>> edges = {}; // no edges
        std::vector<int> expected = {0, 1, 2};
        assert(findSmallestSetOfVertices(n, edges) == expected);
    }

    // Test 4: All vertices reachable from a single source.
    {
        int n = 4;
        std::vector<std::vector<int>> edges = {{1, 0}, {1, 2}, {1, 3}, {0, 2}};
        std::vector<int> expected = {1};
        assert(findSmallestSetOfVertices(n, edges) == expected);
    }

    // Test 5: Already all vertices are sources (no edges) — same as isolated case but with edges that are all self-loops? Actually DAG no self-loop; so empty edges scenario already covered. Use a case where every vertex has in-degree 0 because edges only go from lower to higher but no incoming to the lowest? Example: n=2, edges={{0,1}} -> source=0.
    {
        int n = 2;
        std::vector<std::vector<int>> edges = {{0, 1}};
        std::vector<int> expected = {0};
        assert(findSmallestSetOfVertices(n, edges) == expected);
    }

    // Test 6: Larger graph with multiple layers, ensure order increasing.
    {
        int n = 6;
        std::vector<std::vector<int>> edges = {
            {2, 0}, {2, 1}, {3, 1}, {4, 2}, {5, 3}, {5, 4}
        };
        std::vector<int> expected = {5}; // only vertex 5 has no incoming edges
        assert(findSmallestSetOfVertices(n, edges) == expected);
    }

    // Test 7: Chain where middle node has incoming but others not.
    {
        int n = 4;
        std::vector<std::vector<int>> edges = {{0, 1}, {2, 3}, {1, 2}};
        std::vector<int> expected = {0, 3}; // 0 and 3 have no incoming edges
        assert(findSmallestSetOfVertices(n, edges) == expected);
    }

    // Test 8: Single vertex graph.
    {
        int n = 1;
        std::vector<std::vector<int>> edges = {};
        std::vector<int> expected = {0};
        assert(findSmallestSetOfVertices(n, edges) == expected);
    }

    // Additional edge case: duplicate edges (still counts in-degree twice) but graph is DAG so duplicates not expected; test robustness.
    {
        int n = 2;
        std::vector<std::vector<int>> edges = {{0, 1}, {0, 1}}; // duplicate edge, still in-degree of 1 is 2, but source is 0.
        std::vector<int> expected = {0};
        assert(findSmallestSetOfVertices(n, edges) == expected);
    }

    return 0;
}
#include <vector>
#include <cstddef>

// Returns the smallest set of vertices that can reach all vertices in a DAG.
// The set is exactly the vertices with in-degree zero.
std::vector<int> findSmallestSetOfVertices(int n, const std::vector<std::vector<int>>& edges) {
    std::vector<int> in_degree(n, 0);

    // Count in-degree for each vertex.
    for (const auto& edge : edges) {
        // edge[1] is the destination vertex.
        ++in_degree[edge[1]];
    }

    std::vector<int> result;
    result.reserve(n);

    // Vertices with no incoming edges are the sources that must be included.
    for (int i = 0; i < n; ++i) {
        if (in_degree[i] == 0) {
            result.push_back(i);
        }
    }

    return result;
}
// The key observation is that in any directed acyclic graph, a vertex that has no incoming edges (in-degree 0) cannot be reached from any other vertex, so it must be a source and must be included in the smallest set. Conversely, any vertex with at least one incoming edge can be reached from some source that has a path to it (since the graph is a DAG, following incoming edges backward eventually reaches a source). Therefore, the smallest set of vertices that can reach all nodes is exactly the set of all vertices with in-degree 0. The algorithm simply computes the in-degree of each vertex by iterating over all edges and incrementing the count for the destination vertex of each edge. Then collect all vertices where in-degree equals 0. Because the graph is a DAG, this set is unique and minimal. Edge cases: if `n` is 0 (though guaranteed `>=1`), return empty; if a vertex has no incoming edges but also no outgoing edges (isolated), it is still included as required. Time complexity is O(n + E) where E is the number of edges, because we iterate over all edges once to count in-degrees and then iterate over all n vertices to collect zeros. Space complexity is O(n) for the in-degree array and result vector.
