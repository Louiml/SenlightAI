Write a C++ function `int treeDiameter(TreeNode* root)` that takes the root of a binary tree (where each `TreeNode` has an integer `val` and `left`/`right` child pointers, which may be null) and returns the diameter of the tree, defined as the number of edges on the longest path between any two nodes. The path may or may not pass through the root. For an empty tree (null root), return 0. Ensure your implementation is correct for trees with negative values, single-node trees, and skewed (unbalanced) trees. You are provided with the `TreeNode` struct definition; implement your solution as a free function (not inside a class) with the exact signature above, and do not modify the struct. The function must handle arbitrarily deep trees without stack overflow for reasonable test inputs (you may assume the tree is not extremely deep, but aim for an efficient recursive solution).

The diameter of a binary tree is the maximum length (in edges) of a path between any two nodes. A path’s length equals the sum of the depths of its two endpoints measured from their lowest common ancestor. For any node, the longest path that passes through that node as the highest point (the LCA of the two endpoints) is `leftHeight + rightHeight`, where `leftHeight` is the maximum depth (number of edges from that node to a leaf in its left subtree) and `rightHeight` similarly for the right subtree. Thus, we perform a post-order traversal: recursively compute the height of each subtree, and at each node update a global (or reference) maximum with `leftHeight + rightHeight`. The height of a node is `1 + max(leftHeight, rightHeight)`. For a null node, height is 0. The diameter is the global maximum accumulated over all nodes. Edge cases: empty tree → 0; single node → 0 (leftHeight and rightHeight are 0, sum is 0). Negative values in nodes are irrelevant since we only count edges, not node values. Time complexity is O(n) where n is the number of nodes, since each node is visited once. Space complexity is O(h) for recursion stack, where h is tree height.

#include <algorithm>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper function that returns the height of the subtree rooted at node
// and updates the diameter accumulator passed by reference.
int computeHeightAndDiameter(const TreeNode* node, int& diameter) {
    if (node == nullptr) {
        return 0;
    }
    int leftHeight = computeHeightAndDiameter(node->left, diameter);
    int rightHeight = computeHeightAndDiameter(node->right, diameter);
    // A path through this node as the highest point has length leftHeight + rightHeight
    diameter = std::max(diameter, leftHeight + rightHeight);
    // Height of this node is the longer child height plus one edge to this node
    return std::max(leftHeight, rightHeight) + 1;
}

// Returns the diameter (number of edges on the longest path) of the binary tree.
int treeDiameter(const TreeNode* root) {
    int diameter = 0;
    computeHeightAndDiameter(root, diameter);
    return diameter;
}

#include <cassert>

int main() {
    // Test 1: empty tree
    assert(treeDiameter(nullptr) == 0);

    // Test 2: single node
    TreeNode single(1);
    assert(treeDiameter(&single) == 0);

    // Test 3: two-node tree (root with left child)
    TreeNode n1(1);
    TreeNode n2(2);
    n1.left = &n2;
    assert(treeDiameter(&n1) == 1);

    // Test 4: three-node chain (root, left, left-left) -> diameter 2
    TreeNode a(1);
    TreeNode b(2);
    TreeNode c(3);
    a.left = &b;
    b.left = &c;
    assert(treeDiameter(&a) == 2);

    // Test 5: balanced tree of 3 nodes (root with two children) -> diameter 2
    TreeNode r(1);
    TreeNode l(2);
    TreeNode rr(3);
    r.left = &l;
    r.right = &rr;
    assert(treeDiameter(&r) == 2);

    // Test 6: taller tree where diameter passes through root
    // Structure:
    //        1
    //       / \
    //      2   3
    //     /     \
    //    4       5
    //   /         \
    //  6           7
    // Diameter path: 6-4-2-1-3-5-7 has 6 edges
    TreeNode t1(1);
    TreeNode t2(2);
    TreeNode t3(3);
    TreeNode t4(4);
    TreeNode t5(5);
    TreeNode t6(6);
    TreeNode t7(7);
    t1.left = &t2;
    t1.right = &t3;
    t2.left = &t4;
    t3.right = &t5;
    t4.left = &t6;
    t5.right = &t7;
    assert(treeDiameter(&t1) == 6);

    // Test 7: diameter not passing through root
    //        1
    //       /
    //      2
    //     / \
    //    3   4
    //   /     \
    //  5       6
    // Diameter path: 5-3-2-4-6 has 4 edges (does not include root)
    TreeNode s1(1);
    TreeNode s2(2);
    TreeNode s3(3);
    TreeNode s4(4);
    TreeNode s5(5);
    TreeNode s6(6);
    s1.left = &s2;
    s2.left = &s3;
    s2.right = &s4;
    s3.left = &s5;
    s4.right = &s6;
    assert(treeDiameter(&s1) == 4);

    // Test 8: negative node values don't affect result
    TreeNode neg( -1 );
    TreeNode negLeft( -2 );
    TreeNode negRight( -3 );
    neg.left = &negLeft;
    neg.right = &negRight;
    assert(treeDiameter(&neg) == 2);

    return 0;
}
