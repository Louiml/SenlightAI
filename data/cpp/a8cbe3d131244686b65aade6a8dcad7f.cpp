/*
Write a C++ function `vector<vector<int>> collectLevelOrder(TreeNode* root)` that returns a 2D vector where each inner vector contains the values of all nodes at that depth in the binary tree, in left-to-right order. The tree is given by a standard `TreeNode` struct with integer `val` and `left`/`right` children pointers. The tree may be empty (root is `nullptr`), contain only one node, or be completely unbalanced (e.g., a chain). The function must process the tree level by level (breadth-first) and not rely on recursion. The returned vector should have one subvector per level, with levels appearing from root to deepest. If the tree is empty, return an empty vector.
*/

#include <vector>
#include <queue>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Collects node values level by level (BFS) from a binary tree.
// Returns an empty vector if the tree is empty.
std::vector<std::vector<int>> collectLevelOrder(const TreeNode* root) {
    std::vector<std::vector<int>> result;
    if (root == nullptr) {
        return result;
    }

    std::queue<const TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        std::size_t levelSize = q.size();
        std::vector<int> currentLevel;
        currentLevel.reserve(levelSize);

        for (std::size_t i = 0; i < levelSize; ++i) {
            const TreeNode* node = q.front();
            q.pop();
            currentLevel.push_back(node->val);

            if (node->left != nullptr) {
                q.push(node->left);
            }
            if (node->right != nullptr) {
                q.push(node->right);
            }
        }

        result.push_back(std::move(currentLevel));
    }

    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Empty tree
    {
        TreeNode* root = nullptr;
        assert(collectLevelOrder(root).empty());
    }

    // Test 2: Single node
    {
        TreeNode* root = new TreeNode(5);
        std::vector<std::vector<int>> expected = {{5}};
        assert(collectLevelOrder(root) == expected);
        delete root;
    }

    // Test 3: Complete balanced tree of height 2
    {
        TreeNode* root = new TreeNode(1,
            new TreeNode(2, new TreeNode(4), new TreeNode(5)),
            new TreeNode(3, new TreeNode(6), new TreeNode(7)));
        std::vector<std::vector<int>> expected = {{1}, {2,3}, {4,5,6,7}};
        assert(collectLevelOrder(root) == expected);
        // cleanup (simplified for test; actual code uses smart pointers)
        delete root->left->left;
        delete root->left->right;
        delete root->right->left;
        delete root->right->right;
        delete root->left;
        delete root->right;
        delete root;
    }

    // Test 4: Left-skewed tree (chain of 3 nodes)
    {
        TreeNode* root = new TreeNode(10,
            new TreeNode(20, new TreeNode(30), nullptr), nullptr);
        std::vector<std::vector<int>> expected = {{10}, {20}, {30}};
        assert(collectLevelOrder(root) == expected);
        delete root->left->left;
        delete root->left;
        delete root;
    }

    // Test 5: Tree with missing right child at some levels
    {
        TreeNode* root = new TreeNode(7,
            new TreeNode(8, new TreeNode(9), nullptr),
            new TreeNode(10));
        std::vector<std::vector<int>> expected = {{7}, {8,10}, {9}};
        assert(collectLevelOrder(root) == expected);
        delete root->left->left;
        delete root->left;
        delete root->right;
        delete root;
    }

    // Test 6: Larger tree to check ordering
    {
        TreeNode* root = new TreeNode(1,
            new TreeNode(2),
            new TreeNode(3,
                new TreeNode(4),
                new TreeNode(5)));
        std::vector<std::vector<int>> expected = {{1}, {2,3}, {4,5}};
        assert(collectLevelOrder(root) == expected);
        delete root->left;
        delete root->right->left;
        delete root->right->right;
        delete root->right;
        delete root;
    }

    // Test 7: All null children (just root)
    {
        TreeNode* root = new TreeNode(0);
        std::vector<std::vector<int>> expected = {{0}};
        assert(collectLevelOrder(root) == expected);
        delete root;
    }

    return 0;
}

// The core algorithm is a classic breadth-first traversal using a queue. Start by checking if `root` is `nullptr`; if so, return an empty `vector<vector<int>>`. Otherwise, push the root node into a `queue<TreeNode*>`. In each outer loop iteration, capture the current queue size (`levelSize`) which tells exactly how many nodes belong to the current level. Pop exactly `levelSize` nodes, appending each node's `val` to a temporary `vector<int>` for that level, and push that node's non-null left and right children into the queue for the next level. After processing the current level, push the temporary vector into the result. Continue until the queue is empty. Edge cases include an empty tree (handled at the start), a single-node tree (produces one level with one value), and a skewed tree (each level has exactly one node, so the result will have as many subvectors as nodes). The time complexity is O(n) because each node is enqueued and dequeued exactly once. The space complexity is O(n) for the queue in the worst case (a complete binary tree where the last level contains about n/2 nodes) plus the space of the result itself, which is also O(n) (or O(levels) if ignoring values for queue). We do not modify the tree, so we can pass root by const pointer, though the `TreeNode` structure itself is not const-corrected here.
