Write a C++ function `bool searchBST(const TreeNode* root, int target)` that determines whether a given integer value exists in a binary search tree (BST). The function must work with a binary tree where each node has an integer `val` and pointers to left and right children (using the standard `TreeNode` struct). The function should perform the search iteratively (no recursion) to demonstrate understanding of BST properties, and must correctly handle null trees, single-node trees, and cases where the target is less than, greater than, or equal to each node's value. Additionally, ensure the function does not modify the tree and uses only constant extra space (O(1) auxiliary memory).
// The algorithm exploits the BST property: for any node, all values in the left subtree are strictly less than the node's value, and all values in the right subtree are strictly greater. Starting from the root, compare the target with the current node's value. If equal, return true. If the target is smaller, move to the left child; if larger, move to the right child. Repeat until reaching a null pointer, which means the target is absent. Edge cases include an empty tree (null root) returning false immediately, and a target equal to the root or any internal node returning true. No recursion is used; the loop runs in O(h) time, where h is the tree height (O(log n) for balanced trees, O(n) worst-case for skewed trees), and O(1) extra space.
#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Iteratively search for target in the BST rooted at 'root'.
// Returns true if target exists, false otherwise.
// The function does not modify the tree and uses only constant extra space.
bool searchBST(const TreeNode* root, int target) {
    const TreeNode* current = root;
    while (current != nullptr) {
        if (target == current->val) {
            return true;
        } else if (target < current->val) {
            current = current->left;
        } else { // target > current->val
            current = current->right;
        }
    }
    return false;
}
#include <cassert>

// Reuse the TreeNode definition from above.
int main() {
    // Build BST:        10
    //                 /    \
    //                5      20
    //               / \     / \
    //              3   7   15  25
    TreeNode* root = new TreeNode(10);
    root->left = new TreeNode(5);
    root->right = new TreeNode(20);
    root->left->left = new TreeNode(3);
    root->left->right = new TreeNode(7);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(25);

    // Present values
    assert(searchBST(root, 10) == true);
    assert(searchBST(root, 3) == true);
    assert(searchBST(root, 25) == true);
    assert(searchBST(root, 7) == true);

    // Absent values
    assert(searchBST(root, 4) == false);
    assert(searchBST(root, 100) == false);
    assert(searchBST(root, -1) == false);

    // Null tree
    assert(searchBST(nullptr, 5) == false);

    // Single-node tree
    TreeNode* single = new TreeNode(42);
    assert(searchBST(single, 42) == true);
    assert(searchBST(single, 41) == false);
    assert(searchBST(single, 43) == false);

    // Skewed tree (left chain): 1 -> 2 -> 3
    TreeNode* skewed = new TreeNode(1);
    skewed->right = new TreeNode(2);
    skewed->right->right = new TreeNode(3);
    assert(searchBST(skewed, 3) == true);
    assert(searchBST(skewed, 0) == false);

    // Clean up (optional, but good practice)
    // In a full solution you'd delete all nodes; omitted for brevity.
}
