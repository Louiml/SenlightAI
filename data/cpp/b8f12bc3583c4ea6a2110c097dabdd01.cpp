Write a C++ function `std::vector<std::vector<int>> bottomUpLevelOrder(TreeNode* root)` that takes the root of a binary tree (defined by the standard `TreeNode` struct) and returns the node values in a bottom-up level order traversal. That is, the last level of the tree (deepest leaves) should appear first in the returned vector, and the root level should appear last. Nodes at the same level must appear in left-to-right order. The function should handle an empty tree (return an empty vector), a tree with only a root, skewed trees, and complete trees. The solution must not use a queue; instead, it must use a recursive depth‑first approach that fills a level‑indexed structure and then reverses it at the end.
// The main idea is to perform a recursive preorder traversal while tracking the current depth (starting at 0 for the root). A helper function is used to fill a vector of vectors `levels`, where `levels[d]` accumulates the values of nodes at depth `d`. Before pushing a value, ensure that `levels` has at least `d+1` entries by appending an empty vector when necessary. After the recursion completes, reverse `levels` so that deepest levels come first. Edge cases include an empty tree (return `{}`), a single node (return `{{root->val}}`), and skewed trees (each level has exactly one node). Time complexity is \(O(n)\) because each node is visited exactly once, and reversing the vector costs \(O(h)\) where \(h\) is the height, but \(h \le n\), so overall \(O(n)\). Auxiliary space is \(O(h)\) for the recursion stack plus \(O(n)\) for the output structure, so \(O(n)\) in the worst case (skewed tree). The solution uses `const` correctly by only reading `TreeNode` pointers.
#include <vector>
#include <algorithm>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper to recursively fill levels with node values.
void collectLevels(std::vector<std::vector<int>>& levels, const TreeNode* node, int depth) {
    if (!node) return;
    // Ensure 'levels' has an entry for this depth.
    if (static_cast<int>(levels.size()) <= depth) {
        levels.push_back(std::vector<int>());
    }
    levels[depth].push_back(node->val);
    collectLevels(levels, node->left, depth + 1);
    collectLevels(levels, node->right, depth + 1);
}

// Return node values level-by-level from bottom to top, left to right per level.
std::vector<std::vector<int>> bottomUpLevelOrder(const TreeNode* root) {
    std::vector<std::vector<int>> levels;
    collectLevels(levels, root, 0);
    std::reverse(levels.begin(), levels.end());
    return levels;
}
#include <cassert>
#include <vector>
#include <initializer_list>

// TreeNode definition repeated for test completeness (already included above in solution context)
// In a real standalone test, include the struct from the solution file.

int main() {
    // Test 1: Empty tree
    assert(bottomUpLevelOrder(nullptr).empty());

    // Test 2: Single node
    TreeNode n1(5);
    std::vector<std::vector<int>> res2 = bottomUpLevelOrder(&n1);
    assert(res2.size() == 1 && res2[0] == std::vector<int>{5});

    // Test 3: Balanced tree of 3 nodes: 1 -> left 2, right 3
    TreeNode root3(1);
    TreeNode l3(2);
    TreeNode r3(3);
    root3.left = &l3; root3.right = &r3;
    std::vector<std::vector<int>> res3 = bottomUpLevelOrder(&root3);
    assert(res3 == std::vector<std::vector<int>>{{2,3},{1}});

    // Test 4: Left-skewed tree: 1 -> 2 -> 3
    TreeNode a4(1), b4(2), c4(3);
    a4.left = &b4; b4.left = &c4;
    std::vector<std::vector<int>> res4 = bottomUpLevelOrder(&a4);
    assert(res4 == std::vector<std::vector<int>>{{3},{2},{1}});

    // Test 5: Complete tree of 7 nodes
    TreeNode n5[7];
    for (int i = 0; i < 7; ++i) n5[i].val = i+1; // values 1..7
    n5[0].left = &n5[1]; n5[0].right = &n5[2];
    n5[1].left = &n5[3]; n5[1].right = &n5[4];
    n5[2].left = &n5[5]; n5[2].right = &n5[6];
    std::vector<std::vector<int>> res5 = bottomUpLevelOrder(&n5[0]);
    assert(res5 == std::vector<std::vector<int>>{{4,5,6,7},{2,3},{1}});

    // Test 6: Right-skewed tree: 1 -> right 2 -> right 3
    TreeNode a6(1), b6(2), c6(3);
    a6.right = &b6; b6.right = &c6;
    std::vector<std::vector<int>> res6 = bottomUpLevelOrder(&a6);
    assert(res6 == std::vector<std::vector<int>>{{3},{2},{1}});

    // Test 7: Tree with only one internal node and two leaves (as in test 3) but with different values
    TreeNode x7(10), y7(20), z7(30);
    x7.left = &y7; x7.right = &z7;
    std::vector<std::vector<int>> res7 = bottomUpLevelOrder(&x7);
    assert(res7 == std::vector<std::vector<int>>{{20,30},{10}});

    return 0;
}
