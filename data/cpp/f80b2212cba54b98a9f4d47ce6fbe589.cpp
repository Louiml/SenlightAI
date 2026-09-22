/*
Write a C++ function that takes a directed graph represented by an adjacency list and the number of vertices \( V \), and returns a boolean indicating whether the graph contains a cycle. The graph may contain isolated vertices, multiple edges, and self-loops. The function must use a queue-based (Kahn's algorithm) topological sorting approach with indegree counting, and it should not modify the input graph. Provide a self-contained function suitable for reuse in larger programs.
*/
#include <vector>
#include <queue>

// Returns true if the directed graph contains a cycle.
// The graph is given as an adjacency list with V vertices.
// Uses Kahn's algorithm (BFS topological sort) to detect cycles.
bool hasCycle(const std::vector<std::vector<int>>& adj, int V) {
    // Compute indegree of each vertex.
    std::vector<int> indegree(V, 0);
    for (int u = 0; u < V; ++u) {
        for (int v : adj[u]) {
            ++indegree[v];
        }
    }

    // Queue all vertices with indegree 0.
    std::queue<int> q;
    for (int i = 0; i < V; ++i) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }

    int processed = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        ++processed;

        // Reduce indegree of neighbors.
        for (int v : adj[u]) {
            --indegree[v];
            if (indegree[v] == 0) {
                q.push(v);
            }
        }
    }

    // If not all vertices were processed, there is a cycle.
    return processed != V;
}
#include <cassert>
#include <vector>

// The solution function is declared above (or included here).
bool hasCycle(const std::vector<std::vector<int>>& adj, int V);

int main() {
    // Test 1: Simple acyclic graph (0 -> 1 -> 2)
    {
        std::vector<std::vector<int>> adj(3);
        adj[0].push_back(1);
        adj[1].push_back(2);
        assert(hasCycle(adj, 3) == false);
    }

    // Test 2: Simple cycle (0 -> 1 -> 0)
    {
        std::vector<std::vector<int>> adj(2);
        adj[0].push_back(1);
        adj[1].push_back(0);
        assert(hasCycle(adj, 2) == true);
    }

    // Test 3: Self-loop
    {
        std::vector<std::vector<int>> adj(3);
        adj[1].push_back(1);  // self-loop on vertex 1
        assert(hasCycle(adj, 3) == true);
    }

    // Test 4: Isolated vertices (no edges)
    {
        std::vector<std::vector<int>> adj(5);
        assert(hasCycle(adj, 5) == false);
    }

    // Test 5: Multiple edges and a cycle
    {
        std::vector<std::vector<int>> adj(4);
        adj[0].push_back(1);
        adj[0].push_back(1);  // duplicate edge
        adj[1].push_back(2);
        adj[2].push_back(0);  // forms cycle 0->1->2->0
        assert(hasCycle(adj, 4) == true);
    }

    // Test 6: Larger DAG with disconnected parts
    {
        std::vector<std::vector<int>> adj(6);
        adj[0].push_back(1);
        adj[1].push_back(2);
        adj[3].push_back(4);
        adj[4].push_back(5);
        // vertex 2 and 5 are sinks, all acyclic
        assert(hasCycle(adj, 6) == false);
    }

    // Test 7: Cycle reachable only from part of the graph
    {
        std::vector<std::vector<int>> adj(5);
        adj[0].push_back(1);
        adj[1].push_back(2);
        adj[2].push_back(1);  // cycle 1->2->1
        adj[3].push_back(4);  // separate acyclic part
        assert(hasCycle(adj, 5) == true);
    }

    // Test 8: Empty graph (0 vertices) - edge case
    {
        std::vector<std::vector<int>> adj(0);
        assert(hasCycle(adj, 0) == false);
    }

    return 0;
}
// The solution follows Kahn's algorithm for topological sorting. First, compute the indegree (number of incoming edges) for every vertex by iterating over each vertex's adjacency list and incrementing the indegree of each neighbor. Then, initialize a queue with all vertices whose indegree is 0 — these have no prerequisites. Repeatedly pop a vertex from the queue, increment a counter of processed vertices, and for each neighbor of that vertex, decrement its indegree; if a neighbor's indegree becomes 0, push it onto the queue. After processing all reachable vertices, if the counter is less than the total number of vertices, then some vertices were never queued because they are part of a cycle (or reachable from a cycle). The presence of a cycle is detected by comparing the counter to \( V \). Edge cases include isolated vertices (they will have indegree 0 and be processed lazily), self-loops (they make the vertex's indegree at least 1, so it will never be processed unless another path reduces its indegree, but the self-loop remains, so it won't reach 0), and multiple edges (indegree counts all). Time complexity is \( O(V + E) \) because each vertex and each edge is examined a constant number of times. Space complexity is \( O(V) \) for the indegree array and queue.
