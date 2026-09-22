Implement a C++ function `int kthSmallest(TreeNode<int>* root, int k)` that returns the k-th smallest value in a binary search tree (BST). The tree nodes use the provided `TreeNode` class template, where each node contains an integer `data` and pointers `left` and `right`. You may assume `k` is always between 1 and the number of nodes in the tree. Return `-1` if the tree is empty or if `k` is invalid (e.g., `k <= 0` or `k > number of nodes`). The function must not modify the tree. Define a helper function that performs an iterative or recursive in-order traversal, counting nodes until the k-th is found. Do not use any global or static variables inside your solution; instead, pass necessary parameters by reference or use a local struct/class.
// The standard approach for finding the k-th smallest element in a BST is an in-order traversal, which visits nodes in ascending order. We can perform a recursive in-order traversal: visit left subtree, then current node, then right subtree. We maintain a `count` variable (passed by reference) that increments each time we visit a node. When `count` equals `k`, we record that node’s data and can stop further traversal (though recursion naturally continues; we can use a flag or simply return early). Edge cases: an empty tree should return `-1`; if `k <= 0` or `k > total nodes`, we should also return `-1`. To handle the case where `k` is beyond the tree size, we can either first count the total nodes or let the traversal finish with `ans` still `-1`. The recursive approach uses call stack depth proportional to tree height, so worst-case O(n) time and O(h) auxiliary space (h = height, which can be O(n) in a skewed tree). We must avoid global variables; instead, use a helper that takes `k`, `count` by reference, and `ans` by reference.
#include <iostream>

template <typename T>
class TreeNode {
public:
    T data;
    TreeNode<T>* left;
    TreeNode<T>* right;
    TreeNode(T data) : data(data), left(nullptr), right(nullptr) {}
};

// Helper function that performs in-order traversal and finds k-th smallest.
// `count` is the number of nodes visited so far (passed by reference).
// `result` stores the answer when found (passed by reference).
// Returns true if found to allow early termination.
bool inOrderKth(TreeNode<int>* root, int k, int& count, int& result) {
    if (root == nullptr) return false;
    // Traverse left subtree
    if (inOrderKth(root->left, k, count, result)) return true;
    // Visit current node
    count++;
    if (count == k) {
        result = root->data;
        return true;
    }
    // Traverse right subtree
    return inOrderKth(root->right, k, count, result);
}

// Returns the k-th smallest value in the BST, or -1 if invalid.
int kthSmallest(TreeNode<int>* root, int k) {
    if (root == nullptr || k <= 0) return -1;
    int count = 0;
    int result = -1;
    // Run in-order traversal; if found, result is set.
    inOrderKth(root, k, count, result);
    // If not found (k > number of nodes), result remains -1.
    return result;
}
#include <cassert>
#include <iostream>

// TreeNode class definition is assumed available (same as above).
// Include the solution code here or link it.

int main() {
    // Test 1: Simple BST with 3 nodes
    TreeNode<int>* root = new TreeNode<int>(2);
    root->left = new TreeNode<int>(1);
    root->right = new TreeNode<int>(3);
    assert(kthSmallest(root, 1) == 1);
    assert(kthSmallest(root, 2) == 2);
    assert(kthSmallest(root, 3) == 3);
    assert(kthSmallest(root, 4) == -1); // k > size
    assert(kthSmallest(root, 0) == -1); // invalid k

    // Test 2: Larger BST
    TreeNode<int>* root2 = new TreeNode<int>(10);
    root2->left = new TreeNode<int>(5);
    root2->right = new TreeNode<int>(20);
    root2->left->left = new TreeNode<int>(3);
    root2->left->right = new TreeNode<int>(7);
    root2->right->left = new TreeNode<int>(15);
    root2->right->right = new TreeNode<int>(25);
    assert(kthSmallest(root2, 1) == 3);
    assert(kthSmallest(root2, 2) == 5);
    assert(kthSmallest(root2, 3) == 7);
    assert(kthSmallest(root2, 4) == 10);
    assert(kthSmallest(root2, 5) == 15);
    assert(kthSmallest(root2, 6) == 20);
    assert(kthSmallest(root2, 7) == 25);
    assert(kthSmallest(root2, 8) == -1);

    // Test 3: Single node
    TreeNode<int>* root3 = new TreeNode<int>(42);
    assert(kthSmallest(root3, 1) == 42);
    assert(kthSmallest(root3, 2) == -1);

    // Test 4: Skewed left tree
    TreeNode<int>* root4 = new TreeNode<int>(10);
    root4->left = new TreeNode<int>(8);
    root4->left->left = new TreeNode<int>(6);
    root4->left->left->left = new TreeNode<int>(4);
    assert(kthSmallest(root4, 1) == 4);
    assert(kthSmallest(root4, 4) == 10);
    assert(kthSmallest(root4, 5) == -1);

    // Test 5: Empty tree
    TreeNode<int>* root5 = nullptr;
    assert(kthSmallest(root5, 1) == -1);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
