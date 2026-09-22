/*
Write a C++ function that takes the number of vertices `V` and an edge list (pairs of vertices) of an undirected graph and returns `true` if the graph is bipartite, and `false` otherwise. The graph vertices are numbered from 0 to `V-1`. The graph may be disconnected, and the edge list may be empty (a graph with no edges is always bipartite). The function should handle multiple edges and self-loops correctly (a self-loop makes the graph non-bipartite). Do not include a `main` function; provide only the function and necessary header includes.
*/

#include <vector>
#include <queue>

/**
 * Determines whether an undirected graph is bipartite.
 * @param V Number of vertices (0-indexed).
 * @param edges Vector of pairs (u, v) representing undirected edges.
 * @return true if the graph is bipartite, false otherwise.
 */
bool isBipartite(int V, const std::vector<std::pair<int, int>>& edges) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(V);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    // color[i] = -1 (uncolored), 0 or 1 (color of vertex)
    std::vector<int> color(V, -1);

    // Process each connected component
    for (int start = 0; start < V; ++start) {
        if (color[start] != -1) continue;

        // BFS from start
        std::queue<int> q;
        q.push(start);
        color[start] = 0;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : adj[u]) {
                if (color[v] == -1) {
                    color[v] = 1 - color[u]; // opposite color
                    q.push(v);
                } else if (color[v] == color[u]) {
                    return false; // conflict found
                }
            }
        }
    }
    return true;
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: Simple bipartite graph (path of 3 vertices)
    assert(isBipartite(3, {{0,1},{1,2}}) == true);

    // Test 2: Triangle (cycle of odd length) -> not bipartite
    assert(isBipartite(3, {{0,1},{1,2},{2,0}}) == false);

    // Test 3: Disconnected graph, each part bipartite
    assert(isBipartite(4, {{0,1},{2,3}}) == true);

    // Test 4: Empty graph (no edges) -> bipartite
    assert(isBipartite(5, {}) == true);

    // Test 5: Self-loop -> not bipartite
    assert(isBipartite(2, {{0,0}}) == false);

    // Test 6: Multiple edges between same vertices (still bipartite)
    assert(isBipartite(2, {{0,1},{0,1}}) == true);

    // Test 7: Square (cycle of even length) -> bipartite
    assert(isBipartite(4, {{0,1},{1,2},{2,3},{3,0}}) == true);

    // Test 8: Complete graph K3 (triangle) -> not bipartite
    assert(isBipartite(3, {{0,1},{1,2},{0,2}}) == false);

    // Test 9: Single vertex -> bipartite
    assert(isBipartite(1, {}) == true);

    // Test 10: Bipartite graph with isolated vertex and a triangle
    assert(isBipartite(4, {{1,2},{2,3},{3,1}}) == false);

    return 0;
}

// The problem is to determine if an undirected graph is bipartite, i.e., its vertices can be partitioned into two sets such that every edge connects vertices from different sets. The standard approach is to perform a breadth-first search (BFS) or depth-first search (DFS) on each connected component. Initialize a color array with -1 (uncolored). For each uncolored vertex, assign it color 0, then traverse its component: for each neighbor, if it is uncolored, assign the opposite color; if it is already colored with the same color as the current vertex, then the graph is not bipartite. Because the graph may be disconnected, we must loop over all vertices and start a new BFS/DFS for each uncolored one.
//
// Edge cases: a graph with no edges (all vertices separate) is bipartite; a self-loop (an edge from a vertex to itself) makes it non-bipartite because the vertex would need to be in both sets. Multiple edges between the same vertices do not affect bipartiteness. The time complexity is O(V + E) since we visit each vertex and each edge once. The space complexity is O(V) for color array and the BFS queue or DFS stack.
