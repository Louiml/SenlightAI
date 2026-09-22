Write a C++ function that takes an undirected, unweighted graph as an adjacency list (`std::vector<std::vector<int>>`) and returns a `std::vector<int>` containing the indices of all articulation points (cut vertices) in the graph. The graph is indexed from 0 to V-1, and the result should be sorted in ascending order. The function should correctly handle graphs with isolated vertices (those with degree 0) by excluding them, and should also work for graphs that are disconnected (articulation points must be computed within each connected component).
The main algorithm is an iterative or recursive DFS-based Tarjan’s algorithm for finding articulation points. We perform a depth-first search from each unvisited vertex, maintaining discovery time (`tin`) and the lowest reachable discovery time (`low`) for each vertex. A vertex `u` is an articulation point if:
- It is the root of a DFS tree and has at least two children in that tree.
- It is not the root, and there exists a child `v` such that `low[v] >= tin[u]`.

We must track the root status separately for each DFS tree. Isolated vertices (degree 0) are not articulation points because removing them does not increase the number of connected components (they are already isolated). For disconnected graphs, we restart DFS from every unvisited vertex, and each DFS tree's root is handled independently.

Time complexity: O(V + E) because each vertex and edge is visited once during DFS. Space complexity: O(V) for the recursion stack (or explicit stack in iterative version), and O(V) for auxiliary arrays (tin, low, visited, parent, and result). In the iterative version, we simulate recursion with a stack to avoid stack overflow on large graphs, but the recursive version is simpler and correct for moderate sizes.

Edge cases: single vertex (no articulation points), two vertices connected by one edge (neither is articulation point because removing one leaves a single vertex, which is still connected), a triangle (no articulation points), and a chain (all internal vertices are articulation points, endpoints are not). Also, vertices with degree 0 are never included.
#include <vector>
#include <algorithm>

// Returns a sorted vector of all articulation points (cut vertices) in an undirected graph.
// The graph is given as an adjacency list: graph[u] contains all neighbors of vertex u.
// Vertex indices are 0-based. The result is sorted in ascending order.
std::vector<int> articulationPoints(const std::vector<std::vector<int>>& graph) {
    const int n = static_cast<int>(graph.size());
    std::vector<int> tin(n, -1);   // discovery time of each vertex
    std::vector<int> low(n, 0);    // lowest discovery time reachable from subtree
    std::vector<bool> visited(n, false);
    std::vector<bool> isArticulation(n, false);
    std::vector<int> parent(n, -1);

    int timer = 0;

    // Recursive lambda for DFS
    std::function<void(int)> dfs = [&](int u) {
        visited[u] = true;
        tin[u] = low[u] = timer++;
        int children = 0;

        for (int v : graph[u]) {
            if (!visited[v]) {
                parent[v] = u;
                children++;
                dfs(v);
                // Update low of u based on child v
                low[u] = std::min(low[u], low[v]);

                // Check articulation condition for non-root
                if (parent[u] != -1 && low[v] >= tin[u]) {
                    isArticulation[u] = true;
                }
            } else if (v != parent[u]) {
                // Back edge (cannot be parent's parent edge)
                low[u] = std::min(low[u], tin[v]);
            }
        }

        // Check articulation condition for root
        if (parent[u] == -1 && children > 1) {
            isArticulation[u] = true;
        }
    };

    // Run DFS from every unvisited vertex (handles disconnected graph)
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            // Reset parent for the root of this DFS tree
            parent[i] = -1;
            dfs(i);
        }
    }

    // Collect articulation points and sort
    std::vector<int> result;
    for (int v = 0; v < n; ++v) {
        if (isArticulation[v]) {
            result.push_back(v);
        }
    }
    std::sort(result.begin(), result.end());
    return result;
}
#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Test 1: Simple chain of 3 vertices: 0-1-2
    // Articulation points: 1
    {
        std::vector<std::vector<int>> g(3);
        g[0] = {1};
        g[1] = {0, 2};
        g[2] = {1};
        assert(articulationPoints(g) == std::vector<int>({1}));
    }

    // Test 2: Triangle (cycle of 3): no articulation points
    {
        std::vector<std::vector<int>> g(3);
        g[0] = {1, 2};
        g[1] = {0, 2};
        g[2] = {0, 1};
        assert(articulationPoints(g).empty());
    }

    // Test 3: Two vertices connected by one edge: no articulation points
    {
        std::vector<std::vector<int>> g(2);
        g[0] = {1};
        g[1] = {0};
        assert(articulationPoints(g).empty());
    }

    // Test 4: Single vertex isolated: no articulation points
    {
        std::vector<std::vector<int>> g(1);
        assert(articulationPoints(g).empty());
    }

    // Test 5: Disconnected graph: a chain 0-1-2, and isolated vertex 3, and an edge 4-5
    // Articulation points: only 1 (from the chain), 3, 4, 5 are not.
    {
        std::vector<std::vector<int>> g(6);
        g[0] = {1}; g[1] = {0, 2}; g[2] = {1};
        g[3] = {};
        g[4] = {5}; g[5] = {4};
        assert(articulationPoints(g) == std::vector<int>({1}));
    }

    // Test 6: Star graph centered at 0 with leaves 1,2,3
    // Center 0 is articulation point (root with 3 children), leaves are not.
    {
        std::vector<std::vector<int>> g(4);
        g[0] = {1, 2, 3};
        g[1] = {0};
        g[2] = {0};
        g[3] = {0};
        assert(articulationPoints(g) == std::vector<int>({0}));
    }

    // Test 7: More complex: two cycles sharing a vertex (figure-eight), 0 connected to 1,2, and 1-2, also 0 connected to 3,4, and 3-4
    // No articulation points because every vertex lies on a cycle.
    {
        std::vector<std::vector<int>> g(5);
        g[0] = {1, 2, 3, 4};
        g[1] = {0, 2};
        g[2] = {0, 1};
        g[3] = {0, 4};
        g[4] = {0, 3};
        assert(articulationPoints(g).empty());
    }

    // Test 8: Chain with 4 vertices, articulation points are 1 and 2
    {
        std::vector<std::vector<int>> g(4);
        g[0] = {1};
        g[1] = {0, 2};
        g[2] = {1, 3};
        g[3] = {2};
        auto got = articulationPoints(g);
        std::vector<int> expected = {1, 2};
        assert(got == expected);
    }

    // Test 9: Complete graph K4: no articulation points
    {
        std::vector<std::vector<int>> g(4);
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (i != j) g[i].push_back(j);
            }
        }
        assert(articulationPoints(g).empty());
    }

    // Test 10: Graph where root also has a back edge to ancestor, but root still with only one child
    // Example: 0-1, 1-2, 2-0 (triangle) plus 0-3 (extra leaf). Root 0 has children 1 and 3, so it is articulation point.
    {
        std::vector<std::vector<int>> g(4);
        g[0] = {1, 3, 2};
        g[1] = {0, 2};
        g[2] = {1, 0};
        g[3] = {0};
        assert(articulationPoints(g) == std::vector<int>({0}));
    }

    return 0;
}
