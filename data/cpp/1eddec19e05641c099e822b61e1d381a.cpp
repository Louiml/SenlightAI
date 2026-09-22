Given an undirected, unweighted graph represented by `n` vertices labeled 1 through `n` and `e` edges, write a C++ function `vector<int> bfsShortestPath(int n, const vector<pair<int,int>>& edges, int source, int destination)` that returns the shortest path from `source` to `destination` as a vector of vertex labels in order (including both endpoints). If no path exists, return an empty vector. The graph may contain isolated vertices, multiple edges between the same pair, self-loops, and there is no guarantee that `source` or `destination` exist in the provided edges (they are still valid vertices 1..n). The function must use breadth-first search (BFS) and reconstruct the path using parent pointers. The return value must be exactly the sequence of vertices visited along a shortest path; if multiple shortest paths exist, any one is acceptable. The function must not modify any global state and must be safe to call multiple times with different graphs.

#include <cassert>
#include <vector>
#include <utility>

// Declaration of the function under test
std::vector<int> bfsShortestPath(int n, const std::vector<std::pair<int,int>>& edges,
                                 int source, int destination);

int main() {
    // Example: 4 vertices, edges: 1-2, 2-3, 3-4, plus extra edge 1-4
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{1,4}};
        auto path = bfsShortestPath(4, edges, 1, 4);
        assert(path.size() == 2);           // Direct edge 1-4 is shortest
        assert(path.front() == 1);
        assert(path.back() == 4);
        // path could be {1,4}
    }

    // No path exists
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{3,4}};
        assert(bfsShortestPath(4, edges, 1, 4).empty());
    }

    // Source equals destination
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        auto path = bfsShortestPath(3, edges, 2, 2);
        assert(path.size() == 1);
        assert(path[0] == 2);
    }

    // Self-loop and multiple edges
    {
        std::vector<std::pair<int,int>> edges = {{1,1},{1,2},{2,2},{2,3}};
        auto path = bfsShortestPath(3, edges, 1, 3);
        assert(path.size() == 3);
        assert(path[0] == 1);
        assert(path[1] == 2);
        assert(path[2] == 3);
    }

    // Larger graph: known shortest path length
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,6},{1,4},{4,5},{5,6}};
        auto path = bfsShortestPath(6, edges, 1, 6);
        assert(path.size() == 4); // Either 1-2-3-6 or 1-4-5-6
        assert(path.front() == 1);
        assert(path.back() == 6);
    }

    // Isolated vertices
    {
        std::vector<std::pair<int,int>> edges = {};
        assert(bfsShortestPath(5, edges, 1, 5).empty());
    }

    // Path with only two vertices connected by edge
    {
        std::vector<std::pair<int,int>> edges = {{2,5}};
        auto path = bfsShortestPath(5, edges, 2, 5);
        assert(path.size() == 2);
        assert(path[0] == 2);
        assert(path[1] == 5);
    }

    // Destination unreachable but source has neighbors elsewhere
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{4,5}};
        assert(bfsShortestPath(5, edges, 1, 5).empty());
    }

    // Ensure no modification to graph after multiple calls
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{1,3}};
        auto p1 = bfsShortestPath(3, edges, 1, 3);
        auto p2 = bfsShortestPath(3, edges, 1, 3);
        assert(p1 == p2);
    }

    // Zero edges but source==destination
    {
        auto path = bfsShortestPath(2, {}, 1, 1); // empty edge list
        assert(path.size() == 1);
        assert(path[0] == 1);
    }

    return 0;
}

#include <vector>
#include <queue>
#include <algorithm>
#include <utility>

// Returns the shortest path from source to destination in an undirected graph.
// Edge list edges contains pairs (u,v) where vertices are 1-indexed.
// If no path exists, returns an empty vector.
std::vector<int> bfsShortestPath(int n, const std::vector<std::pair<int,int>>& edges,
                                 int source, int destination) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<bool> visited(n + 1, false);
    std::vector<int> parent(n + 1, -1);

    std::queue<int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    if (!visited[destination]) {
        return {};
    }

    // Reconstruct path
    std::vector<int> path;
    int x = destination;
    while (x != -1) {
        path.push_back(x);
        x = parent[x];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

// The core algorithm is standard BFS on an unweighted graph. Build an adjacency list from the edge list. Initialize a visited boolean array and a parent integer array to -1. Start BFS from `source`: set visited[source]=true, parent[source]=-1, and push into a queue. While the queue is not empty, pop the front node `u`, iterate over all neighbors `v`; if not visited, mark visited, set parent[v]=u, and push. Since BFS explores nodes in order of increasing distance from source, the first time we reach `destination` (or after BFS completes), if `visited[destination]` is true, reconstruct the path by following parent pointers from `destination` back to `source`, then reverse. If `visited[destination]` is false, return an empty vector. Complexity: O(n + e) time for BFS and path reconstruction (O(path length) ≤ O(n)), and O(n + e) space for adjacency list, visited, parent, and queue. Edge cases: source equals destination (path is just {source}); source or destination may be isolated with no edges, resulting in empty path unless they are the same; self-loops are handled naturally; multiple edges do not affect correctness; vertices with no edges are simply never visited.
