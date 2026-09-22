/*
Write a standalone C++ function `solveDijkstra(int n, const std::vector<std::vector<std::pair<int,int>>>& adj)` that takes the number of nodes `n` (nodes are numbered 1 to `n`) and an adjacency list where each edge is a pair `{neighbor, weight}` (all weights are non-negative integers). The graph is undirected. The function must return the shortest distance from node 1 to node `n` using Dijkstra's algorithm, or -1 if node `n` is unreachable. The function should be self-contained, use appropriate const correctness, and avoid global variables or external dependencies beyond standard libraries.
*/

#include <bits/stdc++.h>
using namespace std;

// Returns shortest distance from node 1 to node n using Dijkstra's algorithm.
// If node n is unreachable, returns -1. Graph is undirected, edges have non-negative weights.
long long shortestPathDijkstra(int n, const vector<vector<pair<int, int>>>& adj) {
    const long long INF = LLONG_MAX / 4;
    vector<long long> dist(n + 1, INF);
    vector<bool> visited(n + 1, false);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

    dist[1] = 0;
    pq.push({0, 1});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        if (u == n) return d; // Early exit

        for (const auto& [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    return (dist[n] == INF) ? -1 : dist[n];
}

int main() {
    // Test 1: Simple graph
    vector<vector<pair<int,int>>> adj1(4);
    adj1[1] = {{2, 5}, {3, 2}};
    adj1[2] = {{1, 5}, {3, 1}, {4, 3}};
    adj1[3] = {{1, 2}, {2, 1}, {4, 6}};
    adj1[4] = {{2, 3}, {3, 6}};
    assert(shortestPathDijkstra(4, adj1) == 6);

    // Test 2: Single node
    vector<vector<pair<int,int>>> adj2(1);
    assert(shortestPathDijkstra(1, adj2) == 0);

    // Test 3: Disconnected graph
    vector<vector<pair<int,int>>> adj3(5);
    adj3[1] = {{2, 1}};
    adj3[2] = {{1, 1}};
    adj3[3] = {{4, 2}};
    adj3[4] = {{3, 2}};
    assert(shortestPathDijkstra(5, adj3) == -1);

    // Test 4: Multiple edges (choose min)
    vector<vector<pair<int,int>>> adj4(3);
    adj4[1] = {{2, 10}, {2, 3}};
    adj4[2] = {{1, 10}, {1, 3}, {3, 1}};
    adj4[3] = {{2, 1}};
    assert(shortestPathDijkstra(3, adj4) == 4);

    // Test 5: Direct edge vs indirect
    vector<vector<pair<int,int>>> adj5(4);
    adj5[1] = {{2, 100}, {3, 1}};
    adj5[2] = {{1, 100}, {4, 1}};
    adj5[3] = {{1, 1}, {4, 1}};
    adj5[4] = {{2, 1}, {3, 1}};
    assert(shortestPathDijkstra(4, adj5) == 2);

    // Test 6: Unreachable with n=1? covered above
    return 0;
}

// The problem is a classic single-source shortest path on an undirected weighted graph. Since weights are non-negative, Dijkstra's algorithm is suitable. We use a priority queue (min-heap) to always expand the node with the smallest current distance. Initialize a distance array with a very large value (e.g., `int64_t` max or a safe large constant), set distance of node 1 to 0, and push `{0, 1}`. While the queue is not empty, pop the smallest distance node. If it's already visited or has a worse distance than recorded, skip. For each neighbor, attempt to relax: if `current_distance + edge_weight < dist[neighbor]`, update and push. Early termination when popping node `n` is possible, but for correctness we can also process all nodes. Edge cases: `n == 1` (distance 0), unreachable nodes (return -1), self-loops (ignored naturally because relaxation won't improve), and multiple edges (minimal weight chosen). Time complexity is O((V+E) log V) where V=n, E=number of edges in adjacency list (each undirected edge appears twice). Space complexity is O(V + E) for the adjacency list and distance array.
