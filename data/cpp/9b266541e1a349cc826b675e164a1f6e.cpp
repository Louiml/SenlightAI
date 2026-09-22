/*
Write a C++ function `void recoverBST(TreeNode* root)` that fixes a binary search tree (BST) where exactly two nodes have been accidentally swapped (their values exchanged). The tree contains unique positive integer values. The function must restore the BST in-place without changing the tree structure and without using extra space proportional to the number of nodes (only O(1) auxiliary space is allowed, excluding recursion stack). The input tree is valid except for the two swapped nodes; you may assume the two swapped nodes exist and are distinct.
*/
#include <algorithm>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Fix a BST where exactly two nodes have their values swapped.
void recoverBST(TreeNode* root) {
    TreeNode* first = nullptr;
    TreeNode* last = nullptr;
    TreeNode* prev = nullptr;

    // Helper lambda for in-order traversal (recursive, O(1) extra memory besides stack).
    auto inorder = [&](auto&& self, TreeNode* node) -> void {
        if (node == nullptr) return;
        self(self, node->left);
        if (prev != nullptr && prev->val > node->val) {
            if (first == nullptr) {
                first = prev;
            }
            last = node;
        }
        prev = node;
        self(self, node->right);
    };

    if (root == nullptr) return;
    inorder(inorder, root);
    std::swap(first->val, last->val);
}
#include <cassert>

int main() {
    // Helper to check if a tree is a valid BST (in-order traversal is increasing).
    auto isValidBST = [](TreeNode* root) {
        int prevVal = 0;
        bool first = true;
        bool ok = true;
        auto inorder = [&](auto&& self, TreeNode* node) -> void {
            if (node == nullptr || !ok) return;
            self(self, node->left);
            if (first) {
                prevVal = node->val;
                first = false;
            } else {
                if (prevVal >= node->val) ok = false;
                prevVal = node->val;
            }
            self(self, node->right);
        };
        inorder(inorder, root);
        return ok;
    };

    // Test 1: Two nodes swapped, not adjacent in in-order order.
    TreeNode* t1 = new TreeNode(10);
    t1->left = new TreeNode(5);
    t1->right = new TreeNode(15);
    t1->left->left = new TreeNode(2);
    t1->left->right = new TreeNode(7);
    t1->right->left = new TreeNode(12);
    t1->right->right = new TreeNode(20);
    // Swap 7 and 20 (not adjacent in in-order).
    std::swap(t1->left->right->val, t1->right->right->val);
    recoverBST(t1);
    assert(isValidBST(t1)); // Tree should be valid after fix.

    // Test 2: Two swapped nodes are adjacent in in-order order (e.g., 7 and 10).
    TreeNode* t2 = new TreeNode(7);
    t2->left = new TreeNode(3);
    t2->right = new TreeNode(10);
    t2->left->left = new TreeNode(1);
    t2->left->right = new TreeNode(5);
    t2->right->left = new TreeNode(8);
    t2->right->right = new TreeNode(12);
    // Swap 5 and 8 (these are adjacent in in-order: ...5,8...).
    std::swap(t2->left->right->val, t2->right->left->val);
    recoverBST(t2);
    assert(isValidBST(t2));

    // Test 3: Single-node tree (no swap needed; function should not crash).
    TreeNode* t3 = new TreeNode(42);
    recoverBST(t3);
    assert(isValidBST(t3));

    // Test 4: Root and a deep node swapped.
    TreeNode* t4 = new TreeNode(100);
    t4->left = new TreeNode(50);
    t4->right = new TreeNode(150);
    t4->left->left = new TreeNode(25);
    t4->left->right = new TreeNode(75);
    t4->right->left = new TreeNode(125);
    t4->right->right = new TreeNode(175);
    // Swap root 100 and leaf 25.
    std::swap(t4->val, t4->left->left->val);
    recoverBST(t4);
    assert(isValidBST(t4));

    // Test 5: Swap two nodes that are far apart in a larger tree.
    TreeNode* t5 = new TreeNode(8);
    t5->left = new TreeNode(3);
    t5->right = new TreeNode(10);
    t5->left->left = new TreeNode(1);
    t5->left->right = new TreeNode(6);
    t5->right->right = new TreeNode(14);
    t5->left->right->left = new TreeNode(4);
    t5->left->right->right = new TreeNode(7);
    t5->right->right->left = new TreeNode(13);
    // Swap 1 and 14.
    std::swap(t5->left->left->val, t5->right->right->val);
    recoverBST(t5);
    assert(isValidBST(t5));

    // Test 6: Null root should not crash.
    recoverBST(nullptr);
    assert(true);

    return 0;
}
// The standard approach is to perform an in-order traversal of the BST. In a correct BST, the in-order traversal yields strictly increasing values. When exactly two nodes are swapped, exactly one “descent” (a pair where previous value > current value) will occur if the swapped nodes are not adjacent in in-order order, or exactly two descents if they are adjacent (the two descents merge into one pair of adjacent nodes). During traversal, track three pointers: `first`, `last`, and `prev`. When a descent is found: if `first` is null, set `first = prev` (the larger node of the first descent). Always set `last = current` (the smaller node). After traversal, swap the values of `first` and `last`. Edge cases: if root is null, do nothing; if the two swapped nodes are adjacent in in-order order, the first descent sets `first` and `last`, and the second descent (if any) just updates `last` to the correct node. The algorithm runs in O(n) time (each node visited once) and uses O(h) recursion stack space, where h is the tree height, but no additional heap allocation. The solution respects `const` correctness only where applicable; since the function modifies the tree, it takes a non-const pointer.
