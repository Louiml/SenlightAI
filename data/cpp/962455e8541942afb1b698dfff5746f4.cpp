// Given a binary tree where each node contains an integer value, write a C++ function `findLCA(TreeNode* root, TreeNode* p, TreeNode* q)` that returns a pointer to the lowest common ancestor (LCA) of two given nodes `p` and `q`. The LCA is defined as the lowest node in the tree that has both `p` and `q` as descendants (where a node is allowed to be a descendant of itself). You may assume that both `p` and `q` exist in the tree, and that they are distinct. The function should not modify the tree. For the tree definition, use the provided `TreeNode` struct with `val`, `left`, and `right` members. The function must be `const`-correct: it should accept the tree as a `const TreeNode*` root and return a `const TreeNode*` result, and it should also accept pointers to the target nodes as `const TreeNode*`.
#include <cassert>

int main() {
    // Build a sample tree:
    //        3
    //       / \
    //      5   1
    //     / \ / \
    //    6  2 0  8
    //      / \
    //     7   4
    TreeNode n3(3), n5(5), n1(1), n6(6), n2(2), n0(0), n8(8), n7(7), n4(4);
    n3.left = &n5; n3.right = &n1;
    n5.left = &n6; n5.right = &n2;
    n1.left = &n0; n1.right = &n8;
    n2.left = &n7; n2.right = &n4;

    const TreeNode* root = &n3;

    // LCA of 5 and 1 is 3
    assert(findLCA(root, &n5, &n1) == &n3);
    // LCA of 5 and 4 is 5 (5 is ancestor of 4)
    assert(findLCA(root, &n5, &n4) == &n5);
    // LCA of 6 and 4 is 5
    assert(findLCA(root, &n6, &n4) == &n5);
    // LCA of 7 and 4 is 2
    assert(findLCA(root, &n7, &n4) == &n2);
    // LCA of 0 and 8 is 1
    assert(findLCA(root, &n0, &n8) == &n1);
    // LCA of 2 and 6 is 5
    assert(findLCA(root, &n2, &n6) == &n5);
    // LCA of 3 and 8 is 3 (root is ancestor of 8)
    assert(findLCA(root, &n3, &n8) == &n3);

    // Test a degenerate tree (linked list): 1 -> 2 -> 3
    TreeNode a(1), b(2), c(3);
    a.right = &b;
    b.right = &c;
    const TreeNode* listRoot = &a;
    // LCA of 2 and 3 is 2
    assert(findLCA(listRoot, &b, &c) == &b);
    // LCA of 1 and 3 is 1
    assert(findLCA(listRoot, &a, &c) == &a);

    // Verify const correctness: calling with const pointers compiles.
    const TreeNode* constP = &n7;
    const TreeNode* constQ = &n4;
    assert(findLCA(root, constP, constQ) == &n2);

    return 0;
}
#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// Find the lowest common ancestor of two nodes p and q in a binary tree.
// Assumes both p and q exist in the tree and are distinct.
const TreeNode* findLCA(const TreeNode* root, const TreeNode* p, const TreeNode* q) {
    if (root == nullptr || root == p || root == q) {
        return root;
    }

    const TreeNode* left = findLCA(root->left, p, q);
    const TreeNode* right = findLCA(root->right, p, q);

    if (left != nullptr && right != nullptr) {
        // p and q are in different subtrees, so root is the LCA.
        return root;
    }
    if (left != nullptr) {
        return left;
    }
    return right;  // Might be nullptr, but the assumption guarantees it won't be if both exist.
}
// The classic recursive approach works by traversing the tree from the root downward. At each node, we perform a recursive search in the left and right subtrees for either `p` or `q`. The base case is: if the current node is null, or if it equals either `p` or `q`, return the current node. Otherwise, we recursively search left and right. After both recursive calls return, we have three possibilities: (1) both left and right results are non-null — meaning `p` and `q` were found in different subtrees, so the current node is the LCA; (2) only one side is non-null — then that side’s result is the LCA because both nodes are in that subtree; (3) both are null — this should not happen given the assumption that both nodes exist, but the algorithm would return null. The key insight is that when we encounter a node that matches `p` or `q`, we immediately return it, which handles the case where one node is an ancestor of the other. Edge cases include when `p` and `q` are directly parent-child, when both are in the same subtree, and when the tree has only two nodes. The time complexity is O(n) in the worst case (skewed tree) because we may visit every node, and O(h) average for balanced trees, where h is height. Auxiliary space is O(h) due to recursion stack depth.
