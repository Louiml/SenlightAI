Write a C++ function named `topologicalOrder` that takes an integer `V` (the number of vertices) and a vector of integer vectors `adj` (where `adj[i]` contains the outward neighbors of vertex `i`), and returns a `std::vector<int>` containing the vertices in a valid topological order of the graph. The graph is directed and acyclic (DAG). If the graph contains a cycle, the function may return an incomplete ordering (only containing vertices processed before a cycle is detected), but the input is guaranteed to be a DAG for this task. The vertices are numbered from 0 to V-1. Your function must independently implement the algorithm; do not rely on an external graph library.

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Test 1: Simple chain 0->1->2
    {
        std::vector<std::vector<int>> adj = {{1}, {2}, {}};
        std::vector<int> order = topologicalOrder(3, adj);
        assert(order.size() == 3);
        // Must respect 0 before 1 before 2
        int pos0 = std::find(order.begin(), order.end(), 0) - order.begin();
        int pos1 = std::find(order.begin(), order.end(), 1) - order.begin();
        int pos2 = std::find(order.begin(), order.end(), 2) - order.begin();
        assert(pos0 < pos1 && pos1 < pos2);
    }

    // Test 2: Disconnected vertices with no edges
    {
        std::vector<std::vector<int>> adj = {{}, {}, {}};
        std::vector<int> order = topologicalOrder(3, adj);
        assert(order.size() == 3);
        // All vertices must appear exactly once
        std::vector<int> expected = {0, 1, 2};
        std::sort(order.begin(), order.end());
        assert(order == expected);
    }

    // Test 3: Single vertex
    {
        std::vector<std::vector<int>> adj = {{}};
        std::vector<int> order = topologicalOrder(1, adj);
        assert(order.size() == 1 && order[0] == 0);
    }

    // Test 4: Graph with multiple roots and branches
    // 0->2, 1->2, 2->3
    {
        std::vector<std::vector<int>> adj = {{2}, {2}, {3}, {}};
        std::vector<int> order = topologicalOrder(4, adj);
        assert(order.size() == 4);
        int pos2 = std::find(order.begin(), order.end(), 2) - order.begin();
        int pos3 = std::find(order.begin(), order.end(), 3) - order.begin();
        assert(pos2 < pos3);
        // 0 and 1 must both appear before 2
        int pos0 = std::find(order.begin(), order.end(), 0) - order.begin();
        int pos1 = std::find(order.begin(), order.end(), 1) - order.begin();
        assert(pos0 < pos2 && pos1 < pos2);
    }

    // Test 5: Larger DAG, ensure all vertices present and order valid
    {
        int V = 6;
        std::vector<std::vector<int>> adj(V);
        // 1->0, 2->0, 3->1, 3->2, 4->5
        adj[1].push_back(0);
        adj[2].push_back(0);
        adj[3].push_back(1);
        adj[3].push_back(2);
        adj[4].push_back(5);
        std::vector<int> order = topologicalOrder(V, adj);
        assert(order.size() == V);
        // Build position map
        std::vector<int> pos(V, -1);
        for (int i = 0; i < V; ++i) {
            pos[order[i]] = i;
        }
        // Check all edges respect order
        for (int u = 0; u < V; ++u) {
            for (int v : adj[u]) {
                assert(pos[u] < pos[v]);
            }
        }
    }

    return 0;
}

#include <vector>
#include <queue>

// Return a valid topological ordering of a directed acyclic graph.
// V: number of vertices (0..V-1), adj: adjacency list where adj[i] lists neighbors of vertex i.
std::vector<int> topologicalOrder(int V, const std::vector<std::vector<int>>& adj) {
    std::vector<int> indegree(V, 0);
    for (int i = 0; i < V; ++i) {
        for (int neighbor : adj[i]) {
            ++indegree[neighbor];
        }
    }

    std::queue<int> q;
    for (int i = 0; i < V; ++i) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }

    std::vector<int> result;
    result.reserve(V);
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        result.push_back(node);

        for (int neighbor : adj[node]) {
            --indegree[neighbor];
            if (indegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }

    return result;
}

// The solution uses Kahn's algorithm, which is based on repeatedly removing vertices with no incoming edges. First, compute the indegree (number of incoming edges) for every vertex by iterating over all adjacency lists and incrementing the indegree of each destination. Then, initialize a queue with all vertices that have indegree 0 (these have no prerequisites and can be placed first). While the queue is not empty, pop a vertex, append it to the result vector, and for each outgoing neighbor decrement its indegree; if that neighbor's indegree becomes 0, push it onto the queue. This process continues until the queue is empty. Because the graph is a DAG, the result will contain all V vertices in topological order. Edge cases include a graph with no edges (all vertices have indegree 0, so the order is 0,1,...,V-1), a single vertex, and disconnected graphs where multiple components are processed in arbitrary but valid order. Time complexity is O(V + E) where E is the total number of edges, as each vertex and edge is processed once. Space complexity is O(V) for the indegree array, the queue, and the result vector.
