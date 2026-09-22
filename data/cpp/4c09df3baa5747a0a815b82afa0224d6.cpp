Write a C++ function `buildTreeFromPreAndInorder` that constructs a binary tree from two arrays: `preOrder` (containing the preorder traversal) and `inOrder` (containing the inorder traversal), both given as `std::vector<int>` with unique integer values. The function must return the root pointer to a dynamically allocated tree node (defined as `struct TreeNode { int val; TreeNode* left; TreeNode* right; TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} };`). You may assume the two traversals are valid and correspond to the same non-empty tree. Optimize the node search by building a hash map from value to inorder index once. The returned tree must correctly reproduce the original structure, and the function should handle edge cases such as a single‑node tree or skewed trees.

#include <cassert>
#include <vector>

// Assume TreeNode and buildTreeFromPreAndInorder are already declared.

// Helper to verify tree equality via preorder
bool sameTree(TreeNode* a, TreeNode* b) {
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;
    return (a->val == b->val) && sameTree(a->left, b->left) && sameTree(a->right, b->right);
}

// Helper to delete tree (avoid leaks in tests)
void deleteTree(TreeNode* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    // Test 1: simple balanced tree
    {
        std::vector<int> pre = {2, 8, 10, 6, 4, 12};
        std::vector<int> in  = {10, 8, 6, 2, 4, 12};
        TreeNode* root = buildTreeFromPreAndInorder(pre, in);
        // Expected tree:
        //        2
        //      /   \
        //     8     4
        //    / \     \
        //   10  6     12
        // Manually build expected
        TreeNode* expected = new TreeNode(2);
        expected->left = new TreeNode(8);
        expected->left->left = new TreeNode(10);
        expected->left->right = new TreeNode(6);
        expected->right = new TreeNode(4);
        expected->right->right = new TreeNode(12);
        assert(sameTree(root, expected));
        deleteTree(root);
        deleteTree(expected);
    }

    // Test 2: single node
    {
        std::vector<int> pre = {42};
        std::vector<int> in  = {42};
        TreeNode* root = buildTreeFromPreAndInorder(pre, in);
        TreeNode* expected = new TreeNode(42);
        assert(sameTree(root, expected));
        deleteTree(root);
        deleteTree(expected);
    }

    // Test 3: left‑skewed tree
    {
        std::vector<int> pre = {5, 4, 3, 2, 1};
        std::vector<int> in  = {1, 2, 3, 4, 5};
        TreeNode* root = buildTreeFromPreAndInorder(pre, in);
        // Expected: 5->left=4, 4->left=3, 3->left=2, 2->left=1
        TreeNode* expected = new TreeNode(5);
        expected->left = new TreeNode(4);
        expected->left->left = new TreeNode(3);
        expected->left->left->left = new TreeNode(2);
        expected->left->left->left->left = new TreeNode(1);
        assert(sameTree(root, expected));
        deleteTree(root);
        deleteTree(expected);
    }

    // Test 4: right‑skewed tree
    {
        std::vector<int> pre = {1, 2, 3, 4};
        std::vector<int> in  = {1, 2, 3, 4};
        TreeNode* root = buildTreeFromPreAndInorder(pre, in);
        // Expected: 1->right=2, 2->right=3, 3->right=4
        TreeNode* expected = new TreeNode(1);
        expected->right = new TreeNode(2);
        expected->right->right = new TreeNode(3);
        expected->right->right->right = new TreeNode(4);
        assert(sameTree(root, expected));
        deleteTree(root);
        deleteTree(expected);
    }

    // Test 5: larger mixed tree
    {
        std::vector<int> pre = {10, 20, 40, 50, 30, 60};
        std::vector<int> in  = {40, 20, 50, 10, 60, 30};
        TreeNode* root = buildTreeFromPreAndInorder(pre, in);
        // Expected:
        //       10
        //      /  \
        //    20    30
        //   /  \   /
        //  40  50 60
        TreeNode* expected = new TreeNode(10);
        expected->left = new TreeNode(20);
        expected->left->left = new TreeNode(40);
        expected->left->right = new TreeNode(50);
        expected->right = new TreeNode(30);
        expected->right->left = new TreeNode(60);
        assert(sameTree(root, expected));
        deleteTree(root);
        deleteTree(expected);
    }

    return 0;
}

#include <vector>
#include <unordered_map>
#include <cstddef>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper: recursive construction using a preorder index passed by reference.
TreeNode* buildHelper(const std::vector<int>& preOrder, const std::vector<int>& inOrder,
                      int& preIndex, int inStart, int inEnd,
                      const std::unordered_map<int,int>& inMap) {
    // Base case: no more nodes or invalid inorder range
    if (preIndex >= static_cast<int>(preOrder.size()) || inStart > inEnd) {
        return nullptr;
    }

    // Current root from preorder
    int rootVal = preOrder[preIndex++];
    TreeNode* root = new TreeNode(rootVal);

    // Find root position in inorder
    int inPos = inMap.at(rootVal);

    // Recursively build left and right subtrees
    root->left = buildHelper(preOrder, inOrder, preIndex, inStart, inPos - 1, inMap);
    root->right = buildHelper(preOrder, inOrder, preIndex, inPos + 1, inEnd, inMap);

    return root;
}

// Public function: builds tree from preorder and inorder vectors.
TreeNode* buildTreeFromPreAndInorder(const std::vector<int>& preOrder,
                                     const std::vector<int>& inOrder) {
    // Build hash map for O(1) inorder index lookup
    std::unordered_map<int,int> inMap;
    for (std::size_t i = 0; i < inOrder.size(); ++i) {
        inMap[inOrder[i]] = static_cast<int>(i);
    }

    int preIndex = 0;
    return buildHelper(preOrder, inOrder, preIndex, 0,
                       static_cast<int>(inOrder.size()) - 1, inMap);
}

// The core idea is that in a preorder traversal, the first element is always the root of the current subtree. Using the inorder traversal, we can split the remaining elements into left and right subtrees: everything left of the root’s index in inorder forms the left subtree’s inorder sequence, and everything right forms the right subtree’s inorder sequence. We recursively repeat this process, consuming the preorder array from left to right. To avoid an O(n) search for each node, we precompute a hash map (`std::unordered_map<int,int>`) from inorder value to its index, allowing O(1) lookup. The recursion stops when the current inorder range is empty (start > end) or when all preorder elements are consumed. The main edge cases are: a single node (both ranges length 1), a left‑skewed tree (root’s inorder index is at the start, so right range is empty), and a right‑skewed tree (root’s inorder index is at the end). Time complexity: O(n) because each node is processed once and map lookups are O(1). Space complexity: O(n) for the hash map and the recursion stack (worst case O(n) for a skewed tree).
