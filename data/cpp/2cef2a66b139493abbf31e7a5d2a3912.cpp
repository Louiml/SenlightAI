Write a C++ function that takes an undirected graph represented as an adjacency list (a vector of vectors of integers, where vertices are numbered from 0 to n-1) and returns the number of connected components in the graph. The graph may be disconnected, have isolated vertices, self-loops, or multiple edges between the same pair of vertices. The function should be named `countConnectedComponents` and must accept the adjacency list as a `const` reference to avoid copying. Your implementation should use depth-first search (DFS) to explore each component, marking visited vertices to ensure each component is counted exactly once. The function must handle an empty graph (adjacency list with zero vertices) gracefully, returning 0. You may use any standard library headers, but the solution must be self-contained and not rely on external code.
#include <cassert>
#include <vector>

// The solution function is declared above (not repeated here).
// This test file includes the necessary function.

int main() {
    // Test 1: Empty graph
    {
        std::vector<std::vector<int>> adj;
        assert(countConnectedComponents(adj) == 0);
    }

    // Test 2: Single isolated vertex
    {
        std::vector<std::vector<int>> adj(1);
        assert(countConnectedComponents(adj) == 1);
    }

    // Test 3: Two isolated vertices
    {
        std::vector<std::vector<int>> adj(2);
        assert(countConnectedComponents(adj) == 2);
    }

    // Test 4: Two vertices connected by an edge
    {
        std::vector<std::vector<int>> adj(2);
        adj[0].push_back(1);
        adj[1].push_back(0);
        assert(countConnectedComponents(adj) == 1);
    }

    // Test 5: Disconnected graph with 3 vertices, one edge between 0-1, and vertex 2 isolated
    {
        std::vector<std::vector<int>> adj(3);
        adj[0].push_back(1);
        adj[1].push_back(0);
        assert(countConnectedComponents(adj) == 2);
    }

    // Test 6: Self-loop only on one vertex
    {
        std::vector<std::vector<int>> adj(2);
        adj[0].push_back(0); // self-loop
        assert(countConnectedComponents(adj) == 2); // vertex 0 and vertex 1 isolated
    }

    // Test 7: Parallel edges between two vertices
    {
        std::vector<std::vector<int>> adj(2);
        adj[0].push_back(1);
        adj[0].push_back(1); // parallel
        adj[1].push_back(0);
        adj[1].push_back(0);
        assert(countConnectedComponents(adj) == 1);
    }

    // Test 8: Chain of 4 vertices (0-1-2-3) all connected
    {
        std::vector<std::vector<int>> adj(4);
        adj[0].push_back(1);
        adj[1].push_back(0);
        adj[1].push_back(2);
        adj[2].push_back(1);
        adj[2].push_back(3);
        adj[3].push_back(2);
        assert(countConnectedComponents(adj) == 1);
    }

    // Test 9: Graph with two components: one triangle (0-1-2-0) and one isolated vertex 3
    {
        std::vector<std::vector<int>> adj(4);
        adj[0].push_back(1);
        adj[0].push_back(2);
        adj[1].push_back(0);
        adj[1].push_back(2);
        adj[2].push_back(0);
        adj[2].push_back(1);
        // vertex 3 has no edges
        assert(countConnectedComponents(adj) == 2);
    }
}
#include <vector>
#include <functional>

// Count the number of connected components in an undirected graph.
// The graph is given as an adjacency list, vertices are 0..n-1.
// The function uses DFS and returns the number of components.
int countConnectedComponents(const std::vector<std::vector<int>>& adj) {
    int n = adj.size();
    if (n == 0) return 0;

    std::vector<bool> visited(n, false);
    int components = 0;

    // Recursive DFS lambda to mark all nodes reachable from start.
    std::function<void(int)> dfs = [&](int v) {
        visited[v] = true;
        for (int neighbor : adj[v]) {
            if (!visited[neighbor]) {
                dfs(neighbor);
            }
        }
    };

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            ++components;
            dfs(i);
        }
    }
    return components;
}
// The solution uses a standard DFS-based connected components algorithm. We initialize a boolean visited array (or vector) of size n (number of vertices) with false. We maintain a counter `components` starting at 0. For each vertex i from 0 to n-1, if it is not visited, we increment the counter and perform a recursive DFS from that vertex, marking all reachable vertices as visited. The DFS function recursively visits all neighbors of the current vertex that have not been visited yet. Since we iterate over all vertices and only start a DFS from unvisited ones, each connected component is counted exactly once. Edge cases: an empty graph (n=0) results in zero components; isolated vertices (no edges) each form their own component; self-loops and parallel edges do not affect the count because DFS handles them naturally. Time complexity is O(V + E) where V is the number of vertices and E is the total number of edges (each vertex and edge is processed once). Space complexity is O(V) for the visited array and the recursion stack in the worst case (a chain graph recursion depth equals V, but typical DFS stack depth is O(V) anyway, so overall O(V) auxiliary space).
