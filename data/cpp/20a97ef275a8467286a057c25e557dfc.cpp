/*
Write a C++ function that takes a vector of integers and returns a new vector containing the values in non-decreasing order using a binary search tree (BST) structure. The function must handle duplicate values by storing a count with each node rather than inserting duplicates as separate nodes. The input vector may contain negative numbers, positive numbers, and duplicates. The returned vector should include every occurrence of every value, preserving the multiplicity of duplicates, and must be sorted in ascending order.
*/
#include <vector>

struct Node {
    int val;
    int count;
    Node* left;
    Node* right;
    Node(int v) : val(v), count(1), left(nullptr), right(nullptr) {}
};

// Insert a value into the BST, incrementing count if duplicate.
Node* insertNode(Node* root, int key) {
    if (root == nullptr) return new Node(key);
    if (key < root->val) {
        root->left = insertNode(root->left, key);
    } else if (key > root->val) {
        root->right = insertNode(root->right, key);
    } else {
        root->count++;
    }
    return root;
}

// In-order traversal collecting values with multiplicity.
void collectInOrder(const Node* node, std::vector<int>& result) {
    if (node == nullptr) return;
    collectInOrder(node->left, result);
    for (int i = 0; i < node->count; ++i) {
        result.push_back(node->val);
    }
    collectInOrder(node->right, result);
}

// Return sorted vector of all integers from input, preserving duplicates.
std::vector<int> sortUsingBST(const std::vector<int>& input) {
    Node* root = nullptr;
    for (int value : input) {
        root = insertNode(root, value);
    }
    std::vector<int> result;
    collectInOrder(root, result);
    // Free memory (optional but good practice).
    // For simplicity and brevity, we omit deletion here; in production, a destructor would be needed.
    return result;
}
#include <cassert>
#include <vector>

// Assume sortUsingBST is defined above.

int main() {
    assert((sortUsingBST({}) == std::vector<int>{}));
    assert((sortUsingBST({5}) == std::vector<int>{5}));
    assert((sortUsingBST({3, 1, 2}) == std::vector<int>{1, 2, 3}));
    assert((sortUsingBST({4, 4, 4}) == std::vector<int>{4, 4, 4}));
    assert((sortUsingBST({10, -3, 0, 10, -3, 7}) == std::vector<int>{-3, -3, 0, 7, 10, 10}));
    assert((sortUsingBST({100, 50, 150, 25, 75, 125, 175}) == std::vector<int>{25, 50, 75, 100, 125, 150, 175}));
    assert((sortUsingBST({-1, -2, -3, -3, -2, -1}) == std::vector<int>{-3, -3, -2, -2, -1, -1}));
    assert((sortUsingBST({0, 0, 0, 0}) == std::vector<int>{0, 0, 0, 0}));
    return 0;
}
// The solution builds a BST from the input values. Each node stores a value, a count of how many times that value appeared, and pointers to left and right children. Insertion follows standard BST rules: values smaller than the current node go to the left, larger to the right, and equal values increment the node’s count. After all values are inserted, an in-order traversal of the tree visits nodes in sorted order. For each node visited, we append the node’s value `count` times to the result vector. Duplicate handling is consistent and avoids tree imbalance issues. The algorithm runs in \(O(n \log n)\) average time for `n` insertions, with \(O(n)\) worst-case if the tree becomes skewed. Space complexity is \(O(n)\) for the tree and output vector. Edge cases include an empty input vector, a vector with all identical values, and a vector with a single element.
