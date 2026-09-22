// Write a C++ function that takes the root of a binary tree (defined by a `TreeNode` struct with `val`, `left`, and `right` members, where `nullptr` represents a missing child) and returns a `std::vector<std::vector<int>>` representing the level-order traversal (breadth-first) of the tree, where each inner vector contains the node values at that depth from left to right. The function should be const-correct (the root pointer is `const TreeNode*` or we treat the tree as read-only), handle an empty tree (return an empty vector), and be self-contained with all necessary includes.

The solution uses a recursive depth-first search (DFS) that simulates breadth-first traversal by tracking the current depth. Starting at depth 0, the function visits the root, then recursively processes the left and right subtrees at depth+1. For each node, we check if the output vector already has a subvector for that depth. If not (i.e., `depth >= out.size()`), we create a new subvector containing the node's value; otherwise, we append the value to the existing subvector. This works because DFS visits all nodes at a given depth before going deeper, but crucially, it visits the leftmost nodes first, so the order within each level is correct. Edge cases: empty tree returns an empty vector; a tree with only a root returns `{{root->val}}`; unbalanced trees (e.g., only left children) still produce correct level grouping because each node's depth is explicitly tracked. Time complexity is O(n) where n is the number of nodes, since each node is visited once. Space complexity is O(n) in the worst case (e.g., a completely unbalanced tree) due to the recursion stack, plus O(n) for the output vector itself, but the auxiliary space excluding output is O(h) where h is tree height.

#include <vector>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

// Returns level-order traversal of a binary tree as a vector of vectors.
std::vector<std::vector<int>> levelOrderTraversal(const TreeNode* root) {
    std::vector<std::vector<int>> result;
    if (!root) return result;

    // Recursive helper to collect nodes by depth.
    std::function<void(const TreeNode*, int)> dfs = [&](const TreeNode* node, int depth) {
        if (!node) return;
        if (depth >= result.size()) {
            result.push_back({node->val});
        } else {
            result[depth].push_back(node->val);
        }
        dfs(node->left, depth + 1);
        dfs(node->right, depth + 1);
    };

    dfs(root, 0);
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Empty tree
    assert(levelOrderTraversal(nullptr) == std::vector<std::vector<int>>{});

    // Test 2: Single node
    TreeNode n1(1);
    assert(levelOrderTraversal(&n1) == std::vector<std::vector<int>>{{1}});

    // Test 3: Full binary tree with 3 levels
    TreeNode n7(7), n6(6), n5(5), n4(4), n3(3, &n6, &n7), n2(2, &n4, &n5), n1_root(1, &n2, &n3);
    assert(levelOrderTraversal(&n1_root) == std::vector<std::vector<int>>{{1}, {2, 3}, {4, 5, 6, 7}});

    // Test 4: Left-skewed tree
    TreeNode a(1), b(2), c(3);
    a.left = &b; b.left = &c;
    assert(levelOrderTraversal(&a) == std::vector<std::vector<int>>{{1}, {2}, {3}});

    // Test 5: Right-skewed tree
    TreeNode d(1), e(2), f(3);
    d.right = &e; e.right = &f;
    assert(levelOrderTraversal(&d) == std::vector<std::vector<int>>{{1}, {2}, {3}});

    // Test 6: Tree with missing left child on one node
    TreeNode x(10), y(20), z(30);
    x.left = &y; x.right = nullptr; y.right = &z;
    assert(levelOrderTraversal(&x) == std::vector<std::vector<int>>{{10}, {20}, {30}});

    // Test 7: Tree with missing right child
    TreeNode p(1), q(2), r(3);
    p.left = &q; q.left = &r;
    assert(levelOrderTraversal(&p) == std::vector<std::vector<int>>{{1}, {2}, {3}});

    // Test 8: Unbalanced with mixed depths
    TreeNode u(1), v(2), w(3), t(4);
    u.left = &v; v.right = &w; w.left = &t;
    assert(levelOrderTraversal(&u) == std::vector<std::vector<int>>{{1}, {2}, {3}, {4}});
}
