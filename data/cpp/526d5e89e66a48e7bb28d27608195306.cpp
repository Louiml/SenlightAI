/*
Given a binary tree where each node contains an integer value, write a C++ function `int countNodes(const TreeNode* root)` that returns the total number of nodes in the tree. The tree is represented using the following structure: `struct TreeNode { int val; TreeNode* left; TreeNode* right; TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} };`. An empty tree (nullptr root) should return 0. The function must be const-correct (work with a pointer to a const TreeNode) and must not modify the tree. Assume the tree can have up to 10^5 nodes, and the recursion depth may be large, but you may assume the tree is not deliberately skewed to cause stack overflow in normal test cases.
*/
// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Returns the total number of nodes in the binary tree.
// Empty tree (nullptr) returns 0.
// The function does not modify the tree and is const-correct.
int countNodes(const TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    // Count the current node plus nodes in left and right subtrees.
    return 1 + countNodes(root->left) + countNodes(root->right);
}
#include <cassert>

int main() {
    // Test 1: Empty tree
    assert(countNodes(nullptr) == 0);

    // Test 2: Single node
    TreeNode* node1 = new TreeNode(5);
    assert(countNodes(node1) == 1);

    // Test 3: Three-node tree (root with two children)
    TreeNode* root3 = new TreeNode(1);
    root3->left = new TreeNode(2);
    root3->right = new TreeNode(3);
    assert(countNodes(root3) == 3);

    // Test 4: Left-skewed tree of height 4
    TreeNode* root4 = new TreeNode(1);
    root4->left = new TreeNode(2);
    root4->left->left = new TreeNode(3);
    root4->left->left->left = new TreeNode(4);
    assert(countNodes(root4) == 4);

    // Test 5: A more complex tree
    //        10
    //       /  \
    //      20   30
    //     / \     \
    //    40  50    60
    TreeNode* root5 = new TreeNode(10);
    root5->left = new TreeNode(20);
    root5->right = new TreeNode(30);
    root5->left->left = new TreeNode(40);
    root5->left->right = new TreeNode(50);
    root5->right->right = new TreeNode(60);
    assert(countNodes(root5) == 6);

    // Test 6: Only right child
    TreeNode* root6 = new TreeNode(1);
    root6->right = new TreeNode(2);
    root6->right->right = new TreeNode(3);
    assert(countNodes(root6) == 3);

    // Cleanup (optional for tests, but good practice)
    // In a real program, you'd delete all allocated nodes.
    delete node1;
    delete root3->left;
    delete root3->right;
    delete root3;
    delete root4->left->left->left;
    delete root4->left->left;
    delete root4->left;
    delete root4;
    delete root5->left->left;
    delete root5->left->right;
    delete root5->left;
    delete root5->right->right;
    delete root5->right;
    delete root5;
    delete root6->right->right;
    delete root6->right;
    delete root6;

    return 0;
}
// The solution uses a simple recursive depth-first traversal. The function visits each node exactly once. At each node, it adds 1 to a running count and then recursively processes the left and right subtrees. The base case is when the current node is `nullptr`, which contributes 0. This is the classic post-order/in-order/pre-order counting problem. Edge cases include an empty tree (returns 0), a tree with only one node (returns 1), and unbalanced trees. Time complexity is O(n) where n is the number of nodes, because each node is visited once. Space complexity is O(h) where h is the height of the tree, due to recursion stack usage; in the worst case (a skewed tree) this becomes O(n), but that’s expected for a recursive solution without tail-call optimization.
