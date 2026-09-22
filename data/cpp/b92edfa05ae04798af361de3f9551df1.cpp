// Write a C++ function `bool isBipartite(int n, const std::vector<std::pair<int,int>>& edges, std::vector<int>& coloring)` that determines whether an undirected graph with `n` vertices (numbered 0 to n-1) is bipartite (i.e., 2-colorable). The graph is given as a list of undirected edges. If the graph is bipartite, the function should return `true` and fill the `coloring` vector (of size `n`) with a valid 2-coloring (each element 0 or 1) such that adjacent vertices have different colors. If the graph is not bipartite, return `false` and leave `coloring` unchanged. The graph may be disconnected, may have isolated vertices, and may contain duplicate edges or self-loops (which make it non-bipartite). Your implementation must use breadth-first search (BFS) as the core algorithm.
The standard approach for checking bipartiteness is to attempt a 2-coloring of the graph using BFS (or DFS). Since the graph may be disconnected, we must iterate over all vertices and start a BFS from any uncolored vertex. Each BFS assigns the start vertex color 0, then explores all reachable vertices, assigning each neighbor the opposite color of its parent. If we ever encounter an already-colored neighbor that has the same color as the current vertex, the graph is not bipartite and we return false immediately. Self-loops are also detected because a vertex will find itself already colored with the same color. Duplicate edges are harmless—they simply cause redundant checks. After processing all components successfully, we fill the global coloring vector and return true. Time complexity is O(n + m) where m is the number of edges, since each vertex and edge is processed once. Space complexity is O(n + m) for the adjacency list and BFS queue.
#include <vector>
#include <queue>

/**
 * Determines if an undirected graph is bipartite using BFS.
 * 
 * @param n number of vertices (0..n-1)
 * @param edges list of undirected edges as pairs (u, v)
 * @param coloring output vector of size n, filled with 0/1 if bipartite
 * @return true if graph is bipartite, false otherwise
 */
bool isBipartite(int n, const std::vector<std::pair<int,int>>& edges, std::vector<int>& coloring) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second;
        // Handle self-loops immediately (they make graph non-bipartite)
        if (u == v) return false;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // color[v] = -1 means uncolored; otherwise 0 or 1
    std::vector<int> color(n, -1);
    std::queue<int> q;

    for (int start = 0; start < n; ++start) {
        if (color[start] != -1) continue; // already colored (part of previous component)

        color[start] = 0;
        q.push(start);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : adj[u]) {
                if (color[v] == -1) {
                    color[v] = 1 - color[u];
                    q.push(v);
                } else if (color[v] == color[u]) {
                    // Conflict found -> not bipartite
                    return false;
                }
            }
        }
    }

    // All components colored successfully
    coloring = color;
    return true;
}
#include <cassert>
#include <vector>
#include <iostream>

// Assume the isBipartite function is defined above

int main() {
    // Test 1: Simple triangle graph (3-cycle) -> not bipartite
    {
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,0}};
        std::vector<int> coloring;
        assert(!isBipartite(3, edges, coloring));
        assert(coloring.empty()); // not modified on failure
    }

    // Test 2: Simple path of length 3 (4 vertices) -> bipartite
    {
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,3}};
        std::vector<int> coloring;
        assert(isBipartite(4, edges, coloring));
        assert(coloring.size() == 4);
        // Check proper coloring: adjacent vertices have different colors
        for (const auto& e : edges) {
            assert(coloring[e.first] != coloring[e.second]);
        }
    }

    // Test 3: Disconnected graph with one bipartite component and one isolated vertex
    {
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2}}; // component: 0-1-2, isolated: 3
        std::vector<int> coloring;
        assert(isBipartite(4, edges, coloring));
        assert(coloring.size() == 4);
        // Isolated vertex can have any color
        assert(coloring[3] == 0 || coloring[3] == 1);
    }

    // Test 4: Self-loop -> not bipartite
    {
        std::vector<std::pair<int,int>> edges = {{0,0}};
        std::vector<int> coloring;
        assert(!isBipartite(1, edges, coloring));
    }

    // Test 5: Duplicate edges and even cycle (4-cycle) -> bipartite
    {
        std::vector<std::pair<int,int>> edges = {{0,1},{0,1},{1,2},{2,3},{3,0}};
        std::vector<int> coloring;
        assert(isBipartite(4, edges, coloring));
        for (const auto& e : edges) {
            assert(coloring[e.first] != coloring[e.second]);
        }
    }

    // Test 6: Empty graph (no edges) -> bipartite, all vertices color 0
    {
        std::vector<std::pair<int,int>> edges;
        std::vector<int> coloring;
        assert(isBipartite(5, edges, coloring));
        assert(coloring.size() == 5);
        for (int c : coloring) {
            assert(c == 0);
        }
    }

    // Test 7: Odd cycle (5-cycle) -> not bipartite
    {
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,3},{3,4},{4,0}};
        std::vector<int> coloring;
        assert(!isBipartite(5, edges, coloring));
    }

    // Test 8: Two disjoint edges (both bipartite) -> true
    {
        std::vector<std::pair<int,int>> edges = {{0,1},{2,3}};
        std::vector<int> coloring;
        assert(isBipartite(4, edges, coloring));
        assert(coloring.size() == 4);
        assert(coloring[0] != coloring[1]);
        assert(coloring[2] != coloring[3]);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
