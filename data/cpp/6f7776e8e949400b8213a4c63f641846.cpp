Write a C++ function `vector<int> shortestPathLengths(int n, int s, const vector<vector<int>>& adj)` that, given the number of nodes `n` (nodes labeled 0..n-1), a source vertex `s`, and an undirected graph represented as an adjacency list, returns a vector of length `n` where the `i`-th element is the shortest number of edges from `s` to node `i`. If a node is unreachable, its distance should be `-1`. The graph is guaranteed to be connected? No — handle unreachable nodes. The graph is undirected and may contain cycles, parallel edges, and self-loops. Use BFS starting from `s`. Assume `s` is valid (0 ≤ s < n), and `adj` has exactly `n` sub-vectors.

#include <cassert>
#include <vector>

std::vector<int> shortestPathLengths(int, int, const std::vector<std::vector<int>>&); // declaration

int main() {
    // Single node, no edges
    {
        std::vector<std::vector<int>> adj = {{}};
        assert(shortestPathLengths(1, 0, adj) == std::vector<int>({0}));
    }
    // Two nodes connected
    {
        std::vector<std::vector<int>> adj = {{1}, {0}};
        assert(shortestPathLengths(2, 0, adj) == std::vector<int>({0, 1}));
        assert(shortestPathLengths(2, 1, adj) == std::vector<int>({1, 0}));
    }
    // Three-node line: 0-1-2, source 0
    {
        std::vector<std::vector<int>> adj = {{1}, {0, 2}, {1}};
        assert(shortestPathLengths(3, 0, adj) == std::vector<int>({0, 1, 2}));
    }
    // Unreachable node: graph 0-1, node 2 isolated
    {
        std::vector<std::vector<int>> adj = {{1}, {0}, {}};
        assert(shortestPathLengths(3, 0, adj) == std::vector<int>({0, 1, -1}));
    }
    // Self-loop and parallel edges, source 0
    {
        std::vector<std::vector<int>> adj = {{0, 1, 1, 2}, {0, 1}, {0}};
        assert(shortestPathLengths(3, 0, adj) == std::vector<int>({0, 1, 1}));
    }
    // Cycle with 4 nodes, source 0
    {
        std::vector<std::vector<int>> adj = {{1, 3}, {0, 2}, {1, 3}, {0, 2}};
        assert(shortestPathLengths(4, 0, adj) == std::vector<int>({0, 1, 2, 1}));
    }
    // Larger example: star with center 0 and leaves 1,2,3
    {
        std::vector<std::vector<int>> adj = {{1, 2, 3}, {0}, {0}, {0}};
        assert(shortestPathLengths(4, 0, adj) == std::vector<int>({0, 1, 1, 1}));
    }
    return 0;
}

#include <vector>
#include <queue>

// Returns shortest edge distances from source s to all nodes.
// Unreachable nodes get -1. Graph is undirected, nodes 0..n-1.
std::vector<int> shortestPathLengths(int n, int s, const std::vector<std::vector<int>>& adj) {
    std::vector<int> dist(n, -1);
    std::queue<int> q;
    dist[s] = 0;
    q.push(s);

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int u : adj[v]) {
            if (dist[u] == -1) {
                dist[u] = dist[v] + 1;
                q.push(u);
            }
        }
    }
    return dist;
}

// The problem is a classic BFS shortest-path in an unweighted graph. We initialize a distance array `dist` of size `n` with `-1` meaning unreached. Set `dist[s] = 0`, push `s` into a queue. While the queue is not empty, pop front `v`, iterate over all neighbors `u` in `adj[v]`; if `dist[u] == -1`, set `dist[u] = dist[v] + 1` and push `u`. Because BFS explores level by level, the first time a node is reached gives the shortest distance. Edge cases: self-loops (ignored because `u == v` would already have `dist[u] != -1`), parallel edges (same effect), and unreachable nodes remain `-1`. Complexity: O(n + m) where m is total edges (sum of all adjacency list sizes), and O(n) auxiliary space for the queue and distance array.
