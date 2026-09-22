// Write a C++ function `TreeNode* subtreeWithAllDeepest(TreeNode* root)` that, given the root of a non-empty binary tree where each node stores an integer value, returns the root of the smallest subtree that contains all of the deepest nodes (i.e., nodes at the maximum depth from the root). If there are multiple deepest nodes, the returned subtree must have as its root the lowest common ancestor of all deepest nodes. The tree is defined by the provided `TreeNode` struct with fields `val`, `left`, and `right`. The function should be const-correct (i.e., it should not modify the tree), and it must handle edge cases: a single-node tree (the root itself is the answer), a tree where a deepest node is unique (that node is the answer), and balanced or skewed trees. You may use any auxiliary data structures, but the solution must avoid modifying the original tree structure.
The key observation is that the answer is the lowest common ancestor of all deepest nodes. A natural recursive approach computes, for each node, a pair `(subtree root, depth)` representing: (1) the root of the smallest subtree that contains all deepest nodes *within that node's entire subtree*, and (2) the maximum depth of any node in that subtree (counting edges, so a leaf has depth 0 relative to itself? Actually depth here is measured from the current node as root, i.e., height). For a `nullptr`, return `{nullptr, 0}` (depth 0 for an empty subtree, but the subtree root is null). For a leaf, both left and right are null, so left depth = right depth = 0, and since they are equal, return `{leaf, 1}`. For a general node, compute left and right results recursively. Let `d1` and `d2` be the depths from those results. If `d1 > d2`, all deepest nodes are in the left subtree, so the answer for this node is the left result (but with depth incremented by 1 because we are adding this node as parent). Similarly for `d1 < d2`. If `d1 == d2`, the deepest nodes exist in both subtrees at equal depths, so the current node is their lowest common ancestor, and we return `{current node, d1+1}`. This recursion runs in `O(n)` time and uses `O(h)` auxiliary space for recursion stack, where `h` is the tree height (could be `O(n)` in a skewed tree). Edge cases: empty root? Problem states non-empty, but we handle nullptr gracefully. A single node: both children null, so depths equal (0 vs 0), return the node itself with depth 1, which is correct. Unique deepest node: one subtree higher, so we propagate that subtree root. No modification to tree is done, so const-correctness is respected by taking `TreeNode*` (non-const) but not mutating; you can also take `const TreeNode*` if desired, but the function signature matches the snippet.
#include <utility> // for std::pair

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper type: (subtree root, height of that subtree)
using SubtreeInfo = std::pair<TreeNode*, int>;

// Recursively compute the subtree containing all deepest nodes and its height.
SubtreeInfo dfsDeepest(TreeNode* node) {
    if (node == nullptr) {
        return {nullptr, 0};
    }
    SubtreeInfo leftInfo = dfsDeepest(node->left);
    SubtreeInfo rightInfo = dfsDeepest(node->right);
    int leftHeight = leftInfo.second;
    int rightHeight = rightInfo.second;
    if (leftHeight > rightHeight) {
        return {leftInfo.first, leftHeight + 1};
    }
    if (leftHeight < rightHeight) {
        return {rightInfo.first, rightHeight + 1};
    }
    // Both sides have equal height, so current node is the LCA.
    return {node, leftHeight + 1};
}

// Public function: returns root of the smallest subtree containing all deepest nodes.
TreeNode* subtreeWithAllDeepest(TreeNode* root) {
    return dfsDeepest(root).first;
}
#include <cassert>

// Helper to create a leaf node.
TreeNode* leaf(int val) {
    return new TreeNode(val);
}

// Helper to create an internal node.
TreeNode* node(int val, TreeNode* left, TreeNode* right) {
    return new TreeNode(val, left, right);
}

// Helper to free the tree (not strictly necessary for the test but good practice).
void deleteTree(TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    // Test 1: Single node
    TreeNode* t1 = leaf(5);
    assert(subtreeWithAllDeepest(t1) == t1);
    deleteTree(t1);

    // Test 2: Two nodes, deepest is the left child
    TreeNode* t2 = node(1, leaf(2), nullptr);
    TreeNode* deepest2 = t2->left;
    assert(subtreeWithAllDeepest(t2) == deepest2);
    deleteTree(t2);

    // Test 3: Two deepest nodes in both subtrees
    // Tree:     1
    //         /   \
    //        2     3
    TreeNode* t3 = node(1, leaf(2), leaf(3));
    assert(subtreeWithAllDeepest(t3) == t3); // root is LCA
    deleteTree(t3);

    // Test 4: Three levels, deepest nodes in left subtree only
    // Tree:        1
    //            /   \
    //           2     3
    //          /
    //         4
    TreeNode* t4 = node(1, node(2, leaf(4), nullptr), leaf(3));
    TreeNode* deepest4 = t4->left->left; // node 4
    assert(subtreeWithAllDeepest(t4) == deepest4);
    deleteTree(t4);

    // Test 5: Deepest nodes in both subtrees at level 3, LCA is root's left child
    // Tree:         1
    //            /     \
    //           2       3
    //          / \       \
    //         4   5       6
    TreeNode* t5 = node(1,
                        node(2, leaf(4), leaf(5)),
                        node(3, nullptr, leaf(6)));
    TreeNode* expected5 = t5->left; // node 2 is LCA of nodes 4,5,6
    assert(subtreeWithAllDeepest(t5) == expected5);
    deleteTree(t5);

    // Test 6: Skewed tree (only right children), deepest is the last node
    // Tree: 1 -> 2 -> 3 -> 4
    TreeNode* last = leaf(4);
    TreeNode* t6 = node(1, nullptr, node(2, nullptr, node(3, nullptr, last)));
    assert(subtreeWithAllDeepest(t6) == last);
    deleteTree(t6);

    // Test 7: Deepest node unique but not unique subtree? Just check single deepest.
    // Tree:     1
    //          / \
    //         2   3
    //              \
    //               4
    TreeNode* t7 = node(1, leaf(2), node(3, nullptr, leaf(4)));
    TreeNode* deepest7 = t7->right->right; // node 4
    assert(subtreeWithAllDeepest(t7) == deepest7);
    deleteTree(t7);

    // Test 8: Balanced tree all leaves at same depth, root is answer
    // Tree:       1
    //           /   \
    //          2     3
    //         / \   / \
    //        4   5 6   7
    TreeNode* t8 = node(1, node(2, leaf(4), leaf(5)), node(3, leaf(6), leaf(7)));
    assert(subtreeWithAllDeepest(t8) == t8);
    deleteTree(t8);

    return 0;
}
