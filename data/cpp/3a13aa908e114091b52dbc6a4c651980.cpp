// Write a C++ function `hasPathSum` that takes a binary tree root node (with integer values, left and right child pointers) and an integer `target`, and returns `true` if there exists at least one root-to-leaf path where the sum of node values along that path equals `target`, and `false` otherwise. The tree may be empty, and node values can be negative, zero, or positive. A leaf is a node with no children. The function should correctly handle cases where a path sum equals the target only at an internal node (which must not be considered) and where multiple valid paths exist.

#include <cassert>

int main() {
    // Test 1: Empty tree
    TreeNode* empty = nullptr;
    assert(!hasPathSum(empty, 0));

    // Test 2: Single node, target matches
    TreeNode* single = new TreeNode(5);
    assert(hasPathSum(single, 5));
    assert(!hasPathSum(single, 6));

    // Test 3: Tree: 1 -> left:2, right:3; 2 has left:4, right:5; 3 has right:6
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->right = new TreeNode(6);
    
    // Paths: 1-2-4 (sum 7), 1-2-5 (sum 8), 1-3-6 (sum 10)
    assert(hasPathSum(root, 7));
    assert(hasPathSum(root, 8));
    assert(hasPathSum(root, 10));
    assert(!hasPathSum(root, 6));  // sum 6 not a leaf path
    assert(!hasPathSum(root, 3));  // sum at internal node not valid

    // Test 4: Negative values: root= -2, left= -3, right= 5
    TreeNode* negRoot = new TreeNode(-2);
    negRoot->left = new TreeNode(-3);
    negRoot->right = new TreeNode(5);
    // Leaf paths: -2 + -3 = -5, -2 + 5 = 3
    assert(hasPathSum(negRoot, -5));
    assert(hasPathSum(negRoot, 3));
    assert(!hasPathSum(negRoot, -2)); // internal node sum

    // Test 5: Two equal paths
    TreeNode* dupRoot = new TreeNode(0);
    dupRoot->left = new TreeNode(3);
    dupRoot->right = new TreeNode(3);
    assert(hasPathSum(dupRoot, 3));

    // Cleanup (not strictly necessary for tests, but good practice)
    // (Omitted for brevity; production code would delete nodes.)

    return 0;
}

#include <cstddef>  // for nullptr

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Returns true if there exists a root-to-leaf path whose sum equals target.
bool hasPathSum(TreeNode* root, int target) {
    if (root == nullptr) {
        return false;
    }
    // If it's a leaf node, check if the remaining target equals the node's value.
    if (root->left == nullptr && root->right == nullptr) {
        return root->val == target;
    }
    // Subtract current node's value and recurse on children.
    int remaining = target - root->val;
    return hasPathSum(root->left, remaining) || hasPathSum(root->right, remaining);
}

// The solution uses a recursive depth-first traversal. At each node, we subtract the current node's value from the remaining target and propagate the updated target to children. The base cases are: if the root is `nullptr`, return `false` (no path); if the node is a leaf (both children are `nullptr`), return whether the remaining target equals the node's value. For internal nodes, we recursively check the left and right subtrees with the reduced target. Key edge cases include an empty tree (returns `false`), a single-node tree where the node's value equals target (returns `true`), negative values (handled naturally by subtraction), and paths that sum to target only at non-leaf nodes (correctly ignored). Time complexity is \(O(n)\) where \(n\) is the number of nodes, as each node is visited once. Auxiliary space is \(O(h)\) due to recursion stack depth, where \(h\) is the tree height (worst case \(O(n)\) for skewed trees). No extra data structures are needed.
