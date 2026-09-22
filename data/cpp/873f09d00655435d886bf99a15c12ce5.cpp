Write a standalone C++ function `bool isSubtreeSymmetricCheck(const TreeNode* root, const TreeNode* subRoot)` that determines whether a given binary tree `subRoot` is a subtree of another binary tree `root`. A subtree of `root` is defined as a node of `root` (including the root itself) such that all descendants of that node form an identical tree to `subRoot` in both structure and node values. The function must handle empty trees: an empty `subRoot` is considered a subtree of any tree (including an empty `root`), but a non-empty `subRoot` is never a subtree of an empty `root`. Assume the `TreeNode` struct is defined as `struct TreeNode { int val; TreeNode *left; TreeNode *right; TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} };`. Use a recursive "same tree" comparison to check each candidate node in `root`. Implement with `const` correctness and do not modify the input trees.

// The main algorithm uses two recursive functions: `isSameTree` (or equivalently `isIdentical`) and the main `isSubtree` function. The idea is to traverse every node in `root` and at each node check whether the tree rooted at that node is identical to `subRoot`. For recursion, `isSubtree(root, subRoot)` returns `true` if:  
// - `root` is `nullptr` and `subRoot` is `nullptr` → empty subtree matches empty tree.  
// - Otherwise, if `root` is `nullptr`, return `false` (non-empty subRoot cannot match).  
// - Then check if `isSameTree(root, subRoot)` is true; if so return `true`.  
// - Otherwise, recursively check `isSubtree(root->left, subRoot) || isSubtree(root->right, subRoot)`.  
//
// The `isSameTree` function compares two trees node by node: if both are null, true; if one is null, false; if values differ, false; otherwise recurse on left and right subtrees. Edge cases include: `subRoot` being a leaf, `root` being a single node identical to `subRoot`, `subRoot` appearing multiple times, and `subRoot` being a path deeper in the tree. Time complexity is worst-case \(O(n \cdot m)\) where \(n\) is the number of nodes in `root` and \(m\) in `subRoot`, because for each node in `root` we may call `isSameTree` which traverses up to `m` nodes. Space complexity is \(O(\max(h_n, h_m))\) due to recursion stack, where \(h_n\) and \(h_m` are heights, worst-case \(O(n + m)\) for skewed trees.

#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper: check if two trees are identical.
bool isIdentical(const TreeNode* t1, const TreeNode* t2) {
    if (t1 == nullptr && t2 == nullptr) return true;
    if (t1 == nullptr || t2 == nullptr) return false;
    if (t1->val != t2->val) return false;
    return isIdentical(t1->left, t2->left) && isIdentical(t1->right, t2->right);
}

// Main function: return true if subRoot is a subtree of root.
bool isSubtreeSymmetricCheck(const TreeNode* root, const TreeNode* subRoot) {
    // If subRoot is null, it matches any tree (including null root).
    if (subRoot == nullptr) return true;
    // If root is null but subRoot is not, no match.
    if (root == nullptr) return false;
    // Check if current root matches subRoot, or recurse on children.
    if (isIdentical(root, subRoot)) return true;
    return isSubtreeSymmetricCheck(root->left, subRoot) || 
           isSubtreeSymmetricCheck(root->right, subRoot);
}

#include <cassert>

int main() {
    // Test 1: simple subtree (root has right child 5, subRoot is 5)
    TreeNode* tree1 = new TreeNode(1);
    tree1->left = new TreeNode(2);
    tree1->right = new TreeNode(3);
    tree1->right->right = new TreeNode(5);
    TreeNode* sub1 = new TreeNode(5);
    assert(isSubtreeSymmetricCheck(tree1, sub1) == true);

    // Test 2: subRoot not present
    TreeNode* sub2 = new TreeNode(4);
    assert(isSubtreeSymmetricCheck(tree1, sub2) == false);

    // Test 3: identical trees
    TreeNode* tree3 = new TreeNode(1);
    tree3->left = new TreeNode(2);
    tree3->right = new TreeNode(3);
    TreeNode* sub3 = new TreeNode(1);
    sub3->left = new TreeNode(2);
    sub3->right = new TreeNode(3);
    assert(isSubtreeSymmetricCheck(tree3, sub3) == true);

    // Test 4: empty subRoot is subtree of any tree
    assert(isSubtreeSymmetricCheck(tree1, nullptr) == true);

    // Test 5: non-empty subRoot is not subtree of empty root
    assert(isSubtreeSymmetricCheck(nullptr, sub1) == false);

    // Test 6: both empty
    assert(isSubtreeSymmetricCheck(nullptr, nullptr) == true);

    // Test 7: subRoot is leaf but only deeper value matches structure partially
    TreeNode* tree7 = new TreeNode(1);
    tree7->left = new TreeNode(2);
    tree7->right = new TreeNode(2);
    TreeNode* sub7 = new TreeNode(2);
    assert(isSubtreeSymmetricCheck(tree7, sub7) == true);

    // Test 8: root has one node matching subRoot
    TreeNode* tree8 = new TreeNode(42);
    TreeNode* sub8 = new TreeNode(42);
    assert(isSubtreeSymmetricCheck(tree8, sub8) == true);

    // Test 9: subRoot has extra child, not a subtree
    TreeNode* sub9 = new TreeNode(2);
    sub9->left = new TreeNode(0);
    assert(isSubtreeSymmetricCheck(tree7, sub9) == false);

    // Test 10: deep nested subtree
    TreeNode* tree10 = new TreeNode(1);
    tree10->left = new TreeNode(2);
    tree10->left->left = new TreeNode(3);
    tree10->left->left->right = new TreeNode(4);
    TreeNode* sub10 = new TreeNode(3);
    sub10->right = new TreeNode(4);
    assert(isSubtreeSymmetricCheck(tree10, sub10) == true);

    return 0;
}
