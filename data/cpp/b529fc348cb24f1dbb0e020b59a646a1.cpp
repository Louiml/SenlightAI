// Write a C++ function that performs level-order traversal of a binary tree and returns a vector of vectors, where each inner vector contains the node values at that depth from left to right. The binary tree is defined using a standard `TreeNode` struct with `int val`, `TreeNode* left`, and `TreeNode* right`. The function must handle an empty tree by returning an empty vector of vectors. Traversal should process each level completely before moving to the next, preserving left-to-right order. The function signature must be `std::vector<std::vector<int>> levelOrderTraversal(const TreeNode* root)` with proper const correctness. Provide a complete implementation without a `main` function.
// The solution uses a breadth-first search (BFS) approach with a queue to process nodes level by level. To distinguish between levels, we insert a `nullptr` marker into the queue after each level's nodes. When we encounter a `nullptr`, we know we've finished one level, so we move the accumulated values from that level (stored in a temporary vector) into the result and clear the temporary vector. If there are more nodes remaining in the queue, we push another `nullptr` to mark the end of the next level. For each non-null node, we push its value into the current level's temporary vector and enqueue its left child (if exists) and right child (if exists). The algorithm handles an empty tree by returning an empty result immediately. Edge cases include: a single-node tree, skewed trees, and trees with missing left/right children (which are simply skipped). Time complexity is O(n) since every node is visited once, and space complexity is O(n) in the worst case (e.g., a complete binary tree where the largest level has about n/2 nodes, plus the result storage).
#include <vector>
#include <queue>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Return level-order traversal (left to right, level by level) of a binary tree.
// Empty tree returns an empty vector.
std::vector<std::vector<int>> levelOrderTraversal(const TreeNode* root) {
    std::vector<std::vector<int>> result;
    if (root == nullptr) {
        return result;
    }

    std::queue<const TreeNode*> nodeQueue;
    std::vector<int> currentLevel;
    
    nodeQueue.push(root);
    nodeQueue.push(nullptr);  // Level marker

    while (!nodeQueue.empty()) {
        const TreeNode* current = nodeQueue.front();
        nodeQueue.pop();

        if (current == nullptr) {
            // End of a level
            result.push_back(currentLevel);
            currentLevel.clear();
            if (!nodeQueue.empty()) {
                nodeQueue.push(nullptr);
            }
        } else {
            currentLevel.push_back(current->val);
            if (current->left) {
                nodeQueue.push(current->left);
            }
            if (current->right) {
                nodeQueue.push(current->right);
            }
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// TreeNode definition repeated for the test (kept consistent with solution).
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Test helper to build trees and clean up.
TreeNode* makeNode(int val) { return new TreeNode(val); }
void deleteTree(TreeNode* node) {
    if (node) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

// Function under test (declaration)
std::vector<std::vector<int>> levelOrderTraversal(const TreeNode* root);

int main() {
    // Test 1: Empty tree
    assert(levelOrderTraversal(nullptr) == std::vector<std::vector<int>>{});

    // Test 2: Single node
    TreeNode* root2 = makeNode(5);
    assert(levelOrderTraversal(root2) == std::vector<std::vector<int>>{{5}});
    deleteTree(root2);

    // Test 3: Full binary tree with 7 nodes
    TreeNode* root3 = makeNode(1);
    root3->left = makeNode(2);
    root3->right = makeNode(3);
    root3->left->left = makeNode(4);
    root3->left->right = makeNode(5);
    root3->right->left = makeNode(6);
    root3->right->right = makeNode(7);
    std::vector<std::vector<int>> expected3 = {{1}, {2, 3}, {4, 5, 6, 7}};
    assert(levelOrderTraversal(root3) == expected3);
    deleteTree(root3);

    // Test 4: Left-skewed tree with 3 nodes
    TreeNode* root4 = makeNode(10);
    root4->left = makeNode(20);
    root4->left->left = makeNode(30);
    std::vector<std::vector<int>> expected4 = {{10}, {20}, {30}};
    assert(levelOrderTraversal(root4) == expected4);
    deleteTree(root4);

    // Test 5: Tree with missing children at different levels
    TreeNode* root5 = makeNode(1);
    root5->right = makeNode(2);
    root5->right->left = makeNode(3);
    std::vector<std::vector<int>> expected5 = {{1}, {2}, {3}};
    assert(levelOrderTraversal(root5) == expected5);
    deleteTree(root5);

    // Test 6: Tree where one level has only one node on the right
    TreeNode* root6 = makeNode(1);
    root6->left = makeNode(2);
    root6->right = makeNode(3);
    root6->right->right = makeNode(4);
    std::vector<std::vector<int>> expected6 = {{1}, {2, 3}, {4}};
    assert(levelOrderTraversal(root6) == expected6);
    deleteTree(root6);

    return 0;
}
