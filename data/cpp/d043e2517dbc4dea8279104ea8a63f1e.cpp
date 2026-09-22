// Given a pointer to the root of a complete binary tree (where every level except possibly the last is completely filled, and all nodes in the last level are as far left as possible), write a C++ function that returns the total number of nodes in the tree. The function should take a `const TreeNode*` (where `TreeNode` is a struct with `int val`, `TreeNode *left`, `TreeNode *right`) and return an `int`. The input tree is guaranteed to be complete but may be empty (nullptr root). Your solution must not use any additional data structures for traversal (no queues, stacks, or vectors), and must avoid visiting every node when the tree is a perfect full binary tree (i.e., both left and right subtrees have equal depth). Instead, exploit the completeness property to compute node counts in O(log² n) time on average.
#include <cassert>

int main() {
    // Test 1: Empty tree
    assert(countCompleteTreeNodes(nullptr) == 0);

    // Test 2: Single node
    TreeNode* n1 = new TreeNode(1);
    assert(countCompleteTreeNodes(n1) == 1);

    // Test 3: Complete tree with 3 nodes (root + two children)
    TreeNode* n2 = new TreeNode(2);
    TreeNode* n3 = new TreeNode(3);
    TreeNode* root3 = new TreeNode(1, n2, n3);
    assert(countCompleteTreeNodes(root3) == 3);

    // Test 4: Complete tree with 5 nodes (root, left child with two children, right child with one left child)
    TreeNode* n4 = new TreeNode(4);
    TreeNode* n5 = new TreeNode(5);
    TreeNode* n6 = new TreeNode(6);
    TreeNode* n7 = new TreeNode(7);
    TreeNode* root5 = new TreeNode(1, new TreeNode(2, n4, n5), new TreeNode(3, n6, nullptr));
    assert(countCompleteTreeNodes(root5) == 6);

    // Test 5: Perfect full tree with 7 nodes (depth 3)
    TreeNode* root7 = new TreeNode(1,
                        new TreeNode(2, new TreeNode(4), new TreeNode(5)),
                        new TreeNode(3, new TreeNode(6), new TreeNode(7)));
    assert(countCompleteTreeNodes(root7) == 7);

    // Test 6: Complete tree with 10 nodes (last level partially filled)
    TreeNode* root10 = new TreeNode(1);
    root10->left = new TreeNode(2);
    root10->right = new TreeNode(3);
    root10->left->left = new TreeNode(4);
    root10->left->right = new TreeNode(5);
    root10->right->left = new TreeNode(6);
    root10->right->right = new TreeNode(7);
    root10->left->left->left = new TreeNode(8);
    root10->left->left->right = new TreeNode(9);
    root10->left->right->left = new TreeNode(10);
    assert(countCompleteTreeNodes(root10) == 10);

    // Test 7: Larger complete tree with 15 nodes (perfect full of depth 4)
    TreeNode* root15 = new TreeNode(1);
    // Build manually a full tree with 15 nodes (depth 4)
    // (Using a simple loop or manual construction for clarity)
    // For brevity, we build only a few levels and count via recursion is reliable.
    // Instead, construct a complete tree with 12 nodes to check non-power counts.
    // Let's build a complete tree with 12 nodes.
    TreeNode* root12 = new TreeNode(1);
    root12->left = new TreeNode(2);
    root12->right = new TreeNode(3);
    root12->left->left = new TreeNode(4);
    root12->left->right = new TreeNode(5);
    root12->right->left = new TreeNode(6);
    root12->right->right = new TreeNode(7);
    root12->left->left->left = new TreeNode(8);
    root12->left->left->right = new TreeNode(9);
    root12->left->right->left = new TreeNode(10);
    root12->left->right->right = new TreeNode(11);
    root12->right->left->left = new TreeNode(12);
    assert(countCompleteTreeNodes(root12) == 12);

    // Clean up (optional in tests, but good practice)
    // (Not required for assert tests, but memory management omitted for brevity)

    return 0;
}
#include <cstddef>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Returns the total number of nodes in a complete binary tree.
// Uses the property that if left and right depths are equal, the subtree is a perfect full tree.
int countCompleteTreeNodes(const TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }

    // Compute depth of leftmost path (root depth = 1)
    int leftDepth = 1;
    const TreeNode* node = root;
    while (node->left != nullptr) {
        ++leftDepth;
        node = node->left;
    }

    // Compute depth of rightmost path
    int rightDepth = 1;
    node = root;
    while (node->right != nullptr) {
        ++rightDepth;
        node = node->right;
    }

    if (leftDepth == rightDepth) {
        // Perfect full binary tree: node count = 2^depth - 1
        // Using left shift: 2^leftDepth - 1
        return (1 << leftDepth) - 1;
    }

    // Not full, recurse on both children and add current node
    return 1 + countCompleteTreeNodes(root->left) + countCompleteTreeNodes(root->right);
}
// The key insight is that a complete binary tree is either a perfect full binary tree (all levels fully occupied) or a tree where the last level is partially filled from the left. To count nodes efficiently, we compute the depth of the leftmost path and the rightmost path from a given root. If these depths are equal, the subtree rooted here is a perfect full binary tree, and its node count is `2^depth - 1`. If they differ, then the subtree is not full, so we recursively count nodes in the left and right children, adding 1 for the current root. Because the tree is complete, at least one of the children will be a perfect full tree at each recursion step, so the recursion depth is O(log n) and each level does O(log n) work finding depths, giving O(log² n) time. The base case is a null root returning 0. Edge cases include an empty tree (return 0), a single node (left and right depths both 1, return 1), and deep trees where one side is full. The algorithm avoids traversing all nodes, making it far more efficient than a simple level-order traversal.
