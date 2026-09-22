// Write a C++ function that performs a level-order traversal of a binary tree and returns a vector of vectors, where each inner vector contains the values of all nodes at that depth, from left to right, and the outer vector is ordered from the root level downward. The function must accept a pointer to the tree's root node, which may be null (empty tree). The tree node structure is already defined as `TreeNode` with integer value and left/right child pointers. The function should work for any valid binary tree, including skewed trees and trees with only one node.

#include <cassert>

int main() {
    // Test 1: Empty tree
    {
        TreeNode* root = nullptr;
        std::vector<std::vector<int>> expected = {};
        assert(levelOrderTraversal(root) == expected);
    }
    
    // Test 2: Single node
    {
        TreeNode* root = new TreeNode(5);
        std::vector<std::vector<int>> expected = {{5}};
        assert(levelOrderTraversal(root) == expected);
        delete root;
    }
    
    // Test 3: Complete binary tree of height 2
    {
        TreeNode* root = new TreeNode(1);
        root->left = new TreeNode(2);
        root->right = new TreeNode(3);
        root->left->left = new TreeNode(4);
        root->left->right = new TreeNode(5);
        root->right->left = new TreeNode(6);
        root->right->right = new TreeNode(7);
        std::vector<std::vector<int>> expected = {{1}, {2, 3}, {4, 5, 6, 7}};
        assert(levelOrderTraversal(root) == expected);
        // Cleanup
        delete root->left->left;
        delete root->left->right;
        delete root->right->left;
        delete root->right->right;
        delete root->left;
        delete root->right;
        delete root;
    }
    
    // Test 4: Skewed tree (right-only)
    {
        TreeNode* root = new TreeNode(10);
        root->right = new TreeNode(20);
        root->right->right = new TreeNode(30);
        std::vector<std::vector<int>> expected = {{10}, {20}, {30}};
        assert(levelOrderTraversal(root) == expected);
        delete root->right->right;
        delete root->right;
        delete root;
    }
    
    // Test 5: Tree with missing children in middle level
    {
        TreeNode* root = new TreeNode(1);
        root->left = new TreeNode(2);
        root->right = new TreeNode(3);
        root->left->right = new TreeNode(4);
        std::vector<std::vector<int>> expected = {{1}, {2, 3}, {4}};
        assert(levelOrderTraversal(root) == expected);
        delete root->left->right;
        delete root->left;
        delete root->right;
        delete root;
    }
    
    return 0;
}

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

// Perform level-order traversal and return nodes grouped by depth.
std::vector<std::vector<int>> levelOrderTraversal(const TreeNode* root) {
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

// The solution uses a breadth-first search (BFS) approach with a queue to explore nodes level by level. Start by checking if the root is null; if so, return an empty vector. Initialize a queue and push the root. At each iteration of the outer loop, record the current queue size, which equals the number of nodes in the current level. Then pop exactly that many nodes from the queue, append their values to a temporary `level` vector, and enqueue their non-null left and right children. After processing all nodes of that level, push the `level` vector onto the answer. Edge cases: an empty tree returns an empty result; a tree with only a root returns a single inner vector containing that value; nodes with missing children are simply skipped in the enqueue step. The algorithm visits each node exactly once, so time complexity is O(n), where n is the number of nodes. The maximum auxiliary space used by the queue at any point is the width of the tree, which in the worst case (a complete binary tree of height h) is about n/2, so space complexity is O(n) in the worst case.
