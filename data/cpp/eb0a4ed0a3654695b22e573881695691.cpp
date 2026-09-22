Write a C++ function `int kthSmallestInBST(TreeNode* root, int k)` that accepts the root of a binary search tree (BST) and an integer `k` (1-indexed), and returns the value of the k-th smallest element in the tree. The tree may contain duplicate values, but each node's value is still considered individually when ordering (i.e., if duplicates exist, they occupy consecutive positions). You may assume that the BST is valid and that `k` is between 1 and the total number of nodes. The node structure is already defined as `TreeNode` with `val`, `left`, and `right` pointers. Return the integer value directly; if the tree is empty or `k` is out of range, return 0.
#include <cassert>

// Helper function to create a new node (for test construction).
TreeNode* createNode(int val, TreeNode* left = nullptr, TreeNode* right = nullptr) {
    return new TreeNode(val, left, right);
}

int main() {
    // Test 1: Basic BST with distinct values.
    //     3
    //    / \
    //   1   4
    //    \
    //     2
    TreeNode* root1 = createNode(3,
                                 createNode(1, nullptr, createNode(2)),
                                 createNode(4));
    assert(kthSmallestInBST(root1, 1) == 1);
    assert(kthSmallestInBST(root1, 2) == 2);
    assert(kthSmallestInBST(root1, 3) == 3);
    assert(kthSmallestInBST(root1, 4) == 4);

    // Test 2: Single node tree.
    TreeNode* root2 = createNode(42);
    assert(kthSmallestInBST(root2, 1) == 42);
    assert(kthSmallestInBST(nullptr, 1) == 0);
    assert(kthSmallestInBST(root2, 2) == 0);

    // Test 3: BST with duplicate values (duplicates occupy consecutive positions).
    //     2
    //    / \
    //   2   3
    TreeNode* root3 = createNode(2,
                                 createNode(2),
                                 createNode(3));
    assert(kthSmallestInBST(root3, 1) == 2);
    assert(kthSmallestInBST(root3, 2) == 2);
    assert(kthSmallestInBST(root3, 3) == 3);

    // Test 4: Left-skewed tree (worst-case recursion depth).
    // 5
    //  \
    //   4
    //    \
    //     3
    TreeNode* root4 = createNode(5, nullptr, createNode(4, nullptr, createNode(3)));
    assert(kthSmallestInBST(root4, 1) == 3);
    assert(kthSmallestInBST(root4, 2) == 4);
    assert(kthSmallestInBST(root4, 3) == 5);

    // Test 5: Right-skewed tree.
    // 1
    //  \
    //   2
    //    \
    //     3
    TreeNode* root5 = createNode(1, nullptr, createNode(2, nullptr, createNode(3)));
    assert(kthSmallestInBST(root5, 1) == 1);
    assert(kthSmallestInBST(root5, 2) == 2);
    assert(kthSmallestInBST(root5, 3) == 3);

    // Test 6: Larger balanced tree.
    //       8
    //      / \
    //     3   10
    //    / \    \
    //   1   6    14
    //      / \   /
    //     4   7 13
    TreeNode* root6 = createNode(8,
                                 createNode(3,
                                            createNode(1),
                                            createNode(6,
                                                       createNode(4),
                                                       createNode(7))),
                                 createNode(10,
                                            nullptr,
                                            createNode(14,
                                                       createNode(13))));
    assert(kthSmallestInBST(root6, 1) == 1);
    assert(kthSmallestInBST(root6, 3) == 4);
    assert(kthSmallestInBST(root6, 5) == 7);
    assert(kthSmallestInBST(root6, 7) == 10);
    assert(kthSmallestInBST(root6, 9) == 14);

    // Clean up memory (optional for test correctness but good practice).
    // (Omitted for brevity; in a real test you'd write a deleteTree helper.)

    return 0;
}
#include <cstddef>

// TreeNode definition (assumed to be provided externally, but included here for completeness)
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper function: performs in-order traversal, decrementing k until it reaches 0.
// Returns the node where k becomes 0, or nullptr if not found.
const TreeNode* kthSmallestUtil(const TreeNode* root, int& k) {
    if (root == nullptr) {
        return nullptr;
    }

    // Search left subtree.
    const TreeNode* leftResult = kthSmallestUtil(root->left, k);
    if (leftResult != nullptr) {
        return leftResult;
    }

    // Visit current node.
    --k;
    if (k == 0) {
        return root;
    }

    // Search right subtree.
    return kthSmallestUtil(root->right, k);
}

// Returns the k-th smallest value in the BST, or 0 if tree is empty or k out of range.
int kthSmallestInBST(const TreeNode* root, int k) {
    int remaining = k;
    const TreeNode* result = kthSmallestUtil(root, remaining);
    return (result != nullptr) ? result->val : 0;
}
// The standard approach leverages the BST property that an in-order traversal visits nodes in ascending order. We perform a recursive depth-first in-order traversal, maintaining a reference to `k` that is decremented each time we visit a node. When `k` reaches 0, the current node is the k-th smallest, and we return its value. The recursion first explores the left subtree, then processes the current node, and finally the right subtree. This naturally visits nodes in sorted order. Edge cases include an empty tree (return 0) and a tree with a single node (the node itself is the answer). Duplicate values are handled automatically because each node is visited individually; if the same value appears multiple times, each occurrence occupies a distinct position in traversal order. We must be careful to pass `k` by reference so that decrements persist across recursive calls and across the left/right subtree traversals. If the function returns a null pointer (e.g., when `k` exceeds the total node count), we return 0. Time complexity is O(H + k) in the worst case, where H is height, because we may traverse down to the leftmost node and then visit up to `k` nodes; in the worst case (a skewed tree) this is O(n), and for a balanced tree it is O(log n + k). Space complexity is O(H) for the recursion stack, which is O(n) in the worst case.
