/*
Write a C++ function `int diameterOfBinaryTree(const TreeNode* root)` that takes the root of a binary tree (whose nodes contain an integer `val` and left/right child pointers) and returns the diameter of the tree: the number of edges along the longest path between any two nodes. The path may or may not pass through the root. If the tree is empty, return 0. Your function must not modify the tree. Use the provided `TreeNode` struct definition exactly as given (including its constructors). The solution must compute the diameter efficiently in a single traversal.
*/

#include <algorithm>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

namespace {
// Helper function: returns height of subtree rooted at 'node' and updates 'diameter'.
int heightAndDiameter(const TreeNode* node, int& diameter) {
    if (node == nullptr) {
        return 0;
    }
    int leftHeight = heightAndDiameter(node->left, diameter);
    int rightHeight = heightAndDiameter(node->right, diameter);
    diameter = std::max(diameter, leftHeight + rightHeight);
    return 1 + std::max(leftHeight, rightHeight);
}
}

// Returns the number of edges on the longest path between any two nodes.
int diameterOfBinaryTree(const TreeNode* root) {
    int diameter = 0;
    heightAndDiameter(root, diameter);
    return diameter;
}

#include <cassert>

int main() {
    // Empty tree
    assert(diameterOfBinaryTree(nullptr) == 0);

    // Single node: no edges
    TreeNode single(1);
    assert(diameterOfBinaryTree(&single) == 0);

    // Root with one left child: 1 edge
    TreeNode leftChild(2);
    TreeNode root1(1, &leftChild, nullptr);
    assert(diameterOfBinaryTree(&root1) == 1);

    // Root with two children: path between children goes through root, 2 edges
    TreeNode left(2), right(3);
    TreeNode root2(1, &left, &right);
    assert(diameterOfBinaryTree(&root2) == 2);

    // Skewed tree (chain of 3 nodes): diameter = 2 edges
    TreeNode leaf(3);
    TreeNode mid(2, &leaf, nullptr);
    TreeNode chainRoot(1, &mid, nullptr);
    assert(diameterOfBinaryTree(&chainRoot) == 2);

    // Balanced tree with deeper left subtree; longest path doesn't necessarily go through root
    // Root=1, left=2 (left.left=4, left.right=5), right=3
    // Diameter path: 4->2->5 (2 edges) but also 4->2->1->3 (3 edges), so diameter=3
    TreeNode leftLeaf4(4), rightLeaf5(5);
    TreeNode leftNode(2, &leftLeaf4, &rightLeaf5);
    TreeNode rightNode(3);
    TreeNode balancedRoot(1, &leftNode, &rightNode);
    assert(diameterOfBinaryTree(&balancedRoot) == 3);

    // Deeper subtree not passing through root: root=1, left=2 with left.left=4 with left.left.left=6 (chain), left.right=5
    // Longest path: 6->4->2->5 (4 edges), root right is null, so diameter=4
    TreeNode deepest(6);
    TreeNode node4(4, &deepest, nullptr);
    TreeNode node5(5);
    TreeNode node2(2, &node4, &node5);
    TreeNode root3(1, &node2, nullptr);
    assert(diameterOfBinaryTree(&root3) == 4);

    // Balanced full tree of height 2: diameter = 4 (leaf-to-leaf)
    TreeNode a, b, c, d; // all default val=0
    TreeNode leftSub(0, &a, &b);
    TreeNode rightSub(0, &c, &d);
    TreeNode fullRoot(0, &leftSub, &rightSub);
    assert(diameterOfBinaryTree(&fullRoot) == 4);
}

// The diameter is the maximum of, for every node, the sum of the heights of its left and right subtrees. The standard approach is a recursive depth-first traversal. At each node, compute the height of its left subtree (`lh`) and right subtree (`rh`). The local path length through that node is `lh + rh`, which we compare against a running maximum diameter. The node's own height is `1 + max(lh, rh)`, returned to the parent. Edge cases: an empty tree returns 0; a single node has no edges, so diameter = 0; a chain of length 2 (root with one child) has diameter = 1 (one edge). Since we traverse each node once, time complexity is O(n) where n is the number of nodes. Space complexity is O(h) for the recursion stack, where h is the tree height; in a skewed tree this is O(n), in a balanced tree O(log n). The function should be `const`-correct by taking `const TreeNode*` and not modifying the tree.
