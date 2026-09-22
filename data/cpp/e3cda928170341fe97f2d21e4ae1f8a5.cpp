/*
Write a standalone C++ function that, given a pointer to the root of a valid Binary Search Tree (BST) and pointers to two nodes `p` and `q` existing in that tree, returns a pointer to their Lowest Common Ancestor (LCA). The LCA is defined as the deepest node that is an ancestor (including itself) of both `p` and `q`. The function must work for any valid BST where node values are unique, and both `p` and `q` are guaranteed to be present. Do not modify the tree or the input nodes. The solution should avoid recursion to prevent stack overflow on deep trees, and must handle cases where `p` or `q` is the root, or where one is the ancestor of the other.
*/

#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Returns the lowest common ancestor of two nodes in a BST.
// Assumes p and q are non-null and exist in the tree.
TreeNode* lowestCommonAncestorBST(TreeNode* root, TreeNode* p, TreeNode* q) {
    TreeNode* current = root;
    while (current != nullptr) {
        // Both nodes are in the right subtree.
        if (p->val > current->val && q->val > current->val) {
            current = current->right;
        }
        // Both nodes are in the left subtree.
        else if (p->val < current->val && q->val < current->val) {
            current = current->left;
        }
        // Current node is the LCA.
        else {
            return current;
        }
    }
    return nullptr; // Should never reach here if inputs are valid.
}

#include <cassert>

// Reuse the TreeNode definition from solution (or include here for completeness).
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Function prototype (should match solution).
TreeNode* lowestCommonAncestorBST(TreeNode* root, TreeNode* p, TreeNode* q);

int main() {
    // Build a BST: [6,2,8,0,4,7,9,null,null,3,5]
    //        6
    //       / \
    //      2   8
    //     / \ / \
    //    0  4 7  9
    //      / \
    //     3   5
    TreeNode* root = new TreeNode(6);
    root->left = new TreeNode(2);
    root->right = new TreeNode(8);
    root->left->left = new TreeNode(0);
    root->left->right = new TreeNode(4);
    root->right->left = new TreeNode(7);
    root->right->right = new TreeNode(9);
    root->left->right->left = new TreeNode(3);
    root->left->right->right = new TreeNode(5);

    // Test 1: LCA of 2 and 8 is 6
    TreeNode* p = root->left;  // value 2
    TreeNode* q = root->right; // value 8
    assert(lowestCommonAncestorBST(root, p, q) == root);

    // Test 2: LCA of 2 and 4 is 2 (p is ancestor of q)
    p = root->left;        // 2
    q = root->left->right; // 4
    assert(lowestCommonAncestorBST(root, p, q) == p);

    // Test 3: LCA of 0 and 5 is 2
    p = root->left->left;        // 0
    q = root->left->right->right; // 5
    assert(lowestCommonAncestorBST(root, p, q) == root->left);

    // Test 4: LCA of 7 and 9 is 8
    p = root->right->left;  // 7
    q = root->right->right; // 9
    assert(lowestCommonAncestorBST(root, p, q) == root->right);

    // Test 5: LCA of 3 and 5 is 4
    p = root->left->right->left;  // 3
    q = root->left->right->right; // 5
    assert(lowestCommonAncestorBST(root, p, q) == root->left->right);

    // Test 6: LCA of root (6) and any node is root
    p = root;                    // 6
    q = root->left->right->left; // 3
    assert(lowestCommonAncestorBST(root, p, q) == root);

    // Test 7: LCA of 0 and 0 (same node) is that node
    p = root->left->left; // 0
    q = root->left->left; // 0
    assert(lowestCommonAncestorBST(root, p, q) == p);

    // Cleanup not necessary for test, but in real code would delete tree nodes.

    return 0;
}

// The algorithm exploits the BST property: for any node, all values in the left subtree are smaller, and all in the right subtree are larger. Starting from the root, we traverse downward. At each node, we compare its value with the values of `p` and `q`. If both values are greater than the current node's value, the LCA must be in the right subtree, so we move right. If both are smaller, we move left. Otherwise, one is on the left and the other on the right (or one equals the current node), meaning the current node is the LCA. This works because when the values diverge (one ≤ node ≤ other), the current node is the first node where `p` and `q` are separated into different subtrees, or one equals the node. No explicit ancestor tracking is needed. Edge cases: if `p` or `q` is the root, the loop immediately returns root; if one is an ancestor of the other, the loop stops at the ancestor because the other node's value will be either greater or smaller than the ancestor, but not both on the same side. Time complexity is O(h) where h is the tree height (O(log n) for balanced, O(n) worst-case skew). Space complexity is O(1) auxiliary, since we use only a single pointer traversal. The iterative approach avoids recursion overhead.
