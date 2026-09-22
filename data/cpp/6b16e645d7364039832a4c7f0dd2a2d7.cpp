Write a C++ function named `isBalancedTree` that takes a pointer to the root of a binary tree (where each node contains an integer `data` and left/right child pointers) and returns a boolean indicating whether the tree is height-balanced. A height-balanced tree is defined as one where, for every node, the absolute difference between the heights of its left and right subtrees is at most 1, and both subtrees are themselves balanced. The function must handle the empty tree (null root) as balanced, and it should not modify the tree. The function signature should be `bool isBalancedTree(const Node* root)`, where `Node` is a structure defined as in the snippet. You may assume the tree has no cycles and that the node structure is already defined globally, but you must provide a complete standalone implementation of the function (including any helper functions like `treeHeight`) that can be compiled and tested independently, without relying on any other pre-existing code.
// The solution uses a recursive post-order traversal strategy that combines height calculation and balance checking in a single pass to improve efficiency. The core idea is to define a helper function that returns the height of a subtree if it is balanced, and returns -1 (or a sentinel) if that subtree is unbalanced. For each node, we recursively compute the results for its left and right children. If either child returns -1, the current subtree is unbalanced, so we propagate -1 upward. Otherwise, we check the absolute difference between the two returned heights; if it exceeds 1, we return -1; otherwise, we return `1 + max(leftHeight, rightHeight)`. The main function `isBalancedTree` simply calls this helper on the root and checks whether the result is not -1. The empty tree (null root) returns a height of 0, so it is treated as balanced. Edge cases include a single node (balanced), a skewed tree of depth > 1 (unbalanced), and trees where one subtree is empty and the other has depth 2 (unbalanced). The time complexity is O(n) because each node is visited once and constant work is done per node. The space complexity is O(h) due to the recursion stack, where h is the height of the tree (worst-case O(n) for skewed trees, but best-case O(log n) for balanced trees). This approach is superior to the naive solution of computing heights separately for every node, which would be O(n²) in the worst case.
#include <algorithm>
#include <cstdlib>

struct Node {
    int data;
    Node* left;
    Node* right;
};

// Helper function: returns the height of the subtree if it is balanced,
// otherwise returns -1. The height of an empty tree is 0.
int balancedHeight(const Node* node) {
    if (node == nullptr) {
        return 0;
    }

    int leftHeight = balancedHeight(node->left);
    if (leftHeight == -1) {
        return -1;
    }

    int rightHeight = balancedHeight(node->right);
    if (rightHeight == -1) {
        return -1;
    }

    if (std::abs(leftHeight - rightHeight) > 1) {
        return -1;
    }

    return 1 + std::max(leftHeight, rightHeight);
}

// Returns true if the binary tree rooted at 'root' is height-balanced.
// An empty tree is considered balanced.
bool isBalancedTree(const Node* root) {
    return balancedHeight(root) != -1;
}
#include <cassert>

int main() {
    // Test 1: Empty tree is balanced
    Node* empty = nullptr;
    assert(isBalancedTree(empty) == true);

    // Test 2: Single node is balanced
    Node* single = new Node{1, nullptr, nullptr};
    assert(isBalancedTree(single) == true);

    // Test 3: Perfect balanced tree of height 2 (3 nodes)
    Node* root3 = new Node{1, new Node{2, nullptr, nullptr}, new Node{3, nullptr, nullptr}};
    assert(isBalancedTree(root3) == true);

    // Test 4: Tree from original snippet (root 1 with left child 2, and 2 has children 4,5; no right child) -> unbalanced because left subtree height 2, right height 0
    Node* rootUnbalanced = new Node{1, new Node{2, new Node{4, nullptr, nullptr}, new Node{5, nullptr, nullptr}}, nullptr};
    assert(isBalancedTree(rootUnbalanced) == false);

    // Test 5: Balanced tree with heights 2 and 1 (difference 1)
    Node* rootBalanced = new Node{1, new Node{2, nullptr, nullptr}, new Node{3, new Node{4, nullptr, nullptr}, nullptr}};
    assert(isBalancedTree(rootBalanced) == true);

    // Test 6: Unbalanced tree where left subtree is skewed (height 3) and right is a single node
    Node* rootSkewed = new Node{1, new Node{2, new Node{3, new Node{4, nullptr, nullptr}, nullptr}, nullptr}, new Node{5, nullptr, nullptr}};
    assert(isBalancedTree(rootSkewed) == false);

    // Test 7: Balanced tree with only left subtree of depth 2 (root has no right child) -> unbalanced (difference 2)
    Node* rootLeftOnly = new Node{1, new Node{2, new Node{3, nullptr, nullptr}, nullptr}, nullptr};
    assert(isBalancedTree(rootLeftOnly) == false);

    // Test 8: A more complex balanced tree (7 nodes, perfect of height 2)
    Node* perfect2 = new Node{1, new Node{2, new Node{4, nullptr, nullptr}, new Node{5, nullptr, nullptr}}, new Node{3, new Node{6, nullptr, nullptr}, new Node{7, nullptr, nullptr}}};
    assert(isBalancedTree(perfect2) == true);

    // Cleanup (optional for this test, but good practice)
    // (Actual deletion would require recursive function; omitted for brevity)

    return 0;
}
