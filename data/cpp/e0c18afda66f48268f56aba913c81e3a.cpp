// You are given the root of a binary search tree (BST) and an integer key. Write a C++ function `int findFloor(TreeNode* root, int key)` that returns the largest value in the BST that is less than or equal to the given `key`. If no such value exists (i.e., the `key` is smaller than every node’s value), return `-1`. The BST may contain duplicate values, and the tree can be empty (`root == nullptr`). The function must not modify the tree.

The solution uses the BST property: all values in the left subtree are smaller than the root, and all values in the right subtree are larger. Start with `floor = -1` and a pointer `current = root`. While `current` is not null, compare `current->data` with `key`:  
- If equal, immediately return `key` (the floor is exactly the key).  
- If `key > current->data`, this node is a valid candidate for the floor, so update `floor = current->data` and move to the right child to look for a larger candidate that is still ≤ key.  
- If `key < current->data`, the current node is too large, so move to the left child (do not update floor).  
At the end, return `floor`. This works because every time we move right, we have found a closer floor, and when we move left we discard all larger values. Edge cases: empty tree → `-1`; all nodes > key → `-1`; key present → return key; duplicates do not affect logic. Time complexity is O(h), where h is the tree height (O(log n) for balanced BST, O(n) worst-case for skewed). Space complexity is O(1) auxiliary, as only a few pointers are used.

#include <cstddef>  // for nullptr

// Definition for a binary tree node.
struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Returns the largest value in the BST <= key, or -1 if none exists.
int findFloor(TreeNode* root, int key) {
    int floor = -1;
    TreeNode* current = root;

    while (current != nullptr) {
        if (current->data == key) {
            return key;
        }
        if (key > current->data) {
            floor = current->data;      // this node is a valid floor candidate
            current = current->right;   // look for a larger floor
        } else {
            current = current->left;    // this node is too large, go left
        }
    }
    return floor;
}

#include <cassert>

int main() {
    // Build test tree:        8
    //                       /   \
    //                      4    12
    //                     / \   / \
    //                    2   6 10  14
    TreeNode* root = new TreeNode(8);
    root->left = new TreeNode(4);
    root->right = new TreeNode(12);
    root->left->left = new TreeNode(2);
    root->left->right = new TreeNode(6);
    root->right->left = new TreeNode(10);
    root->right->right = new TreeNode(14);

    // Basic floor tests
    assert(findFloor(root, 5) == 4);
    assert(findFloor(root, 8) == 8);
    assert(findFloor(root, 9) == 8);
    assert(findFloor(root, 15) == 14);
    assert(findFloor(root, 1) == -1);
    assert(findFloor(root, 0) == -1);

    // Duplicate values: attach a duplicate 6 as left child of 6
    TreeNode* dup = new TreeNode(6);
    root->left->right->left = dup;
    assert(findFloor(root, 6) == 6);
    assert(findFloor(root, 7) == 6);

    // Empty tree
    assert(findFloor(nullptr, 5) == -1);

    // Single-node tree
    TreeNode* single = new TreeNode(3);
    assert(findFloor(single, 3) == 3);
    assert(findFloor(single, 4) == 3);
    assert(findFloor(single, 2) == -1);

    // Clean up (simplified; production code would use a destructor)
    // Not required for assert checks, but omitted for brevity.

    return 0;
}
