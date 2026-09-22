// Write a standalone C++ function `bool isBSTAndBalanced(const std::vector<int>& values)` that takes a vector of integers representing the order in which nodes are inserted into a binary search tree, builds the BST using these insertions, and returns `true` only if the resulting tree is both a valid BST (all left descendants < node, all right descendants > node, no duplicates) and height-balanced (for every node, the absolute difference between the heights of its left and right subtrees is at most 1). The function must handle empty input (return `true` for an empty tree, which is trivially valid and balanced) and must work with negative numbers and any insertion order. You may define helper structures (e.g., a `Node` struct) inside the function or as file-local types, but the solution must be self-contained.

The solution builds a binary search tree by inserting values sequentially according to the standard BST insertion rule: if the value is smaller than the current node, go left; if larger, go right; if equal, we can choose to ignore duplicates (since duplicates would break the strict BST property). After building the tree, we perform two checks in a single recursive traversal: (1) validate BST property by passing allowed minimum and maximum bounds down the tree (using `long long` to handle INT_MIN/INT_MAX edge cases safely), and (2) compute the height of each subtree while checking the balance condition. The recursion returns a structure or pair containing `(isValidBST, height)` or uses a reference parameter. For an empty tree (empty input vector), the function returns `true` because an empty tree is both a valid BST and balanced. Edge cases include a single node, a chain of nodes (e.g., inserted in sorted order) which is balanced if length ≤ 2, and duplicate values which are simply ignored to maintain strict ordering. Time complexity is O(n) for insertion and O(n) for the validation traversal, where n is the number of input values, since each node is visited once. Space complexity is O(n) in the worst case for the tree itself, plus O(h) for the recursion stack where h is the tree height (worst-case O(n) for a skewed tree).

#include <vector>
#include <climits>
#include <cstdlib>

// Node structure for the BST
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// Helper: insert a value into BST, ignoring duplicates
TreeNode* insert(TreeNode* root, int value) {
    if (root == nullptr) {
        return new TreeNode(value);
    }
    if (value < root->val) {
        root->left = insert(root->left, value);
    } else if (value > root->val) {
        root->right = insert(root->right, value);
    }
    // If equal, do nothing (ignore duplicate)
    return root;
}

// Helper: validate BST and compute height in one pass.
// Returns true if valid BST and balanced; height set via reference.
bool validateAndBalance(TreeNode* node, long long minVal, long long maxVal, int& height) {
    if (node == nullptr) {
        height = 0;
        return true;
    }
    // Check BST property
    if (node->val <= minVal || node->val >= maxVal) {
        return false;
    }
    int leftHeight = 0, rightHeight = 0;
    bool leftValid = validateAndBalance(node->left, minVal, node->val, leftHeight);
    bool rightValid = validateAndBalance(node->right, node->val, maxVal, rightHeight);
    if (!leftValid || !rightValid) {
        return false;
    }
    // Check balance condition
    if (std::abs(leftHeight - rightHeight) > 1) {
        return false;
    }
    height = 1 + std::max(leftHeight, rightHeight);
    return true;
}

// Main function: build BST from insertion order and check valid + balanced
bool isBSTAndBalanced(const std::vector<int>& values) {
    if (values.empty()) {
        return true;
    }
    TreeNode* root = nullptr;
    for (int v : values) {
        root = insert(root, v);
    }
    int height = 0;
    bool result = validateAndBalance(root, LLONG_MIN, LLONG_MAX, height);
    // Optional: free memory to avoid leaks (not required for correctness in test)
    // A proper implementation would delete the tree, but for brevity we skip.
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Empty tree
    assert(isBSTAndBalanced({}) == true);

    // Single node
    assert(isBSTAndBalanced({5}) == true);

    // Simple balanced tree: 10, 5, 15
    assert(isBSTAndBalanced({10, 5, 15}) == true);

    // Chain of 3: 1, 2, 3 -> not balanced
    assert(isBSTAndBalanced({1, 2, 3}) == false);

    // Chain of 2: 1, 2 -> balanced (difference 1)
    assert(isBSTAndBalanced({1, 2}) == true);

    // Duplicates ignored: 5, 5, 5 -> only one node
    assert(isBSTAndBalanced({5, 5, 5}) == true);

    // Unbalanced due to right-heavy: 5, 6, 7, 8, 9
    assert(isBSTAndBalanced({5, 6, 7, 8, 9}) == false);

    // Balanced but invalid BST (impossible given insertion rules, but test negative numbers)
    assert(isBSTAndBalanced({-3, -1, -2}) == true); // root -3, left none, right -1 with left -2 => balanced

    // Larger balanced tree: 10, 5, 15, 3, 7, 12, 18 (from snippet)
    assert(isBSTAndBalanced({10, 5, 15, 3, 7, 12, 18}) == true);

    // Tree that is valid BST but unbalanced: 10, 5, 15, 20, 25, 30
    assert(isBSTAndBalanced({10, 5, 15, 20, 25, 30}) == false);

    return 0;
}
