// Given two binary trees represented by their root nodes, write a C++ function `bool sameTree(const TreeNode* p, const TreeNode* q)` that determines whether the trees are structurally identical and have the same node values at corresponding positions. The trees are composed of `TreeNode` objects with integer values and left/right child pointers. An empty tree (nullptr root) is considered a valid tree; two empty trees are identical, while an empty tree and a non-empty tree are not. The function must handle unbalanced trees, trees with duplicate values, and large inputs efficiently. The comparison must be recursive, checking nodes in a depth-first manner. Do not modify the input trees. Use `const` pointers to indicate that the trees are not modified.
#include <cassert>

int main() {
    // Test 1: Both empty
    assert(sameTree(nullptr, nullptr) == true);

    // Test 2: One empty, one not
    TreeNode single(5);
    assert(sameTree(nullptr, &single) == false);
    assert(sameTree(&single, nullptr) == false);

    // Test 3: Identical single-node trees
    TreeNode a(1);
    TreeNode b(1);
    assert(sameTree(&a, &b) == true);

    // Test 4: Same structure, different value
    TreeNode c(2);
    assert(sameTree(&a, &c) == false);

    // Test 5: More complex identical trees
    // Tree1:   2        Tree2:   2
    //         / \              / \
    //        1   3            1   3
    TreeNode t1_3(3);
    TreeNode t1_1(1);
    TreeNode t1_2(2, &t1_1, &t1_3);
    TreeNode t2_3(3);
    TreeNode t2_1(1);
    TreeNode t2_2(2, &t2_1, &t2_3);
    assert(sameTree(&t1_2, &t2_2) == true);

    // Test 6: Same values, different structure
    // Tree1:   2        Tree2:   2
    //         /                  \
    //        1                    1
    TreeNode d_1(1);
    TreeNode d_2(2, &d_1, nullptr);
    TreeNode e_1(1);
    TreeNode e_2(2, nullptr, &e_1);
    assert(sameTree(&d_2, &e_2) == false);

    // Test 7: Left subtree only differs
    // Tree1:   2        Tree2:   2
    //         /                /
    //        1                3
    TreeNode f_1(1);
    TreeNode f_2(2, &f_1, nullptr);
    TreeNode g_1(3);
    TreeNode g_2(2, &g_1, nullptr);
    assert(sameTree(&f_2, &g_2) == false);

    // Test 8: Right subtree differs
    // Tree1:   2        Tree2:   2
    //         \                \
    //          1                1
    //                              \
    //                               9
    TreeNode h_1(1);
    TreeNode h_2(2, nullptr, &h_1);
    TreeNode i_9(9);
    TreeNode i_1(1, nullptr, &i_9);
    TreeNode i_2(2, nullptr, &i_1);
    assert(sameTree(&h_2, &i_2) == false);

    // Test 9: Deeper identical trees
    // Tree1:        5          Tree2:        5
    //              / \                    / \
    //             3   8                  3   8
    //            / \   \                / \   \
    //           1   4   9              1   4   9
    TreeNode j1_4(4), j1_1(1), j1_3(3, &j1_1, &j1_4), j1_9(9), j1_8(8, nullptr, &j1_9), j1_5(5, &j1_3, &j1_8);
    TreeNode j2_4(4), j2_1(1), j2_3(3, &j2_1, &j2_4), j2_9(9), j2_8(8, nullptr, &j2_9), j2_5(5, &j2_3, &j2_8);
    assert(sameTree(&j1_5, &j2_5) == true);

    // Test 10: Unbalanced vs balanced with same total nodes
    // Tree1 (chain): 1-2-3 (right only)
    TreeNode k3(3);
    TreeNode k2(2, nullptr, &k3);
    TreeNode k1(1, nullptr, &k2);
    // Tree2 (balanced): 2 with children 1 and 3
    TreeNode l1(1);
    TreeNode l3(3);
    TreeNode l2(2, &l1, &l3);
    assert(sameTree(&k1, &l2) == false);

    return 0;
}
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

// Compare two binary trees recursively for structural and value equality.
// Returns true if both trees are identical (same structure and same values at each node).
bool sameTree(const TreeNode* p, const TreeNode* q) {
    // Both null: identical
    if (p == nullptr && q == nullptr) {
        return true;
    }
    // One null, or values differ: not identical
    if (p == nullptr || q == nullptr || p->val != q->val) {
        return false;
    }
    // Recursively check left and right subtrees
    return sameTree(p->left, q->left) && sameTree(p->right, q->right);
}
// The solution uses a recursive depth-first traversal that compares corresponding nodes in both trees simultaneously. At each recursive call, it checks three conditions: (1) if both nodes are null, the subtrees are identical; (2) if only one is null or the values differ, the subtrees are not identical; (3) otherwise, both nodes exist and have equal values, so the result is the logical AND of recursively comparing the left children and the right children. The recursion naturally explores the entire structure, and if any mismatch is found, it propagates false upward. The base case handles null pointers gracefully. The time complexity is O(n), where n is the total number of nodes in the smaller tree if they differ, or the total number of nodes if they are identical, because each node is visited at most once. The space complexity is O(h) in the worst case for the recursion stack, where h is the height of the tree (O(n) for skewed trees, O(log n) for balanced trees). Edge cases include both trees being null (returns true), one null and one non-null (returns false), trees with same structure but different values (returns false), and trees with different structures but same values (returns false). Since the function uses `const TreeNode*`, it cannot modify the trees and is safe to call on read-only data.
