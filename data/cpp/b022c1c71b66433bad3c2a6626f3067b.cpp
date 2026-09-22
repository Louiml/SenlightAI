// Write a C++ function `bool hasDirectedCycle(int numVertices, const std::vector<std::vector<int>>& adjacencyList)` that determines whether a directed graph contains a cycle. The graph is given by `numVertices` (number of vertices labeled `0` to `numVertices-1`) and an adjacency list where `adjacencyList[u]` contains all vertices `v` such that there is a directed edge from `u` to `v`. The graph may be disconnected, may contain self-loops, and may have parallel edges (though they don’t affect cycle detection). The function must return `true` if the graph contains at least one directed cycle, otherwise `false`. Solve the problem using **Kahn’s algorithm (BFS-based topological sort)**. Handle empty graphs (`numVertices == 0`), single vertices with no edges, self-loops, and multi‑edge cases correctly.

// Kahn’s algorithm performs topological sorting by repeatedly removing vertices with zero in‑degree. If after processing all possible vertices we have visited fewer than `numVertices` vertices, then a cycle exists because some vertices never reach zero in‑degree.  
//
// **Algorithm steps:**  
// 1. Compute the in‑degree of every vertex by iterating over all edges in the adjacency list.  
// 2. Initialize a queue with all vertices that have in‑degree `0`.  
// 3. While the queue is not empty:  
//    - Pop a vertex, increment a counter `processed`.  
//    - For each neighbor of this popped vertex, decrement its in‑degree; if it becomes `0`, push it onto the queue.  
// 4. After the loop, if `processed == numVertices`, the graph is acyclic; otherwise, it contains a cycle.  
//
// **Edge cases:**  
// - Empty graph (`numVertices == 0`): the loop over vertices does nothing, `processed == 0`, so return `false` (no cycle).  
// - Disconnected graph: vertices in separate components are handled independently; the BFS processes each component that has at least one zero‑in‑degree vertex.  
// - Self‑loop `u → u`: this adds 1 to `in‑degree[u]`. If `u` has no other incoming edges, its in‑degree is 1, so it is never pushed initially. It may later be decremented only if another vertex points to it and that vertex is processed; but since it points to itself, after processing all others, `u` remains with in‑degree ≥1, so it is never processed → `processed < numVertices` → cycle detected.  
// - Parallel edges `u → v` twice: each adds 1 to `in‑degree[v]`, so `in‑degree[v] = 2`. When `u` is processed, it decrements twice, correctly bringing it to zero only after both decrements.  
//
// **Time complexity:** O(V + E), where V = `numVertices` and E = total number of edges in the adjacency list (including duplicates). Each vertex is pushed and popped at most once, and each edge is examined once when its source is popped.  
// **Space complexity:** O(V) for the in‑degree array and the queue.

#include <vector>
#include <queue>
#include <cstddef>

// Returns true if a directed graph contains a cycle, false otherwise.
bool hasDirectedCycle(int numVertices, const std::vector<std::vector<int>>& adjacencyList) {
    // Handle empty graph
    if (numVertices == 0) return false;

    // Compute in-degrees
    std::vector<int> inDegree(numVertices, 0);
    for (int u = 0; u < numVertices; ++u) {
        for (int v : adjacencyList[u]) {
            inDegree[v]++;
        }
    }

    // Queue for Kahn's algorithm
    std::queue<int> q;
    for (int i = 0; i < numVertices; ++i) {
        if (inDegree[i] == 0) {
            q.push(i);
        }
    }

    int processed = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        processed++;

        for (int v : adjacencyList[u]) {
            inDegree[v]--;
            if (inDegree[v] == 0) {
                q.push(v);
            }
        }
    }

    // If processed all vertices, graph is acyclic; otherwise, cycle exists
    return processed != numVertices;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple cycle
    std::vector<std::vector<int>> g1 = {{1}, {2}, {0}};
    assert(hasDirectedCycle(3, g1) == true);

    // Test 2: DAG (no cycle)
    std::vector<std::vector<int>> g2 = {{1}, {2}, {}};
    assert(hasDirectedCycle(3, g2) == false);

    // Test 3: Disconnected graph with one cycle
    std::vector<std::vector<int>> g3 = {{1}, {0}, {3}, {}};
    assert(hasDirectedCycle(4, g3) == true);

    // Test 4: Self-loop
    std::vector<std::vector<int>> g4 = {{0}};
    assert(hasDirectedCycle(1, g4) == true);

    // Test 5: Empty graph
    std::vector<std::vector<int>> g5 = {};
    assert(hasDirectedCycle(0, g5) == false);

    // Test 6: Single vertex, no edges
    std::vector<std::vector<int>> g6 = {{}};
    assert(hasDirectedCycle(1, g6) == false);

    // Test 7: Parallel edges causing cycle
    std::vector<std::vector<int>> g7 = {{1, 1}, {0}};
    assert(hasDirectedCycle(2, g7) == true);

    // Test 8: Two disconnected DAGs
    std::vector<std::vector<int>> g8 = {{1}, {}, {3}, {}};
    assert(hasDirectedCycle(4, g8) == false);

    // Test 9: Graph with cycle not reachable from first vertex
    std::vector<std::vector<int>> g9 = {{}, {2, 3}, {1}, {2}};
    assert(hasDirectedCycle(4, g9) == true);

    // Test 10: Large acyclic graph
    std::vector<std::vector<int>> g10 = {{1, 2}, {3}, {3}, {}};
    assert(hasDirectedCycle(4, g10) == false);

    return 0;
}
