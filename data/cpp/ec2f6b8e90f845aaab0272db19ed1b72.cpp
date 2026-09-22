Write a C++ function that inserts a given integer value into a Binary Search Tree (BST) whose nodes store integers. The function should accept a pointer to the root of the BST (which may be `nullptr`) and the integer value to insert. It must return the root pointer of the BST after insertion, preserving the BST property (all nodes in the left subtree of a node have values less than the node's value, and all nodes in the right subtree have values greater than or equal to the node's value). If the tree is empty, the new node becomes the root. Duplicate values are allowed and should be inserted to the right. The function should modify the tree in place and not change the structure of existing nodes.

// The standard iterative insertion algorithm for a BST is used: start at the root and traverse down while comparing the target value with the current node's value. If the value is less than the current node's value, move to the left child; otherwise (greater than or equal), move to the right child. When a `nullptr` child is encountered, that is the correct position to create a new node with the given value and attach it. For an empty tree, simply create and return the new node. The traversal is guaranteed to terminate because each step moves to a child, and the tree is finite. Edge cases include inserting into an empty tree, inserting a value that equals an existing node (goes right), and inserting the smallest or largest value (ends up as the leftmost or rightmost leaf). Time complexity is \(O(h)\) where \(h\) is the height of the tree (which could be \(O(n)\) in a skewed tree), and space complexity is \(O(1)\) auxiliary (no recursion, only a few pointers).

#include <cstddef>

template <typename T>
class TreeNode {
public:
    T val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(T value) : val(value), left(nullptr), right(nullptr) {}
};

// Inserts a new node with the given value into the BST rooted at `root`.
// Returns the root of the updated tree.
template <typename T>
TreeNode<T>* insertIntoBST(TreeNode<T>* root, T val) {
    if (root == nullptr) {
        return new TreeNode<T>(val);
    }
    TreeNode<T>* curr = root;
    while (true) {
        if (val < curr->val) {
            if (curr->left == nullptr) {
                curr->left = new TreeNode<T>(val);
                break;
            }
            curr = curr->left;
        } else {
            if (curr->right == nullptr) {
                curr->right = new TreeNode<T>(val);
                break;
            }
            curr = curr->right;
        }
    }
    return root;
}

#include <cassert>
#include <iostream>

// TreeNode definition is assumed from the solution.

// Helper to check BST property and count nodes
bool isBST(TreeNode<int>* node, int minVal, int maxVal) {
    if (node == nullptr) return true;
    if (node->val < minVal || node->val > maxVal) return false;
    return isBST(node->left, minVal, node->val) && isBST(node->right, node->val, maxVal);
}

int main() {
    // Test 1: Insert into empty tree
    TreeNode<int>* root1 = nullptr;
    root1 = insertIntoBST(root1, 5);
    assert(root1 != nullptr);
    assert(root1->val == 5);
    assert(root1->left == nullptr && root1->right == nullptr);

    // Test 2: Insert into existing tree, valid BST
    TreeNode<int>* root2 = new TreeNode<int>(10);
    root2->left = new TreeNode<int>(5);
    root2->right = new TreeNode<int>(15);
    root2 = insertIntoBST(root2, 7);
    assert(isBST(root2, INT_MIN, INT_MAX));
    assert(root2->left->right->val == 7);

    // Test 3: Insert duplicate value (goes right)
    TreeNode<int>* root3 = new TreeNode<int>(10);
    root3 = insertIntoBST(root3, 10);
    assert(root3->right != nullptr);
    assert(root3->right->val == 10);
    assert(root3->left == nullptr);

    // Test 4: Insert minimum value
    TreeNode<int>* root4 = new TreeNode<int>(10);
    root4->left = new TreeNode<int>(8);
    root4 = insertIntoBST(root4, 3);
    assert(root4->left->left->val == 3);

    // Test 5: Insert maximum value
    TreeNode<int>* root5 = new TreeNode<int>(10);
    root5->right = new TreeNode<int>(12);
    root5 = insertIntoBST(root5, 20);
    assert(root5->right->right->val == 20);

    // Test 6: Many insertions still produce valid BST
    TreeNode<int>* root6 = nullptr;
    for (int v : {15, 10, 20, 8, 12, 17, 25}) {
        root6 = insertIntoBST(root6, v);
    }
    assert(isBST(root6, INT_MIN, INT_MAX));

    // Test 7: Tree with left and right after insertion
    TreeNode<int>* root7 = new TreeNode<int>(50);
    root7->left = new TreeNode<int>(30);
    root7->right = new TreeNode<int>(70);
    root7 = insertIntoBST(root7, 60);
    assert(root7->right->left->val == 60);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
