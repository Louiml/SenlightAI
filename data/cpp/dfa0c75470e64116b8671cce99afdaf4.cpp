/*
Write a C++ function `bool isHeightBalanced(const Node* root)` that determines whether a binary tree is height-balanced. A binary tree is height-balanced if, for every node in the tree, the absolute difference between the heights of its left and right subtrees is at most 1. The function must compute the height of each subtree simultaneously while checking the balance condition, avoiding separate traversals. The tree is represented using a `Node` struct with `int data`, `Node* left`, and `Node* right`, where empty subtrees are `nullptr`. The function must handle the empty tree as balanced, define the height of an empty subtree as -1, and return `true` for an empty or single-node tree. Nodes have no parent pointers, and the function must not modify the tree (i.e., it must be `const`‑correct). For a tree with `n` nodes, the solution must run in `O(n)` time and use `O(h)` auxiliary space (recursion stack), where `h` is the tree height.
*/
#include <algorithm> // for std::max
#include <cstdlib>   // for std::abs

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Helper structure to carry both height and balance status.
struct BalanceInfo {
    int height;
    bool balanced;
};

// Recursively check balance and compute height in one pass.
BalanceInfo checkBalance(const Node* root) {
    if (root == nullptr) {
        return {-1, true}; // empty subtree: height -1, balanced
    }
    BalanceInfo leftInfo = checkBalance(root->left);
    BalanceInfo rightInfo = checkBalance(root->right);

    bool currentBalanced = leftInfo.balanced && rightInfo.balanced &&
                           (std::abs(leftInfo.height - rightInfo.height) <= 1);
    int currentHeight = 1 + std::max(leftInfo.height, rightInfo.height);
    return {currentHeight, currentBalanced};
}

// Public function: returns true if the tree rooted at 'root' is height-balanced.
bool isHeightBalanced(const Node* root) {
    return checkBalance(root).balanced;
}
#include <cassert>

// Helper to build a tree manually for tests (since no buildTree in solution).
Node* createNode(int val) {
    return new Node(val);
}

int main() {
    // Test 1: Empty tree is balanced.
    assert(isHeightBalanced(nullptr) == true);

    // Test 2: Single node is balanced.
    Node* single = createNode(1);
    assert(isHeightBalanced(single) == true);

    // Test 3: Balanced tree with 3 nodes (root, left, right).
    Node* balanced3 = createNode(1);
    balanced3->left = createNode(2);
    balanced3->right = createNode(3);
    assert(isHeightBalanced(balanced3) == true);

    // Test 4: Unbalanced left-heavy chain (node left child only, right null).
    Node* leftChain = createNode(1);
    leftChain->left = createNode(2);
    leftChain->left->left = createNode(3);
    // Heights: left subtree height 1, right null height -1 => diff 2 > 1 => not balanced.
    assert(isHeightBalanced(leftChain) == false);

    // Test 5: Balanced but deeper tree (perfect tree of height 2).
    Node* balanced7 = createNode(4);
    balanced7->left = createNode(2);
    balanced7->right = createNode(6);
    balanced7->left->left = createNode(1);
    balanced7->left->right = createNode(3);
    balanced7->right->left = createNode(5);
    balanced7->right->right = createNode(7);
    assert(isHeightBalanced(balanced7) == true);

    // Test 6: Tree where left subtree is unbalanced.
    Node* root6 = createNode(1);
    root6->left = createNode(2);
    root6->right = createNode(3);
    root6->left->left = createNode(4);
    root6->left->left->left = createNode(5); // makes left subtree height 3 vs right height 0? Actually left height 2, right height 0 => diff 2 > 1 → false.
    assert(isHeightBalanced(root6) == false);

    // Test 7: Tree where both children are null but one child has left child (impossible, but test null safety).
    Node* root7 = createNode(1);
    root7->right = createNode(2);
    root7->right->left = createNode(3);
    // left null height -1, right height 1 => diff 2 → false.
    assert(isHeightBalanced(root7) == false);

    // Clean up (not strictly necessary for tests, but good practice).
    // (Skipping full deletion for brevity; test environment manages memory.)

    return 0;
}
// The core idea is a single post‑order traversal that returns both the height and the balanced status of each subtree, avoiding redundant height computations. For every node, we recursively compute a pair `{height, balanced}` for its left and right children. The height of the current node is `1 + max(leftHeight, rightHeight)` (with empty subtree height being -1). The current node is balanced if both children are balanced **and** the absolute difference in their heights is ≤ 1. The overall tree is balanced if the root's pair reports `balanced == true`. Edge cases include the empty tree (height -1, balanced true), a single node (height 0, balanced true), skewed trees (height difference > 1, balanced false), and trees where one child is empty and the other has height > 1 (difference > 1, balanced false). Time complexity is `O(n)` because each node is visited once. Space complexity is `O(h)` due to the recursion stack, which for a skewed tree is `O(n)` and for a balanced tree is `O(log n)`.
