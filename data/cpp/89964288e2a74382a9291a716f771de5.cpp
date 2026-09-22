/*
Write a C++ function `std::vector<int> findEulerianCircuit(int n, const std::vector<std::pair<int,int>>& edges)` that takes the number of vertices `n` (vertices are numbered from 1 to `n`) and a list of undirected edges, and returns a vector containing a valid Eulerian circuit (a closed trail that visits every edge exactly once and returns to the starting vertex) if one exists. If no Eulerian circuit exists, return an empty vector. The input graph is connected and may contain multiple edges between the same pair of vertices. The returned circuit should be a sequence of vertices where each consecutive pair forms an edge, the first and last vertices are the same, and the circuit uses every edge exactly once. You must handle graphs with up to `n = 100,000` vertices and `m = 200,000` edges efficiently. If the graph has vertices with odd degree, no Eulerian circuit exists; check this and also verify that all edges are reachable from the start vertex (the graph is connected when considering only vertices that have at least one edge). The solution must avoid recursion depth issues by using an iterative stack-based method.
*/
#include <vector>
#include <list>
#include <stack>
#include <algorithm>

// Find an Eulerian circuit in an undirected graph, or return empty if none exists.
// Vertices are numbered 1..n. Edges are given as pairs (u, v) with u != v (may repeat).
std::vector<int> findEulerianCircuit(int n, const std::vector<std::pair<int,int>>& edges) {
    int m = static_cast<int>(edges.size());
    if (m == 0) {
        // No edges, trivial circuit of a single vertex if n >= 1
        if (n >= 1) return {1};
        return {};
    }

    // Build adjacency: for each vertex, store list of edge indices.
    std::vector<std::list<int>> adj(n + 1);
    std::vector<int> degree(n + 1, 0);
    for (int i = 0; i < m; ++i) {
        int u = edges[i].first;
        int v = edges[i].second;
        adj[u].push_back(i);
        adj[v].push_back(i);
        degree[u]++;
        degree[v]++;
    }

    // Eulerian circuit requires all vertices with degree > 0 must have even degree.
    for (int i = 1; i <= n; ++i) {
        if (degree[i] % 2 == 1) return {};
    }

    // Find a start vertex that has at least one edge.
    int start = -1;
    for (int i = 1; i <= n; ++i) {
        if (degree[i] > 0) { start = i; break; }
    }
    if (start == -1) return {}; // should not happen since m > 0

    // Iterative Hierholzer's algorithm.
    std::vector<bool> edgeUsed(m, false);
    std::stack<int> pathStack;
    std::vector<int> circuit; // will hold reverse order

    pathStack.push(start);
    std::vector<std::list<int>::iterator> nextEdge(n + 1);
    for (int i = 1; i <= n; ++i) nextEdge[i] = adj[i].begin();

    while (!pathStack.empty()) {
        int u = pathStack.top();
        bool advanced = false;
        // Find an unused edge from u.
        while (nextEdge[u] != adj[u].end()) {
            int eidx = *nextEdge[u];
            if (!edgeUsed[eidx]) {
                edgeUsed[eidx] = true;
                // determine the other vertex
                int v = edges[eidx].first == u ? edges[eidx].second : edges[eidx].first;
                // move iterator forward so we don't recheck this edge for u
                ++nextEdge[u];
                pathStack.push(v);
                advanced = true;
                break;
            } else {
                ++nextEdge[u];
            }
        }
        if (!advanced) {
            // no more unused edges from u, pop and add to circuit
            circuit.push_back(u);
            pathStack.pop();
        }
    }

    // If we didn't traverse all edges, graph is disconnected (for edges)
    if (static_cast<int>(circuit.size()) != m + 1) return {};

    // circuit is currently in reverse order, reverse to get correct circuit
    std::reverse(circuit.begin(), circuit.end());
    // The last vertex should equal first vertex.
    if (circuit.front() != circuit.back()) return {};
    return circuit;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above. Here is the test harness.
// (For brevity, I'm including the function again in a real submission,
// but for this test block we assume it's already declared.)

int main() {
    // Test 1: Simple triangle
    std::vector<std::pair<int,int>> e1 = {{1,2},{2,3},{3,1}};
    auto r1 = findEulerianCircuit(3, e1);
    assert(r1.size() == 4);
    assert(r1.front() == r1.back());
    // Verify all edges are used (simple check: consecutive pairs are symmetric)
    // Actual validation: ensure each edge appears exactly once.
    std::vector<std::pair<int,int>> used;
    for (size_t i = 0; i+1 < r1.size(); ++i) {
        used.push_back({r1[i], r1[i+1]});
    }
    // Check each original edge appears in either direction
    for (auto& e : e1) {
        bool found = false;
        for (auto& u : used) {
            if ((u.first == e.first && u.second == e.second) ||
                (u.first == e.second && u.second == e.first)) {
                found = true; break;
            }
        }
        assert(found);
    }

    // Test 2: Square with diagonal (all even degrees)
    std::vector<std::pair<int,int>> e2 = {{1,2},{2,3},{3,4},{4,1},{1,3}};
    auto r2 = findEulerianCircuit(4, e2);
    assert(r2.size() == 6);
    assert(r2.front() == r2.back());

    // Test 3: Odd degree -> no circuit
    std::vector<std::pair<int,int>> e3 = {{1,2},{2,3}};
    auto r3 = findEulerianCircuit(3, e3);
    assert(r3.empty());

    // Test 4: Multiple edges between same pair
    std::vector<std::pair<int,int>> e4 = {{1,2},{1,2},{2,1}};
    auto r4 = findEulerianCircuit(2, e4);
    assert(r4.size() == 4);
    assert(r4.front() == r4.back());

    // Test 5: Disconnected edges -> no circuit
    std::vector<std::pair<int,int>> e5 = {{1,2},{3,4}};
    auto r5 = findEulerianCircuit(4, e5);
    assert(r5.empty());

    // Test 6: Single edge
    std::vector<std::pair<int,int>> e6 = {{1,2}};
    auto r6 = findEulerianCircuit(2, e6);
    assert(r6.empty()); // both vertices odd degree

    // Test 7: No edges
    auto r7 = findEulerianCircuit(3, {});
    assert(r7.size() == 1 && r7[0] == 1);

    return 0;
}
// The main algorithm is Hierholzer's algorithm, which finds an Eulerian circuit in `O(n + m)` time. First, we check that the graph has no vertices with odd degree (necessary for an Eulerian circuit). Then we pick any vertex that has at least one edge as the starting point (if no edges exist, return an empty circuit or a single vertex? For the task, we assume `m > 0`, but handle `m == 0` by returning a single vertex `1` if `n >= 1`? The specification says "closed trail" but with no edges, a single vertex is a trivial circuit. We'll return `{1}` for `n >= 1` when `m == 0`). We then perform an iterative DFS-like traversal: maintain a stack for the current path, and for each vertex, keep an iterator into its adjacency list to avoid revisiting edges. We use a global edge counter to ensure we traverse exactly `m` edges. While the stack is not empty, we try to advance to the next unvisited edge from the top vertex; if no edges remain, we pop the vertex and push it onto the result (the circuit in reverse order). Finally, after processing all edges, we reverse the result to get the circuit. We also need to remove edges efficiently—since we have a list of edges, we can store adjacency as a list of indices into an edge array, and mark edge indices as used. Use two lists per vertex: one for edges and one for iterators. Edge case: If any vertex with degree > 0 is not connected to the chosen start, the circuit will not cover all edges; we can check that the total number of edges traversed equals `m`; if not, return an empty vector. Time complexity is `O(n + m)` and space complexity `O(n + m)` for adjacency and stack.
