/*
Write a C++ function that takes an undirected graph represented by the number of vertices `v` and a list of undirected edges (pairs of integers from 1 to `v`), and returns a sorted vector of pairs representing all bridges in the graph. A bridge is an edge whose removal increases the number of connected components. The input graph is simple (no self-loops or multiple edges between the same pair of vertices), may be disconnected, and vertices are 1-indexed. The output should contain each bridge exactly once, with the smaller vertex index first, and the entire list sorted in ascending lexicographic order (by first element, then second).
*/
#include <bits/stdc++.h>

/**
 * Finds all bridges in an undirected graph.
 * 
 * @param v Number of vertices (1-indexed).
 * @param edges List of undirected edges as pairs (u, v).
 * @return Sorted vector of bridges, each pair with smaller vertex first.
 */
std::vector<std::pair<int, int>> findBridges(int v, const std::vector<std::pair<int, int>>& edges) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(v + 1);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    std::vector<int> disc(v + 1, 0), low(v + 1, 0), parent(v + 1, 0);
    std::vector<bool> visited(v + 1, false);
    std::vector<std::pair<int, int>> bridges;
    int time = 0;

    // Recursive DFS (using a lambda for simplicity)
    std::function<void(int)> dfs = [&](int u) {
        visited[u] = true;
        disc[u] = low[u] = ++time;
        for (int w : adj[u]) {
            if (!visited[w]) {
                parent[w] = u;
                dfs(w);
                low[u] = std::min(low[u], low[w]);
                if (low[w] > disc[u]) {
                    // Bridge found
                    int a = u, b = w;
                    if (a > b) std::swap(a, b);
                    bridges.emplace_back(a, b);
                }
            } else if (w != parent[u]) {
                // Back edge to ancestor
                low[u] = std::min(low[u], disc[w]);
            }
        }
    };

    // Handle disconnected graph
    for (int i = 1; i <= v; ++i) {
        if (!visited[i]) {
            dfs(i);
        }
    }

    std::sort(bridges.begin(), bridges.end());
    return bridges;
}
#include <bits/stdc++.h>
#include <cassert>

// Include the solution function declaration here (or copy the implementation above)

int main() {
    // Test 1: Simple tree graph (all edges are bridges)
    {
        int v = 6;
        std::vector<std::pair<int, int>> edges = {{1,2}, {2,3}, {2,4}, {4,5}, {4,6}};
        auto bridges = findBridges(v, edges);
        std::vector<std::pair<int, int>> expected = {{1,2}, {2,3}, {2,4}, {4,5}, {4,6}};
        assert(bridges == expected);
    }

    // Test 2: Graph with a cycle (no bridges in cycle)
    {
        int v = 4;
        std::vector<std::pair<int, int>> edges = {{1,2}, {2,3}, {3,1}, {3,4}};
        auto bridges = findBridges(v, edges);
        std::vector<std::pair<int, int>> expected = {{3,4}};
        assert(bridges == expected);
    }

    // Test 3: Disconnected graph with bridges
    {
        int v = 6;
        std::vector<std::pair<int, int>> edges = {{1,2}, {2,3}, {4,5}};
        auto bridges = findBridges(v, edges);
        std::vector<std::pair<int, int>> expected = {{1,2}, {2,3}, {4,5}};
        assert(bridges == expected);
    }

    // Test 4: Isolated vertices (no edges)
    {
        int v = 3;
        std::vector<std::pair<int, int>> edges;
        auto bridges = findBridges(v, edges);
        assert(bridges.empty());
    }

    // Test 5: Single edge
    {
        int v = 2;
        std::vector<std::pair<int, int>> edges = {{1,2}};
        auto bridges = findBridges(v, edges);
        std::vector<std::pair<int, int>> expected = {{1,2}};
        assert(bridges == expected);
    }

    // Test 6: Complete graph K4 (no bridges)
    {
        int v = 4;
        std::vector<std::pair<int, int>> edges = {{1,2}, {1,3}, {1,4}, {2,3}, {2,4}, {3,4}};
        auto bridges = findBridges(v, edges);
        assert(bridges.empty());
    }

    // Test 7: Graph where bridges need sorting (unsorted edge input)
    {
        int v = 5;
        std::vector<std::pair<int, int>> edges = {{3,4}, {1,2}, {2,3}, {4,5}};
        auto bridges = findBridges(v, edges);
        std::vector<std::pair<int, int>> expected = {{1,2}, {2,3}, {4,5}};
        assert(bridges == expected);
        // Note: {3,4} not a bridge because 4-5 is a leaf branch
    }

    // Test 8: Edge case with larger vertex indexing (vertex 10)
    {
        int v = 10;
        std::vector<std::pair<int, int>> edges = {{9,10}, {10,8}, {8,9}};
        auto bridges = findBridges(v, edges);
        assert(bridges.empty()); // cycle among 8,9,10
    }

    // Test 9: Two connected components, one has a bridge
    {
        int v = 7;
        std::vector<std::pair<int, int>> edges = {{1,2}, {2,3}, {3,1}, {5,6}, {6,7}};
        auto bridges = findBridges(v, edges);
        std::vector<std::pair<int, int>> expected = {{5,6}, {6,7}};
        assert(bridges == expected);
    }

    // Test 10: Star graph (center 1, leaves 2..5) – all edges are bridges
    {
        int v = 5;
        std::vector<std::pair<int, int>> edges = {{1,2}, {1,3}, {1,4}, {1,5}};
        auto bridges = findBridges(v, edges);
        std::vector<std::pair<int, int>> expected = {{1,2}, {1,3}, {1,4}, {1,5}};
        assert(bridges == expected);
    }

    return 0;
}
// The problem is solved using Tarjan’s algorithm for finding bridges in an undirected graph. During a depth-first search (DFS) from each unvisited vertex, we maintain three arrays: `disc[u]` gives the discovery time of vertex `u`, `low[u]` gives the smallest discovery time reachable from the subtree of `u` (including back edges to ancestors), and `parent[u]` records the DFS parent. For each tree edge `(u, v)` where `v` is a child of `u`, the edge is a bridge if `low[v] > disc[u]`. This condition holds because no back edge from the subtree rooted at `v` connects to `u` or any ancestor of `u`, meaning removing the edge disconnects `v`'s subtree. Since the graph is undirected, we only consider edges to non-parents to avoid counting undirected edges twice. For each bridge found, we store the pair with smaller endpoint first, then sort the result. Edge cases include disconnected graphs (run DFS from every unvisited vertex), single-vertex or no-edge graphs (no bridges), and graphs with cycles (no bridges within cycles). Time complexity is O(V + E) for DFS, plus O(B log B) for sorting the bridges, where B is the number of bridges. Space complexity is O(V + E) for adjacency list and auxiliary arrays.
