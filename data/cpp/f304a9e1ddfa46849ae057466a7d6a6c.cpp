Given a Binary Search Tree (BST) implemented with integer values, write a C++ function that takes the root node of the tree and returns the value of the lowest common ancestor (LCA) of two given integer values `a` and `b` using an iterative traversal (not recursion). The function should assume that both `a` and `b` exist in the BST. If the root is `nullptr` or either value is not present, return `-1`. The function should work efficiently using the BST property (left child < parent < right child) and must not use any additional data structures like stacks or queues.

// The solution leverages the BST property: for any node, all values in the left subtree are smaller, and all values in the right subtree are larger. The lowest common ancestor of `a` and `b` is the first node whose value lies strictly between `a` and `b` (inclusive, if one of them equals the node's value). We traverse iteratively from the root:  
// - If the current node's value is greater than both `a` and `b`, move to the left child.  
// - If the current node's value is less than both `a` and `b`, move to the right child.  
// - Otherwise, the current node is the LCA because splitting occurs here (or one of the targets is the node itself).  
// Edge cases: If the root is null, return -1. If either value is not in the tree, the traversal will eventually reach a null pointer while moving left/right; in that case return -1. No recursion or extra memory is used, so the time complexity is O(h) where h is the height of the tree (worst-case O(n) for skewed trees) and space complexity is O(1).

#include <cstddef>  // for nullptr

struct BSTNode {
    int value;
    BSTNode* left;
    BSTNode* right;
};

// Find the lowest common ancestor of two values in a BST.
// Returns -1 if root is null or either value is not present.
int lowestCommonAncestor(BSTNode* root, int a, int b) {
    if (root == nullptr) return -1;

    BSTNode* current = root;
    while (current != nullptr) {
        if (current->value > a && current->value > b) {
            current = current->left;
        } else if (current->value < a && current->value < b) {
            current = current->right;
        } else {
            // Current is the LCA if both values exist.
            // Verify both are present in the subtree rooted at current.
            // Since we can't search recursively, we do a simple existence check.
            // If either is missing, return -1.
            bool foundA = false, foundB = false;
            BSTNode* temp = current;
            while (temp) {
                if (temp->value == a) { foundA = true; break; }
                if (a < temp->value) temp = temp->left;
                else temp = temp->right;
            }
            temp = current;
            while (temp) {
                if (temp->value == b) { foundB = true; break; }
                if (b < temp->value) temp = temp->left;
                else temp = temp->right;
            }
            if (foundA && foundB) return current->value;
            else return -1;
        }
    }
    return -1;
}

#include <cassert>

int main() {
    // Build a BST:       6
    //                  /   \
    //                 4     8
    //                / \   / \
    //               3   5 7   9
    BSTNode* n6 = new BSTNode{6, nullptr, nullptr};
    BSTNode* n4 = new BSTNode{4, nullptr, nullptr};
    BSTNode* n8 = new BSTNode{8, nullptr, nullptr};
    BSTNode* n3 = new BSTNode{3, nullptr, nullptr};
    BSTNode* n5 = new BSTNode{5, nullptr, nullptr};
    BSTNode* n7 = new BSTNode{7, nullptr, nullptr};
    BSTNode* n9 = new BSTNode{9, nullptr, nullptr};
    n6->left = n4; n6->right = n8;
    n4->left = n3; n4->right = n5;
    n8->left = n7; n8->right = n9;

    assert(lowestCommonAncestor(n6, 3, 5) == 4);
    assert(lowestCommonAncestor(n6, 3, 9) == 6);
    assert(lowestCommonAncestor(n6, 6, 8) == 6);  // LCA can be one of the nodes
    assert(lowestCommonAncestor(n6, 7, 9) == 8);
    assert(lowestCommonAncestor(n6, 1, 2) == -1);  // value not present
    assert(lowestCommonAncestor(nullptr, 1, 2) == -1);
    assert(lowestCommonAncestor(n6, 3, 4) == 4);   // one is ancestor of the other

    // Clean up (not strictly necessary for test, but good practice)
    delete n3; delete n5; delete n4; delete n7; delete n9; delete n8; delete n6;
    return 0;
}
