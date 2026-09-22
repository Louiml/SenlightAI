// Given a binary tree where each node stores an integer value and pointers to its left and right children, write a C++ function `TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q)` that returns the lowest common ancestor (LCA) of two given nodes `p` and `q` in the tree. The LCA of two nodes is defined as the deepest node that has both `p` and `q` as descendants (where a node is considered a descendant of itself). Assume `p` and `q` are distinct and both exist in the tree. The tree nodes are defined exactly as in the prompt. The function should work for any binary tree (not necessarily a binary search tree) and handle cases where `root` is `nullptr`.

#include <cassert>

int main() {
    // Build a tree:
    //          3
    //         / \
    //        5   1
    //       / \ / \
    //      6  2 0  8
    //        / \
    //       7   4
    TreeNode* n3 = new TreeNode(3);
    TreeNode* n5 = new TreeNode(5);
    TreeNode* n1 = new TreeNode(1);
    TreeNode* n6 = new TreeNode(6);
    TreeNode* n2 = new TreeNode(2);
    TreeNode* n0 = new TreeNode(0);
    TreeNode* n8 = new TreeNode(8);
    TreeNode* n7 = new TreeNode(7);
    TreeNode* n4 = new TreeNode(4);

    n3->left = n5;
    n3->right = n1;
    n5->left = n6;
    n5->right = n2;
    n1->left = n0;
    n1->right = n8;
    n2->left = n7;
    n2->right = n4;

    // LCA of 5 and 1 is 3
    assert(lowestCommonAncestor(n3, n5, n1) == n3);
    // LCA of 6 and 4 is 5
    assert(lowestCommonAncestor(n3, n6, n4) == n5);
    // LCA of 7 and 4 is 2
    assert(lowestCommonAncestor(n3, n7, n4) == n2);
    // LCA of 5 and 4 is 5 (since 5 is ancestor of 4)
    assert(lowestCommonAncestor(n3, n5, n4) == n5);
    // LCA of 0 and 8 is 1
    assert(lowestCommonAncestor(n3, n0, n8) == n1);
    // LCA of 3 and 8 is 3 (root is ancestor)
    assert(lowestCommonAncestor(n3, n3, n8) == n3);
    // LCA of 6 and 0 is 3
    assert(lowestCommonAncestor(n3, n6, n0) == n3);
    // LCA of 7 and 8 is 3
    assert(lowestCommonAncestor(n3, n7, n8) == n3);

    // Clean up memory (optional in test, but good practice)
    // Not doing full deletion for brevity.

    return 0;
}

#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Recursive helper that returns a pointer to the LCA if found in the subtree,
// or a non-null pointer to a single target if exactly one is present.
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (root == nullptr) {
        return nullptr;
    }

    // Recursively search left and right subtrees.
    TreeNode* leftResult = lowestCommonAncestor(root->left, p, q);
    TreeNode* rightResult = lowestCommonAncestor(root->right, p, q);

    // If the current node is one of the targets, it is the LCA.
    if (root == p || root == q) {
        return root;
    }

    // If both subtrees returned a non-null result, current node is the LCA.
    if (leftResult != nullptr && rightResult != nullptr) {
        return root;
    }

    // Otherwise propagate the single non-null result upward.
    if (leftResult != nullptr) {
        return leftResult;
    }
    return rightResult;
}

// The solution uses a recursive post-order traversal. The recursion returns a non-null pointer if the subtree rooted at the current node contains either `p` or `q` (or both). We first recurse on the left and right children. After those recursive calls, we check the current node: if it equals `p` or `q`, we return it immediately because that node itself is a candidate for the LCA (since a node is considered a descendant of itself). Otherwise, if both left and right recursive calls return non-null, then the current node is the LCA because `p` and `q` are in different subtrees. If only one side returns non-null, we propagate that result upward. The key edge case is when the current node is one of the target nodes and the other target is in one of its subtrees — in that case, the current node is the LCA, and it is correctly returned when we check `root == p || root == q`. Time complexity is O(n) because each node is visited at most once. Space complexity is O(h) for the recursion stack, where h is the tree height (worst-case O(n) for a skewed tree).
