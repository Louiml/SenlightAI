Write a C++ function `void flatten(TreeNode* root)` that transforms a binary tree into a "linked list" in place, following the preorder traversal order. The flattened tree should use the existing `right` pointers to form a singly linked list, and every node's `left` pointer must be set to `nullptr`. The function should handle an empty tree (null root) gracefully, and must not allocate any new `TreeNode` objects or use any auxiliary container (like a vector or stack). The transformation must be done with a recursive auxiliary function that returns the last node of the flattened subtree, so that the right subtree can be reattached after the left subtree's flattened chain. The function should work correctly for skewed trees, balanced trees, and trees with only one child.

#include <cassert>

// Helper to count nodes in the flattened list and verify left pointers are null.
int checkFlattened(TreeNode* root) {
    int count = 0;
    TreeNode* cur = root;
    while (cur != nullptr) {
        assert(cur->left == nullptr);
        count++;
        cur = cur->right;
    }
    return count;
}

int main() {
    // Test 1: Empty tree
    flatten(nullptr);

    // Test 2: Single node
    TreeNode* t1 = new TreeNode(1);
    flatten(t1);
    assert(t1->left == nullptr && t1->right == nullptr);

    // Test 3: Left skewed tree: 1 -> left 2 -> left 3
    TreeNode* t3 = new TreeNode(1);
    t3->left = new TreeNode(2);
    t3->left->left = new TreeNode(3);
    flatten(t3);
    assert(t3->val == 1 && t3->left == nullptr && t3->right->val == 2);
    assert(t3->right->right->val == 3 && t3->right->right->right == nullptr);
    assert(checkFlattened(t3) == 3);

    // Test 4: Right skewed tree: 1 -> right 2 -> right 3
    TreeNode* t4 = new TreeNode(1);
    t4->right = new TreeNode(2);
    t4->right->right = new TreeNode(3);
    flatten(t4);
    assert(t4->val == 1 && t4->left == nullptr && t4->right->val == 2);
    assert(t4->right->right->val == 3 && t4->right->right->right == nullptr);
    assert(checkFlattened(t4) == 3);

    // Test 5: Balanced tree:      1
    //                          /   \
    //                         2     3
    //                        / \   / \
    //                       4   5 6   7
    TreeNode* t5 = new TreeNode(1);
    t5->left = new TreeNode(2);
    t5->right = new TreeNode(3);
    t5->left->left = new TreeNode(4);
    t5->left->right = new TreeNode(5);
    t5->right->left = new TreeNode(6);
    t5->right->right = new TreeNode(7);
    flatten(t5);
    int expected5[] = {1,2,4,5,3,6,7};
    TreeNode* cur = t5;
    for (int i = 0; i < 7; ++i) {
        assert(cur != nullptr && cur->val == expected5[i]);
        assert(cur->left == nullptr);
        cur = cur->right;
    }
    assert(cur == nullptr);

    // Test 6: Tree where only left subtree has right child: 1 with left 2, 2 has right 3
    TreeNode* t6 = new TreeNode(1);
    t6->left = new TreeNode(2);
    t6->left->right = new TreeNode(3);
    flatten(t6);
    assert(t6->val == 1 && t6->right->val == 2 && t6->right->right->val == 3);
    assert(checkFlattened(t6) == 3);

    // Test 7: Tree where right subtree has left child: 1 with right 2, 2 has left 3
    TreeNode* t7 = new TreeNode(1);
    t7->right = new TreeNode(2);
    t7->right->left = new TreeNode(3);
    flatten(t7);
    assert(t7->val == 1 && t7->right->val == 2 && t7->right->right->val == 3);
    assert(checkFlattened(t7) == 3);

    // Test 8: Deep left chain with right leaf at each node (demonstrating reattachment)
    // Tree: 1-left->2-left->3, 1-right->4, 2-right->5, 3-right->6
    TreeNode* t8 = new TreeNode(1);
    t8->left = new TreeNode(2);
    t8->left->left = new TreeNode(3);
    t8->right = new TreeNode(4);
    t8->left->right = new TreeNode(5);
    t8->left->left->right = new TreeNode(6);
    flatten(t8);
    int expected8[] = {1,2,3,6,5,4};
    cur = t8;
    for (int i = 0; i < 6; ++i) {
        assert(cur != nullptr && cur->val == expected8[i]);
        cur = cur->right;
    }
    assert(cur == nullptr);

    // Clean up not required for assert tests, but in real code one would delete nodes.

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

// Flattens the binary tree rooted at `root` into a linked list in preorder order.
// After the call, every left pointer becomes nullptr, and right pointers form the list.
// Returns the last node of the flattened subtree to help reattach the right subtree.
static TreeNode* flattenSubtree(TreeNode* node) {
    if (node == nullptr) return nullptr;
    if (node->left == nullptr && node->right == nullptr) return node;
    
    if (node->left != nullptr) {
        TreeNode* originalRight = node->right;
        // Flatten the left subtree and move it to the right.
        node->right = node->left;
        node->left = nullptr;
        TreeNode* lastLeft = flattenSubtree(node->right);
        
        // Attach the original right subtree to the end of the flattened left chain.
        if (originalRight != nullptr) {
            lastLeft->right = originalRight;
            TreeNode* lastRight = flattenSubtree(originalRight);
            return (lastRight != nullptr) ? lastRight : lastLeft;
        } else {
            return lastLeft;
        }
    } else {
        // Only right child exists; just recurse.
        return flattenSubtree(node->right);
    }
}

// Public function: flattens the tree in place.
void flatten(TreeNode* root) {
    flattenSubtree(root);
}

// The core challenge is to flatten the tree in place without extra memory. A recursive approach works by processing the left subtree first, then the right. The auxiliary function `flattenSubtree(TreeNode* node)` returns the last node of the flattened linked list rooted at `node`. The steps for a non-null node are: (1) If `node` has no children, return it. (2) If `node->left` exists, recursively flatten the left subtree and get its last node, then temporarily save the original right subtree. (3) Move the flattened left subtree to `node->right` and set `node->left` to `nullptr`. (4) Attach the original right subtree to the end of the flattened left chain by calling `flattenSubtree` on the saved right subtree and connecting it to the last node of the left chain. (5) Return the last node of the combined flattened chain. Edge cases include: empty tree, only right child (just recurse on right), only left child (no right to reattach), and full binary trees. Time complexity is O(n) because each node is visited a constant number of times; space complexity is O(h) for the recursion stack where h is tree height (could be O(n) for skewed trees).
