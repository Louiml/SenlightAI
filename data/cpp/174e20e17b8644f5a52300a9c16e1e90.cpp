// Given a tree with `n` nodes (labeled 0 to n−1), each holding an integer value, write a C++ function `int findUnmatchedNode(const std::vector<int>& values, const std::vector<std::pair<int,int>>& edges)` that returns the smallest-indexed node whose value is not the maximum value in its own subtree (including itself). In other words, find the lowest index `u` such that there exists some node `v` in the subtree rooted at `u` where `values[v] > values[u]`. Return −1 if no such node exists (i.e., every node’s value is at least as large as all values in its subtree). The tree is undirected and the input is 0‑based.
#include <cassert>
#include <vector>
#include <utility>

// Assume the solution function is declared above.

int main() {
    // Test 1: Simple chain 0-1, values [1,2] -> node 0 has subtree max 2 > 1.
    {
        std::vector<int> values = {1, 2};
        std::vector<std::pair<int,int>> edges = {{0,1}};
        assert(findUnmatchedNode(values, edges) == 0);
    }

    // Test 2: Chain 0-1, values [2,1] -> node 1 has subtree max 1, node 0 has 2, none strictly greater.
    {
        std::vector<int> values = {2, 1};
        std::vector<std::pair<int,int>> edges = {{0,1}};
        assert(findUnmatchedNode(values, edges) == -1);
    }

    // Test 3: Star centered at 0, leaves 1,2. values [1,2,3] -> node 0 subtree max 3 > 1.
    {
        std::vector<int> values = {1, 2, 3};
        std::vector<std::pair<int,int>> edges = {{0,1},{0,2}};
        assert(findUnmatchedNode(values, edges) == 0);
    }

    // Test 4: Same star but center is largest: [5,1,2] -> node 1 has only itself, node 2 same, center 5, all fine -> -1.
    {
        std::vector<int> values = {5, 1, 2};
        std::vector<std::pair<int,int>> edges = {{0,1},{0,2}};
        assert(findUnmatchedNode(values, edges) == -1);
    }

    // Test 5: Two-node tree with equal values [3,3] -> no strictly greater, return -1.
    {
        std::vector<int> values = {3, 3};
        std::vector<std::pair<int,int>> edges = {{0,1}};
        assert(findUnmatchedNode(values, edges) == -1);
    }

    // Test 6: A larger tree where node 1 is the answer because its descendant has larger value.
    // Tree: 0-1-2, values [2,1,3] -> node 0's subtree max = 3 > 2? Actually node 0's subtree includes all, so 0 also qualifies, but smallest index is 0.
    {
        std::vector<int> values = {2, 1, 3};
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2}};
        assert(findUnmatchedNode(values, edges) == 0);
    }

    // Test 7: Same tree but values [2,3,1] -> node 0's subtree max = 3 (from node1) >2, so answer 0. But node1's subtree max = 3 (itself) not larger. Correct.
    {
        std::vector<int> values = {2, 3, 1};
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2}};
        assert(findUnmatchedNode(values, edges) == 0);
    }

    // Test 8: Tree where only a leaf is the answer: 0-1-2, values [3,2,4] -> node 0 subtree max = 4 > 3, so 0. To get only node1 as answer, make node0 the global max.
    {
        std::vector<int> values = {4, 2, 3};
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2}};
        // node0's subtree max = 4 (itself) not >4, node1's subtree max = 3 >2 => answer 1
        assert(findUnmatchedNode(values, edges) == 1);
    }

    // Test 9: Single node
    {
        std::vector<int> values = {7};
        std::vector<std::pair<int,int>> edges;
        assert(findUnmatchedNode(values, edges) == -1);
    }

    // Test 10: Disconnected? Not possible by definition, but test a larger tree with duplicates.
    {
        std::vector<int> values = {5, 5, 1, 2};
        std::vector<std::pair<int,int>> edges = {{0,1},{0,2},{1,3}};
        // Node0 subtree max = 5 (from 0,1) not >5; node1 subtree max=5 not >5; node2 subtree max=1; node3 subtree max=2; all good -> -1.
        assert(findUnmatchedNode(values, edges) == -1);
    }

    return 0;
}
#include <vector>
#include <algorithm>
#include <functional>

// Given tree values and 0-based edges, returns the smallest index u such that
// there exists a node v in subtree(u) with values[v] > values[u]. If none, returns -1.
int findUnmatchedNode(const std::vector<int>& values, const std::vector<std::pair<int,int>>& edges) {
    const int n = static_cast<int>(values.size());
    if (n == 0) return -1;

    std::vector<std::vector<int>> adj(n);
    for (const auto& edge : edges) {
        adj[edge.first].push_back(edge.second);
        adj[edge.second].push_back(edge.first);
    }

    std::vector<int> subtreeMax(n);
    std::vector<bool> visited(n, false);

    // DFS to compute the maximum value in each node's subtree.
    std::function<int(int)> dfs = [&](int node) -> int {
        visited[node] = true;
        int curMax = values[node];
        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                curMax = std::max(curMax, dfs(neighbor));
            }
        }
        subtreeMax[node] = curMax;
        return curMax;
    };

    dfs(0);  // Tree is connected by definition.

    // Find the smallest index whose subtree contains a strictly larger value.
    for (int i = 0; i < n; ++i) {
        if (subtreeMax[i] > values[i]) {
            return i;
        }
    }
    return -1;
}
// The problem requires determining, for each node, whether any node in its subtree has a greater value than the node itself. Using an Euler tour (DFS order), each subtree corresponds to a contiguous interval `[tin[u], tout[u])` in the tour, where `tin[u]` is the entry time and `tout[u]` is the exit time (exclusive). The size of the subtree is `tout[u] - tin[u]`. We can precompute suffix maximums of the Euler tour values: `maxFrom[i]` gives the maximum value in positions `[i, n−1]`. Then the maximum in the subtree of `u` is `max( maxFrom[tin[u]], ... )` but careful: the subtree interval is `[tin[u], tout[u])`, so we need a prefix maximum for the left part and suffix for the right part. However simpler: we can compute `prefixMax[i]` = max of first `i+1` elements and `suffixMax[i]` = max of last `n-i` elements. Then the maximum in interval `[L, R)` is `max(prefixMax[R−1])` but that only works for prefix. Better approach: use a sparse table or segment tree for O(1) range maximum queries, but given constraints are moderate, we can sort nodes by value descending and use a Fenwick tree to mark visited nodes in Euler order: process nodes from largest value to smallest; for a node `u`, check if any descendant has already been processed (i.e., its Euler interval contains a processed position). This can be done with a Fenwick tree supporting range sum queries. If the sum over `[tin[u]+1, tout[u]-1]` is positive, then there exists a descendant with a strictly greater value (because we process in descending order, any earlier processed node has value >= current, but duplicates? We need strictly greater, so we must carefully process duplicates by grouping equal values and checking after each group). Alternatively, a simpler method: compute `maxInSubtree[u]` using DFS and then check if `maxInSubtree[u] > values[u]`. That is direct and O(n). The provided code snippet uses Euler tour and prefix/suffix maximums to check outside the subtree? Actually the snippet does something else: it finds the maximum value node that has a larger value outside its subtree? But reading the snippet, it computes `mx` as max of prefix before `tin[u]` and suffix after `tout[u]` (outside subtree), and if that max > value, it returns the node. That is the opposite: it finds a node whose value is not the global maximum among nodes outside its subtree? Actually it returns the highest-indexed? The snippet sorts values descending and returns the first (largest) value node that has a larger outside its subtree? That seems to be finding a node that is not the maximum of the whole tree. However our task is different: find the smallest index node whose value is not the maximum in its own subtree. So we need to compute maximum in each subtree. We'll do a single DFS to compute subtree maximums and then scan nodes in order 0..n−1 to find the first where `subtreeMax[u] > values[u]`. Time O(n), space O(n) for adjacency and auxiliary arrays. Edge cases: if tree has one node, it is trivially the maximum of its subtree, so return −1. Duplicate values: if a node's value equals the maximum in its subtree, it is still considered "maximum" (i.e., not strictly greater), so condition is strictly greater.
