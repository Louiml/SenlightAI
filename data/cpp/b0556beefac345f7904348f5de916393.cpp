/*
Given a pointer to the root of a non-empty binary tree where each node stores an integer and has `left` and `right` pointers (possibly `NULL`), write a C++ free function `int countLeafNodes(const BinaryTreeNode<int>* root)` that returns the total number of leaf nodes in the tree. A leaf node is defined as a node with both `left` and `right` pointers equal to `NULL`. The function must be const-correct (i.e., it must not modify the tree) and must work for any valid tree shape, including a single-node tree (where the root is a leaf) and skewed trees.
*/
#include <cstddef>

// Binary tree node structure (same as provided).
template <typename T>
class BinaryTreeNode {
public:
    T data;
    BinaryTreeNode<T>* left;
    BinaryTreeNode<T>* right;
    BinaryTreeNode(T data) : data(data), left(nullptr), right(nullptr) {}
};

// Count the number of leaf nodes in a binary tree.
// A leaf is a node with both children == nullptr.
int countLeafNodes(const BinaryTreeNode<int>* root) {
    if (root == nullptr) {
        return 0;
    }
    if (root->left == nullptr && root->right == nullptr) {
        return 1;
    }
    return countLeafNodes(root->left) + countLeafNodes(root->right);
}
#include <cassert>

// (The BinaryTreeNode definition and countLeafNodes function are included here.)

int main() {
    // Test 1: Single node (root is a leaf)
    BinaryTreeNode<int>* root1 = new BinaryTreeNode<int>(5);
    assert(countLeafNodes(root1) == 1);
    delete root1;

    // Test 2: Full binary tree with 3 nodes (root + 2 leaves)
    BinaryTreeNode<int>* root2 = new BinaryTreeNode<int>(1);
    root2->left = new BinaryTreeNode<int>(2);
    root2->right = new BinaryTreeNode<int>(3);
    assert(countLeafNodes(root2) == 2);
    delete root2->left;
    delete root2->right;
    delete root2;

    // Test 3: Skewed tree (left-only chain of 4 nodes, only last is leaf)
    BinaryTreeNode<int>* root3 = new BinaryTreeNode<int>(10);
    root3->left = new BinaryTreeNode<int>(20);
    root3->left->left = new BinaryTreeNode<int>(30);
    root3->left->left->left = new BinaryTreeNode<int>(40);
    assert(countLeafNodes(root3) == 1);
    delete root3->left->left->left;
    delete root3->left->left;
    delete root3->left;
    delete root3;

    // Test 4: More complex tree: root with left child (which has two children) and right child (leaf)
    BinaryTreeNode<int>* root4 = new BinaryTreeNode<int>(0);
    root4->left = new BinaryTreeNode<int>(1);
    root4->right = new BinaryTreeNode<int>(2);
    root4->left->left = new BinaryTreeNode<int>(3);
    root4->left->right = new BinaryTreeNode<int>(4);
    // Leaves are: node 2, node 3, node 4 => total 3
    assert(countLeafNodes(root4) == 3);
    delete root4->left->left;
    delete root4->left->right;
    delete root4->left;
    delete root4->right;
    delete root4;

    // Test 5: Tree where root has only right child (right-skewed chain of 2)
    BinaryTreeNode<int>* root5 = new BinaryTreeNode<int>(7);
    root5->right = new BinaryTreeNode<int>(8);
    assert(countLeafNodes(root5) == 1);
    delete root5->right;
    delete root5;

    return 0;
}
// The solution uses a recursive depth-first traversal. For any non-null node, we check if it is a leaf (both children are `NULL`); if so, we return 1. Otherwise, we recursively count leaves in the left and right subtrees and sum them. The base case is implicit: when a child pointer is `NULL`, we treat its contribution as 0 (no recursion call is made). The primary edge case is the root itself being a leaf (tree with exactly one node) — the function must return 1. Another edge case is a node with only one child; we only recurse on the existing child. Since each node is visited exactly once, the time complexity is O(N), where N is the number of nodes. The auxiliary space complexity is O(H) for the recursion stack, where H is the tree height (worst-case O(N) for a skewed tree, best-case O(log N) for a balanced tree). No extra data structures are needed.
