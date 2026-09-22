/*
Write a C++ function named `invertBinaryTree` that takes a pointer to the root of a binary tree (where each node contains an integer value and pointers to left and right children) and returns a pointer to the root of the tree after swapping the left and right subtrees of every node. The function must operate recursively, preserving the structure of the original tree (i.e., modifying it in place, not creating copies). You must also handle edge cases: an empty tree (null pointer) should return null, and a tree with only one node should remain unchanged. The function should be declared as a free function (not a class method) and must be `const`-correct on parameters where appropriate (e.g., the input pointer itself is not modified, but the objects it points to are). Use the `TreeNode` struct definition exactly as provided in the snippet.
*/

#include <utility>  // for std::swap

// Definition for a binary tree node (as provided in the problem).
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Recursively invert (mirror) a binary tree by swapping left and right children at every node.
// Returns the root of the modified tree (same pointer as input for non-empty trees, nullptr for empty).
TreeNode* invertBinaryTree(TreeNode* root) {
    // Base case: empty tree or leaf node (where both children are null) — nothing to swap.
    if (root == nullptr) {
        return nullptr;
    }

    // Swap the left and right child pointers of the current node.
    // Use std::swap to avoid a temporary variable.
    std::swap(root->left, root->right);

    // Recursively invert the left subtree (now the original right subtree) and right subtree.
    invertBinaryTree(root->left);
    invertBinaryTree(root->right);

    // Return the root of the (now inverted) tree.
    return root;
}

#include <cassert>

int main() {
    // Test 1: Empty tree
    TreeNode* empty = nullptr;
    assert(invertBinaryTree(empty) == nullptr);

    // Test 2: Single node tree
    TreeNode single(42);
    TreeNode* singleRoot = &single;
    assert(invertBinaryTree(singleRoot) == singleRoot);
    assert(singleRoot->left == nullptr && singleRoot->right == nullptr);
    assert(singleRoot->val == 42);

    // Test 3: Simple tree: root(1) with left=2, right=3
    TreeNode n2(2), n3(3), n1(1, &n2, &n3);
    TreeNode* root3 = invertBinaryTree(&n1);
    assert(root3 == &n1);
    assert(root3->left == &n3 && root3->right == &n2);
    assert(root3->left->left == nullptr && root3->left->right == nullptr);
    assert(root3->right->left == nullptr && root3->right->right == nullptr);

    // Test 4: Asymmetric tree: root(1) with left=2 (which has left=4), right=3
    TreeNode n4(4);
    TreeNode n2b(2, &n4, nullptr);
    TreeNode n3b(3);
    TreeNode n1b(1, &n2b, &n3b);
    TreeNode* root4 = invertBinaryTree(&n1b);
    assert(root4 == &n1b);
    assert(root4->left == &n3b && root4->right == &n2b);
    assert(root4->right->left == nullptr && root4->right->right == &n4);
    assert(root4->right->right->left == nullptr && root4->right->right->right == nullptr);

    // Test 5: Larger tree: root(1) with left=2 (left=4, right=5) and right=3 (left=6, right=7)
    TreeNode n7(7), n6(6), n5(5), n4b(4);
    TreeNode n2c(2, &n4b, &n5);
    TreeNode n3c(3, &n6, &n7);
    TreeNode n1c(1, &n2c, &n3c);
    TreeNode* root5 = invertBinaryTree(&n1c);
    assert(root5 == &n1c);
    assert(root5->left == &n3c && root5->right == &n2c);
    // Check swapped children of right subtree (originally n2c)
    assert(root5->right->left == &n5 && root5->right->right == &n4b);
    // Check swapped children of left subtree (originally n3c)
    assert(root5->left->left == &n7 && root5->left->right == &n6);
    // Verify leaves
    assert(root5->left->left->left == nullptr && root5->left->left->right == nullptr);
    assert(root5->left->right->left == nullptr && root5->left->right->right == nullptr);
    assert(root5->right->left->left == nullptr && root5->right->left->right == nullptr);
    assert(root5->right->right->left == nullptr && root5->right->right->right == nullptr);

    return 0;
}

// The core algorithm is a straightforward recursive depth-first traversal. At each node, we swap its left and right child pointers, then recursively call the function on the left child (which, after the swap, is the original right subtree) and on the right child (original left subtree). The recursion terminates when we encounter a null pointer (empty subtree). This approach visits every node exactly once. The key edge cases are: (1) an empty tree (`root == nullptr`), which we simply return as null; (2) a tree with only one node (both children null), where the swap does nothing and recursion hits nulls immediately; (3) asymmetric trees where one child is null—swapping still works fine because null pointers are assigned to the other side. Time complexity is O(n) where n is the number of nodes, because each node is visited once. Space complexity is O(h), where h is the tree height, due to recursion stack usage (in the worst case of a skewed tree, h = n, so O(n) in the worst case; in a balanced tree, h = log n). The function modifies the tree in place, so no extra space for storing copies is needed.
