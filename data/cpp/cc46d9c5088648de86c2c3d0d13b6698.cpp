Write a C++ function `int maxPathSum(TreeNode* root)` that, given the root of a binary tree where each node contains an integer value (which can be negative, zero, or positive), returns the maximum possible sum of any non-empty path. A path is defined as a sequence of adjacent nodes connected by edges, where no node appears more than once, and the path does not necessarily need to pass through the root. The path sum is the sum of the node values along that sequence. The function must handle empty trees, trees with only negative values, and large positive values. You may assume the `TreeNode` structure is defined as in the snippet, with integer `val` and `TreeNode* left` and `right` pointers.
#include <cassert>

int main() {
    // Test 1: root = [1,2,3] -> 2+1+3=6
    TreeNode* root1 = new TreeNode(1, new TreeNode(2), new TreeNode(3));
    assert(maxPathSum(root1) == 6);

    // Test 2: root = [-10,9,20,null,null,15,7] -> 15+20+7=42
    TreeNode* root2 = new TreeNode(-10);
    root2->left = new TreeNode(9);
    root2->right = new TreeNode(20);
    root2->right->left = new TreeNode(15);
    root2->right->right = new TreeNode(7);
    assert(maxPathSum(root2) == 42);

    // Test 3: single node with negative value -> that node itself
    TreeNode* root3 = new TreeNode(-3);
    assert(maxPathSum(root3) == -3);

    // Test 4: all negative values: [-5,-1,-2] -> best path is the largest single node (-1)
    TreeNode* root4 = new TreeNode(-5);
    root4->left = new TreeNode(-1);
    root4->right = new TreeNode(-2);
    assert(maxPathSum(root4) == -1);

    // Test 5: root = [2,-1] -> path 2 (since -1 is negative, ignore it) sum=2
    TreeNode* root5 = new TreeNode(2);
    root5->left = new TreeNode(-1);
    assert(maxPathSum(root5) == 2);

    // Test 6: empty tree (nullptr) -> 0
    assert(maxPathSum(nullptr) == 0);

    // Test 7: chain: [1,2,3] in right-only path -> 1+2+3=6
    TreeNode* root7 = new TreeNode(1);
    root7->right = new TreeNode(2);
    root7->right->right = new TreeNode(3);
    assert(maxPathSum(root7) == 6);

    // Test 8: balanced tree with only positive values: [5,4,6] -> 4+5+6=15
    TreeNode* root8 = new TreeNode(5, new TreeNode(4), new TreeNode(6));
    assert(maxPathSum(root8) == 15);

    // Test 9: tree with zeros and negatives: [0,-1,0] -> best is 0 (from root)
    TreeNode* root9 = new TreeNode(0);
    root9->left = new TreeNode(-1);
    root9->right = new TreeNode(0);
    assert(maxPathSum(root9) == 0);

    // Test 10: larger tree: [-1,-2,10,-6,1,-3] where optimal is 10+1=11 (right-left? Actually check: 10 alone is 10, 10+(-3)? skip; better -1+10? no path; correct max is 10+1? 1 is left child of -2? Let's build a known optimal: root=-1, left=-2, right=10; -2->left=-6, -2->right=1; 10->left=-3 -> path 1+(-2)+10? no that's 9, but path 10+(-3)? =7, path 1 alone=1, best is 10-3? actually 10+(-3)=7, 10 alone=10, 1+(-2)+10=9, so max=10. Need a better example: root=5, left=4, right=3, left->left=-1, right->right=-2 -> best is 4+5+3=12? 4+5=9, 5+3=8, 4+5+3=12 yes.
    TreeNode* root10 = new TreeNode(5);
    root10->left = new TreeNode(4);
    root10->right = new TreeNode(3);
    root10->left->left = new TreeNode(-1);
    root10->right->right = new TreeNode(-2);
    assert(maxPathSum(root10) == 12);

    // Clean up (optional, but for completeness; in real code use smart pointers or delete)
    // Not necessary for test correctness.
}
#include <algorithm>
#include <climits>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper function that returns the maximum downward path sum starting at root,
// and updates the global answer with the best path that passes through root.
int maxDown(TreeNode* root, int& ans) {
    if (!root) return 0;
    int leftGain = std::max(0, maxDown(root->left, ans));
    int rightGain = std::max(0, maxDown(root->right, ans));
    // Update the best path that goes through the current node.
    ans = std::max(ans, leftGain + rightGain + root->val);
    // Return the maximum downward path sum that can continue upward.
    return std::max(leftGain, rightGain) + root->val;
}

// Returns the maximum path sum of any non-empty path in the binary tree.
int maxPathSum(const TreeNode* root) {
    if (!root) return 0;
    int ans = INT_MIN;
    // Need a non-const pointer to call helper, but the helper does not modify tree.
    maxDown(const_cast<TreeNode*>(root), ans);
    return ans;
}
// The key observation is that any path in a binary tree can be decomposed around a highest node (the "turning point") where the path goes down into the left subtree, down into the right subtree, or only continues downward. For each node, we consider two quantities: the maximum path sum that starts at this node and goes downward (following either the left or right child) without branching, and the maximum path sum that passes through this node and potentially extends into both subtrees. To avoid negative contributions when propagating upward, we clamp the downward sums to zero (i.e., we ignore a child's contribution if it is negative). At each node, we compute the candidate path sum that goes through the node as `leftGain + rightGain + node->val` where `leftGain = max(0, maxDown(left))` and `rightGain = max(0, maxDown(right))`, and update a global maximum. The function `maxDown(root, ans)` returns the maximum sum of a path that starts at `root` and goes downward into one subtree, and also updates `ans` with the best path that passes through `root`. The algorithm performs a single post-order traversal. Edge cases: an empty tree returns 0; a tree with all negative values will still choose the largest (least negative) node by clamping gains to zero, because a single node path is the maximum. Time complexity is O(n) where n is the number of nodes, and space complexity is O(h) for the recursion stack where h is the tree height (O(n) in worst case for a skewed tree, O(log n) for balanced).
