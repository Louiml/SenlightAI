// Write a C++ function `bool isUndirectedAcyclicForest(int n, const std::vector<std::pair<int,int>>& edges)` that determines whether an undirected graph with `n` vertices (labeled `0` through `n-1`) is a valid tree. A valid tree must be connected and contain no cycles. The input may include isolated vertices, self-loops (an edge from a vertex to itself), parallel edges (duplicate edges between the same two vertices), and the graph may be disconnected. Return `true` only if the graph is a single connected component with exactly `n-1` edges and no cycles; otherwise, return `false`. For `n = 0` (empty graph) or `n = 1` (single vertex with no edges), return `true` (a single vertex with no edges is trivially a tree). The function must handle arbitrary `n` (including large values) and must not modify the input vector.

The correct way to validate a tree is to check two key properties: (1) the graph is connected, and (2) the number of edges equals `n-1` (which, for an undirected graph, implies acyclicity when connected). A tempting but flawed approach is to use a BFS/DFS that attempts to detect cycles by checking whether any visited node is revisited, but that requires careful parent tracking to avoid mistaking an undirected edge back to the parent as a cycle. A simpler, robust method: first check if `edges.size() == n-1` (since any connected graph with `n` vertices must have at least `n-1` edges, and if it has exactly `n-1` edges and is connected, it must be a tree). Then run a BFS/DFS from vertex `0` (or any vertex) to verify all vertices are reachable. If an edge count mismatch occurs, return `false` immediately. Additionally, handle edge cases: `n == 0` or `n == 1` (the latter with zero edges is a tree; if `n == 1` and edges exist, return `false` because `edges.size() != n-1`). Self-loops and parallel edges will increase the edge count beyond `n-1`, causing an early `false`. The BFS uses a queue and a visited boolean vector; for each neighbor, if it is not visited, mark it and push it. Since we already have the edge count condition, we don't need to explicitly detect cycles during BFS; connectivity alone suffices. Time complexity: O(n + m) for building adjacency and BFS, where m is the number of edges. Space complexity: O(n + m) for adjacency and visited arrays.

#include <vector>
#include <queue>

// Determine if an undirected graph with 'n' vertices (labeled 0..n-1)
// is a tree: connected and acyclic. The graph is given as an edge list.
bool isUndirectedAcyclicForest(int n, const std::vector<std::pair<int,int>>& edges) {
    // Trivial cases: empty or single-vertex graph (with no edges) is a tree.
    if (n == 0) return true;
    if (n == 1) return edges.empty();

    // A tree with n vertices must have exactly n-1 edges.
    // This also excludes self-loops and parallel edges from being valid.
    if (edges.size() != static_cast<size_t>(n - 1)) {
        return false;
    }

    // Build adjacency list.
    std::vector<std::vector<int>> adj(n);
    for (const auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        // If a self-loop appears (u == v), it would make a cycle, and also
        // the edge count would exceed n-1? Actually if n==1 this is caught
        // above; for n>1, a self-loop still keeps edge count n-1 but creates
        // a cycle, so we must handle it explicitly.
        if (u == v) return false;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // BFS from vertex 0 to check connectivity.
    std::vector<bool> visited(n, false);
    std::queue<int> q;
    q.push(0);
    visited[0] = true;
    int visitedCount = 1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                visitedCount++;
                q.push(v);
            }
        }
    }

    // The graph is connected if we visited all n vertices.
    return visitedCount == n;
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: Simple valid tree with 3 vertices: 0-1, 1-2.
    std::vector<std::pair<int,int>> edges1 = {{0,1},{1,2}};
    assert(isUndirectedAcyclicForest(3, edges1) == true);

    // Test 2: Graph with a cycle (triangle).
    std::vector<std::pair<int,int>> edges2 = {{0,1},{1,2},{2,0}};
    assert(isUndirectedAcyclicForest(3, edges2) == false);

    // Test 3: Disconnected graph.
    std::vector<std::pair<int,int>> edges3 = {{0,1}};
    assert(isUndirectedAcyclicForest(3, edges3) == false);

    // Test 4: Single vertex with no edges.
    std::vector<std::pair<int,int>> edges4;
    assert(isUndirectedAcyclicForest(1, edges4) == true);

    // Test 5: Empty graph (n=0).
    assert(isUndirectedAcyclicForest(0, edges4) == true);

    // Test 6: Parallel edges between same two vertices.
    std::vector<std::pair<int,int>> edges6 = {{0,1},{0,1}};
    assert(isUndirectedAcyclicForest(2, edges6) == false);

    // Test 7: Self-loop.
    std::vector<std::pair<int,int>> edges7 = {{0,0}};
    assert(isUndirectedAcyclicForest(1, edges7) == false);

    // Test 8: Larger valid tree (5 vertices, 4 edges).
    std::vector<std::pair<int,int>> edges8 = {{0,1},{1,2},{2,3},{3,4}};
    assert(isUndirectedAcyclicForest(5, edges8) == true);

    // Test 9: Valid tree but with vertex not connected (edge count still n-1 but disconnected).
    // n=4, edges: 0-1, 1-2, 2-0 (cycle) plus isolated vertex 3? That would be 3 edges, but connectedness fails.
    std::vector<std::pair<int,int>> edges9 = {{0,1},{1,2},{2,0}};
    assert(isUndirectedAcyclicForest(4, edges9) == false);

    // Test 10: n=1 with an edge to nonexistent vertex (invalid input semantics, but edge count fails).
    std::vector<std::pair<int,int>> edges10 = {{0,1}};
    assert(isUndirectedAcyclicForest(1, edges10) == false);

    return 0;
}
