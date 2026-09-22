/*
Write a C++ function that, given a binary search tree (BST) whose nodes contain integer values, returns a pointer to the first (i.e., leftmost) node in the tree whose value equals a given target integer `k`. If no such node exists, return `nullptr`. The BST may contain duplicate values, and the function must not modify the tree. The tree is represented using a node structure with `data`, `left`, and `right` members, where `left` and `right` are `std::unique_ptr` to child nodes. The function should be named `FindFirstEqualK` and take a constant reference to a `std::unique_ptr` to the root node as its first parameter, and an integer `k` as its second parameter.
*/

#include <memory>

// Node structure for a binary search tree.
template <typename T>
struct BST_node {
    T data;
    std::unique_ptr<BST_node<T>> left;
    std::unique_ptr<BST_node<T>> right;
    explicit BST_node(T val) : data(val), left(nullptr), right(nullptr) {}
};

// Return pointer to the first (leftmost) node with data equal to k in the BST.
// Return nullptr if no such node exists.
template <typename T>
BST_node<T>* FindFirstEqualK(const std::unique_ptr<BST_node<T>>& tree, T k) {
    BST_node<T>* first_so_far = nullptr;
    BST_node<T>* curr = tree.get();
    while (curr) {
        if (curr->data < k) {
            curr = curr->right.get();
        } else if (curr->data > k) {
            curr = curr->left.get();
        } else { // curr->data == k
            // Record this node and search for an earlier occurrence in left subtree.
            first_so_far = curr;
            curr = curr->left.get();
        }
    }
    return first_so_far;
}

#include <cassert>
#include <memory>
#include "solution.h" // Assume the above solution is in this header.

int main() {
    // Build a BST: 
    //       5
    //      / \
    //     3   8
    //    / \   \
    //   2   5   9
    //      /
    //     5
    std::unique_ptr<BST_node<int>> root = std::make_unique<BST_node<int>>(5);
    root->left = std::make_unique<BST_node<int>>(3);
    root->left->left = std::make_unique<BST_node<int>>(2);
    root->left->right = std::make_unique<BST_node<int>>(5);
    root->left->right->left = std::make_unique<BST_node<int>>(5);
    root->right = std::make_unique<BST_node<int>>(8);
    root->right->right = std::make_unique<BST_node<int>>(9);

    // Find the first occurrence of 5: should be leftmost 5, which is root->left->right->left.
    BST_node<int>* result = FindFirstEqualK(root, 5);
    assert(result != nullptr);
    assert(result->data == 5);
    assert(result == root->left->right->left.get());

    // Find first occurrence of 2: should be root->left->left.
    result = FindFirstEqualK(root, 2);
    assert(result != nullptr);
    assert(result == root->left->left.get());

    // Find first occurrence of 8: should be root->right.
    result = FindFirstEqualK(root, 8);
    assert(result != nullptr);
    assert(result == root->right.get());

    // Find first occurrence of 9: should be root->right->right.
    result = FindFirstEqualK(root, 9);
    assert(result != nullptr);
    assert(result == root->right->right.get());

    // Find first occurrence of 7: not present, should be nullptr.
    result = FindFirstEqualK(root, 7);
    assert(result == nullptr);

    // Empty tree: should return nullptr.
    std::unique_ptr<BST_node<int>> empty_tree;
    result = FindFirstEqualK(empty_tree, 1);
    assert(result == nullptr);

    // Single-node tree with value 4.
    auto single = std::make_unique<BST_node<int>>(4);
    result = FindFirstEqualK(single, 4);
    assert(result == single.get());
    result = FindFirstEqualK(single, 5);
    assert(result == nullptr);

    // Duplicate values only on the right: 3 -> right child also 3.
    auto dup_right = std::make_unique<BST_node<int>>(3);
    dup_right->right = std::make_unique<BST_node<int>>(3);
    result = FindFirstEqualK(dup_right, 3);
    assert(result == dup_right.get()); // The first (leftmost) is the root, not the right child.

    // Duplicate values in a chain: 2 -> left 2 -> left 2.
    auto chain = std::make_unique<BST_node<int>>(2);
    chain->left = std::make_unique<BST_node<int>>(2);
    chain->left->left = std::make_unique<BST_node<int>>(2);
    result = FindFirstEqualK(chain, 2);
    assert(result == chain->left->left.get()); // Deepest left is first.

    return 0;
}

// The approach leverages the BST property: for any node, all keys in the left subtree are less than or equal to the node's key, and all keys in the right subtree are greater than or equal. To find the first (leftmost) occurrence of `k`, traverse from the root downward. Maintain a pointer `first_so_far` that records the most recently seen node with value equal to `k`. At each step, compare the current node's data with `k`:
// - If `curr->data < k`, the target must be in the right subtree, so move `curr` to `curr->right`.
// - If `curr->data > k`, the target must be in the left subtree, so move `curr` to `curr->left`.
// - If `curr->data == k`, record this node as `first_so_far` and continue searching in the left subtree, because there could be an earlier (smaller-keyed) occurrence of `k` in that left subtree.
// The loop continues until `curr` becomes `nullptr`. At the end, return `first_so_far`. This correctly returns the leftmost occurrence because we always search leftward after finding a match, and we never move right after finding a match (since any duplicate to the right would be later, not first). Edge cases: if the tree is empty, the function returns `nullptr` immediately. If `k` is not in the tree, `first_so_far` remains `nullptr`. If duplicates exist only on the right, the first match is correct because we stop at the first equal node and only go left, not right. Time complexity is O(h) where h is the tree height, worst-case O(n) for a skewed tree, and best-case O(log n) for a balanced tree. Space complexity is O(1) auxiliary, since only a few pointers are used.
