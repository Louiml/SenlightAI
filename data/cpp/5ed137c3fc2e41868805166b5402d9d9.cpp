Write a C++ function that takes a binary tree represented by its root node (where each node stores an integer `data` and has pointers to `left` and `right` children, with `nullptr` for absent children) and returns a `std::vector<int>` containing the maximum value on each level of the tree, in order from the root level (level 0) to the deepest level. The tree may be unbalanced, may have nodes with negative values, and may have only a root. The function should not modify the tree and must use a level-order (BFS) traversal to compute the result.
The solution uses a breadth-first search (BFS) traversal with a queue of node–level pairs. Start by pushing the root with level 0. While the queue is not empty, pop a pair, record the node’s value into a map keyed by level (or track the maximum per level in a vector as we go). To avoid using a map and extra memory for storing all values per level, we can directly update a vector where the index corresponds to the level: if the current level is beyond the vector size, push the node’s data; otherwise, update the existing entry with the maximum. This yields a compact representation. Important edge cases: empty tree (if root is `nullptr`, return an empty vector), and trees where some levels have only negative values (initialization must be with the first node value, not `INT_MIN`, which would accidentally mask valid negatives). Time complexity is O(N), where N is the number of nodes, because each node is visited once. Space complexity is O(W) for the queue, where W is the maximum width of the tree; for a skewed tree W=1, for a complete tree W can be up to N/2.
#include <vector>
#include <queue>
#include <algorithm>

// Definition for a binary tree node.
struct TreeNode {
    int data;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Returns the maximum value on each level of the binary tree, in level order from root to deepest.
std::vector<int> levelOrderMax(TreeNode* root) {
    std::vector<int> result;
    if (root == nullptr) return result;

    std::queue<std::pair<TreeNode*, int>> q;
    q.push({root, 0});

    while (!q.empty()) {
        auto [node, level] = q.front();
        q.pop();

        if (level == static_cast<int>(result.size())) {
            result.push_back(node->data);  // first node on this level
        } else {
            result[level] = std::max(result[level], node->data);
        }

        if (node->left) q.push({node->left, level + 1});
        if (node->right) q.push({node->right, level + 1});
    }
    return result;
}
#include <cassert>
#include <vector>
#include "solution.h"  // assuming the above code is in solution.h

int main() {
    // Test 1: single node
    TreeNode* root1 = new TreeNode(5);
    assert(levelOrderMax(root1) == std::vector<int>({5}));

    // Test 2: two-level full tree
    TreeNode* root2 = new TreeNode(1);
    root2->left = new TreeNode(3);
    root2->right = new TreeNode(2);
    assert(levelOrderMax(root2) == std::vector<int>({1, 3}));

    // Test 3: skewed left tree
    TreeNode* root3 = new TreeNode(-10);
    root3->left = new TreeNode(-5);
    root3->left->left = new TreeNode(-1);
    assert(levelOrderMax(root3) == std::vector<int>({-10, -5, -1}));

    // Test 4: unbalanced with negative and positive
    TreeNode* root4 = new TreeNode(0);
    root4->left = new TreeNode(-3);
    root4->right = new TreeNode(8);
    root4->left->left = new TreeNode(7);
    root4->right->right = new TreeNode(-2);
    assert(levelOrderMax(root4) == std::vector<int>({0, 8, 7}));

    // Test 5: null root
    assert(levelOrderMax(nullptr) == std::vector<int>());

    // Test 6: larger tree, all levels have multiple nodes
    TreeNode* root5 = new TreeNode(10);
    root5->left = new TreeNode(20);
    root5->right = new TreeNode(30);
    root5->left->left = new TreeNode(40);
    root5->left->right = new TreeNode(50);
    root5->right->left = new TreeNode(60);
    root5->right->right = new TreeNode(70);
    assert(levelOrderMax(root5) == std::vector<int>({10, 30, 70}));

    return 0;
}
