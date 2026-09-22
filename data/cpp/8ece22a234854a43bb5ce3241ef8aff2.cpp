// Write a C++ function named `replaceWithSubtreeSizes` that takes a binary tree represented by a root pointer to a node structure with `left`, `right`, and `val` fields (where `val` is of type `int`), and modifies the tree in-place so that each node's `val` is replaced by the total number of nodes in its own subtree (including itself). For a null tree (empty tree), the function should do nothing. The function should be a free function (not a method of a class) and must handle trees of arbitrary shape, including unbalanced trees and nodes with only one child. The node structure and function signature are provided in the solution template.

The task is to compute the size of every subtree in a binary tree and store that size in each node's value field. The most straightforward approach is a post-order traversal (left, right, then node) because the size of a node's subtree depends on the sizes of its left and right subtrees, which must be computed first. For a null node, the size is 0 (used for propagation). For a leaf, both children are null, so its size is `0 + 0 + 1 = 1`. For an internal node, the size is `size(left) + size(right) + 1`. The recursion naturally handles nodes with one missing child because a null child contributes 0. The base case is a null pointer, which returns 0. The function modifies the tree in-place and does not need to return anything (or could return the size for recursion, but since the task is to write a function that modifies the tree, we can implement a helper that returns the size and use it internally; the public function just takes the root and calls the helper).

Edge cases to consider: an empty tree (root is null) — the function should simply return without doing anything; a tree with a single node — that node gets value 1; unbalanced trees where some nodes have only one child — the missing child contributes 0. The algorithm runs in O(n) time, where n is the number of nodes, because each node is visited exactly once. The space complexity is O(h) due to the recursion stack, where h is the height of the tree (in the worst case O(n) for a skewed tree, O(log n) for a balanced tree).

#include <cstddef>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper function that returns the size of the subtree rooted at node
// and updates node->val to that size.
int computeAndSetSizes(TreeNode* node) {
    if (node == nullptr) {
        return 0;
    }
    int leftSize = computeAndSetSizes(node->left);
    int rightSize = computeAndSetSizes(node->right);
    node->val = leftSize + rightSize + 1;
    return node->val;
}

// Public function: replaces every node's val with the size of its subtree.
void replaceWithSubtreeSizes(TreeNode* root) {
    if (root != nullptr) {
        computeAndSetSizes(root);
    }
}

#include <cassert>

int main() {
    // Test 1: Empty tree (nullptr) should not crash and should do nothing.
    TreeNode* empty = nullptr;
    replaceWithSubtreeSizes(empty);
    assert(empty == nullptr);

    // Test 2: Single node tree.
    TreeNode* single = new TreeNode(0);
    replaceWithSubtreeSizes(single);
    assert(single->val == 1);
    delete single;

    // Test 3: Simple tree: root with two leaves.
    // Structure:
    //     0
    //    / \
    //   0   0
    TreeNode* root3 = new TreeNode(0);
    root3->left = new TreeNode(0);
    root3->right = new TreeNode(0);
    replaceWithSubtreeSizes(root3);
    assert(root3->val == 3);
    assert(root3->left->val == 1);
    assert(root3->right->val == 1);
    delete root3->left;
    delete root3->right;
    delete root3;

    // Test 4: Unbalanced tree with only left children.
    // Structure:
    //     0
    //    /
    //   0
    //  /
    // 0
    TreeNode* root4 = new TreeNode(0);
    root4->left = new TreeNode(0);
    root4->left->left = new TreeNode(0);
    replaceWithSubtreeSizes(root4);
    assert(root4->val == 3);
    assert(root4->left->val == 2);
    assert(root4->left->left->val == 1);
    delete root4->left->left;
    delete root4->left;
    delete root4;

    // Test 5: Tree where a node has only a right child.
    // Structure:
    //     0
    //      \
    //       0
    //      /
    //     0
    TreeNode* root5 = new TreeNode(0);
    root5->right = new TreeNode(0);
    root5->right->left = new TreeNode(0);
    replaceWithSubtreeSizes(root5);
    assert(root5->val == 3);
    assert(root5->right->val == 2);
    assert(root5->right->left->val == 1);
    delete root5->right->left;
    delete root5->right;
    delete root5;

    // Test 6: More complex balanced tree.
    // Structure:
    //         0
    //        / \
    //       0   0
    //      / \   \
    //     0   0   0
    TreeNode* root6 = new TreeNode(0);
    root6->left = new TreeNode(0);
    root6->left->left = new TreeNode(0);
    root6->left->right = new TreeNode(0);
    root6->right = new TreeNode(0);
    root6->right->right = new TreeNode(0);
    replaceWithSubtreeSizes(root6);
    assert(root6->val == 6);
    assert(root6->left->val == 3);
    assert(root6->left->left->val == 1);
    assert(root6->left->right->val == 1);
    assert(root6->right->val == 2);
    assert(root6->right->right->val == 1);
    delete root6->left->left;
    delete root6->left->right;
    delete root6->left;
    delete root6->right->right;
    delete root6->right;
    delete root6;

    return 0;
}
