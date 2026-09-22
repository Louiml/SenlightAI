Write a C++ function that takes a pointer to the root of a binary tree where each node stores an integer, and returns the minimum depth of the tree. The minimum depth is defined as the number of nodes along the shortest path from the root down to the nearest leaf node. A leaf is a node with no children. If the tree is empty (root is `nullptr`), return 0. The function must properly handle cases where a node has only one child (left or right), and must not count that missing child as a leaf. Ensure the function is `const`-correct and works for trees with negative values, large positive values, or any integer values.

// The solution uses a recursive depth-first traversal. For a null root, the depth is 0. For a leaf (both children null), the depth is 1. For an internal node, we recursively compute the minimum depth of the left and right subtrees. However, a key edge case arises when one side is null: if the left child is null and the right child is non-null, then the depth is simply the right subtree's depth plus 1, because the null side does not provide a valid path (since a null child is not a leaf, and we cannot go deeper there). Similarly, if the right child is null and the left is non-null, use the left subtree's depth plus 1. When both children are non-null, we take the minimum of the two depths and add 1. Time complexity is O(n) where n is the number of nodes, because each node is visited once. Space complexity is O(h) for the recursion stack, where h is the height of the tree; in the worst case (skewed tree) it is O(n), and in the best case (balanced tree) it is O(log n). The algorithm correctly distinguishes missing children from leaves.

#include <algorithm>  // for std::min

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Returns the minimum depth of the binary tree rooted at `root`.
// An empty tree (root == nullptr) has depth 0.
// For a node with only one child, the missing side is not a leaf,
// so we only consider the existing child's depth.
int minDepth(const TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    if (root->left == nullptr && root->right == nullptr) {
        return 1;
    }
    if (root->left == nullptr) {
        return minDepth(root->right) + 1;
    }
    if (root->right == nullptr) {
        return minDepth(root->left) + 1;
    }
    // Both children exist: take the smaller depth.
    return std::min(minDepth(root->left), minDepth(root->right)) + 1;
}

#include <cassert>

// Test helper to delete a tree and avoid memory leaks.
void deleteTree(TreeNode* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

int main() {
    // Test 1: Empty tree
    assert(minDepth(nullptr) == 0);

    // Test 2: Single node (leaf)
    TreeNode* t2 = new TreeNode(5);
    assert(minDepth(t2) == 1);
    deleteTree(t2);

    // Test 3: Root with left child only -> depth 2
    TreeNode* t3 = new TreeNode(1);
    t3->left = new TreeNode(2);
    assert(minDepth(t3) == 2);
    deleteTree(t3);

    // Test 4: Root with right child only -> depth 2
    TreeNode* t4 = new TreeNode(3);
    t4->right = new TreeNode(4);
    assert(minDepth(t4) == 2);
    deleteTree(t4);

    // Test 5: Root with left child that is a leaf, and right child that has one leaf -> min depth is 2 (left path)
    TreeNode* t5 = new TreeNode(0);
    t5->left = new TreeNode(1);           // leaf
    t5->right = new TreeNode(2);
    t5->right->left = new TreeNode(3);    // leaf
    assert(minDepth(t5) == 2);
    deleteTree(t5);

    // Test 6: Both sides have depth > 1, left depth 3, right depth 4 -> min is 3
    TreeNode* t6 = new TreeNode(10);
    t6->left = new TreeNode(20);
    t6->left->left = new TreeNode(30);
    t6->left->left->left = new TreeNode(40);
    t6->right = new TreeNode(50);
    t6->right->right = new TreeNode(60);
    t6->right->right->right = new TreeNode(70);
    t6->right->right->right->right = new TreeNode(80);
    assert(minDepth(t6) == 3);
    deleteTree(t6);

    // Test 7: Skewed left tree with 5 nodes -> depth 5
    TreeNode* t7 = new TreeNode(1);
    t7->left = new TreeNode(2);
    t7->left->left = new TreeNode(3);
    t7->left->left->left = new TreeNode(4);
    t7->left->left->left->left = new TreeNode(5);
    assert(minDepth(t7) == 5);
    deleteTree(t7);

    // Test 8: Node with negative and positive values, short path via right
    TreeNode* t8 = new TreeNode(-3);
    t8->left = new TreeNode(-10);
    t8->left->left = new TreeNode(-20);
    t8->left->right = new TreeNode(-30);
    t8->right = new TreeNode(7);
    assert(minDepth(t8) == 2); // via right leaf
    deleteTree(t8);

    // Test 9: Only one leaf deep on left, but right is null -> depth uses left only
    TreeNode* t9 = new TreeNode(9);
    t9->left = new TreeNode(8);
    t9->left->right = new TreeNode(7);
    t9->left->right->left = new TreeNode(6);
    assert(minDepth(t9) == 4); // path 9->8->7->6
    deleteTree(t9);

    // Test 10: Perfect binary tree of height 3 (nodes levels 1-3) -> min depth = 3
    TreeNode* t10 = new TreeNode(1);
    t10->left = new TreeNode(2);
    t10->right = new TreeNode(3);
    t10->left->left = new TreeNode(4);
    t10->left->right = new TreeNode(5);
    t10->right->left = new TreeNode(6);
    t10->right->right = new TreeNode(7);
    assert(minDepth(t10) == 3);
    deleteTree(t10);

    return 0;
}
