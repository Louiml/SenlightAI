Write a C++ function `vector<vector<int>> findAllAncestors(int n, vector<vector<int>>& edges)` that takes a positive integer `n` representing the number of nodes in a Directed Acyclic Graph (DAG), and a vector of directed edges where each edge is `[from, to]`, and returns a vector of `n` vectors. The `i`-th vector in the result must contain all ancestor nodes of node `i` (nodes that can reach `i` via a directed path), sorted in ascending order. Nodes with no ancestors should have an empty vector. The graph is guaranteed to be acyclic, contain no duplicate edges, and may have up to 1000 nodes and 2000 edges. Your function should handle the case where the graph has no edges (all nodes have no ancestors). Implement the solution efficiently using an adjacency list and a depth-first search (DFS) traversal from each node to propagate ancestry information.
// The core idea is to perform a DFS starting from each node `u` and traverse all reachable nodes. For every `v` reached during the DFS from `u`, we add `u` to `ans[v]` if it’s not already the last element (ensuring uniqueness because the graph is acyclic and we process nodes in increasing order, but duplicates can occur through different paths). Because the DFS from node `u` only visits nodes that are descendants of `u`, every visited node gets `u` added as an ancestor. The result lists are then automatically sorted because we process ancestors in increasing order (`i = 0` to `n-1`). Important edge cases: nodes with no outgoing edges contribute nothing; disconnected nodes remain empty; the graph is a DAG so no cycles, simplifying the DFS without visited sets (though a visited set per DFS would be safer if cycles existed). Time complexity is `O(n + e)` per DFS, but we run it for every node, so total is `O(n * (n + e))` in the worst case, but since `n <= 1000` and `e <= 2000`, it’s acceptable. Space complexity is `O(n + e)` for the adjacency list and result, plus recursion stack `O(n)` in the worst case.
#include <vector>
#include <unordered_map>

// Given a DAG with n nodes and directed edges, return the sorted list of ancestors for each node.
std::vector<std::vector<int>> findAllAncestors(int n, std::vector<std::vector<int>>& edges) {
    // Build adjacency list: from -> list of to
    std::unordered_map<int, std::vector<int>> adj;
    for (const auto& edge : edges) {
        adj[edge[0]].push_back(edge[1]);
    }

    // Result: ans[i] will contain ancestors of node i, in ascending order.
    std::vector<std::vector<int>> ans(n);

    // Helper DFS to propagate the current ancestor to all descendants.
    // We use a recursive lambda capturing by reference to avoid extra parameters.
    // The vector 'seen' prevents re-adding the same ancestor multiple times within a single DFS traversal.
    std::function<void(int, int)> dfs = [&](int ancestor, int current) {
        for (int neighbor : adj[current]) {
            // If ancestor not already added to neighbor's list (using last element check since lists are sorted and we add in increasing ancestor order)
            if (ans[neighbor].empty() || ans[neighbor].back() != ancestor) {
                ans[neighbor].push_back(ancestor);
                dfs(ancestor, neighbor);
            }
        }
    };

    // For each node as an ancestor, run DFS to add it to all reachable nodes.
    for (int i = 0; i < n; ++i) {
        dfs(i, i);
    }

    return ans;
}
#include <cassert>
#include <vector>

// (The solution function above is assumed to be included here.)

int main() {
    // Example 1 from the problem statement.
    std::vector<std::vector<int>> edges1 = {{0,3},{0,4},{1,3},{2,4},{2,7},{3,5},{3,6},{3,7},{4,6}};
    std::vector<std::vector<int>> expected1 = {{},{},{},{0,1},{0,2},{0,1,3},{0,1,2,3,4},{0,1,2,3}};
    auto res1 = findAllAncestors(8, edges1);
    assert(res1.size() == 8);
    for (int i = 0; i < 8; ++i) {
        assert(res1[i] == expected1[i]);
    }

    // Example 2 from the problem statement.
    std::vector<std::vector<int>> edges2 = {{0,1},{0,2},{0,3},{0,4},{1,2},{1,3},{1,4},{2,3},{2,4},{3,4}};
    std::vector<std::vector<int>> expected2 = {{},{0},{0,1},{0,1,2},{0,1,2,3}};
    auto res2 = findAllAncestors(5, edges2);
    assert(res2.size() == 5);
    for (int i = 0; i < 5; ++i) {
        assert(res2[i] == expected2[i]);
    }

    // Single node, no edges.
    std::vector<std::vector<int>> edges3 = {};
    auto res3 = findAllAncestors(1, edges3);
    assert(res3.size() == 1);
    assert(res3[0].empty());

    // Chain: 0 -> 1 -> 2 -> 3
    std::vector<std::vector<int>> edges4 = {{0,1},{1,2},{2,3}};
    auto res4 = findAllAncestors(4, edges4);
    assert(res4 == std::vector<std::vector<int>>({{},{0},{0,1},{0,1,2}}));

    // Disconnected nodes and a small component.
    std::vector<std::vector<int>> edges5 = {{1,2}};
    auto res5 = findAllAncestors(4, edges5);
    assert(res5 == std::vector<std::vector<int>>({{},{},{1},{}}));

    // Star graph: 0 is ancestor of all others.
    std::vector<std::vector<int>> edges6 = {{0,1},{0,2},{0,3}};
    auto res6 = findAllAncestors(4, edges6);
    assert(res6 == std::vector<std::vector<int>>({{},{0},{0},{0}}));
}
