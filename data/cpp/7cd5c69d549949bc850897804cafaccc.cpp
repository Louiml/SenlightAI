Write a C++ function that, given the root of a binary tree, returns the sum of all left leaves. A left leaf is a leaf node (a node with no left or right child) that is also the left child of its parent. The tree is represented by a `TreeNode` struct with an integer value and pointers to left and right children. The function must be named `sumOfLeftLeaves`, take a `TreeNode*` argument, and return an `int`. The input tree is non-empty in practical usage, but your function should safely handle a null root by returning 0.

#include <cassert>

int main() {
    // Test 1: Empty tree.
    assert(sumOfLeftLeaves(nullptr) == 0);

    // Test 2: Single node tree (root is not a left leaf).
    TreeNode* root1 = new TreeNode(5);
    assert(sumOfLeftLeaves(root1) == 0);
    delete root1;

    // Test 3: Simple two-node tree (left leaf exists).
    TreeNode* root2 = new TreeNode(1);
    root2->left = new TreeNode(2);
    assert(sumOfLeftLeaves(root2) == 2);
    delete root2->left;
    delete root2;

    // Test 4: More complex tree with left leaves at different levels.
    //      3
    //     / \
    //    9  20
    //      /  \
    //     15   7
    // Left leaves: 9 (left child of 3) and 15 (left child of 20). Sum = 24.
    TreeNode* root3 = new TreeNode(3);
    root3->left = new TreeNode(9);
    root3->right = new TreeNode(20);
    root3->right->left = new TreeNode(15);
    root3->right->right = new TreeNode(7);
    assert(sumOfLeftLeaves(root3) == 24);
    delete root3->left;
    delete root3->right->left;
    delete root3->right->right;
    delete root3->right;
    delete root3;

    // Test 5: Tree with no left leaves.
    //      1
    //     / \
    //    2   3
    //         \
    //          4 (right child, not left leaf)
    // Left leaves: none (2 is not a leaf because it has a left? Wait, add it)
    // Let's build a clear case: root->right->left = null, root->right->right = leaf.
    // Simpler: root->left is internal, root->right is leaf.
    TreeNode* root4 = new TreeNode(1);
    root4->left = new TreeNode(2);
    root4->right = new TreeNode(3);
    root4->left->left = new TreeNode(4); // 4 is left leaf? Actually 4 is left child of 2, leaf. Yes sum = 4.
    // Wait, this actually has a left leaf. Let's instead make a tree with no left leaves:
    //      1
    //       \
    //        2
    // For isolated root with only right child, no left leaves.
    TreeNode* root5 = new TreeNode(1);
    root5->right = new TreeNode(2);
    assert(sumOfLeftLeaves(root5) == 0);
    delete root5->right;
    delete root5;
    // Clean up and test root4 as well.
    delete root4->left->left;
    delete root4->left;
    delete root4->right;
    delete root4;

    // Test 6: Deep tree with left leaf at deepest level.
    //      1
    //     /
    //    2
    //   /
    //  3   (left leaf)
    TreeNode* root6 = new TreeNode(1);
    root6->left = new TreeNode(2);
    root6->left->left = new TreeNode(3);
    assert(sumOfLeftLeaves(root6) == 3);
    delete root6->left->left;
    delete root6->left;
    delete root6;

    // Test 7: Mixed tree with right-leaf and left-leaf.
    //      1
    //     / \
    //    2   3
    //   /     \
    //  4       5  (4 is left leaf, 5 is right leaf)
    // Sum = 4.
    TreeNode* root7 = new TreeNode(1);
    root7->left = new TreeNode(2);
    root7->right = new TreeNode(3);
    root7->left->left = new TreeNode(4);
    root7->right->right = new TreeNode(5);
    assert(sumOfLeftLeaves(root7) == 4);
    delete root7->left->left;
    delete root7->right->right;
    delete root7->left;
    delete root7->right;
    delete root7;

    return 0;
}

#include <cstddef> // for nullptr

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper function that recursively calculates the sum of left leaves.
int sumLeftLeavesHelper(const TreeNode* root, bool is_left) {
    if (root == nullptr) {
        return 0;
    }
    // Check if it's a leaf node.
    if (root->left == nullptr && root->right == nullptr) {
        return is_left ? root->val : 0;
    }
    // Recurse on left and right children.
    return sumLeftLeavesHelper(root->left, true) +
           sumLeftLeavesHelper(root->right, false);
}

// Public function to compute the sum of all left leaves in the tree.
int sumOfLeftLeaves(const TreeNode* root) {
    return sumLeftLeavesHelper(root, false);
}

// The solution uses a depth-first recursive traversal of the tree, passing a boolean flag indicating whether the current node is a left child of its parent. At each node: if the node is null, return 0 (base case). If the node is a leaf (no children), return its value if `is_left` is true, otherwise return 0. For internal nodes, recursively compute the sum for the left subtree with `is_left=true` and the right subtree with `is_left=false`, then return their sum. The main function calls the helper with `is_left=false` because the root is not a left child. Edge cases include: empty tree (returns 0), a single-node tree (root is not a left leaf, so returns 0), and trees where left leaves exist at various depths. The time complexity is O(n) because each node is visited exactly once, and space complexity is O(h) due to the recursion depth, where h is the height of the tree.
