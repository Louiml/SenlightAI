// Write a C++ function that takes two binary tree root pointers as input and returns a boolean value indicating whether the two trees are structurally identical and have the same node values at corresponding positions. The trees are represented using a node structure with integer data and left/right child pointers. The function should handle empty trees (nullptr roots) gracefully, treating two null trees as identical, and should not modify the input trees. The comparison must be recursive, checking the current node's data and then recursively comparing left and right subtrees in a consistent order.
// The solution uses a recursive depth-first traversal. The base case handles when either or both nodes are null: if both are null, they are identical; if only one is null, they are not. For non-null nodes, we first compare their data values; if they differ, the trees are not identical. Otherwise, we recursively compare the left children of both nodes and the right children of both nodes, and the result is the logical AND of these two recursive comparisons. This ensures both structure and values match. Edge cases include both trees being empty (returns true), one tree empty and the other not (returns false), and trees with different shapes but same values (e.g., a left child vs a right child) — these are correctly handled because the recursive calls are order-sensitive. The time complexity is O(n) where n is the number of nodes in the smaller tree (early exit on mismatch), and the space complexity is O(h) for the recursion stack, where h is the height of the tree (worst case O(n) for skewed trees).
#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Returns true if the two binary trees rooted at p and q are identical
// (same structure and same node values at corresponding positions).
bool areIdentical(TreeNode* p, TreeNode* q) {
    // Both null -> identical
    if (p == nullptr && q == nullptr) return true;
    // Exactly one null -> not identical
    if (p == nullptr || q == nullptr) return false;
    // Check current value and recursively check subtrees
    return (p->val == q->val) &&
           areIdentical(p->left, q->left) &&
           areIdentical(p->right, q->right);
}
#include <cassert>

int main() {
    // Test 1: Both trees empty
    TreeNode* t1 = nullptr;
    TreeNode* t2 = nullptr;
    assert(areIdentical(t1, t2) == true);

    // Test 2: One empty, one not
    TreeNode* t3 = new TreeNode(1);
    assert(areIdentical(t1, t3) == false);
    assert(areIdentical(t3, t1) == false);

    // Test 3: Single node with same value
    TreeNode* t4 = new TreeNode(1);
    assert(areIdentical(t3, t4) == true);

    // Test 4: Single node with different value
    TreeNode* t5 = new TreeNode(2);
    assert(areIdentical(t3, t5) == false);

    // Test 5: More complex identical trees
    // Tree A:      1
    //            /   \
    //           2     3
    //          / \     \
    //         4   5     6
    TreeNode* a = new TreeNode(1);
    a->left = new TreeNode(2);
    a->right = new TreeNode(3);
    a->left->left = new TreeNode(4);
    a->left->right = new TreeNode(5);
    a->right->right = new TreeNode(6);

    TreeNode* b = new TreeNode(1);
    b->left = new TreeNode(2);
    b->right = new TreeNode(3);
    b->left->left = new TreeNode(4);
    b->left->right = new TreeNode(5);
    b->right->right = new TreeNode(6);

    assert(areIdentical(a, b) == true);

    // Test 6: Same structure but different value
    TreeNode* c = new TreeNode(1);
    c->left = new TreeNode(2);
    c->right = new TreeNode(3);
    c->left->left = new TreeNode(4);
    c->left->right = new TreeNode(5);
    c->right->right = new TreeNode(7); // changed from 6 to 7

    assert(areIdentical(a, c) == false);

    // Test 7: Same values but different structure
    TreeNode* d = new TreeNode(1);
    d->left = new TreeNode(2);
    d->right = new TreeNode(3);
    d->left->left = new TreeNode(4);
    d->left->right = new TreeNode(5);
    d->right->left = new TreeNode(6); // moved to left instead of right

    assert(areIdentical(a, d) == false);

    // Test 8: One tree has extra node
    TreeNode* e = new TreeNode(1);
    e->left = new TreeNode(2);
    e->right = new TreeNode(3);
    e->left->left = new TreeNode(4);
    e->left->right = new TreeNode(5);
    e->right->right = new TreeNode(6);
    e->right->right->left = new TreeNode(7); // extra node

    assert(areIdentical(a, e) == false);

    // Clean up (optional, not needed for assert tests but good practice)
    // Note: In a real environment, you'd delete all allocated nodes to avoid leaks.
    // Here we skip deletion for brevity in the test.

    return 0;
}
