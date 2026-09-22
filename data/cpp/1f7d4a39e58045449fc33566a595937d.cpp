Given a binary tree represented by a `Node` class where each node stores an integer `val` and pointers to `left` and `right` children, write a C++ function `int treeHeight(const Node* root)` that returns the height of the tree. The height is defined as the number of edges on the longest path from the root to a leaf node. An empty tree (null root) has height -1, and a tree with a single node has height 0. The function must be `const`-correct, meaning it should accept a `const Node*` and must not modify the tree. Assume the tree is not necessarily balanced or complete.

// The problem is a classic tree traversal problem solved using recursion. The height of a node is defined recursively as: if the node is null, height is -1; otherwise, height = 1 + max(height(left child), height(right child)). This works because the height of a tree is the longest path to a leaf, and for a leaf, both children are null giving height 0 = 1 + max(-1, -1). The recursion explores each node exactly once. Edge cases include an empty tree (null root) returning -1, a single-node tree returning 0, and a skewed tree (e.g., all left children) where the recursion depth equals the number of nodes. Time complexity is O(n) where n is the number of nodes, because each node is visited once. Space complexity is O(h) for the recursion stack, where h is the height of the tree; in the worst case (skewed tree) this is O(n).

#include <algorithm> // for std::max

class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node(int val) : val(val), left(nullptr), right(nullptr) {}
};

// Return the height of a binary tree (edges from root to deepest leaf).
// Empty tree height = -1, single node height = 0.
int treeHeight(const Node* root) {
    if (root == nullptr) {
        return -1;
    }
    int leftHeight = treeHeight(root->left);
    int rightHeight = treeHeight(root->right);
    return 1 + std::max(leftHeight, rightHeight);
}

#include <cassert>

int main() {
    // Empty tree
    assert(treeHeight(nullptr) == -1);

    // Single node
    Node* a = new Node(10);
    assert(treeHeight(a) == 0);

    // Two nodes: root with left child
    Node* b = new Node(20);
    a->left = b;
    assert(treeHeight(a) == 1);

    // Add right child to root, height still 1
    Node* c = new Node(30);
    a->right = c;
    assert(treeHeight(a) == 1);

    // Left child b has a left leaf -> height 2
    Node* d = new Node(40);
    b->left = d;
    assert(treeHeight(a) == 2);

    // Right child c has right child -> height still 2
    Node* e = new Node(50);
    c->right = e;
    assert(treeHeight(a) == 2);

    // Extend right path to depth 3
    Node* f = new Node(60);
    e->right = f;
    assert(treeHeight(a) == 3);

    // Skewed tree: only left children
    Node* g = new Node(70);
    d->left = g;
    assert(treeHeight(a) == 4);

    // Cleanup (optional, not needed for correctness)
    delete a; delete b; delete c; delete d; delete e; delete f; delete g;

    return 0;
}
