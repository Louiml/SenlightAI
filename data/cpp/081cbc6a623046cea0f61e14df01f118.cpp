/*
Write a C++ function `int binaryTreeSize(const Node* root)` that computes the total number of nodes in a binary tree given a pointer to its root. The tree is defined by the `Node` struct with fields `int key; Node* left; Node* right;`. The function must return 0 for an empty tree (null pointer) and must handle arbitrarily shaped trees, including skewed trees where every node has only one child. The function should not modify the tree, be `const`-correct, and use recursion to traverse the entire tree.
*/

struct Node {
    int key;
    Node* left;
    Node* right;
    Node(int x) : key(x), left(nullptr), right(nullptr) {}
};

// Recursively compute the total number of nodes in the binary tree.
int binaryTreeSize(const Node* root) {
    if (root == nullptr) {
        return 0;
    }
    return 1 + binaryTreeSize(root->left) + binaryTreeSize(root->right);
}

#include <cassert>

int main() {
    // Empty tree
    assert(binaryTreeSize(nullptr) == 0);

    // Single node tree
    Node* single = new Node(42);
    assert(binaryTreeSize(single) == 1);

    // Balanced tree (like the example)
    Node* root = new Node(10);
    root->left = new Node(20);
    root->right = new Node(30);
    root->right->right = new Node(60);
    root->left->left = new Node(40);
    root->left->right = new Node(50);
    root->left->right->left = new Node(70);
    root->left->right->right = new Node(80);
    assert(binaryTreeSize(root) == 8);

    // Left-skewed tree
    Node* leftSkew = new Node(1);
    leftSkew->left = new Node(2);
    leftSkew->left->left = new Node(3);
    leftSkew->left->left->left = new Node(4);
    assert(binaryTreeSize(leftSkew) == 4);

    // Right-skewed tree
    Node* rightSkew = new Node(1);
    rightSkew->right = new Node(2);
    rightSkew->right->right = new Node(3);
    assert(binaryTreeSize(rightSkew) == 3);

    // Tree with only left child on some nodes and right on others
    Node* mixed = new Node(5);
    mixed->left = new Node(6);
    mixed->right = new Node(7);
    mixed->left->right = new Node(8);
    assert(binaryTreeSize(mixed) == 4);

    return 0;
}

// The solution uses a recursive depth-first traversal. For any node, the size of the subtree rooted at that node equals 1 (for the node itself) plus the size of its left subtree plus the size of its right subtree. The base case occurs when the pointer is `nullptr`, in which case the size is 0. This approach naturally handles edge cases: an empty tree returns 0, a single-node tree returns 1, and skewed trees still visit every node exactly once. The time complexity is O(n), where n is the number of nodes, because each node is visited exactly once. The space complexity is O(h), where h is the height of the tree, due to the maximum depth of the recursive call stack (for a skewed tree, h equals n; for a balanced tree, h is O(log n)).
