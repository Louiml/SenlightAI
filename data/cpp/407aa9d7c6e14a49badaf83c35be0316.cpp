// Given a rooted tree with `n` nodes (labeled from 0 to n-1) where node 0 is the root and each node except the root has exactly one parent, write a C++ function `int longestCommonAncestor(const std::vector<std::vector<int>>& children, int a, int b)` that returns the lowest common ancestor (LCA) of two nodes `a` and `b`. The function receives a vector `children` where `children[u]` is a list of direct child nodes of `u`. The tree is not necessarily binary, and any node may have any number of children. The function must handle any valid tree structure without cycles or multiple parents, and must return the node index (an integer) that is the LCA of `a` and `b`. If `a` or `b` is the same node, the LCA is that node itself. The tree can have up to 100,000 nodes, and the depth can be large, so the algorithm must be efficient.

#include <cassert>
#include <vector>

// Declare the function to test
int longestCommonAncestor(const std::vector<std::vector<int>>& children, int a, int b);

int main() {
    // Test 1: Simple tree: 0 -> 1, 2, 3; 2 -> 4; 4 -> 5
    std::vector<std::vector<int>> tree1 = {
        {1, 2, 3}, // 0
        {},        // 1
        {4},       // 2
        {},        // 3
        {5},       // 4
        {}         // 5
    };
    assert(longestCommonAncestor(tree1, 2, 3) == 0);
    assert(longestCommonAncestor(tree1, 4, 5) == 4);
    assert(longestCommonAncestor(tree1, 1, 1) == 1);
    assert(longestCommonAncestor(tree1, 0, 5) == 0);

    // Test 2: Chain tree: 0 -> 1 -> 2 -> 3 -> 4
    std::vector<std::vector<int>> chain = {
        {1}, {2}, {3}, {4}, {}
    };
    assert(longestCommonAncestor(chain, 2, 4) == 2);
    assert(longestCommonAncestor(chain, 0, 4) == 0);
    assert(longestCommonAncestor(chain, 3, 4) == 3);

    // Test 3: Larger tree with multiple branches
    std::vector<std::vector<int>> tree3 = {
        {1, 2},    // 0
        {3, 4},    // 1
        {5},       // 2
        {},        // 3
        {6},       // 4
        {},        // 5
        {}         // 6
    };
    assert(longestCommonAncestor(tree3, 3, 6) == 1);
    assert(longestCommonAncestor(tree3, 5, 6) == 0);
    assert(longestCommonAncestor(tree3, 3, 4) == 1);

    // Test 4: Root only tree with two leaves directly from root
    std::vector<std::vector<int>> tree4 = {{1, 2}, {}, {}};
    assert(longestCommonAncestor(tree4, 1, 2) == 0);

    return 0;
}

#include <vector>
#include <cstring>
#include <algorithm>

// Returns the lowest common ancestor of nodes a and b in a rooted tree.
// children[u] is a list of direct child nodes of u. The root is node 0.
int longestCommonAncestor(const std::vector<std::vector<int>>& children, int a, int b) {
    int n = (int)children.size();
    const int LOG = 20; // 2^20 > 1e6, enough for n up to 1e5
    static int up[100005][20];
    static int depth[100005];
    
    // Reset arrays for current call (assuming multiple calls possible)
    std::memset(up, -1, sizeof(up));
    std::memset(depth, 0, sizeof(depth));
    
    // DFS to set parent and depth
    // Since the tree is rooted at 0 and each node has exactly one parent,
    // we can use a stack-based DFS to avoid recursion depth issues.
    std::vector<int> stack;
    stack.push_back(0);
    depth[0] = 0;
    up[0][0] = -1; // root has no parent
    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        for (int child : children[u]) {
            depth[child] = depth[u] + 1;
            up[child][0] = u;
            stack.push_back(child);
        }
    }
    
    // Precompute binary lifting table
    for (int j = 1; j < LOG; ++j) {
        for (int i = 0; i < n; ++i) {
            if (up[i][j-1] != -1) {
                up[i][j] = up[up[i][j-1]][j-1];
            } else {
                up[i][j] = -1;
            }
        }
    }
    
    // Function to bring node u up by k levels (using binary representation)
    auto lift = [&](int u, int k) {
        for (int j = 0; j < LOG; ++j) {
            if (k & (1 << j)) {
                u = up[u][j];
                if (u == -1) break;
            }
        }
        return u;
    };
    
    // Ensure a is the deeper node
    if (depth[a] < depth[b]) std::swap(a, b);
    
    // Bring a up to the same depth as b
    a = lift(a, depth[a] - depth[b]);
    
    // If one was an ancestor of the other
    if (a == b) return a;
    
    // Move both upward together
    for (int j = LOG-1; j >= 0; --j) {
        if (up[a][j] != -1 && up[a][j] != up[b][j]) {
            a = up[a][j];
            b = up[b][j];
        }
    }
    
    return up[a][0];
}

// The solution uses the Binary Lifting technique for LCA queries. First, we perform a depth-first search (DFS) from the root (node 0) to compute each node's parent and depth. Then, we precompute a table `up[u][j]` that stores the 2^j-th ancestor of node `u`. Specifically, `up[u][0]` is the direct parent of `u` (or -1 if `u` is the root), and for `j > 0`, `up[u][j] = up[ up[u][j-1] ][j-1]` (if the intermediate ancestor exists, otherwise -1). To find the LCA: first bring the deeper node up to the same depth as the shallower node by jumping using powers of two. Then, if the nodes are equal, that's the LCA. Otherwise, move both nodes upward together using the largest jumps that keep them distinct, and finally return the parent of either node. Edge cases: when one node is an ancestor of the other; when the tree is a chain (depth up to n-1); and when the input nodes are the root. Time complexity is O(n log n) for preprocessing and O(log n) per query. Space complexity is O(n log n) for the table, plus O(n) for adjacency and depth.
