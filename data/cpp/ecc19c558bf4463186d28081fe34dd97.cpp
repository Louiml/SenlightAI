// Write a C++ function `bool isBinarySearchTree(TreeNode* root)` that determines whether a given binary tree satisfies the binary search tree (BST) property: for every node, all values in its left subtree are strictly less than the node’s value, and all values in its right subtree are strictly greater than the node’s value. The function must handle empty trees, duplicate values (which are invalid), and single-node trees. The tree is defined by the provided `TreeNode` structure with `val`, `left`, and `right` pointers. The function should not modify the tree and should return `true` if the tree is a valid BST, `false` otherwise. Use a recursive post-order traversal that returns, for each subtree, whether it is a valid BST, its minimum value, and its maximum value.
The solution performs a recursive depth-first traversal. For each node, it recursively checks the left and right subtrees. If either subtree is not a valid BST, or if the maximum value of the left subtree is not strictly less than the current node’s value, or the minimum value of the right subtree is not strictly greater than the current node’s value, then the tree rooted at that node is invalid. The base case is a null node, which is considered a valid BST (and its min/max can be ignored, but we return some placeholder values). For a non-null node, we start with its value as both the min and max of the current subtree, then merge the results from the children. This approach correctly handles edge cases: an empty tree is valid, a single node without duplicates is valid, a left child equal to the parent is invalid, a right child equal to the parent is invalid, and deeper violations are caught because each subtree returns its min and max to the parent. The time complexity is O(n) where n is the number of nodes, because each node is visited exactly once. Space complexity is O(h) where h is the height of the tree, due to the recursion stack (worst-case O(n) for a skewed tree).
#include <climits>
#include <algorithm>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper struct to hold the result of a subtree check.
struct SubtreeInfo {
    bool isValid;
    long long minVal;
    long long maxVal;
};

// Recursive helper that checks BST property and returns min/max of subtree.
SubtreeInfo checkSubtree(const TreeNode* node) {
    if (!node) {
        // Empty subtree is valid; use extreme values so comparisons succeed.
        return {true, LLONG_MAX, LLONG_MIN};
    }
    
    SubtreeInfo left = checkSubtree(node->left);
    if (!left.isValid) return {false, 0, 0};
    
    SubtreeInfo right = checkSubtree(node->right);
    if (!right.isValid) return {false, 0, 0};
    
    // Check BST condition: left max < node val < right min
    if (left.maxVal >= node->val || right.minVal <= node->val) {
        return {false, 0, 0};
    }
    
    // Merge: new min is the smallest of left min (or node if no left) and node
    long long newMin = (node->left) ? left.minVal : node->val;
    long long newMax = (node->right) ? right.maxVal : node->val;
    
    return {true, newMin, newMax};
}

// Main function to test if a binary tree is a valid BST.
bool isBinarySearchTree(const TreeNode* root) {
    return checkSubtree(root).isValid;
}
#include <cassert>

int main() {
    // Test empty tree
    assert(isBinarySearchTree(nullptr) == true);
    
    // Test single node
    TreeNode* single = new TreeNode(5);
    assert(isBinarySearchTree(single) == true);
    delete single;
    
    // Valid BST:     2
    //               / \
    //              1   3
    TreeNode* root1 = new TreeNode(2);
    root1->left = new TreeNode(1);
    root1->right = new TreeNode(3);
    assert(isBinarySearchTree(root1) == true);
    
    // Invalid: left child equals parent
    TreeNode* root2 = new TreeNode(2);
    root2->left = new TreeNode(2);
    assert(isBinarySearchTree(root2) == false);
    
    // Invalid: right child equals parent
    TreeNode* root3 = new TreeNode(2);
    root3->right = new TreeNode(2);
    assert(isBinarySearchTree(root3) == false);
    
    // Invalid: right subtree has value less than root
    //       10
    //      /  \
    //     5   15
    //         /
    //        12  (12 < 15 but in right subtree, OK)  -- Actually valid
    TreeNode* root4 = new TreeNode(10);
    root4->left = new TreeNode(5);
    root4->right = new TreeNode(15);
    root4->right->left = new TreeNode(12);
    assert(isBinarySearchTree(root4) == true);
    
    // Invalid: left subtree has value greater than root
    //      10
    //      /
    //     15
    TreeNode* root5 = new TreeNode(10);
    root5->left = new TreeNode(15);
    assert(isBinarySearchTree(root5) == false);
    
    // Deep invalid: right child's left subtree contains value > parent's right
    //      10
    //        \
    //        15
    //       /
    //      11  (11 < 15 but 10 < 11, so it's in correct range) -- valid
    TreeNode* root6 = new TreeNode(10);
    root6->right = new TreeNode(15);
    root6->right->left = new TreeNode(11);
    assert(isBinarySearchTree(root6) == true);
    
    // Clean up (for simplicity, but proper deletion would recurse)
    // This is test-only; production code would use smart pointers or a destructor.
    delete root1->left; delete root1->right; delete root1;
    delete root2->left; delete root2;
    delete root3->right; delete root3;
    delete root4->right->left; delete root4->right; delete root4->left; delete root4;
    delete root5->left; delete root5;
    delete root6->right->left; delete root6->right; delete root6;
    
    return 0;
}
