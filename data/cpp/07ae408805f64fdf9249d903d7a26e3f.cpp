Write a C++ function `int isCyclicGraph(int V, const vector<int> adj[])` that determines whether a directed graph with `V` vertices (labeled `0` to `V-1`) contains at least one cycle. The graph is provided as an adjacency list, where `adj[u]` is a vector of all vertices `v` such that a directed edge `u -> v` exists. The function must return `1` if the graph has a cycle and `0` otherwise. The input graph may be disconnected (consisting of multiple weakly connected components), and there may be self-loops (edges from a vertex to itself) which must be treated as cycles. The function must not use any global or static variables, and the adjacency list should not be modified. The solution should use depth-first search with explicit tracking of the recursion stack to detect back edges.
// The standard technique for cycle detection in a directed graph is to perform a depth-first search (DFS) from each unvisited vertex. During DFS, maintain two boolean arrays: `visited` (whether a vertex has ever been encountered in any DFS traversal) and `recStack` (whether a vertex is currently on the active recursion path of the current DFS call). When exploring an edge from current vertex `u` to neighbor `v`: if `v` has not been visited, recursively DFS into `v`; if that recursive call returns `true`, a cycle is found upstream and we propagate `true`. If `v` is already in the current recursion stack (`recStack[v] == true`), then we've found a back edge, which indicates a cycle, so return `true`. If `v` was visited but not in the recursion stack, it's a cross edge or forward edge and we ignore it. After exploring all neighbors of `u`, mark `recStack[u] = false` to remove it from the current path. Since the graph may be disconnected, iterate over all vertices and start a DFS from any unvisited vertex. Self-loops are naturally handled because when we encounter `adj[u]` containing `u`, `recStack[u]` is `true` at that point, so we immediately return `true`. The time complexity is O(V + E) where E is the total number of edges, because each vertex and edge is examined once across all DFS calls. The space complexity is O(V) for the `visited` and `recStack` arrays plus the recursion stack depth, which in the worst case of a chain is O(V).
#include <vector>

// Detects if a directed graph has a cycle using DFS with recursion stack tracking.
// Returns 1 if a cycle exists, otherwise 0.
int isCyclicGraph(int V, const std::vector<int> adj[]) {
    std::vector<bool> visited(V, false);
    std::vector<bool> recStack(V, false);

    // Recursive helper lambda (since C++14, we use a std::function or implement as a separate function)
    // We'll use a nested lambda with a self-reference via std::function for clarity.
    std::function<bool(int)> dfs = [&](int u) -> bool {
        visited[u] = true;
        recStack[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) {
                if (dfs(v)) return true;
            } else if (recStack[v]) {
                // v is on the current DFS path, means a back edge -> cycle
                return true;
            }
        }
        recStack[u] = false;
        return false;
    };

    for (int i = 0; i < V; ++i) {
        if (!visited[i]) {
            if (dfs(i)) return 1;
        }
    }
    return 0;
}
#include <cassert>
#include <vector>
// Assume isCyclicGraph is declared above.

int main() {
    // Test 1: Simple cycle 0->1->2->0
    std::vector<int> adj1[3];
    adj1[0].push_back(1);
    adj1[1].push_back(2);
    adj1[2].push_back(0);
    assert(isCyclicGraph(3, adj1) == 1);

    // Test 2: Acyclic DAG
    std::vector<int> adj2[4];
    adj2[0].push_back(1);
    adj2[0].push_back(2);
    adj2[1].push_back(3);
    adj2[2].push_back(3);
    assert(isCyclicGraph(4, adj2) == 0);

    // Test 3: Self-loop
    std::vector<int> adj3[2];
    adj3[0].push_back(0);
    adj3[1].push_back(0); // also extra edge
    assert(isCyclicGraph(2, adj3) == 1);

    // Test 4: Disconnected components with one cyclic
    std::vector<int> adj4[5];
    // Component A: 0->1
    adj4[0].push_back(1);
    // Component B: 2->3->4->2 (cycle)
    adj4[2].push_back(3);
    adj4[3].push_back(4);
    adj4[4].push_back(2);
    assert(isCyclicGraph(5, adj4) == 1);

    // Test 5: Disconnected components, all acyclic
    std::vector<int> adj5[6];
    adj5[0].push_back(1);
    adj5[2].push_back(3);
    adj5[4].push_back(5);
    assert(isCyclicGraph(6, adj5) == 0);

    // Test 6: Empty graph (no edges)
    std::vector<int> adj6[3];
    assert(isCyclicGraph(3, adj6) == 0);

    // Test 7: Single vertex with no edges
    std::vector<int> adj7[1];
    assert(isCyclicGraph(1, adj7) == 0);

    // Test 8: Two-node cycle 0->1 and 1->0
    std::vector<int> adj8[2];
    adj8[0].push_back(1);
    adj8[1].push_back(0);
    assert(isCyclicGraph(2, adj8) == 1);

    // Test 9: Graph with forward edge and cross edge but no cycle
    // 0->1, 0->2, 1->2
    std::vector<int> adj9[3];
    adj9[0].push_back(1);
    adj9[0].push_back(2);
    adj9[1].push_back(2);
    assert(isCyclicGraph(3, adj9) == 0);

    // Test 10: Long chain with a cycle at the end
    // 0->1->2->3->4->2
    std::vector<int> adj10[5];
    adj10[0].push_back(1);
    adj10[1].push_back(2);
    adj10[2].push_back(3);
    adj10[3].push_back(4);
    adj10[4].push_back(2);
    assert(isCyclicGraph(5, adj10) == 1);

    return 0;
}
