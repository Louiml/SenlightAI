Write a C++ function `int countCompleteComponents(int n, vector<vector<int>>& edges)` that takes the number of nodes `n` (labeled from `0` to `n-1`) and an undirected graph's edge list (each edge as a pair `[u, v]`), and returns the number of connected components in the graph that are *complete* — i.e., every pair of distinct nodes within the component has an edge between them. A component with a single node (no edges) is considered complete. The graph is simple (no self-loops or duplicate edges). The function must handle `n == 0` (return `0`), isolated nodes, and components of any size. You may assume the input graph is valid as described.
#include <cassert>
#include <vector>

int main() {
    // Example 1: n=6, edges = [[0,1],[0,2],[1,2],[3,4]] -> component {0,1,2} complete, {3,4} not (missing edge), {5} isolated complete -> 2
    {
        int n = 6;
        std::vector<std::vector<int>> edges = {{0,1},{0,2},{1,2},{3,4}};
        assert(countCompleteComponents(n, edges) == 2);
    }

    // Example 2: n=6, edges = [[0,1],[0,2],[1,2],[3,4],[3,5]] -> {0,1,2} complete, {3,4,5} not complete (missing edge between 4 and 5) -> 1
    {
        int n = 6;
        std::vector<std::vector<int>> edges = {{0,1},{0,2},{1,2},{3,4},{3,5}};
        assert(countCompleteComponents(n, edges) == 1);
    }

    // Example 3: n=2, no edges -> both isolated components, each complete -> 2
    {
        int n = 2;
        std::vector<std::vector<int>> edges = {};
        assert(countCompleteComponents(n, edges) == 2);
    }

    // Example 4: n=1, no edges -> single node complete -> 1
    {
        int n = 1;
        std::vector<std::vector<int>> edges = {};
        assert(countCompleteComponents(n, edges) == 1);
    }

    // Example 5: n=3, three edges forming a triangle -> one complete component -> 1
    {
        int n = 3;
        std::vector<std::vector<int>> edges = {{0,1},{1,2},{0,2}};
        assert(countCompleteComponents(n, edges) == 1);
    }

    // Example 6: n=0 -> 0
    {
        int n = 0;
        std::vector<std::vector<int>> edges = {};
        assert(countCompleteComponents(n, edges) == 0);
    }

    // Example 7: n=4, edges = [[0,1],[1,2],[2,0]] -> component {0,1,2} complete, node 3 isolated complete -> 2
    {
        int n = 4;
        std::vector<std::vector<int>> edges = {{0,1},{1,2},{2,0}};
        assert(countCompleteComponents(n, edges) == 2);
    }

    // Example 8: n=5, edges = [[0,1],[1,2],[2,3],[3,4]] -> a path, none complete except possibly single nodes? All nodes connected, but graph is not complete (missing many edges) -> 0
    {
        int n = 5;
        std::vector<std::vector<int>> edges = {{0,1},{1,2},{2,3},{3,4}};
        assert(countCompleteComponents(n, edges) == 0);
    }

    // Example 9: n=3, edges = [[0,1]] -> component {0,1} not complete (missing edge 0-1? actually it has edge, but missing 1-0? wait it has edge, but for complete need also all pairs, here {0,1} has one edge, complete since only 2 nodes, yes complete) and node 2 isolated complete -> 2
    {
        int n = 3;
        std::vector<std::vector<int>> edges = {{0,1}};
        assert(countCompleteComponents(n, edges) == 2);
    }

    // Example 10: n=3, edges = [[0,1],[1,2]] -> component {0,1,2} missing edge 0-2, not complete -> 0
    {
        int n = 3;
        std::vector<std::vector<int>> edges = {{0,1},{1,2}};
        assert(countCompleteComponents(n, edges) == 0);
    }

    return 0;
}
#include <vector>
#include <queue>

// Counts connected components that are complete (cliques).
// A complete component has edges between every pair of distinct nodes.
int countCompleteComponents(int n, std::vector<std::vector<int>>& edges) {
    if (n == 0) return 0;

    // Build adjacency list
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        int u = e[0];
        int v = e[1];
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<bool> visited(n, false);
    int complete_count = 0;

    for (int start = 0; start < n; ++start) {
        if (visited[start]) continue;

        // BFS to explore a component
        std::queue<int> q;
        q.push(start);
        visited[start] = true;
        int nodes = 0;
        int double_edges = 0; // counts each adjacency entry (each undirected edge twice)

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            ++nodes;
            for (int v : adj[u]) {
                ++double_edges;
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }

        // For a complete graph on k nodes, double_edges = k*(k-1)
        if (double_edges == nodes * (nodes - 1)) {
            ++complete_count;
        }
    }

    return complete_count;
}
// The solution performs a BFS or DFS from each unvisited node to identify all nodes in its connected component. For each component, we count two quantities: the number of nodes `nodes` and the number of edges `edges` (counting both directions for each adjacency, so `edges` equals twice the number of undirected edges inside the component, because the adjacency list stores each edge twice). A connected component is complete if every pair of distinct nodes is directly connected. In a complete undirected graph on `k` nodes, there are exactly `k*(k-1)/2` edges, but since our `edges` counts each undirected edge twice (once for each endpoint's adjacency list), the condition becomes `edges == nodes * (nodes - 1)`. For `nodes == 1`, this gives `0 == 0`, which is true, so isolated nodes count as complete components. We visit each node once and each adjacency entry once, so time complexity is `O(n + m)` where `m` is the number of undirected edges (since each edge appears twice). Space complexity is `O(n)` for the visited array and the BFS queue, plus `O(m)` for the adjacency list. Edge cases: `n == 0` returns `0`; graphs with no edges are all isolated nodes, all complete; the graph may be disconnected, requiring traversal from each unvisited node.
