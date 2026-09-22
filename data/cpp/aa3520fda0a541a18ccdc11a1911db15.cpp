Write a C++ function `treeNode* buildTreeFromPreIn(const std::vector<int>& preorder, const std::vector<int>& inorder)` that reconstructs a binary tree from its preorder and inorder traversal sequences. The tree nodes should store integer data, with each node having `left` and `right` pointers. The function must return the root pointer of the constructed tree. You may assume the input sequences are valid for a single binary tree with distinct node values. The function should handle an empty tree (empty vectors) by returning `nullptr`. After constructing the tree, you should verify correctness by comparing the preorder and inorder traversals of the reconstructed tree with the original inputs using iterative or recursive traversal helpers that you implement within the test file.

// The key insight is that in preorder traversal, the first element is always the root of the (sub)tree. In inorder traversal, elements to the left of the root's value belong to the left subtree, and elements to the right belong to the right subtree. So we can recursively partition the inorder array based on the current root's value. We maintain an index into the preorder array (passed by reference or as a static index) that advances each time we create a new node. For each recursive call, we are given the bounds `[start, end]` of the current subtree within the inorder array. We find the position of the current preorder element in inorder using a linear search (or an unordered_map for O(1) lookup, but linear is acceptable for small inputs). The left subtree is built from `(start, pos-1)` and the right from `(pos+1, end)`. The base case is when `start == end`, meaning a leaf node. For an empty range (`start > end`), return `nullptr`. Edge cases: empty input vectors → return `nullptr`; single-node tree; skewed trees (all left or all right). Time complexity is O(n²) in the worst case using linear search (each search takes O(n) and there are n nodes), but with a hash map it becomes O(n). Space complexity is O(n) due to recursion stack depth and the map.

#include <vector>
#include <unordered_map>
#include <stdexcept>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper function that builds the tree recursively.
// preIndex is passed by reference to keep track of the next preorder element.
TreeNode* buildHelper(
    const std::vector<int>& preorder,
    const std::vector<int>& inorder,
    int& preIndex,
    int inStart,
    int inEnd,
    const std::unordered_map<int, int>& inPos
) {
    if (inStart > inEnd) {
        return nullptr;
    }

    int rootVal = preorder[preIndex++];
    TreeNode* root = new TreeNode(rootVal);

    // If this is a leaf node
    if (inStart == inEnd) {
        return root;
    }

    int inIndex = inPos.at(rootVal);

    // Build left subtree first, then right subtree
    root->left = buildHelper(preorder, inorder, preIndex, inStart, inIndex - 1, inPos);
    root->right = buildHelper(preorder, inorder, preIndex, inIndex + 1, inEnd, inPos);

    return root;
}

// Main reconstruction function.
TreeNode* buildTreeFromPreIn(
    const std::vector<int>& preorder,
    const std::vector<int>& inorder
) {
    if (preorder.empty() || inorder.empty()) {
        return nullptr;
    }

    // Build a map from value to its index in inorder for O(1) lookup
    std::unordered_map<int, int> inPos;
    for (size_t i = 0; i < inorder.size(); ++i) {
        inPos[inorder[i]] = static_cast<int>(i);
    }

    int preIndex = 0;
    return buildHelper(
        preorder,
        inorder,
        preIndex,
        0,
        static_cast<int>(inorder.size()) - 1,
        inPos
    );
}

#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

// Helper to collect preorder traversal
void collectPre(TreeNode* node, std::vector<int>& out) {
    if (!node) return;
    out.push_back(node->val);
    collectPre(node->left, out);
    collectPre(node->right, out);
}

// Helper to collect inorder traversal
void collectIn(TreeNode* node, std::vector<int>& out) {
    if (!node) return;
    collectIn(node->left, out);
    out.push_back(node->val);
    collectIn(node->right, out);
}

// Helper to delete tree (avoid memory leaks)
void deleteTree(TreeNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

int main() {
    // Test 1: Empty tree
    {
        std::vector<int> pre = {};
        std::vector<int> in = {};
        TreeNode* root = buildTreeFromPreIn(pre, in);
        assert(root == nullptr);
    }

    // Test 2: Single node
    {
        std::vector<int> pre = {42};
        std::vector<int> in = {42};
        TreeNode* root = buildTreeFromPreIn(pre, in);
        assert(root != nullptr);
        assert(root->val == 42);
        assert(root->left == nullptr);
        assert(root->right == nullptr);
        deleteTree(root);
    }

    // Test 3: Balanced tree (from the snippet example)
    {
        std::vector<int> pre = {5, 2, 1, 3, 4, 7, 6};
        std::vector<int> in = {1, 2, 3, 4, 5, 6, 7};
        TreeNode* root = buildTreeFromPreIn(pre, in);
        std::vector<int> preOut, inOut;
        collectPre(root, preOut);
        collectIn(root, inOut);
        assert(preOut == pre);
        assert(inOut == in);
        deleteTree(root);
    }

    // Test 4: Left-skewed tree
    {
        std::vector<int> pre = {4, 3, 2, 1};
        std::vector<int> in = {1, 2, 3, 4};
        TreeNode* root = buildTreeFromPreIn(pre, in);
        std::vector<int> preOut, inOut;
        collectPre(root, preOut);
        collectIn(root, inOut);
        assert(preOut == pre);
        assert(inOut == in);
        deleteTree(root);
    }

    // Test 5: Right-skewed tree
    {
        std::vector<int> pre = {1, 2, 3, 4};
        std::vector<int> in = {1, 2, 3, 4};
        TreeNode* root = buildTreeFromPreIn(pre, in);
        std::vector<int> preOut, inOut;
        collectPre(root, preOut);
        collectIn(root, inOut);
        assert(preOut == pre);
        assert(inOut == in);
        deleteTree(root);
    }

    // Test 6: Larger random-like tree (from snippet example 3)
    {
        std::vector<int> pre = {0, 1, 3, 4, 2, 5, 7, 8, 6, 10};
        std::vector<int> in = {3, 1, 4, 0, 7, 5, 8, 2, 6, 10};
        TreeNode* root = buildTreeFromPreIn(pre, in);
        std::vector<int> preOut, inOut;
        collectPre(root, preOut);
        collectIn(root, inOut);
        assert(preOut == pre);
        assert(inOut == in);
        deleteTree(root);
    }

    return 0;
}
