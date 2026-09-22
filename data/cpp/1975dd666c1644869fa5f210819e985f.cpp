Write a C++ function `mergeBSTs(Node* root1, Node* root2)` that takes two binary search trees (BSTs) whose nodes contain integer values, performs an in-order traversal of each tree to obtain sorted lists, merges those two sorted lists into a single sorted vector of integers, and returns that vector. The function must handle empty trees gracefully and preserve all duplicate values. The Node structure is provided as a standard binary tree node with `int data`, `Node* left`, and `Node* right` pointers. Assume that each BST is valid (i.e., for every node, all values in its left subtree are smaller than its data, and all values in its right subtree are larger). The function should use only O(H1 + H2) auxiliary space for the recursive traversal (where H1 and H2 are the heights of the trees) plus O(N1 + N2) space for the output vectors, and the overall time complexity must be O(N1 + N2) where N1 and N2 are the numbers of nodes in each tree. The final merged vector must be in non-decreasing order.
// The solution performs two in-order traversals—one for each BST—to collect the node values into two separate vectors. Since an in-order traversal of a BST visits nodes in ascending order, each vector is already sorted. Then, a standard two-pointer merge is used to combine the two sorted vectors into a single sorted vector. This merge algorithm compares the current elements from each vector, appends the smaller one to the result, and advances the corresponding pointer. If one vector is exhausted, the remaining elements of the other vector are appended directly. Edge cases include one or both trees being null (in which case the traversal produces an empty vector and the merge returns the other vector's contents, or an empty result if both are null). Duplicate values across or within the trees are preserved by the merge because the comparison uses `<` (or `<=` equivalently) and appends from one side when equal. The time complexity is O(N1 + N2) for the traversals plus O(N1 + N2) for the merge, totaling O(N1 + N2). The auxiliary space is O(H1 + H2) for recursion stacks plus O(N1 + N2) for the returned vector and the intermediate vectors (though the intermediate vectors could be optimized, the provided structure uses them, so space is O(N1 + N2 + H1 + H2)).
#include <vector>

struct Node {
    int data;
    Node *left;
    Node *right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Helper to perform in-order traversal and collect node values into a vector.
void collectInorder(Node* root, std::vector<int>& values) {
    if (root == nullptr) {
        return;
    }
    collectInorder(root->left, values);
    values.push_back(root->data);
    collectInorder(root->right, values);
}

// Merge two sorted vectors into one sorted vector.
std::vector<int> mergeSortedVectors(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> result;
    result.reserve(a.size() + b.size());
    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        if (a[i] < b[j]) {
            result.push_back(a[i]);
            ++i;
        } else {
            result.push_back(b[j]);
            ++j;
        }
    }
    while (i < a.size()) {
        result.push_back(a[i]);
        ++i;
    }
    while (j < b.size()) {
        result.push_back(b[j]);
        ++j;
    }
    return result;
}

// Public function that merges two BSTs and returns a sorted vector of all values.
std::vector<int> mergeBSTs(Node* root1, Node* root2) {
    std::vector<int> values1, values2;
    collectInorder(root1, values1);
    collectInorder(root2, values2);
    return mergeSortedVectors(values1, values2);
}
#include <cassert>
#include <vector>
#include "solution.h"  // assuming the above code is in solution.h

// Helper to build a BST manually for testing.
Node* insert(Node* root, int value) {
    if (root == nullptr) return new Node(value);
    if (value < root->data) {
        root->left = insert(root->left, value);
    } else {
        root->right = insert(root->right, value);
    }
    return root;
}

void deleteTree(Node* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    // Test 1: Both trees non-empty, no duplicates
    Node* t1 = nullptr;
    t1 = insert(t1, 5);
    t1 = insert(t1, 3);
    t1 = insert(t1, 8);
    t1 = insert(t1, 1);

    Node* t2 = nullptr;
    t2 = insert(t2, 4);
    t2 = insert(t2, 7);
    t2 = insert(t2, 9);

    std::vector<int> result = mergeBSTs(t1, t2);
    std::vector<int> expected = {1, 3, 4, 5, 7, 8, 9};
    assert(result == expected);
    deleteTree(t1);
    deleteTree(t2);

    // Test 2: One tree is empty
    Node* t3 = nullptr;
    Node* t4 = nullptr;
    t4 = insert(t4, 10);
    t4 = insert(t4, -2);

    result = mergeBSTs(t3, t4);
    expected = {-2, 10};
    assert(result == expected);
    deleteTree(t4);

    // Test 3: Both trees empty
    result = mergeBSTs(nullptr, nullptr);
    assert(result.empty());

    // Test 4: Duplicate values across trees
    Node* t5 = nullptr;
    t5 = insert(t5, 2);
    t5 = insert(t5, 5);
    Node* t6 = nullptr;
    t6 = insert(t6, 2);
    t6 = insert(t6, 5);

    result = mergeBSTs(t5, t6);
    expected = {2, 2, 5, 5};
    assert(result == expected);
    deleteTree(t5);
    deleteTree(t6);

    // Test 5: Single-node trees
    Node* t7 = new Node(100);
    Node* t8 = new Node(50);
    result = mergeBSTs(t7, t8);
    expected = {50, 100};
    assert(result == expected);
    deleteTree(t7);
    deleteTree(t8);

    return 0;
}
