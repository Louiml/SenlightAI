// Given the root of a binary search tree (BST) where each node contains an integer value, write a C++ function `int findExtreme(const node* root, bool findMin)` that returns either the minimum value (if `findMin` is `true`) or the maximum value (if `findMin` is `false`) using the BST property (left subtree smaller, right subtree larger). The tree may be empty, in which case return `-1`. The function must be `const`-correct (take a `const node*` and not modify the tree). You may assume all node values are non-negative integers (so `-1` safely signals an empty tree). Do not use recursion or any auxiliary data structures—only iterative traversal.
#include <cassert>

int main() {
    // Construct the same BST as in the original snippet:
    //         15
    //        /  \
    //       10   20
    //      / \   / \
    //     8  12 17 25
    node* root = new node(15);
    root->left = new node(10);
    root->right = new node(20);
    root->left->left = new node(8);
    root->left->right = new node(12);
    root->right->left = new node(17);
    root->right->right = new node(25);

    // Test both extremes on a non-empty tree.
    assert(findExtreme(root, true) == 8);   // minimum
    assert(findExtreme(root, false) == 25); // maximum

    // Test a single-node tree.
    node* single = new node(42);
    assert(findExtreme(single, true) == 42);
    assert(findExtreme(single, false) == 42);

    // Test an empty tree.
    assert(findExtreme(nullptr, true) == -1);
    assert(findExtreme(nullptr, false) == -1);

    // Test a left-skewed tree (worst case for minimum).
    node* leftSkew = new node(10);
    leftSkew->left = new node(8);
    leftSkew->left->left = new node(5);
    assert(findExtreme(leftSkew, true) == 5);
    assert(findExtreme(leftSkew, false) == 10);

    // Test a right-skewed tree (worst case for maximum).
    node* rightSkew = new node(3);
    rightSkew->right = new node(7);
    rightSkew->right->right = new node(11);
    assert(findExtreme(rightSkew, true) == 3);
    assert(findExtreme(rightSkew, false) == 11);

    // Clean up memory (not strictly required for tests, but good practice).
    // In a real test harness, you would delete nodes recursively.
    // For brevity here we skip deletion.

    return 0;
}
#include <cstddef> // for nullptr

// Node structure for a binary search tree.
struct node {
    int data;
    node* left;
    node* right;
    node(int val) : data(val), left(nullptr), right(nullptr) {}
};

/**
 * Find the minimum or maximum value in a binary search tree.
 * @param root pointer to the root of the BST (may be null).
 * @param findMin if true, return minimum; otherwise return maximum.
 * @return the extreme value, or -1 if the tree is empty.
 */
int findExtreme(const node* root, bool findMin) {
    if (root == nullptr) {
        return -1;
    }

    const node* curr = root;
    if (findMin) {
        // Traverse left to reach the minimum.
        while (curr->left != nullptr) {
            curr = curr->left;
        }
    } else {
        // Traverse right to reach the maximum.
        while (curr->right != nullptr) {
            curr = curr->right;
        }
    }
    return curr->data;
}
// The BST property guarantees that the minimum value is found by repeatedly moving to the left child until no left child exists, and the maximum is found by repeatedly moving to the right child until no right child exists. Since we are only reading the tree and not modifying it, the root pointer is passed as `const node*` and we can use a `const node*` cursor for traversal. The primary edge case is an empty tree (`root == nullptr`), where we return `-1` to indicate no value exists. Another edge case is a tree with only one node—then both minimum and maximum equal the root's value, and the loop simply does not execute. Time complexity is `O(h)`, where `h` is the height of the tree (in the worst case, a skewed tree gives `O(n)` with `n` nodes, but for a balanced BST it is `O(log n)`). Space complexity is `O(1)` because we only use a single pointer variable; no recursion stack or additional storage is used.
