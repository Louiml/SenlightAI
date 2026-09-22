/*
Given a binary tree whose nodes are defined by `TreeNode` with integer `val`, and `left`/`right` pointers, write a C++ free function `void flattenTree(TreeNode* root)` that transforms the tree in‑place into a right‑skewed "linked list" following a **pre‑order traversal** order. After flattening, every node's `left` pointer must be `nullptr`, and the `right` pointer must point to the next node in pre‑order sequence. The function must work for empty trees, single‑node trees, trees with only left children, only right children, and arbitrary balanced/unbalanced trees. Do **not** allocate any new nodes and do **not** use auxiliary containers (e.g., vectors, stacks) — only modify existing pointers in‑place. The signature should be `void flattenTree(TreeNode* root)` and the function must be self‑contained (no reliance on any external `Utils.h` or global state).
*/

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Flatten the binary tree into a right-skewed linked list in pre-order.
void flattenTree(TreeNode* root) {
    if (root == nullptr) {
        return;
    }

    // Recursively flatten left and right subtrees first.
    flattenTree(root->left);
    flattenTree(root->right);

    // If there is no left child, nothing else to do for this node.
    if (root->left == nullptr) {
        return;
    }

    // Save the flattened right subtree.
    TreeNode* rightSubtree = root->right;

    // Move the flattened left subtree to the right.
    root->right = root->left;
    root->left = nullptr;

    // Find the rightmost node of the new right chain (was left subtree).
    TreeNode* rightmost = root->right;
    while (rightmost->right != nullptr) {
        rightmost = rightmost->right;
    }

    // Attach the original right subtree to the end.
    rightmost->right = rightSubtree;
}

#include <cassert>

// Helper to create a tree node.
TreeNode* node(int val, TreeNode* left = nullptr, TreeNode* right = nullptr) {
    return new TreeNode(val, left, right);
}

// Helper to collect pre-order values (for verification).
void collectPreorder(TreeNode* root, std::vector<int>& out) {
    if (!root) return;
    out.push_back(root->val);
    collectPreorder(root->left, out);
    collectPreorder(root->right, out);
}

int main() {
    // Test 1: Empty tree.
    flattenTree(nullptr);

    // Test 2: Single node.
    TreeNode* single = node(1);
    flattenTree(single);
    assert(single->left == nullptr);
    assert(single->right == nullptr);

    // Test 3: Tree from the prompt.
    TreeNode* root = node(1,
                          node(2, node(3), node(4)),
                          node(5, nullptr, node(6)));
    flattenTree(root);
    std::vector<int> values;
    collectPreorder(root, values);
    assert((values == std::vector<int>{1, 2, 3, 4, 5, 6}));
    // Verify each node has no left child.
    for (TreeNode* cur = root; cur != nullptr; cur = cur->right) {
        assert(cur->left == nullptr);
    }

    // Test 4: Only left children (skewed left).
    TreeNode* leftSkewed = node(1,
                                node(2,
                                     node(3, node(4), nullptr),
                                     nullptr),
                                nullptr);
    flattenTree(leftSkewed);
    values.clear();
    collectPreorder(leftSkewed, values);
    assert((values == std::vector<int>{1, 2, 3, 4}));

    // Test 5: Only right children (already flattened).
    TreeNode* rightSkewed = node(1, nullptr, node(2, nullptr, node(3)));
    flattenTree(rightSkewed);
    values.clear();
    collectPreorder(rightSkewed, values);
    assert((values == std::vector<int>{1, 2, 3}));

    // Test 6: Root with no left and a large right subtree.
    TreeNode* noLeft = node(1, nullptr, node(2, nullptr, node(3)));
    flattenTree(noLeft);
    values.clear();
    collectPreorder(noLeft, values);
    assert((values == std::vector<int>{1, 2, 3}));

    // Test 7: Root with only left and that left has only right child.
    TreeNode* mixed = node(1, node(2, nullptr, node(3)), nullptr);
    flattenTree(mixed);
    values.clear();
    collectPreorder(mixed, values);
    assert((values == std::vector<int>{1, 2, 3}));

    return 0;
}

// The core idea is to process the tree recursively in a manner that preserves pre‑order order. For a given root, we need to attach its entire left subtree (flattened) immediately after the root, and then attach the original right subtree after the flattened left subtree. A straightforward recursive approach: first recursively flatten the left subtree and the right subtree. Then, take the flattened left subtree and insert it between root and root->right. Specifically: save `rightSubtree = root->right` (which is already flattened). Set `root->right = root->left`, and set `root->left = nullptr`. Then traverse to the rightmost node of the new right chain (which originally was the flattened left subtree) and attach `rightSubtree` to its `right`. This yields the correct order: root, then all nodes from the left subtree in pre‑order, then all nodes from the right subtree in pre‑order. However, the snippet in the prompt recursively calls only `flatten(root->right)` after reconnecting, which is insufficient because the left subtree is not flattened before being moved. The correct algorithm must flatten both subtrees first. Edge cases: empty tree (do nothing), node with no left child (the step of moving left to right is trivial, and the rightmost traversal simply finds the original `right`). Time complexity: each node is visited once during the recursive flattening, and the rightmost traversal visits each edge at most a constant number of times, giving O(n) time where n is the number of nodes. Space complexity: O(h) due to recursion stack, h = height of tree (worst‑case O(n) for skewed trees, but that’s expected for in‑place pointer manipulation without extra containers).
