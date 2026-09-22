// Given an unweighted, undirected graph with `n` vertices labeled 1 through `n` and `m` edges, write a C++ function that returns the shortest path from a source vertex `s` to a target vertex `t` as a vector of vertex labels in order from `s` to `t`. If no path exists, return a vector containing only `-1`. The graph has no self-loops and no multiple edges between the same pair of vertices, and both `s` and `t` are valid vertex labels.

#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.

int main() {
    // Case 1: Simple path
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4}};
        std::vector<int> result = shortestPath(edges, 4, 3, 1, 4);
        assert(result == std::vector<int>({1,2,3,4}));
    }

    // Case 2: No path exists
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{3,4}};
        std::vector<int> result = shortestPath(edges, 4, 2, 1, 4);
        assert(result == std::vector<int>({-1}));
    }

    // Case 3: Source equals target
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        std::vector<int> result = shortestPath(edges, 3, 2, 2, 2);
        assert(result == std::vector<int>({2}));
    }

    // Case 4: Multiple possible shortest paths, expect one of length 2
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{2,4},{3,4}};
        std::vector<int> result = shortestPath(edges, 4, 4, 1, 4);
        assert(result.size() == 3); // 1, then 2 or 3, then 4
        assert(result.front() == 1 && result.back() == 4);
    }

    // Case 5: Larger graph with alternate routes
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{1,4}};
        std::vector<int> result = shortestPath(edges, 4, 4, 1, 4);
        assert(result.size() == 3); // direct edge 1-4 gives length 2 (vertices 1,4)
        assert(result == std::vector<int>({1,4}));
    }

    // Case 6: Graph where source/target are unreachable because source is isolated
    {
        std::vector<std::pair<int,int>> edges = {{2,3},{3,4}};
        std::vector<int> result = shortestPath(edges, 4, 2, 1, 4);
        assert(result == std::vector<int>({-1}));
    }

    // Case 7: Single vertex graph, source=target
    {
        std::vector<std::pair<int,int>> edges;
        std::vector<int> result = shortestPath(edges, 1, 0, 1, 1);
        assert(result == std::vector<int>({1}));
    }

    // Case 8: Two vertices connected
    {
        std::vector<std::pair<int,int>> edges = {{1,2}};
        std::vector<int> result = shortestPath(edges, 2, 1, 1, 2);
        assert(result == std::vector<int>({1,2}));
    }

    return 0;
}

#include <vector>
#include <queue>
#include <algorithm>

/**
 * @brief Compute the shortest path (by number of edges) between two vertices in an unweighted, undirected graph.
 * @param edges Vector of pairs (u, v) with 1-based vertex labels.
 * @param n     Number of vertices (labels 1..n).
 * @param m     Number of edges (same as edges.size()).
 * @param s     Source vertex (1-based).
 * @param t     Target vertex (1-based).
 * @return Vector of vertex labels (1-based) representing the path from s to t, or {-1} if no path exists.
 */
std::vector<int> shortestPath(const std::vector<std::pair<int, int>>& edges, int n, int m, int s, int t) {
    // Build adjacency list with 0-based indexing.
    std::vector<std::vector<int>> adj(n);
    for (const auto& edge : edges) {
        int u = edge.first - 1;
        int v = edge.second - 1;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // BFS from source to find parents and track visited.
    std::vector<int> parent(n, -1);
    std::vector<bool> visited(n, false);
    std::queue<int> q;
    q.push(s - 1);
    visited[s - 1] = true;

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        for (int nbr : adj[node]) {
            if (!visited[nbr]) {
                visited[nbr] = true;
                parent[nbr] = node;
                q.push(nbr);
            }
        }
    }

    // Reconstruct path from target back to source.
    std::vector<int> path;
    int curr = t - 1;
    while (curr != -1) {
        path.push_back(curr + 1); // convert back to 1-based.
        curr = parent[curr];
    }

    // Check if we actually reached the source.
    if (path.back() != s) {
        return {-1};
    }

    std::reverse(path.begin(), path.end());
    return path;
}

// The shortest path in an unweighted graph is found via breadth-first search (BFS) starting from the source. BFS explores vertices in order of increasing distance, so the first time we reach a vertex, it is via the shortest possible path. To reconstruct the path, we store for each vertex the parent that discovered it during BFS. We then trace backwards from the target using the parent pointers until we reach the source, and reverse the resulting sequence. Edge cases: if the source equals the target, the path is a single vertex. If the target is unreachable, the parent chain from the target will never reach the source; after tracing back, the last element of the reversed path will not equal the source, so we return `{-1}`. Additionally, since vertices are 1-indexed in the problem but internally we use 0-indexing, we convert at input and output boundaries. Time complexity is O(n + m) for BFS, and the path reconstruction is O(path length) ≤ O(n). Space complexity is O(n + m) for the adjacency list and O(n) for auxiliary arrays.
