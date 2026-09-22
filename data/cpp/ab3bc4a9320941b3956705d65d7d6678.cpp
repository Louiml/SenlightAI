// Write a C++ function `bool isValidBST(const TreeNode* root)` that determines whether a given binary tree is a valid binary search tree (BST). A BST is defined by: for every node, all values in its left subtree are strictly less than the node’s value, all values in its right subtree are strictly greater than the node’s value, and both subtrees themselves satisfy the same recursive property. The tree may contain duplicate values, large integers (including `INT_MAX` and `INT_MIN`), and may be empty. The function must handle these cases correctly and avoid integer overflow when comparing values. Use `long long` for bounds to safely represent the theoretical minimum and maximum allowed values. The function should return `true` for an empty tree or a single node, and `false` otherwise if any violation is found.

// The solution uses a recursive helper that traverses the tree while passing a valid range `(minVal, maxVal)` for each node. At each node, we check that `minVal < node->val && node->val < maxVal`. Then we recursively validate the left subtree with `(minVal, node->val)` and the right subtree with `(node->val, maxVal)`. The initial call for the root passes `LLONG_MIN` and `LLONG_MAX` (or using `long long` with `INT_MIN-1` and `INT_MAX+1` as in the original snippet, but `LLONG_MIN`/`LLONG_MAX` is safer and avoids any reliance on `INT_MIN`/`INT_MAX` values). Edge cases: an empty tree is valid; a tree with duplicate values is invalid if duplicates appear in a position that violates strict inequality (e.g., a node equal to its ancestor); the root’s left subtree must have all values less than the root, and the right subtree greater. The algorithm runs in O(n) time, where n is the number of nodes, because each node is visited exactly once. The space complexity is O(h) for the recursion stack, where h is the tree height (worst-case O(n) for a skewed tree, O(log n) for a balanced tree). Using `long long` bounds ensures no overflow when comparing against extreme integer values.

#include <cstddef>
#include <climits>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper: recursively check BST property within [minVal, maxVal] (strict).
bool isValidBSTHelper(const TreeNode* node, long long minVal, long long maxVal) {
    if (node == nullptr) {
        return true;
    }
    if (node->val <= minVal || node->val >= maxVal) {
        return false;
    }
    return isValidBSTHelper(node->left, minVal, node->val) &&
           isValidBSTHelper(node->right, node->val, maxVal);
}

// Public function: checks whether the given binary tree is a valid BST.
bool isValidBST(const TreeNode* root) {
    return isValidBSTHelper(root, LLONG_MIN, LLONG_MAX);
}

#include <cassert>

int main() {
    // Test 1: Empty tree is valid
    assert(isValidBST(nullptr) == true);

    // Test 2: Single node tree is valid
    TreeNode n1(5);
    assert(isValidBST(&n1) == true);

    // Test 3: Valid BST: 2 < 5 < 7
    TreeNode n2(2);
    TreeNode n3(7);
    TreeNode n4(5);
    n4.left = &n2;
    n4.right = &n3;
    assert(isValidBST(&n4) == true);

    // Test 4: Invalid BST: left child equal to root (5 not < 5)
    TreeNode n5(5);
    TreeNode n6(5);
    n6.left = &n5;
    assert(isValidBST(&n6) == false);

    // Test 5: Invalid BST: right subtree contains value less than root
    TreeNode n7(3);
    TreeNode n8(4);
    TreeNode n9(5);
    n9.left = &n7;
    n9.right = &n8; // 4 is less than 5, but placed in right subtree
    assert(isValidBST(&n9) == false);

    // Test 6: Valid BST with multiple levels
    TreeNode a(1);
    TreeNode b(3);
    TreeNode c(2);
    c.left = &a;
    c.right = &b;
    TreeNode d(6);
    TreeNode e(4);
    e.left = &c;
    e.right = &d;
    assert(isValidBST(&e) == true);

    // Test 7: Invalid BST due to duplicate in right subtree
    TreeNode f(2);
    TreeNode g(2);
    TreeNode h(3);
    h.left = &f;
    h.right = &g;
    assert(isValidBST(&h) == false);

    // Test 8: Extremes: INT_MIN and INT_MAX as nodes
    TreeNode i(INT_MIN);
    TreeNode j(INT_MAX);
    TreeNode k(0);
    k.left = &i;
    k.right = &j;
    assert(isValidBST(&k) == true);

    // Test 9: Extremes invalid: root = INT_MIN with left child = INT_MIN-1 not possible, but test root = INT_MAX with right child = INT_MAX
    TreeNode l(INT_MAX);
    TreeNode m(INT_MAX);
    l.right = &m;
    assert(isValidBST(&l) == false);

    return 0;
}
