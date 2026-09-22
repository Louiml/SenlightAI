// Write a C++ function `int countInternalNodes(Node* root)` that takes the root of a binary tree (built using the provided `Node` structure, where each node stores an integer `val` and left/right child pointers) and returns the total number of internal nodes in the tree. An internal node is defined as any node that has at least one child (i.e., either left or right pointer is not `NULL`). The tree may be empty (`root == NULL`), may have only a root with no children, or may be skewed (all nodes on one side) or balanced. The function must not modify the tree and must be `const`-correct. The function should be robust for trees containing negative values as well; values themselves do not affect the count.
#include <cassert>
#include <iostream>

// Reuse Node definition from solution (or here for completeness)
struct Node {
    int val;
    Node* left;
    Node* right;
    Node(int value) : val(value), left(nullptr), right(nullptr) {}
};

// Function under test
int countInternalNodes(const Node* root);

int main() {
    // Test 1: empty tree
    Node* empty = nullptr;
    assert(countInternalNodes(empty) == 0);

    // Test 2: single node (leaf)
    Node* single = new Node(5);
    assert(countInternalNodes(single) == 0);

    // Test 3: root with only left child
    Node* rootLeft = new Node(1);
    rootLeft->left = new Node(2);
    assert(countInternalNodes(rootLeft) == 1);

    // Test 4: root with only right child
    Node* rootRight = new Node(1);
    rootRight->right = new Node(3);
    assert(countInternalNodes(rootRight) == 1);

    // Test 5: full binary tree with 3 nodes (root internal, two leaves)
    Node* full = new Node(10);
    full->left = new Node(20);
    full->right = new Node(30);
    assert(countInternalNodes(full) == 1);

    // Test 6: skewed tree of 3 nodes (all internal except last leaf)
    Node* skewed = new Node(1);
    skewed->left = new Node(2);
    skewed->left->left = new Node(3);
    assert(countInternalNodes(skewed) == 2);

    // Test 7: balanced tree with 7 nodes (internal nodes: root, left child, right child)
    Node* balanced = new Node(1);
    balanced->left = new Node(2);
    balanced->right = new Node(3);
    balanced->left->left = new Node(4);
    balanced->left->right = new Node(5);
    balanced->right->left = new Node(6);
    balanced->right->right = new Node(7);
    assert(countInternalNodes(balanced) == 3);

    // Test 8: tree with only root having both children but one child leaf, other subtree deeper
    Node* mixed = new Node(1);
    mixed->left = new Node(2);
    mixed->right = new Node(3);
    mixed->right->left = new Node(4);
    assert(countInternalNodes(mixed) == 2); // root and node 3

    // Cleanup not strictly necessary for asserts but good practice (omitted for brevity)
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <cstddef>

// Definition for a binary tree node.
struct Node {
    int val;
    Node* left;
    Node* right;
    Node(int value) : val(value), left(NULL), right(NULL) {}
};

// Count the number of internal nodes (nodes with at least one child) in a binary tree.
int countInternalNodes(const Node* root) {
    if (root == NULL) {
        return 0;
    }

    // Check if current node is internal (has at least one child)
    bool isInternal = (root->left != NULL) || (root->right != NULL);
    
    // If internal, add 1 and recurse into children; if leaf, return 0
    int count = isInternal ? 1 : 0;
    if (root->left != NULL) {
        count += countInternalNodes(root->left);
    }
    if (root->right != NULL) {
        count += countInternalNodes(root->right);
    }
    return count;
}
// The approach is a straightforward recursive traversal of the binary tree. For a given node, we first check if it is `NULL`; if so, return 0. Otherwise, we check if the node is an internal node by testing whether either `left` or `right` is non-null. If it is internal, we add 1 to the result and recursively count internal nodes in both subtrees. If it is a leaf (both children `NULL`), we return 0 without further recursion (since leaves have no subtrees). The recursion visits every node exactly once, so the time complexity is \(O(n)\) where \(n\) is the number of nodes. The auxiliary space complexity is \(O(h)\) where \(h\) is the height of the tree, due to recursion stack usage (worst-case \(O(n)\) for a skewed tree). Edge cases: empty tree returns 0; a single-node tree (root is leaf) returns 0; a root with exactly one child returns 1 (since root is internal, child is leaf).
