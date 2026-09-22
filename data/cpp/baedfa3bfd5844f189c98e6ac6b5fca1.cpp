Write a C++ function that, given a binary tree represented by `TreeNode` structures (each node has an integer `val` and pointers `left` and `right` to its children, with `nullptr` for empty), and a target integer `sum`, returns a vector of all root-to-leaf paths where the sum of node values along the path equals `sum`. Each path must be represented as a vector of integers in the order from root to leaf. If no such path exists, return an empty vector. Empty trees (root is `nullptr`) return an empty vector. The tree may contain negative values, and nodes may have zero, one, or two children. The function should be named `pathSum` and take `const TreeNode* root` and `int targetSum` as parameters, returning `std::vector<std::vector<int>>`. Do not modify the tree. Ensure the solution correctly handles large inputs and deep recursion (you may assume the tree depth is within reasonable recursion limits for this task).

#include <cassert>
#include <vector>

// TreeNode definition is included as in the solution.

int main() {
    // Test 1: Tree from the original problem.
    TreeNode* root1 = new TreeNode(5);
    root1->left = new TreeNode(4);
    root1->right = new TreeNode(8);
    root1->left->left = new TreeNode(11);
    root1->right->left = new TreeNode(13);
    root1->right->right = new TreeNode(4);
    root1->left->left->left = new TreeNode(7);
    root1->left->left->right = new TreeNode(2);
    root1->right->right->left = new TreeNode(5);
    root1->right->right->right = new TreeNode(1);
    auto result1 = pathSum(root1, 22);
    std::vector<std::vector<int>> expected1 = {{5,4,11,2}, {5,8,4,5}};
    assert(result1 == expected1);

    // Test 2: Empty tree.
    assert(pathSum(nullptr, 0).empty());

    // Test 3: Single node equals target.
    TreeNode* root2 = new TreeNode(7);
    auto result2 = pathSum(root2, 7);
    assert(result2.size() == 1 && result2[0] == std::vector<int>{7});

    // Test 4: Single node does not equal target.
    auto result3 = pathSum(root2, 8);
    assert(result3.empty());

    // Test 5: Tree with negative values and multiple paths.
    TreeNode* root3 = new TreeNode(1);
    root3->left = new TreeNode(-2);
    root3->right = new TreeNode(3);
    root3->left->left = new TreeNode(4);
    root3->left->right = new TreeNode(5);
    // Paths summing to 3: [1, -2, 4] (sum=3), [3] (sum=3) — but [3] is leaf, so valid.
    auto result4 = pathSum(root3, 3);
    std::vector<std::vector<int>> expected4 = {{1,-2,4}, {3}};
    assert(result4 == expected4);

    // Test 6: No valid paths.
    auto result5 = pathSum(root3, 99);
    assert(result5.empty());

    // Test 7: Path where sum becomes zero only at a leaf but not leaf in between.
    TreeNode* root4 = new TreeNode(10);
    root4->left = new TreeNode(5);
    root4->left->left = new TreeNode(-5);
    // Path [10,5,-5] sums to 10, and leaf -5. But the node 5 is not a leaf, so it's still valid.
    auto result6 = pathSum(root4, 10);
    std::vector<std::vector<int>> expected6 = {{10,5,-5}};
    assert(result6 == expected6);

    // Clean up (optional for completeness of test).
    // In a real program, we'd delete tree nodes; but for test brevity, we skip to avoid clutter.
    return 0;
}

#include <vector>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Returns all root-to-leaf paths whose node values sum to targetSum.
std::vector<std::vector<int>> pathSum(const TreeNode* root, int targetSum) {
    std::vector<std::vector<int>> result;
    if (root == nullptr) {
        return result;
    }

    std::vector<int> currentPath;
    // Recursive helper using depth-first search.
    // We capture by reference for efficiency.
    std::function<void(const TreeNode*, int)> dfs = [&](const TreeNode* node, int remaining) {
        if (node == nullptr) {
            return;
        }
        remaining -= node->val;
        currentPath.push_back(node->val);

        // Check if leaf and sum matches.
        if (node->left == nullptr && node->right == nullptr && remaining == 0) {
            result.push_back(currentPath);
        } else {
            dfs(node->left, remaining);
            dfs(node->right, remaining);
        }

        currentPath.pop_back(); // Backtrack
    };

    dfs(root, targetSum);
    return result;
}

// The core algorithm is a depth-first search (DFS) that traverses every root-to-leaf path while maintaining a running sum and the current path. At each node, we subtract the node’s value from the remaining target sum and push the node’s value onto the current path. When we reach a leaf (both children are `nullptr`), we check if the remaining sum equals the leaf’s value; if so, we record a copy of the current path (including the leaf) in the result. After processing both children, we pop the node’s value from the path to backtrack. Important edge cases: (1) an empty tree should return an empty vector; (2) a single-node tree that equals the target should return that node’s value as a path; (3) negative values are allowed, so we cannot prune based on sum being too large or small; (4) we must not modify the tree, so traversal uses a `const TreeNode*`; (5) each result path must be a separate copy, not a reference to the reused `path` vector. Time complexity is O(N) where N is the number of nodes, because each node is visited once, and copying each valid path costs O(L) where L is path length, but total copying across all valid paths is O(N * H) in worst case (when every path is valid, H is height). Space complexity is O(H) for the recursion call stack and the path vector, plus O(P) where P is the number of valid paths (for the result storage).
