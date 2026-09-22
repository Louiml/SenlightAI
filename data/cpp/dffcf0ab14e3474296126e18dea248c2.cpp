/*
Write a C++ function `int interactiveGame(int n, const std::vector<std::pair<int,int>>& edges, const std::vector<int>& xSet, const std::vector<int>& ySet, int firstQueryResult, int secondQueryResult)` that simulates an interactive game on an undirected tree with `n` nodes (numbered 1..n). The function receives the tree edges, two sets `xSet` and `ySet` (each containing distinct node IDs), and two integers `firstQueryResult` and `secondQueryResult`. The game works as follows: you first ask the judge for one node from `ySet` (specifically, you output `"B " + ySet[0]` and the judge replies with a node `tmp`). You then perform a DFS starting from `tmp` and find the first node (in DFS traversal order) that belongs to `xSet`; call this node `ret`. You output `"A " + ret` and the judge replies with a node (this is `secondQueryResult`). Finally, if `secondQueryResult` is in `ySet`, you output `"C " + ret`; otherwise you output `"C -1"`. Your function should return `ret` if `secondQueryResult` is in `ySet`, else return -1. The function must not perform any I/O; it only computes the result based on the given parameters. Assume all inputs are valid, the tree is connected, and both sets are non-empty. Note that the DFS should visit neighbors in the order they appear in the adjacency list (which is the order given in `edges`). Edge cases: if `tmp` itself is in `xSet`, then `ret` is `tmp`; if no node in `xSet` is reachable from `tmp` (which should not happen because the tree is connected and `xSet` is non-empty), but handle it safely by returning -1.
*/

#include <vector>
#include <algorithm>
#include <functional>

// Simulate the interactive game on a tree.
// Returns ret if secondQueryResult is in ySet, otherwise -1.
int interactiveGame(int n,
                    const std::vector<std::pair<int, int>>& edges,
                    const std::vector<int>& xSet,
                    const std::vector<int>& ySet,
                    int firstQueryResult,
                    int secondQueryResult) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Sort copies of sets for binary search
    std::vector<int> xSorted = xSet;
    std::vector<int> ySorted = ySet;
    std::sort(xSorted.begin(), xSorted.end());
    std::sort(ySorted.begin(), ySorted.end());

    int ret = -1;
    bool found = false;

    // DFS to find first node in xSet
    std::function<void(int, int)> dfs = [&](int node, int parent) {
        if (found) return;
        if (std::binary_search(xSorted.begin(), xSorted.end(), node)) {
            ret = node;
            found = true;
            return;
        }
        for (int neighbor : adj[node]) {
            if (neighbor != parent) {
                dfs(neighbor, node);
                if (found) return;
            }
        }
    };

    dfs(firstQueryResult, -1);

    // If no node found (shouldn't happen), return -1
    if (ret == -1) return -1;

    // Check if secondQueryResult is in ySet
    if (std::binary_search(ySorted.begin(), ySorted.end(), secondQueryResult)) {
        return ret;
    } else {
        return -1;
    }
}

#include <cassert>
#include <vector>
#include <utility>

// Include the solution here (or link) — for demonstration we assume it's available.
// For completeness, the solution code is repeated in the test section conceptually.

int main() {
    // Tree: 1-2, 2-3, 2-4 (4 nodes)
    std::vector<std::pair<int,int>> edges1 = {{1,2}, {2,3}, {2,4}};
    std::vector<int> x1 = {3, 4};
    std::vector<int> y1 = {1, 2};

    // firstQueryResult = 2 (judge reply to "B 1")
    // DFS from 2: order of neighbors: 1,3,4 (since edges added as 1-2,2-3,2-4)
    // First found in x1 is 3 (because neighbor 1 not in x, then 3 in x)
    assert(interactiveGame(4, edges1, x1, y1, 2, 1) == 3); // 1 is in y1 -> return ret=3
    assert(interactiveGame(4, edges1, x1, y1, 2, 4) == 3); // 4 in y1 -> return 3
    assert(interactiveGame(4, edges1, x1, y1, 2, 5) == -1); // 5 not in y1 -> -1

    // Tree: 1-2-3 (path)
    std::vector<std::pair<int,int>> edges2 = {{1,2}, {2,3}};
    std::vector<int> x2 = {1, 3};
    std::vector<int> y2 = {2, 3};

    // firstQueryResult = 2 (judge reply)
    // DFS from 2: neighbors order: 1,3 -> first found in x2 is 1
    assert(interactiveGame(3, edges2, x2, y2, 2, 3) == 1); // 3 in y2 -> return 1
    assert(interactiveGame(3, edges2, x2, y2, 2, 2) == 1); // 2 in y2 -> return 1
    assert(interactiveGame(3, edges2, x2, y2, 2, 1) == -1); // 1 not in y2 -> -1

    // firstQueryResult = 3 (judge reply)
    // DFS from 3: neighbor 2 -> then from 2 neighbor 1 -> first found in x2 is 1? Wait: DFS from 3: node 3 not in x2? Actually 3 is in x2, so ret=3.
    assert(interactiveGame(3, edges2, x2, y2, 3, 3) == 3); // 3 in y2 -> ret=3
    assert(interactiveGame(3, edges2, x2, y2, 3, 2) == 3); // 2 in y2 -> ret=3
    assert(interactiveGame(3, edges2, x2, y2, 3, 5) == -1);

    // Single node tree (n=1) with both sets containing only node 1
    std::vector<std::pair<int,int>> edges3;
    std::vector<int> x3 = {1};
    std::vector<int> y3 = {1};
    assert(interactiveGame(1, edges3, x3, y3, 1, 1) == 1); // ret=1, second in y -> return 1
    assert(interactiveGame(1, edges3, x3, y3, 1, 2) == -1); // second not in y -> -1

    return 0;
}

// The core algorithm is straightforward: build an adjacency list from the edges. The function simulates the first query by fixing `tmp = firstQueryResult` (which represents the judge's reply to `"B " + ySet[0]`). Then we perform a DFS starting from `tmp`, traversing the tree and, when we first encounter a node that is in the sorted `xSet` (using binary_search for O(log |xSet|) lookup), we set `ret` to that node and stop. Because the tree is connected and `xSet` is non-empty, we will always find such a node. The DFS order is determined by the order of neighbors in the adjacency list; we recursively visit each neighbor not equal to the parent. We do not need to mark visited nodes beyond skipping the parent because the graph is a tree (no cycles). After obtaining `ret`, we check if `secondQueryResult` is in `ySet` (using binary_search on sorted `ySet`). If yes, return `ret`; otherwise return -1. Complexity: building adjacency list is O(n) per call, DFS visits each node once, so O(n) time. Binary searches are O(log k) where k is size of sets. Space: O(n) for adjacency list and recursion stack (worst-case O(n) in a path). Edge cases: if `firstQueryResult` equals a node in `xSet`, `ret` is that node; if `secondQueryResult` is not in `ySet`, return -1. The function does not modify any input vectors; we sort copies of the sets internally to ensure const-correctness.
