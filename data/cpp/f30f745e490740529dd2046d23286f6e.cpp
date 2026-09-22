Write a C++ function that, given the root of a binary tree, returns the height of the tree. The height is defined as the number of edges on the longest downward path from the root to a leaf. An empty tree (null root) has a height of -1, a single-node tree has a height of 0, and so on. The function must handle arbitrarily deep trees and work correctly for unbalanced trees.

// The solution uses a recursive depth-first traversal. The base case occurs when the root is `nullptr`, for which we return -1 (representing that there are no edges from a non-existent node). For a non-null root, the height is 1 plus the maximum of the heights of the left and right subtrees, because the path from the root to the deepest leaf goes through one edge to a child, and then continues from that child to its deepest leaf. This recursive definition naturally handles edge cases: an empty tree returns -1, a single node returns 1 + max(-1, -1) = 0, and a tree with only a left child returns 1 + max(height(left), -1). Since every node is visited exactly once, the time complexity is O(n) where n is the number of nodes. The space complexity is O(h) due to the recursion stack, where h is the height of the tree (worst-case O(n) for a skewed tree). No special handling of duplicate values or tree mutations is needed because the function is read-only.

#include <algorithm>

// Definition for a binary tree node.
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Returns the height of the binary tree. Empty tree returns -1.
int getHeight(const Node* root) {
    if (root == nullptr) {
        return -1;
    }
    return 1 + std::max(getHeight(root->left), getHeight(root->right));
}

#include <cassert>

int main() {
    // Empty tree
    assert(getHeight(nullptr) == -1);

    // Single node
    Node* n1 = new Node(1);
    assert(getHeight(n1) == 0);

    // Two nodes (root with left child)
    Node* n2 = new Node(2);
    n1->left = n2;
    assert(getHeight(n1) == 1);

    // Root with right child only
    Node* n3 = new Node(3);
    Node* root2 = new Node(0);
    root2->right = n3;
    assert(getHeight(root2) == 1);

    // Balanced tree of height 2
    Node* n4 = new Node(4);
    Node* n5 = new Node(5);
    Node* root3 = new Node(10);
    root3->left = n4;
    root3->right = n5;
    n4->left = new Node(6);
    n4->right = new Node(7);
    assert(getHeight(root3) == 2);

    // Skewed left tree of height 3
    Node* root4 = new Node(1);
    root4->left = new Node(2);
    root4->left->left = new Node(3);
    root4->left->left->left = new Node(4);
    assert(getHeight(root4) == 3);

    // Cleanup (not strictly needed for asserts, but good practice)
    // In a full test you would delete all nodes recursively.
}
