// Write a C++ function named `houseRobber` that accepts a binary tree where each node contains a non-negative integer value representing the amount of money in that house. The houses are arranged in a tree structure, and if a thief robs a house, they cannot rob its immediate parent or children. The function must return the maximum total amount of money that can be robbed without robbing two directly connected houses. The tree is provided via a `TreeNode` struct with `val`, `left`, and `right` members. The function should handle empty trees (return 0) and arbitrary tree shapes, including unbalanced ones. You may use `std::map` or any other data structures for memoization.

#include <cassert>

int main() {
    // Test 1: Empty tree
    assert(houseRobber(nullptr) == 0);

    // Test 2: Single node
    TreeNode n1(10);
    assert(houseRobber(&n1) == 10);

    // Test 3: Tree with 3 nodes (root 3, left 2, right 3)
    TreeNode root3(3);
    TreeNode left3(2);
    TreeNode right3(3);
    root3.left = &left3;
    root3.right = &right3;
    // Options: rob root (3) + skip children -> 3; or rob children (2+3=5) -> 5
    assert(houseRobber(&root3) == 5);

    // Test 4: The example from the snippet: [3,2,3,null,3,null,1]
    TreeNode r(3);
    TreeNode l(2);
    TreeNode rr(3);
    TreeNode lr(3);
    TreeNode rrr(1);
    r.left = &l;
    r.right = &rr;
    l.right = &lr;
    rr.right = &rrr;
    // Rob root (3) + grandchildren (3+1=4) -> 3+4=7; or rob children (2+3=5) -> 5; max=7
    assert(houseRobber(&r) == 7);

    // Test 5: Chain of 4 nodes: 1 -> 2 -> 3 -> 4 (right only)
    TreeNode a(1);
    TreeNode b(2);
    TreeNode c(3);
    TreeNode d(4);
    a.right = &b;
    b.right = &c;
    c.right = &d;
    // Options: rob 1 and 3: 1+3=4; rob 2 and 4: 2+4=6; max=6
    assert(houseRobber(&a) == 6);

    // Test 6: Full binary tree of 3 levels with all leaf values 5, internal values 1
    TreeNode root6(1);
    TreeNode l1(1), r1(1);
    root6.left = &l1; root6.right = &r1;
    TreeNode ll(5), lr(5), rl(5), rr(5);
    l1.left = &ll; l1.right = &lr;
    r1.left = &rl; r1.right = &rr;
    // Options: rob root (1) + all grandchildren (5+5+5+5=20) -> 21; or rob children (1+1=2) -> 2; max=21
    assert(houseRobber(&root6) == 21);

    // Test 7: Unbalanced deep tree (all left children): 1,2,3
    TreeNode n7_1(1);
    TreeNode n7_2(2);
    TreeNode n7_3(3);
    n7_1.left = &n7_2;
    n7_2.left = &n7_3;
    // Options: rob 1 and 3: 1+3=4; rob 2: 2; max=4
    assert(houseRobber(&n7_1) == 4);

    return 0;
}

#include <map>
#include <algorithm>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper function with memoization
int robHelper(const TreeNode* root, std::map<const TreeNode*, int>& memo) {
    if (root == nullptr) {
        return 0;
    }
    auto it = memo.find(root);
    if (it != memo.end()) {
        return it->second;
    }

    int robCurrent = root->val;
    if (root->left != nullptr) {
        robCurrent += robHelper(root->left->left, memo) + robHelper(root->left->right, memo);
    }
    if (root->right != nullptr) {
        robCurrent += robHelper(root->right->left, memo) + robHelper(root->right->right, memo);
    }

    int skipCurrent = robHelper(root->left, memo) + robHelper(root->right, memo);

    int result = std::max(robCurrent, skipCurrent);
    memo[root] = result;
    return result;
}

// Main function: returns the maximum amount robbable from the tree
int houseRobber(const TreeNode* root) {
    std::map<const TreeNode*, int> memo;
    return robHelper(root, memo);
}

// The problem is a classic tree dynamic programming (DP) problem, a generalization of the House Robber problem to a binary tree. The base case is an empty node, which returns 0. For a given node, there are two mutually exclusive strategies:
// 1. **Rob the current node**: Then we cannot rob its direct children, but we can rob its grandchildren. The value obtained is `root->val + rob(left->left) + rob(left->right) + rob(right->left) + rob(right->right)`.
// 2. **Skip the current node**: Then we can rob its left and right subtrees independently, giving `rob(left) + rob(right)`.
//
// The answer for a node is the maximum of these two strategies. Since subtrees are computed multiple times (e.g., a node may be considered as a grandchild and as a child), memoization is essential to avoid exponential time. Using a `std::map<TreeNode*, int>` to store the computed result for each node ensures each node is processed exactly once. The recursion is straightforward.
//
// Edge cases: empty tree returns 0; a single node returns its value; nodes with only one child; deep chains where the recursion depth equals the number of nodes (so stack overflow possible for extremely skewed trees in practice, but acceptable for typical constraints). Time complexity is O(n), where n is the number of nodes, and space complexity is O(n) for the memoization map plus O(h) for recursion stack, where h is tree height.
