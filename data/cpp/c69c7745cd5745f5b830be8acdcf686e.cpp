/*
Write a C++ function that takes two pointers to binary tree nodes (representing the roots of two binary trees) and returns a boolean value indicating whether the two trees are structurally identical and have identical node values at every corresponding position. The function should handle empty trees gracefully: two null pointers are considered equal, while one null and one non-null pointer are unequal. You may assume the tree node structure is defined as `struct TreeNode { int val; TreeNode* left; TreeNode* right; TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} };`. The function must be recursive, use `const` correctness where appropriate (i.e., the node pointers are not modified), and must not rely on any global state.
*/

#include <cstddef>

// Definition for a binary tree node (provided in task description).
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Returns true if two binary trees rooted at p and q are identical in structure and values.
bool isSameBinaryTree(const TreeNode* p, const TreeNode* q) {
    if (p == nullptr && q == nullptr) {
        return true;
    }
    if (p == nullptr || q == nullptr) {
        return false;
    }
    if (p->val != q->val) {
        return false;
    }
    // Both nodes exist and have the same value; compare subtrees recursively.
    return isSameBinaryTree(p->left, q->left) && isSameBinaryTree(p->right, q->right);
}

#include <cassert>

int main() {
    // Test 1: Both trees empty
    assert(isSameBinaryTree(nullptr, nullptr) == true);

    // Test 2: One tree empty, the other not
    TreeNode* p1 = new TreeNode(1);
    assert(isSameBinaryTree(p1, nullptr) == false);
    assert(isSameBinaryTree(nullptr, p1) == false);

    // Test 3: Both trees same single node
    TreeNode* p2 = new TreeNode(5);
    TreeNode* q2 = new TreeNode(5);
    assert(isSameBinaryTree(p2, q2) == true);

    // Test 4: Single nodes with different values
    TreeNode* p3 = new TreeNode(5);
    TreeNode* q3 = new TreeNode(6);
    assert(isSameBinaryTree(p3, q3) == false);

    // Test 5: Identical two-level trees
    TreeNode* p4 = new TreeNode(1);
    p4->left = new TreeNode(2);
    p4->right = new TreeNode(3);
    TreeNode* q4 = new TreeNode(1);
    q4->left = new TreeNode(2);
    q4->right = new TreeNode(3);
    assert(isSameBinaryTree(p4, q4) == true);

    // Test 6: Same structure but different values at right child
    TreeNode* p5 = new TreeNode(1);
    p5->left = new TreeNode(2);
    p5->right = new TreeNode(3);
    TreeNode* q5 = new TreeNode(1);
    q5->left = new TreeNode(2);
    q5->right = new TreeNode(4);
    assert(isSameBinaryTree(p5, q5) == false);

    // Test 7: Same values but different structure (left vs right only)
    TreeNode* p6 = new TreeNode(1);
    p6->left = new TreeNode(2);
    TreeNode* q6 = new TreeNode(1);
    q6->right = new TreeNode(2);
    assert(isSameBinaryTree(p6, q6) == false);

    // Test 8: One tree has extra node
    TreeNode* p7 = new TreeNode(1);
    p7->left = new TreeNode(2);
    TreeNode* q7 = new TreeNode(1);
    q7->left = new TreeNode(2);
    q7->right = new TreeNode(3);
    assert(isSameBinaryTree(p7, q7) == false);

    // Clean up (optional in this context, but done for completeness)
    // For brevity, memory cleanup is omitted here since the tests are short-lived.
    
    return 0;
}

// The solution uses a recursive depth-first comparison. At each call, we check the current pair of nodes: if both are null, they match; if exactly one is null, they do not match; if their values differ, they do not match. If all these checks pass, we recursively check the left subtrees and right subtrees, and the trees are equal only if both recursive calls return true. This covers all cases: empty trees, single-node trees, and larger structures. Edge cases include both roots being null (returns true), one root null (returns false), and nodes with equal values but different children structures (handled by recursion). The time complexity is O(n) where n is the total number of nodes in the smaller tree (worst case, the larger tree if they have identical shapes up to its full size); more precisely, we visit every node of both trees until a mismatch is found, so O(min(size(p), size(q))) in the best case and O(size(p)+size(q)) in the worst case. The space complexity is O(h) due to the recursion stack, where h is the height of the trees (worst case O(n) for skewed trees, average O(log n) for balanced trees).
