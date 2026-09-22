// Write a C++ free function named `minConnectionsToMakeNetwork` that takes an integer `n` representing the number of computers in a network and a vector of connections represented as pairs of indices `[a, b]`, where each connection is an undirected link between computer `a` and computer `b`. The function should return the minimum number of extra cables needed to connect all computers into a single connected network, or return `-1` if it is impossible to connect all computers even after rearranging and using all existing cables. A connection can be moved from one pair of computers to another pair freely, and the input may contain duplicate connections or self-loops, which should be ignored. The computers are labeled from `0` to `n-1`.
// The problem reduces to counting the number of connected components in an undirected graph of `n` nodes and `m` edges, because to connect `k` connected components into one, we need at least `k - 1` cables. The total number of available cables is `m`, and to connect `n` nodes we need at least `n - 1` cables in a tree. Therefore, if `m < n - 1`, it is impossible and we return `-1`. Otherwise, we build an adjacency list, ignoring self-loops (though duplicates are harmless since BFS still visits correctly), and perform a BFS or DFS from each unvisited node to count the number of connected components. The answer is `numberOfComponents - 1`, since that many existing cables can be repurposed to link the components together. Edge cases: empty connections (multiple components), complete graph (single component → 0 cables), and when `m` is exactly `n-1` but the graph is disconnected (impossible because a forest with `n-1` edges always has exactly one component, but we still handle generically). Time complexity is `O(n + m)` because we visit each node and edge once. Space complexity is `O(n + m)` for the adjacency list and visited array.
#include <vector>
#include <queue>

/**
 * Returns the minimum number of cables needed to connect all computers,
 * or -1 if impossible.
 *
 * @param n The total number of computers (0 to n-1).
 * @param connections Vector of pairs [a, b] representing an undirected cable.
 */
int minConnectionsToMakeNetwork(int n, const std::vector<std::vector<int>>& connections) {
    int m = connections.size();
    if (m < n - 1) {
        return -1;
    }

    // Build adjacency list, ignoring self-loops
    std::vector<std::vector<int>> adj(n);
    for (const auto& edge : connections) {
        int a = edge[0];
        int b = edge[1];
        if (a == b) continue; // self-loop, no effect
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    std::vector<bool> visited(n, false);
    int components = 0;

    // BFS for each unvisited node
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            ++components;
            std::queue<int> q;
            q.push(i);
            visited[i] = true;
            while (!q.empty()) {
                int node = q.front();
                q.pop();
                for (int neighbor : adj[node]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }

    return components - 1;
}
#include <cassert>
#include <vector>

int minConnectionsToMakeNetwork(int n, const std::vector<std::vector<int>>& connections);

int main() {
    // Example 1: 4 computers, 2 cables => need 1 more cable
    assert(minConnectionsToMakeNetwork(4, {{0,1},{0,2}}) == 1);

    // Example 2: 6 computers, 4 cables, but n-1=5 needed => impossible
    assert(minConnectionsToMakeNetwork(6, {{0,1},{0,2},{0,3},{1,2}}) == -1);

    // Example 3: Already connected with redundant cables
    assert(minConnectionsToMakeNetwork(3, {{0,1},{1,2},{0,2}}) == 0);

    // Edge case: single computer, no cables
    assert(minConnectionsToMakeNetwork(1, {}) == 0);

    // Disconnected graph with enough cables to rearrange
    assert(minConnectionsToMakeNetwork(5, {{0,1},{2,3},{2,4},{0,1},{3,4}}) == 1);

    // Self-loops and duplicates are ignored
    assert(minConnectionsToMakeNetwork(3, {{0,0},{1,1},{2,2}}) == -1); // 0 edges actually, m=0

    // Two isolated computers with one cable missing
    assert(minConnectionsToMakeNetwork(2, {}) == -1);

    // Two computers with one cable
    assert(minConnectionsToMakeNetwork(2, {{0,1}}) == 0);

    // All isolated nodes with enough cables? Not possible if m>=n-1, so test a case with 4 nodes and 3 cables that still leave components
    assert(minConnectionsToMakeNetwork(4, {{0,1},{1,2},{0,2}}) == 1); // component {0,1,2} and {3}, need 1

    return 0;
}
