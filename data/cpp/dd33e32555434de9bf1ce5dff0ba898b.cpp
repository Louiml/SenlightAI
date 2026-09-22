/*
Write a C++ function that takes a binary tree node and returns the maximum depth (height) of the tree. The tree is represented using the same node structure as shown in the code snippet: each node has an `int` data field and two pointers, `left` and `right`. The function should handle an empty tree (null pointer) by returning 0, and a tree with only one node by returning 1. The maximum depth is defined as the number of nodes along the longest path from the root down to the farthest leaf node. The tree is not guaranteed to be balanced, and you should not assume any particular shape. Your function must be recursive and work correctly for arbitrary binary trees, including those with only left children, only right children, or both.
*/
#include <algorithm> // for std::max

// Definition of a binary tree node (consistent with the provided code)
struct treenode {
    int data;
    treenode* left;
    treenode* right;
};

// Return the maximum depth (height) of a binary tree.
// Height is defined as the number of nodes along the longest root-to-leaf path.
// An empty tree (null pointer) has height 0.
int maxDepth(const treenode* node) {
    if (node == nullptr) {
        return 0;
    }
    int leftDepth = maxDepth(node->left);
    int rightDepth = maxDepth(node->right);
    return 1 + std::max(leftDepth, rightDepth);
}
#include <cassert>

int main() {
    // Test 1: Empty tree
    assert(maxDepth(nullptr) == 0);

    // Test 2: Single node tree
    treenode n1{10, nullptr, nullptr};
    assert(maxDepth(&n1) == 1);

    // Test 3: Balanced tree of height 3
    treenode n2{20, nullptr, nullptr};
    treenode n3{30, nullptr, nullptr};
    treenode n4{40, nullptr, nullptr};
    treenode n5{50, nullptr, nullptr};
    treenode root{1, &n2, &n3};
    n2.left = &n4;
    n2.right = &n5;  // tree: root(1) -> left(20) with two children, right(30) leaf => height 3
    assert(maxDepth(&root) == 3);

    // Test 4: Left-skewed tree (height equals node count)
    treenode a{1, nullptr, nullptr};
    treenode b{2, &a, nullptr};
    treenode c{3, &b, nullptr};
    assert(maxDepth(&c) == 3);

    // Test 5: Right-skewed tree
    treenode x{1, nullptr, nullptr};
    treenode y{2, nullptr, &x};
    treenode z{3, nullptr, &y};
    assert(maxDepth(&z) == 3);

    // Test 6: Unbalanced tree with longer right branch
    treenode p{1, nullptr, nullptr};
    treenode q{2, nullptr, nullptr};
    treenode r{3, &p, &q};
    treenode s{4, nullptr, &r};
    treenode t{5, &s, nullptr};
    // Path: 5->4->3->2 (or 1) gives depth 4
    assert(maxDepth(&t) == 4);

    // Test 7: Tree with only left children but deep
    treenode m1{1, nullptr, nullptr};
    treenode m2{2, &m1, nullptr};
    treenode m3{3, &m2, nullptr};
    treenode m4{4, &m3, nullptr};
    assert(maxDepth(&m4) == 4);

    return 0;
}
// The solution uses a recursive depth-first traversal. The base case occurs when the current node pointer is null, in which case the depth is 0 (no nodes in this subtree). For a non-null node, the depth is 1 (for the current node) plus the maximum of the depths of its left and right subtrees, computed recursively. This correctly handles all cases: an empty tree returns 0; a single-node tree returns 1 (since both children are null, the max of 0 and 0 is 0, then add 1); a skewed tree returns the number of nodes along the chain. The algorithm visits every node exactly once, so time complexity is O(n), where n is the number of nodes. Space complexity is O(h) due to the recursive call stack, where h is the tree's height (worst case O(n) for a skewed tree). No special edge cases like cycles exist in a valid binary tree, but the function must guard against null pointers at the root and intermediate levels.
