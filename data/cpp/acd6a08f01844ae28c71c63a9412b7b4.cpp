// Write a C++ function `int longestZigZagPath(TreeNode* root)` that accepts the root of a binary tree and returns the length of the longest ZigZag path contained in that tree. A ZigZag path starts at any node and alternates direction at each step: if you moved to a right child, the next move must be to a left child (and vice versa), continuing until no further move is possible. The path length is the number of edges traversed (nodes visited minus 1). A single node has length 0. The tree may be large (up to 50,000 nodes), so the solution must be efficient. The function must be provided as a free function (not a class method) and must be `const`-correct where applicable.
#include <cassert>

int main() {
    // Test 1: Single node tree -> length 0
    TreeNode n1(1);
    assert(longestZigZagPath(&n1) == 0);

    // Test 2: Two nodes (root with left child) -> path root->left length 1
    TreeNode n2_left(2);
    TreeNode n2_root(1, &n2_left, nullptr);
    assert(longestZigZagPath(&n2_root) == 1);

    // Test 3: Root with both children but only one edge each -> length 1
    TreeNode n3_left(2), n3_right(3);
    TreeNode n3_root(1, &n3_left, &n3_right);
    assert(longestZigZagPath(&n3_root) == 1);

    // Test 4: Root with left-left path where direction alternates? Actually left-left is not zigzag, length 1
    TreeNode n4_leftleft(3), n4_left(2, &n4_leftleft, nullptr);
    TreeNode n4_root(1, &n4_left, nullptr);
    assert(longestZigZagPath(&n4_root) == 1);

    // Test 5: A proper zigzag: root(left)->left(right)->right(left)->left = length 3
    TreeNode n5_leaf(5);
    TreeNode n5_right(4, nullptr, &n5_leaf); // 4's right is 5
    TreeNode n5_left(3, &n5_right, nullptr); // 3's left is 4
    TreeNode n5_root(1, &n5_left, nullptr); // root's left is 3
    assert(longestZigZagPath(&n5_root) == 3);

    // Test 6: A longer zigzag: root->right (1)->left (2)->right (3)->left (4) length 4
    TreeNode n6_leaf(5);
    TreeNode n6_n4(4, &n6_leaf, nullptr);
    TreeNode n6_n3(3, nullptr, &n6_n4);
    TreeNode n6_n2(2, &n6_n3, nullptr);
    TreeNode n6_root(1, nullptr, &n6_n2);
    assert(longestZigZagPath(&n6_root) == 4);

    // Test 7: Tree with multiple zigzags, ensure maximum is found
    TreeNode n7_leaf(9);
    TreeNode n7_right(8, &n7_leaf, nullptr); // 8's left is 9
    TreeNode n7_left(7, nullptr, &n7_right); // 7's right is 8
    TreeNode n7_root(6, &n7_left, nullptr); // root's left is 7 -> path root->left->right->left length 3
    // Add another branch: root->right->left->right length 3
    TreeNode n7_branch_leaf(12);
    TreeNode n7_branch_right(11, nullptr, &n7_branch_leaf);
    TreeNode n7_branch_left(10, &n7_branch_right, nullptr);
    n7_root.right = &n7_branch_left;
    assert(longestZigZagPath(&n7_root) == 3);

    // Test 8: Null tree -> 0
    assert(longestZigZagPath(nullptr) == 0);

    return 0;
}
#include <algorithm>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

// Helper function that performs DFS and updates the global answer.
void zigZagDfs(TreeNode* node, int fromLeft, int fromRight, int& answer) {
    if (!node) return;

    // Update the answer with the best path ending at this node.
    answer = std::max(answer, std::max(fromLeft, fromRight));

    // Recurse left: the next move must be to the right child, so fromRight becomes fromLeft+1.
    zigZagDfs(node->left, 0, fromLeft + 1, answer);
    // Recurse right: the next move must be to the left child, so fromLeft becomes fromRight+1.
    zigZagDfs(node->right, fromRight + 1, 0, answer);
}

// Returns the length (number of edges) of the longest ZigZag path in the tree.
int longestZigZagPath(TreeNode* root) {
    if (!root) return 0;
    int answer = 0;
    zigZagDfs(root, 0, 0, answer);
    return answer;
}
// The solution uses a depth-first search (DFS) traversal of the tree. The key idea is to track, for each node, the longest ZigZag path that ends at that node coming from its left child (i.e., the last move was from the parent to this node via a left edge) and similarly for coming from its right child. When processing a node, we update the global answer with these two lengths. Then we recurse:  
// - From the current node, if we go to its left child, the next move in a ZigZag path must be to a right child (since the previous move was left), so we pass `lengthFromLeft + 1` as the length of the path that ends at the left child coming from a right direction, and `0` as the length coming from a left direction because the left child cannot start a path that came from a left edge from its parent.  
// - Symmetrically, when going to the right child, we pass `lengthFromRight + 1` as the length from left, and `0` as the length from right.  
// The recursion terminates at null nodes. The global result is updated at each node with the maximum of the two incoming lengths. Edge cases: single-node tree returns 0; a path that always goes in one direction (e.g., all left children) has length equal to the number of edges because each step alternates direction from a fixed starting direction, but if the tree only has left children, then from the root you go left, then right (if exists) and so on; if no right child exists at any point, the path stops. The algorithm runs in O(N) time, visiting each node once, and uses O(H) recursion stack space, where H is the height (worst-case O(N) for skewed trees).
