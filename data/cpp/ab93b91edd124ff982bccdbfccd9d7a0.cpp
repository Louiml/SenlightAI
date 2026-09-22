Write a C++ function that takes two binary tree root pointers as input and returns a new binary tree that is the merge of the two input trees. When two trees are merged, the value of each node in the resulting tree is the sum of the values of the corresponding nodes in the original trees (if a node exists in only one tree, the value is taken from that tree; if a node exists in neither, the merged tree has no node there). The function must not modify the input trees and must create a completely new tree structure. You may assume each tree node has an integer value, and left/right child pointers that can be `nullptr`. Provide the function signature as specified below.
#include <cassert>

int main() {
    // Helper to manually build a tree for testing (local function).
    // In a real program this would be separate; here we use a lambda for brevity.
    // But for simplicity, we'll build trees directly.

    // Test 1: Both trees null.
    {
        TreeNode* t1 = nullptr;
        TreeNode* t2 = nullptr;
        TreeNode* result = mergeTrees(t1, t2);
        assert(result == nullptr);
    }

    // Test 2: One tree null, other has a single node.
    {
        TreeNode* t1 = nullptr;
        TreeNode* t2 = new TreeNode(5);
        TreeNode* result = mergeTrees(t1, t2);
        assert(result != nullptr);
        assert(result->val == 5);
        assert(result->left == nullptr);
        assert(result->right == nullptr);
        delete result;
        delete t2;
    }

    // Test 3: Both trees single node.
    {
        TreeNode* t1 = new TreeNode(3);
        TreeNode* t2 = new TreeNode(4);
        TreeNode* result = mergeTrees(t1, t2);
        assert(result != nullptr);
        assert(result->val == 7);
        assert(result->left == nullptr);
        assert(result->right == nullptr);
        delete result;
        delete t1;
        delete t2;
    }

    // Test 4: More complex overlapping and non-overlapping nodes.
    {
        // Tree1:
        //       1
        //      / \
        //     3   2
        //    /
        //   5
        TreeNode* t1 = new TreeNode(1);
        t1->left = new TreeNode(3);
        t1->right = new TreeNode(2);
        t1->left->left = new TreeNode(5);

        // Tree2:
        //       2
        //      / \
        //     1   3
        //      \   \
        //       4   7
        TreeNode* t2 = new TreeNode(2);
        t2->left = new TreeNode(1);
        t2->right = new TreeNode(3);
        t2->left->right = new TreeNode(4);
        t2->right->right = new TreeNode(7);

        // Merged:
        //       3
        //      / \
        //     4   5
        //    / \   \
        //   5   4   7
        TreeNode* result = mergeTrees(t1, t2);
        assert(result != nullptr);
        assert(result->val == 3);
        assert(result->left != nullptr && result->left->val == 4);
        assert(result->right != nullptr && result->right->val == 5);
        assert(result->left->left != nullptr && result->left->left->val == 5);
        assert(result->left->right != nullptr && result->left->right->val == 4);
        assert(result->right->right != nullptr && result->right->right->val == 7);

        // Free memory (simple recursive deletion not shown; for test we just delete all direct nodes)
        // Note: In a real scenario you'd write a helper to delete trees recursively.
        // For this test, we just delete what we allocated for simplicity, but proper cleanup would be required.
        delete result->left->left;
        delete result->left->right;
        delete result->right->right;
        delete result->left;
        delete result->right;
        delete result;

        // Clean up original trees too.
        delete t1->left->left;
        delete t1->left;
        delete t1->right;
        delete t1;
        delete t2->left->right;
        delete t2->left;
        delete t2->right->right;
        delete t2->right;
        delete t2;
    }

    return 0;
}
#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Merge two binary trees by summing corresponding node values.
// Returns a newly allocated tree; the input trees are not modified.
TreeNode* mergeTrees(const TreeNode* t1, const TreeNode* t2) {
    // If both nodes are null, the merged node is null.
    if (t1 == nullptr && t2 == nullptr) {
        return nullptr;
    }

    // Sum the values of the two nodes, treating null as 0.
    int sum = (t1 ? t1->val : 0) + (t2 ? t2->val : 0);

    // Create the new node.
    TreeNode* merged = new TreeNode(sum);

    // Recursively merge left and right subtrees.
    merged->left  = mergeTrees(t1 ? t1->left : nullptr, t2 ? t2->left : nullptr);
    merged->right = mergeTrees(t1 ? t1->right : nullptr, t2 ? t2->right : nullptr);

    return merged;
}
// The solution uses a recursive approach. The base case occurs when both input nodes are `nullptr`, in which case the merged node is also `nullptr`. Otherwise, the value for the current merged node is the sum of the two input nodes' values (treating a `nullptr` node's value as 0). After creating the new node via `new`, the function recursively computes the merged left subtree and merged right subtree, passing the left children of both inputs (or `nullptr` if a child is missing), and similarly for the right children. This mirrors the structure of both trees simultaneously. Edge cases include one or both trees being empty (the entire tree), or one of the two roots being `nullptr`, which is handled naturally by the base case and the conditional access to child pointers. Since every node in the larger of the two trees is visited exactly once, the time complexity is \(O(n)\), where \(n\) is the number of nodes in the larger tree. The space complexity is \(O(h)\) for the recursion stack, where \(h\) is the height of the resulting tree (worst case \(O(n)\) for a skewed tree). The function creates a new node for every merged node, so the auxiliary space (excluding input trees) is \(O(n)\) as well. The implementation uses `const` correctness for the input pointers (they are `const TreeNode*`), but since we create new nodes, we return a non-const pointer. We also take care to avoid dereferencing `nullptr`.
