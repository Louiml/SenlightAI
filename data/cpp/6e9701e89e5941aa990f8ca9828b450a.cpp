// Write a standalone C++ function named `hasCycle` that takes the number of vertices `V` (non-negative integer) and an adjacency list representation of a directed graph (a `vector<int> adj[]` or `vector<vector<int>>`), and returns a `bool` indicating whether the graph contains at least one directed cycle. The function must work for any V (including 0 and 1, where a self-loop is a cycle) and handle graphs with multiple edges or disconnected components. The graph is 0-indexed, and each edge is directed from `u` to `v`. Do not modify the input graph; the function should be self-contained with no external dependencies beyond standard headers.
#include <cassert>
#include <vector>

// The function declaration is assumed from the Solution section above.
bool hasCycle(int V, const std::vector<std::vector<int>>& adj);

int main() {
    // Test 1: Empty graph (V=0)
    std::vector<std::vector<int>> adj0;
    assert(hasCycle(0, adj0) == false);

    // Test 2: Single vertex, no edges
    std::vector<std::vector<int>> adj1(1);
    assert(hasCycle(1, adj1) == false);

    // Test 3: Single vertex with self-loop
    std::vector<std::vector<int>> adj2(1);
    adj2[0].push_back(0);
    assert(hasCycle(1, adj2) == true);

    // Test 4: Acyclic graph (5 vertices, no cycle)
    std::vector<std::vector<int>> adj3(5);
    adj3[0].push_back(1);
    adj3[0].push_back(2);
    adj3[1].push_back(3);
    adj3[2].push_back(4);
    assert(hasCycle(5, adj3) == false);

    // Test 5: Simple cycle 0->1->2->0
    std::vector<std::vector<int>> adj4(3);
    adj4[0].push_back(1);
    adj4[1].push_back(2);
    adj4[2].push_back(0);
    assert(hasCycle(3, adj4) == true);

    // Test 6: Disconnected: one acyclic component and one cyclic component
    std::vector<std::vector<int>> adj5(4);
    adj5[0].push_back(1);  // component with 0,1
    adj5[2].push_back(3);  // component with 2,3 -> cycle
    adj5[3].push_back(2);
    assert(hasCycle(4, adj5) == true);

    // Test 7: Multiple edges (parallel edges) in acyclic graph still no cycle
    std::vector<std::vector<int>> adj6(2);
    adj6[0].push_back(1);
    adj6[0].push_back(1); // duplicate edge
    assert(hasCycle(2, adj6) == false);

    return 0;
}
#include <vector>
#include <queue>

// Returns true if the directed graph has a cycle, false otherwise.
// V: number of vertices (0..V-1)
// adj: adjacency list, adj[u] contains all v such that edge u->v exists.
bool hasCycle(int V, const std::vector<std::vector<int>>& adj) {
    std::vector<int> indegree(V, 0);
    for (int u = 0; u < V; ++u) {
        for (int v : adj[u]) {
            ++indegree[v];
        }
    }

    std::queue<int> q;
    for (int i = 0; i < V; ++i) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }

    int processed = 0;
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        ++processed;
        for (int neighbor : adj[node]) {
            --indegree[neighbor];
            if (indegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }

    return processed != V;
}
// The main algorithm uses **Kahn's algorithm** (BFS-based topological sorting). Compute the in-degree of every vertex by iterating over all adjacency lists. Initialize a queue with all vertices having in-degree 0. Then repeatedly remove a vertex from the queue, append it to a count (or list) of processed vertices, and for each outgoing neighbor, decrement its in-degree; if it drops to 0, enqueue it. If the total number of processed vertices equals V, the graph is acyclic (a valid topological order exists); otherwise, at least one cycle exists because some vertices never reach in-degree 0. Important edge cases: (1) V=0 or V=1 with no edges → no cycle; (2) V=1 with a self-loop (edge 0→0) → cycle; (3) disconnected graphs with cycles still detect because the cycle vertices remain unprocessed; (4) multiple edges do not break the in-degree counting. Time complexity is O(V + E) where E is total edges, and space complexity is O(V) for the in-degree array and queue.
