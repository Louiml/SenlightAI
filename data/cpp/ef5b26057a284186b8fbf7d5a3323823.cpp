Given a Binary Search Tree (BST) where each node contains an integer value and pointers to left and right children, write a C++ function that takes the root of the BST and a pointer to a node `p` (which is guaranteed to exist in the tree) and returns a pointer to the in-order successor of `p` in the BST. The in-order successor of a node is the node with the smallest value greater than `p->val`. If `p` is the maximum-valued node in the tree (i.e., it has no successor), the function must return `nullptr`. The tree is not modified, and the function must run efficiently without using recursion or extra memory beyond a few local variables. The input tree and node pointers are non-`null`, but the successor may be absent. For example, in a BST with values `[2,1,3]` rooted at node value `2`, the successor of the node with value `1` is the node with value `2`, and the successor of the node with value `3` is `nullptr`.

// The key insight is that for an in-order successor of `p` in a BST, we look for the smallest node whose value is strictly greater than `p->val`. We can find this by traversing from the root downward: if the current node's value is less than or equal to `p->val`, then this node and everything in its left subtree cannot be a successor (they are either smaller or equal), so we move to the right child. If the current node's value is greater than `p->val`, then this node is a potential successor, but there might be a smaller successor in its left subtree, so we record this node as the current best answer and move to the left child. This process continues until we reach a null pointer. At the end, the recorded candidate (if any) is the in-order successor. Edge cases: if `p` is the maximum node, we will never find a node greater than `p->val`, so the answer remains `nullptr`. Duplicate values are handled by the `<=` condition, ensuring we only consider strictly greater values. The algorithm runs in `O(h)` time where `h` is the height of the tree (worst-case `O(n)` for a skewed tree, average `O(log n)` for balanced), and uses `O(1)` auxiliary space beyond the pointer variables.

#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Returns pointer to the in-order successor of node p in the BST rooted at root.
// Returns nullptr if p has no successor (p is the maximum node).
TreeNode* inorderSuccessorBST(TreeNode* root, TreeNode* p) {
    TreeNode* current = root;
    TreeNode* successor = nullptr;
    while (current != nullptr) {
        if (current->val <= p->val) {
            // current and its left subtree are not candidates; go right
            current = current->right;
        } else {
            // current is a potential successor; try to find a smaller one on the left
            successor = current;
            current = current->left;
        }
    }
    return successor;
}

#include <cassert>

int main() {
    // Build a BST:       5
    //                  /   \
    //                 3     7
    //                / \   / \
    //               2   4 6   8
    TreeNode n2(2), n4(4), n3(3), n6(6), n8(8), n7(7), n5(5);
    n3.left = &n2; n3.right = &n4;
    n7.left = &n6; n7.right = &n8;
    n5.left = &n3; n5.right = &n7;

    assert(inorderSuccessorBST(&n5, &n2) == &n3);
    assert(inorderSuccessorBST(&n5, &n3) == &n4);
    assert(inorderSuccessorBST(&n5, &n4) == &n5);
    assert(inorderSuccessorBST(&n5, &n5) == &n6);
    assert(inorderSuccessorBST(&n5, &n6) == &n7);
    assert(inorderSuccessorBST(&n5, &n7) == &n8);
    assert(inorderSuccessorBST(&n5, &n8) == nullptr);

    // Single-node tree
    TreeNode single(10);
    assert(inorderSuccessorBST(&single, &single) == nullptr);

    // Left-skewed tree: 1 -> 2 -> 3 (root is 1)
    TreeNode n1(1), n2s(2), n3s(3);
    n1.right = &n2s; n2s.right = &n3s;
    assert(inorderSuccessorBST(&n1, &n1) == &n2s);
    assert(inorderSuccessorBST(&n1, &n2s) == &n3s);
    assert(inorderSuccessorBST(&n1, &n3s) == nullptr);

    return 0;
}
