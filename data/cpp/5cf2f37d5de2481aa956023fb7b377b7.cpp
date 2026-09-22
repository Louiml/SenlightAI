Write a C++ function `std::vector<std::vector<int>> levelOrderTraversal(TreeNode* root)` that returns a vector of vectors containing the values of a binary tree in level order (also known as breadth-first traversal). Each inner vector represents one level of the tree, ordered from top (root) to bottom, and within each level values are ordered from left to right. You may assume the `TreeNode` struct is defined with fields `int val; TreeNode *left; TreeNode *right;` and a constructor `TreeNode(int x)`. The function must handle an empty tree (return an empty vector) and unbalanced trees where some branches are shorter than others. Do not use recursion—implement the traversal iteratively using a queue to achieve true level-order processing.
// The solution uses a standard breadth-first search (BFS) approach with a queue. We start by pushing the root node into the queue. Then, while the queue is not empty, we process all nodes currently in the queue—these form the current level. We record their values into a `vector<int>`, and for each node we push its left and right children (if they exist) into the queue for the next level. After processing the full current level, we append the collected values to the result vector and continue. Edge cases: empty tree returns an empty outer vector; a tree with only a root returns one inner vector with a single element; unbalanced trees are naturally handled because we process exactly the nodes present at each level. Time complexity is `O(n)` where `n` is the number of nodes, since each node is visited exactly once. Space complexity is `O(w)` where `w` is the maximum width of the tree (the maximum number of nodes at any level), which in the worst case (a complete binary tree) is `O(n/2) = O(n)` for the queue, plus the output vector itself which holds `n` integers.
#include <vector>
#include <queue>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Perform level-order (BFS) traversal of a binary tree iteratively.
std::vector<std::vector<int>> levelOrderTraversal(TreeNode* root) {
    std::vector<std::vector<int>> result;
    if (root == nullptr) {
        return result;
    }

    std::queue<TreeNode*> nodeQueue;
    nodeQueue.push(root);

    while (!nodeQueue.empty()) {
        int levelSize = static_cast<int>(nodeQueue.size());
        std::vector<int> currentLevel;
        currentLevel.reserve(levelSize);

        for (int i = 0; i < levelSize; ++i) {
            TreeNode* node = nodeQueue.front();
            nodeQueue.pop();
            currentLevel.push_back(node->val);

            if (node->left != nullptr) {
                nodeQueue.push(node->left);
            }
            if (node->right != nullptr) {
                nodeQueue.push(node->right);
            }
        }
        result.push_back(currentLevel);
    }

    return result;
}
#include <cassert>
#include <vector>

// Main test harness
int main() {
    // Test 1: Empty tree
    TreeNode* empty = nullptr;
    std::vector<std::vector<int>> result = levelOrderTraversal(empty);
    assert(result.empty());

    // Test 2: Single node tree
    TreeNode* single = new TreeNode(5);
    result = levelOrderTraversal(single);
    assert((result == std::vector<std::vector<int>>{{5}}));

    // Test 3: Complete binary tree
    //        1
    //       / \
    //      2   3
    //     / \ / \
    //    4  5 6  7
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(7);
    result = levelOrderTraversal(root);
    assert((result == std::vector<std::vector<int>>{{1}, {2, 3}, {4, 5, 6, 7}}));

    // Test 4: Unbalanced tree (only left children)
    //        1
    //       /
    //      2
    //     /
    //    3
    TreeNode* leftChain = new TreeNode(1);
    leftChain->left = new TreeNode(2);
    leftChain->left->left = new TreeNode(3);
    result = levelOrderTraversal(leftChain);
    assert((result == std::vector<std::vector<int>>{{1}, {2}, {3}}));

    // Test 5: Tree with missing right child at one level
    //       10
    //      /  \
    //     20   30
    //    /
    //   40
    TreeNode* root2 = new TreeNode(10);
    root2->left = new TreeNode(20);
    root2->right = new TreeNode(30);
    root2->left->left = new TreeNode(40);
    result = levelOrderTraversal(root2);
    assert((result == std::vector<std::vector<int>>{{10}, {20, 30}, {40}}));

    // Test 6: Tree with values repeated
    //       7
    //      / \
    //     7   7
    //    /
    //   7
    TreeNode* root3 = new TreeNode(7);
    root3->left = new TreeNode(7);
    root3->right = new TreeNode(7);
    root3->left->left = new TreeNode(7);
    result = levelOrderTraversal(root3);
    assert((result == std::vector<std::vector<int>>{{7}, {7, 7}, {7}}));

    // Clean up (optional; not required for assert tests)
    // For simplicity, we skip explicit deletion of allocated nodes.

    return 0;
}
