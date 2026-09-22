/*
Implement a C++ function that determines whether one binary tree is a subtree of another binary tree, given pointers to their root nodes. A tree `t` is considered a subtree of tree `s` if there exists a node in `s` such that the subtree rooted at that node is structurally identical to `t`, meaning the node values and the entire structure (including all descendants) match exactly. The function should handle `nullptr` roots correctly: an empty tree is not considered a subtree of any tree, and no tree is a subtree of an empty tree. You may assume the trees contain unique or duplicate values, and the function must work correctly for both cases. The trees are represented using a standard node structure with `val`, `left`, and `right` members. The function should be `const`-safe where possible, take const pointers to the nodes, and return a `bool`.
*/

#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper: check if two subtrees are structurally identical.
static bool isIdentical(const TreeNode* a, const TreeNode* b) {
    if (a == nullptr && b == nullptr) {
        return true;
    }
    if (a == nullptr || b == nullptr) {
        return false;
    }
    return (a->val == b->val) &&
           isIdentical(a->left, b->left) &&
           isIdentical(a->right, b->right);
}

// Helper: traverse the main tree and search for a matching subtree.
static bool findSubtree(const TreeNode* s, const TreeNode* t) {
    if (s == nullptr) {
        return false;
    }
    if (isIdentical(s, t)) {
        return true;
    }
    return findSubtree(s->left, t) || findSubtree(s->right, t);
}

// Public function: check if t is a subtree of s.
bool isSubtree(const TreeNode* s, const TreeNode* t) {
    // An empty tree is not considered a subtree.
    if (s == nullptr || t == nullptr) {
        return false;
    }
    return findSubtree(s, t);
}

#include <cassert>
#include <cstddef>

// (TreeNode definition and solution functions are included here)

int main() {
    // Test 1: Exact match, t is the whole tree s.
    TreeNode* s1 = new TreeNode(3);
    s1->left = new TreeNode(4);
    s1->right = new TreeNode(5);
    s1->left->left = new TreeNode(1);
    s1->left->right = new TreeNode(2);
    TreeNode* t1 = new TreeNode(4);
    t1->left = new TreeNode(1);
    t1->right = new TreeNode(2);
    assert(isSubtree(s1, t1) == true);

    // Test 2: No matching subtree.
    TreeNode* t2 = new TreeNode(4);
    t2->left = new TreeNode(1);
    t2->right = new TreeNode(3);
    assert(isSubtree(s1, t2) == false);

    // Test 3: Empty t should return false.
    assert(isSubtree(s1, nullptr) == false);

    // Test 4: Empty s with non-empty t returns false.
    assert(isSubtree(nullptr, t1) == false);

    // Test 5: Both empty returns false.
    assert(isSubtree(nullptr, nullptr) == false);

    // Test 6: t is a leaf node present in s.
    TreeNode* t3 = new TreeNode(5);
    assert(isSubtree(s1, t3) == true);

    // Test 7: Duplicate values, t appears multiple times – should return true.
    TreeNode* s2 = new TreeNode(1);
    s2->left = new TreeNode(2);
    s2->right = new TreeNode(2);
    s2->left->left = new TreeNode(3);
    s2->left->right = new TreeNode(4);
    s2->right->left = new TreeNode(3);
    s2->right->right = new TreeNode(4);
    TreeNode* t4 = new TreeNode(2);
    t4->left = new TreeNode(3);
    t4->right = new TreeNode(4);
    assert(isSubtree(s2, t4) == true);

    // Test 8: Similar values but different structure – should return false.
    TreeNode* t5 = new TreeNode(2);
    t5->left = new TreeNode(4);
    t5->right = new TreeNode(3);
    assert(isSubtree(s2, t5) == false);
}

// The solution uses a recursive approach with two helper functions. The first helper, `isIdentical`, checks whether two given subtrees are structurally identical by comparing node values and recursively comparing left and right children. It handles the base cases: if both nodes are `nullptr`, they match; if exactly one is `nullptr`, they do not match. The second helper, `findSubtree`, traverses the main tree `s` depth-first. At each node, it checks whether the subtree rooted at that node is identical to `t` using the first helper. If not, it recursively searches the left and right children. The main function simply calls the traversal helper. Key edge cases include: an empty `t` (which should return `false` because an empty tree is not a subtree in this definition), an empty `s` with non-empty `t` (returns `false`), and a case where `t` matches `s` exactly (returns `true`). The time complexity is \(O(m \cdot n)\) in the worst case, where \(m\) is the number of nodes in `s` and \(n` is the number of nodes in `t`, because for each node in `s` we may perform a full comparison against `t`. The auxiliary space complexity is \(O(h_s + h_t)\) due to recursion depth, where \(h_s\) and \(h_t` are the heights of the two trees, and in the worst case this is \(O(m + n)\) for skewed trees.
