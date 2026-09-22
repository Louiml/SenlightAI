/*
Write a C++ function that takes an array of integers and its size, and returns the root pointer of a binary search tree (BST) constructed by inserting the elements in the given order using an *iterative* insertion method (no recursion). Duplicate values must be ignored (not inserted). The function should be named `buildBSTFromArray` and take parameters `(const std::vector<int>& values)`. It should return a `Node*` pointing to the root of the constructed BST. The `Node` structure is defined as: `struct Node { int data; Node* left; Node* right; };` You may assume the input vector is non-empty.
*/

#include <vector>

struct Node {
    int data;
    Node* left;
    Node* right;
};

// Build a BST by iteratively inserting each value from the vector. Duplicates are ignored.
// Returns pointer to the root of the constructed BST.
Node* buildBSTFromArray(const std::vector<int>& values) {
    if (values.empty()) return nullptr;

    Node* root = new Node{values[0], nullptr, nullptr};

    for (size_t i = 1; i < values.size(); ++i) {
        int key = values[i];
        Node* current = root;
        Node* parent = nullptr;

        // Find insertion position
        while (current != nullptr) {
            parent = current;
            if (key < current->data) {
                current = current->left;
            } else if (key > current->data) {
                current = current->right;
            } else {
                // Duplicate value, skip
                break;
            }
        }

        // If not a duplicate, insert new node
        if (current == nullptr) {
            Node* newNode = new Node{key, nullptr, nullptr};
            if (key < parent->data) {
                parent->left = newNode;
            } else {
                parent->right = newNode;
            }
        }
    }

    return root;
}

#include <cassert>
#include <vector>
#include <iostream>

// Helper to count nodes in a BST
int countNodes(Node* node) {
    if (!node) return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
}

// Helper to check BST property
bool isBST(Node* node, int min, int max) {
    if (!node) return true;
    if (node->data <= min || node->data >= max) return false;
    return isBST(node->left, min, node->data) && isBST(node->right, node->data, max);
}

// Helper to free tree memory
void deleteTree(Node* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

int main() {
    // Test 1: Basic insertion
    {
        std::vector<int> v = {5, 3, 7, 1, 4, 6, 8};
        Node* root = buildBSTFromArray(v);
        assert(root != nullptr);
        assert(root->data == 5);
        assert(root->left->data == 3);
        assert(root->right->data == 7);
        assert(isBST(root, INT_MIN, INT_MAX));
        assert(countNodes(root) == 7);
        deleteTree(root);
    }

    // Test 2: Duplicates ignored
    {
        std::vector<int> v = {4, 2, 4, 6, 2, 6, 8};
        Node* root = buildBSTFromArray(v);
        assert(countNodes(root) == 4); // 4, 2, 6, 8
        assert(isBST(root, INT_MIN, INT_MAX));
        deleteTree(root);
    }

    // Test 3: Single element
    {
        std::vector<int> v = {42};
        Node* root = buildBSTFromArray(v);
        assert(root != nullptr);
        assert(root->data == 42);
        assert(root->left == nullptr && root->right == nullptr);
        assert(countNodes(root) == 1);
        deleteTree(root);
    }

    // Test 4: Already sorted (skewed right)
    {
        std::vector<int> v = {1, 2, 3, 4, 5};
        Node* root = buildBSTFromArray(v);
        assert(root->data == 1);
        assert(root->right->data == 2);
        assert(root->right->right->data == 3);
        assert(isBST(root, INT_MIN, INT_MAX));
        assert(countNodes(root) == 5);
        deleteTree(root);
    }

    // Test 5: Reverse sorted (skewed left)
    {
        std::vector<int> v = {9, 7, 5, 3, 1};
        Node* root = buildBSTFromArray(v);
        assert(root->data == 9);
        assert(root->left->data == 7);
        assert(root->left->left->data == 5);
        assert(isBST(root, INT_MIN, INT_MAX));
        assert(countNodes(root) == 5);
        deleteTree(root);
    }

    // Test 6: All duplicates
    {
        std::vector<int> v = {3, 3, 3, 3};
        Node* root = buildBSTFromArray(v);
        assert(root != nullptr);
        assert(root->data == 3);
        assert(countNodes(root) == 1);
        assert(isBST(root, INT_MIN, INT_MAX));
        deleteTree(root);
    }

    // Test 7: Negative and large numbers
    {
        std::vector<int> v = {-10, 20, -30, 0, 15, -5, 25};
        Node* root = buildBSTFromArray(v);
        assert(isBST(root, INT_MIN, INT_MAX));
        assert(countNodes(root) == 7);
        deleteTree(root);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The solution constructs a BST iteratively. We start with an empty root. For each value in the vector, we traverse the tree from the root to find the correct insertion position, keeping track of the parent node. If the value is less than current node's data, go left; if greater, go right; if equal, skip (duplicate). When we reach a null position, we create a new node and attach it to the parent. The algorithm maintains the BST property: left subtree < parent < right subtree. Edge cases include inserting the first element (root creation), duplicate values (ignored), and values that require deep traversal. Time complexity is O(n * h) where h is the height of the tree (worst-case O(n^2) for skewed trees, average O(n log n) for random data). Space complexity is O(n) for the tree itself plus O(1) auxiliary space (excluding recursion stack since it's iterative).
