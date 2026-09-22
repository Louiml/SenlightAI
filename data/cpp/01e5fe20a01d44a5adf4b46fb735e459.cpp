// Write a C++ function `int maxPathSum(TreeNode* root)` that, given the root of a non-empty binary tree where each node contains an integer value (which may be negative), returns the maximum possible sum of a path between any two nodes in the tree. A path is defined as any sequence of nodes where each adjacent pair is connected by an edge, and a path cannot contain repeated nodes—effectively, it is any continuous segment along a root-to-leaf route or any simple path that goes down from one node, may pass through a common ancestor, and ends at another node. The path must include at least one node, and the sum is the total of the node values along that path. The function should handle trees with negative values, ensuring that the chosen path might consist of a single node if all other combinations yield lower sums.

// The solution uses a recursive depth-first traversal of the tree. For each node, we compute the maximum path sum that starts at that node and goes downward to any descendant (including the node itself) — this is called the "single‑branch" contribution. This value is computed as the maximum of: (1) the node's value alone, (2) node's value plus the best left‑branch contribution, and (3) node's value plus the best right‑branch contribution. Note that we never take both left and right in this downward contribution because that would create a "U‑shaped" path that cannot continue upward to the parent. However, the global answer may include a path that bends through the current node, connecting the best left branch and the best right branch. Therefore, at each node, we update a global maximum (initialized to the smallest possible integer) with the sum of the node's value, the best left‑branch (if positive), and the best right‑branch (if positive). Because a path can also be a single node, the update includes the node's value alone, and because negative branches would only lower the sum, we effectively ignore them by taking `max(0, bestLeft)` and `max(0, bestRight)` when computing the bending path. The function returns the single‑branch contribution to its parent. Edge cases include: leaves (both children null) where the single‑branch contribution is just the node's value, trees with all negative values where the answer is the maximum node value, and deep trees causing recursion depth proportional to height. Time complexity is O(n) as each node is visited once, and space complexity is O(h) for the recursion stack, where h is the tree height (worst case O(n) for a skewed tree).

#include <algorithm>
#include <climits>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

int maxPathSum(TreeNode* root) {
    int globalMax = INT_MIN;
    
    // Recursively compute the best downward path sum starting at node.
    // Also update the global maximum to include paths that bend at this node.
    auto dfs = [&](TreeNode* node, auto&& dfs_ref) -> int {
        if (!node) return 0;  // No contribution from null children
        
        // Get best downward sums from left and right subtrees.
        // We ignore negative contributions because they would only reduce a path sum.
        int leftBest = std::max(0, dfs_ref(node->left, dfs_ref));
        int rightBest = std::max(0, dfs_ref(node->right, dfs_ref));
        
        // Path that goes through this node (possibly bending) has sum:
        int throughNode = node->val + leftBest + rightBest;
        globalMax = std::max(globalMax, throughNode);
        
        // Return the best single‑branch path starting at this node and going down,
        // which will be used by the parent.
        return node->val + std::max(leftBest, rightBest);
    };
    
    dfs(root, dfs);
    return globalMax;
}

#include <cassert>

int main() {
    // Test 1: Single node
    TreeNode* t1 = new TreeNode(-5);
    assert(maxPathSum(t1) == -5);
    
    // Test 2: Three nodes, all positive: path is entire tree
    TreeNode* t2 = new TreeNode(1);
    t2->left = new TreeNode(2);
    t2->right = new TreeNode(3);
    assert(maxPathSum(t2) == 6);
    
    // Test 3: All negative, best is a single node
    TreeNode* t3 = new TreeNode(-2);
    t3->left = new TreeNode(-1);
    t3->right = new TreeNode(-3);
    assert(maxPathSum(t3) == -1);
    
    // Test 4: Mixed values, best path bends through root
    TreeNode* t4 = new TreeNode(-10);
    t4->left = new TreeNode(9);
    t4->right = new TreeNode(20);
    t4->right->left = new TreeNode(15);
    t4->right->right = new TreeNode(7);
    // Best path: 15 -> 20 -> 7 = 42
    assert(maxPathSum(t4) == 42);
    
    // Test 5: Left skewed tree with negative root
    TreeNode* t5 = new TreeNode(-3);
    t5->left = new TreeNode(-2);
    t5->left->left = new TreeNode(-1);
    // Best is single node -1
    assert(maxPathSum(t5) == -1);
    
    // Test 6: Root negative but children positive, bend at child
    TreeNode* t6 = new TreeNode(-5);
    t6->left = new TreeNode(2);
    t6->left->left = new TreeNode(3);
    t6->left->right = new TreeNode(4);
    // Best path: 3 -> 2 -> 4 = 9
    assert(maxPathSum(t6) == 9);
    
    // Test 7: Large positive right subtree, root connects them
    TreeNode* t7 = new TreeNode(10);
    t7->left = new TreeNode(-2);
    t7->left->right = new TreeNode(100);
    t7->right = new TreeNode(-1);
    // Best path: -2 -> 100 or 100 alone? 100 alone is best, then 10+100? Path 10->-2->100 = 108 is better
    assert(maxPathSum(t7) == 108);
    
    // Test 8: Very simple two-node tree
    TreeNode* t8 = new TreeNode(5);
    t8->left = new TreeNode(4);
    assert(maxPathSum(t8) == 9); // 4+5
    
    // Test 9: Zero values
    TreeNode* t9 = new TreeNode(0);
    t9->right = new TreeNode(-1);
    t9->right->left = new TreeNode(0);
    assert(maxPathSum(t9) == 0); // best is 0 alone or 0+0? path 0 (root) alone sum 0, or root->right->left sum -1, best 0.
    
    // Test 10: Complex tree with both positive and negative, ensure correct
    TreeNode* t10 = new TreeNode(1);
    t10->left = new TreeNode(2);
    t10->right = new TreeNode(3);
    t10->left->left = new TreeNode(-4);
    t10->left->right = new TreeNode(5);
    // Best path: 5 -> 2 -> 1 -> 3 = 11
    assert(maxPathSum(t10) == 11);
    
    return 0;
}
