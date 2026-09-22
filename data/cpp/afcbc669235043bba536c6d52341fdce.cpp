// Write a C++ function that takes a directed acyclic graph (DAG) represented as an adjacency list, a number of vertices `n`, and a source vertex `src`. The function should compute the shortest path distances from `src` to all other vertices using topological sort-based relaxation, and return a `std::vector<int>` of length `n` where each element is the shortest distance from `src` to that vertex, or `INT_MAX` if unreachable. The adjacency list is given as `std::vector<std::vector<std::pair<int,int>>>` where each pair is (neighbor, weight). Assume the graph is a DAG (no cycles) and weights can be any integer (positive, negative, or zero), so Dijkstra's algorithm would fail due to negative edges. The function must handle unreachable vertices and should not modify the input graph.
// The shortest path in a DAG can be found in linear time using a topological order. First, perform a topological sort of the graph using DFS; the topological order ensures that when we process a vertex, all its incoming edges have already been relaxed. Initialize a distance array with `INT_MAX` and set `dist[src] = 0`. Then, pop vertices from the topological sort stack. For each vertex, if its current distance is not `INT_MAX`, relax all its outgoing edges by updating `dist[neighbor] = min(dist[neighbor], dist[node] + weight)`. Since the graph is a DAG, processing vertices in topological order guarantees that we never need to revisit a node—each edge is relaxed exactly once. Edge cases: if `src` is not connected to some vertices, their distances remain `INT_MAX`. If the graph has multiple components, the topological sort still covers all vertices. The time complexity is O(V+E) for DFS and edge relaxation, and space complexity is O(V) for the stack, visited array, and distance vector.
#include <vector>
#include <stack>
#include <climits>
#include <algorithm>

// Perform DFS to generate topological order for the given DAG.
void topologicalDFS(int node, const std::vector<std::vector<std::pair<int,int>>>& adj,
                    std::vector<bool>& visited, std::stack<int>& order) {
    visited[node] = true;
    for (const auto& edge : adj[node]) {
        int neighbor = edge.first;
        if (!visited[neighbor]) {
            topologicalDFS(neighbor, adj, visited, order);
        }
    }
    order.push(node);
}

// Compute shortest paths from src in a DAG using topological order.
std::vector<int> shortestPathDAG(int n, int src, const std::vector<std::vector<std::pair<int,int>>>& adj) {
    // Step 1: Perform topological sort on the DAG.
    std::vector<bool> visited(n, false);
    std::stack<int> topoOrder;
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            topologicalDFS(i, adj, visited, topoOrder);
        }
    }

    // Step 2: Initialize distances.
    std::vector<int> dist(n, INT_MAX);
    dist[src] = 0;

    // Step 3: Relax edges in topological order.
    while (!topoOrder.empty()) {
        int node = topoOrder.top();
        topoOrder.pop();

        if (dist[node] != INT_MAX) {
            for (const auto& edge : adj[node]) {
                int neighbor = edge.first;
                int weight = edge.second;
                if (dist[node] + weight < dist[neighbor]) {
                    dist[neighbor] = dist[node] + weight;
                }
            }
        }
    }

    return dist;
}
#include <cassert>
#include <vector>
#include <climits>

// The solution function is included here for testing (in a real scenario, include the header).
std::vector<int> shortestPathDAG(int n, int src, const std::vector<std::vector<std::pair<int,int>>>& adj);

int main() {
    // Test 1: Simple DAG
    {
        int n = 4;
        std::vector<std::vector<std::pair<int,int>>> adj(n);
        adj[0].push_back({1, 2});
        adj[0].push_back({2, 4});
        adj[1].push_back({2, 1});
        adj[2].push_back({3, 3});
        std::vector<int> result = shortestPathDAG(n, 0, adj);
        std::vector<int> expected = {0, 2, 3, 6};
        assert(result == expected);
    }

    // Test 2: Negative edges, still DAG
    {
        int n = 3;
        std::vector<std::vector<std::pair<int,int>>> adj(n);
        adj[0].push_back({1, 5});
        adj[0].push_back({2, -2});
        adj[1].push_back({2, -3});
        std::vector<int> result = shortestPathDAG(n, 0, adj);
        std::vector<int> expected = {0, 5, -2};
        assert(result == expected);
    }

    // Test 3: Unreachable vertices
    {
        int n = 5;
        std::vector<std::vector<std::pair<int,int>>> adj(n);
        adj[0].push_back({1, 1});
        adj[2].push_back({3, 2});
        std::vector<int> result = shortestPathDAG(n, 0, adj);
        std::vector<int> expected = {0, 1, INT_MAX, INT_MAX, INT_MAX};
        assert(result == expected);
    }

    // Test 4: Single vertex graph
    {
        int n = 1;
        std::vector<std::vector<std::pair<int,int>>> adj(n);
        std::vector<int> result = shortestPathDAG(n, 0, adj);
        std::vector<int> expected = {0};
        assert(result == expected);
    }

    // Test 5: Source not in any edges
    {
        int n = 3;
        std::vector<std::vector<std::pair<int,int>>> adj(n);
        adj[1].push_back({2, 7});
        std::vector<int> result = shortestPathDAG(n, 2, adj);
        std::vector<int> expected = {INT_MAX, INT_MAX, 0};
        assert(result == expected);
    }

    return 0;
}
