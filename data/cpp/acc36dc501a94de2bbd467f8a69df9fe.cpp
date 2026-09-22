Write a standalone C++ function `isValidBST` that takes a binary search tree root node (defined by the provided `TreeNode` struct) and returns `true` if the tree satisfies the BST property: for every node, all values in its left subtree are strictly less than the node's value, and all values in its right subtree are strictly greater. The function must handle `nullptr` roots (returning `true`), trees with duplicate values (which are invalid), and nodes whose values may be as large as `±10^9` (so the initial bounds cannot simply be `INT_MIN`/`INT_MAX`). You may use the helper approach shown, but the final public function should be named `isValidBST(TreeNode* root)` and must be `const`-correct (i.e., it should not modify the tree). Provide a complete implementation with the `TreeNode` definition included.
The solution uses a recursive helper that carries the allowed lower and upper bounds (`_min` and `_max`) for each node. At each node, we check that the node's value lies strictly within these bounds. Then we recurse on the left subtree with the same lower bound but the node's value as the new upper bound (because all left descendants must be less than the current node). For the right subtree, we recurse with the node's value as the new lower bound (because all right descendants must be greater). The recursion bottoms out at `nullptr`, which is always valid. This approach naturally handles duplicates: if a child has the same value as its parent, the strict inequality check will fail. Edge cases include an empty tree (valid), a single node (valid), and trees with negative values or values near the limit of `int`. To avoid overflow and to safely handle the extremal values, we use `long` for the bounds and set the initial bounds to `-1e10` and `1e10`, which are well outside the typical `int` range. The time complexity is O(n) where n is the number of nodes, because we visit each node exactly once. The space complexity is O(h) for the recursion stack, where h is the tree height (O(log n) for balanced trees, O(n) for skewed trees in the worst case).
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

// Helper function: checks if the subtree rooted at 'node' has all values
// strictly between lower and upper bounds.
bool isValidBSTHelper(long long lower, long long upper, const TreeNode* node) {
    if (node == nullptr) {
        return true; // Empty subtree is valid
    }

    long long val = node->val;
    if (val <= lower || val >= upper) {
        return false; // Violates the strict bound
    }

    // Left subtree must be within [lower, val]; right subtree within [val, upper]
    return isValidBSTHelper(lower, val, node->left) &&
           isValidBSTHelper(val, upper, node->right);
}

// Public function: validates the entire binary search tree.
bool isValidBST(const TreeNode* root) {
    // Use bounds wider than int range to accommodate any legal int value
    const long long NEG_INF = -10000000000LL;
    const long long POS_INF = 10000000000LL;
    return isValidBSTHelper(NEG_INF, POS_INF, root);
}
int main() {
    // Test 1: Empty tree is valid
    assert(isValidBST(nullptr) == true);

    // Test 2: Single node
    TreeNode n1(5);
    assert(isValidBST(&n1) == true);

    // Test 3: Simple valid BST:     2
    //                              / \
    //                             1   3
    TreeNode n3(3);
    TreeNode n2(1);
    TreeNode root(2, &n2, &n3);
    assert(isValidBST(&root) == true);

    // Test 4: Invalid BST:          5
    //                              / \
    //                             1   4
    //                                / \
    //                               3   6   (right subtree has 3 < 5)
    TreeNode n6(6);
    TreeNode n5(4, new TreeNode(3), &n6);
    TreeNode root2(5, new TreeNode(1), n5);
    assert(isValidBST(&root2) == false);

    // Test 5: Duplicate values are invalid
    TreeNode dup_left(5);
    TreeNode dup_root(5, &dup_left, nullptr);
    assert(isValidBST(&dup_root) == false);

    // Test 6: Left subtree contains a value greater than parent (invalid)
    TreeNode bad_left(10);
    TreeNode root3(5, &bad_left, nullptr);
    assert(isValidBST(&root3) == false);

    // Test 7: Extreme values near INT_MAX and INT_MIN
    TreeNode extreme_left(-2147483648);
    TreeNode extreme_right(2147483647);
    TreeNode root4(0, &extreme_left, &extreme_right);
    assert(isValidBST(&root4) == true);

    // Test 8: Skewed right tree that is valid
    TreeNode a(1);
    TreeNode b(2, nullptr, &a); // 2 -> left null, right 1 (invalid, because 1<2 but should be >2)
    assert(isValidBST(&b) == false);

    // Test 9: Skewed left valid tree: 3 -> 2 -> 1
    TreeNode l1(1);
    TreeNode l2(2, &l1, nullptr);
    TreeNode l3(3, &l2, nullptr);
    assert(isValidBST(&l3) == true);
}
