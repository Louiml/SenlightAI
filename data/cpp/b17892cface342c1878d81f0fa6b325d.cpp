/*
Write a C++ function `zigzagLevelOrder` that takes a binary tree's root node (a `TreeNode*`) and returns a `std::vector<std::vector<int>>` containing the node values in zigzag level order: traverse left-to-right on odd levels (1-indexed) and right-to-left on even levels. The tree may be empty (root is `nullptr`), and nodes store integer values. The function should not modify the tree, only read it. The result must be a vector of levels, where each level is a vector of integers in the required order.
*/

#include <vector>
#include <queue>
#include <algorithm>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Return node values in zigzag level order (left-to-right then right-to-left alternating).
std::vector<std::vector<int>> zigzagLevelOrder(const TreeNode* root) {
    std::vector<std::vector<int>> result;
    if (root == nullptr) {
        return result;
    }

    std::queue<const TreeNode*> q;
    q.push(root);
    bool leftToRight = true; // start with left-to-right on level 1

    while (!q.empty()) {
        int levelSize = q.size();
        std::vector<int> level;
        level.reserve(levelSize);

        for (int i = 0; i < levelSize; ++i) {
            const TreeNode* node = q.front();
            q.pop();
            level.push_back(node->val);

            if (node->left != nullptr) {
                q.push(node->left);
            }
            if (node->right != nullptr) {
                q.push(node->right);
            }
        }

        if (!leftToRight) {
            std::reverse(level.begin(), level.end());
        }

        result.push_back(std::move(level));
        leftToRight = !leftToRight;
    }

    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Empty tree
    {
        std::vector<std::vector<int>> expected = {};
        assert(zigzagLevelOrder(nullptr) == expected);
    }

    // Test 2: Single node
    {
        TreeNode node(5);
        std::vector<std::vector<int>> expected = {{5}};
        assert(zigzagLevelOrder(&node) == expected);
    }

    // Test 3: Two-level tree
    //        1
    //       / \
    //      2   3
    {
        TreeNode node3(3);
        TreeNode node2(2);
        TreeNode root(1, &node2, &node3);
        std::vector<std::vector<int>> expected = {{1}, {3, 2}}; // level 2 right-to-left
        assert(zigzagLevelOrder(&root) == expected);
    }

    // Test 4: Three-level tree
    //        1
    //       / \
    //      2   3
    //     / \   \
    //    4   5   6
    {
        TreeNode node6(6);
        TreeNode node5(5);
        TreeNode node4(4);
        TreeNode node3(3, nullptr, &node6);
        TreeNode node2(2, &node4, &node5);
        TreeNode root(1, &node2, &node3);
        std::vector<std::vector<int>> expected = {{1}, {3, 2}, {4, 5, 6}}; // level 3 left-to-right
        assert(zigzagLevelOrder(&root) == expected);
    }

    // Test 5: Left-skewed tree
    //  1
    // /
    // 2
    // /
    // 3
    {
        TreeNode node3(3);
        TreeNode node2(2, &node3, nullptr);
        TreeNode root(1, &node2, nullptr);
        std::vector<std::vector<int>> expected = {{1}, {2}, {3}};
        assert(zigzagLevelOrder(&root) == expected);
    }

    // Test 6: Right-skewed tree
    // 1
    //  \
    //   2
    //    \
    //     3
    {
        TreeNode node3(3);
        TreeNode node2(2, nullptr, &node3);
        TreeNode root(1, nullptr, &node2);
        std::vector<std::vector<int>> expected = {{1}, {2}, {3}};
        assert(zigzagLevelOrder(&root) == expected);
    }

    // Test 7: Complete full binary tree with 7 nodes
    //        1
    //       / \
    //      2   3
    //     / \ / \
    //    4  5 6  7
    {
        TreeNode node7(7);
        TreeNode node6(6);
        TreeNode node5(5);
        TreeNode node4(4);
        TreeNode node3(3, &node6, &node7);
        TreeNode node2(2, &node4, &node5);
        TreeNode root(1, &node2, &node3);
        std::vector<std::vector<int>> expected = {{1}, {3, 2}, {4, 5, 6, 7}};
        assert(zigzagLevelOrder(&root) == expected);
    }

    return 0;
}

// The solution uses a breadth-first search (BFS) with a queue to visit nodes level by level. Start by pushing the root if it is non-null; if the root is null, return an empty vector. For each level, record the number of nodes currently in the queue (`n`), then pop exactly `n` nodes, collecting their values into a temporary level vector, and push their left and right children (if any) for the next level. After processing all nodes of the current level, if the level number is even (1-indexed), reverse the level vector to achieve zigzag order; otherwise keep it as is. Then append the level to the result and increment the level counter. Edge cases include an empty tree (returns empty result) and a tree with a single node (single level, left-to-right). Time complexity is O(N) where N is the number of nodes, because each node is enqueued and dequeued exactly once, and each level reversal costs O(size of level) in total O(N). Space complexity is O(W) for the queue, where W is the maximum width of the tree, plus O(N) for the output vector itself.
