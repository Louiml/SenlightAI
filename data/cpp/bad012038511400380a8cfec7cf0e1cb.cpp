// Write a C++ function that takes the root of a binary search tree (BST) and two integers `low` and `high` (with `low <= high`), and returns the root of a new BST containing only the nodes whose values are within the inclusive range `[low, high]`. The trimming must preserve the original BST structure (i.e., the relative ordering of nodes and their left/right relationships must remain valid according to BST properties). The tree may be empty, and all node values are distinct. The function should not leak memory and should build a new tree rather than modifying the original tree in place.

// Since the tree is a BST, we can solve this recursively. For a given node, if its value is less than `low`, then the entire left subtree (which contains even smaller values) is invalid, so we can discard it and recursively trim the right subtree. Similarly, if the node’s value is greater than `high`, discard the right subtree and recursively trim the left subtree. If the node’s value is within the range, we keep the node but must recursively trim both its left and right children to ensure all descendants are also within the range. The base case is a null node, which returns null. We build a new tree by allocating new nodes only for those that fall within the range; for each kept node, we create a copy and attach trimmed subtrees. Edge cases include an empty tree (return nullptr), all nodes outside the range (return nullptr), or the root itself being outside the range, in which case we may need to return a descendant as the new root. Time complexity is O(n) because each node is visited once, and space complexity is O(h) for the recursion stack, where h is the height of the tree (O(n) in the worst case for a skewed tree, O(log n) for a balanced tree).

#include <memory>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Recursively trim the BST to keep only nodes with values in [low, high].
// Returns a newly allocated tree; the original tree is not modified.
TreeNode* trimBST(const TreeNode* root, int low, int high) {
    if (root == nullptr) {
        return nullptr;
    }
    if (root->val < low) {
        // Node and its left subtree are too small; trim the right subtree.
        return trimBST(root->right, low, high);
    }
    if (root->val > high) {
        // Node and its right subtree are too large; trim the left subtree.
        return trimBST(root->left, low, high);
    }
    // Node is within range; keep it and trim both subtrees.
    TreeNode* newRoot = new TreeNode(root->val);
    newRoot->left = trimBST(root->left, low, high);
    newRoot->right = trimBST(root->right, low, high);
    return newRoot;
}

#include <cassert>

// Helper to recursively delete a tree.
void deleteTree(TreeNode* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

// Helper to check if a tree is a valid BST and all values are within [low, high].
bool isValidTrimmed(const TreeNode* node, int low, int high, int* prev) {
    if (node == nullptr) return true;
    if (node->val < low || node->val > high) return false;
    if (prev != nullptr && node->val <= *prev) return false;
    int* newPrev = const_cast<int*>(&node->val); // hack for simplicity in test
    return isValidTrimmed(node->left, low, high, &node->val) &&
           isValidTrimmed(node->right, low, high, &node->val);
}

// Helper to collect inorder values for comparison.
void inorderCollect(const TreeNode* node, std::vector<int>& out) {
    if (node == nullptr) return;
    inorderCollect(node->left, out);
    out.push_back(node->val);
    inorderCollect(node->right, out);
}

int main() {
    // Example 1: root = [1,0,2], low=1, high=2 -> trimmed tree [1,null,2]
    TreeNode* t1 = new TreeNode(1, new TreeNode(0), new TreeNode(2));
    TreeNode* r1 = trimBST(t1, 1, 2);
    std::vector<int> v1;
    inorderCollect(r1, v1);
    assert((v1 == std::vector<int>{1, 2}));
    deleteTree(r1);
    deleteTree(t1);

    // Example 2: root = [3,0,4,null,2,null,null,1], low=1, high=3 -> [3,2,null,1]
    TreeNode* t2 = new TreeNode(3,
                  new TreeNode(0, nullptr, new TreeNode(2, new TreeNode(1), nullptr)),
                  new TreeNode(4));
    TreeNode* r2 = trimBST(t2, 1, 3);
    std::vector<int> v2;
    inorderCollect(r2, v2);
    assert((v2 == std::vector<int>{1, 2, 3}));
    deleteTree(r2);
    deleteTree(t2);

    // Empty tree
    TreeNode* r3 = trimBST(nullptr, 0, 10);
    assert(r3 == nullptr);

    // All nodes outside range -> empty result
    TreeNode* t4 = new TreeNode(5, new TreeNode(3), new TreeNode(7));
    TreeNode* r4 = trimBST(t4, 10, 20);
    assert(r4 == nullptr);
    deleteTree(t4);

    // Single node outside range
    TreeNode* t5 = new TreeNode(8);
    TreeNode* r5 = trimBST(t5, 1, 7);
    assert(r5 == nullptr);
    deleteTree(t5);

    // Single node inside range
    TreeNode* t6 = new TreeNode(4);
    TreeNode* r6 = trimBST(t6, 1, 7);
    assert(r6 != nullptr && r6->val == 4 && r6->left == nullptr && r6->right == nullptr);
    deleteTree(r6);
    deleteTree(t6);

    // Skewed tree, trim left part
    TreeNode* t7 = new TreeNode(1, nullptr, new TreeNode(2, nullptr, new TreeNode(3)));
    TreeNode* r7 = trimBST(t7, 2, 3);
    std::vector<int> v7;
    inorderCollect(r7, v7);
    assert((v7 == std::vector<int>{2, 3}));
    deleteTree(r7);
    deleteTree(t7);

    return 0;
}
