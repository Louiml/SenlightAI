// Given a binary tree where each node contains an integer `data` and pointers to its left and right children, write a C++ function `std::vector<int> levelOrderTraversal(const TreeNode* root)` that returns the node values in level order (breadth-first order), left to right at each level. The tree may be empty (nullptr root), and negative values are allowed. Do not modify the tree; the function must work with `const` access. Your implementation should not rely on recursion but use a queue to process nodes level by level.

Level-order traversal visits nodes by depth: first the root, then all its immediate children, then their children, etc., from left to right. The standard approach uses a queue (e.g., `std::queue<const TreeNode*>`). Start by pushing the root if it exists. While the queue is not empty, pop the front node, record its value, and push its left child (if non-null) then its right child (if non-null). Because we process nodes in FIFO order, this ensures nodes at each depth are visited left-to-right. Edge cases include an empty tree (return empty vector) and a tree with only one node (returns that node). Time complexity is O(n) where n is the number of nodes, since each node is enqueued and dequeued exactly once. Space complexity is O(w) for the queue, where w is the maximum width of the tree (at most n in the worst case, e.g., a complete tree’s last level). The solution uses `const` pointers to avoid modifying the tree.

#include <vector>
#include <queue>

// Definition of TreeNode (provided by the task context)
struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
};

// Returns the values of the tree nodes in level order (left to right per level).
std::vector<int> levelOrderTraversal(const TreeNode* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result;
    }

    std::queue<const TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        const TreeNode* current = q.front();
        q.pop();
        result.push_back(current->data);

        if (current->left != nullptr) {
            q.push(current->left);
        }
        if (current->right != nullptr) {
            q.push(current->right);
        }
    }

    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Build a sample tree: 
    //        1
    //       / \
    //      2   3
    //     / \   \
    //    4   5   6
    TreeNode* n1 = new TreeNode{1, nullptr, nullptr};
    TreeNode* n2 = new TreeNode{2, nullptr, nullptr};
    TreeNode* n3 = new TreeNode{3, nullptr, nullptr};
    TreeNode* n4 = new TreeNode{4, nullptr, nullptr};
    TreeNode* n5 = new TreeNode{5, nullptr, nullptr};
    TreeNode* n6 = new TreeNode{6, nullptr, nullptr};
    n1->left = n2;
    n1->right = n3;
    n2->left = n4;
    n2->right = n5;
    n3->right = n6;

    // Test level order on a non-empty tree
    std::vector<int> expected = {1, 2, 3, 4, 5, 6};
    assert(levelOrderTraversal(n1) == expected);

    // Test empty tree
    assert(levelOrderTraversal(nullptr).empty());

    // Test single node tree
    TreeNode* single = new TreeNode{42, nullptr, nullptr};
    assert(levelOrderTraversal(single) == std::vector<int>{42});

    // Test left-skewed tree: 1->2->3
    TreeNode* skew = new TreeNode{1, nullptr, nullptr};
    skew->left = new TreeNode{2, nullptr, nullptr};
    skew->left->left = new TreeNode{3, nullptr, nullptr};
    assert(levelOrderTraversal(skew) == std::vector<int>({1, 2, 3}));

    // Test right-skewed tree: 1->2->3
    TreeNode* rskew = new TreeNode{1, nullptr, nullptr};
    rskew->right = new TreeNode{2, nullptr, nullptr};
    rskew->right->right = new TreeNode{3, nullptr, nullptr};
    assert(levelOrderTraversal(rskew) == std::vector<int>({1, 2, 3}));

    // Test with negative values and unbalanced
    TreeNode* neg = new TreeNode{-1, nullptr, nullptr};
    neg->left = new TreeNode{-2, nullptr, nullptr};
    neg->right = new TreeNode{3, nullptr, nullptr};
    neg->left->right = new TreeNode{-4, nullptr, nullptr};
    assert(levelOrderTraversal(neg) == std::vector<int>({-1, -2, 3, -4}));

    // Cleanup (optional, not required for assert checks)
    // In real code, delete all nodes, but omitted for brevity.

    return 0;
}
