Write a C++ function named `evaluateMoveScore` that simulates a simplified one-player game tree where each move immediately produces a terminal board value stored in a node of a binary tree. The function must accept a pointer to a `BoardNode` (a struct with an integer `value` and pointers to up to two child nodes) and return the best achievable result from that node by applying a depth-limited minimax-like evaluation: at even depths, choose the maximum value among the node’s value and the results of its children; at odd depths, choose the minimum value among the node’s value and the results of its children. Depth is defined as 0 for the root node passed to the function, increasing by 1 for each child. If a child pointer is null, ignore that branch. The function should handle arbitrary tree structures, including unbalanced and missing children, and must not modify the tree. The result should be a `double` for consistency with typical game evaluation scores.
The core algorithm is a recursive depth-limited minimax evaluation on a binary tree. At each node, the function compares the node’s own `value` with the results of recursively evaluating its left and right children, but the choice of whether to take the maximum or minimum depends on the current depth parity. For even depths (including the root depth 0), we aim to maximize the outcome, so we start with the node’s value as the best candidate and update it with the maximum of itself and any child result. For odd depths, we minimize similarly. Base case: if a node has no children, the result is simply its value, regardless of depth. Edge cases include null child pointers (skip them) and a tree with only one node. The recursion naturally handles unbalanced structures. Time complexity is O(n) where n is the total number of nodes, because each node is visited exactly once. Space complexity is O(h) for the recursion stack, where h is the maximum tree height (O(n) in worst-case skewed tree). No additional data structures are needed.
#include <algorithm>  // for std::max and std::min

// Simple binary tree node with an integer value and up to two children.
struct BoardNode {
    double value;
    BoardNode* left;
    BoardNode* right;
    BoardNode(double v, BoardNode* l = nullptr, BoardNode* r = nullptr)
        : value(v), left(l), right(r) {}
};

// Evaluate the best achievable score from the given node using depth-limited minimax.
// Depth parity determines whether the node is a maximizing (even depth) or minimizing (odd depth) node.
double evaluateMoveScore(const BoardNode* node, int depth = 0) {
    if (node == nullptr) {
        // Should not be called with null, but handle gracefully.
        return 0.0;
    }

    // Base case: leaf node (no children) returns its own value.
    if (node->left == nullptr && node->right == nullptr) {
        return node->value;
    }

    // Initialize best result with the current node's value.
    double best = node->value;

    // Recursively evaluate children if they exist.
    if (node->left != nullptr) {
        double childResult = evaluateMoveScore(node->left, depth + 1);
        best = (depth % 2 == 0) ? std::max(best, childResult) : std::min(best, childResult);
    }
    if (node->right != nullptr) {
        double childResult = evaluateMoveScore(node->right, depth + 1);
        best = (depth % 2 == 0) ? std::max(best, childResult) : std::min(best, childResult);
    }

    return best;
}
#include <cassert>

int main() {
    // Leaf-only tree.
    BoardNode leaf1(5.0);
    assert(evaluateMoveScore(&leaf1) == 5.0);

    // Root with two leaves, root at depth 0 (maximizing).
    BoardNode left1(3.0), right1(7.0);
    BoardNode root1(1.0, &left1, &right1);
    assert(evaluateMoveScore(&root1) == 7.0);  // max(1,3,7)

    // Root with two leaves, root at depth 0, but child depth is odd (minimizing at children).
    // At children, they are leaves so they just return their own values. Root maximizes => max(1,3,7)=7.
    // Same as above, but test different values.
    BoardNode left2(10.0), right2(2.0);
    BoardNode root2(4.0, &left2, &right2);
    assert(evaluateMoveScore(&root2) == 10.0);  // max(4,10,2)

    // Deeper tree: root max, then child min, then grandchild max.
    // Build:       root(0)   
    //           /          \
    //        L(1)          R(1)
    //       /   \          /   \
    //    LL(2) LR(2)   RL(2) RR(2)
    // Values: LL=8, LR=3, RL=1, RR=6; L=5, R=4; root=2.
    BoardNode ll(8.0), lr(3.0), rl(1.0), rr(6.0);
    BoardNode l(5.0, &ll, &lr);
    BoardNode r(4.0, &rl, &rr);
    BoardNode root3(2.0, &l, &r);
    // At LL and LR (depth2) they are leaves => 8 and 3. L (depth1) minimizes => min(5,8,3)=3
    // At RL and RR => 1 and 6. R (depth1) minimizes => min(4,1,6)=1
    // Root (depth0) maximizes => max(2,3,1)=3
    assert(evaluateMoveScore(&root3) == 3.0);

    // Unbalanced tree: root has only left child, which has only right child.
    BoardNode grandchild(9.0);
    BoardNode child(6.0, nullptr, &grandchild);
    BoardNode root4(1.0, &child, nullptr);
    // depth0 max: max(1, result from child at depth1)
    // child depth1 min: min(6, result from grandchild at depth2)
    // grandchild depth2 max: max(9) = 9 (leaf)
    // child: min(6,9)=6
    // root: max(1,6)=6
    assert(evaluateMoveScore(&root4) == 6.0);

    // All children null (leaf) with negative value.
    BoardNode leaf5(-3.5);
    assert(evaluateMoveScore(&leaf5) == -3.5);

    // Single node with both children null but value 0.
    BoardNode zero(0.0);
    assert(evaluateMoveScore(&zero) == 0.0);

    return 0;
}
