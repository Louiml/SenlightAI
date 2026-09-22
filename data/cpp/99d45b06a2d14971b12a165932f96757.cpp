// Write a C++ function `Node* lowestCommonAncestor(Node* root, Node* p, Node* q)` that, given the root of a valid binary search tree (BST) where each node contains an integer `data` and pointers to left and right children, returns the pointer to the lowest common ancestor (LCA) of two given nodes `p` and `q`. The LCA is defined as the deepest node that is an ancestor of both `p` and `q` (a node can be an ancestor of itself). The function must work correctly for any two distinct nodes present in the tree, including cases where one node is an ancestor of the other. The tree is assumed to be non-empty and contain unique integer values. You must implement the function iteratively (not recursively) using the BST property that for any node, all values in its left subtree are smaller and all in its right subtree are larger. The function should handle the edge case where the two nodes are identical (return that node). Assume the nodes `p` and `q` are guaranteed to exist in the tree. The function signature is fixed, and the solution must be provided as a self-contained free function with appropriate includes and comments.

The core idea leverages the BST ordering property: for a given node `root`, if both `p->data` and `q->data` are less than `root->data`, then both nodes must lie in the left subtree, so the LCA is in the left subtree. Similarly, if both are greater than `root->data`, the LCA is in the right subtree. If the node’s value is between the two values (or equal to one of them), then this node is the LCA because it is the first point where the paths to `p` and `q` diverge. The iterative algorithm starts at the root and repeatedly moves left or right accordingly until it finds the split point. Edge cases: if `p` and `q` are the same node, the condition `root->data < p->data && root->data < q->data` and `root->data > p->data && root->data > q->data` will always be false (since equal values are neither less nor greater), so we immediately return that node. If one node is an ancestor of the other (e.g., `p` is the ancestor), then at the ancestor node, the condition for moving will be false because `root->data` equals one of the values, so we return that ancestor. The algorithm assumes a valid BST with unique values; if the tree is empty, the function should handle it by returning `nullptr` (though the problem guarantees non-empty). Time complexity is O(h) where h is the height of the tree (O(log n) for balanced BST, O(n) for skewed). Space complexity is O(1) since no recursion or additional data structures are used.

#include <iostream>

// Node structure for a binary search tree
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Iterative function to find the lowest common ancestor of two nodes in a BST
Node* lowestCommonAncestor(Node* root, Node* p, Node* q) {
    // Traverse the tree starting from root
    while (root != nullptr) {
        // If both nodes are in the right subtree, move right
        if (root->data < p->data && root->data < q->data) {
            root = root->right;
        }
        // If both nodes are in the left subtree, move left
        else if (root->data > p->data && root->data > q->data) {
            root = root->left;
        }
        // Otherwise, this node is the LCA
        else {
            return root;
        }
    }
    // This will never be reached if both p and q exist in the tree
    return nullptr;
}

#include <cassert>

// Helper to create a small test BST manually
// Tree:
//        8
//      /   \
//     3     10
//    / \      \
//   1   6      14
//      / \    /
//     4   7  13
Node* createTestTree() {
    Node* root = new Node(8);
    root->left = new Node(3);
    root->right = new Node(10);
    root->left->left = new Node(1);
    root->left->right = new Node(6);
    root->right->right = new Node(14);
    root->left->right->left = new Node(4);
    root->left->right->right = new Node(7);
    root->right->right->left = new Node(13);
    return root;
}

int main() {
    Node* tree = createTestTree();
    Node* n1 = tree->left->left;          // 1
    Node* n2 = tree->left->right;         // 6
    Node* n3 = tree->right;               // 10
    Node* n4 = tree->left;                // 3
    Node* n5 = tree->right->right->left;  // 13
    Node* n6 = tree->left->right->right;  // 7
    Node* rootNode = tree;                // 8

    // LCA of 1 and 6 is 3
    assert(lowestCommonAncestor(tree, n1, n2) == n4);
    // LCA of 6 and 13 is 8
    assert(lowestCommonAncestor(tree, n2, n5) == rootNode);
    // LCA of 10 and 13 is 10 (one is ancestor of other)
    assert(lowestCommonAncestor(tree, n3, n5) == n3);
    // LCA of 4 and 7 is 6
    assert(lowestCommonAncestor(tree, tree->left->right->left, n6) == n2);
    // LCA of 3 and 10 is 8
    assert(lowestCommonAncestor(tree, n4, n3) == rootNode);
    // LCA of identical nodes (6 and 6) returns 6
    assert(lowestCommonAncestor(tree, n2, n2) == n2);
    // LCA of 1 and 13 is 8
    assert(lowestCommonAncestor(tree, n1, n5) == rootNode);
    // LCA of leftmost and rightmost leaves (1 and 13) is root 8
    assert(lowestCommonAncestor(tree, n1, n5) == rootNode);
    // LCA of 7 and 13 is 8
    assert(lowestCommonAncestor(tree, n6, n5) == rootNode);
    // LCA of 1 and 4 is 3
    assert(lowestCommonAncestor(tree, n1, tree->left->right->left) == n4);

    // Cleanup (not strictly necessary for test but good practice)
    // In a real program, you'd delete nodes, but for test brevity, we skip.

    return 0;
}
