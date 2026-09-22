// Write a standalone C++ function `double minimumSpanningTreeCost(const std::vector<std::vector<int>>& graph)` that takes a symmetric adjacency matrix representing a connected weighted undirected graph with vertices numbered from 0 to n-1 (where n = graph.size()), and returns the total weight of the minimum spanning tree (MST) using Prim's algorithm. The matrix entries are non-negative integer edge weights, with `graph[i][i] = 0` and `graph[i][j] = graph[j][i]` for all i, j. If two vertices are not directly connected by an edge, the entry is a large sentinel value (e.g., 1e9). The graph is guaranteed to be connected, so an MST always exists. Your function must not modify the input matrix (use `const` correctly) and must not use global variables. The return type is `double` to accommodate potential larger sums, although weights are integers.
Prim's algorithm builds the MST by starting from an arbitrary vertex and repeatedly adding the cheapest edge that connects a vertex already in the tree to a vertex outside it. A standard implementation maintains two arrays: `inMST` (boolean) to mark vertices already in the tree, and `minDist` to store the minimum edge weight from each vertex to the current tree. Initially, pick vertex 0 as the starting vertex, set `minDist[0] = 0` and all others to a large value (e.g., 1e9). Then, for n iterations, select the vertex `u` not yet in the MST with the smallest `minDist`, add its weight to the total cost, mark it as in the tree, and update `minDist` for all neighbors `v` not yet in the tree: `minDist[v] = min(minDist[v], graph[u][v])`. For a connected graph with `n` vertices, this runs in O(n^2) time because each of the n selection steps scans all n vertices, and each update step loops over all neighbors (effectively O(n) per selection). Space usage is O(n) for the two helper arrays. Edge cases: the graph is connected by guarantee, so no need to detect disconnected components; if n = 1, the MST cost is 0 (loop runs once, selects the only vertex, adds 0); duplicate weights are handled naturally since we only take the minimum; the sentinel value (1e9) is large enough to never be chosen as the minimum edge when a real edge exists, but for isolated vertices (not possible in a connected graph) it would remain large and cause an incorrect total – but this cannot occur given the problem constraints.
#include <vector>
#include <algorithm>
#include <limits>

/**
 * Computes the total weight of the Minimum Spanning Tree (MST) of a connected
 * undirected weighted graph using Prim's algorithm.
 *
 * @param graph A symmetric adjacency matrix where graph[i][j] is the weight of
 *              edge (i, j), graph[i][i] = 0, and missing edges are represented
 *              by a sentinel value (e.g., 1e9). The graph is guaranteed to be
 *              connected.
 * @return The sum of edge weights in the MST as a double.
 */
double minimumSpanningTreeCost(const std::vector<std::vector<int>>& graph) {
    const int n = static_cast<int>(graph.size());
    if (n == 0) return 0.0;

    const int INF = 1000000000; // sentinel for "no edge"
    std::vector<bool> inMST(n, false);
    std::vector<int> minDist(n, INF);
    minDist[0] = 0; // start from vertex 0

    double totalCost = 0.0;

    for (int iter = 0; iter < n; ++iter) {
        // Find the vertex not yet in MST with the smallest minDist
        int u = -1;
        int best = INF;
        for (int i = 0; i < n; ++i) {
            if (!inMST[i] && minDist[i] < best) {
                best = minDist[i];
                u = i;
            }
        }

        if (u == -1) {
            // Should not happen for a connected graph, but return cost if it does
            break;
        }

        inMST[u] = true;
        totalCost += minDist[u];

        // Update minDist for neighbors of u not yet in MST
        for (int v = 0; v < n; ++v) {
            if (!inMST[v] && graph[u][v] < minDist[v]) {
                minDist[v] = graph[u][v];
            }
        }
    }

    return totalCost;
}
#include <cassert>
#include <vector>

// The solution function is declared here (from the previous section)
double minimumSpanningTreeCost(const std::vector<std::vector<int>>& graph);

int main() {
    // Test 1: Simple triangle graph with weights 1,2,3 -> MST picks 1 and 2 => total 3
    std::vector<std::vector<int>> g1 = {
        {0, 1, 3},
        {1, 0, 2},
        {3, 2, 0}
    };
    assert(minimumSpanningTreeCost(g1) == 3.0);

    // Test 2: Single vertex => cost 0
    std::vector<std::vector<int>> g2 = {{0}};
    assert(minimumSpanningTreeCost(g2) == 0.0);

    // Test 3: Four vertices in a line: 0-1 (1), 1-2 (2), 2-3 (3), plus extra edges 0-3 (10) and 1-3 (4)
    // MST uses edges 0-1,1-2,1-3 (or 0-1,1-2,2-3) total 1+2+3=6
    std::vector<std::vector<int>> g3 = {
        {0, 1, 1000000000, 10},
        {1, 0, 2, 4},
        {1000000000, 2, 0, 3},
        {10, 4, 3, 0}
    };
    assert(minimumSpanningTreeCost(g3) == 6.0);

    // Test 4: Complete graph with all weights equal to 5 -> MST has n-1 edges each weight 5
    std::vector<std::vector<int>> g4 = {
        {0, 5, 5, 5},
        {5, 0, 5, 5},
        {5, 5, 0, 5},
        {5, 5, 5, 0}
    };
    assert(minimumSpanningTreeCost(g4) == 15.0);

    // Test 5: Graph with zero-weight edges (allowed)
    std::vector<std::vector<int>> g5 = {
        {0, 0, 2},
        {0, 0, 0},
        {2, 0, 0}
    };
    assert(minimumSpanningTreeCost(g5) == 0.0);

    // Test 6: Larger star graph: center 0 connected to leaves 1,2,3 with weights 7,8,9,
    // leaves not connected to each other (sentinel)
    std::vector<std::vector<int>> g6 = {
        {0, 7, 8, 9},
        {7, 0, 1000000000, 1000000000},
        {8, 1000000000, 0, 1000000000},
        {9, 1000000000, 1000000000, 0}
    };
    assert(minimumSpanningTreeCost(g6) == 24.0);

    return 0;
}
