Write a C++ function that takes the root of a binary tree (where each node contains an integer value) and an integer `targetSum`, and returns a vector of vectors of integers representing all root-to-leaf paths where the sum of the node values along the path equals `targetSum`. A leaf is defined as a node with no children. The function must handle empty trees and any valid tree size between 0 and 5000 nodes, with node values and `targetSum` in the range [-1000, 1000]. The returned paths should preserve the order in which nodes are encountered from root to leaf.
The solution uses a recursive depth-first search (DFS) approach. At each node, we maintain the current path (a vector of node values) and the running sum from the root to that node. When we reach a leaf (a node with no left or right child), we check if the running sum equals `targetSum`. If so, we add the current path to the result list. To avoid copying the path vector at every call, we can pass a copy (as in the snippet) or use a reference with backtracking; for simplicity and clarity, a copy is acceptable given the constraint of up to 5000 nodes, but a more efficient approach uses a single path vector that we push onto and pop from after each recursive call (backtracking). The key edge cases include: an empty tree (returns empty result), a tree with only a root that may or may not match the target, and trees where multiple paths sum to the target. The algorithm visits each node exactly once, so the time complexity is O(N) where N is the number of nodes. The space complexity is O(H) for the recursion stack (where H is the tree height) plus O(N) for the result storage in the worst case (e.g., when all paths match). The recursive function should not modify the tree's node values (unlike the snippet which mutates `root->val`); instead, we pass the accumulated sum separately.
#include <vector>

// TreeNode definition
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper function to perform DFS
void dfs(TreeNode* node, int currentSum, int targetSum, std::vector<int>& path, std::vector<std::vector<int>>& result) {
    if (node == nullptr) {
        return;
    }
    
    // Add current node to path and update sum
    path.push_back(node->val);
    currentSum += node->val;
    
    // Check if leaf and sum matches target
    if (node->left == nullptr && node->right == nullptr) {
        if (currentSum == targetSum) {
            result.push_back(path);
        }
    } else {
        // Recurse on children
        if (node->left != nullptr) {
            dfs(node->left, currentSum, targetSum, path, result);
        }
        if (node->right != nullptr) {
            dfs(node->right, currentSum, targetSum, path, result);
        }
    }
    
    // Backtrack
    path.pop_back();
}

// Main function to find all root-to-leaf paths with sum equal to targetSum
std::vector<std::vector<int>> findPathsWithSum(TreeNode* root, int targetSum) {
    std::vector<std::vector<int>> result;
    std::vector<int> path;
    dfs(root, 0, targetSum, path, result);
    return result;
}
#include <cassert>
#include <vector>
#include <iostream>

// (TreeNode and function definitions from above are assumed to be available here)

int main() {
    // Test 1: Example from problem statement
    // Tree: [5,4,8,11,null,13,4,7,2,null,null,5,1]
    TreeNode* root1 = new TreeNode(5);
    root1->left = new TreeNode(4);
    root1->right = new TreeNode(8);
    root1->left->left = new TreeNode(11);
    root1->left->left->left = new TreeNode(7);
    root1->left->left->right = new TreeNode(2);
    root1->right->left = new TreeNode(13);
    root1->right->right = new TreeNode(4);
    root1->right->right->left = new TreeNode(5);
    root1->right->right->right = new TreeNode(1);
    std::vector<std::vector<int>> result1 = findPathsWithSum(root1, 22);
    assert(result1.size() == 2);
    assert(result1[0] == std::vector<int>({5,4,11,2}));
    assert(result1[1] == std::vector<int>({5,8,4,5}));
    
    // Test 2: Simple tree, no path matches
    TreeNode* root2 = new TreeNode(1);
    root2->left = new TreeNode(2);
    root2->right = new TreeNode(3);
    std::vector<std::vector<int>> result2 = findPathsWithSum(root2, 5);
    assert(result2.empty());
    
    // Test 3: Single root matching target
    TreeNode* root3 = new TreeNode(7);
    std::vector<std::vector<int>> result3 = findPathsWithSum(root3, 7);
    assert(result3.size() == 1);
    assert(result3[0] == std::vector<int>({7}));
    
    // Test 4: Single root not matching target
    TreeNode* root4 = new TreeNode(1);
    std::vector<std::vector<int>> result4 = findPathsWithSum(root4, 2);
    assert(result4.empty());
    
    // Test 5: Empty tree
    std::vector<std::vector<int>> result5 = findPathsWithSum(nullptr, 0);
    assert(result5.empty());
    
    // Test 6: Multiple paths, including negative values
    // Tree: [1, -2, -3, 1, 3, -2, null, -1]
    TreeNode* root6 = new TreeNode(1);
    root6->left = new TreeNode(-2);
    root6->right = new TreeNode(-3);
    root6->left->left = new TreeNode(1);
    root6->left->right = new TreeNode(3);
    root6->right->left = new TreeNode(-2);
    root6->left->left->left = new TreeNode(-1);
    std::vector<std::vector<int>> result6 = findPathsWithSum(root6, -1);
    // Paths: [1,-2,1,-1] = -1 and [1,-2,3] = 2? Actually compute:
    // Path1: 1+(-2)+1+(-1) = -1
    // Path2: 1+(-2)+3 = 2
    // Path3: 1+(-3)+(-2) = -4
    assert(result6.size() == 1);
    assert(result6[0] == std::vector<int>({1,-2,1,-1}));
    
    std::cout << "All tests passed!" << std::endl;
    
    // Clean up (not strictly necessary for asserts, but good practice)
    // (Delete nodes would require a helper; omitted for brevity in this test context)
    return 0;
}
