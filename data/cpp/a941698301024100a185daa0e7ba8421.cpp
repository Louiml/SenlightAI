/*
You are given a rooted tree with `n` nodes (numbered from 1 to `n`), where node 1 is the root. The parent of each node `i` (for `i = 2, ..., n`) is provided. Define the "cost" of a node as follows: if a node has no children, its cost is 0; otherwise, its cost is equal to the number of its children plus the maximum cost among its children. Write a C++ function `int treeCost(const std::vector<std::vector<int>>& children)` that takes an adjacency list where `children[i]` contains the direct children of node `i` (with `children[0]` unused or empty for convenience) and returns the cost of the root node (node 1). The tree is guaranteed to be a valid rooted tree (each node except the root has exactly one parent, and there are no cycles). The number of nodes `n` satisfies `1 <= n <= 200,000`, and the parent list is provided such that the adjacency list is already built. Your function should avoid recursion depth issues (use an explicit stack or iterative DFS) because `n` can be large. Return an `int` (the answer fits in a standard 32-bit signed integer).
*/

#include <vector>
#include <stack>
#include <algorithm>

// Compute the cost of the root node (1) given an adjacency list of children.
// children[i] contains direct children of node i; children[0] is unused.
// Returns the cost of node 1 as defined by the problem.
int treeCost(const std::vector<std::vector<int>>& children) {
    const int n = static_cast<int>(children.size()) - 1; // nodes are 1..n
    std::vector<int> dp(n + 1, 0); // dp[u] = cost of subtree rooted at u
    std::vector<bool> visited(n + 1, false);
    std::vector<int> order; // post-order sequence
    std::stack<int> st;
    
    // Iterative DFS to produce post-order (children before parent)
    if (n >= 1) {
        st.push(1);
        visited[1] = true;
        while (!st.empty()) {
            int u = st.top();
            // Check if all children are processed
            bool all_children_processed = true;
            for (int v : children[u]) {
                if (!visited[v]) {
                    all_children_processed = false;
                    visited[v] = true;
                    st.push(v);
                    break; // go deeper first
                }
            }
            if (all_children_processed) {
                order.push_back(u);
                st.pop();
            }
        }
    }
    
    // Process nodes in post-order: children already computed
    for (int u : order) {
        int max_child_cost = 0;
        for (int v : children[u]) {
            max_child_cost = std::max(max_child_cost, dp[v]);
        }
        dp[u] = max_child_cost + static_cast<int>(children[u].size());
    }
    
    if (n == 0) {
        return 0; // no nodes (but per constraints n>=1)
    }
    return dp[1];
}

#include <cassert>
#include <vector>
#include <iostream>

// Function under test (included for completeness, but will be linked externally in practice)
int treeCost(const std::vector<std::vector<int>>& children);

int main() {
    // Test 1: single node (root only)
    {
        std::vector<std::vector<int>> children(2); // index 0 unused, index 1 = root
        assert(treeCost(children) == 0);
    }
    // Test 2: chain of 4 nodes: 1->2->3->4
    {
        std::vector<std::vector<int>> children(5);
        children[1].push_back(2);
        children[2].push_back(3);
        children[3].push_back(4);
        // leaf 4 cost = 0
        // node 3: max(0)+1 = 1
        // node 2: max(1)+1 = 2
        // node 1: max(2)+1 = 3
        assert(treeCost(children) == 3);
    }
    // Test 3: star: root with 3 leaves
    {
        std::vector<std::vector<int>> children(5);
        children[1] = {2, 3, 4};
        // leaves cost 0, root = max(0)+3 = 3
        assert(treeCost(children) == 3);
    }
    // Test 4: balanced tree: root 1, children 2 and 3, node 2 has child 4, node 3 has child 5 and 6
    {
        std::vector<std::vector<int>> children(7);
        children[1] = {2, 3};
        children[2] = {4};
        children[3] = {5, 6};
        // leaf 4,5,6 cost 0
        // node 2: max(0)+1 = 1
        // node 3: max(0)+2 = 2
        // root: max(1,2)+2 = 4
        assert(treeCost(children) == 4);
    }
    // Test 5: root with child that has many children
    {
        std::vector<std::vector<int>> children(10);
        children[1] = {2};
        children[2] = {3,4,5,6,7,8,9};
        // node 2: max(0)+7 = 7
        // root: max(7)+1 = 8
        assert(treeCost(children) == 8);
    }
    // Test 6: root with two children, each with deep chain
    {
        std::vector<std::vector<int>> children(5);
        children[1] = {2, 3};
        children[2] = {4};
        children[3] = {5};
        // node 4 leaf cost 0 -> node2 = 1
        // node 5 leaf cost 0 -> node3 = 1
        // root = max(1,1)+2 = 3
        assert(treeCost(children) == 3);
    }
    // Test 7: node numbering not by insertion order (but still 1..n)
    {
        // 1 has child 4, 4 has children 2 and 3
        std::vector<std::vector<int>> children(5);
        children[1] = {4};
        children[4] = {2, 3};
        // leaves 2,3 cost 0 -> node4 = 2
        // root = max(2)+1 = 3
        assert(treeCost(children) == 3);
    }
    // Test 8: large chain to test iterative approach (n=10000)
    {
        int n = 10000;
        std::vector<std::vector<int>> children(n + 1);
        for (int i = 1; i < n; ++i) {
            children[i].push_back(i + 1);
        }
        // Chain length n: cost = n-1
        assert(treeCost(children) == n - 1);
    }
    // Test 9: complete binary tree of height 2 (root with 2 children, each with 2 leaves)
    {
        std::vector<std::vector<int>> children(8);
        children[1] = {2, 3};
        children[2] = {4, 5};
        children[3] = {6, 7};
        // leaves cost 0 -> nodes 2,3: max(0)+2=2
        // root: max(2,2)+2=4
        assert(treeCost(children) == 4);
    }
    // Test 10: root with no children but size vector includes index 0
    {
        std::vector<std::vector<int>> children(2);
        children[1].clear();
        assert(treeCost(children) == 0);
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The problem is a classic tree DP. For each node, we need the maximum cost among its children, plus the count of its children. This can be computed bottom-up. Since recursion depth can be up to `n` (in worst-case a chain), a recursive DFS might cause a stack overflow for large `n`. Instead, we can process nodes in reverse order of their discovery during an iterative DFS (post-order traversal). Alternatively, because the tree is rooted and parents are given, we can compute the DP by iterating nodes from `n` down to 1 if we know each node's parent (since children always have indices greater than their parent in the given input pattern, but that is not guaranteed in general). The safest approach: perform an iterative DFS from the root using a stack to get a post-order sequence, then process each node in that order. For each node `u`, let `dp[u] = 0` initially; for each child `v`, we have already computed `dp[v]` (because we process post-order), so we take the maximum `dp[v]` and add the number of children to get `dp[u]`. The time complexity is `O(n)` because each edge is visited once. Space complexity is `O(n)` for the adjacency list, the `dp` array, and the stack (or recursion call stack, but we avoid that). Important edge cases: leaf nodes (cost 0), single-node tree (cost 0), and a star tree (root has many children, each leaf cost 0, so root cost = number of children). Also ensure that the `children` vector is indexed from 1 to `n` (index 0 empty), and that the function works for any tree shape regardless of node numbering order.
