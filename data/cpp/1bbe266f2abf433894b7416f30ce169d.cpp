// Given two integer vectors `preorder` and `postorder` that represent the pre-order and post-order traversal of a binary tree (with unique node values), write a C++ function `buildTreeFromPrePost` that reconstructs and returns the root pointer of the binary tree. The tree is not necessarily full (i.e., some nodes may have only one child), and the traversals are guaranteed to be valid and correspond to the same tree. The function should handle empty input by returning `nullptr`. The node structure is defined as `TreeNode` with fields `val`, `left`, and `right`. The solution must not use global or static state; it should be a standalone function.
// The key insight is that in a pre-order traversal, the first element is the root, and the second element (if it exists) is the root of the left subtree. In a post-order traversal, the last element is the root. By locating the left subtree's root value (which is `preorder[preStart + 1]`) within the post-order range, we can determine the size of the left subtree: `leftSize = (index in postorder of that value) - postStart + 1`. Then, recursively build the left subtree using the corresponding slice of pre-order and post-order, and the right subtree using the remaining elements. Edge cases: if `preStart == preEnd` (a single node) return that node; if the left subtree doesn't exist (i.e., `preStart + 1` equals `preEnd` and the postorder index of that value is `postEnd`), then the right subtree is empty. However, because the algorithm always assumes the second pre-order element is the left root, it correctly handles missing right or left subtrees because the recursive calls will return `nullptr` when ranges are invalid. To improve efficiency, we can precompute a hash map from value to its index in the post-order vector, avoiding a linear search each time. Time complexity is O(n) due to the hash map and each recursive call does constant work, with O(n) auxiliary space for the map and recursion stack (depth O(n) in worst case for skewed trees, but typically O(log n) for balanced trees).
#include <vector>
#include <unordered_map>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper recursive function that builds the tree from given ranges.
TreeNode* buildHelper(const std::vector<int>& preorder, int preStart, int preEnd,
                      const std::vector<int>& postorder, int postStart, int postEnd,
                      const std::unordered_map<int, int>& postIndex) {
    if (preStart > preEnd || postStart > postEnd) {
        return nullptr;
    }

    // The first pre-order element is the root.
    TreeNode* node = new TreeNode(preorder[preStart]);

    // If this is a leaf node (single element in both ranges), return it.
    if (preStart == preEnd) {
        return node;
    }

    // The second pre-order element is the root of the left subtree.
    int leftRootVal = preorder[preStart + 1];
    int leftRootPostIdx = postIndex.at(leftRootVal);

    // Number of nodes in the left subtree.
    int leftSize = leftRootPostIdx - postStart + 1;

    // Recursively build left subtree.
    node->left = buildHelper(preorder, preStart + 1, preStart + leftSize,
                             postorder, postStart, leftRootPostIdx, postIndex);

    // Recursively build right subtree (note: postEnd - 1 excludes the current root from postorder).
    node->right = buildHelper(preorder, preStart + leftSize + 1, preEnd,
                              postorder, leftRootPostIdx + 1, postEnd - 1, postIndex);

    return node;
}

// Main function to reconstruct a binary tree from preorder and postorder traversals.
TreeNode* buildTreeFromPrePost(const std::vector<int>& preorder, const std::vector<int>& postorder) {
    if (preorder.empty() || postorder.empty() || preorder.size() != postorder.size()) {
        return nullptr;
    }

    // Build a map from value to its index in postorder for O(1) lookup.
    std::unordered_map<int, int> postIndex;
    for (int i = 0; i < static_cast<int>(postorder.size()); ++i) {
        postIndex[postorder[i]] = i;
    }

    return buildHelper(preorder, 0, static_cast<int>(preorder.size()) - 1,
                       postorder, 0, static_cast<int>(postorder.size()) - 1,
                       postIndex);
}
#include <cassert>
#include <vector>
#include <queue>

// Helper to check tree structure via level-order traversal (including nulls).
std::vector<int> levelOrderWithNulls(TreeNode* root) {
    std::vector<int> result;
    if (!root) return result;
    std::queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* cur = q.front();
        q.pop();
        if (cur) {
            result.push_back(cur->val);
            q.push(cur->left);
            q.push(cur->right);
        } else {
            result.push_back(-1); // using -1 for null nodes (assume values positive/distinct)
        }
    }
    return result;
}

// Helper to delete tree and free memory.
void deleteTree(TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    // Test 1: Example from problem statement
    std::vector<int> pre1 = {1, 2, 3, 4, 5, 6, 7};
    std::vector<int> post1 = {4, 5, 2, 6, 7, 3, 1};
    TreeNode* root1 = buildTreeFromPrePost(pre1, post1);
    // Expected level order: 1, 2, 3, 4, 5, 6, 7
    std::vector<int> expected1 = {1, 2, 3, 4, 5, 6, 7};
    assert(levelOrderWithNulls(root1) == expected1);
    deleteTree(root1);

    // Test 2: Single node tree
    std::vector<int> pre2 = {5};
    std::vector<int> post2 = {5};
    TreeNode* root2 = buildTreeFromPrePost(pre2, post2);
    std::vector<int> expected2 = {5};
    assert(levelOrderWithNulls(root2) == expected2);
    deleteTree(root2);

    // Test 3: Left-skewed tree (each node has only left child)
    std::vector<int> pre3 = {1, 2, 3, 4};
    std::vector<int> post3 = {4, 3, 2, 1};
    TreeNode* root3 = buildTreeFromPrePost(pre3, post3);
    // Level order (with nulls for missing right children): 1, 2, -1, 3, -1, -1, -1, 4, -1, ... (we only test first few)
    std::vector<int> expected3 = {1, 2, -1, 3, -1, -1, -1, 4, -1, -1, -1, -1, -1, -1, -1};
    // But our levelOrderWithNulls will include trailing nulls; we compare first 15 elements explicitly.
    std::vector<int> actual3 = levelOrderWithNulls(root3);
    assert(actual3.size() >= 15);
    for (int i = 0; i < 15; ++i) {
        assert(actual3[i] == expected3[i]);
    }
    deleteTree(root3);

    // Test 4: Right-skewed tree (each node has only right child)
    std::vector<int> pre4 = {1, 2, 3};
    std::vector<int> post4 = {3, 2, 1};
    TreeNode* root4 = buildTreeFromPrePost(pre4, post4);
    // Level order: 1, -1, 2, -1, -1, -1, 3
    std::vector<int> expected4 = {1, -1, 2, -1, -1, -1, 3};
    std::vector<int> actual4 = levelOrderWithNulls(root4);
    assert(actual4.size() >= 7);
    for (int i = 0; i < 7; ++i) {
        assert(actual4[i] == expected4[i]);
    }
    deleteTree(root4);

    // Test 5: Empty input
    std::vector<int> pre5;
    std::vector<int> post5;
    TreeNode* root5 = buildTreeFromPrePost(pre5, post5);
    assert(root5 == nullptr);

    // Test 6: Full binary tree
    std::vector<int> pre6 = {1, 2, 4, 5, 3, 6, 7};
    std::vector<int> post6 = {4, 5, 2, 6, 7, 3, 1};
    TreeNode* root6 = buildTreeFromPrePost(pre6, post6);
    std::vector<int> expected6 = {1, 2, 3, 4, 5, 6, 7};
    assert(levelOrderWithNulls(root6) == expected6);
    deleteTree(root6);

    return 0;
}
