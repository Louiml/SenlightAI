Write a C++ function `vector<vector<int>> findBridges(int n, const vector<vector<int>>& connections)` that receives the number of nodes `n` (numbered `0` to `n-1`) in an undirected connected graph and a list of undirected edges `connections`, where each element is a vector of two integers `[u, v]` representing an edge between nodes `u` and `v`. The graph is connected and may contain duplicate edges or self-loops (though the problem assumes valid input, your function should still handle them gracefully without breaking). The function must return a vector of all bridges (critical edges) in the graph, where each bridge is represented as a vector `{u, v}` with `u < v` (to avoid duplicate opposite order). A bridge is an edge whose removal increases the number of connected components. The graph is guaranteed to be connected initially, so the output should list every edge that, if removed, disconnects the graph. Your solution must use Depth-First Search (DFS) with discovery time and low-link values (Tarjan's bridge-finding algorithm). The function should not modify the input `connections`; it must be passed by const reference. The output order of bridges does not matter. The graph may have up to `10^5` nodes and `10^5` edges, so the algorithm must run in linear time relative to the number of nodes and edges.

// The approach uses a depth-first search (DFS) traversal starting from node 0. For each node, we record its discovery time (`firstTime`) and the earliest discovery time reachable from that node via its subtree edges (also including back edges to ancestors), which is stored in `minTime`. The DFS is recursive; we skip the parent edge to avoid treating the edge back to the parent as a back edge. When visiting a child, we first recursively call DFS and then update the current node's `minTime` with the child's `minTime`. A bridge exists if the child's `minTime` is strictly greater than the current node's `firstTime`, meaning there is no back edge from the child’s subtree that connects to an ancestor of the current node. We then record that edge as a bridge. Important edge cases: (1) The graph is connected, so DFS from node 0 covers all nodes; (2) duplicate edges are handled because if there are multiple edges between the same pair, the second edge becomes a back edge, so the low value of the child will not be greater than the parent's discovery time, and thus it is not reported as a bridge; (3) self-loops are skipped because the parent check `child == parent` will skip only the immediate parent, but a self-loop still appears as a child equal to the node itself, which when visited will not be skipped (because the parent is the caller, not the node itself), so it will be processed as a back edge and not cause a bridge. The time complexity is O(V + E) because each node and edge is visited once. The space complexity is O(V) for the visited, firstTime, minTime arrays, plus the recursion stack in the worst case O(V) for a path graph, and the adjacency list O(E) storage.

#include <vector>
#include <algorithm>

// Find all bridges in an undirected connected graph using Tarjan's algorithm.
// Returns a vector of bridges, each as {u, v} with u < v.
std::vector<std::vector<int>> findBridges(int n, const std::vector<std::vector<int>>& connections) {
    // Build adjacency list
    std::vector<std::vector<int>> graph(n);
    for (const auto& edge : connections) {
        int u = edge[0];
        int v = edge[1];
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    std::vector<int> firstTime(n, -1);
    std::vector<int> minTime(n, -1);
    std::vector<bool> visited(n, false);
    std::vector<std::vector<int>> bridges;
    int time = 0;

    // Recursive DFS lambda (using std::function for simplicity)
    std::function<void(int, int)> dfs = [&](int node, int parent) {
        visited[node] = true;
        firstTime[node] = minTime[node] = time++;

        for (int child : graph[node]) {
            if (child == parent) continue;
            if (!visited[child]) {
                dfs(child, node);
                minTime[node] = std::min(minTime[node], minTime[child]);
                if (firstTime[node] < minTime[child]) {
                    // It's a bridge; store with smaller first
                    if (node < child)
                        bridges.push_back({node, child});
                    else
                        bridges.push_back({child, node});
                }
            } else {
                // back edge to an ancestor
                minTime[node] = std::min(minTime[node], firstTime[child]);
            }
        }
    };

    // Graph is connected, start from 0
    dfs(0, -1);

    return bridges;
}

#include <cassert>
#include <vector>
#include <algorithm>

// Assume the solution function is defined above; include it here.

int main() {
    // Test 1: Simple line graph 0-1-2, only middle edge is a bridge? Actually both are bridges.
    {
        int n = 3;
        std::vector<std::vector<int>> conn = {{0,1},{1,2}};
        auto result = findBridges(n, conn);
        std::vector<std::vector<int>> expected = {{0,1},{1,2}};
        // sort each bridge internally and the outer vector for comparison
        for (auto& e : result) std::sort(e.begin(), e.end());
        for (auto& e : expected) std::sort(e.begin(), e.end());
        std::sort(result.begin(), result.end());
        std::sort(expected.begin(), expected.end());
        assert(result == expected);
    }

    // Test 2: Triangle - no bridges
    {
        int n = 3;
        std::vector<std::vector<int>> conn = {{0,1},{1,2},{0,2}};
        auto result = findBridges(n, conn);
        assert(result.empty());
    }

    // Test 3: Single edge connecting two components (but graph is one component with two nodes)
    {
        int n = 2;
        std::vector<std::vector<int>> conn = {{0,1}};
        auto result = findBridges(n, conn);
        std::vector<std::vector<int>> expected = {{0,1}};
        assert(result == expected);
    }

    // Test 4: Star graph center 0 with leaves 1,2,3 - all edges are bridges
    {
        int n = 4;
        std::vector<std::vector<int>> conn = {{0,1},{0,2},{0,3}};
        auto result = findBridges(n, conn);
        std::vector<std::vector<int>> expected = {{0,1},{0,2},{0,3}};
        for (auto& e : result) std::sort(e.begin(), e.end());
        for (auto& e : expected) std::sort(e.begin(), e.end());
        std::sort(result.begin(), result.end());
        std::sort(expected.begin(), expected.end());
        assert(result == expected);
    }

    // Test 5: Cycle with a tail: 0-1-2-0 (cycle) and 2-3 (tail), only 2-3 is a bridge
    {
        int n = 4;
        std::vector<std::vector<int>> conn = {{0,1},{1,2},{2,0},{2,3}};
        auto result = findBridges(n, conn);
        std::vector<std::vector<int>> expected = {{2,3}};
        assert(result == expected);
    }

    // Test 6: Duplicate edges (parallel edge) - no bridge between those nodes
    {
        int n = 2;
        std::vector<std::vector<int>> conn = {{0,1},{0,1}};
        auto result = findBridges(n, conn);
        assert(result.empty());
    }

    // Test 7: Self-loop (ignored) and a bridge
    {
        int n = 3;
        std::vector<std::vector<int>> conn = {{0,0},{0,1},{1,2}};
        auto result = findBridges(n, conn);
        std::vector<std::vector<int>> expected = {{0,1},{1,2}};
        for (auto& e : result) std::sort(e.begin(), e.end());
        for (auto& e : expected) std::sort(e.begin(), e.end());
        std::sort(result.begin(), result.end());
        std::sort(expected.begin(), expected.end());
        assert(result == expected);
    }

    // Test 8: Large cycle - no bridges
    {
        int n = 1000;
        std::vector<std::vector<int>> conn;
        for (int i = 0; i < n; i++) {
            conn.push_back({i, (i+1)%n});
        }
        auto result = findBridges(n, conn);
        assert(result.empty());
    }

    // Test 9: Two cycles connected by a single bridge
    {
        int n = 6;
        std::vector<std::vector<int>> conn = {{0,1},{1,2},{2,0},{2,3},{3,4},{4,5},{5,3}};
        auto result = findBridges(n, conn);
        std::vector<std::vector<int>> expected = {{2,3}};
        assert(result == expected);
    }
}
