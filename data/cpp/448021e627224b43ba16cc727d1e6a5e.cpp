Write a C++ function `bool isDAG(int n, const std::vector<std::pair<int,int>>& edges)` that takes the number of vertices `n` (labeled `0` to `n-1`) and a list of directed edges, and returns `true` if the graph is a directed acyclic graph (DAG), and `false` if it contains a cycle. The graph may have multiple edges or self-loops. Use Kahn’s algorithm (BFS-based topological sort) to detect whether all vertices can be processed; if the count of processed vertices equals `n`, the graph is a DAG.

The solution builds an adjacency list and computes the indegree of each vertex from the given directed edges. A queue is initialized with all vertices having indegree zero. While the queue is not empty, pop a vertex, decrement the indegree of its neighbors, and push any neighbor whose indegree becomes zero. Track the number of popped vertices. If this count equals `n`, then a topological ordering exists, so the graph is acyclic; otherwise, a cycle exists that prevents processing all vertices. Edge cases include: empty graph (n=0) returns true, isolated vertices (indegree zero) are processed, self-loops (edge from a vertex to itself) increase indegree and prevent that vertex from being processed if it’has no other incoming edges, and multiple edges between the same pair are handled by incrementing indegree accordingly. Time complexity is O(n + E) where E is the number of edges, and space complexity is O(n + E) for the adjacency list and indegree array.

#include <vector>
#include <queue>

// Returns true if the directed graph with 'n' vertices and the given directed edges is a DAG.
bool isDAG(int n, const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency list and compute indegrees
    std::vector<std::vector<int>> adj(n);
    std::vector<int> indegree(n, 0);
    for (const auto& edge : edges) {
        int from = edge.first;
        int to = edge.second;
        // Validate vertex indices (optional but safe)
        if (from < 0 || from >= n || to < 0 || to >= n) return false; // or throw
        adj[from].push_back(to);
        indegree[to]++;
    }

    // Queue vertices with zero indegree
    std::queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }

    int processed = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        processed++;
        // Decrease indegree of neighbors; enqueue when it becomes zero
        for (int v : adj[u]) {
            indegree[v]--;
            if (indegree[v] == 0) {
                q.push(v);
            }
        }
    }

    // If all vertices processed, no cycle; otherwise cycle exists
    return processed == n;
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: Example from snippet - DAG (6 vertices, edges as given)
    std::vector<std::pair<int,int>> edges1 = { {5,0}, {5,2}, {4,0}, {4,1}, {2,3}, {3,1} };
    assert(isDAG(6, edges1) == true);

    // Test 2: Simple cycle of 3 vertices
    std::vector<std::pair<int,int>> edges2 = { {0,1}, {1,2}, {2,0} };
    assert(isDAG(3, edges2) == false);

    // Test 3: Self-loop
    std::vector<std::pair<int,int>> edges3 = { {0,0} };
    assert(isDAG(1, edges3) == false);

    // Test 4: Empty graph with no edges
    std::vector<std::pair<int,int>> edges4 = {};
    assert(isDAG(4, edges4) == true);

    // Test 5: Graph with multiple edges and a cycle
    std::vector<std::pair<int,int>> edges5 = { {0,1}, {0,1}, {1,0} };
    assert(isDAG(2, edges5) == false);

    // Test 6: DAG with isolated vertex and multiple edges (no cycle)
    std::vector<std::pair<int,int>> edges6 = { {0,1}, {0,2}, {1,2} };
    assert(isDAG(4, edges6) == true); // vertex 3 isolated

    // Test 7: DAG with a single edge
    std::vector<std::pair<int,int>> edges7 = { {2,1} };
    assert(isDAG(3, edges7) == true);

    // Test 8: Zero vertices (edge case)
    std::vector<std::pair<int,int>> edges8 = {};
    assert(isDAG(0, edges8) == true);

    // Test 9: Disconnected graph with a cycle in one component
    std::vector<std::pair<int,int>> edges9 = { {0,1}, {1,0}, {2,3} };
    assert(isDAG(4, edges9) == false);

    return 0;
}
