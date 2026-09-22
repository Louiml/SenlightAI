Write a C++ function that, given the root of a binary tree where each node contains a single digit (0-9), returns the sum of all numbers formed by root-to-leaf paths. Each path represents a number by reading its nodes from root to leaf in order (e.g., a path with nodes 1, 2, 3 forms the number 123). The function should handle empty trees (return 0), single-node trees, and trees with missing left or right children. All node data are guaranteed to be non-negative single digits, and the result may exceed 32-bit limits, so use `long long`.

#include <cassert>

// Helper to create a tree manually (for testing).
TreeNode* createNode(int val) {
    return new TreeNode(val);
}

int main() {
    // Test 1: Single node tree (root only) -> number = 5
    TreeNode* root1 = createNode(5);
    assert(treePathsSum(root1) == 5);
    
    // Test 2: Simple tree: root=1, left child=2, right child=3
    // Paths: 12 and 13 -> sum = 25
    TreeNode* root2 = createNode(1);
    root2->left = createNode(2);
    root2->right = createNode(3);
    assert(treePathsSum(root2) == 25);
    
    // Test 3: Left-skewed tree: 1->2->3 (as left children)
    // Path: 123 -> sum = 123
    TreeNode* root3 = createNode(1);
    root3->left = createNode(2);
    root3->left->left = createNode(3);
    assert(treePathsSum(root3) == 123);
    
    // Test 4: Tree with missing right child: root=4, left=5, left-left=6
    // Path: 456 -> sum = 456
    TreeNode* root4 = createNode(4);
    root4->left = createNode(5);
    root4->left->left = createNode(6);
    assert(treePathsSum(root4) == 456);
    
    // Test 5: Empty tree (null root) -> sum = 0
    assert(treePathsSum(nullptr) == 0);
    
    // Test 6: Larger tree: root=1, left=2, right=3, left's left=4, left's right=5
    // Paths: 124, 125, 13 -> sum = 124+125+13 = 262
    TreeNode* root6 = createNode(1);
    root6->left = createNode(2);
    root6->right = createNode(3);
    root6->left->left = createNode(4);
    root6->left->right = createNode(5);
    assert(treePathsSum(root6) == 262);
    
    // Test 7: All nodes same digit: root=0, left=0, right=0
    // Paths: 00 and 00 -> sum = 0 (since leading zeros are ignored)
    TreeNode* root7 = createNode(0);
    root7->left = createNode(0);
    root7->right = createNode(0);
    assert(treePathsSum(root7) == 0);
    
    // Test 8: Deep right chain: 9->8->7 (as right children)
    // Path: 987 -> sum = 987
    TreeNode* root8 = createNode(9);
    root8->right = createNode(8);
    root8->right->right = createNode(7);
    assert(treePathsSum(root8) == 987);
    
    return 0;
}

#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Helper function: recursively compute path sum starting from this node.
// currentValue holds the number formed from root up to (but not including) this node.
long long sumPathsHelper(const TreeNode* node, long long currentValue) {
    if (node == nullptr) {
        return 0;
    }
    
    currentValue = currentValue * 10 + node->data;
    
    // Leaf node: return the number formed.
    if (node->left == nullptr && node->right == nullptr) {
        return currentValue;
    }
    
    // Recurse on left and right subtrees and sum the results.
    return sumPathsHelper(node->left, currentValue) + sumPathsHelper(node->right, currentValue);
}

// Public function: compute sum of all root-to-leaf path numbers.
long long treePathsSum(const TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    return sumPathsHelper(root, 0);
}

// The solution uses a recursive depth-first traversal that carries the current number formed from the root to the current node. At each node, the current value is updated as `current = current * 10 + node->data`. When a leaf is reached (both children are null), the accumulated value is returned. For internal nodes, the function recursively computes the sum from the left and right subtrees and adds them together. If a node has only one child, the recursion naturally handles the other child being null (which returns 0). Edge cases include an empty tree, where the function should return 0; this can be handled by the initial check in the public function. The algorithm visits each node exactly once, so time complexity is O(n), where n is the number of nodes. Space complexity is O(h) for the recursion stack, where h is the tree height; in the worst case (a skewed tree) this is O(n), and in a balanced tree it is O(log n). No additional data structures are needed.
