Write a C++ function `TreeNode* insertIntoBST(TreeNode* root, int val)` that inserts a new node with the given integer value into an existing binary search tree (BST) and returns the root of the tree after insertion. The BST is defined by a standard `TreeNode` struct with integer `val` and pointers `left` and `right`. The value to insert is guaranteed not to already exist in the tree. The function must preserve the BST property (all values in the left subtree are smaller, all in the right subtree are larger). When the tree is empty (root is `nullptr`), the function should create and return a new node containing the value. The insertion must follow the standard BST search path: compare the value with the current node, move left if smaller, right if larger, and attach the new node as a leaf in the correct position. You may write either an iterative or recursive solution, but the function signature must match exactly.
The main algorithm follows the binary search tree insertion procedure. Start at the root and repeatedly compare the target value with the current node's value. Since the value is guaranteed to be absent, no equal-case handling is needed; if it were present, standard BST insertion would either do nothing or violate the guarantee, so we can ignore that scenario. If the value is greater than the current node's value, move to the right child; if the right child exists, continue the loop, otherwise create a new node and attach it as the right child, then stop. Similarly, if the value is smaller, move to the left child, and if the left child is null, attach the new node there. The loop terminates once a leaf position is found. Edge cases include an empty tree (root is `nullptr`), where we simply return a new node with the value. The tree structure after insertion is guaranteed to remain a valid BST because we only insert at a leaf position following the ordering rule. Time complexity is O(h), where h is the height of the tree; in the worst case (a skewed tree) this is O(n), and in a balanced tree it is O(log n). Space complexity is O(1) for the iterative approach, as we only use a pointer to traverse, and the new node allocation is constant. The solution provided below uses an iterative approach with a pointer that starts at the root and walks down the tree.
#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Inserts a new node with the given value into the BST and returns the root.
// The value is guaranteed not to exist in the tree. If the root is null,
// creates and returns a new node.
TreeNode* insertIntoBST(TreeNode* root, int val) {
    if (root == nullptr) {
        return new TreeNode(val);
    }

    TreeNode* current = root;
    while (true) {
        if (val < current->val) {
            if (current->left == nullptr) {
                current->left = new TreeNode(val);
                break;
            }
            current = current->left;
        } else { // val > current->val (since no duplicates)
            if (current->right == nullptr) {
                current->right = new TreeNode(val);
                break;
            }
            current = current->right;
        }
    }
    return root;
}
#include <cassert>

// Assume TreeNode and insertIntoBST are defined as above.

// Helper to check if a tree is a valid BST (in-order traversal yields sorted values).
bool isBST(TreeNode* node, int minVal, int maxVal) {
    if (node == nullptr) return true;
    if (node->val <= minVal || node->val >= maxVal) return false;
    return isBST(node->left, minVal, node->val) && isBST(node->right, node->val, maxVal);
}

// Helper to check if a value exists in the tree.
bool contains(TreeNode* node, int val) {
    if (node == nullptr) return false;
    if (node->val == val) return true;
    return val < node->val ? contains(node->left, val) : contains(node->right, val);
}

int main() {
    // Test 1: Insert into an empty tree.
    TreeNode* root1 = nullptr;
    root1 = insertIntoBST(root1, 5);
    assert(root1 != nullptr);
    assert(root1->val == 5);
    assert(root1->left == nullptr && root1->right == nullptr);

    // Test 2: Insert into a single-node tree.
    TreeNode* root2 = new TreeNode(4);
    root2 = insertIntoBST(root2, 2);
    assert(root2->val == 4);
    assert(root2->left != nullptr && root2->left->val == 2);
    assert(root2->right == nullptr);
    assert(isBST(root2, INT_MIN, INT_MAX));

    // Test 3: Insert into a larger tree, check BST property and value presence.
    TreeNode* root3 = new TreeNode(4);
    root3->left = new TreeNode(2);
    root3->right = new TreeNode(7);
    root3->left->left = new TreeNode(1);
    root3->left->right = new TreeNode(3);
    root3 = insertIntoBST(root3, 5);
    assert(contains(root3, 5));
    assert(isBST(root3, INT_MIN, INT_MAX));
    // The new node must be a leaf in the right position.
    // The current tree: 4 left=2 right=7, 2 left=1 right=3, 7 left=5 right=null.
    assert(root3->right->left != nullptr && root3->right->left->val == 5);
    assert(root3->right->left->left == nullptr && root3->right->left->right == nullptr);

    // Test 4: Insert a value smaller than all existing values.
    TreeNode* root4 = new TreeNode(10);
    root4->left = new TreeNode(5);
    root4->right = new TreeNode(15);
    root4 = insertIntoBST(root4, 3);
    assert(contains(root4, 3));
    assert(isBST(root4, INT_MIN, INT_MAX));
    assert(root4->left->left != nullptr && root4->left->left->val == 3);

    // Test 5: Insert a value larger than all existing values.
    TreeNode* root5 = new TreeNode(10);
    root5->left = new TreeNode(5);
    root5->right = new TreeNode(15);
    root5 = insertIntoBST(root5, 20);
    assert(contains(root5, 20));
    assert(isBST(root5, INT_MIN, INT_MAX));
    assert(root5->right->right != nullptr && root5->right->right->val == 20);

    // Test 6: Insert into a left-skewed tree (worst case for performance but works).
    TreeNode* root6 = new TreeNode(1);
    root6->right = new TreeNode(2);
    root6->right->right = new TreeNode(3);
    root6 = insertIntoBST(root6, 4);
    assert(contains(root6, 4));
    assert(isBST(root6, INT_MIN, INT_MAX));
    assert(root6->right->right->right != nullptr && root6->right->right->right->val == 4);

    // Clean up (optional in a real program but good practice).
    // For brevity, we omit deletion of dynamically allocated nodes.

    return 0;
}
