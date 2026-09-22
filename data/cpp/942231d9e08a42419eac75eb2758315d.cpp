// Write a C++ function named `isBalancedBinaryTree` that takes a pointer to the root of a binary tree (where each node stores an integer value and has `left` and `right` child pointers, using the given `TreeNode` structure) and returns a `bool` indicating whether the tree is height‑balanced. A binary tree is height‑balanced if, for every node, the absolute difference between the heights of its left and right subtrees is at most 1. The function must handle an empty tree (returning `true`) and a tree with only one node (also `true`). The solution should be efficient and avoid redundant height computations.
The problem is a classic tree traversal task. The main algorithm computes the height of a subtree while simultaneously checking the balance condition. A naive approach would compute the height of each subtree separately for every node, leading to O(n²) time. Instead, we can use a single post‑order traversal where each recursive call returns the height of the subtree and also indicates whether that subtree is balanced. We can implement a helper function that returns the height of the subtree, but outputs `-1` if the subtree is unbalanced. For a given node, we recursively obtain the left and right heights. If either is `-1`, the subtree is unbalanced, so we return `-1`. Otherwise, if the absolute difference between the left and right heights exceeds 1, we return `-1`. If all is well, we return `max(leftHeight, rightHeight) + 1` as the height of the current subtree. The top‑level function simply calls this helper and checks whether the returned value is not `-1`. Edge cases: empty tree (returns `true`), single node (height 0, balanced), left‑skewed or right‑skewed trees of height > 1 (unbalanced), and balanced trees of any size. Time complexity: O(n), because each node is visited exactly once during the post‑order traversal. Space complexity: O(h) for the recursion stack, where h is the height of the tree (worst‑case O(n) for a skewed tree, O(log n) for a balanced tree).
#include <algorithm>
#include <cstdlib>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper function that returns the height of the subtree rooted at 'node'.
// If the subtree is not balanced, it returns -1.
int heightIfBalanced(const TreeNode* node) {
    if (node == nullptr) {
        return 0;
    }
    int leftHeight = heightIfBalanced(node->left);
    if (leftHeight == -1) {
        return -1;
    }
    int rightHeight = heightIfBalanced(node->right);
    if (rightHeight == -1) {
        return -1;
    }
    if (std::abs(leftHeight - rightHeight) > 1) {
        return -1;
    }
    return std::max(leftHeight, rightHeight) + 1;
}

// Returns true if the binary tree is height-balanced.
bool isBalancedBinaryTree(const TreeNode* root) {
    return heightIfBalanced(root) != -1;
}
#include <cassert>

int main() {
    // Test 1: Empty tree
    assert(isBalancedBinaryTree(nullptr) == true);

    // Test 2: Single node
    TreeNode n1(1);
    assert(isBalancedBinaryTree(&n1) == true);

    // Test 3: Skewed tree (left chain of height 2)
    TreeNode a(1), b(2), c(3);
    a.left = &b;
    b.left = &c;
    assert(isBalancedBinaryTree(&a) == false);

    // Test 4: Balanced tree of height 2
    TreeNode d(1), e(2), f(3), g(4), h(5);
    d.left = &e;
    d.right = &f;
    e.left = &g;
    f.right = &h;
    assert(isBalancedBinaryTree(&d) == true);

    // Test 5: Tree with a right chain of height 2
    TreeNode p(1), q(2), r(3);
    p.right = &q;
    q.right = &r;
    assert(isBalancedBinaryTree(&p) == false);

    // Test 6: Tree with difference 1 at root but unbalanced deeper
    TreeNode x(1), y(2), z(3), w(4), v(5), u(6), t(7);
    x.left = &y;
    x.right = &z;
    y.left = &w;
    y.right = &v;
    z.left = &u;
    z.right = &t;
    // Here left subtree height 2, right subtree height 2, balanced at root
    // but all leaves are at depth 2, still balanced
    assert(isBalancedBinaryTree(&x) == true);

    // Test 7: Unbalanced because left subtree length 3, right subtree length 1
    TreeNode m(1), n(2), o(3), q2(4), r2(5), s(6);
    m.left = &n;
    m.right = &o;
    n.left = &q2;
    q2.left = &r2; // left chain height 3
    o.right = &s;  // right height 1 (but |3-1|=2)
    assert(isBalancedBinaryTree(&m) == false);

    return 0;
}
