/*
Write a C++ function `TreeNode* mirrorTree(TreeNode* root)` that takes the root of a binary tree and returns the root of the mirrored (inverted) tree, where every node's left and right subtrees are swapped recursively. The tree nodes are defined by the provided `TreeNode` struct with integer values and left/right child pointers. The function must handle an empty tree (nullptr) gracefully and preserve the original tree's structure aside from the mirroring. It should modify the tree in place and return the same root pointer. The function must be `const`-safe only in the sense that it does not modify the tree unless swapping pointers, and it should not allocate new nodes.
*/
#include <cstddef>  // for nullptr

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Recursively mirror the binary tree in place and return the root.
TreeNode* mirrorTree(TreeNode* root) {
    if (root == nullptr) {
        return nullptr;
    }
    // Invert left and right subtrees first.
    mirrorTree(root->left);
    mirrorTree(root->right);
    // Swap the left and right children.
    TreeNode* temp = root->left;
    root->left = root->right;
    root->right = temp;
    return root;
}
#include <cassert>

// Helper to manually build a tree and compare with expected mirror.
int main() {
    // Test 1: Empty tree
    assert(mirrorTree(nullptr) == nullptr);

    // Test 2: Single node
    TreeNode* single = new TreeNode(5);
    assert(mirrorTree(single) == single);
    assert(single->left == nullptr && single->right == nullptr);
    delete single;

    // Test 3: Full small tree: root=1, left=2, right=3
    TreeNode* n1 = new TreeNode(1);
    n1->left = new TreeNode(2);
    n1->right = new TreeNode(3);
    TreeNode* mirrored = mirrorTree(n1);
    assert(mirrored == n1);  // same root pointer
    assert(n1->left->val == 3 && n1->right->val == 2);
    assert(n1->left->left == nullptr && n1->left->right == nullptr);
    assert(n1->right->left == nullptr && n1->right->right == nullptr);
    delete n1->left; delete n1->right; delete n1;

    // Test 4: Deeper tree: root=4, left subtree = (2,1,3), right subtree = (7,6,9)
    TreeNode* root = new TreeNode(4);
    root->left = new TreeNode(2);
    root->left->left = new TreeNode(1);
    root->left->right = new TreeNode(3);
    root->right = new TreeNode(7);
    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(9);
    mirrorTree(root);
    // Root children swapped
    assert(root->left->val == 7 && root->right->val == 2);
    // Left subtree (originally right) now mirrored accordingly
    assert(root->left->left->val == 9 && root->left->right->val == 6);
    // Right subtree (originally left) now mirrored accordingly
    assert(root->right->left->val == 3 && root->right->right->val == 1);
    // Cleanup
    delete root->left->left; delete root->left->right; delete root->left;
    delete root->right->left; delete root->right->right; delete root->right;
    delete root;

    // Test 5: Skewed tree (only left children)
    TreeNode* skewed = new TreeNode(10);
    skewed->left = new TreeNode(20);
    skewed->left->left = new TreeNode(30);
    mirrorTree(skewed);
    assert(skewed->right->val == 20);
    assert(skewed->right->right->val == 30);
    assert(skewed->left == nullptr);
    delete skewed->right->right; delete skewed->right; delete skewed;

    return 0;
}
// The solution recursively visits each node in a post-order manner: first recursively invert the left subtree, then the right subtree, and finally swap the left and right child pointers of the current node. This works because after both children are inverted, swapping them yields the fully mirrored subtree. The base case is when the node is `nullptr`, in which case we simply return `nullptr`. The algorithm processes each node exactly once, so time complexity is O(n) where n is the number of nodes. The recursion uses stack space proportional to the tree height, so space complexity is O(h) in the worst case (O(n) for a skewed tree, O(log n) for a balanced tree), excluding the input/output storage. Edge cases include an empty tree (returns nullptr) and a single-node tree (returns same node with both children null). No special handling is needed for duplicate values or missing children; the swap naturally handles null pointers.
