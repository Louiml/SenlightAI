Write a C++ free function `std::vector<int> dfsOfGraph(int V, const std::vector<std::vector<int>>& adj)` that performs a Depth-First Search (DFS) traversal on an undirected graph starting from vertex 0. The graph is represented as an adjacency list, where `adj[u]` contains all vertices directly connected to `u`. The function must return a vector containing the order in which vertices are visited, starting from vertex 0 and following the standard DFS algorithm (visit a node, then recursively visit its unvisited neighbors in the order they appear in the adjacency list). The graph may be disconnected, but the traversal should only visit the connected component containing vertex 0. Assume the graph is undirected, vertices are labeled from 0 to V-1, and V ≥ 1. The function must be self-contained and not modify the input graph.

// The solution uses recursive DFS. We maintain a boolean visited array initialized to false for all V vertices. Starting from vertex 0, we mark it visited, add it to the result vector, then iterate over all neighbors in the adjacency list order. For each unvisited neighbor, we recursively call DFS on it. This naturally mimics the traversal order required. Edge cases include: (1) a graph where vertex 0 has no edges—the function returns just {0}; (2) a disconnected graph where vertex 0 is in a small component—only that component is visited; (3) self-loops and parallel edges are handled because visited check prevents revisits. Since each vertex and edge is processed once (for undirected graph, each edge is seen twice in adjacency but only processed from one side), time complexity is O(V + E). Space complexity is O(V) for the visited array and recursion stack in the worst case (a chain graph), plus O(V) for the output vector.

#include <vector>

// Perform DFS traversal on an undirected graph starting from vertex 0.
// Returns a vector containing the order of visited vertices.
std::vector<int> dfsOfGraph(int V, const std::vector<std::vector<int>>& adj) {
    std::vector<int> result;
    std::vector<bool> visited(V, false);

    // Helper lambda for recursive DFS
    void dfs(int node) {
        visited[node] = true;
        result.push_back(node);
        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                dfs(neighbor);
            }
        }
    }

    // Start traversal from vertex 0
    if (V > 0) {
        dfs(0);
    }

    return result;
}

#include <cassert>
#include <vector>

// The solution function is assumed to be defined above (or included).
// Test cases:
int main() {
    // Test 1: Single vertex, no edges
    {
        int V = 1;
        std::vector<std::vector<int>> adj(V);
        std::vector<int> expected = {0};
        assert(dfsOfGraph(V, adj) == expected);
    }

    // Test 2: Line graph 0-1-2
    {
        int V = 3;
        std::vector<std::vector<int>> adj(V);
        adj[0].push_back(1);
        adj[1].push_back(0);
        adj[1].push_back(2);
        adj[2].push_back(1);
        std::vector<int> expected = {0, 1, 2};
        assert(dfsOfGraph(V, adj) == expected);
    }

    // Test 3: Star graph with center 0
    {
        int V = 4;
        std::vector<std::vector<int>> adj(V);
        adj[0] = {1, 2, 3};
        adj[1].push_back(0);
        adj[2].push_back(0);
        adj[3].push_back(0);
        std::vector<int> expected = {0, 1, 2, 3};
        assert(dfsOfGraph(V, adj) == expected);
    }

    // Test 4: Disconnected graph – vertex 0 isolated, others connected
    {
        int V = 4;
        std::vector<std::vector<int>> adj(V);
        adj[1].push_back(2);
        adj[2].push_back(1);
        // vertex 0 has no edges
        std::vector<int> expected = {0};
        assert(dfsOfGraph(V, adj) == expected);
    }

    // Test 5: Cycle graph 0-1-2-3-0
    {
        int V = 4;
        std::vector<std::vector<int>> adj(V);
        adj[0] = {1, 3};
        adj[1] = {0, 2};
        adj[2] = {1, 3};
        adj[3] = {2, 0};
        std::vector<int> expected = {0, 1, 2, 3};
        assert(dfsOfGraph(V, adj) == expected);
    }

    // Test 6: Complex graph with branching
    {
        int V = 6;
        std::vector<std::vector<int>> adj(V);
        adj[0] = {1, 2};
        adj[1] = {0, 3, 4};
        adj[2] = {0, 5};
        adj[3] = {1};
        adj[4] = {1};
        adj[5] = {2};
        std::vector<int> expected = {0, 1, 3, 4, 2, 5};
        assert(dfsOfGraph(V, adj) == expected);
    }

    // Test 7: Self-loop at vertex 0
    {
        int V = 2;
        std::vector<std::vector<int>> adj(V);
        adj[0].push_back(0); // self-loop
        adj[0].push_back(1);
        adj[1].push_back(0);
        std::vector<int> expected = {0, 1};
        assert(dfsOfGraph(V, adj) == expected);
    }

    // Test 8: Parallel edges between 0 and 1
    {
        int V = 2;
        std::vector<std::vector<int>> adj(V);
        adj[0].push_back(1);
        adj[0].push_back(1); // parallel
        adj[1].push_back(0);
        adj[1].push_back(0);
        std::vector<int> expected = {0, 1};
        assert(dfsOfGraph(V, adj) == expected);
    }

    return 0;
}
