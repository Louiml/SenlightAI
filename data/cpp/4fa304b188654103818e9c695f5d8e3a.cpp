/*
Write a C++ function `vector<vector<int>> levelOrderTraversal(TreeNode* root)` that performs a level-order (breadth-first) traversal of a binary tree and returns a 2D vector where each inner vector contains the values of the nodes at a specific depth, from left to right. The binary tree node is defined as: `struct TreeNode { int val; TreeNode *left; TreeNode *right; TreeNode() : val(0), left(nullptr), right(nullptr) {} TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {} };`. The function must handle an empty tree (return an empty vector), correctly process trees of any shape (including unbalanced), and preserve the left‑to‑right order within each level. You may assume the node values are unique for testing purposes but the function should work with duplicates as well. Provide a complete implementation with necessary headers and comments.
*/

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

// Perform level-order traversal of a binary tree and return a vector of vectors,
// where each inner vector contains node values at that depth from left to right.
std::vector<std::vector<int>> levelOrderTraversal(TreeNode* root) {
    std::vector<std::vector<int>> result;
    if (root == nullptr) {
        return result;
    }

    std::queue<TreeNode*> nodeQueue;
    nodeQueue.push(root);

    while (!nodeQueue.empty()) {
        int levelSize = nodeQueue.size();
        std::vector<int> currentLevel;
        currentLevel.reserve(levelSize);

        for (int i = 0; i < levelSize; ++i) {
            TreeNode* current = nodeQueue.front();
            nodeQueue.pop();

            if (current->left != nullptr) {
                nodeQueue.push(current->left);
            }
            if (current->right != nullptr) {
                nodeQueue.push(current->right);
            }

            currentLevel.push_back(current->val);
        }

        result.push_back(currentLevel);
    }

    return result;
}

#include <cassert>

int main() {
    // Test 1: Empty tree
    TreeNode* empty = nullptr;
    assert(levelOrderTraversal(empty) == std::vector<std::vector<int>>{});

    // Test 2: Single node
    TreeNode single(1);
    assert(levelOrderTraversal(&single) == std::vector<std::vector<int>>{{1}});

    // Test 3: Full binary tree:        1
    //                                /   \
    //                               2     3
    //                              / \   / \
    //                             4   5 6   7
    TreeNode n4(4), n5(5), n6(6), n7(7);
    TreeNode n2(2, &n4, &n5);
    TreeNode n3(3, &n6, &n7);
    TreeNode n1(1, &n2, &n3);
    assert(levelOrderTraversal(&n1) == std::vector<std::vector<int>>{{1}, {2, 3}, {4, 5, 6, 7}});

    // Test 4: Left-skewed tree: 1 -> 2 -> 3
    TreeNode n3s(3);
    TreeNode n2s(2, &n3s, nullptr);
    TreeNode n1s(1, &n2s, nullptr);
    assert(levelOrderTraversal(&n1s) == std::vector<std::vector<int>>{{1}, {2}, {3}});

    // Test 5: Right-skewed tree: 1 -> 2 -> 3
    TreeNode m3(3);
    TreeNode m2(2, nullptr, &m3);
    TreeNode m1(1, nullptr, &m2);
    assert(levelOrderTraversal(&m1) == std::vector<std::vector<int>>{{1}, {2}, {3}});

    // Test 6: Tree with missing right child:       1
    //                                           /   \
    //                                          2     3
    //                                         / \
    //                                        4   5
    TreeNode r5(5);
    TreeNode r4(4);
    TreeNode r2(2, &r4, &r5);
    TreeNode r3(3);
    TreeNode r1(1, &r2, &r3);
    assert(levelOrderTraversal(&r1) == std::vector<std::vector<int>>{{1}, {2, 3}, {4, 5}});

    // Test 7: Tree with only left children:       1
    //                                             /
    //                                            2
    //                                           /
    //                                          3
    TreeNode c3(3);
    TreeNode c2(2, &c3, nullptr);
    TreeNode c1(1, &c2, nullptr);
    assert(levelOrderTraversal(&c1) == std::vector<std::vector<int>>{{1}, {2}, {3}});

    // Test 8: Tree with only right children:       1
    //                                               \
    //                                                2
    //                                                 \
    //                                                  3
    TreeNode b3(3);
    TreeNode b2(2, nullptr, &b3);
    TreeNode b1(1, nullptr, &b2);
    assert(levelOrderTraversal(&b1) == std::vector<std::vector<int>>{{1}, {2}, {3}});

    return 0;
}

// The solution uses a breadth‑first traversal via a queue. We start by pushing the root into a queue. Then, while the queue is not empty, we capture the current queue size (`sz`), which equals the number of nodes in the current level. We process exactly `sz` nodes: for each node, we pop it, push its left and right children (if non‑null) into the queue for the next level, and append the node’s value to a temporary vector representing the current level. After processing all nodes of the current level, we push that temporary vector into the result. This ensures that each inner vector corresponds to one depth level, and nodes are visited left‑to‑right because we enqueue left child before right child. Edge cases include an empty tree (immediately return an empty vector), a tree with only one node (returns `{{root->val}}`), and unbalanced trees where some levels may have few nodes—this is naturally handled by the `sz` loop. Time complexity is O(n) where n is the number of nodes, because each node is enqueued and dequeued exactly once. Space complexity is O(m) for the queue, where m is the maximum number of nodes at any single level (worst case O(n) for a complete tree’s last level), plus O(n) for the output vector.
