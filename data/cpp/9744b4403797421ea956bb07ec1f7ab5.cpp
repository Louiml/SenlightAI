Write a C++ function named `isValidBST` that takes a pointer to the root of a binary tree (where each node has an integer value and left/right child pointers) and returns `true` if the tree is a valid binary search tree (BST), and `false` otherwise. A valid BST satisfies: for every node, all values in its left subtree are strictly less than the node's value, and all values in its right subtree are strictly greater. The tree may contain duplicate values, negative numbers, and nodes with a single child or no children. An empty tree (null root) is considered valid. Implement the function with no global state and ensure it correctly handles extreme integer limits (e.g., `INT_MIN` and `INT_MAX`).

#include <cassert>
#include <climits>

int main() {
    // Empty tree is valid
    assert(isValidBST(nullptr) == true);

    // Single node
    TreeNode* single = new TreeNode(5);
    assert(isValidBST(single) == true);

    // Simple valid BST: 2 -> left 1, right 3
    TreeNode* validRoot = new TreeNode(2);
    validRoot->left = new TreeNode(1);
    validRoot->right = new TreeNode(3);
    assert(isValidBST(validRoot) == true);

    // Invalid: left child equals root (duplicate not allowed)
    TreeNode* dupRoot = new TreeNode(2);
    dupRoot->left = new TreeNode(2);
    assert(isValidBST(dupRoot) == false);

    // Valid tree with negative and extreme values
    TreeNode* extremeRoot = new TreeNode(INT_MIN);
    extremeRoot->right = new TreeNode(INT_MAX);
    assert(isValidBST(extremeRoot) == true);

    // Invalid: right subtree has a smaller value deeper
    TreeNode* invalidDeep = new TreeNode(10);
    invalidDeep->left = new TreeNode(5);
    invalidDeep->right = new TreeNode(15);
    invalidDeep->right->left = new TreeNode(6);
    assert(isValidBST(invalidDeep) == false);

    // Valid complex tree
    TreeNode* complex = new TreeNode(8);
    complex->left = new TreeNode(3);
    complex->left->left = new TreeNode(1);
    complex->left->right = new TreeNode(6);
    complex->right = new TreeNode(10);
    complex->right->right = new TreeNode(14);
    complex->right->right->left = new TreeNode(13);
    assert(isValidBST(complex) == true);

    // Invalid: root with left child greater than root
    TreeNode* leftGreater = new TreeNode(3);
    leftGreater->left = new TreeNode(5);
    assert(isValidBST(leftGreater) == false);

    // Invalid: root with right child less than root
    TreeNode* rightLess = new TreeNode(3);
    rightLess->right = new TreeNode(1);
    assert(isValidBST(rightLess) == false);

    return 0;
}

#include <limits>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Validates whether a binary tree satisfies BST properties.
bool isValidBST(TreeNode* root) {
    return isValidBSTHelper(root, std::numeric_limits<long long>::min(), std::numeric_limits<long long>::max());
}

bool isValidBSTHelper(TreeNode* node, long long minBound, long long maxBound) {
    if (!node) {
        return true;
    }
    if (node->val <= minBound || node->val >= maxBound) {
        return false;
    }
    return isValidBSTHelper(node->left, minBound, node->val) &&
           isValidBSTHelper(node->right, node->val, maxBound);
}

// The approach uses recursive range validation. For each node, we maintain a valid range `(low, high)` that the node's value must fall into (exclusive). Initially, the range is from minus infinity to plus infinity. For the left child, the range becomes `(low, parent->val)`; for the right child, `(parent->val, high)`. If any node violates its range, the tree is invalid. This handles duplicates because we enforce strict inequality (`<=` and `>=`). An empty node is trivially valid. To safely handle edge values like `INT_MIN` and `INT_MAX`, we use `long long` for the bounds, avoiding overflow when comparing or setting bounds. The recursion visits each node exactly once, so time complexity is `O(n)`, and space complexity in the worst case (skewed tree) is `O(n)` due to recursion stack; average case is `O(log n)`.
