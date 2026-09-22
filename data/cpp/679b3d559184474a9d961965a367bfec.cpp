Write a C++ function `bool isSubtree(TreeNode* root, TreeNode* subRoot)` that determines whether `subRoot` is a subtree of `root`. A subtree of a binary tree `root` is a tree consisting of a node in `root` and all of its descendants. The function must return `true` if `subRoot` is identical to any subtree of `root`, and `false` otherwise. Both trees are binary trees where each node has an integer `val`, and pointers `left` and `right` (possibly `nullptr`). You may assume both trees are non-empty (i.e., neither root is `nullptr`). The function must be const-correct where possible and include a helper to check whether two trees are exactly identical.

// The solution uses two recursive functions. The helper `isSameTree(p, q)` checks if two trees are structurally identical and have equal node values. It does this by first handling the base cases: if both pointers are `nullptr`, they match; if exactly one is `nullptr`, they do not; then it compares the current node values and recursively checks left and right subtrees. The main `isSubtree(root, subRoot)` first checks if the entire `root` tree is identical to `subRoot` using `isSameTree`. If not, and `root` is not `nullptr`, it recursively checks whether `subRoot` is a subtree of either the left or right child of `root`. If `root` becomes `nullptr` without finding a match, it returns `false`. Edge cases include when both trees have only one node, when the subtree appears multiple times, or when the subtree is larger than the root (which will naturally return `false`). Time complexity is O(m·n) in the worst case, where m and n are the number of nodes in `root` and `subRoot` respectively, because for each node in `root` we might call `isSameTree` which costs O(n). Space complexity is O(max(m, n)) due to recursion stack depth.

#include <algorithm>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper: check if two trees are exactly identical.
bool isSameTree(const TreeNode* p, const TreeNode* q) {
    if (p == nullptr && q == nullptr) return true;
    if (p == nullptr || q == nullptr) return false;
    if (p->val != q->val) return false;
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}

// Main: check if subRoot is a subtree of root.
bool isSubtree(const TreeNode* root, const TreeNode* subRoot) {
    if (root == nullptr) return false;
    if (isSameTree(root, subRoot)) return true;
    return isSubtree(root->left, subRoot) || isSubtree(root->right, subRoot);
}

#include <cassert>

int main() {
    // Tree: root = [3,4,5,1,2,null,null,0]
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(4);
    root->right = new TreeNode(5);
    root->left->left = new TreeNode(1);
    root->left->right = new TreeNode(2);
    root->left->right->left = new TreeNode(0);

    // Subtree: [4,1,2]
    TreeNode* sub1 = new TreeNode(4);
    sub1->left = new TreeNode(1);
    sub1->right = new TreeNode(2);
    assert(isSubtree(root, sub1) == true);

    // Subtree: [5]
    TreeNode* sub2 = new TreeNode(5);
    assert(isSubtree(root, sub2) == true);

    // Subtree: [4,1,3] (wrong value)
    TreeNode* sub3 = new TreeNode(4);
    sub3->left = new TreeNode(1);
    sub3->right = new TreeNode(3);
    assert(isSubtree(root, sub3) == false);

    // Subtree: [1,0,null]
    TreeNode* sub4 = new TreeNode(1);
    sub4->left = new TreeNode(0);
    assert(isSubtree(root, sub4) == true);

    // Subtree equal to full root
    assert(isSubtree(root, root) == true);

    // Subtree larger than root (root = [1], subRoot = [1,2])
    TreeNode* singleRoot = new TreeNode(1);
    TreeNode* bigger = new TreeNode(1);
    bigger->left = new TreeNode(2);
    assert(isSubtree(singleRoot, bigger) == false);

    // Subtree not present because structure differs
    TreeNode* mismatch = new TreeNode(4);
    mismatch->left = new TreeNode(1);
    mismatch->left->left = new TreeNode(2);
    assert(isSubtree(root, mismatch) == false);

    // Cleanup (not strictly necessary for asserts but good practice in full code)
    // For brevity, skip explicit deletion here.

    return 0;
}
