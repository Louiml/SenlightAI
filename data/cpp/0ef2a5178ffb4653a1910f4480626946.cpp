// Write a C++ function that, given a pointer to the root node of a binary search tree (BST) and an integer target value, returns `1` if the target exists in the tree, `-1` if the tree is empty, and `0` if the target is not found in a non-empty tree. The function must use recursive search, leveraging the BST property: for each node, all values in the left subtree are smaller, and all values in the right subtree are larger. The tree is represented by a `struct node` containing an integer `data`, and pointers `left` and `right`. You may assume the tree is a valid BST with unique values. The function should be defined as `int bstSearch(struct node* root, int target)`, taking the root pointer by value (not by pointer-to-pointer), and should handle the empty tree case.

The solution recursively traverses the BST. Starting at the root, it first checks if the root is `nullptr` — if so, the tree is empty, and we return `-1` (special case). Then, if the current node's data equals the target, we return `1`. If the target is less than the current node's data, we recurse into the left subtree; if greater, into the right subtree. The recursion unwinds by returning the result of the subtree search. An important edge case is distinguishing between "empty tree" (return `-1`) and "target not found in a non-empty tree" (return `0`). For example, a leaf node with no children will have a nullptr subtree, but since the recursion is only called when a comparison is made, that nullptr indicates "not found" in the current path, so we return `0` from that recursive call (not `-1`). The base case at the top of the function handles only the root being null initially; all deeper nulls are handled by returning `0` from the recursive calls. Time complexity is O(h), where h is the tree height (O(log n) for a balanced tree, O(n) for a skewed tree). Space complexity is O(h) for the recursion stack.

struct node {
    int data;
    struct node* left;
    struct node* right;
};

/**
 * Recursively search for a target value in a binary search tree.
 * @param root Pointer to the root node of the BST.
 * @param target The integer value to search for.
 * @return 1 if found, 0 if not found in non-empty tree, -1 if tree is empty.
 */
int bstSearch(const struct node* root, int target) {
    if (root == nullptr) {
        return -1;  // Empty tree case
    }

    if (root->data == target) {
        return 1;
    }

    if (target < root->data) {
        if (root->left == nullptr) {
            return 0;  // Not found in non-empty subtree
        }
        return bstSearch(root->left, target);
    } else {
        if (root->right == nullptr) {
            return 0;  // Not found in non-empty subtree
        }
        return bstSearch(root->right, target);
    }
}

#include <cassert>

int main() {
    // Build a simple BST:       10
    //                         /    \
    //                        5      20
    //                       / \     / \
    //                      3   7   15  30
    struct node n3 = {3, nullptr, nullptr};
    struct node n7 = {7, nullptr, nullptr};
    struct node n5 = {5, &n3, &n7};
    struct node n15 = {15, nullptr, nullptr};
    struct node n30 = {30, nullptr, nullptr};
    struct node n20 = {20, &n15, &n30};
    struct node n10 = {10, &n5, &n20};

    // Found cases
    assert(bstSearch(&n10, 10) == 1);
    assert(bstSearch(&n10, 5) == 1);
    assert(bstSearch(&n10, 3) == 1);
    assert(bstSearch(&n10, 7) == 1);
    assert(bstSearch(&n10, 15) == 1);
    assert(bstSearch(&n10, 20) == 1);
    assert(bstSearch(&n10, 30) == 1);

    // Not found in non-empty tree
    assert(bstSearch(&n10, 1) == 0);
    assert(bstSearch(&n10, 8) == 0);
    assert(bstSearch(&n10, 25) == 0);
    assert(bstSearch(&n10, 100) == 0);

    // Empty tree
    struct node* empty = nullptr;
    assert(bstSearch(empty, 42) == -1);

    // Single-node tree
    struct node singleton = {7, nullptr, nullptr};
    assert(bstSearch(&singleton, 7) == 1);
    assert(bstSearch(&singleton, 8) == 0);

    return 0;
}
