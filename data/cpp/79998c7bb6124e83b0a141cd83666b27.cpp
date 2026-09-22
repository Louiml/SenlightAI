/*
Write a C++ function that takes a pointer to the root of a binary tree (where each node contains an integer value and left/right child pointers) and an integer `targetSum`. The function must return `true` if there exists at least one root-to-leaf path such that the sum of all node values along that path equals `targetSum`, and `false` otherwise. A leaf is defined as a node with no left or right children. Assume the tree is non-empty but may have only a single node. Handle cases where `targetSum` is negative or zero, and ensure the function does not modify the tree.
*/

#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper function that performs DFS and updates result if a valid path is found.
void dfsPathSum(const TreeNode* node, int remaining, bool& found) {
    if (found) return;                     // Already found a valid path
    if (node == nullptr) return;           // Reached a null pointer

    // Check if the current node is a leaf and the remaining sum matches.
    if (node->left == nullptr && node->right == nullptr) {
        if (remaining - node->val == 0) {
            found = true;
        }
        return;
    }

    // Recurse on children with updated remaining sum.
    dfsPathSum(node->left, remaining - node->val, found);
    dfsPathSum(node->right, remaining - node->val, found);
}

// Returns true if there is a root-to-leaf path summing to targetSum.
bool hasPathSum(const TreeNode* root, int targetSum) {
    if (root == nullptr) return false;
    bool found = false;
    dfsPathSum(root, targetSum, found);
    return found;
}

#include <cassert>

int main() {
    // Test 1: Single node, target matches
    TreeNode n1(5);
    assert(hasPathSum(&n1, 5) == true);
    assert(hasPathSum(&n1, 6) == false);

    // Test 2: Simple two-level tree
    //     3
    //    / \
    //   9  20
    TreeNode n2(9);
    TreeNode n3(20);
    TreeNode n4(3, &n2, &n3);
    assert(hasPathSum(&n4, 12) == true);   // 3+9
    assert(hasPathSum(&n4, 23) == true);   // 3+20
    assert(hasPathSum(&n4, 32) == false);  // wrong sum

    // Test 3: Negative values
    //     -2
    //    / \
    //  -3  -4
    TreeNode n5(-3);
    TreeNode n6(-4);
    TreeNode n7(-2, &n5, &n6);
    assert(hasPathSum(&n7, -5) == true);   // -2 + -3
    assert(hasPathSum(&n7, -6) == true);   // -2 + -4
    assert(hasPathSum(&n7, 0) == false);

    // Test 4: Path only goes left
    //     1
    //    /
    //   2
    //  /
    // 3
    TreeNode n8(3);
    TreeNode n9(2, &n8, nullptr);
    TreeNode n10(1, &n9, nullptr);
    assert(hasPathSum(&n10, 6) == true);   // 1+2+3
    assert(hasPathSum(&n10, 3) == false);

    // Test 5: Root with one child only (right)
    //     10
    //       \
    //        5
    TreeNode n11(5);
    TreeNode n12(10, nullptr, &n11);
    assert(hasPathSum(&n12, 15) == true);  // 10+5
    assert(hasPathSum(&n12, 10) == false);

    // Test 6: Empty tree (nullptr root) should return false
    assert(hasPathSum(nullptr, 0) == false);

    // Test 7: Deeper tree with multiple paths
    //       5
    //      / \
    //     4   8
    //    /   / \
    //   11  13  4
    //  /  \      \
    // 7    2      1
    TreeNode n13(7);
    TreeNode n14(2);
    TreeNode n15(11, &n13, &n14);
    TreeNode n16(13);
    TreeNode n17(1);
    TreeNode n18(4, nullptr, &n17);
    TreeNode n19(4, &n15, nullptr);
    TreeNode n20(8, &n16, &n18);
    TreeNode n21(5, &n19, &n20);
    assert(hasPathSum(&n21, 22) == true);  // 5+4+11+2
    assert(hasPathSum(&n21, 27) == true);  // 5+8+13+1 (leaf 1)
    assert(hasPathSum(&n21, 26) == true);  // 5+8+4+9? wait, but leaf 1 gives 18, leaf 4 gives 18? no, 5+8+4+1=18; 5+8+4+? Actually 5+8+13=26? no 5+8+13=26 (leaf 13 is a leaf? no, 13 has no children? yes it is a leaf in this test? Actually 13 is a leaf because it has no children.) So check 26 true.
    assert(hasPathSum(&n21, 17) == false); // no such path

    return 0;
}

// The solution uses a depth-first search (DFS) recursive helper that tracks the remaining sum needed. At each node, subtract the node's value from the current remaining sum. If the node is a leaf (both children null), check whether the remaining sum is exactly zero; if so, set a boolean flag to true. Otherwise, recursively explore the left and right subtrees. Prune recursion early once the flag is true to avoid unnecessary traversal. Edge cases include: an empty tree returns false (though the problem statement says non-empty, the function should still handle null root gracefully), a single-node tree where the node value must equal the target sum, and paths that go only left or only right (i.e., a "path" is not necessarily balanced). Time complexity is O(N), where N is the number of nodes, because in the worst case we visit every node. Space complexity is O(H) for the call stack, where H is the tree height (O(N) for a skewed tree, O(log N) for a balanced tree).
