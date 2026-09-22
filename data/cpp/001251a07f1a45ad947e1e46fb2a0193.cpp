/*
Write a C++ function that takes a binary search tree (BST) implemented via the provided `Node` and `BST` classes (you may copy them into your solution) and returns the maximum value stored in the tree. The tree may be empty, may contain only one node, or may contain duplicate values. However, the BST insertion logic enforces uniqueness, so duplicates will not occur. Your function should not modify the tree, and it must handle cases where the tree is empty by returning a sentinel value like `INT_MIN` (or `-1` if all node values are guaranteed non‑negative, but for full generality use `INT_MIN`). The function should be a free function, not a member of `BST`, and must be declared `const`‑correct (take a `const BST&` or a `const Node*`). For testing purposes, you may write a helper to insert nodes, but the tested function itself must be standalone. The input tree is not guaranteed to be balanced.
*/

#include <climits>   // for INT_MIN
#include <algorithm> // for std::max

// Forward declaration of Node to match the given structure.
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Helper recursive function that computes the maximum value in a subtree.
int findMaxHelper(const Node* node) {
    if (node == nullptr) {
        return INT_MIN;  // sentinel for empty subtree
    }
    int currentMax = node->data;
    currentMax = std::max(currentMax, findMaxHelper(node->left));
    currentMax = std::max(currentMax, findMaxHelper(node->right));
    return currentMax;
}

// Public function: returns the maximum value in the entire BST.
// Returns INT_MIN if the tree is empty.
int getMaxValue(const Node* root) {
    return findMaxHelper(root);
}

#include <cassert>
#include <climits>

// The Node structure and getMaxValue function are assumed to be included above.

// Minimal helper to build a tree for testing (not the solution function).
Node* insert(Node* node, int data) {
    if (node == nullptr) return new Node(data);
    if (data < node->data) {
        node->left = insert(node->left, data);
    } else if (data > node->data) {
        node->right = insert(node->right, data);
    }
    return node;
}

// Helper to delete tree and avoid memory leaks.
void deleteTree(Node* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

int main() {
    // Test 1: Empty tree.
    assert(getMaxValue(nullptr) == INT_MIN);

    // Test 2: Single node.
    Node* root1 = new Node(42);
    assert(getMaxValue(root1) == 42);
    deleteTree(root1);

    // Test 3: Tree with positive values.
    Node* root2 = nullptr;
    root2 = insert(root2, 10);
    root2 = insert(root2, 7);
    root2 = insert(root2, 15);
    root2 = insert(root2, 8);
    root2 = insert(root2, 11);
    root2 = insert(root2, 4);
    root2 = insert(root2, 17);
    root2 = insert(root2, 3);
    root2 = insert(root2, 5);
    root2 = insert(root2, 9);
    assert(getMaxValue(root2) == 17);
    deleteTree(root2);

    // Test 4: All negative values.
    Node* root3 = nullptr;
    root3 = insert(root3, -5);
    root3 = insert(root3, -10);
    root3 = insert(root3, -2);
    root3 = insert(root3, -8);
    assert(getMaxValue(root3) == -2);
    deleteTree(root3);

    // Test 5: Left‑skewed tree (worst case recursion).
    Node* root4 = nullptr;
    for (int i = 100; i >= 1; --i) {
        root4 = insert(root4, i);
    }
    assert(getMaxValue(root4) == 100);
    deleteTree(root4);

    // Test 6: Right‑skewed tree.
    Node* root5 = nullptr;
    for (int i = 1; i <= 100; ++i) {
        root5 = insert(root5, i);
    }
    assert(getMaxValue(root5) == 100);
    deleteTree(root5);

    return 0;
}

// The simplest approach is to recursively traverse the entire tree, tracking the maximum value seen. Since the tree is a BST, one might be tempted to only go down the rightmost path — and that works only if the tree is a valid BST (which it is, per the insertion logic). However, to be robust against malformed trees or to demonstrate a general tree‑maximum algorithm, a full traversal is safer. The algorithm: start with an initial maximum of `INT_MIN` (or `-1` if you know all data ≥ 0). Recursively visit every node: compare the node's `data` with the current maximum, then recurse on left and right children. For an empty tree (null root), return the sentinel. The base case is a null node. Edge cases: empty tree (return sentinel), single node (return its data), all negative values (sentinel must be lower than any possible value, so use `INT_MIN`). Time complexity is O(n) where n is the number of nodes, because each node is visited exactly once. Space complexity is O(h) due to recursion stack, where h is the tree height; in the worst‑case skewed tree h = n, so O(n) space, but for a balanced tree it is O(log n).
