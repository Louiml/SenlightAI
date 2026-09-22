// Write a C++ function `bool hasUniqueTopologicalOrder(const MGraph& graph)` that determines whether a directed graph has a unique topological ordering. The function must return `true` if and only if every vertex can be included in exactly one valid topological sequence. The graph is represented by an adjacency-matrix structure with at most 100 vertices (MAXV), with `Edge[i][j] == 1` indicating a directed edge from vertex `i` to vertex `j`. The algorithm must be based on repeatedly removing vertices with in-degree zero; the topological order is unique if and only if at every step there is exactly one zero‑in‑degree vertex. Handle graphs with cycles (return `false`), and also handle the trivial cases of zero vertices (return `true` — an empty order is unique) and zero edges (return `true` if there is exactly one vertex, `false` if more than one vertex because any permutation is valid). Do not modify the input graph; ensure the function is `const`‑correct.

The solution uses Kahn’s algorithm adapted to count the number of zero‑in‑degree vertices at each iteration. First, compute the in‑degree of every vertex by iterating through the adjacency matrix: for each row `i`, if `Edge[i][j]` is 1, increment `indegree[j]`. Then, for exactly `numVertices` iterations, scan all vertices to count how many have in‑degree zero. If the count is 0, the graph has a cycle (since no vertex can be removed) → return `false`. If the count is greater than 1, there are multiple possible choices for the next vertex, meaning more than one topological order exists → return `false`. Otherwise, select that unique zero‑in‑degree vertex, mark it as processed (e.g., set its in‑degree to -1), and decrement the in‑degree of every successor (`j` where `Edge[zeroIndex][j]` is 1). Continue. If all vertices are processed successfully (the loop completes), the topological order is unique → return `true`. Edge cases: an empty graph (0 vertices) is trivially unique; a graph with one vertex and no edges is unique; a graph with multiple isolated vertices (no edges) has multiple orders (any permutation) so `false`. Time complexity is O(V²) because we scan all vertices V times, and each scan checks the adjacency matrix indirectly via in‑degrees (the matrix itself is accessed only when decrementing, but total edge decrements are O(E) over the whole run). Space complexity is O(V) for the in‑degree array.

#include <vector>
#include <algorithm>

struct MGraph {
    int numVertices;
    std::vector<std::vector<int>> Edge;  // adjacency matrix, Edge[i][j] = 1 if edge i->j
};

// Returns true iff the directed graph has a unique topological ordering.
bool hasUniqueTopologicalOrder(const MGraph& graph) {
    int n = graph.numVertices;
    if (n == 0) return true;  // empty graph: empty order is unique

    std::vector<int> indegree(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (graph.Edge[i][j]) indegree[j]++;
        }
    }

    for (int step = 0; step < n; ++step) {
        int zeroCount = 0;
        int zeroIndex = -1;
        for (int i = 0; i < n; ++i) {
            if (indegree[i] == 0) {
                zeroCount++;
                zeroIndex = i;
            }
        }
        if (zeroCount == 0) return false;      // cycle
        if (zeroCount > 1) return false;       // ambiguous order

        indegree[zeroIndex] = -1;              // mark as removed
        for (int j = 0; j < n; ++j) {
            if (graph.Edge[zeroIndex][j]) {
                indegree[j]--;
            }
        }
    }
    return true;  // all vertices processed successfully
}

#include <cassert>

int main() {
    // Test 1: Single vertex, no edges -> unique
    MGraph g1;
    g1.numVertices = 1;
    g1.Edge = {{0}};
    assert(hasUniqueTopologicalOrder(g1) == true);

    // Test 2: Two vertices, no edges -> multiple orders (false)
    MGraph g2;
    g2.numVertices = 2;
    g2.Edge = {{0,0},{0,0}};
    assert(hasUniqueTopologicalOrder(g2) == false);

    // Test 3: Two vertices, one edge 0->1 -> unique order [0,1]
    MGraph g3;
    g3.numVertices = 2;
    g3.Edge = {{0,1},{0,0}};
    assert(hasUniqueTopologicalOrder(g3) == true);

    // Test 4: 0->1 and 0->2, two edges from same source -> order 0, then 1 and 2 interchangeable (false)
    MGraph g4;
    g4.numVertices = 3;
    g4.Edge = {{0,1,1},{0,0,0},{0,0,0}};
    assert(hasUniqueTopologicalOrder(g4) == false);

    // Test 5: Chain 0->1->2 -> unique order [0,1,2]
    MGraph g5;
    g5.numVertices = 3;
    g5.Edge = {{0,1,0},{0,0,1},{0,0,0}};
    assert(hasUniqueTopologicalOrder(g5) == true);

    // Test 6: Cycle 0->1->0 -> no topological order (false)
    MGraph g6;
    g6.numVertices = 2;
    g6.Edge = {{0,1},{1,0}};
    assert(hasUniqueTopologicalOrder(g6) == false);

    // Test 7: Empty graph (0 vertices) -> unique (trivial)
    MGraph g7;
    g7.numVertices = 0;
    g7.Edge = {};
    assert(hasUniqueTopologicalOrder(g7) == true);

    // Test 8: 0->2, 1->2 -> order must start with 0 or 1 (two choices) -> false
    MGraph g8;
    g8.numVertices = 3;
    g8.Edge = {{0,0,1},{0,0,1},{0,0,0}};
    assert(hasUniqueTopologicalOrder(g8) == false);

    // Test 9: 0->1, 0->2, 1->2 -> unique order [0,1,2] (because 1 must come before 2)
    MGraph g9;
    g9.numVertices = 3;
    g9.Edge = {{0,1,1},{0,0,1},{0,0,0}};
    assert(hasUniqueTopologicalOrder(g9) == true);

    // Test 10: 0->1, 2->1, 2->0 -> unique order [2,0,1]
    MGraph g10;
    g10.numVertices = 3;
    g10.Edge = {{0,1,0},{0,0,0},{1,1,0}};
    assert(hasUniqueTopologicalOrder(g10) == true);

    return 0;
}
