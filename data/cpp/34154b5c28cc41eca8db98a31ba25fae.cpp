// Write a C++ function named `bottomUpLevelOrder` that takes the root of a binary tree (defined with `struct TreeNode { int val; TreeNode *left; TreeNode *right; TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} };`) and returns a `std::vector<std::vector<int>>` containing the values of the nodes grouped by their level, but in bottom-up order (i.e., starting from the deepest level and ending at the root). The traversal within each level must be from left to right. Handle an empty tree by returning an empty vector. Use only standard library headers.

The solution performs a standard breadth-first search (BFS) using a queue. At each level, we capture the values of all nodes currently in the queue before processing the next level. To achieve bottom-up order, we can either collect levels in top-down order and then reverse the final vector, or we can push each level to the front (e.g., using `insert` at the beginning, though that is less efficient). The simplest approach is: if the root is null, return immediately. Otherwise, initialize a queue with the root. While the queue is not empty, determine the current level size (`q.size()`), then pop that many nodes, appending their values to a temporary `level` vector and enqueueing their non-null children. After finishing the level, append `level` to the result vector. After the loop, reverse the result vector using `std::reverse`. This correctly orders levels from leaf to root, and within each level, left-to-right order is preserved because we always enqueue left child before right child. Edge cases include an empty tree (immediate empty return) and a tree with only a root (result contains one level). Time complexity is O(n) where n is the number of nodes, since each node is visited exactly once. Space complexity is O(n) for the queue (in the worst case, the last level can contain about n/2 nodes) plus O(n) for the output vector.

#include <vector>
#include <queue>
#include <algorithm>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Return bottom-up level order traversal of the binary tree.
std::vector<std::vector<int>> bottomUpLevelOrder(const TreeNode* root) {
    std::vector<std::vector<int>> result;
    if (root == nullptr) {
        return result;
    }

    std::queue<const TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = static_cast<int>(q.size());
        std::vector<int> currentLevel;
        currentLevel.reserve(levelSize);

        for (int i = 0; i < levelSize; ++i) {
            const TreeNode* node = q.front();
            q.pop();
            currentLevel.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }

        result.push_back(currentLevel);
    }

    std::reverse(result.begin(), result.end());
    return result;
}

#include <cassert>
#include <vector>

// Assume the definition of TreeNode and bottomUpLevelOrder from above are included.

int main() {
    // Test 1: Empty tree.
    {
        TreeNode* root = nullptr;
        std::vector<std::vector<int>> expected = {};
        assert(bottomUpLevelOrder(root) == expected);
    }

    // Test 2: Single node.
    {
        TreeNode root(5);
        std::vector<std::vector<int>> expected = {{5}};
        assert(bottomUpLevelOrder(&root) == expected);
    }

    // Test 3: Two levels (root with two children).
    {
        TreeNode root(1);
        TreeNode left(2);
        TreeNode right(3);
        root.left = &left;
        root.right = &right;
        std::vector<std::vector<int>> expected = {{2, 3}, {1}};
        assert(bottomUpLevelOrder(&root) == expected);
    }

    // Test 4: Three levels, asymmetric tree.
    {
        TreeNode root(3);
        TreeNode n9(9);
        TreeNode n20(20);
        TreeNode n15(15);
        TreeNode n7(7);
        root.left = &n9;
        root.right = &n20;
        n20.left = &n15;
        n20.right = &n7;
        std::vector<std::vector<int>> expected = {{15, 7}, {9, 20}, {3}};
        assert(bottomUpLevelOrder(&root) == expected);
    }

    // Test 5: Skewed tree (right-only).
    {
        TreeNode root(1);
        TreeNode n2(2);
        TreeNode n3(3);
        root.right = &n2;
        n2.right = &n3;
        std::vector<std::vector<int>> expected = {{3}, {2}, {1}};
        assert(bottomUpLevelOrder(&root) == expected);
    }

    // Test 6: Skewed left-only tree.
    {
        TreeNode root(10);
        TreeNode n20(20);
        TreeNode n30(30);
        root.left = &n20;
        n20.left = &n30;
        std::vector<std::vector<int>> expected = {{30}, {20}, {10}};
        assert(bottomUpLevelOrder(&root) == expected);
    }

    // Test 7: Complete binary tree with 4 levels (values 1-15).
    {
        TreeNode nodes[15];
        for (int i = 0; i < 15; ++i) nodes[i] = TreeNode(i + 1);
        for (int i = 0; i < 7; ++i) {
            nodes[i].left = &nodes[2*i + 1];
            nodes[i].right = &nodes[2*i + 2];
        }
        std::vector<std::vector<int>> expected = {
            {8, 9, 10, 11, 12, 13, 14, 15},
            {4, 5, 6, 7},
            {2, 3},
            {1}
        };
        assert(bottomUpLevelOrder(&nodes[0]) == expected);
    }

    return 0;
}
