Write a C++ function `int deepestLeavesSum(TreeNode* root)` that takes the root of a binary tree and returns the sum of the values of all leaf nodes at the deepest level (i.e., the maximum depth). The tree nodes are defined by the standard `TreeNode` struct with integer value, left and right child pointers. The function must handle empty trees (return 0) and trees with only one node. You may not modify the tree structure, and your solution must traverse the tree efficiently.

// The solution computes the height of the tree first, which gives the maximum depth level where the deepest leaves reside. Then it performs a level-order (breadth-first) traversal of the tree using a queue. During the traversal, it tracks the current level number, starting from the top (level 1). When the current level equals the total height, all nodes at that level are guaranteed to be leaves (because if they had children, the height would be greater). At that point, we sum all node values at that final level. The algorithm handles edge cases like an empty tree (returns 0 immediately) and a single-node tree (height=1, the root itself is the deepest leaf). Time complexity is O(n) because each node is visited twice (once for height computation recursively and once in BFS), and space complexity is O(w) for the queue where w is the maximum width of the tree, plus O(h) recursion stack for height computation. The solution is straightforward and uses standard tree traversal techniques.

#include <queue>
#include <algorithm>

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

// Returns the height of the tree (number of levels from root to deepest leaf).
int treeHeight(TreeNode* node) {
    if (node == nullptr) {
        return 0;
    }
    return 1 + std::max(treeHeight(node->left), treeHeight(node->right));
}

// Sums the values of all leaves located at the deepest level.
int deepestLeavesSum(TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    
    int maxDepth = treeHeight(root);
    int sum = 0;
    int currentLevel = 1;
    
    std::queue<TreeNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; ++i) {
            TreeNode* current = q.front();
            q.pop();
            
            if (currentLevel == maxDepth) {
                sum += current->val;
            }
            
            if (current->left != nullptr) {
                q.push(current->left);
            }
            if (current->right != nullptr) {
                q.push(current->right);
            }
        }
        ++currentLevel;
    }
    
    return sum;
}

#include <cassert>

int main() {
    // Test 1: Empty tree
    assert(deepestLeavesSum(nullptr) == 0);

    // Test 2: Single node
    TreeNode* root2 = new TreeNode(5);
    assert(deepestLeavesSum(root2) == 5);
    delete root2;

    // Test 3: Simple tree with all leaves at same depth
    //        1
    //       / \
    //      2   3
    //     / \   \
    //    4   5   6
    TreeNode* root3 = new TreeNode(1);
    root3->left = new TreeNode(2);
    root3->right = new TreeNode(3);
    root3->left->left = new TreeNode(4);
    root3->left->right = new TreeNode(5);
    root3->right->right = new TreeNode(6);
    assert(deepestLeavesSum(root3) == 15); // 4+5+6
    delete root3->left->left;
    delete root3->left->right;
    delete root3->right->right;
    delete root3->left;
    delete root3->right;
    delete root3;

    // Test 4: Unbalanced tree
    //        1
    //       /
    //      2
    //     /
    //    3
    //   /
    //  4
    TreeNode* root4 = new TreeNode(1);
    root4->left = new TreeNode(2);
    root4->left->left = new TreeNode(3);
    root4->left->left->left = new TreeNode(4);
    assert(deepestLeavesSum(root4) == 4);
    delete root4->left->left->left;
    delete root4->left->left;
    delete root4->left;
    delete root4;

    // Test 5: Left subtree deeper
    //       10
    //      /  \
    //     5   15
    //    / 
    //   3   
    TreeNode* root5 = new TreeNode(10);
    root5->left = new TreeNode(5);
    root5->right = new TreeNode(15);
    root5->left->left = new TreeNode(3);
    assert(deepestLeavesSum(root5) == 3);
    delete root5->left->left;
    delete root5->left;
    delete root5->right;
    delete root5;

    return 0;
}
