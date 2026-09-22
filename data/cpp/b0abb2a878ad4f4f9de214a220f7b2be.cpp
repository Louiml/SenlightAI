// Write a C++ function `bool isBST(const TreeNode* root)` that determines whether a given binary tree satisfies the Binary Search Tree (BST) property: for every node, all values in its left subtree are strictly less than the node’s value, and all values in its right subtree are strictly greater than the node’s value. The tree is represented using the provided `TreeNode` structure with integer `val`, and `left`/`right` pointers. The function must handle empty trees (return `true`), trees with duplicate values (such trees are invalid because strict inequality is required), and large or skewed trees without causing stack overflow beyond typical recursion limits (the recursion depth equals tree height). Provide a robust solution that avoids the common mistake of only checking child nodes directly; it must verify that the entire left subtree's maximum is less than the root and the entire right subtree's minimum is greater than the root, either by recursive helper functions or by passing allowed value ranges.

#include <cassert>

int main() {
    // Empty tree is a valid BST
    assert(isBST(nullptr) == true);

    // Single node
    TreeNode* single = new TreeNode(5);
    assert(isBST(single) == true);
    delete single;

    // Valid small tree:      10
    //                       /  \
    //                      5    15
    TreeNode* root1 = new TreeNode(10);
    root1->left = new TreeNode(5);
    root1->right = new TreeNode(15);
    assert(isBST(root1) == true);
    delete root1->left;
    delete root1->right;
    delete root1;

    // Invalid: left child equal to root
    TreeNode* root2 = new TreeNode(7);
    root2->left = new TreeNode(7);
    assert(isBST(root2) == false);
    delete root2->left;
    delete root2->right; // nullptr delete is safe
    delete root2;

    // Invalid: right child equal to root
    TreeNode* root3 = new TreeNode(7);
    root3->right = new TreeNode(7);
    assert(isBST(root3) == false);
    delete root3->left;
    delete root3->right;
    delete root3;

    // Invalid: deeper violation (left subtree has value 20 > 10)
    //       10
    //      /
    //     5
    //      \
    //       20
    TreeNode* root4 = new TreeNode(10);
    root4->left = new TreeNode(5);
    root4->left->right = new TreeNode(20);
    assert(isBST(root4) == false);
    delete root4->left->right;
    delete root4->left;
    delete root4->right;
    delete root4;

    // Valid complex tree:          15
    //                            /  \
    //                           10   20
    //                          / \    \
    //                         8  12   25
    TreeNode* root5 = new TreeNode(15);
    root5->left = new TreeNode(10);
    root5->left->left = new TreeNode(8);
    root5->left->right = new TreeNode(12);
    root5->right = new TreeNode(20);
    root5->right->right = new TreeNode(25);
    assert(isBST(root5) == true);
    delete root5->left->left;
    delete root5->left->right;
    delete root5->left;
    delete root5->right->right;
    delete root5->right;
    delete root5;

    // Invalid: using INT_MIN as a node value
    // The tree root = INT_MIN, right = 0 is valid (INT_MIN < 0)
    TreeNode* root6 = new TreeNode(INT_MIN);
    root6->right = new TreeNode(0);
    assert(isBST(root6) == true);
    delete root6->right;
    delete root6->left;
    delete root6;

    // Invalid: right subtree contains node equal to root's value
    //      5
    //       \
    //        5
    TreeNode* root7 = new TreeNode(5);
    root7->right = new TreeNode(5);
    assert(isBST(root7) == false);
    delete root7->right;
    delete root7->left;
    delete root7;

    return 0;
}

#include <climits>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper: recursively validate BST with allowed value range.
bool isValidBSTHelper(const TreeNode* node, long long lower, long long upper) {
    if (node == nullptr) return true;
    if (node->val <= lower || node->val >= upper) return false;
    return isValidBSTHelper(node->left, lower, node->val) &&
           isValidBSTHelper(node->right, node->val, upper);
}

// Determine whether the binary tree rooted at 'root' is a valid BST.
bool isBST(const TreeNode* root) {
    return isValidBSTHelper(root, LLONG_MIN, LLONG_MAX);
}

// The core idea is to verify the BST property recursively, ensuring that each node's value stays within a valid interval derived from its ancestors. One intuitive approach (inspired by the provided snippet) checks the immediate left subtree’s maximum and immediate right subtree’s minimum for the current root, then recursively validates both children. However, that approach is inefficient (O(n²) in the worst case) because it recomputes subtree extrema repeatedly. A cleaner and more efficient method uses a recursive helper that carries lower and upper bounds (`long long` to safely handle `INT_MIN`/`INT_MAX`): for a node, its value must be strictly greater than the lower bound and strictly less than the upper bound. Then recurse on the left with the same lower bound and the node’s value as the new upper bound, and on the right with the node’s value as the new lower bound and the same upper bound. This ensures all descendants respect the strict ordering. For an empty tree, return `true`. Edge cases include: a single node (always valid), a left child equal to the root (invalid), a right child equal to the root (invalid), and deeply skewed trees (recursion depth equals height, but that’s acceptable for typical constraints). Time complexity is O(n) because each node is visited exactly once. Space complexity is O(h) for the recursion stack, where h is the tree height (O(n) for a skewed tree, O(log n) for balanced).
