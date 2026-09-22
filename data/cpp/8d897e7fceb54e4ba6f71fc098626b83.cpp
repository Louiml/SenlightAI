// Write a C++ function `int maximumFlow(int n, int source, int sink, const std::vector<std::tuple<int,int,int>>& edges)` that computes the maximum flow in an undirected graph with `n` vertices numbered from `1` to `n`. The graph edges are given as tuples `(u, v, capacity)`, and each edge represents an undirected connection meaning flow can travel in both directions consuming the same capacity. If there are multiple edges between the same pair of vertices, their capacities are added together. The function must return the value of the maximum flow from `source` to `sink` using the Ford-Fulkerson algorithm with BFS (Edmonds-Karp). Handle the case where source and sink are the same (return 0), and ensure the residual graph is correctly initialized and updated for undirected edges.

The solution uses the standard Edmonds-Karp algorithm, which is Ford-Fulkerson with BFS to find augmenting paths, guaranteeing polynomial time complexity. The graph is stored as an adjacency matrix `capacity` of size `(n+1) x (n+1)` since vertices are 1-indexed. For each undirected edge `(u, v, w)`, both `capacity[u][v]` and `capacity[v][u]` are increased by `w`, representing the ability to send flow in either direction. The residual graph `rGraph` is initialized as a copy of the capacity matrix. In each iteration, BFS is run from `source` to `sink` to find a path where residual capacity is positive. If a path exists, the bottleneck (minimum residual capacity along the path) is found by tracing back via parent pointers, and the residual capacities are updated: subtract the bottleneck on forward edges and add on reverse edges. This process repeats until no augmenting path exists. The maximum flow is the sum of all bottlenecks. Edge cases: if `source == sink`, there is no meaningful flow, so return 0. Multiple edges are handled by summing capacities because we add to the matrix. Self-loops (u==v) are ignored since they never contribute to flow; we can simply skip them when building the capacity matrix. Time complexity is O(V * E^2) in the worst case for Edmonds-Karp (where V is vertices and E edges), but with an adjacency matrix it's O(V^3) per BFS and at most O(V*E) augmentations, so overall O(V^5) worst-case but typically much faster. Space complexity is O(V^2) for the matrices.

#include <bits/stdc++.h>
using namespace std;

// Computes the maximum flow from source to sink in an undirected graph with n vertices (1-indexed).
// edges is a vector of tuples (u, v, capacity) representing undirected edges with the given capacity.
int maximumFlow(int n, int source, int sink, const vector<tuple<int,int,int>>& edges) {
    if (source == sink) {
        return 0;
    }

    // Capacity adjacency matrix
    vector<vector<int>> capacity(n + 1, vector<int>(n + 1, 0));

    // Build capacity for undirected edges (add in both directions)
    for (const auto& [u, v, w] : edges) {
        if (u == v) {
            continue; // self-loop contributes nothing
        }
        capacity[u][v] += w;
        capacity[v][u] += w;
    }

    // Residual graph initialization
    vector<vector<int>> rGraph = capacity;

    int maxFlow = 0;
    vector<int> parent(n + 1, -1);

    // BFS to find an augmenting path
    auto bfs = [&]() -> bool {
        fill(parent.begin(), parent.end(), -1);
        vector<bool> visited(n + 1, false);
        queue<int> q;
        q.push(source);
        visited[source] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 1; v <= n; ++v) {
                if (!visited[v] && rGraph[u][v] > 0) {
                    q.push(v);
                    parent[v] = u;
                    visited[v] = true;
                    if (v == sink) {
                        return true;
                    }
                }
            }
        }
        return false;
    };

    // Edmonds-Karp: repeatedly find shortest augmenting path
    while (bfs()) {
        // Find bottleneck capacity along the path from source to sink
        int pathFlow = INT_MAX;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = min(pathFlow, rGraph[u][v]);
        }

        // Update residual capacities
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            rGraph[u][v] -= pathFlow;
            rGraph[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}

#include <cassert>
#include <vector>
#include <tuple>
using namespace std;

// Forward declaration of the solution function (included from the solution section)
int maximumFlow(int n, int source, int sink, const vector<tuple<int,int,int>>& edges);

int main() {
    // Example 1: Simple path 1-2-3 with capacities 5 and 5, max flow = 5
    vector<tuple<int,int,int>> edges1 = {{1,2,5}, {2,3,5}};
    assert(maximumFlow(3, 1, 3, edges1) == 5);

    // Example 2: Undirected edge with same capacity, flow can go either way
    vector<tuple<int,int,int>> edges2 = {{1,2,10}};
    assert(maximumFlow(2, 1, 2, edges2) == 10);
    assert(maximumFlow(2, 2, 1, edges2) == 10);

    // Example 3: Multiple edges added together
    vector<tuple<int,int,int>> edges3 = {{1,2,3}, {1,2,4}, {2,3,2}};
    assert(maximumFlow(3, 1, 3, edges3) == 5); // total capacity 1->2 is 7, bottleneck 2->3 is 2? Actually max flow = 5? Wait: 1->2 total 7, 2->3 2, but undirected means 2->3 also 2. So max flow from 1 to 3 is min(7,2)=2? Let's compute: capacities: edge1-2 has 3+4=7 both directions, edge2-3 has 2 both directions. So max flow 1->3 is 2. So adjust.
    // Correcting: [[1,2,3],[1,2,4],[2,3,2]] => 1->2 total 7, 2->3 2, so max flow = 2
    assert(maximumFlow(3, 1, 3, edges3) == 2);

    // Example 4: Source equals sink returns 0
    vector<tuple<int,int,int>> edges4 = {{1,2,5}};
    assert(maximumFlow(2, 1, 1, edges4) == 0);

    // Example 5: Disconnected graph
    vector<tuple<int,int,int>> edges5 = {{1,2,5}, {3,4,7}};
    assert(maximumFlow(4, 1, 4, edges5) == 0);

    // Example 6: Complex graph with multiple paths
    // 1->2 capacity 10, 1->3 5, 2->3 5, 2->4 10, 3->4 10, sink=4
    vector<tuple<int,int,int>> edges6 = {{1,2,10}, {1,3,5}, {2,3,5}, {2,4,10}, {3,4,10}};
    // max flow = 15 (1->2 10, 1->3 5, then through to 4)
    assert(maximumFlow(4, 1, 4, edges6) == 15);

    // Example 7: Cycle with same capacity
    vector<tuple<int,int,int>> edges7 = {{1,2,1}, {2,3,1}, {3,1,1}};
    assert(maximumFlow(3, 1, 3, edges7) == 1);

    // Example 8: Large capacity
    vector<tuple<int,int,int>> edges8 = {{1,2,1000000}};
    assert(maximumFlow(2, 1, 2, edges8) == 1000000);

    return 0;
}
