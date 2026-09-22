// Write a C++ function `int treeDiameter(TreeNode* root)` that accepts a pointer to the root of a binary tree (where each `TreeNode` has an integer `val` and pointers `left` and `right`) and returns the diameter of the tree. The diameter is defined as the number of edges on the longest path between any two nodes (the path may or may not pass through the root). Assume the tree may be empty (containing zero nodes), have only one node, be skewed in either direction, or be perfectly balanced. You must implement the function using a recursive depth-first search, and your code must compile and run in C++17 or later. The `TreeNode` structure is provided as follows (do not modify it):
// ```cpp
// struct TreeNode {
//     int val;
//     TreeNode *left;
//     TreeNode *right;
//     TreeNode() : val(0), left(nullptr), right(nullptr) {}
//     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
//     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
// };
// ```
// Your function should be a free function (not a class method) and must be self-contained. You may add helper functions inside an anonymous namespace if needed.

// The core idea is to perform a post-order traversal of the tree. For each node, we compute the height (longest downward path in terms of edges) of its left and right subtrees. The diameter passing through that node is the sum of the left height and right height (because the path goes from the deepest leaf in the left subtree, through the node, to the deepest leaf in the right subtree). We update a global maximum with this sum. Then we return the height of the current node as `1 + max(left_height, right_height)` (if the node is null, we return `-1` to make leaf heights equal to 0 edges). The diameter of the whole tree is the maximum sum encountered. Edge cases: an empty tree (root == nullptr) has diameter 0; a single node (leaf) has diameter 0 because no edges exist; a skewed tree's diameter is simply its height. Time complexity is O(n) where n is the number of nodes, as each node is visited exactly once. Space complexity is O(h) for the recursion stack, where h is the tree height (worst-case O(n) for a skewed tree, average O(log n) for balanced).

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
    // Helper function: returns the height (number of edges) of the subtree rooted at cur,
    // and updates the diameter candidate through this node.
    int dfsHeight(TreeNode* cur, int& diameter) {
        if (cur == nullptr) {
            return -1; // null node contributes -1 so leaf height = 0
        }
        int leftHeight = dfsHeight(cur->left, diameter);
        int rightHeight = dfsHeight(cur->right, diameter);
        diameter = std::max(diameter, leftHeight + rightHeight + 2); // edges through cur
        return 1 + std::max(leftHeight, rightHeight);
    }
}

// Returns the diameter (number of edges) of the binary tree rooted at root.
int treeDiameter(TreeNode* root) {
    int result = 0;
    if (root != nullptr) {
        dfsHeight(root, result);
    }
    return result;
}

#include <cassert>

int main() {
    // Test 1: empty tree
    TreeNode* nullRoot = nullptr;
    assert(treeDiameter(nullRoot) == 0);

    // Test 2: single node
    TreeNode single(1);
    assert(treeDiameter(&single) == 0);

    // Test 3: two nodes (root with left child)
    TreeNode leftChild(2);
    TreeNode root2(1, &leftChild, nullptr);
    assert(treeDiameter(&root2) == 1);

    // Test 4: three nodes balanced (root with both children)
    TreeNode left3(2);
    TreeNode right3(3);
    TreeNode root3(1, &left3, &right3);
    assert(treeDiameter(&root3) == 2);

    // Test 5: skewed right chain of 4 nodes: diameter = 3 edges
    TreeNode n4(4);
    TreeNode n3(3, nullptr, &n4);
    TreeNode n2(2, nullptr, &n3);
    TreeNode n1(1, nullptr, &n2);
    assert(treeDiameter(&n1) == 3);

    // Test 6: tree where diameter does not pass through root
    // Root 1 with left subtree of height 2 and right subtree height 1,
    // but deeper inside left subtree there's a longer path.
    // Build: root=1, left=2 (with left=4, right=5), right=3.
    // Left subtree has diameter 2 (between 4 and 5), plus path 4-2-1-3 = 3 edges,
    // overall diameter = 3.
    TreeNode left6_child1(4);
    TreeNode left6_child2(5);
    TreeNode left6(2, &left6_child1, &left6_child2);
    TreeNode right6(3);
    TreeNode root6(1, &left6, &right6);
    assert(treeDiameter(&root6) == 3);

    // Test 7: deep left subtree causing large diameter
    // Build a chain: 1 - 2 - 3 - 4 (left children only), and add a right leaf to node 2.
    // Diameter is from node 4 up to node 2 then to right leaf = 4 edges.
    TreeNode leaf(5);
    TreeNode node4(4);
    TreeNode node3(3, &node4, nullptr);
    TreeNode node2(2, &node3, &leaf);
    TreeNode root7(1, &node2, nullptr);
    assert(treeDiameter(&root7) == 4);

    return 0;
}
