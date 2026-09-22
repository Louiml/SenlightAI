/*
Write a C++ function `std::vector<int> topological_sort_kahn(const std::vector<std::vector<int>>& graph)` that takes a directed graph represented as an adjacency list of `n` nodes (vertices numbered `0` to `n-1`) and returns a topological ordering of its vertices as a vector of integers. If the graph has a cycle (i.e., no valid topological order exists), the function must return an empty vector. The input graph may have isolated nodes (no incoming or outgoing edges) and may be disconnected. Use Kahn's algorithm (BFS-based with an indegree array) rather than depth-first search. The function must not modify the input graph and must be const-correct. Assume the graph is valid (no negative node indices, node indices in range `[0, n)`), but the graph may contain parallel edges (duplicate edges) which should be handled correctly.
*/

#include <vector>
#include <queue>
#include <cstddef>

// Perform a topological sort using Kahn's algorithm (BFS-based).
// Returns a topological ordering of the nodes if one exists, otherwise an empty vector.
// The input graph is an adjacency list: graph[i] contains the successors of node i.
// Nodes are assumed to be numbered from 0 to n-1 where n = graph.size().
// The graph is not modified (const reference). Handles isolated nodes, disconnected graphs,
// and parallel edges correctly. If a cycle is present, returns an empty vector.
std::vector<int> topological_sort_kahn(const std::vector<std::vector<int>>& graph) {
    const std::size_t n = graph.size();
    
    // Compute indegree of each node.
    std::vector<int> indegree(n, 0);
    for (std::size_t u = 0; u < n; ++u) {
        for (int v : graph[u]) {
            ++indegree[v];
        }
    }
    
    // Initialize queue with all nodes having indegree 0.
    std::queue<int> q;
    for (std::size_t i = 0; i < n; ++i) {
        if (indegree[i] == 0) {
            q.push(static_cast<int>(i));
        }
    }
    
    // Process nodes in topological order.
    std::vector<int> result;
    result.reserve(n);
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        result.push_back(u);
        
        // For each successor, decrement indegree and enqueue if it becomes 0.
        for (int v : graph[static_cast<std::size_t>(u)]) {
            --indegree[v];
            if (indegree[v] == 0) {
                q.push(v);
            }
        }
    }
    
    // If not all nodes were processed, a cycle exists.
    if (result.size() != n) {
        return std::vector<int>();
    }
    return result;
}

#include <cassert>
#include <vector>
#include <algorithm>

// The solution function is declared in the "Solution" section above; for testing we re-declare it here.
std::vector<int> topological_sort_kahn(const std::vector<std::vector<int>>& graph);

int main() {
    // Test 1: Simple DAG: 0 -> 1 -> 2
    {
        std::vector<std::vector<int>> g = {{1}, {2}, {}};
        auto order = topological_sort_kahn(g);
        assert(order.size() == 3);
        // Verify topological property: for every edge u->v, u appears before v in order.
        std::vector<int> pos(3);
        for (int i = 0; i < 3; ++i) pos[order[i]] = i;
        assert(pos[0] < pos[1]);
        assert(pos[1] < pos[2]);
    }

    // Test 2: Disconnected graph with isolated nodes: 0 isolated, 1 -> 2
    {
        std::vector<std::vector<int>> g = {{}, {2}, {}};
        auto order = topological_sort_kahn(g);
        assert(order.size() == 3);
        std::vector<int> pos(3);
        for (int i = 0; i < 3; ++i) pos[order[i]] = i;
        assert(pos[1] < pos[2]);  // Edge 1->2 respected
        // Node 0 can appear anywhere.
    }

    // Test 3: Parallel edges: 0 -> 1, and 0 -> 1 again (duplicate)
    {
        std::vector<std::vector<int>> g = {{1, 1}, {}};
        auto order = topological_sort_kahn(g);
        assert(order.size() == 2);
        assert(order[0] == 0);
        assert(order[1] == 1);
    }

    // Test 4: Graph with a cycle: 0 -> 1 -> 0
    {
        std::vector<std::vector<int>> g = {{1}, {0}};
        auto order = topological_sort_kahn(g);
        assert(order.empty());
    }

    // Test 5: Self-loop: 0 -> 0
    {
        std::vector<std::vector<int>> g = {{0}};
        auto order = topological_sort_kahn(g);
        assert(order.empty());
    }

    // Test 6: Empty graph (0 nodes)
    {
        std::vector<std::vector<int>> g = {};
        auto order = topological_sort_kahn(g);
        assert(order.empty());
    }

    // Test 7: Single node with no edges
    {
        std::vector<std::vector<int>> g = {{}};
        auto order = topological_sort_kahn(g);
        assert(order == std::vector<int>{0});
    }

    // Test 8: Larger DAG: 2 -> 3, 0 -> 1, 1 -> 3
    {
        std::vector<std::vector<int>> g = {{1}, {3}, {3}, {}};
        auto order = topological_sort_kahn(g);
        assert(order.size() == 4);
        std::vector<int> pos(4);
        for (int i = 0; i < 4; ++i) pos[order[i]] = i;
        assert(pos[0] < pos[1]);
        assert(pos[1] < pos[3]);
        assert(pos[2] < pos[3]);
    }

    // Test 9: DAG with a node that becomes zero-indegree later after multiple dependencies
    // 0 -> 2, 1 -> 2, 2 -> 3
    {
        std::vector<std::vector<int>> g = {{2}, {2}, {3}, {}};
        auto order = topological_sort_kahn(g);
        assert(order.size() == 4);
        std::vector<int> pos(4);
        for (int i = 0; i < 4; ++i) pos[order[i]] = i;
        assert(pos[0] < pos[2]);
        assert(pos[1] < pos[2]);
        assert(pos[2] < pos[3]);
    }

    // Test 10: Disconnected with multiple components, one of which has a cycle
    {
        std::vector<std::vector<int>> g = {{1}, {0}, {3}, {}}; // 0<->1 cycle, 2->3 DAG
        auto order = topological_sort_kahn(g);
        assert(order.empty());
    }

    return 0;
}

// Kahn's algorithm works by repeatedly removing nodes with zero indegree (no incoming edges) from the graph. The key steps are: (1) Compute the indegree of each node by iterating over all edges; (2) Initialize a queue (or deque) with all nodes that have indegree 0; (3) While the queue is not empty, pop a node, append it to the result vector, and for each of its outgoing neighbors, decrement the neighbor's indegree. If a neighbor's indegree becomes 0, push it onto the queue; (4) After processing all nodes, if the result vector's size equals the total number of nodes, return the result (which is a valid topological ordering). Otherwise, a cycle exists, so return an empty vector. Edge cases include isolated nodes (which have indegree 0 and will appear anywhere in the order), parallel edges (which increment indegree multiple times and must be decremented once per occurrence, which is handled naturally by iterating over all edges), and empty graphs (where the queue starts empty and the result is empty, which is correct). The algorithm runs in O(V + E) time because each vertex is enqueued and dequeued at most once, and each edge is examined once. Space complexity is O(V) for the indegree array and the queue, plus O(V) for the result vector.
