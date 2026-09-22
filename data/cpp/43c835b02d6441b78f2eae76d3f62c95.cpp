// Write a C++ function `std::vector<int> topologicalOrder(const std::vector<std::pair<int,int>>& edges, int n)` that takes the number of vertices `n` (vertices numbered 1..n) and a list of directed edges (each pair `(u,v)` means an edge from u to v), and returns one valid topological ordering of the vertices as a vector. The input is guaranteed to represent a DAG (no cycles). If multiple orders exist, any valid one is acceptable. The function should handle the case of isolated vertices (no edges) by including them anywhere in the output. Return an empty vector if the input is invalid (not a DAG) — though in this task the input is always a DAG. Use Kahn’s algorithm with an index-based selection of vertices with zero indegree (similar to the snippet’s linear scan), not a priority queue.

The solution uses Kahn’s algorithm: compute indegree for each vertex, then repeatedly find a vertex with zero indegree (scanning from 1 to n each time), append it to the result, mark it as visited (set indegree to -1), and decrement the indegree of all its outgoing neighbors. Since we scan from lowest index each iteration, the output is deterministic (a specific valid topological order). Edge cases: vertices with no incoming or outgoing edges are naturally handled because they start with indegree 0 and are selected early. If the graph has a cycle, no vertex would have zero indegree at some step, so the result size would be less than n; in that case we return an empty vector. The main algorithm runs in O(n + m) for building adjacency and O(n^2) for the selection loop (because each iteration scans up to n vertices). Space complexity is O(n + m) for adjacency lists, indegree, and output. The implementation uses a 2D vector for adjacency (indexed by source vertex) to mirror the original snippet’s `map` structure but with dynamic sizes.

#include <vector>
#include <utility>

// Returns one valid topological order of vertices 1..n for a DAG.
// If a cycle exists, returns an empty vector.
std::vector<int> topologicalOrder(const std::vector<std::pair<int,int>>& edges, int n) {
    // Build adjacency list and compute indegrees.
    std::vector<std::vector<int>> adj(n + 1);
    std::vector<int> indeg(n + 1, 0);
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second;
        adj[u].push_back(v);
        indeg[v]++;
    }

    std::vector<int> order;
    order.reserve(n);

    for (int step = 0; step < n; ++step) {
        // Find the first vertex with zero indegree.
        int chosen = -1;
        for (int v = 1; v <= n; ++v) {
            if (indeg[v] == 0) {
                chosen = v;
                break;
            }
        }
        if (chosen == -1) {
            // Cycle detected: no zero-indegree vertex.
            return {};
        }
        // Mark as visited by setting indegree to -1.
        indeg[chosen] = -1;
        order.push_back(chosen);
        // Remove outgoing edges.
        for (int neighbor : adj[chosen]) {
            indeg[neighbor]--;
        }
    }

    return order;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be defined above.

int main() {
    // Example: 3 vertices, edges 1->2, 1->3, 2->3 => unique order 1,2,3
    std::vector<std::pair<int,int>> edges1 = {{1,2},{1,3},{2,3}};
    auto r1 = topologicalOrder(edges1, 3);
    assert(r1.size() == 3);
    assert(r1[0] == 1 && r1[1] == 2 && r1[2] == 3);

    // Example: isolated vertices (no edges) with n=4 => order 1,2,3,4
    std::vector<std::pair<int,int>> edges2;
    auto r2 = topologicalOrder(edges2, 4);
    assert(r2.size() == 4);
    assert(r2[0] == 1 && r2[1] == 2 && r2[2] == 3 && r2[3] == 4);

    // Example: DAG with branching, multiple valid orders. Check it is a topological order.
    std::vector<std::pair<int,int>> edges3 = {{1,3},{2,3},{3,4}};
    auto r3 = topologicalOrder(edges3, 4);
    assert(r3.size() == 4);
    // Verify each edge u->v appears before v in order.
    for (const auto& e : edges3) {
        int posU = -1, posV = -1;
        for (int i = 0; i < r3.size(); ++i) {
            if (r3[i] == e.first) posU = i;
            if (r3[i] == e.second) posV = i;
        }
        assert(posU != -1 && posV != -1 && posU < posV);
    }

    // Example: self-loop (cycle) => should return empty vector.
    std::vector<std::pair<int,int>> edges4 = {{1,1}};
    auto r4 = topologicalOrder(edges4, 1);
    assert(r4.empty());

    // Example: larger DAG with chain 5->4->3->2->1 and n=5.
    std::vector<std::pair<int,int>> edges5 = {{5,4},{4,3},{3,2},{2,1}};
    auto r5 = topologicalOrder(edges5, 5);
    assert(r5.size() == 5);
    assert(r5[0] == 1 && r5[1] == 2 && r5[2] == 3 && r5[3] == 4 && r5[4] == 5);

    return 0;
}
