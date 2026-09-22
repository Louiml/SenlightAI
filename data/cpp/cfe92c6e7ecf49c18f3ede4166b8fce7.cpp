// Write a C++ function `bool isSameTree(const TreeNode* p, const TreeNode* q)` that determines whether two binary trees are structurally identical and have the same node values. The `TreeNode` structure is defined as `struct TreeNode { int val; TreeNode* left; TreeNode* right; TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} };`. The function must handle empty trees (nullptr) correctly: two null trees are identical, and a null tree combined with any non-null tree is not identical. The function should be recursive, comparing both nodes' values and then their left and right subtrees. You may assume that the input trees are valid and do not contain cycles.
// The solution uses a recursive depth-first approach that mirrors the tree structure. At each recursive call, we compare the current nodes from both trees. There are four base cases: (1) both are null → identical; (2) one is null and the other is not → not identical; (3) both non-null but have different values → not identical. If all these fail, we recurse on the left children and right children simultaneously. The result is `true` only if both left subtrees match and both right subtrees match. The recursion naturally handles unbalanced trees because it checks nulls at every level. Edge cases include empty trees, single-node trees, trees with different structures but same values (e.g., one with a left child vs. one with a right child), and deep recursion that might risk stack overflow for very skewed trees (but that's acceptable for typical interview-sized inputs). Time complexity is O(n) where n is the total number of nodes in the smaller tree (worst-case both trees have m and n nodes and we compare all overlapping parts, but if they differ early, we stop early). Space complexity is O(h) for the recursion stack, where h is the height of the smaller tree.
#include <cstddef>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Recursively checks if two binary trees are structurally identical and have equal values.
bool isSameTree(const TreeNode* p, const TreeNode* q) {
    // Both null: identical.
    if (p == nullptr && q == nullptr) {
        return true;
    }
    // One null, the other not: not identical.
    if (p == nullptr || q == nullptr) {
        return false;
    }
    // Values differ: not identical.
    if (p->val != q->val) {
        return false;
    }
    // Recurse on left and right subtrees.
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}
#include <cassert>

int main() {
    // Test 1: Both trees empty.
    assert(isSameTree(nullptr, nullptr) == true);

    // Test 2: One empty, one non-empty.
    TreeNode* a = new TreeNode(1);
    assert(isSameTree(a, nullptr) == false);
    assert(isSameTree(nullptr, a) == false);

    // Test 3: Single nodes with same value.
    TreeNode* b = new TreeNode(1);
    assert(isSameTree(a, b) == true);

    // Test 4: Single nodes with different values.
    TreeNode* c = new TreeNode(2);
    assert(isSameTree(a, c) == false);

    // Test 5: Identical three-node trees (root with left and right).
    TreeNode* t1 = new TreeNode(2);
    t1->left = new TreeNode(1);
    t1->right = new TreeNode(3);
    TreeNode* t2 = new TreeNode(2);
    t2->left = new TreeNode(1);
    t2->right = new TreeNode(3);
    assert(isSameTree(t1, t2) == true);

    // Test 6: Same structure but different value in a leaf.
    TreeNode* t3 = new TreeNode(2);
    t3->left = new TreeNode(1);
    t3->right = new TreeNode(4);
    assert(isSameTree(t1, t3) == false);

    // Test 7: Different structure.
    TreeNode* t4 = new TreeNode(2);
    t4->left = new TreeNode(1);
    t4->left->right = new TreeNode(3);
    assert(isSameTree(t1, t4) == false);

    // Test 8: Asymmetric trees.
    TreeNode* t5 = new TreeNode(1);
    t5->left = new TreeNode(2);
    TreeNode* t6 = new TreeNode(1);
    t6->right = new TreeNode(2);
    assert(isSameTree(t5, t6) == false);

    // Test 9: Deep tree with identical values.
    TreeNode* d1 = new TreeNode(1);
    d1->left = new TreeNode(2);
    d1->left->left = new TreeNode(3);
    TreeNode* d2 = new TreeNode(1);
    d2->left = new TreeNode(2);
    d2->left->left = new TreeNode(3);
    assert(isSameTree(d1, d2) == true);

    // Cleanup (not strictly required for assert tests but good practice).
    delete a; delete b; delete c;
    delete t1; delete t2; delete t3; delete t4; delete t5; delete t6;
    delete d1; delete d2;
    return 0;
}
