// Given a directed graph with `n` vertices numbered from 1 to `n` and `m` directed edges, write a C++ function `longestPathLength(int n, const std::vector<std::pair<int,int>>& edges)` that returns the length of the longest path in the graph (the maximum number of edges you can traverse along a directed path without revisiting a vertex). The graph may contain cycles, so you must first detect if a cycle exists; if the graph contains a directed cycle, return `-1` to indicate that the longest path is unbounded. Otherwise, return the maximum number of edges in any path (a path of a single vertex has length 0). The graph is guaranteed to be connected in terms of edges, but not necessarily weakly connected. The function should be self-contained and should not rely on any global or mutable state beyond its inputs.

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: Simple DAG 1->2->3, longest path length = 2
    std::vector<std::pair<int,int>> e1 = {{1,2},{2,3}};
    assert(longestPathLength(3, e1) == 2);

    // Test 2: Cycle 1->2->1, should return -1
    std::vector<std::pair<int,int>> e2 = {{1,2},{2,1}};
    assert(longestPathLength(2, e2) == -1);

    // Test 3: Self loop 1->1, should return -1
    std::vector<std::pair<int,int>> e3 = {{1,1}};
    assert(longestPathLength(1, e3) == -1);

    // Test 4: No edges, n=3, longest path = 0 (any single vertex)
    std::vector<std::pair<int,int>> e4;
    assert(longestPathLength(3, e4) == 0);

    // Test 5: Disconnected DAG: 1->2 and 3 alone, longest path = 1
    std::vector<std::pair<int,int>> e5 = {{1,2}};
    assert(longestPathLength(3, e5) == 1);

    // Test 6: Two independent paths: 1->2 and 3->4->5, longest = 2
    std::vector<std::pair<int,int>> e6 = {{1,2},{3,4},{4,5}};
    assert(longestPathLength(5, e6) == 2);

    // Test 7: Longer chain 1->2->3->4->5, longest = 4
    std::vector<std::pair<int,int>> e7 = {{1,2},{2,3},{3,4},{4,5}};
    assert(longestPathLength(5, e7) == 4);

    // Test 8: Multiple incoming edges: 1->3, 2->3, 3->4, longest = 2 (1->3->4 or 2->3->4)
    std::vector<std::pair<int,int>> e8 = {{1,3},{2,3},{3,4}};
    assert(longestPathLength(4, e8) == 2);

    // Test 9: Cycle with extra branch: edges 1->2, 2->3, 3->2 (cycle on 2-3), returns -1
    std::vector<std::pair<int,int>> e9 = {{1,2},{2,3},{3,2}};
    assert(longestPathLength(3, e9) == -1);

    // Test 10: Single vertex with no edges, n=1, longest = 0
    std::vector<std::pair<int,int>> e10;
    assert(longestPathLength(1, e10) == 0);

    return 0;
}

#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>

// Compute the longest path length in a directed graph.
// Returns -1 if a cycle exists, otherwise the maximum number of edges in any directed path.
// Vertices are 1-indexed in the input edges, but we convert to 0-indexed internally.
int longestPathLength(int n, const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency list and in-degree array (0-indexed).
    std::vector<std::vector<int>> graph(n);
    std::vector<int> indegree(n, 0);
    for (const auto& [from, to] : edges) {
        // Validate input? Assume from,to are in [1,n].
        graph[from-1].push_back(to-1);
        ++indegree[to-1];
    }

    // Kahn's algorithm: topological sort with queue.
    std::queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }

    int processed = 0;
    std::vector<int> dp(n, 0); // dp[v] = longest path length ending at v.

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        ++processed;

        // Update DP for all outgoing neighbors.
        for (int v : graph[u]) {
            dp[v] = std::max(dp[v], dp[u] + 1);
            if (--indegree[v] == 0) {
                q.push(v);
            }
        }
    }

    // If we didn't process all vertices, a cycle exists.
    if (processed != n) {
        return -1;
    }

    // The longest path length is the maximum dp value.
    return *std::max_element(dp.begin(), dp.end());
}

// The problem asks for the longest path length in a directed graph, but only if the graph is acyclic (DAG). The key insight is to use topological sorting. If during Kahn’s algorithm (BFS-based topological sort) we process all vertices, then the graph is a DAG; if not all vertices are processed, a cycle exists. For a DAG, we can compute the longest path using dynamic programming on the topological order: initialize `dp[v] = 0` for all vertices, then for each vertex in topological order, for each outgoing edge `u->v`, update `dp[v] = max(dp[v], dp[u] + 1)`. The answer is the maximum value in `dp`. Edge cases: (1) A graph with no edges (m=0) has longest path length 0 because any single vertex is a path of length 0. (2) A graph with a self-loop or any cycle returns -1. (3) Disconnected components: longest path across any component is fine because we take the max over all vertices. (4) The input vertices are labeled 1..n; we can use 0-based indexing internally for convenience. Time complexity: O(n + m) for Kahn's algorithm and DP. Space complexity: O(n + m) for adjacency list, in-degree array, and DP array.
