// Given the root of a binary search tree (BST) and an integer key, write a C++ function `deleteNodeFromBST(TreeNode* root, int key)` that deletes the node with the given key from the BST and returns the root of the modified tree. The tree must remain a valid BST after deletion. If the key is not present, return the original tree unchanged. Node values are unique. Handle the three cases: node with no children (leaf), node with one child, and node with two children. For the two‑child case, replace the deleted node’s value with its in‑order successor (the smallest value in its right subtree) and then recursively delete that successor from the right subtree. The function should be iterative or recursive, but must not allocate new nodes; it should modify the existing tree in place.
#include <cassert>

// Helper to check if a tree is a valid BST (in-order traversal is sorted).
bool isBST(TreeNode* root, int minVal, int maxVal) {
    if (root == nullptr) return true;
    if (root->val <= minVal || root->val >= maxVal) return false;
    return isBST(root->left, minVal, root->val) && isBST(root->right, root->val, maxVal);
}

// Helper to count nodes.
int countNodes(TreeNode* root) {
    if (!root) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

int main() {
    // Test 1: Delete leaf node (5)
    TreeNode* t1 = new TreeNode(10, new TreeNode(5), new TreeNode(15));
    t1 = deleteNodeFromBST(t1, 5);
    assert(t1 && t1->val == 10);
    assert(t1->left == nullptr && t1->right && t1->right->val == 15);
    assert(isBST(t1, INT_MIN, INT_MAX) && countNodes(t1) == 2);

    // Test 2: Delete node with one child (10 from tree with left child only)
    TreeNode* t2 = new TreeNode(10, new TreeNode(5), nullptr);
    t2 = deleteNodeFromBST(t2, 10);
    assert(t2 && t2->val == 5 && t2->left == nullptr && t2->right == nullptr);
    assert(isBST(t2, INT_MIN, INT_MAX));

    // Test 3: Delete node with two children (10)
    TreeNode* t3 = new TreeNode(10,
                                new TreeNode(5),
                                new TreeNode(15, new TreeNode(12), new TreeNode(20)));
    t3 = deleteNodeFromBST(t3, 10);
    assert(t3 && t3->val == 12); // successor 12 replaces 10
    assert(isBST(t3, INT_MIN, INT_MAX) && countNodes(t3) == 4);

    // Test 4: Delete non-existent key (99) – tree unchanged
    TreeNode* t4 = new TreeNode(8, new TreeNode(3), new TreeNode(10));
    TreeNode* original = t4; // save pointer before call
    t4 = deleteNodeFromBST(t4, 99);
    assert(t4 == original); // same root, no modification
    assert(isBST(t4, INT_MIN, INT_MAX) && countNodes(t4) == 3);

    // Test 5: Delete root from single-node tree
    TreeNode* t5 = new TreeNode(7);
    t5 = deleteNodeFromBST(t5, 7);
    assert(t5 == nullptr);

    // Test 6: Larger tree – delete 20 (node with two children, right subtree has left child)
    TreeNode* t6 = new TreeNode(20,
                                new TreeNode(10),
                                new TreeNode(30,
                                             new TreeNode(25),
                                             new TreeNode(40)));
    t6 = deleteNodeFromBST(t6, 20);
    assert(t6 && t6->val == 25); // successor 25
    assert(isBST(t6, INT_MIN, INT_MAX) && countNodes(t6) == 4);

    // Clean up (optional for exercise, but good practice)
    // (In a full test, you'd write a destructor to free all nodes.)
    return 0;
}
#include <cstddef>

// Definition for a binary tree node (provided in the task).
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Deletes the node with the given key from a BST and returns the root of the updated tree.
TreeNode* deleteNodeFromBST(TreeNode* root, int key) {
    if (root == nullptr) {
        return nullptr;
    }

    if (key < root->val) {
        // Key is in the left subtree.
        root->left = deleteNodeFromBST(root->left, key);
    } else if (key > root->val) {
        // Key is in the right subtree.
        root->right = deleteNodeFromBST(root->right, key);
    } else {
        // Found the node to delete.
        if (root->left == nullptr) {
            // No left child (or leaf with no right child).
            TreeNode* rightChild = root->right;
            delete root;  // free memory (optional in exercise, but good practice)
            return rightChild;
        }
        if (root->right == nullptr) {
            // No right child.
            TreeNode* leftChild = root->left;
            delete root;
            return leftChild;
        }
        // Two children: find in‑order successor (smallest in right subtree).
        TreeNode* successor = root->right;
        while (successor->left != nullptr) {
            successor = successor->left;
        }
        // Copy successor's value to current node.
        root->val = successor->val;
        // Recursively delete the successor from the right subtree.
        root->right = deleteNodeFromBST(root->right, successor->val);
    }
    return root;
}
// The algorithm follows the standard BST deletion procedure. Starting from the root, we compare the target key with the current node’s value. If the key is smaller, recurse on the left child; if larger, recurse on the right child. Once we find the node to delete (current value equals key), we handle three cases:
// 1. **Leaf node** (both children null): return `nullptr` to the parent, effectively removing it.
// 2. **One child**: return the non‑null child to the parent, bypassing the current node.
// 3. **Two children**: find the in‑order successor by moving to the right child and then repeatedly going left until the leftmost node. Copy that successor’s value into the current node, then recursively call delete on the right subtree with the successor’s value to remove the duplicate.
//
// Edge cases: 
// - The root itself may be the target, possibly with one child or two children; the recursive structure handles this because the function returns the new root.
// - If the key is not found, the recursion terminates at a null node and returns that null upward without changes.
// - The recursion depth is at most the height of the tree, but for a skewed tree it could be O(n). Space is O(h) due to the call stack (if using recursion). Time complexity is O(h) for a balanced BST, worst‑case O(n) for a skewed tree.
