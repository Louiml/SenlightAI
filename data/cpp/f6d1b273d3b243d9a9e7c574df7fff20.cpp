// Given two vectors of integers, `preorder` and `inorder`, which represent the preorder and inorder traversals of a binary tree containing unique integer values, write a C++ function `buildUniqueBinaryTree` that reconstructs and returns a pointer to the root node of the corresponding binary tree. You must not modify the input vectors. The function should handle an empty input (when both vectors are empty) by returning `nullptr`. The tree nodes must be dynamically allocated with `new` and properly connected. You may assume the input always represents a valid binary tree (i.e., the vectors are non-empty together and contain the same unique values, with `preorder[0]` being the root). The function signature must be: `TreeNode* buildUniqueBinaryTree(const std::vector<int>& preorder, const std::vector<int>& inorder);` where `TreeNode` is defined as `struct TreeNode { int val; TreeNode *left; TreeNode *right; TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} };`. Include all necessary headers and define the `TreeNode` struct in your solution.

#include <cassert>
#include <vector>
#include <functional>

// Helper to recursively delete tree to avoid leaks (not required for asserts but good practice).
void deleteTree(TreeNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

// Helper to compare two trees structurally.
bool sameTree(TreeNode* a, TreeNode* b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
    return a->val == b->val && sameTree(a->left, b->left) && sameTree(a->right, b->right);
}

int main() {
    // Test 1: Empty input
    std::vector<int> pre1 = {}, in1 = {};
    TreeNode* root1 = buildUniqueBinaryTree(pre1, in1);
    assert(root1 == nullptr);

    // Test 2: Single node
    std::vector<int> pre2 = {5}, in2 = {5};
    TreeNode* root2 = buildUniqueBinaryTree(pre2, in2);
    assert(root2 != nullptr);
    assert(root2->val == 5);
    assert(root2->left == nullptr && root2->right == nullptr);
    delete root2;

    // Test 3: Balanced tree (preorder: 1,2,4,5,3,6,7; inorder: 4,2,5,1,6,3,7)
    std::vector<int> pre3 = {1,2,4,5,3,6,7};
    std::vector<int> in3 = {4,2,5,1,6,3,7};
    TreeNode* root3 = buildUniqueBinaryTree(pre3, in3);
    TreeNode* expected3 = new TreeNode(1);
    expected3->left = new TreeNode(2);
    expected3->left->left = new TreeNode(4);
    expected3->left->right = new TreeNode(5);
    expected3->right = new TreeNode(3);
    expected3->right->left = new TreeNode(6);
    expected3->right->right = new TreeNode(7);
    assert(sameTree(root3, expected3));
    deleteTree(root3);
    deleteTree(expected3);

    // Test 4: Left-skewed tree (preorder: 1,2,3; inorder: 3,2,1)
    std::vector<int> pre4 = {1,2,3};
    std::vector<int> in4 = {3,2,1};
    TreeNode* root4 = buildUniqueBinaryTree(pre4, in4);
    assert(root4->val == 1);
    assert(root4->left->val == 2);
    assert(root4->left->left->val == 3);
    assert(root4->right == nullptr);
    deleteTree(root4);

    // Test 5: Right-skewed tree (preorder: 1,2,3; inorder: 1,2,3)
    std::vector<int> pre5 = {1,2,3};
    std::vector<int> in5 = {1,2,3};
    TreeNode* root5 = buildUniqueBinaryTree(pre5, in5);
    assert(root5->val == 1);
    assert(root5->right->val == 2);
    assert(root5->right->right->val == 3);
    assert(root5->left == nullptr);
    deleteTree(root5);

    // Test 6: Two-node tree (root with left child)
    std::vector<int> pre6 = {10, 20}, in6 = {20, 10};
    TreeNode* root6 = buildUniqueBinaryTree(pre6, in6);
    assert(root6->val == 10);
    assert(root6->left->val == 20);
    assert(root6->right == nullptr);
    deleteTree(root6);

    return 0;
}

#include <vector>
#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper function that recursively builds the tree.
TreeNode* buildHelper(const std::vector<int>& preorder, const std::vector<int>& inorder,
                      int preIndex, int inStart, int inEnd) {
    if (inStart > inEnd) return nullptr;

    // The current root value comes from preorder at preIndex.
    int rootVal = preorder[preIndex];
    TreeNode* node = new TreeNode(rootVal);

    // Find the root's position in inorder within [inStart, inEnd].
    int inIndex = inStart;
    while (inorder[inIndex] != rootVal) {
        ++inIndex;
    }

    // Build left subtree using next preorder index and inorder range [inStart, inIndex-1].
    node->left = buildHelper(preorder, inorder, preIndex + 1, inStart, inIndex - 1);

    // Build right subtree: the preorder index for right subtree is preIndex + (size of left subtree) + 1.
    int leftSize = inIndex - inStart;
    node->right = buildHelper(preorder, inorder, preIndex + leftSize + 1, inIndex + 1, inEnd);

    return node;
}

// Reconstruct a binary tree from its preorder and inorder traversals.
// Returns nullptr if input is empty.
TreeNode* buildUniqueBinaryTree(const std::vector<int>& preorder, const std::vector<int>& inorder) {
    if (preorder.empty() || inorder.empty()) return nullptr;
    return buildHelper(preorder, inorder, 0, 0, static_cast<int>(inorder.size()) - 1);
}

// The key insight is that in a preorder traversal, the first element is always the root of the current subtree. In an inorder traversal, the root splits the sequence into a left subtree (elements before the root) and a right subtree (elements after the root). The algorithm uses a recursive approach: at each step, take the next element from the preorder vector (tracked by an index that increments as we consume roots), find its position in the inorder vector within the current range `[i,j]`. The elements from `i` to `k-1` form the left subtree, and `k+1` to `j` form the right subtree. Recursively build those subtrees. The base case is when `i > j`, meaning no nodes remain for that subtree, return `nullptr`. Since all values are unique and the input is guaranteed valid, we can linearly search for the root in the inorder segment. Edge cases: empty input (return `nullptr`), a single-node tree, and skewed trees (where one subtree is always empty). Time complexity is O(n²) in the worst case (e.g., a skewed tree where each search scans the entire remaining inorder segment), but averages O(n log n) for balanced trees. Space complexity is O(n) due to the recursion stack depth in the worst case (skewed tree) and the dynamic allocations.
