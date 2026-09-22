// Write a C++ function `generateFullBinaryTrees(int n)` that returns a vector of shared pointers to all structurally distinct full binary trees containing exactly `n` nodes, where a full binary tree is defined as a tree in which every node has either zero or two children (so only odd `n` can produce valid full trees). The function must handle the edge case of even `n` by returning an empty vector, and for `n == 1` it must return a vector containing a single root node with null children. The returned trees should be represented with a struct `TreeNode` having fields `value` (an int, you may set it to 0 for all nodes), `left` (shared_ptr<TreeNode>), and `right` (shared_ptr<TreeNode>). You must ensure that the generated trees are deep copies (i.e., each tree is independent, not sharing subtrees across different trees in the result). The solution should be recursive, partitioning the remaining `n-1` nodes between left and right subtrees in all possible ways where both sides have odd counts.

// The core idea is recursion with memoization not strictly required for this task, but recursion is natural. A full binary tree with `n` nodes must have `n` odd because each internal node contributes exactly 2 children, and a tree with only leaves has 1 node; the total node count is always `2 * leaves - 1`. For recursion, a tree with `n > 1` nodes has a root and two subtrees whose total node counts sum to `n-1`. Since each subtree must itself be a full binary tree, both subtree node counts must be odd. So we iterate `left_count` over odd numbers from 1 to `n-2` (since `right_count = n-1 - left_count` must be at least 1 and odd). For each partition, we recursively generate all full trees for the left count and right count, then combine every left tree with every right tree under a new root. The base cases: if `n` is even, return empty; if `n == 1`, return a vector with one tree consisting of a single node. The number of trees for a given `n` follows the Catalan numbers: for `n = 2k+1`, the count is the k-th Catalan number. The time complexity is exponential in `n` because the number of trees grows exponentially; specifically, the total number of nodes in all generated trees is proportional to `n * C(n)`, where `C(n)` is the Catalan number for `n`. Space complexity is also exponential because we must store all trees; each tree uses O(n) nodes. The recursion depth is O(n) in the worst case. Edge cases: even `n` (including 0) returns empty; `n = 1` returns one tree; large odd `n` may quickly produce enormous outputs, so the function is practical only for small `n` (e.g., up to 15 or 17).

#include <memory>
#include <vector>

// Definition for a binary tree node.
struct TreeNode {
    int value;
    std::shared_ptr<TreeNode> left;
    std::shared_ptr<TreeNode> right;

    explicit TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

// Generate all full binary trees with exactly n nodes.
// A full binary tree has every node with 0 or 2 children.
// Returns an empty vector if n is even (impossible to have a full tree).
std::vector<std::shared_ptr<TreeNode>> generateFullBinaryTrees(int n) {
    // Even node count cannot form a full binary tree.
    if (n % 2 == 0) {
        return {};
    }

    // Base case: a single node is a full binary tree.
    if (n == 1) {
        return {std::make_shared<TreeNode>(0)};
    }

    std::vector<std::shared_ptr<TreeNode>> result;

    // The root consumes one node, so the subtrees share n-1 nodes.
    // Both left and right subtree sizes must be odd (each >= 1).
    for (int leftCount = 1; leftCount < n; leftCount += 2) {
        int rightCount = n - 1 - leftCount;

        // rightCount is guaranteed to be at least 1 and odd because leftCount is odd
        // and n-1 is even.

        auto leftTrees = generateFullBinaryTrees(leftCount);
        auto rightTrees = generateFullBinaryTrees(rightCount);

        // Combine every left tree with every right tree.
        for (const auto& left : leftTrees) {
            for (const auto& right : rightTrees) {
                auto root = std::make_shared<TreeNode>(0);
                root->left = left;
                root->right = right;
                result.push_back(root);
            }
        }
    }

    return result;
}

#include <cassert>
#include <memory>
#include <vector>

// Include the solution's definitions here (TreeNode and generateFullBinaryTrees).
#include "solution.h"

// Helper to count nodes in a tree.
int countNodes(const std::shared_ptr<TreeNode>& root) {
    if (!root) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

// Helper to verify that a tree is a full binary tree.
bool isFull(const std::shared_ptr<TreeNode>& root) {
    if (!root) return false; // Not a valid tree.
    if (!root->left && !root->right) return true; // Leaf.
    if (root->left && root->right) {
        return isFull(root->left) && isFull(root->right);
    }
    return false; // Exactly one child present.
}

int main() {
    // n = 1: exactly one tree (a single node).
    auto trees1 = generateFullBinaryTrees(1);
    assert(trees1.size() == 1);
    assert(countNodes(trees1[0]) == 1);
    assert(isFull(trees1[0]));

    // n = 3: exactly one tree (root with two leaves).
    auto trees3 = generateFullBinaryTrees(3);
    assert(trees3.size() == 1);
    assert(countNodes(trees3[0]) == 3);
    assert(isFull(trees3[0]));

    // n = 5: exactly two distinct trees.
    auto trees5 = generateFullBinaryTrees(5);
    assert(trees5.size() == 2);
    for (const auto& t : trees5) {
        assert(countNodes(t) == 5);
        assert(isFull(t));
    }

    // n = 7: five distinct trees.
    auto trees7 = generateFullBinaryTrees(7);
    assert(trees7.size() == 5);
    for (const auto& t : trees7) {
        assert(countNodes(t) == 7);
        assert(isFull(t));
    }

    // Even n returns empty vector.
    auto trees2 = generateFullBinaryTrees(2);
    assert(trees2.empty());
    auto trees0 = generateFullBinaryTrees(0);
    assert(trees0.empty());
    auto trees8 = generateFullBinaryTrees(8);
    assert(trees8.empty());

    // n = 9: fourteen trees.
    auto trees9 = generateFullBinaryTrees(9);
    assert(trees9.size() == 14);
    for (const auto& t : trees9) {
        assert(countNodes(t) == 9);
        assert(isFull(t));
    }

    return 0;
}
