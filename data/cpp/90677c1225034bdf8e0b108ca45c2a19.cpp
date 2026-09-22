Given an undirected graph with `n` vertices (numbered 1 to n) and `m` edges, where each edge has a positive integer weight, write a C++ function that determines whether there exists an assignment of integer values to all vertices such that for every edge `(u, v)` with weight `w`, the absolute difference between the assigned values of `u` and `v` equals `w`. Your function should return `true` if at least one such assignment exists, and `false` otherwise. This is the "Difference Graph Problem" (DGP): find a vertex labelling satisfying all edge distance constraints.
The problem asks whether a graph can be embedded on the integer line such that each edge's length matches exactly its given weight. A necessary and sufficient condition is that every cycle in the graph must be "consistent": for any cycle with edges of weights \(w_1, w_2, \dots, w_k\), we must be able to assign signs \(\epsilon_i \in \{+1, -1\}\) so that \(\sum_{i} \epsilon_i w_i = 0\) (the cycle closes). This is equivalent to checking that for every cycle, the sum of weights along one traversal direction equals the sum along the reverse traversal. More practically, we can build a spanning forest, assign arbitrary values by DFS (e.g., root at 0, each child gets parent's value ± edge weight), and then verify all non-tree edges: the absolute difference between the two endpoints must equal the edge weight. If any non-tree edge is violated, the graph is infeasible. If all are satisfied, the assignment is valid. Since we can choose signs freely when traversing the tree, for a connected component we can always satisfy all tree edges; the only constraints are the back edges. For a forest, each component is independent. Complexity: DFS visits each vertex and each edge once, so \(O(n + m)\) time and \(O(n + m)\) space using adjacency lists. Edge cases: disconnected graphs (each component solved separately), multiple edges between same vertices, self-loops (only possible if weight is 0, but weights are positive so self-loops are impossible or trivially unsatisfiable), and large weight values (use 64-bit integers). If the graph has a cycle where the sum condition fails, no assignment exists. If the graph is a tree, any assignment works, so always feasible.
#include <vector>
#include <functional>
#include <cmath>
#include <cstdint>

// Edge structure: from, to, weight
struct Edge {
    int from;
    int to;
    int64_t weight;
};

// Determines whether there exists a vertex labelling satisfying |val[u] - val[v]| = w for every edge.
// n: number of vertices (1-indexed), edges: list of undirected edges.
bool hasDifferenceAssignment(int n, const std::vector<Edge>& edges) {
    // Build adjacency list: for each vertex, list of (neighbor, weight)
    std::vector<std::vector<std::pair<int, int64_t>>> adj(n + 1);
    for (const auto& e : edges) {
        adj[e.from].push_back({e.to, e.weight});
        adj[e.to].push_back({e.from, e.weight});
    }

    std::vector<int64_t> value(n + 1, 0);
    std::vector<bool> visited(n + 1, false);
    std::vector<int> parent(n + 1, -1);

    // For each unvisited component, assign values via DFS and check all edges.
    std::function<bool(int)> dfs = [&](int v) {
        visited[v] = true;
        for (const auto& [u, w] : adj[v]) {
            if (!visited[u]) {
                // Explore both possible signs; try + first, then -.
                value[u] = value[v] + w;
                if (dfs(u)) return true;
                value[u] = value[v] - w;
                if (dfs(u)) return true;
                // If neither subtree works, this branch fails; but since we're just checking existence,
                // we need to backtrack carefully. Instead, we use a simpler approach: assign any consistent set
                // and verify all edges (see below). This early attempt is incomplete; real solution uses union-find
                // or assignment with verification.
            }
        }
        return true;
    };

    // Full approach: Since edges are undirected and weights positive, we can assign values by DFS
    // using +w for tree edges, then verify all non-tree edges. The choice of sign for tree edges (always +)
    // is fine because we can always reflect a component's values.
    // Reinitialize and do proper DFS.
    std::vector<int64_t> val(n + 1, 0);
    std::fill(visited.begin(), visited.end(), false);

    for (int start = 1; start <= n; ++start) {
        if (visited[start]) continue;
        visited[start] = true;
        val[start] = 0;
        std::vector<int> stack = {start};
        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();
            for (const auto& [u, w] : adj[v]) {
                if (!visited[u]) {
                    visited[u] = true;
                    val[u] = val[v] + w;  // choose + direction arbitrarily
                    stack.push_back(u);
                }
            }
        }
    }

    // Now verify every edge. If any fails, graph is infeasible.
    for (const auto& e : edges) {
        if (std::llabs(val[e.from] - val[e.to]) != e.weight) {
            return false;
        }
    }
    return true;
}
#include <cassert>
#include <vector>
#include <cstdint>

// Edge structure (same as in solution)
struct Edge {
    int from;
    int to;
    int64_t weight;
};

// Function declaration (must match the solution)
bool hasDifferenceAssignment(int n, const std::vector<Edge>& edges);

int main() {
    // Test 1: Simple triangle with consistent weights (1,1,1) -> feasible
    {
        std::vector<Edge> edges = {{1,2,1},{2,3,1},{3,1,1}};
        assert(hasDifferenceAssignment(3, edges) == true);
    }

    // Test 2: Triangle with infeasible weights (1,1,3) -> no assignment
    {
        std::vector<Edge> edges = {{1,2,1},{2,3,1},{3,1,3}};
        assert(hasDifferenceAssignment(3, edges) == false);
    }

    // Test 3: Single edge -> always feasible
    {
        std::vector<Edge> edges = {{1,2,5}};
        assert(hasDifferenceAssignment(2, edges) == true);
    }

    // Test 4: Disconnected components -> each feasible
    {
        std::vector<Edge> edges = {{1,2,2},{3,4,3}};
        assert(hasDifferenceAssignment(4, edges) == true);
    }

    // Test 5: Square with consistent weights (1,1,1,1) -> feasible
    {
        std::vector<Edge> edges = {{1,2,1},{2,3,1},{3,4,1},{4,1,1}};
        assert(hasDifferenceAssignment(4, edges) == true);
    }

    // Test 6: Square with inconsistent diagonal-independent cycle (1,1,1,3) -> infeasible
    {
        std::vector<Edge> edges = {{1,2,1},{2,3,1},{3,4,3},{4,1,1}};
        assert(hasDifferenceAssignment(4, edges) == false);
    }

    // Test 7: Tree with 4 nodes -> always feasible
    {
        std::vector<Edge> edges = {{1,2,2},{1,3,4},{2,4,3}};
        assert(hasDifferenceAssignment(4, edges) == true);
    }

    // Test 8: Two nodes connected by two edges of different weights -> infeasible (contradictory)
    {
        std::vector<Edge> edges = {{1,2,2},{1,2,3}};
        assert(hasDifferenceAssignment(2, edges) == false);
    }

    // Test 9: Large weight values
    {
        std::vector<Edge> edges = {{1,2,1000000000},{2,3,1000000000},{3,1,2000000000}};
        assert(hasDifferenceAssignment(3, edges) == true);  // 1e9 + 1e9 = 2e9
    }

    // Test 10: Graph with a single vertex and no edges -> trivially feasible
    {
        std::vector<Edge> edges;
        assert(hasDifferenceAssignment(1, edges) == true);
    }

    return 0;
}
