Write a C++ function named `topologicalSortOrder` that takes a directed acyclic graph represented as an adjacency list (a `vector<vector<int>>`) and returns a `vector<int>` containing a valid topological ordering of its vertices. The input graph may have vertices with no outgoing edges, isolated vertices, or disconnected components. If the graph contains a cycle, return an empty vector. The vertices are labeled from `0` to `n-1`, where `n` is the number of vertices. The function must be efficient for large graphs with up to 100,000 vertices and 1,000,000 edges, using Kahn's algorithm (BFS-based topological sort) and must not modify the input graph.
#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Test 1: Simple DAG: 0->1, 0->2, 1->3, 2->3
    {
        std::vector<std::vector<int>> graph = {{1, 2}, {3}, {3}, {}};
        std::vector<int> order = topologicalSortOrder(graph);
        assert(order.size() == 4);
        // Check ordering constraints
        auto pos = [&](int x) { return std::find(order.begin(), order.end(), x) - order.begin(); };
        assert(pos(0) < pos(1) && pos(0) < pos(2) && pos(1) < pos(3) && pos(2) < pos(3));
    }
    
    // Test 2: Disconnected components with isolated vertex
    {
        std::vector<std::vector<int>> graph = {{1}, {}, {3}, {}};
        std::vector<int> order = topologicalSortOrder(graph);
        assert(order.size() == 4);
        assert(order[0] == 0 || order[0] == 1 || order[0] == 2 || order[0] == 3); // any order ok, just check size and constraints
        auto pos = [&](int x) { return std::find(order.begin(), order.end(), x) - order.begin(); };
        assert(pos(0) < pos(1)); // edge 0->1
        assert(pos(2) < pos(3)); // edge 2->3
    }
    
    // Test 3: Single vertex
    {
        std::vector<std::vector<int>> graph = {{}};
        std::vector<int> order = topologicalSortOrder(graph);
        assert(order.size() == 1 && order[0] == 0);
    }
    
    // Test 4: Cycle detection
    {
        std::vector<std::vector<int>> graph = {{1}, {2}, {0}}; // 0->1->2->0
        std::vector<int> order = topologicalSortOrder(graph);
        assert(order.empty());
    }
    
    // Test 5: More complex DAG with multiple zero-indegree starts
    {
        std::vector<std::vector<int>> graph = {{}, {2}, {3}, {4}, {}};
        std::vector<int> order = topologicalSortOrder(graph);
        assert(order.size() == 5);
        auto pos = [&](int x) { return std::find(order.begin(), order.end(), x) - order.begin(); };
        assert(pos(1) < pos(2) && pos(2) < pos(3) && pos(3) < pos(4));
    }
    
    // Test 6: Edge case with self-loop (cycle)
    {
        std::vector<std::vector<int>> graph = {{0}}; // self-loop
        std::vector<int> order = topologicalSortOrder(graph);
        assert(order.empty());
    }
    
    // Test 7: Large linear chain
    {
        std::vector<std::vector<int>> graph(1000);
        for (int i = 0; i < 999; ++i) {
            graph[i].push_back(i + 1);
        }
        std::vector<int> order = topologicalSortOrder(graph);
        assert(order.size() == 1000);
        for (int i = 0; i < 1000; ++i) {
            assert(order[i] == i); // must be exactly the chain order
        }
    }
    
    return 0;
}
#include <vector>
#include <queue>
#include <algorithm>

// Return a topological ordering of vertices in a directed acyclic graph.
// If the graph has a cycle, return an empty vector.
std::vector<int> topologicalSortOrder(const std::vector<std::vector<int>>& graph) {
    const int n = static_cast<int>(graph.size());
    std::vector<int> indegree(n, 0);
    
    for (int v = 0; v < n; ++v) {
        for (int to : graph[v]) {
            ++indegree[to];
        }
    }
    
    std::queue<int> q;
    for (int v = 0; v < n; ++v) {
        if (indegree[v] == 0) {
            q.push(v);
        }
    }
    
    std::vector<int> result;
    result.reserve(n);
    
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        result.push_back(current);
        
        for (int neighbor : graph[current]) {
            --indegree[neighbor];
            if (indegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }
    
    if (static_cast<int>(result.size()) != n) {
        return {};
    }
    return result;
}
// The solution uses Kahn's algorithm, which relies on indegrees of vertices. First, compute the indegree of every vertex by iterating over all adjacency lists. Then, push all vertices with indegree zero into a queue. Repeatedly pop a vertex from the queue, append it to the result vector, and for each of its outgoing neighbors, decrement their indegree. If a neighbor's indegree becomes zero, push it into the queue. If after processing all vertices the result size equals `n`, a valid topological order exists; otherwise, the graph has a cycle, and we return an empty vector. This handles isolated vertices (which start with indegree zero and are output immediately), disconnected components (they are processed independently), and cycles (detected by the size mismatch). Time complexity is O(V+E) since each vertex is processed once and each edge examined once. Space complexity is O(V) for the indegree array and queue, plus O(V) for the result vector.
