Given two integer arrays `preorder` and `inorder` that represent the preorder and inorder traversals of a binary tree, write a C++ function `buildTreeFromTraversals` that reconstructs and returns the root of the original binary tree. The arrays contain unique positive integers, are non-empty, and are guaranteed to be valid traversals of the same tree. The function should use the standard recursive divide-and-conquer approach with an unordered map to find root positions in constant time. The tree nodes are defined as `struct TreeNode` with `int val`, `TreeNode* left`, and `TreeNode* right` members, and a constructor that initializes `val` and sets both children to `nullptr`. Ensure the function handles the base case where the subtree range is empty, and that it correctly builds each subtree by consuming the preorder sequence in order. The solution must not allocate extra memory beyond the recursion stack and the map used for indexing.

// The algorithm reconstructs the binary tree by leveraging the properties of preorder and inorder traversals. In preorder traversal, the first element is always the root of the current subtree. In inorder traversal, the root divides the sequence into the left subtree (elements to the left of the root) and the right subtree (elements to the right). We use a global or helper recursion index `idx` that starts at 0 and increments each time we create a node, ensuring we consume preorder elements in the correct order. We preprocess the inorder array into an unordered map `mp` that maps each value to its index, allowing O(1) lookup for the root position. The recursion function takes the preorder array, the inorder array, and the current range `[st, en]` within the inorder array. If `st > en`, the subtree is empty, so we return `nullptr`. Otherwise, we take `pre[idx]` as the root value, increment `idx`, create a new `TreeNode`, find its position `pos` in the inorder array using the map, then recursively build the left subtree with range `[st, pos-1]` and the right subtree with range `[pos+1, en]`. This works because the preorder traversal ensures that after processing the root, the next elements correspond to the left subtree's preorder followed by the right subtree's preorder. Edge cases include a single-node tree where both `st == en` and `pos == st`, and trees with empty left or right subtrees where `st > pos-1` or `pos+1 > en`. The time complexity is O(n) because we visit each node once and do O(1) work per node (map lookup and recursion). The space complexity is O(n) for the recursion stack in the worst case (skewed tree) and O(n) for the unordered map.

#include <unordered_map>
#include <vector>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
private:
    std::unordered_map<int, int> inorderIndex;
    int preorderIdx;

    TreeNode* buildSubtree(const std::vector<int>& preorder, const std::vector<int>& inorder, int st, int en) {
        if (st > en) {
            return nullptr;
        }

        int rootVal = preorder[preorderIdx++];
        TreeNode* root = new TreeNode(rootVal);
        int pos = inorderIndex.at(rootVal);

        root->left = buildSubtree(preorder, inorder, st, pos - 1);
        root->right = buildSubtree(preorder, inorder, pos + 1, en);

        return root;
    }

public:
    TreeNode* buildTree(const std::vector<int>& preorder, const std::vector<int>& inorder) {
        inorderIndex.clear();
        preorderIdx = 0;
        for (int i = 0; i < static_cast<int>(inorder.size()); ++i) {
            inorderIndex[inorder[i]] = i;
        }
        return buildSubtree(preorder, inorder, 0, static_cast<int>(inorder.size()) - 1);
    }
};

#include <cassert>
#include <vector>
#include <functional>

// Helper to check if two trees are identical
bool sameTree(TreeNode* a, TreeNode* b) {
    if (a == nullptr || b == nullptr) return a == b;
    return a->val == b->val && sameTree(a->left, b->left) && sameTree(a->right, b->right);
}

// Helper to delete tree and avoid leaks
void deleteTree(TreeNode* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

int main() {
    Solution sol;

    // Test 1: Simple tree with left and right children
    {
        std::vector<int> pre = {3, 9, 20, 15, 7};
        std::vector<int> in = {9, 3, 15, 20, 7};
        TreeNode* root = sol.buildTree(pre, in);
        assert(root->val == 3);
        assert(root->left->val == 9);
        assert(root->right->val == 20);
        assert(root->right->left->val == 15);
        assert(root->right->right->val == 7);
        deleteTree(root);
    }

    // Test 2: Single node
    {
        std::vector<int> pre = {5};
        std::vector<int> in = {5};
        TreeNode* root = sol.buildTree(pre, in);
        assert(root->val == 5);
        assert(root->left == nullptr);
        assert(root->right == nullptr);
        deleteTree(root);
    }

    // Test 3: Left-skewed tree (only left children)
    {
        std::vector<int> pre = {1, 2, 3};
        std::vector<int> in = {3, 2, 1};
        TreeNode* root = sol.buildTree(pre, in);
        assert(root->val == 1);
        assert(root->left->val == 2);
        assert(root->left->left->val == 3);
        assert(root->left->right == nullptr);
        assert(root->right == nullptr);
        deleteTree(root);
    }

    // Test 4: Right-skewed tree (only right children)
    {
        std::vector<int> pre = {1, 2, 3};
        std::vector<int> in = {1, 2, 3};
        TreeNode* root = sol.buildTree(pre, in);
        assert(root->val == 1);
        assert(root->right->val == 2);
        assert(root->right->right->val == 3);
        assert(root->left == nullptr);
        deleteTree(root);
    }

    // Test 5: Tree with missing right child at some level
    {
        std::vector<int> pre = {4, 1, 2, 3};
        std::vector<int> in = {1, 2, 3, 4};
        TreeNode* root = sol.buildTree(pre, in);
        assert(root->val == 4);
        assert(root->left->val == 1);
        assert(root->left->right->val == 2);
        assert(root->left->right->right->val == 3);
        assert(root->right == nullptr);
        deleteTree(root);
    }

    // Test 6: Larger balanced tree
    {
        std::vector<int> pre = {10, 5, 1, 7, 40, 50};
        std::vector<int> in = {1, 5, 7, 10, 40, 50};
        TreeNode* root = sol.buildTree(pre, in);
        assert(root->val == 10);
        assert(root->left->val == 5);
        assert(root->left->left->val == 1);
        assert(root->left->right->val == 7);
        assert(root->right->val == 40);
        assert(root->right->right->val == 50);
        deleteTree(root);
    }

    // Test 7: Verify with a manually built tree
    {
        TreeNode* expected = new TreeNode(8);
        expected->left = new TreeNode(3);
        expected->right = new TreeNode(10);
        expected->left->left = new TreeNode(1);
        expected->left->right = new TreeNode(6);
        expected->left->right->left = new TreeNode(4);
        expected->left->right->right = new TreeNode(7);
        expected->right->right = new TreeNode(14);
        expected->right->right->left = new TreeNode(13);

        std::vector<int> pre = {8, 3, 1, 6, 4, 7, 10, 14, 13};
        std::vector<int> in = {1, 3, 4, 6, 7, 8, 10, 13, 14};
        TreeNode* actual = sol.buildTree(pre, in);
        assert(sameTree(expected, actual));

        deleteTree(expected);
        deleteTree(actual);
    }

    return 0;
}
