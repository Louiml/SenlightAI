// Write a C++ function that performs a topological sort of a directed acyclic graph (DAG) using Kahn’s algorithm and returns the sorted order as a `std::vector<int>`. The function must take the number of vertices `V` and a graph represented as an adjacency list (`std::vector<std::vector<int>>`), and it should output the topological ordering. If the graph contains a cycle, the function should return an empty vector to indicate that no valid topological order exists. The input graph may have multiple edges and isolated vertices. Vertices are numbered from 0 to V-1. You must not modify the input graph or rely on any global state.
// Kahn’s algorithm processes vertices with indegree zero first. Compute the indegree of every vertex by iterating over all edges. Initialize a queue (or any FIFO structure) with vertices having indegree zero. Repeatedly remove a vertex from the queue, append it to the result vector, decrement the indegree of all its neighbors, and enqueue any neighbor whose indegree becomes zero. If the result vector size equals `V`, the graph is a DAG and the result is a valid topological order. If the size is less than `V`, a cycle exists (since at least one vertex never reaches indegree zero). Edge cases include: a graph with no edges (all vertices indegree zero, any order works), a graph with isolated vertices, and duplicate edges (which correctly increase indegree multiple times). Since the input graph is provided as a const reference, we copy the adjacency list internally if needed or work with local indegree counts. Time complexity is O(V + E) because each vertex and each edge is processed once. Space complexity is O(V) for the indegree array and queue.
#include <vector>
#include <queue>

// Perform topological sort of a DAG using Kahn's algorithm.
// Returns the topological order as a vector<int>.
// If the graph contains a cycle, returns an empty vector.
std::vector<int> topologicalSort(const std::vector<std::vector<int>>& adj) {
    int V = adj.size();
    std::vector<int> indegree(V, 0);
    
    // Compute indegree of each vertex
    for (int i = 0; i < V; ++i) {
        for (int neighbor : adj[i]) {
            ++indegree[neighbor];
        }
    }
    
    // Queue vertices with indegree zero
    std::queue<int> q;
    for (int i = 0; i < V; ++i) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }
    
    std::vector<int> result;
    result.reserve(V);
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        result.push_back(u);
        
        for (int neighbor : adj[u]) {
            --indegree[neighbor];
            if (indegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }
    
    // If result size is less than V, a cycle exists
    if ((int)result.size() != V) {
        return {};
    }
    return result;
}
#include <cassert>
#include <vector>
#include <set>

int main() {
    // Test case 1: Simple DAG 0->1, 0->2, 1->3, 2->3
    {
        std::vector<std::vector<int>> adj = {{1,2},{3},{3},{}};
        std::vector<int> result = topologicalSort(adj);
        assert(result.size() == 4);
        std::set<std::vector<int>> validOrders = {{0,1,2,3}, {0,2,1,3}};
        assert(validOrders.count(result) == 1);
    }
    
    // Test case 2: Graph with a cycle 0->1, 1->0
    {
        std::vector<std::vector<int>> adj = {{1},{0}};
        std::vector<int> result = topologicalSort(adj);
        assert(result.empty());
    }
    
    // Test case 3: Graph with isolated vertices and no edges (V=3)
    {
        std::vector<std::vector<int>> adj = {{},{},{}};
        std::vector<int> result = topologicalSort(adj);
        assert(result.size() == 3);
        std::set<int> vertices(result.begin(), result.end());
        assert(vertices == std::set<int>({0,1,2}));
    }
    
    // Test case 4: Linear chain 0->1->2->3
    {
        std::vector<std::vector<int>> adj = {{1},{2},{3},{}};
        std::vector<int> result = topologicalSort(adj);
        assert(result == std::vector<int>({0,1,2,3}));
    }
    
    // Test case 5: Duplicate edges 0->1 (twice)
    {
        std::vector<std::vector<int>> adj = {{1,1},{}};
        std::vector<int> result = topologicalSort(adj);
        assert(result.size() == 2);
        assert(result[0] == 0 && result[1] == 1);
    }
    
    // Test case 6: Single vertex with no edges
    {
        std::vector<std::vector<int>> adj = {{}};
        std::vector<int> result = topologicalSort(adj);
        assert(result == std::vector<int>({0}));
    }
    
    // Test case 7: Self-loop (cycle) at vertex 0
    {
        std::vector<std::vector<int>> adj = {{0}};
        std::vector<int> result = topologicalSort(adj);
        assert(result.empty());
    }
    
    // Test case 8: DAG with multiple valid orders, check all vertices appear once
    {
        std::vector<std::vector<int>> adj = {{2,3},{2,3},{4},{4},{}};
        std::vector<int> result = topologicalSort(adj);
        assert(result.size() == 5);
        std::set<int> vertices(result.begin(), result.end());
        assert(vertices == std::set<int>({0,1,2,3,4}));
        // verify order is valid: 0 before 2 and 3, 1 before 2 and 3, 2 before 4, 3 before 4
        int pos0 = std::find(result.begin(), result.end(), 0) - result.begin();
        int pos1 = std::find(result.begin(), result.end(), 1) - result.begin();
        int pos2 = std::find(result.begin(), result.end(), 2) - result.begin();
        int pos3 = std::find(result.begin(), result.end(), 3) - result.begin();
        int pos4 = std::find(result.begin(), result.end(), 4) - result.begin();
        assert(pos0 < pos2 && pos0 < pos3);
        assert(pos1 < pos2 && pos1 < pos3);
        assert(pos2 < pos4 && pos3 < pos4);
    }
    
    return 0;
}
