// Write a C++ function `bool areIsomorphic(Node* root1, Node* root2)` that determines whether two binary search trees are isomorphic. Two binary trees are considered isomorphic if one can be transformed into the other by swapping the left and right children of any number of nodes. The function must handle empty trees, trees with different structures, and duplicate values. The trees are composed of nodes with integer `data` values and `left`/`right` pointers. The function should return `true` if the trees are isomorphic, and `false` otherwise.
The problem is classic tree isomorphism. We recursively compare both trees. The base cases: if both roots are `nullptr`, they are isomorphic (true); if one is `nullptr` and the other is not, they are not isomorphic (false). Then we compare the data values — if they differ, return false. For non-null nodes with equal data, there are two possible ways the subtrees could be isomorphic: (1) without swapping — left subtree of root1 compared to left subtree of root2, and right to right; (2) with swapping — left of root1 compared to right of root2, and right of root1 compared to left of root2. Return true if either combination is fully isomorphic. Edge cases include both trees being empty (true), one empty and one not (false), and trees of identical structure but swapped children at some level (true). The time complexity is O(n) where n is the number of nodes in the smaller tree (since we stop early on mismatch), but worst-case visits all nodes in both trees — O(n1+n2). Space complexity is O(h) for recursion stack, where h is the height of the trees (worst-case O(n) for skewed trees).
#include <cstddef>

// Definition for a binary tree node.
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Checks if two binary trees are isomorphic (swappable children).
bool areIsomorphic(Node* root1, Node* root2) {
    // Both empty -> isomorphic
    if (root1 == nullptr && root2 == nullptr) {
        return true;
    }
    // One empty, one not -> not isomorphic
    if (root1 == nullptr || root2 == nullptr) {
        return false;
    }
    // Different node values -> not isomorphic
    if (root1->data != root2->data) {
        return false;
    }
    // Two possible structural matches: no swap or swap
    bool noSwap = areIsomorphic(root1->left, root2->left) && areIsomorphic(root1->right, root2->right);
    bool withSwap = areIsomorphic(root1->left, root2->right) && areIsomorphic(root1->right, root2->left);
    return noSwap || withSwap;
}
#include <cassert>

// Helper to create a new node (for testing)
Node* newNode(int data) {
    return new Node(data);
}

int main() {
    // Test 1: Both empty -> true
    assert(areIsomorphic(nullptr, nullptr) == true);

    // Test 2: One empty, one not -> false
    Node* t1 = newNode(1);
    assert(areIsomorphic(t1, nullptr) == false);
    assert(areIsomorphic(nullptr, t1) == false);

    // Test 3: Same single node -> true
    Node* t2 = newNode(1);
    assert(areIsomorphic(t1, t2) == true);

    // Test 4: Different data values -> false
    Node* t3 = newNode(2);
    assert(areIsomorphic(t1, t3) == false);

    // Test 5: Balanced trees with swapped children at root -> true
    // Tree1:      1        Tree2:      1
    //            / \                / \
    //           2   3              3   2
    Node* tree1 = newNode(1);
    tree1->left = newNode(2);
    tree1->right = newNode(3);
    Node* tree2 = newNode(1);
    tree2->left = newNode(3);
    tree2->right = newNode(2);
    assert(areIsomorphic(tree1, tree2) == true);

    // Test 6: Balanced trees with different structure -> false
    // Tree1:      1        Tree2:      1
    //            / \                / \
    //           2   3              2   4
    Node* tree3 = newNode(1);
    tree3->left = newNode(2);
    tree3->right = newNode(4);
    assert(areIsomorphic(tree1, tree3) == false);

    // Test 7: Skewed tree vs mirror image -> true
    // Tree1: 1 - 2 - 3   (right chain)
    // Tree2: 1 - 2 - 3   (left chain)
    Node* skew1 = newNode(1);
    skew1->right = newNode(2);
    skew1->right->right = newNode(3);
    Node* skew2 = newNode(1);
    skew2->left = newNode(2);
    skew2->left->left = newNode(3);
    assert(areIsomorphic(skew1, skew2) == true);

    // Test 8: Same structure but different values deeper -> false
    Node* deep1 = newNode(1);
    deep1->left = newNode(2);
    deep1->left->left = newNode(3);
    Node* deep2 = newNode(1);
    deep2->left = newNode(2);
    deep2->left->left = newNode(4);
    assert(areIsomorphic(deep1, deep2) == false);

    // Clean up (not strictly required for test)
    // In real code, use proper memory management.
    return 0;
}
