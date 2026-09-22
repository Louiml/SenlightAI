Write a C++ function that takes a binary search tree (BST) represented by a `Nodo*` root pointer (where each node stores an integer `dato` and has left `izd` and right `der` child pointers) and returns the number of nodes that have exactly one child (i.e., degree 1 nodes). The function must handle an empty tree (return 0) and should not modify the tree. The type `Nodo` must be defined as a plain struct with public members, exactly as in the original snippet.

// The solution uses a recursive depth-first traversal of the BST. At each node, count how many children are non-null (0, 1, or 2). If exactly one child exists, add 1 to the count, then recursively process both children regardless. Base case: if the root is null, return 0. This works for any binary tree, but is particularly simple for a BST because no value comparisons are needed for counting — only structural checks. Edge cases: empty tree returns 0; a leaf node (0 children) contributes 0; a node with both children contributes 0 but still recurses; a node with only a left or only a right child contributes 1. Time complexity is O(n) because every node is visited once, and space complexity is O(h) for the recursion stack, where h is tree height (O(log n) for balanced BST, O(n) for skewed).

#include <cstddef>

// Node structure for a binary search tree.
struct Nodo {
    int dato;
    Nodo* der; // right child
    Nodo* izd; // left child
};

// Count nodes in the BST that have exactly one child (degree 1).
// Recursively traverses the tree and returns the count.
int countSingleChildNodes(const Nodo* root) {
    if (root == nullptr) {
        return 0;
    }
    int count = 0;
    // Check if this node has exactly one child.
    if ((root->izd == nullptr) != (root->der == nullptr)) {
        count = 1;
    }
    // Recurse on both subtrees.
    return count + countSingleChildNodes(root->izd) + countSingleChildNodes(root->der);
}

#include <cassert>

int main() {
    // Test 1: Empty tree.
    Nodo* empty = nullptr;
    assert(countSingleChildNodes(empty) == 0);

    // Test 2: Single node (leaf) -> 0.
    Nodo leaf{5, nullptr, nullptr};
    assert(countSingleChildNodes(&leaf) == 0);

    // Test 3: Root with only left child -> 1.
    Nodo leftChild{3, nullptr, nullptr};
    Nodo rootLeft{4, nullptr, &leftChild}; // izd points to leftChild, der = nullptr
    assert(countSingleChildNodes(&rootLeft) == 1);

    // Test 4: Root with only right child -> 1.
    Nodo rightChild{6, nullptr, nullptr};
    Nodo rootRight{4, &rightChild, nullptr}; // der points to rightChild, izd = nullptr
    assert(countSingleChildNodes(&rootRight) == 1);

    // Test 5: Root with both children -> 0.
    Nodo rootBoth{4, &rightChild, &leftChild};
    assert(countSingleChildNodes(&rootBoth) == 0);

    // Test 6: Larger tree: root 5, left=3 (has right child only), right=8 (has both children).
    // Structure: 5 has left 3 and right 8; 3 has right child 4; 8 has left 7 and right 9.
    Nodo node4{4, nullptr, nullptr};
    Nodo node3{3, nullptr, &node4}; // 3 has only right child
    Nodo node7{7, nullptr, nullptr};
    Nodo node9{9, nullptr, nullptr};
    Nodo node8{8, &node9, &node7}; // 8 has both children
    Nodo root{5, &node8, &node3};  // root has both children
    // Single-child nodes: only node3 (degree 1). Others are leaves or degree 2. Total=1.
    assert(countSingleChildNodes(&root) == 1);

    // Test 7: Skewed tree (all right children) -> every node except leaf has exactly one child.
    Nodo n1{1, nullptr, nullptr};
    Nodo n2{2, &n1, nullptr};
    Nodo n3{3, &n2, nullptr};
    Nodo n4{4, &n3, nullptr};
    // Nodes n4 (der), n3 (der), n2 (der) each have exactly one child. Total=3.
    assert(countSingleChildNodes(&n4) == 3);

    return 0;
}
