// Write a C++ function `bool isBalanced(const TreeNode* root)` that determines whether a binary tree is height-balanced. A binary tree is considered height-balanced if, for every node, the absolute difference between the heights of its left and right subtrees is at most 1. The `TreeNode` structure is defined as `struct TreeNode { int val; TreeNode *left; TreeNode *right; TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} };`. The function should handle empty trees (return `true`), trees with only one node (return `true`), and unbalanced trees where the height difference exceeds 1 at any node. Use a recursive approach that computes subtree heights while detecting imbalance early to avoid unnecessary traversal.

The solution uses a helper function `getDepth` that recursively computes the height of a subtree while simultaneously checking balance condition. For each node, it recursively computes the heights of its left and right children. If either child returns a special sentinel value (e.g., `-1`) indicating an imbalance, the function immediately propagates that sentinel upward. Otherwise, it checks if the absolute difference between the left and right heights is greater than 1. If so, it returns `-1`; otherwise, it returns the maximum height plus 1. The main function `isBalanced` simply calls `getDepth` and checks whether the result is not `-1`. Edge cases include `nullptr` root (returns `true`), a single node (height 0, returns `true`), and a chain of nodes (unbalanced at some level). Time complexity is O(n), where n is the number of nodes, because each node is visited exactly once. Space complexity is O(h), where h is the tree height, due to recursion stack.

#include <algorithm>
#include <cstdlib>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper function: returns height of subtree rooted at node, or -1 if unbalanced.
int getHeightOrUnbalanced(const TreeNode* node) {
    if (node == nullptr) {
        return 0;
    }
    int leftHeight = getHeightOrUnbalanced(node->left);
    if (leftHeight == -1) {
        return -1;
    }
    int rightHeight = getHeightOrUnbalanced(node->right);
    if (rightHeight == -1) {
        return -1;
    }
    if (std::abs(leftHeight - rightHeight) > 1) {
        return -1;
    }
    return std::max(leftHeight, rightHeight) + 1;
}

// Check if binary tree is height-balanced.
bool isBalanced(const TreeNode* root) {
    return getHeightOrUnbalanced(root) != -1;
}

#include <cassert>

int main() {
    // Empty tree
    assert(isBalanced(nullptr) == true);

    // Single node
    TreeNode n1(1);
    assert(isBalanced(&n1) == true);

    // Balanced tree: root(1) with left(2) and right(3), left has left(4)
    TreeNode n4(4);
    TreeNode n2(2);
    TreeNode n3(3);
    TreeNode n1b(1);
    n2.left = &n4;
    n1b.left = &n2;
    n1b.right = &n3;
    assert(isBalanced(&n1b) == true);

    // Unbalanced: chain root(1)->left(2)->left(3)
    TreeNode n3c(3);
    TreeNode n2c(2);
    TreeNode n1c(1);
    n2c.left = &n3c;
    n1c.left = &n2c;
    assert(isBalanced(&n1c) == false);

    // Unbalanced: root(1) with left height 3 and right height 1
    TreeNode n5(5);
    TreeNode n4d(4);
    TreeNode n3d(3);
    TreeNode n2d(2);
    TreeNode n1d(1);
    n3d.left = &n5;
    n2d.left = &n3d;
    n1d.left = &n2d;
    TreeNode rr(10);
    n1d.right = &rr;
    assert(isBalanced(&n1d) == false);

    // Balanced with right-heavy: root(1), left(2), right(3) with right child(4)
    TreeNode n4e(4);
    TreeNode n3e(3);
    TreeNode n2e(2);
    TreeNode n1e(1);
    n3e.right = &n4e;
    n1e.left = &n2e;
    n1e.right = &n3e;
    assert(isBalanced(&n1e) == true);

    return 0;
}
