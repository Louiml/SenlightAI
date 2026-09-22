/*
Given a sequence of `n` directed edges describing a graph where vertices are labeled from `1` to `n`, and an integer `k`, write a C++ function `int countReachable(int n, int k, const std::vector<std::pair<int,int>>& edges)` that returns the number of distinct vertices reachable from vertex `k` by following any number of directed edges (including `k` itself). The graph may contain cycles, duplicate edges, and isolated vertices. The function must handle the case where `n` can be up to 10^5 and the number of edges up to 10^5 efficiently.
*/

#include <vector>
#include <queue>

// Count the number of vertices reachable from start vertex k in a directed graph.
// vertices are labeled 1..n, edges are pairs (from, to).
int countReachable(int n, int k, const std::vector<std::pair<int,int>>& edges) {
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
    }

    std::vector<bool> visited(n + 1, false);
    std::queue<int> q;
    q.push(k);
    visited[k] = true;
    int count = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        ++count;
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
    return count;
}

#include <cassert>
#include <vector>
#include <utility>

// Include or copy the solution function here.

int main() {
    // Single vertex, no edges
    assert(countReachable(1, 1, {}) == 1);
    // Chain 1->2->3, start at 1
    assert(countReachable(3, 1, {{1,2},{2,3}}) == 3);
    // Chain 1->2->3, start at 2
    assert(countReachable(3, 2, {{1,2},{2,3}}) == 2);
    // Cycle 1->2, 2->1, start at 1
    assert(countReachable(2, 1, {{1,2},{2,1}}) == 2);
    // Disconnected graph, start at isolated vertex
    assert(countReachable(5, 3, {{1,2},{4,5}}) == 1);
    // Duplicate edges
    assert(countReachable(3, 1, {{1,2},{1,2},{2,3}}) == 3);
    // Larger test with branching
    // Graph: 1->2, 1->3, 3->4, start at 1
    assert(countReachable(4, 1, {{1,2},{1,3},{3,4}}) == 4);
    // Start vertex appears only as destination
    assert(countReachable(3, 2, {{1,2}}) == 1);
    // Self-loop
    assert(countReachable(2, 1, {{1,1},{2,2}}) == 1);
    return 0;
}

// The problem requires computing the set of vertices reachable from a given starting vertex in a directed graph. The most straightforward approach is to build an adjacency list from the edge list, then perform a breadth-first search (BFS) or depth-first search (DFS) starting from vertex `k`. Since we only need the count of reachable vertices, we can use a boolean vector `visited` of size `n+1` (1-indexed) to mark visited vertices. Starting from `k`, we push it into a queue/stack and mark it visited. For each vertex popped, we iterate over its adjacency list and push any unvisited neighbor, marking it visited. We count every vertex that is marked visited. This handles cycles naturally because visited vertices are never revisited. Duplicate edges are harmless because we only process each neighbor once. Isolated vertices are irrelevant unless they are the start. The time complexity is O(n + m) where m is the number of edges, since each vertex and edge is processed at most once. The space complexity is O(n + m) for the adjacency list and visited array. Edge cases include `k` out of range (though the problem states it's within 1..n), empty edge list, and the graph possibly being disconnected. The algorithm is correct because BFS/DFS explores all reachable vertices.
