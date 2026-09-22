// Write a C++ function that takes a binary tree root node (defined by `TreeNode` with integer `val`, `left`, and `right` pointers) and returns a `vector<vector<int>>` containing the values of the nodes at each level from bottom to top (i.e., the leaf level appears first, then the level above it, and finally the root level last). The function must handle empty trees (root is `nullptr`) by returning an empty vector, and it must preserve the left-to-right order of nodes within each level. The solution should not allocate the tree itself; it only assumes the tree is already constructed and valid.

#include <cassert>
#include <vector>
#include <iostream>

// TreeNode and levelOrderBottom are declared above (assume included).

int main() {
    // Test 1: Empty tree.
    TreeNode* empty = nullptr;
    assert(levelOrderBottom(empty).empty());

    // Test 2: Single node.
    TreeNode n1(5);
    std::vector<std::vector<int>> result = levelOrderBottom(&n1);
    assert(result.size() == 1);
    assert(result[0] == std::vector<int>({5}));

    // Test 3: Balanced tree of height 2 (example from snippet).
    TreeNode n3(3), n9(9), n20(20), n15(15), n7(7);
    n3.left = &n9;
    n3.right = &n20;
    n20.left = &n15;
    n20.right = &n7;
    result = levelOrderBottom(&n3);
    assert(result.size() == 3);
    assert(result[0] == std::vector<int>({15, 7})); // leaf level
    assert(result[1] == std::vector<int>({9, 20})); // middle level
    assert(result[2] == std::vector<int>({3}));     // root level

    // Test 4: Skewed left tree.
    TreeNode a(1), b(2), c(3);
    a.left = &b;
    b.left = &c;
    result = levelOrderBottom(&a);
    assert(result.size() == 3);
    assert(result[0] == std::vector<int>({3}));
    assert(result[1] == std::vector<int>({2}));
    assert(result[2] == std::vector<int>({1}));

    // Test 5: Tree with missing children (right subtree null).
    TreeNode d(10), e(20), f(30);
    d.left = &e;
    e.right = &f;
    result = levelOrderBottom(&d);
    assert(result.size() == 3);
    assert(result[0] == std::vector<int>({30}));
    assert(result[1] == std::vector<int>({20}));
    assert(result[2] == std::vector<int>({10}));

    // Test 6: Complete binary tree of height 3.
    TreeNode r(1), l2(2), r2(3), l3(4), r3(5), l4(6), r4(7);
    r.left = &l2; r.right = &r2;
    l2.left = &l3; l2.right = &r3;
    r2.left = &l4; r2.right = &r4;
    result = levelOrderBottom(&r);
    assert(result.size() == 3);
    assert(result[0] == std::vector<int>({4,5,6,7}));
    assert(result[1] == std::vector<int>({2,3}));
    assert(result[2] == std::vector<int>({1}));

    // Test 7: Tree with only right chain.
    TreeNode p(1), q(2), s(3);
    p.right = &q;
    q.right = &s;
    result = levelOrderBottom(&p);
    assert(result.size() == 3);
    assert(result[0] == std::vector<int>({3}));
    assert(result[1] == std::vector<int>({2}));
    assert(result[2] == std::vector<int>({1}));

    // Test 8: Single child at root (left only).
    TreeNode x(1), y(2);
    x.left = &y;
    result = levelOrderBottom(&x);
    assert(result.size() == 2);
    assert(result[0] == std::vector<int>({2}));
    assert(result[1] == std::vector<int>({1}));

    // Test 9: Two-level tree with only right child at root.
    TreeNode u(1), v(2);
    u.right = &v;
    result = levelOrderBottom(&u);
    assert(result.size() == 2);
    assert(result[0] == std::vector<int>({2}));
    assert(result[1] == std::vector<int>({1}));

    // Test 10: Large balanced tree (height 3) with 7 nodes (already covered).

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <vector>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper: recursively build bottom-up level order traversal.
// cur_level contains the current level's nodes (may contain nullptr).
// result is filled from deepest level upwards.
static void bottomUpHelper(const std::vector<TreeNode*>& cur_level,
                           std::vector<std::vector<int>>& result) {
    if (cur_level.empty()) {
        return;
    }

    std::vector<int> cur_vals;
    std::vector<TreeNode*> next_level;

    for (TreeNode* node : cur_level) {
        if (node != nullptr) {
            cur_vals.push_back(node->val);
            next_level.push_back(node->left);
            next_level.push_back(node->right);
        }
    }

    bottomUpHelper(next_level, result);

    // After deeper levels are appended, add this level.
    if (!cur_vals.empty()) {
        result.push_back(std::move(cur_vals));
    }
}

// Returns level-order traversal of a binary tree, from bottom to top.
std::vector<std::vector<int>> levelOrderBottom(TreeNode* root) {
    std::vector<std::vector<int>> result;
    if (root == nullptr) {
        return result;
    }
    std::vector<TreeNode*> cur_level;
    cur_level.push_back(root);
    bottomUpHelper(cur_level, result);
    return result;
}

// The core challenge is to reverse the usual top-down level-order traversal. A standard approach is to perform a recursive (or iterative) level-order traversal that processes levels from top to bottom, but instead of immediately appending each level's values, we build the result bottom-up by either using recursion that unwinds after processing deeper levels, or by stacking the levels and then reversing. Here, the provided snippet uses a recursive helper that first collects the current level's values and the next level's nodes, then calls itself on the next level, and only after the recursive call returns (i.e., after all deeper levels are processed) does it push the current level's values to the result. This naturally yields bottom-up ordering. Key edge cases: empty tree (returns empty vector), a root with no children (returns a single level containing only the root), and trees with `null` children in the level vector (filtered out before adding to next level and before pushing values). Time complexity is O(N) where N is the number of nodes, because each node is visited exactly once per level it appears in (each node appears in exactly one level). Space complexity is O(N) in the worst case (e.g., a completely unbalanced tree) due to the recursion stack and the vectors storing levels, though the recursion depth equals tree height, which can be O(N) for skewed trees. Alternatively, an iterative BFS with a stack or reversing the result would also be O(N) time and O(N) space.
