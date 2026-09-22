// Write a C++ function `countLeafNodes(const std::vector<int>& values)` that takes a vector of integers and returns the number of leaf nodes in the binary search tree (BST) that would be constructed by inserting those values in the given order, ignoring duplicate values (insert each distinct value only once). The BST insertion rule is: if a value is less than the current node's value, go left; otherwise go right. A leaf node is a node with no children. If the input vector is empty, return 0. The function must be standalone (no external globals) and must not modify the input vector.
#include <cassert>
#include <vector>

// Assume the solution function is declared above.
int main() {
    // Empty tree -> 0 leaves
    assert(countLeafNodes({}) == 0);

    // Single distinct value -> 1 leaf
    assert(countLeafNodes({5}) == 1);

    // All duplicates -> 1 leaf
    assert(countLeafNodes({3, 3, 3}) == 1);

    // Sorted ascending: 1-2-3 -> only 3 is a leaf
    assert(countLeafNodes({1, 2, 3}) == 1);

    // Sorted descending: 3-2-1 -> only 1 is a leaf
    assert(countLeafNodes({3, 2, 1}) == 1);

    // Balanced-ish: 5,3,7,2,4,6,8 -> leaves: 2,4,6,8 => 4 leaves
    assert(countLeafNodes({5, 3, 7, 2, 4, 6, 8}) == 4);

    // Duplicates ignored: 10,5,15,5,3,7,20 -> tree has 10,5,15,3,7,20 -> leaves: 3,7,20 => 3
    assert(countLeafNodes({10, 5, 15, 5, 3, 7, 20}) == 3);

    // Only root with left child only: 10,5 -> leaf is 5
    assert(countLeafNodes({10, 5}) == 1);

    // Only root with right child only: 10,20 -> leaf is 20
    assert(countLeafNodes({10, 20}) == 1);
}
#include <vector>
#include <set>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    explicit TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper to insert into BST, returns true if inserted (new node), false if value existed.
static TreeNode* insertRecursive(TreeNode* root, int value) {
    if (root == nullptr) {
        return new TreeNode(value);
    }
    if (value < root->val) {
        root->left = insertRecursive(root->left, value);
    } else if (value > root->val) {
        root->right = insertRecursive(root->right, value);
    }
    // if equal, do nothing (duplicate)
    return root;
}

// Helper to count leaf nodes recursively.
static int countLeavesRecursive(const TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    if (root->left == nullptr && root->right == nullptr) {
        return 1;
    }
    return countLeavesRecursive(root->left) + countLeavesRecursive(root->right);
}

// Build BST from vector (ignoring duplicates) and count its leaf nodes.
int countLeafNodes(const std::vector<int>& values) {
    TreeNode* root = nullptr;
    std::set<int> seen;
    for (int v : values) {
        if (seen.find(v) == seen.end()) {
            root = insertRecursive(root, v);
            seen.insert(v);
        }
    }
    int result = countLeavesRecursive(root);
    // Clean up memory (not strictly required for the task but good practice)
    // We'll skip deletion for brevity in a competitive context.
    return result;
}
// The approach is to build a BST by iterating through the input vector. For each value, if it has not been inserted before (tracked via a set or by checking the tree), insert it according to standard BST rules: recursively compare with the root, moving left if smaller, right otherwise. Duplicates are skipped entirely (only the first occurrence matters). After construction, count the leaf nodes by traversing the tree recursively: a node is a leaf if both its left and right pointers are null; otherwise, recurse into its children. Edge cases include an empty vector (return 0), a single distinct value (returns 1), and all duplicate values (still 1 leaf). Time complexity is O(n * h) for insertion where h is tree height (worst-case O(n^2) for sorted input, average O(n log n)), and O(n) for counting leaves. Space complexity is O(n) for storing the tree plus recursion stack.
