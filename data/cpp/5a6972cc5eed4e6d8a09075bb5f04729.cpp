// Given a binary tree whose nodes contain integer values and whose structure is built by inserting nodes along specified paths from the root, write a C++ function `int maxDepth(const TreeNode* root)` that computes the maximum depth (number of nodes along the longest root-to-leaf path). The tree is represented using a standard `TreeNode` struct with `val`, `left`, and `right` pointers. The function must handle empty trees (return 0) and single-node trees (return 1) correctly. You may assume the tree is built without cycles and that values are not necessarily unique across different branches.
// The solution uses recursive depth-first traversal. For each node, if it is null, the depth is 0; otherwise, the depth is 1 plus the maximum of the depths of the left and right subtrees. This is a classic post-order traversal where we compute the depth of children before combining. Key edge cases include: an empty tree passed as `nullptr` (return 0), a single-node tree (left and right are null, so depth = 1 + max(0,0) = 1), and unbalanced trees where one side is much deeper than the other—the algorithm correctly takes the maximum. Time complexity is O(n), where n is the number of nodes, since each node is visited exactly once. Space complexity is O(h), where h is the height of the tree, due to the recursive call stack; in the worst case (a skewed tree), h = n, so space is O(n) worst-case, O(log n) for balanced trees.
#include <algorithm>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Compute the maximum depth (number of nodes on the longest root-to-leaf path).
int maxDepth(const TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    int leftDepth = maxDepth(root->left);
    int rightDepth = maxDepth(root->right);
    return 1 + std::max(leftDepth, rightDepth);
}
#include <cassert>
#include <vector>

// Assume the Node struct and maxDepth function from the solution are available.

int main() {
    // Test 1: Empty tree
    TreeNode* empty = nullptr;
    assert(maxDepth(empty) == 0);

    // Test 2: Single node tree
    TreeNode* single = new TreeNode(5);
    assert(maxDepth(single) == 1);

    // Test 3: Balanced tree of depth 3
    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5
    TreeNode* root1 = new TreeNode(1);
    root1->left = new TreeNode(2);
    root1->right = new TreeNode(3);
    root1->left->left = new TreeNode(4);
    root1->left->right = new TreeNode(5);
    assert(maxDepth(root1) == 3);

    // Test 4: Left-skewed tree of depth 4
    TreeNode* root2 = new TreeNode(1);
    root2->left = new TreeNode(2);
    root2->left->left = new TreeNode(3);
    root2->left->left->left = new TreeNode(4);
    assert(maxDepth(root2) == 4);

    // Test 5: Right-skewed tree of depth 2
    TreeNode* root3 = new TreeNode(1);
    root3->right = new TreeNode(2);
    assert(maxDepth(root3) == 2);

    // Test 6: Mixed tree, left deeper than right
    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5
    //  /
    // 6
    TreeNode* root4 = new TreeNode(1);
    root4->left = new TreeNode(2);
    root4->right = new TreeNode(3);
    root4->left->left = new TreeNode(4);
    root4->left->right = new TreeNode(5);
    root4->left->left->left = new TreeNode(6);
    assert(maxDepth(root4) == 4);

    // Clean up memory (not required for assert checks, but good practice)
    delete single;
    delete root1->left->left;
    delete root1->left->right;
    delete root1->left;
    delete root1->right;
    delete root1;
    delete root2->left->left->left;
    delete root2->left->left;
    delete root2->left;
    delete root2;
    delete root3->right;
    delete root3;
    delete root4->left->left->left;
    delete root4->left->left;
    delete root4->left->right;
    delete root4->left;
    delete root4->right;
    delete root4;

    return 0;
}
