// Write a C++ function `longestPathLength(TreeNode* root)` that takes the root of a binary tree (where each node stores an integer value and has pointers to left and right children, with `nullptr` for missing children) and returns the maximum number of nodes along any root-to-leaf path. A leaf is a node with no children. The function must be `const`-correct (i.e., it should not modify the tree) and should handle an empty tree (root `nullptr`) by returning 0. For example, if the tree has a single node, the longest path length is 1; if a tree has left branch with 3 nodes and right branch with 2 nodes, the function returns 3. Do not use any global variables; implement the logic recursively or iteratively as you prefer.
// The problem asks for the height (in terms of number of nodes) of the deepest root-to-leaf path. The key is that the longest path length equals 1 (for the current node) plus the maximum of the longest path lengths of its left and right subtrees. If the node is `nullptr`, the length is 0 (base case). For a leaf node (both children `nullptr`), the left and right subtree lengths are each 0, so the result becomes 1 + max(0,0) = 1, which is correct. Edge cases: empty tree returns 0; single-node tree returns 1; unbalanced trees are handled naturally because the recursive formula takes the maximum of both sides. Time complexity is O(n), where n is the number of nodes, because each node is visited exactly once. Space complexity is O(h) for the recursion call stack, where h is the tree height (worst case O(n) for a skewed tree, O(log n) for a balanced tree). The solution avoids global state by using a helper function that returns the depth from a given node.
#include <algorithm>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Returns the maximum number of nodes on any root-to-leaf path.
int longestPathLength(const TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    int left_length = longestPathLength(root->left);
    int right_length = longestPathLength(root->right);
    return 1 + std::max(left_length, right_length);
}
#include <cassert>

int main() {
    // Empty tree
    assert(longestPathLength(nullptr) == 0);

    // Single node
    TreeNode* n1 = new TreeNode(1);
    assert(longestPathLength(n1) == 1);

    // Two-node tree: root with left child
    TreeNode* n2 = new TreeNode(2);
    n1->left = n2;
    assert(longestPathLength(n1) == 2);

    // Three-node tree: root with left and right children
    TreeNode* n3 = new TreeNode(3);
    n1->right = n3;
    assert(longestPathLength(n1) == 2);

    // Unbalanced: left chain of 4 nodes, right chain of 2
    TreeNode* root = new TreeNode(10);
    root->left = new TreeNode(20);
    root->left->left = new TreeNode(30);
    root->left->left->left = new TreeNode(40);
    root->right = new TreeNode(50);
    root->right->left = new TreeNode(60);
    assert(longestPathLength(root) == 4);

    // Balanced tree with 7 nodes (full binary tree)
    TreeNode* full = new TreeNode(1);
    full->left = new TreeNode(2);
    full->right = new TreeNode(3);
    full->left->left = new TreeNode(4);
    full->left->right = new TreeNode(5);
    full->right->left = new TreeNode(6);
    full->right->right = new TreeNode(7);
    assert(longestPathLength(full) == 3);

    // Clean up (not strictly needed for assert tests but good practice)
    delete n2;
    delete n3;
    delete n1;
    delete root->left->left->left;
    delete root->left->left;
    delete root->left;
    delete root->right->left;
    delete root->right;
    delete root;
    delete full->left->left;
    delete full->left->right;
    delete full->right->left;
    delete full->right->right;
    delete full->left;
    delete full->right;
    delete full;

    return 0;
}
