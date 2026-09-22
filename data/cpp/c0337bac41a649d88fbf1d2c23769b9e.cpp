Write a C++ function `int sumRootToLeaf(TreeNode* root)` that takes the root of a binary tree where each node stores a value of either 0 or 1. Each root-to-leaf path, read from root to leaf, forms a binary number (with the root as the most significant bit). The function must return the sum of all such binary numbers formed by every leaf path in the tree. The tree is guaranteed to be non-empty, with up to 1000 nodes, and the final sum fits within a 32-bit signed integer. The solution must handle trees where nodes may have only one child or no children, and must compute the sum without converting intermediate path values to strings, using integer arithmetic for efficiency.

#include <cassert>

int main() {
    // Test 1: Example from prompt: [1,0,1,0,1,0,1]
    // Tree structure:
    //        1
    //       / \
    //      0   1
    //     / \ / \
    //    0  1 0  1
    // Paths: 100 (4), 101 (5), 110 (6), 111 (7) -> sum = 22
    TreeNode* leaf1 = new TreeNode(0);
    TreeNode* leaf2 = new TreeNode(1);
    TreeNode* leaf3 = new TreeNode(0);
    TreeNode* leaf4 = new TreeNode(1);
    TreeNode* n1 = new TreeNode(0, leaf1, leaf2);
    TreeNode* n2 = new TreeNode(1, leaf3, leaf4);
    TreeNode* root1 = new TreeNode(1, n1, n2);
    assert(sumRootToLeaf(root1) == 22);

    // Test 2: Single node tree [0] -> path "0" = 0
    TreeNode* root2 = new TreeNode(0);
    assert(sumRootToLeaf(root2) == 0);

    // Test 3: Single node tree [1] -> path "1" = 1
    TreeNode* root3 = new TreeNode(1);
    assert(sumRootToLeaf(root3) == 1);

    // Test 4: Left-skewed [1,0,1] -> path "101" = 5
    TreeNode* child4 = new TreeNode(0, nullptr, nullptr);
    TreeNode* leaf4b = new TreeNode(1);
    TreeNode* root4 = new TreeNode(1, child4, nullptr);
    child4->right = leaf4b;
    assert(sumRootToLeaf(root4) == 5);

    // Test 5: Right-skewed [1,1,0] -> path "110" = 6
    TreeNode* child5 = new TreeNode(1, nullptr, nullptr);
    TreeNode* leaf5b = new TreeNode(0);
    TreeNode* root5 = new TreeNode(1, nullptr, child5);
    child5->right = leaf5b;
    assert(sumRootToLeaf(root5) == 6);

    // Test 6: Tree with only left child leaf [1,0] -> path "10" = 2
    TreeNode* leaf6 = new TreeNode(0);
    TreeNode* root6 = new TreeNode(1, leaf6, nullptr);
    assert(sumRootToLeaf(root6) == 2);

    // Test 7: Tree [0,1] -> path "01" = 1
    TreeNode* leaf7 = new TreeNode(1);
    TreeNode* root7 = new TreeNode(0, leaf7, nullptr);
    assert(sumRootToLeaf(root7) == 1);

    // Test 8: Full binary tree with all ones, height 2: [1,1,1,1,1,1,1] -> paths 111,111,111,111 = 7*4=28
    TreeNode* l8a = new TreeNode(1);
    TreeNode* l8b = new TreeNode(1);
    TreeNode* l8c = new TreeNode(1);
    TreeNode* l8d = new TreeNode(1);
    TreeNode* n8a = new TreeNode(1, l8a, l8b);
    TreeNode* n8b = new TreeNode(1, l8c, l8d);
    TreeNode* root8 = new TreeNode(1, n8a, n8b);
    assert(sumRootToLeaf(root8) == 28);

    // Clean up not required for assert checks, but would be done in production code.

    return 0;
}

#include <cstdint>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper function that recursively traverses the tree.
// currentVal holds the binary value represented by the path from root to the current node (inclusive).
void dfs(const TreeNode* node, int currentVal, int& totalSum) {
    if (node == nullptr) {
        return;
    }
    
    // Update the current value with this node's bit.
    currentVal = (currentVal << 1) | node->val;
    
    // If it's a leaf, add the current value to the total sum.
    if (node->left == nullptr && node->right == nullptr) {
        totalSum += currentVal;
        return;
    }
    
    // Recurse on existing children.
    if (node->left) {
        dfs(node->left, currentVal, totalSum);
    }
    if (node->right) {
        dfs(node->right, currentVal, totalSum);
    }
}

// Main solution function: returns the sum of all root-to-leaf binary numbers.
int sumRootToLeaf(const TreeNode* root) {
    int totalSum = 0;
    dfs(root, 0, totalSum);
    return totalSum;
}

// The optimal approach is a depth-first traversal using recursion. At each node, we maintain the current binary value formed by the path from the root to that node. When we encounter a leaf (a node with both children null), we add the current accumulated value to the total sum. The key observation is that when moving from a parent with value `val` to a child, the new accumulated value is `(current * 2) + child->val`. This mimics shifting left and adding the new bit. Edge cases include a tree with only one node (the root is a leaf), and asymmetric trees where internal nodes may have only one child — we still recurse on existing children and ignore null children. The recursion depth can be as large as the height of the tree (up to 1000 in a degenerate chain), which is acceptable for recursion in typical environments. Time complexity is O(N), where N is the number of nodes, as each node is visited exactly once. Space complexity is O(H) for the recursion stack, where H is the height of the tree, worst-case O(N) for a skewed tree.
