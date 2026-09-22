// Write a C++ function `bool isPerfectBinaryTree(TreeNode* root)` that determines whether a given binary tree is perfect. A perfect binary tree is one where all internal nodes have exactly two children and all leaf nodes are at the same depth (level). The function should handle empty trees (returning `true`), single-node trees (returning `true`), and correctly identify trees where nodes have only one child or leaves appear at different levels. You may use the provided `TreeNode` structure with fields `iData`, `ptrLeft`, and `ptrRight`, but you must not rely on any helper functions from the original snippet—your function must be self-contained (you may define additional private/local helper lambdas or nested static functions within your solution, but the main entry point is the single free function `isPerfectBinaryTree`).

The solution requires checking two properties simultaneously: (1) every internal node must have either zero or two children (no node with exactly one child), and (2) all leaves must be at the same depth. The standard approach is recursive: compute the height of the tree first (depth from root to deepest leaf), then traverse the tree again, verifying that every leaf is at exactly that height and every internal node has two children. Alternatively, a single recursive function can return both the subtree height and a boolean indicating whether that subtree is perfect. The recursive logic: if the node is `nullptr`, it's trivially perfect with height 0. For a non-null node, recurse on left and right children. If either child is `nullptr` while the other is not, or if the child subtrees have different heights, the tree is not perfect. Otherwise, the subtree is perfect if both children are perfect, and its height is `1 + max(leftHeight, rightHeight)` (which will equal `1 + leftHeight` since they match). Edge cases: empty tree returns `true` (height 0); single node returns `true` (height 1, both children `nullptr`). Time complexity is O(n) where n is the number of nodes, since each node is visited once. Space complexity is O(h) for the recursion stack, where h is the height (O(log n) for a perfect tree, O(n) in the worst-case skewed tree).

#include <algorithm>

struct TreeNode {
    int iData;
    TreeNode* ptrLeft;
    TreeNode* ptrRight;
};

// Helper structure to return both height and perfection status
struct TreeInfo {
    int height;
    bool isPerfect;
};

// Recursive helper function
TreeInfo checkPerfect(TreeNode* node) {
    if (node == nullptr) {
        return {0, true};
    }

    TreeInfo leftInfo = checkPerfect(node->ptrLeft);
    TreeInfo rightInfo = checkPerfect(node->ptrRight);

    // If either subtree is not perfect, or one child exists without the other, or heights differ
    if (!leftInfo.isPerfect || !rightInfo.isPerfect ||
        (node->ptrLeft == nullptr) != (node->ptrRight == nullptr) ||
        leftInfo.height != rightInfo.height) {
        return {std::max(leftInfo.height, rightInfo.height) + 1, false};
    }

    // Both subtrees perfect and equal height
    return {leftInfo.height + 1, true};
}

// Main function: check if the binary tree is perfect
bool isPerfectBinaryTree(TreeNode* root) {
    return checkPerfect(root).isPerfect;
}

#include <cassert>

// Helper to create a node for testing
TreeNode* createNode(int val) {
    TreeNode* node = new TreeNode;
    node->iData = val;
    node->ptrLeft = nullptr;
    node->ptrRight = nullptr;
    return node;
}

// Helper to clean up memory
void deleteTree(TreeNode* node) {
    if (node == nullptr) return;
    deleteTree(node->ptrLeft);
    deleteTree(node->ptrRight);
    delete node;
}

int main() {
    // Test 1: Empty tree is perfect
    assert(isPerfectBinaryTree(nullptr) == true);

    // Test 2: Single node is perfect
    TreeNode* single = createNode(1);
    assert(isPerfectBinaryTree(single) == true);
    deleteTree(single);

    // Test 3: Perfect tree of height 2 (3 nodes)
    TreeNode* root1 = createNode(1);
    root1->ptrLeft = createNode(2);
    root1->ptrRight = createNode(3);
    assert(isPerfectBinaryTree(root1) == true);
    deleteTree(root1);

    // Test 4: Perfect tree of height 3 (7 nodes)
    TreeNode* root2 = createNode(1);
    root2->ptrLeft = createNode(2);
    root2->ptrRight = createNode(3);
    root2->ptrLeft->ptrLeft = createNode(4);
    root2->ptrLeft->ptrRight = createNode(5);
    root2->ptrRight->ptrLeft = createNode(6);
    root2->ptrRight->ptrRight = createNode(7);
    assert(isPerfectBinaryTree(root2) == true);
    deleteTree(root2);

    // Test 5: Not perfect - node with one child
    TreeNode* root3 = createNode(1);
    root3->ptrLeft = createNode(2);
    assert(isPerfectBinaryTree(root3) == false);
    deleteTree(root3);

    // Test 6: Not perfect - leaves at different depths
    TreeNode* root4 = createNode(1);
    root4->ptrLeft = createNode(2);
    root4->ptrRight = createNode(3);
    root4->ptrLeft->ptrLeft = createNode(4);
    assert(isPerfectBinaryTree(root4) == false);
    deleteTree(root4);

    // Test 7: Not perfect - internal node with one child and deep structure
    TreeNode* root5 = createNode(1);
    root5->ptrLeft = createNode(2);
    root5->ptrRight = createNode(3);
    root5->ptrLeft->ptrLeft = createNode(4);
    root5->ptrLeft->ptrRight = createNode(5);
    root5->ptrRight->ptrLeft = createNode(6);
    assert(isPerfectBinaryTree(root5) == false);
    deleteTree(root5);

    return 0;
}
