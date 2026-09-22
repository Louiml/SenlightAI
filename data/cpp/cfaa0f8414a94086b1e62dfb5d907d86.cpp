// Write a C++ function `int longestRootToLeafPathSum(Node* root)` that takes a root pointer to a binary tree where each node stores an integer value (which may be negative), and returns the sum of the node values along the root-to-leaf path having the greatest length. If multiple root-to-leaf paths tie for the maximum length, return the largest sum among them. The tree’s structure is given by the following `Node` definition: `struct Node { int data; Node* left; Node* right; Node(int val) : data(val), left(nullptr), right(nullptr) {} };`. The function should handle an empty tree (return 0), single-node trees, negative values, and unbalanced trees. You must not use any global or static variables; all state should be passed via parameters or local variables.

#include <cassert>

int main() {
    // Test 1: Empty tree -> 0
    assert(longestRootToLeafPathSum(nullptr) == 0);

    // Test 2: Single node
    Node* n1 = new Node(5);
    assert(longestRootToLeafPathSum(n1) == 5);
    delete n1;

    // Test 3: Balanced tree, all paths equal length -> largest sum
    //    1
    //   / \
    //  2   3
    // / \ / \
    // 4 5 6 7
    Node* root3 = new Node(1);
    root3->left = new Node(2);
    root3->right = new Node(3);
    root3->left->left = new Node(4);
    root3->left->right = new Node(5);
    root3->right->left = new Node(6);
    root3->right->right = new Node(7);
    assert(longestRootToLeafPathSum(root3) == 17); // 1+3+6+7 = 17 vs 1+2+4+5 = 12
    // Cleanup
    delete root3->left->left; delete root3->left->right;
    delete root3->right->left; delete root3->right->right;
    delete root3->left; delete root3->right; delete root3;

    // Test 4: Negative values, longer path wins despite lower sum
    //      -1
    //     /  \
    //    2    3
    //   /      \
    //  4        5
    // Longest path left side: -1+2+4 = 5 (len 3), right side: -1+3+5 = 7 (len 3, bigger sum)
    Node* root4 = new Node(-1);
    root4->left = new Node(2);
    root4->right = new Node(3);
    root4->left->left = new Node(4);
    root4->right->right = new Node(5);
    assert(longestRootToLeafPathSum(root4) == 7);
    delete root4->left->left; delete root4->right->right;
    delete root4->left; delete root4->right; delete root4;

    // Test 5: Unbalanced, longer path has very negative sum but still wins
    //    0
    //    |
    //    -100
    //    |
    //    50
    // Path length 3 sum = -50, vs root alone (len 1 sum 0) -> longest wins
    Node* root5 = new Node(0);
    root5->left = new Node(-100);
    root5->left->left = new Node(50);
    assert(longestRootToLeafPathSum(root5) == -50);
    delete root5->left->left; delete root5->left; delete root5;

    // Test 6: Equal length paths with negative sums, pick larger sum
    //     10
    //    /  \
    //  -5    -3
    //  /      \
    // -20      -2
    // Both len 3: left sum = -15, right sum = 5 -> pick 5
    Node* root6 = new Node(10);
    root6->left = new Node(-5);
    root6->right = new Node(-3);
    root6->left->left = new Node(-20);
    root6->right->right = new Node(-2);
    assert(longestRootToLeafPathSum(root6) == 5);
    delete root6->left->left; delete root6->right->right;
    delete root6->left; delete root6->right; delete root6;

    return 0;
}

#include <algorithm>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Recursive helper that traverses the tree, tracking current path length and sum.
// It updates maxLen and maxSum by reference when a leaf is reached.
void dfsHelper(Node* node, int currentLen, int currentSum, int& maxLen, int& maxSum) {
    if (node == nullptr) {
        // Reached null (past a leaf). Update the best length and sum.
        if (currentLen > maxLen) {
            maxLen = currentLen;
            maxSum = currentSum;
        } else if (currentLen == maxLen) {
            maxSum = std::max(maxSum, currentSum);
        }
        return;
    }
    currentSum += node->data;
    // Traverse left and right children. Pass len+1 and the updated sum.
    dfsHelper(node->left, currentLen + 1, currentSum, maxLen, maxSum);
    dfsHelper(node->right, currentLen + 1, currentSum, maxLen, maxSum);
}

// Returns the sum of values on the longest root-to-leaf path.
// If multiple paths have the same maximum length, return the largest sum.
int longestRootToLeafPathSum(Node* root) {
    if (root == nullptr) {
        return 0;
    }
    int maxLen = 0;
    int maxSum = 0; // Will be overwritten at first leaf.
    dfsHelper(root, 0, 0, maxLen, maxSum);
    return maxSum;
}

// The core idea is a depth-first traversal that tracks two pieces of information for each recursive call: the current path length and the accumulated sum from the root to the current node. When a recursion reaches a null child (meaning a leaf has been passed), we compare the current length with the best known maximum length. If the current length is strictly greater, it becomes the new maximum length and its sum becomes the best sum. If the current length equals the maximum, we update the best sum to the larger of the current sum and the existing best sum. Backtracking is unnecessary because we pass copies of `sum` and `len` by value. Edge cases include an empty tree (base case immediately returns 0), negative values (the sum may decrease, but comparisons are straightforward), and nodes with only one child (the other child is null, so the leaf condition is handled at that null). For a tree with `n` nodes, every node is visited exactly once, so the time complexity is O(n). Recursion depth is the height of the tree, which in the worst case (a skewed tree) is O(n), giving worst-case space O(n) on the call stack; for a balanced tree it would be O(log n). No extra data structures are used beyond constant space.
