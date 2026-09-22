// Write a C++ function `bool containsValue(const BinaryTreeNode<int>* root, int target)` that determines whether a given integer `target` exists anywhere in a binary tree. The binary tree is represented by a node structure `BinaryTreeNode<int>` with integer data and left/right child pointers that may be `nullptr`. The function must handle an empty tree, a target found at the root, a target found deep in the left or right subtree, and a target that does not exist. The function should be `const`-correct (take a pointer to const node) and must not modify the tree. Implement the function recursively, short-circuiting to return `true` as soon as the target is discovered, without exploring the rest of the tree unnecessarily. Provide a complete solution with proper recursion, base cases, and considerations for deep recursion on skew trees.

// The solution uses a straightforward recursive depth-first search. The algorithm is: if the current node is `nullptr`, return `false` (base case—target not found in this branch). If the current node's data equals `target`, return `true` immediately. Otherwise, recursively search the left subtree; if that returns `true`, propagate `true` up without checking the right subtree (short-circuit). If the left subtree returns `false`, recursively search the right subtree and return its result. This ensures every node is visited at most once, and the search terminates early once the target is found. Edge cases: empty tree (`root == nullptr`) returns `false`; target equals root data returns `true` without recursion; target missing from left but present in right returns correctly; duplicates (multiple nodes with same value) return `true` on first encounter. Time complexity is O(n) in the worst case (target absent or found at the last visited leaf), where n is the number of nodes; best case is O(1) if target is at the root. Space complexity is O(h) due to the recursion call stack, where h is the tree height—worst case O(n) for a skewed tree, best case O(log n) for a balanced tree.

#include <cstddef>

// Node structure for the binary tree (assumed provided elsewhere, but included for completeness)
template <typename T>
struct BinaryTreeNode {
    T data;
    BinaryTreeNode<T>* left;
    BinaryTreeNode<T>* right;
    BinaryTreeNode(T val) : data(val), left(nullptr), right(nullptr) {}
};

// Returns true if 'target' exists anywhere in the binary tree rooted at 'root'.
// Uses const-correct pointer to avoid modifying the tree.
bool containsValue(const BinaryTreeNode<int>* root, int target) {
    // Base case: empty subtree — target not found.
    if (root == nullptr) {
        return false;
    }

    // If current node holds the target, success.
    if (root->data == target) {
        return true;
    }

    // Search left subtree first; short-circuit if found.
    if (containsValue(root->left, target)) {
        return true;
    }

    // Otherwise, search right subtree and return its result.
    return containsValue(root->right, target);
}

#include <cassert>

// Helper to create a new node
BinaryTreeNode<int>* makeNode(int val) {
    return new BinaryTreeNode<int>(val);
}

// Helper to delete the whole tree (not required for tests but good for cleanup)
void deleteTree(BinaryTreeNode<int>* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

int main() {
    // Test 1: Empty tree
    BinaryTreeNode<int>* empty = nullptr;
    assert(containsValue(empty, 5) == false);

    // Test 2: Single node, target present
    BinaryTreeNode<int>* single = makeNode(10);
    assert(containsValue(single, 10) == true);
    assert(containsValue(single, 99) == false);

    // Build a simple tree:
    //        1
    //       / \
    //      2   3
    //     / \   \
    //    4   5   6
    BinaryTreeNode<int>* root = makeNode(1);
    root->left = makeNode(2);
    root->right = makeNode(3);
    root->left->left = makeNode(4);
    root->left->right = makeNode(5);
    root->right->right = makeNode(6);

    // Target at root
    assert(containsValue(root, 1) == true);

    // Target in left subtree
    assert(containsValue(root, 4) == true);
    assert(containsValue(root, 5) == true);

    // Target in right subtree
    assert(containsValue(root, 6) == true);

    // Target not present anywhere
    assert(containsValue(root, 7) == false);

    // Target present multiple times (duplicate value)
    root->right->right->right = makeNode(4); // add another 4
    assert(containsValue(root, 4) == true);

    // Skewed tree (chain) to test deep recursion
    BinaryTreeNode<int>* chain = makeNode(1);
    chain->right = makeNode(2);
    chain->right->right = makeNode(3);
    chain->right->right->right = makeNode(4);
    assert(containsValue(chain, 4) == true);
    assert(containsValue(chain, 5) == false);

    // Clean up dynamically allocated memory (for completeness)
    deleteTree(root);
    deleteTree(single);
    deleteTree(chain);
    // empty has no nodes to delete

    return 0;
}
