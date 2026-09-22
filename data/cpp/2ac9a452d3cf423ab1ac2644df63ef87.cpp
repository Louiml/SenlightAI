/*
Write a C++ function that takes an integer `n` (number of nodes labeled 1 through n), a vector of integer pairs representing undirected edges, and a starting node. The function must return the number of distinct nodes reachable from the starting node via any path (including the starting node itself) — this is the size of the connected component containing the start node. The graph is simple (no self-loops, no duplicate edges) and undirected, with node indices from 1 to `n`. If `n` is 0, return 0. The function must not modify its inputs.
*/

#include <vector>
#include <queue>
#include <cstddef>

// Returns the number of nodes reachable from 'start' in an undirected graph.
// 'n' is the total number of nodes (labeled 1..n). Edges are given as pairs (u,v).
int connectedComponentSize(int n, const std::vector<std::pair<int, int>>& edges, int start) {
    if (n <= 0) {
        return 0;
    }

    // Build adjacency list (1-based indexing)
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        if (u >= 1 && u <= n && v >= 1 && v <= n) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }

    std::vector<bool> visited(n + 1, false);
    std::queue<int> q;
    q.push(start);
    visited[start] = true;
    int componentSize = 1; // start node itself

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        for (int neighbor : adj[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                componentSize++;
                q.push(neighbor);
            }
        }
    }

    return componentSize;
}

#include <cassert>
#include <vector>
#include <utility>

int connectedComponentSize(int n, const std::vector<std::pair<int, int>>& edges, int start);

int main() {
    // Graph: 1-2-3 and 4 isolated
    std::vector<std::pair<int, int>> edges1 = {{1,2}, {2,3}};
    assert(connectedComponentSize(4, edges1, 1) == 3);
    assert(connectedComponentSize(4, edges1, 2) == 3);
    assert(connectedComponentSize(4, edges1, 4) == 1);

    // Empty graph with zero nodes
    assert(connectedComponentSize(0, {}, 0) == 0);

    // Single node, no edges
    assert(connectedComponentSize(1, {}, 1) == 1);

    // Fully connected graph of 5 nodes
    std::vector<std::pair<int, int>> edges2;
    for (int i = 1; i <= 5; ++i) {
        for (int j = i + 1; j <= 5; ++j) {
            edges2.push_back({i, j});
        }
    }
    assert(connectedComponentSize(5, edges2, 3) == 5);

    // Two disconnected components: {1,2} and {3,4,5}
    std::vector<std::pair<int, int>> edges3 = {{1,2}, {3,4}, {4,5}};
    assert(connectedComponentSize(5, edges3, 1) == 2);
    assert(connectedComponentSize(5, edges3, 3) == 3);

    // Graph with a cycle: 1-2-3-1
    std::vector<std::pair<int, int>> edges4 = {{1,2}, {2,3}, {3,1}};
    assert(connectedComponentSize(3, edges4, 2) == 3);

    // Large n with start node not in any edge but n>0
    assert(connectedComponentSize(10, {{1,2}}, 5) == 1);

    // Edge list includes out-of-range nodes (should be ignored)
    std::vector<std::pair<int, int>> edges5 = {{1,2}, {6,7}}; // n=3 so second edge ignored
    assert(connectedComponentSize(3, edges5, 1) == 2);
    assert(connectedComponentSize(3, edges5, 3) == 1);

    return 0;
}

// The problem reduces to counting the vertices in the connected component of the starting node. A standard breadth-first search (BFS) is appropriate: build an adjacency list from the edge list, then traverse from the start node, marking visited nodes and counting each newly discovered node. Initialize a visited array of size `n+1` (to allow 1-based indexing) with `false`, and a queue. Push the start node, mark it visited, and set a counter to 1 (since the start node itself is reachable). While the queue is not empty, pop a node, iterate over its neighbors, and for each unvisited neighbor, mark it visited, increment the counter, and push it. Return the counter. Edge cases: `n=0` should immediately return 0 (no nodes exist); an isolated start node returns 1; a disconnected graph returns the size of only the start's component. The graph is undirected, so edges must be added to both adjacency lists. Time complexity is O(n + m) for building and traversing the graph, where m is the number of edges. Space complexity is O(n + m) for the adjacency list and visited array.
