Write a C++ function `vector<vector<int>> levelOrderTraversal(TreeNode* root)` that performs a level-order (breadth-first) traversal of a binary tree and returns a vector of vectors, where each inner vector contains the node values at that depth, in left-to-right order. The function must handle an empty tree (return an empty vector), and must not modify the tree. You may assume `TreeNode` is defined as in the snippet, with `int val`, and `TreeNode *left`, `TreeNode *right` pointers, and constructors. Test with both a skewed tree and a balanced tree, including cases where some nodes have only one child.
#include <cassert>
#include <vector>

// TreeNode definition and levelOrderTraversal function would be here.

int main() {
    // Test 1: Empty tree
    TreeNode* empty = nullptr;
    assert(levelOrderTraversal(empty) == std::vector<std::vector<int>>{});

    // Test 2: Single node
    TreeNode single(5);
    assert(levelOrderTraversal(&single) == std::vector<std::vector<int>>{{5}});

    // Test 3: Balanced tree:      1
    //                            / \
    //                           2   3
    //                          / \   \
    //                         4   5   6
    TreeNode n2(2), n3(3), n4(4), n5(5), n6(6);
    TreeNode n1(1, &n2, &n3);
    n2.left = &n4; n2.right = &n5;
    n3.right = &n6;
    std::vector<std::vector<int>> expected3 = {{1}, {2, 3}, {4, 5, 6}};
    assert(levelOrderTraversal(&n1) == expected3);

    // Test 4: Left-skewed tree: 10 -> 9 -> 8 -> 7
    TreeNode n8(8), n7(7);
    TreeNode n9(9, &n8, nullptr);
    TreeNode n10(10, &n9, nullptr);
    n8.left = &n7;
    std::vector<std::vector<int>> expected4 = {{10}, {9}, {8}, {7}};
    assert(levelOrderTraversal(&n10) == expected4);

    // Test 5: Tree with missing right child at second level
    //       100
    //       /
    //      50
    //       \
    //       25
    TreeNode n25(25);
    TreeNode n50(50, nullptr, &n25);
    TreeNode n100(100, &n50, nullptr);
    std::vector<std::vector<int>> expected5 = {{100}, {50}, {25}};
    assert(levelOrderTraversal(&n100) == expected5);

    return 0;
}
#include <vector>
#include <queue>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Perform level-order (breadth-first) traversal of a binary tree.
// Returns a vector of vectors, where each inner vector contains values at that depth.
std::vector<std::vector<int>> levelOrderTraversal(const TreeNode* root) {
    std::vector<std::vector<int>> result;
    if (root == nullptr) {
        return result;
    }

    std::queue<const TreeNode*> nodeQueue;
    nodeQueue.push(root);

    while (!nodeQueue.empty()) {
        int levelSize = static_cast<int>(nodeQueue.size());
        std::vector<int> currentLevel;
        currentLevel.reserve(levelSize);

        for (int i = 0; i < levelSize; ++i) {
            const TreeNode* current = nodeQueue.front();
            nodeQueue.pop();
            currentLevel.push_back(current->val);

            if (current->left != nullptr) {
                nodeQueue.push(current->left);
            }
            if (current->right != nullptr) {
                nodeQueue.push(current->right);
            }
        }

        result.push_back(currentLevel);
    }

    return result;
}
// The solution uses a standard level-order traversal with a queue. Start by checking if `root` is null; if so, return an empty vector. Otherwise, push the root into a queue. While the queue is not empty, record the current queue size (`levelSize`) — this equals the number of nodes at the current depth. Process exactly that many nodes: pop each, append its value to a temporary vector for that level, and push its non-null children (left then right) into the queue for the next level. After finishing the level, push the temporary vector into the result. This approach ensures each node is visited exactly once, and each level is grouped correctly. Edge cases include an empty tree (immediate return), a tree with only one node, and nodes with missing children (skip null children). Time complexity is O(n), where n is the number of nodes, because each node is enqueued and dequeued once. Space complexity is O(n) for the queue in the worst case (e.g., a complete tree’s last level), plus O(n) for the output vector, so overall O(n).
