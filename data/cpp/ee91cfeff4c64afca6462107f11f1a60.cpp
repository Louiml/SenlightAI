// Write a C++ function that takes a pointer to the root of a binary tree whose nodes store integer values (represented by a simple `BSTNode<int>` struct with `left`, `right`, and a `print()` method), and inverts the tree by swapping every node's left and right children recursively. The function must modify the tree in place and return nothing (void). It should handle an empty tree (null pointer) gracefully without crashing. The operation must preserve all node values and the overall structure, only mirroring the left/right subtrees. After the call, the tree's in-order traversal order is reversed.

#include <cassert>
#include <vector>

// Helper to manually check in-order traversal (for validation only).
void collectInOrder(BSTNode* node, std::vector<int>& out) {
    if (!node) return;
    collectInOrder(node->left, out);
    out.push_back(node->data);
    collectInOrder(node->right, out);
}

// Helper to delete tree.
void deleteTree(BSTNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

int main() {
    // Test 1: Empty tree (null root) does nothing.
    BSTNode* root1 = nullptr;
    mirrorTree(root1);
    assert(root1 == nullptr);

    // Test 2: Single node remains unchanged.
    BSTNode* root2 = new BSTNode(5);
    mirrorTree(root2);
    assert(root2->left == nullptr && root2->right == nullptr && root2->data == 5);
    delete root2;

    // Test 3: Small tree (mirrors correctly).
    BSTNode* root3 = new BSTNode(1);
    root3->left = new BSTNode(2);
    root3->right = new BSTNode(3);
    mirrorTree(root3);
    assert(root3->left->data == 3 && root3->right->data == 2);
    deleteTree(root3);

    // Test 4: Larger tree, check in-order reversal.
    BSTNode* root4 = new BSTNode(10);
    root4->left = new BSTNode(20);
    root4->right = new BSTNode(30);
    root4->left->left = new BSTNode(40);
    root4->left->right = new BSTNode(50);
    root4->right->left = new BSTNode(60);
    root4->right->right = new BSTNode(70);
    mirrorTree(root4);
    std::vector<int> result;
    collectInOrder(root4, result);
    std::vector<int> expected = {70, 30, 60, 10, 50, 20, 40};
    assert(result == expected);
    deleteTree(root4);

    // Test 5: Skewed left tree becomes skewed right.
    BSTNode* root5 = new BSTNode(1);
    root5->left = new BSTNode(2);
    root5->left->left = new BSTNode(3);
    mirrorTree(root5);
    assert(root5->right->right->data == 3);
    assert(root5->left == nullptr);
    deleteTree(root5);

    return 0;
}

#include <cstddef>

// Simple binary tree node for integer values.
struct BSTNode {
    int data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Mirror the given binary tree in place by swapping left and right children
// of every node recursively. Does nothing if node is null.
void mirrorTree(BSTNode* node) {
    if (node == nullptr) {
        return;
    }
    // First mirror the subtrees (post-order traversal).
    mirrorTree(node->left);
    mirrorTree(node->right);
    // Swap left and right child pointers.
    BSTNode* temp = node->left;
    node->left = node->right;
    node->right = temp;
}

// The solution uses a recursive post-order traversal: for each node, first recursively mirror its left subtree, then its right subtree, and finally swap the node's left and right child pointers. The base case is when the node is null, in which case nothing is done. This ensures that by the time we swap at a node, both its subtrees have already been fully mirrored, so the swap correctly inverts the entire tree. Edge cases include a null root (empty tree) and a leaf node (both children null), which require no swapping. The algorithm visits every node exactly once, so time complexity is O(n) where n is the number of nodes. Auxiliary space is O(h) due to the recursion stack, h being the height of the tree (O(n) worst-case for a skewed tree, O(log n) average for balanced). The function takes a non-const pointer because it mutates the tree, and no `const` is applied to the parameter since we modify the nodes.
