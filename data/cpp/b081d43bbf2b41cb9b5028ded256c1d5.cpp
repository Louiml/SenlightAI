// Write a C++ function named `isValidBST` that takes a pointer to the root of a binary tree (where each node has an integer `val`, and pointers `left` and `right`) and returns a `bool` indicating whether the tree satisfies the binary search tree (BST) property. The BST property requires that for every node, all values in its left subtree are strictly less than the node’s value, and all values in its right subtree are strictly greater than the node’s value. The function must handle empty trees (return `true`), duplicate values (which violate the strict inequality), and very large or very small integer values (use `long long` boundaries to avoid overflow when comparing with `INT_MIN`/`INT_MAX`). Implement a recursive depth‑first traversal that carries allowed lower and upper bounds for each subtree, checking each node against these bounds.

The solution uses a recursive helper that processes each node with an allowed interval `[low, high]` (exclusive bounds). Initially, the root has bounds `[LLONG_MIN, LLONG_MAX]` because any `int` value is valid. For each node:
- If the node is null, return `true` (empty subtree is valid).
- If `node->val <= low` or `node->val >= high`, the node violates its allowed range, so return `false`.
- Recursively check the left subtree with bounds `[low, node->val]` and the right subtree with bounds `[node->val, high]`.
This approach correctly enforces strict ordering without needing to compare each node against its ancestors explicitly. Edge cases include empty trees, single‑node trees, trees with duplicates (which fail because `<=`/`>=` triggers), and trees where values touch `INT_MIN`/`INT_MAX` (handled by using `long long` bounds). Time complexity is O(n) because each node is visited once. Space complexity is O(h) for the recursion stack, where h is the tree height; in the worst case (skewed tree), h = n, giving O(n) space, but for balanced trees it is O(log n).

#include <climits>
#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper function to recursively validate the BST property.
bool dfs(const TreeNode* root, long long low, long long high) {
    if (root == nullptr) {
        return true;
    }
    // Current node's value must be strictly between low and high.
    if (root->val <= low || root->val >= high) {
        return false;
    }
    // Left subtree must be within (low, root->val), right within (root->val, high).
    return dfs(root->left, low, root->val) &&
           dfs(root->right, root->val, high);
}

// Return true if the binary tree is a valid BST.
bool isValidBST(const TreeNode* root) {
    return dfs(root, LLONG_MIN, LLONG_MAX);
}

#include <cassert>
#include <climits>

// TreeNode definition is assumed from the solution above.

int main() {
    // Test 1: Empty tree is valid.
    assert(isValidBST(nullptr) == true);

    // Test 2: Single node.
    TreeNode n1(5);
    assert(isValidBST(&n1) == true);

    // Test 3: Valid BST with left and right children.
    TreeNode n2(3), n3(7);
    TreeNode root1(5, &n2, &n3);
    assert(isValidBST(&root1) == true);

    // Test 4: Invalid because right child is equal to root.
    TreeNode n4(5);
    TreeNode root2(5, nullptr, &n4);
    assert(isValidBST(&root2) == false);

    // Test 5: Invalid because left child is greater than root.
    TreeNode n5(10);
    TreeNode root3(5, &n5, nullptr);
    assert(isValidBST(&root3) == false);

    // Test 6: Complex valid tree.
    TreeNode a1(1), a2(3), a3(6), a4(9);
    TreeNode a5(2, &a1, &a2);
    TreeNode a6(8, &a3, &a4);
    TreeNode root4(5, &a5, &a6);
    assert(isValidBST(&root4) == true);

    // Test 7: Violation deep inside: left subtree contains a node larger than root.
    TreeNode b1(7), b2(4);
    TreeNode b3(6, &b1, nullptr);  // b1=7 > root=5, but placed in left subtree
    TreeNode b4(4, nullptr, &b2);
    TreeNode root5(5, &b3, &b4);
    assert(isValidBST(&root5) == false);

    // Test 8: Values at INT_MIN and INT_MAX.
    TreeNode c1(INT_MIN), c2(INT_MAX);
    TreeNode root6(0, &c1, &c2);
    assert(isValidBST(&root6) == true);

    // Test 9: Duplicate values in different branches.
    TreeNode d1(5), d2(5);
    TreeNode root7(5, &d1, &d2);
    assert(isValidBST(&root7) == false);

    // Test 10: Skewed but valid tree.
    TreeNode e1(10), e2(20), e3(30);
    e2.left = &e1;
    e3.left = &e2;
    assert(isValidBST(&e3) == true);
}
