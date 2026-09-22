Write a C++ function named `mirrorTree` that takes a pointer to the root of a binary tree (with nodes defined as in the provided snippet, each containing an integer value and left/right child pointers) and recursively transforms the tree into its mirror image by swapping the left and right children of every node. The function should return a pointer to the new root (which is the same as the original root after mirroring, since we modify in place). The function must handle an empty tree (null pointer) gracefully, performing no operation and returning nullptr. The solution must not use any external libraries beyond standard ones, must avoid memory leaks, and must not create new nodes—only rearrange existing ones.
#include <cassert>

int main()
{
    // Test 1: Empty tree
    Tree* empty = nullptr;
    assert(mirrorTree(empty) == nullptr);

    // Test 2: Single node
    Tree* single = new Tree(42);
    mirrorTree(single);
    assert(single->root_data == 42);
    assert(single->left == nullptr && single->right == nullptr);
    delete single;

    // Test 3: Simple tree with two children
    // Original:    1
    //            /   \
    //           2     3
    Tree* root = new Tree(1);
    root->left = new Tree(2);
    root->right = new Tree(3);
    Tree* mirrored = mirrorTree(root);
    assert(mirrored == root);
    assert(root->left != nullptr && root->left->root_data == 3);
    assert(root->right != nullptr && root->right->root_data == 2);
    assert(root->left->left == nullptr && root->left->right == nullptr);
    assert(root->right->left == nullptr && root->right->right == nullptr);

    // Cleanup
    delete root->left;
    delete root->right;
    delete root;

    // Test 4: Larger tree mirror (mirror twice to get original structure back)
    // Original:       1
    //               /   \
    //              2     3
    //             / \   /
    //            4   5 6
    Tree* big = new Tree(1);
    big->left = new Tree(2);
    big->right = new Tree(3);
    big->left->left = new Tree(4);
    big->left->right = new Tree(5);
    big->right->left = new Tree(6);

    Tree* first = mirrorTree(big);
    // After first mirror, check some structure
    // Structure should be:
    //       1
    //     /   \
    //    3     2
    //     \   / \
    //      6 5   4
    assert(first->left->root_data == 3);
    assert(first->right->root_data == 2);
    assert(first->left->right->root_data == 6);
    assert(first->left->left == nullptr);
    assert(first->right->right->root_data == 4);

    // Mirror again to get original back
    Tree* second = mirrorTree(first);
    assert(second == big);
    assert(second->left->root_data == 2);
    assert(second->right->root_data == 3);
    assert(second->left->left->root_data == 4);
    assert(second->left->right->root_data == 5);
    assert(second->right->left->root_data == 6);
    assert(second->right->right == nullptr);

    // Cleanup (free all dynamically allocated nodes)
    delete second->left->left;
    delete second->left->right;
    delete second->right->left;
    delete second->left;
    delete second->right;
    delete second;

    // Test 5: Left-skewed tree
    Tree* skewed = new Tree(1);
    skewed->left = new Tree(2);
    skewed->left->left = new Tree(3);
    skewed->left->left->left = new Tree(4);
    mirrorTree(skewed);
    assert(skewed->right != nullptr);
    assert(skewed->right->right != nullptr);
    assert(skewed->right->right->right != nullptr);
    assert(skewed->right->root_data == 2);
    assert(skewed->right->right->root_data == 3);
    assert(skewed->right->right->right->root_data == 4);
    assert(skewed->left == nullptr);

    delete skewed->right->right->right;
    delete skewed->right->right;
    delete skewed->right;
    delete skewed;

    return 0;
}
#include <algorithm> // for std::swap

struct Tree
{
    int root_data;
    Tree *left;
    Tree *right;

    explicit Tree(int value) : root_data(value), left(nullptr), right(nullptr) {}
};

// Recursively mirror the binary tree in-place and return the root.
// Returns nullptr if the input tree is empty.
Tree* mirrorTree(Tree* root)
{
    if (root == nullptr)
    {
        return nullptr;
    }

    // Swap the left and right children of the current node.
    std::swap(root->left, root->right);

    // Recursively mirror the (now swapped) subtrees.
    mirrorTree(root->left);
    mirrorTree(root->right);

    return root;
}
// The main algorithm is a recursive post-order or pre-order swap: for any non-null node, swap its left and right child pointers, then recursively apply the same operation to both the (now swapped) left and right subtrees. Since we swap the pointers first, the recursion can be done on either the original left or right before swapping, but it's cleaner to swap first then recurse on both children. The base case is when the node is null—return null immediately. After processing both children, return the same node pointer. Important edge cases: a single-node tree (no children) should remain unchanged but the function still returns that node; a skewed tree should work correctly; an empty tree (null root) should return null without dereferencing. Time complexity is O(n) where n is the number of nodes, since each node is visited exactly once. Auxiliary space is O(h) where h is the height of the tree due to recursion stack (O(n) in worst-case skewed tree, O(log n) for balanced). No extra memory for nodes is used.
