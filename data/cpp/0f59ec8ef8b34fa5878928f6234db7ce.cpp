Write a C++ function `int countSCC(int n, const std::vector<std::pair<int, int>>& edges)` that takes the number of vertices \( n \) (labeled from 1 to \( n \)) and a list of directed edges as pairs `(from, to)`, and returns the number of strongly connected components (SCCs) in the directed graph. The graph is guaranteed to have no self-loops and no duplicate edges. The function should ignore vertex 0 if it appears (i.e., vertices are 1-indexed, but the input may contain 0 for illustrative purposes, which should be ignored). The function must use Kosaraju's algorithm, performing two DFS passes: first on the original graph to compute finish times, then on the transposed graph in decreasing finish time order to collect SCCs. The result is an integer count. The function must be robust for up to \( 10^5 \) vertices and \( 10^5 \) edges.
#include <cassert>
#include <vector>
#include <utility>

int countSCC(int n, const std::vector<std::pair<int, int>>& edges);

int main() {
    // Test 1: Single vertex, no edges -> 1 SCC
    assert(countSCC(1, {}) == 1);

    // Test 2: Two vertices, one edge 1->2 -> 2 SCCs
    assert(countSCC(2, {{1,2}}) == 2);

    // Test 3: Two vertices, edge back and forth -> 1 SCC
    assert(countSCC(2, {{1,2},{2,1}}) == 1);

    // Test 4: Three vertices, cycle 1->2->3->1 -> 1 SCC
    assert(countSCC(3, {{1,2},{2,3},{3,1}}) == 1);

    // Test 5: Disconnected vertices -> each is its own SCC
    assert(countSCC(4, {}) == 4);

    // Test 6: Vertex 0 should be ignored, but n=3 with edges involving 0
    assert(countSCC(3, {{0,1},{1,2}}) == 2); // only 1->2 matter, 2 isolated? Actually 2 has no outgoing, so 2 SCCs: {1,2}? No, since 1->2 only, SCCs: {1},{2},{3}? Wait: 1->2, vertex 3 isolated. So 3 SCCs. Let's test: edges {0,1} ignored, {1,2} -> 1->2. So SCCs: {1},{2},{3} -> count 3.
    assert(countSCC(3, {{0,1},{1,2}}) == 3);

    // Test 7: Two SCCs connected chain: 1->2, 2->1, 2->3, 3->3? Actually 3 has self? but no self loops. Chain: 1<->2 and 2->3, 3 isolated? 3 has no outgoing, so SCCs: {1,2} and {3} -> 2 SCCs.
    assert(countSCC(3, {{1,2},{2,1},{2,3}}) == 2);

    // Test 8: More complex: 1->2, 2->3, 3->1, 3->4, 4->5, 5->4 -> SCCs: {1,2,3} and {4,5} -> 2
    assert(countSCC(5, {{1,2},{2,3},{3,1},{3,4},{4,5},{5,4}}) == 2);

    // Test 9: Chain of 3 vertices: 1->2, 2->3 -> 3 SCCs
    assert(countSCC(3, {{1,2},{2,3}}) == 3);

    // Test 10: Large n but no edges -> n SCCs
    assert(countSCC(100000, {}) == 100000);

    return 0;
}
#include <vector>
#include <algorithm>
#include <functional>

// Count strongly connected components using Kosaraju's algorithm.
// Vertices are 1-indexed; any vertex with label 0 is ignored.
// Edges are given as pairs (from, to).
int countSCC(int n, const std::vector<std::pair<int, int>>& edges) {
    // Build adjacency lists for original and transposed graphs, ignoring vertex 0.
    std::vector<std::vector<int>> adj(n + 1);
    std::vector<std::vector<int>> adjT(n + 1);
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second;
        if (u == 0 || v == 0) continue; // ignore invalid vertices
        adj[u].push_back(v);
        adjT[v].push_back(u);
    }

    std::vector<bool> visited(n + 1, false);
    std::vector<int> finishOrder; // will store vertices in order of finishing times

    // First DFS on original graph to compute finish times.
    std::function<void(int)> dfs1 = [&](int v) {
        visited[v] = true;
        for (int child : adj[v]) {
            if (!visited[child]) {
                dfs1(child);
            }
        }
        finishOrder.push_back(v);
    };

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            dfs1(i);
        }
    }

    // Process in reverse finish order (decreasing finish time).
    std::reverse(finishOrder.begin(), finishOrder.end());

    std::fill(visited.begin(), visited.end(), false);
    int componentCount = 0;

    // DFS on transposed graph to collect one SCC per call.
    std::function<void(int)> dfs2 = [&](int v) {
        visited[v] = true;
        for (int child : adjT[v]) {
            if (!visited[child]) {
                dfs2(child);
            }
        }
    };

    for (int v : finishOrder) {
        if (!visited[v]) {
            ++componentCount;
            dfs2(v);
        }
    }

    return componentCount;
}
// Kosaraju's algorithm works in two phases. First, perform a depth-first search (DFS) on the original graph, recording each vertex's "finish time" (the time when DFS finishes exploring all its outgoing edges). After the first DFS, sort vertices by decreasing finish time. Second, transpose the graph (reverse every edge direction) and perform DFS again on the transposed graph, iterating through the sorted vertices. Each DFS in the second phase discovers exactly one SCC, because the transposed graph makes the SCCs "reachable" in the reverse topological order. Edge cases include disconnected graphs (multiple starting points), isolated vertices (each is its own SCC), and cycles (all in one SCC). Time complexity is \( O(n + m) \) for both DFS passes plus \( O(n \log n) \) for sorting, which simplifies to \( O(n \log n + m) \). Space complexity is \( O(n + m) \) for adjacency lists and auxiliary arrays.
