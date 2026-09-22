// Write a C++ function that takes a pointer to the root of a binary tree where each node contains a digit (0-9), and returns the sum of all root-to-leaf numbers. A root-to-leaf number is formed by concatenating the digits along the path from the root to a leaf. For example, if the path is 1 -> 2 -> 3, the number is 123. The tree is non-empty. The function should be named `sumRootToLeafNumbers` and take a `const TreeNode*` as parameter. Return 0 if the tree is empty (although the problem guarantees non-empty, handle it gracefully). Assume the tree nodes are defined as in the provided snippet (with `val`, `left`, `right`). Do not modify the tree.

The solution uses depth-first traversal (pre-order) accumulating the current number: each time we go down one level, we multiply the current accumulated number by 10 and add the node's value. When we reach a leaf (both children null), we add the accumulated number to the total sum. For an empty tree (root == nullptr), we return 0. The recursion handles internal nodes by summing the results from left and right subtrees. Edge cases include: a single-node tree (leaf) returns its value; a tree with only left or only right children works fine. Time complexity is O(N) because we visit each node exactly once. Space complexity is O(H) due to recursion stack, where H is the height of the tree (O(N) in worst case for skewed tree, O(log N) for balanced). The original snippet was inefficient (O(N^2) due to vector copying), but we can improve to O(N) by accumulating directly.

#include <cstddef>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Sum of all root-to-leaf numbers formed by concatenating node values.
int sumRootToLeafNumbers(const TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }

    // Helper recursive function to accumulate the sum.
    // current is the number formed from root to this node.
    auto dfs = [&](const TreeNode* node, int current) -> int {
        if (node == nullptr) {
            return 0;
        }

        current = current * 10 + node->val;

        // If it's a leaf, return the current number.
        if (node->left == nullptr && node->right == nullptr) {
            return current;
        }

        // Otherwise, sum results from left and right subtrees.
        int leftSum = dfs(node->left, current);
        int rightSum = dfs(node->right, current);
        return leftSum + rightSum;
    };

    return dfs(root, 0);
}

#include <cassert>

int main() {
    // Test 1: Single-node tree
    TreeNode n1(5);
    assert(sumRootToLeafNumbers(&n1) == 5);

    // Test 2: Two-level tree: 1 -> 2, 3
    TreeNode leaf2(2), leaf3(3);
    TreeNode root1(1, &leaf2, &leaf3);
    assert(sumRootToLeafNumbers(&root1) == 12 + 13); // 25

    // Test 3: Three-level tree: 4 -> 9 -> 5, 1; also 4 -> 0 -> null? Let's build 4->9->5 and 4->9->1 and 4->0->7
    TreeNode leaf5(5), leaf1(1), leaf7(7);
    TreeNode node9(9, &leaf5, &leaf1);
    TreeNode node0(0, &leaf7, nullptr);
    TreeNode root2(4, &node9, &node0);
    // Paths: 495, 491, 407 => sum = 495+491+407 = 1393
    assert(sumRootToLeafNumbers(&root2) == 1393);

    // Test 4: Empty tree
    assert(sumRootToLeafNumbers(nullptr) == 0);

    // Test 5: Skewed tree: 1 -> 2 -> 3
    TreeNode leaf3b(3);
    TreeNode node2b(2, &leaf3b, nullptr);
    TreeNode root3(1, &node2b, nullptr);
    assert(sumRootToLeafNumbers(&root3) == 123);

    // Test 6: Only right child: 1 -> null -> 2
    TreeNode leaf2c(2);
    TreeNode root4(1, nullptr, &leaf2c);
    assert(sumRootToLeafNumbers(&root4) == 12);

    // Test 7: All zeros: 0 -> 0 -> 0
    TreeNode leaf0(0);
    TreeNode node0b(0, &leaf0, nullptr);
    TreeNode root5(0, &node0b, nullptr);
    assert(sumRootToLeafNumbers(&root5) == 0);

    // Test 8: More complex tree from LeetCode example: [1,2,3] already tested
    // Additional: [4,9,0,5,1] expected 1026
    TreeNode l5(5), l1b(1), l0(0);
    TreeNode l9(9, &l5, &l1b);
    TreeNode root6(4, &l9, &l0);
    assert(sumRootToLeafNumbers(&root6) == 495 + 491 + 40); // 1026

    // Note: In test 8, leaf0 is a leaf, so path 4->0 gives 40.
    // sum = 495 + 491 + 40 = 1026

    return 0;
}
