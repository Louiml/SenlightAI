/*
Given a binary tree where each node stores an integer value, write a C++ function `addOneRow(TreeNode* root, int v, int d)` that inserts a new row of nodes with value `v` at depth `d` (the root is at depth 1). For every node at depth `d-1`, replace its left child with a new node with value `v` whose left child is the original left child, and replace its right child with a new node with value `v` whose right child is the original right child. If `d == 1`, create a new root node with value `v` and make the original tree its left child. The function should return the root of the modified tree. You may assume the tree is non-empty and `d` is at least 1, but `d` may be greater than the tree height — in that case, simply return the original tree unchanged. The tree node structure is provided as `struct TreeNode` with `val`, `left`, and `right` members, and a default constructor, a value constructor, and a three-parameter constructor. Write the solution as a free function that matches the specification and uses `TreeNode*` as the return type.
*/

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

// Helper to recursively insert a row at depth d (1-indexed).
void insertRowHelper(TreeNode* node, int currentDepth, int targetDepth, int value) {
    if (node == nullptr) {
        return;
    }
    if (currentDepth == targetDepth - 1) {
        // Save original children
        TreeNode* oldLeft = node->left;
        TreeNode* oldRight = node->right;
        // Create new row nodes
        node->left = new TreeNode(value);
        node->left->left = oldLeft;
        node->right = new TreeNode(value);
        node->right->right = oldRight;
        return;
    }
    insertRowHelper(node->left, currentDepth + 1, targetDepth, value);
    insertRowHelper(node->right, currentDepth + 1, targetDepth, value);
}

// Adds a row of nodes with value 'v' at depth 'd' (root depth = 1).
TreeNode* addOneRow(TreeNode* root, int v, int d) {
    if (d == 1) {
        TreeNode* newRoot = new TreeNode(v);
        newRoot->left = root;
        return newRoot;
    }
    insertRowHelper(root, 1, d, v);
    return root;
}

#include <cassert>

// Helper to delete tree (not strictly needed for asserts but good practice)
void deleteTree(TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// Helper to compare two trees structurally and by values
bool areEqual(TreeNode* a, TreeNode* b) {
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;
    return (a->val == b->val) &&
           areEqual(a->left, b->left) &&
           areEqual(a->right, b->right);
}

int main() {
    // Test 1: d == 1, should create a new root
    TreeNode* t1 = new TreeNode(1);
    TreeNode* r1 = addOneRow(t1, 5, 1);
    TreeNode* expected1 = new TreeNode(5, new TreeNode(1), nullptr);
    assert(areEqual(r1, expected1));
    deleteTree(r1);
    deleteTree(expected1);

    // Test 2: d == 2 on a simple tree
    TreeNode* t2 = new TreeNode(4);
    t2->left = new TreeNode(2);
    t2->right = new TreeNode(6);
    TreeNode* r2 = addOneRow(t2, 9, 2);
    TreeNode* expected2 = new TreeNode(4,
                              new TreeNode(9, new TreeNode(2), nullptr),
                              new TreeNode(9, nullptr, new TreeNode(6)));
    assert(areEqual(r2, expected2));
    deleteTree(r2);
    deleteTree(expected2);

    // Test 3: d > height, should return tree unchanged
    TreeNode* t3 = new TreeNode(1);
    TreeNode* r3 = addOneRow(t3, 7, 5);
    TreeNode* expected3 = new TreeNode(1);
    assert(areEqual(r3, expected3));
    deleteTree(r3);
    deleteTree(expected3);

    // Test 4: d == 2 on a leaf (height 1), adds both children
    TreeNode* t4 = new TreeNode(10);
    TreeNode* r4 = addOneRow(t4, -3, 2);
    TreeNode* expected4 = new TreeNode(10,
                              new TreeNode(-3),
                              new TreeNode(-3));
    assert(areEqual(r4, expected4));
    deleteTree(r4);
    deleteTree(expected4);

    // Test 5: deep tree, d == 4, adding row above leaves
    // Build:       1
    //            /   \
    //           2     3
    //          / \   / \
    //         4   5 6   7
    TreeNode* t5 = new TreeNode(1);
    t5->left = new TreeNode(2);
    t5->right = new TreeNode(3);
    t5->left->left = new TreeNode(4);
    t5->left->right = new TreeNode(5);
    t5->right->left = new TreeNode(6);
    t5->right->right = new TreeNode(7);
    TreeNode* r5 = addOneRow(t5, 100, 4);
    // Expected after insertion at depth 4:
    //       1
    //    /     \
    //   2       3
    //  / \     / \
    // 4   5   6   7
    // |   |   |   |
    // 100 100 100 100 (each leaf gets a child with value 100, but no grandchildren)
    TreeNode* expected5 = new TreeNode(1,
                            new TreeNode(2,
                                new TreeNode(4, new TreeNode(100), nullptr),
                                new TreeNode(5, new TreeNode(100), nullptr)),
                            new TreeNode(3,
                                new TreeNode(6, new TreeNode(100), nullptr),
                                new TreeNode(7, new TreeNode(100), nullptr)));
    assert(areEqual(r5, expected5));
    deleteTree(r5);
    deleteTree(expected5);

    // Test 6: When d==1 and original tree is empty? Not possible per spec, but test with one node.
    // Already covered in Test 1.

    return 0;
}

// The solution uses a recursive depth-first traversal. The base case occurs when the current node is `nullptr`, in which case we do nothing. The special case `d == 1` is handled at the top level: create a new node with value `v`, set its left pointer to the original root, and return it. For `d > 1`, we recursively traverse the tree, passing the current depth. When we reach a node whose depth equals `d-1`, we patch in the new row: save the original left and right children, then set the node’s left child to a new node with value `v` whose left is the saved left child, and set the node’s right child to a new node with value `v` whose right is the saved right child. If `d` is greater than the tree height, recursion will hit `nullptr` before reaching depth `d-1`, and the tree remains unchanged. The algorithm visits each node once, so time complexity is O(N) where N is the number of nodes in the tree. Auxiliary space is O(H) for the recursion stack, where H is the tree height (worst-case O(N) for a skewed tree). The solution correctly handles edge cases: `d==1` (new root), `d==2` (add children directly at root), and `d` larger than height (no change).
