Write a C++ function that takes the root of a binary search tree (BST) and returns `true` if the tree is a valid BST, and `false` otherwise. The tree nodes are defined by the given `TreeNode` struct, which contains an integer value and pointers to left and right children. A valid BST satisfies the following properties: for every node, all values in its left subtree are strictly less than the node's value, and all values in its right subtree are strictly greater than the node's value. The tree may contain duplicate values, but if any duplicate value breaks the strict ordering, the tree is invalid. The input tree may be empty (null root), which is considered a valid BST. The function should handle very large or very small integer values correctly by using a wider integer type (e.g., `long long`) for boundary comparisons.
#include <cassert>

int main() {
    // Test 1: Empty tree is valid
    TreeNode* empty = nullptr;
    assert(isValidBST(empty) == true);

    // Test 2: Single node is valid
    TreeNode single(5);
    assert(isValidBST(&single) == true);

    // Test 3: Simple valid BST: 2 (left:1, right:3)
    TreeNode n1(1), n3(3), root1(2, &n1, &n3);
    assert(isValidBST(&root1) == true);

    // Test 4: Invalid: root 2, left 3 (violates BST)
    TreeNode badLeft(3), root2(2, &badLeft, nullptr);
    assert(isValidBST(&root2) == false);

    // Test 5: Invalid: duplicate value (root 5, left 5)
    TreeNode dupLeft(5), root3(5, &dupLeft, nullptr);
    assert(isValidBST(&root3) == false);

    // Test 6: Valid but unbalanced: 1 -> null -> 2 -> null -> 3
    TreeNode n6(3), n5(2, nullptr, &n6), n4(1, nullptr, &n5);
    assert(isValidBST(&n4) == true);

    // Test 7: Invalid: right subtree has a value smaller than root
    TreeNode deep(4), rightNode(6, &deep, nullptr), root7(5, nullptr, &rightNode);
    assert(isValidBST(&root7) == false);

    // Test 8: Extremely large values near INT_MAX/INT_MIN still handled
    TreeNode bigLeft(INT_MIN), bigRight(INT_MAX), bigRoot(0, &bigLeft, &bigRight);
    assert(isValidBST(&bigRoot) == true);

    // Test 9: Invalid: INT_MAX as left child of 0 (should be less)
    TreeNode wrongMax(INT_MAX), root9(0, &wrongMax, nullptr);
    assert(isValidBST(&root9) == false);

    // Test 10: Larger valid BST: 10 (left:5 with left:2,right:7; right:15 with right:20)
    TreeNode n2a(2), n7(7), n5a(5, &n2a, &n7);
    TreeNode n20(20), n15(15, nullptr, &n20);
    TreeNode root10(10, &n5a, &n15);
    assert(isValidBST(&root10) == true);

    return 0;
}
#include <climits>
#include <cstddef>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

// Helper that recursively validates the BST with allowed value bounds.
bool isSubtreeValid(const TreeNode* node, long long minVal, long long maxVal) {
    if (node == nullptr) {
        return true;
    }
    if (node->val <= minVal || node->val >= maxVal) {
        return false;
    }
    return isSubtreeValid(node->left, minVal, node->val) &&
           isSubtreeValid(node->right, node->val, maxVal);
}

// Returns true if the binary tree rooted at 'root' is a valid BST.
bool isValidBST(const TreeNode* root) {
    return isSubtreeValid(root, LLONG_MIN, LLONG_MAX);
}
// The solution uses a recursive depth-first traversal that propagates allowed value ranges (min and max) down the tree. For each node, we check if its value lies strictly between the inherited min and max bounds (exclusive). If not, the tree is invalid. Then we recursively validate the left subtree with the max bound updated to the node's value, and the right subtree with the min bound updated to the node's value. The base case is a null node, which returns true. The top-level call uses `LLONG_MIN` and `LLONG_MAX` as the initial bounds to handle any possible integer value. Edge cases include: empty tree, trees with duplicate values (e.g., root value 5 and left child 5 — invalid), trees with values at the extremes of `int` range, and unbalanced trees. Time complexity is O(n) where n is the number of nodes, since every node is visited exactly once. Space complexity is O(h) for the recursion stack, where h is the tree height, which is O(n) in the worst case (skewed tree) and O(log n) on average for balanced trees.
