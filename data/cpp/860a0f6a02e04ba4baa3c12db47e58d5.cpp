Given a non-empty binary tree where each node contains an integer (which may be negative), write a C++ function `int maxPathSum(const TreeNode* root)` that returns the maximum possible sum of a path in the tree. A path is defined as any sequence of nodes where each adjacent pair is connected by an edge, and a node may appear at most once in the path. The path may start and end at any node, and it does not need to pass through the root. The function must handle trees with negative values and must process each node exactly once.
// The key observation is that for any node, the maximum path that passes through that node can be formed by taking the node’s value plus, optionally, the best “downward path” from its left child and/or its right child. However, a path cannot branch in both children at a node and then continue upward to the parent; if the path passes through the node and continues to the parent, it can only choose one branch (or none). So we perform a post-order traversal: for each node, we compute the maximum downward path sum starting at that node and going strictly downward (i.e., ending at that node or below). This downward sum is `node->val + max(0, maxDown(left)) + max(0, maxDown(right))` when the path is allowed to end at the node (for the global answer), but the value returned to the parent must only include at most one child branch: `node->val + max(0, maxDown(left), maxDown(right))`. The global answer is the maximum over all nodes of the “through-node” sum. Edge cases include: negative-valued nodes (we still consider them, but a downward path may be better if we skip a negative child by using `max(0, ...)`), single-node trees, and all-negative trees (the answer is the maximum single node value). Time complexity is O(n) where n is the number of nodes, and space complexity is O(h) for the recursion stack, where h is the tree height (O(n) in the worst case for a skewed tree).
#include <algorithm>
#include <climits>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper function that returns the maximum downward path sum starting at the given node
// and updates the global maximum path sum.
int maxDownPath(const TreeNode* node, int& globalMax) {
    if (!node) return 0;

    int leftDown = std::max(0, maxDownPath(node->left, globalMax));
    int rightDown = std::max(0, maxDownPath(node->right, globalMax));

    // Maximum path that passes through this node (may end here or go downward in both branches).
    int throughNode = node->val + leftDown + rightDown;
    globalMax = std::max(globalMax, throughNode);

    // To continue upward, we can only take one branch.
    return node->val + std::max(leftDown, rightDown);
}

// Returns the maximum path sum in the binary tree.
// The tree is assumed to be non-empty.
int maxPathSum(const TreeNode* root) {
    int globalMax = INT_MIN;
    maxDownPath(root, globalMax);
    return globalMax;
}
#include <cassert>

int main() {
    // Test case 1: Single node
    TreeNode n1(5);
    assert(maxPathSum(&n1) == 5);

    // Test case 2: All negative tree, path is the largest single node
    TreeNode a(-10);
    TreeNode b(-2);
    TreeNode c(-3);
    TreeNode d(-1);
    a.left = &b;
    a.right = &c;
    b.left = &d;
    // Tree:       -10
    //           /    \
    //         -2      -3
    //         /
    //       -1
    // Maximum path is just -1 (single node) because all values are negative.
    assert(maxPathSum(&a) == -1);

    // Test case 3: Classic positive path through negative root
    TreeNode p1(10);
    TreeNode p2(2);
    TreeNode p3(10);
    TreeNode p4(20);
    TreeNode p5(1);
    TreeNode p6(-25);
    TreeNode p7(-3);
    TreeNode p8(4);
    TreeNode p9(5);
    p1.left = &p2;
    p1.right = &p3;
    p2.left = &p4;
    p2.right = &p5;
    p3.left = &p6;
    p3.right = &p7;
    p6.left = &p8;
    p6.right = &p9;
    // Tree:            10
    //               /     \
    //              2      10
    //            /  \    /  \
    //          20    1  -25  -3
    //                  /  \
    //                 4    5
    // Best path: 20 -> 2 -> 10 -> 10 -> 1 = 43? Actually 20+2+10+10+1 = 43, but also 4+(-25)+10+10 = -1, so best is 43.
    assert(maxPathSum(&p1) == 43);

    // Test case 4: Mixed negatives and positives, path not through root
    TreeNode q1(-3);
    TreeNode q2(4);
    TreeNode q3(5);
    TreeNode q4(6);
    q1.left = &q2;
    q1.right = &q3;
    q2.right = &q4;
    // Tree:      -3
    //           /   \
    //          4     5
    //           \
    //            6
    // Best path: 6+4+5 = 15 (through node 4, not root).
    assert(maxPathSum(&q1) == 15);

    // Test case 5: Zeros and negatives
    TreeNode r1(0);
    TreeNode r2(-1);
    TreeNode r3(0);
    TreeNode r4(-2);
    r1.left = &r2;
    r1.right = &r3;
    r2.right = &r4;
    // Tree:      0
    //          /   \
    //        -1     0
    //          \
    //           -2
    // Best path: 0 (root alone) or 0 (right child alone) or -1+0 = -1, so max is 0.
    assert(maxPathSum(&r1) == 0);

    return 0;
}
