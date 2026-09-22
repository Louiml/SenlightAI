// Write a C++ function `int minDifferenceBetweenNodes(TreeNode* root)` that takes the root of a binary search tree (BST) and returns the minimum absolute difference between the values of any two distinct nodes in the tree. You may assume the tree has at least two nodes. The TreeNode structure is defined as: `struct TreeNode { int val; TreeNode *left; TreeNode *right; TreeNode() : val(0), left(nullptr), right(nullptr) {} TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {} };`. The function must be efficient and correctly handle cases where the tree is skewed (e.g., a linked list) or balanced, with positive or negative values.
#include <cassert>

int main() {
    // Test 1: Simple BST with 3 nodes
    TreeNode* root1 = new TreeNode(4);
    root1->left = new TreeNode(2);
    root1->right = new TreeNode(6);
    assert(minDifferenceBetweenNodes(root1) == 2); // Differences: 2-4=2, 4-6=2

    // Test 2: Skewed tree (linked list)
    TreeNode* root2 = new TreeNode(1);
    root2->right = new TreeNode(3);
    root2->right->right = new TreeNode(7);
    assert(minDifferenceBetweenNodes(root2) == 2); // Differences: 1-3=2, 3-7=4

    // Test 3: Tree with negative and positive values
    TreeNode* root3 = new TreeNode(0);
    root3->left = new TreeNode(-10);
    root3->right = new TreeNode(10);
    assert(minDifferenceBetweenNodes(root3) == 10); // Differences: -10-0=10, 0-10=10

    // Test 4: Tree with exactly two nodes
    TreeNode* root4 = new TreeNode(5);
    root4->left = new TreeNode(3);
    assert(minDifferenceBetweenNodes(root4) == 2);

    // Test 5: Tree with duplicate values (but distinct nodes)
    TreeNode* root5 = new TreeNode(5);
    root5->left = new TreeNode(5);
    root5->right = new TreeNode(8);
    assert(minDifferenceBetweenNodes(root5) == 0); // Difference between equal values is 0

    // Test 6: Larger balanced tree
    TreeNode* root6 = new TreeNode(8);
    root6->left = new TreeNode(3);
    root6->right = new TreeNode(10);
    root6->left->left = new TreeNode(1);
    root6->left->right = new TreeNode(6);
    root6->left->right->right = new TreeNode(7);
    root6->right->right = new TreeNode(14);
    root6->right->right->left = new TreeNode(13);
    assert(minDifferenceBetweenNodes(root6) == 1); // Differences between 6 and 7, or 13 and 14

    // Test 7: Empty? Not allowed per spec but ensure function doesn't crash (though result undefined)
    // TreeNode* root7 = nullptr; // Not tested per spec (at least two nodes)

    // Test 8: Highly skewed left
    TreeNode* root8 = new TreeNode(10);
    root8->left = new TreeNode(9);
    root8->left->left = new TreeNode(8);
    root8->left->left->left = new TreeNode(7);
    assert(minDifferenceBetweenNodes(root8) == 1);

    // Test 9: Root has only right children with large gaps
    TreeNode* root9 = new TreeNode(0);
    root9->right = new TreeNode(100);
    root9->right->right = new TreeNode(200);
    assert(minDifferenceBetweenNodes(root9) == 100);

    // Test 10: All negative values
    TreeNode* root10 = new TreeNode(-5);
    root10->left = new TreeNode(-8);
    root10->right = new TreeNode(-2);
    assert(minDifferenceBetweenNodes(root10) == 3); // Differences: -8-(-5)=3, -5-(-2)=3

    return 0;
}
#include <vector>
#include <algorithm>
#include <climits>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper function to perform in-order traversal and collect node values.
void collectInorder(TreeNode* root, std::vector<int>& values) {
    if (root == nullptr) return;
    collectInorder(root->left, values);
    values.push_back(root->val);
    collectInorder(root->right, values);
}

// Returns the minimum absolute difference between any two distinct nodes in a BST.
int minDifferenceBetweenNodes(TreeNode* root) {
    std::vector<int> values;
    collectInorder(root, values);
    
    int result = INT_MAX;
    for (size_t i = 1; i < values.size(); ++i) {
        result = std::min(result, values[i] - values[i - 1]);
    }
    return result;
}
// The key property of a BST is that an in-order traversal yields the node values in ascending order. Since the difference between any two nodes is minimized when they are adjacent in sorted order, we can perform an in-order traversal to collect the values into a vector, then iterate through the vector computing the difference between consecutive elements. The minimum difference among consecutive sorted values is the answer. Edge cases include: the tree having exactly two nodes (the vector size is 2, and we compare the single pair), or values that are large in magnitude (use `INT_MAX` as initial result). The recursion for in-order traversal visits each node exactly once, so the time complexity is O(n), where n is the number of nodes. The auxiliary space for the vector is O(n), and the recursion stack in the worst-case (skewed tree) is O(n). The solution does not modify the tree, so it can be marked as `const` if desired, but we'll keep it simple.
