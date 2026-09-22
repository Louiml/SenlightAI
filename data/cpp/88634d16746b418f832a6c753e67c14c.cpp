/*
Write a C++ function `bool isValidBST(TreeNode* root)` that determines whether a given binary tree is a valid Binary Search Tree (BST). The tree nodes are defined with an integer `val`, and pointers `left` and `right`. A valid BST satisfies the property that for every node, all values in its left subtree are strictly less than the node's value, and all values in its right subtree are strictly greater. The tree may contain duplicate values, and the function must return `false` if any violation exists. The task requires handling extreme integer bounds (e.g., `INT_MIN`, `INT_MAX`) correctly, even when node values approach these limits. The function should be implemented recursively, using a helper that tracks allowed minimum and maximum value ranges for each subtree.
*/

#include <climits>
#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// Helper function that validates the BST property with given bounds.
bool isValidBSTHelper(const TreeNode* node, long long min_val, long long max_val) {
    if (node == nullptr) {
        return true;
    }
    if (node->val <= min_val || node->val >= max_val) {
        return false;
    }
    return isValidBSTHelper(node->left, min_val, node->val) &&
           isValidBSTHelper(node->right, node->val, max_val);
}

// Public function: returns true if the tree is a valid binary search tree.
bool isValidBST(const TreeNode* root) {
    return isValidBSTHelper(root, LLONG_MIN, LLONG_MAX);
}

#include <cassert>

int main() {
    // Test 1: Empty tree is valid
    TreeNode* nullTree = nullptr;
    assert(isValidBST(nullTree) == true);

    // Test 2: Single node
    TreeNode singleNode(5);
    assert(isValidBST(&singleNode) == true);

    // Test 3: Valid BST with multiple nodes
    TreeNode n1(2), n2(1), n3(3);
    n1.left = &n2; n1.right = &n3;
    assert(isValidBST(&n1) == true);

    // Test 4: Invalid BST (right child smaller than root)
    TreeNode m1(2), m2(3), m3(1);
    m1.right = &m2; m2.left = &m3;
    assert(isValidBST(&m1) == false);

    // Test 5: Duplicate value in subtree
    TreeNode d1(2), d2(2), d3(3);
    d1.left = &d2; d1.right = &d3;
    assert(isValidBST(&d1) == false);

    // Test 6: Values at INT_MIN and INT_MAX
    TreeNode e1(INT_MAX), e2(INT_MIN), e3(INT_MAX);
    e1.left = &e2; e1.right = &e3;
    assert(isValidBST(&e1) == false); // duplicate INT_MAX

    TreeNode f1(INT_MAX), f2(INT_MIN);
    f1.left = &f2;
    assert(isValidBST(&f1) == true);

    // Test 7: Skewed valid tree
    TreeNode g1(1), g2(2), g3(3);
    g1.right = &g2; g2.right = &g3;
    assert(isValidBST(&g1) == true);

    // Test 8: Skewed invalid tree
    TreeNode h1(3), h2(2), h3(1);
    h1.left = &h2; h2.left = &h3;
    assert(isValidBST(&h1) == true); // left subtree all smaller, valid

    // Cleanup (not necessary in this simple test, but good practice)
    return 0;
}

// The solution uses a recursive helper function that propagates lower and upper bounds down the tree. Initially, the bounds are set to `LLONG_MIN` and `LLONG_MAX` to avoid overflow when comparing with `INT_MIN` and `INT_MAX`. At each node, we check if its value lies strictly within the current `(min_val, max_val)` interval; if not, the tree is invalid. Then we recurse on the left child with `max_val` updated to the current node's value (since all left descendants must be smaller), and on the right child with `min_val` updated to the current node's value (since all right descendants must be larger). The base case is a null node, which is always valid. This approach correctly handles duplicate values (they cause failure because the check uses `<` and `>`). Edge cases include a single-node tree (valid), trees with values at `INT_MIN` or `INT_MAX` (handled by using `long long` bounds), and skewed trees (still processed in O(n) time). Time complexity is O(n) where n is the number of nodes, and space complexity is O(h) due to recursion stack, where h is the tree height (worst-case O(n) for skewed trees).
