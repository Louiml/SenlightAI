Write a C++ function that takes the number of vertices `V` and an adjacency list `adj` of a weighted, undirected, connected graph, where `adj[i]` is a vector of vectors, each inner vector containing two integers `{neighbor, weight}`. The function must compute and return the sum of the weights of the edges in the Minimum Spanning Tree (MST) of the graph. The graph is guaranteed to be connected, but not necessarily complete, and edge weights can be positive or zero (non-negative integers). The graph may contain up to 10^5 vertices and up to 10^6 edges. The function should handle large inputs efficiently and be robust against duplicate edges between the same pair of vertices (in which case the smallest weight edge should effectively be considered by the algorithm) and self-loops (which can be ignored since they do not affect the MST).
#include <cassert>
#include <vector>
using namespace std;

// Function declaration (must match the solution's signature)
int minimumSpanningTreeWeight(int V, const vector<vector<int>> adj[]);

int main() {
    // Test 1: Example from the prompt
    {
        int V = 3;
        vector<vector<int>> adj[V];
        adj[0].push_back({1, 5});
        adj[1].push_back({0, 5});
        adj[1].push_back({2, 3});
        adj[2].push_back({1, 3});
        adj[0].push_back({2, 1});
        adj[2].push_back({0, 1});
        assert(minimumSpanningTreeWeight(V, adj) == 4);
    }

    // Test 2: Triangle with equal weights -> any two edges sum = 4
    {
        int V = 3;
        vector<vector<int>> adj[V];
        adj[0].push_back({1, 2});
        adj[1].push_back({0, 2});
        adj[1].push_back({2, 2});
        adj[2].push_back({1, 2});
        adj[0].push_back({2, 2});
        adj[2].push_back({0, 2});
        assert(minimumSpanningTreeWeight(V, adj) == 4);
    }

    // Test 3: Linear graph with 4 vertices and zero-weight edges
    {
        int V = 4;
        vector<vector<int>> adj[V];
        adj[0].push_back({1, 0});
        adj[1].push_back({0, 0});
        adj[1].push_back({2, 5});
        adj[2].push_back({1, 5});
        adj[2].push_back({3, 1});
        adj[3].push_back({2, 1});
        // MST: edges (0-1) weight 0, (1-2) weight 5, (2-3) weight 1 => total 6
        assert(minimumSpanningTreeWeight(V, adj) == 6);
    }

    // Test 4: Duplicate edges (including a cheaper duplicate) and a self-loop
    {
        int V = 2;
        vector<vector<int>> adj[V];
        adj[0].push_back({1, 10});
        adj[1].push_back({0, 10});
        adj[0].push_back({1, 3}); // cheaper duplicate
        adj[1].push_back({0, 3});
        adj[0].push_back({0, 7}); // self-loop, ignored
        // MST must use weight 3 edge => total 3
        assert(minimumSpanningTreeWeight(V, adj) == 3);
    }

    // Test 5: Single vertex (connected trivially)
    {
        int V = 1;
        vector<vector<int>> adj[V]; // no edges
        assert(minimumSpanningTreeWeight(V, adj) == 0);
    }

    // Test 6: Larger complete graph with 4 vertices, all edge weights = 1 (MST has 3 edges)
    {
        int V = 4;
        vector<vector<int>> adj[V];
        for (int i = 0; i < V; ++i) {
            for (int j = i + 1; j < V; ++j) {
                adj[i].push_back({j, 1});
                adj[j].push_back({i, 1});
            }
        }
        assert(minimumSpanningTreeWeight(V, adj) == 3);
    }

    return 0;
}
#include <bits/stdc++.h>
using namespace std;

// Computes the sum of MST edge weights using Prim's algorithm.
int minimumSpanningTreeWeight(int V, const vector<vector<int>> adj[]) {
    // Min-heap of {weight, node}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> visited(V, 0);
    int totalWeight = 0;

    // Start from vertex 0 with weight 0
    pq.push({0, 0});

    while (!pq.empty()) {
        auto [wt, node] = pq.top();
        pq.pop();

        if (visited[node]) continue; // Skip if already added to MST (handles duplicates and self-loops)

        visited[node] = 1;
        totalWeight += wt;

        for (const auto& edge : adj[node]) {
            int neighbor = edge[0];
            int edgeWeight = edge[1];
            if (!visited[neighbor]) {
                pq.push({edgeWeight, neighbor});
            }
        }
    }
    return totalWeight;
}
// The problem asks for the total weight of a Minimum Spanning Tree (MST) in a connected, undirected, weighted graph. The most efficient and straightforward approach for dense or sparse graphs with potentially many edges is Prim’s algorithm using a min-heap (priority queue). Initialize a min-heap that stores pairs `{weight, node}` and start from an arbitrary vertex (e.g., vertex 0) with weight 0. Maintain a boolean visited array. While the heap is not empty, pop the smallest weight pair. If the node is already visited, skip it (this handles duplicate edges). Otherwise, mark it visited, add the weight to the sum, and push all its unvisited neighbors into the heap with their edge weights. Because Prim’s algorithm processes each vertex once and each edge at most once (each edge is pushed from both endpoints, but only when the source endpoint is visited, and each push is processed once), the time complexity is O((V + E) log V) using a priority queue. The space complexity is O(V + E) for the adjacency list and the visited array, plus O(V) for the heap in the worst case. Edge cases: the graph is connected by guarantee, so the algorithm will visit all vertices; self-loops are safely ignored because the node will already be visited when popped, or they never help reduce MST weight; duplicate edges simply cause multiple heap entries, but the visited check ensures only the smallest gets added. The solution uses `const` references for the adjacency list and proper `const` correctness; no `main` is included.
