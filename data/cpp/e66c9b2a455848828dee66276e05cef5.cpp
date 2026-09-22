Write a standalone C++ function named `maximumDepth` that accepts a pointer to the root of a binary tree (using the provided `TreeNode` struct definition) and returns the maximum depth of the tree as an `int`. The maximum depth is defined as the number of nodes along the longest path from the root node down to the farthest leaf node. The function must handle an empty tree (null root) by returning `0`, and it must work correctly for trees with only one node, unbalanced trees (e.g., a chain), and balanced trees. The function should be const-correct: it must not modify the tree and should take the root pointer as a `const TreeNode*` parameter. Provide a complete, self-contained implementation with necessary headers, and ensure the function is public and callable from external code.

#include <cassert>

int main() {
    // Test 1: Empty tree -> depth 0
    assert(maximumDepth(nullptr) == 0);

    // Test 2: Single node -> depth 1
    TreeNode n1(5);
    assert(maximumDepth(&n1) == 1);

    // Test 3: Balanced tree of height 3
    //       1
    //      / \
    //     2   3
    //    / \   \
    //   4   5   6
    TreeNode n6(6);
    TreeNode n5(5);
    TreeNode n4(4);
    TreeNode n3(3, nullptr, &n6);
    TreeNode n2(2, &n4, &n5);
    TreeNode n1_root(1, &n2, &n3);
    assert(maximumDepth(&n1_root) == 3);

    // Test 4: Left-skewed tree (chain) -> depth equals number of nodes (4)
    TreeNode a(1);
    TreeNode b(2, &a, nullptr);
    TreeNode c(3, &b, nullptr);
    TreeNode d(4, &c, nullptr);
    assert(maximumDepth(&d) == 4);

    // Test 5: Tree with only right children -> depth 3
    TreeNode x(1, nullptr, nullptr);
    TreeNode y(2, nullptr, &x);
    TreeNode z(3, nullptr, &y);
    assert(maximumDepth(&z) == 3);

    // Test 6: Unbalanced but with both sides -> depth determined by left subtree
    //       10
    //      /  
    //     5   
    //      \
    //       7
    TreeNode t3(7);
    TreeNode t2(5, nullptr, &t3);
    TreeNode t1(10, &t2, nullptr);
    assert(maximumDepth(&t1) == 3);

    // Test 7: Large balanced-ish tree (not exhaustive but sanity check)
    //       1
    //      / \
    //     2   3
    //    / 
    //   4   
    TreeNode s4(4);
    TreeNode s2(2, &s4, nullptr);
    TreeNode s3(3);
    TreeNode s1(1, &s2, &s3);
    assert(maximumDepth(&s1) == 3);

    // All tests passed
    return 0;
}

#include <algorithm> // for std::max

// Definition for a binary tree node (as provided in the task).
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Returns the maximum depth (number of nodes on the longest root-to-leaf path) of a binary tree.
// Takes a const pointer to the root to guarantee the tree is not modified.
int maximumDepth(const TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    int leftDepth = maximumDepth(root->left);
    int rightDepth = maximumDepth(root->right);
    return 1 + std::max(leftDepth, rightDepth);
}

// The solution uses a recursive depth-first search approach. For a given node, the maximum depth of the subtree rooted at that node is `1` (for the node itself) plus the maximum of the depths of its left and right subtrees. The base case is when the node is `nullptr`, meaning there is no subtree, so the depth is `0`. This recursive definition directly follows the problem: the depth of an empty tree is 0, and the depth of a non-empty tree is 1 plus the larger of the depths of its children.  
//
// Edge cases include: an empty tree (`root == nullptr`) returns 0; a single-node tree returns 1; a skewed tree (e.g., only left children) correctly accumulates depth along that chain; and a balanced tree returns the height of the taller subtree. Since the function only reads node pointers and does not modify values, declaring the parameter as `const TreeNode*` is appropriate and enforces non-mutation.  
//
// Time complexity is O(n), where n is the number of nodes, because each node is visited exactly once in the recursion. Space complexity is O(h) in the worst case due to the recursion call stack, where h is the height of the tree (which can be n for a skewed tree, or log n for a balanced tree). No additional auxiliary data structures are used beyond the implicit recursion stack.
