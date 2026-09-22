Write a C++ function named `findCommonAncestor` that takes two integer keys and a binary search tree (BST) containing unique integer keys, and returns the key of their lowest common ancestor (LCA). The BST is represented by a `TreeNode` structure with `int val`, `TreeNode* left`, and `TreeNode* right` members. The function must handle the case where either key is not present in the tree by returning `-1` (assuming keys are non-negative). The function signature should be `int findCommonAncestor(TreeNode* root, int key1, int key2)`. You may assume the tree is complete and correctly ordered, and that keys are unique.
// The solution leverages the BST property: for any node, all keys in the left subtree are smaller, and all keys in the right subtree are larger. To find the LCA of two keys, traverse the tree starting at the root. At each node, if both keys are less than the current node's value, move to the left child; if both are greater, move to the right child. The first node where the keys diverge (one is less and the other greater, or one equals the node's value) is the LCA candidate. However, we must confirm that both keys actually exist in the tree; otherwise, the LCA is invalid. Therefore, after finding the candidate, we perform a separate search for each key to verify existence. Alternatively, we can check existence during traversal by tracking whether we encounter each key along the path. The time complexity of the traversal alone is O(h), where h is the tree height (O(log n) for balanced, O(n) worst-case). Verification adds another O(h) per key, so total O(h) plus O(h) = O(h). Space complexity is O(1) for iterative traversal, or O(h) for recursive. Edge cases include: one key being ancestor of the other (LCA is that key), keys not present (return -1), and root being null (return -1). We must also handle equal keys? The problem states unique keys, so no duplicates. For safety, handle `key1 == key2` by returning that key if it exists, else -1.
#include <algorithm>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper function to check if a key exists in the BST.
bool contains(TreeNode* root, int key) {
    while (root) {
        if (key == root->val) return true;
        else if (key < root->val) root = root->left;
        else root = root->right;
    }
    return false;
}

// Return the lowest common ancestor key of two integers in a BST, or -1 if not found.
int findCommonAncestor(TreeNode* root, int key1, int key2) {
    if (!root) return -1;
    
    // Verify both keys exist in the tree.
    if (!contains(root, key1) || !contains(root, key2)) return -1;
    
    TreeNode* current = root;
    while (current) {
        // Both keys are in the left subtree.
        if (key1 < current->val && key2 < current->val) {
            current = current->left;
        }
        // Both keys are in the right subtree.
        else if (key1 > current->val && key2 > current->val) {
            current = current->right;
        }
        // Keys diverge or one equals current: this is the LCA.
        else {
            return current->val;
        }
    }
    // Should never reach here if keys exist.
    return -1;
}
#include <cassert>

int main() {
    // Build a BST:       8
    //                  /   \
    //                 3    10
    //                / \     \
    //               1   6     14
    //                  / \    /
    //                 4   7  13
    TreeNode* root = new TreeNode(8);
    root->left = new TreeNode(3);
    root->right = new TreeNode(10);
    root->left->left = new TreeNode(1);
    root->left->right = new TreeNode(6);
    root->left->right->left = new TreeNode(4);
    root->left->right->right = new TreeNode(7);
    root->right->right = new TreeNode(14);
    root->right->right->left = new TreeNode(13);

    // Standard cases
    assert(findCommonAncestor(root, 1, 7) == 3);
    assert(findCommonAncestor(root, 4, 7) == 6);
    assert(findCommonAncestor(root, 1, 13) == 8);
    assert(findCommonAncestor(root, 10, 14) == 10); // 10 is ancestor of 14
    assert(findCommonAncestor(root, 8, 8) == 8);    // same key

    // Missing key cases
    assert(findCommonAncestor(root, 1, 100) == -1);
    assert(findCommonAncestor(root, 100, 1) == -1);
    assert(findCommonAncestor(root, 100, 200) == -1);

    // Edge cases: empty tree
    assert(findCommonAncestor(nullptr, 1, 2) == -1);

    // Left-skewed tree: 5 -> 4 -> 3 -> 2 -> 1
    TreeNode* leftSkew = new TreeNode(5);
    leftSkew->left = new TreeNode(4);
    leftSkew->left->left = new TreeNode(3);
    leftSkew->left->left->left = new TreeNode(2);
    leftSkew->left->left->left->left = new TreeNode(1);
    assert(findCommonAncestor(leftSkew, 1, 4) == 4);
    assert(findCommonAncestor(leftSkew, 2, 3) == 3);

    // Clean up (not necessary for asserts but good practice in real code)
    // In a test harness, we'd delete nodes; omitted for brevity.
}
