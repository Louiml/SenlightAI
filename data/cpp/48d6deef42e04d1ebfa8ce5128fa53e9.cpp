Write a C++ function `bool flipEquiv(TreeNode* root1, TreeNode* root2)` that determines whether two binary trees are "flip equivalent." Two binary trees are considered flip equivalent if, by performing any number of flip operations (where a flip swaps the left and right children of any node), the two trees become identical. The function should take the roots of two binary trees and return `true` if they are flip equivalent, and `false` otherwise. The tree nodes are defined by the `TreeNode` struct with fields `val`, `left`, and `right`. You may assume both trees are valid binary trees with no cycles, and node values may repeat. Handle null trees gracefully (two null trees are flip equivalent; one null and one non-null are not).
The solution uses recursion to compare the two trees structurally while allowing optional left/right swaps at every node. The key idea is that for two non-null nodes to be equivalent, their values must match. Then, we check two possible configurations at each node: either both left subtrees correspond and both right subtrees correspond (no flip), or the left of one corresponds to the right of the other and vice versa (flip). If either configuration is fully satisfied recursively, the trees are flip equivalent. Base cases: if both roots are null, return true; if exactly one is null, return false; if values differ, return false. Edge cases include nodes with only one child (the recursive calls will naturally handle nulls) and duplicate values (the two-branch check handles all arrangements). Time complexity is O(n) in the worst case, where n is the total number of nodes, because each pair of corresponding nodes is visited once per configuration. Space complexity is O(h) for the recursion stack, where h is the tree's height, which in the worst case (skewed tree) is O(n).
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

// Determines if two binary trees are flip equivalent (can be made identical by swapping children of any nodes).
bool flipEquiv(TreeNode* root1, TreeNode* root2) {
    // Both null trees are equivalent
    if (root1 == nullptr && root2 == nullptr) {
        return true;
    }
    // One null and the other non-null are not equivalent
    if (root1 == nullptr || root2 == nullptr) {
        return false;
    }
    // Node values must match
    if (root1->val != root2->val) {
        return false;
    }
    
    // Case 1: no flip at this node — left with left, right with right
    bool noFlip = flipEquiv(root1->left, root2->left) &&
                  flipEquiv(root1->right, root2->right);
    // Case 2: flip at this node — left with right, right with left
    bool flipped = flipEquiv(root1->left, root2->right) &&
                   flipEquiv(root1->right, root2->left);
    
    return noFlip || flipped;
}
#include <cassert>

// Helper to build a single node for testing
TreeNode* makeNode(int val, TreeNode* left = nullptr, TreeNode* right = nullptr) {
    return new TreeNode(val, left, right);
}

// Helper to delete tree recursively
void deleteTree(TreeNode* root) {
    if (root) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
}

int main() {
    // Test 1: Two null trees
    assert(flipEquiv(nullptr, nullptr) == true);

    // Test 2: One null, one non-null
    TreeNode* single = makeNode(1);
    assert(flipEquiv(single, nullptr) == false);
    assert(flipEquiv(nullptr, single) == false);

    // Test 3: Identical trees (no flips needed)
    TreeNode* tree1a = makeNode(1, makeNode(2), makeNode(3));
    TreeNode* tree2a = makeNode(1, makeNode(2), makeNode(3));
    assert(flipEquiv(tree1a, tree2a) == true);

    // Test 4: Trees with a single flip at root
    TreeNode* tree1b = makeNode(1, makeNode(2), makeNode(3));
    TreeNode* tree2b = makeNode(1, makeNode(3), makeNode(2));
    assert(flipEquiv(tree1b, tree2b) == true);

    // Test 5: Trees with nested flips
    TreeNode* tree1c = makeNode(1,
                    makeNode(2, nullptr, makeNode(4)),
                    makeNode(3, makeNode(5), nullptr));
    TreeNode* tree2c = makeNode(1,
                    makeNode(3, nullptr, makeNode(5)),
                    makeNode(2, makeNode(4), nullptr));
    assert(flipEquiv(tree1c, tree2c) == true);

    // Test 6: Non-equivalent trees (different values)
    TreeNode* tree1d = makeNode(1, makeNode(2), makeNode(3));
    TreeNode* tree2d = makeNode(1, makeNode(2), makeNode(4));
    assert(flipEquiv(tree1d, tree2d) == false);

    // Test 7: Non-equivalent trees with same values but different shape
    TreeNode* tree1e = makeNode(1, makeNode(2), nullptr);
    TreeNode* tree2e = makeNode(1, nullptr, makeNode(2));
    // Flip works here? Actually left vs right match after flip: tree1e left=2, tree2e right=2, tree1e right=null, tree2e left=null -> true.
    assert(flipEquiv(tree1e, tree2e) == true);

    // Test 8: Non-equivalent due to structural mismatch
    TreeNode* tree1f = makeNode(1, makeNode(2), makeNode(3));
    TreeNode* tree2f = makeNode(1, makeNode(2, makeNode(4), nullptr), nullptr);
    assert(flipEquiv(tree1f, tree2f) == false);

    // Clean up all allocated memory
    deleteTree(single);
    deleteTree(tree1a);
    deleteTree(tree2a);
    deleteTree(tree1b);
    deleteTree(tree2b);
    deleteTree(tree1c);
    deleteTree(tree2c);
    deleteTree(tree1d);
    deleteTree(tree2d);
    deleteTree(tree1e);
    deleteTree(tree2e);
    deleteTree(tree1f);
    deleteTree(tree2f);

    return 0;
}
