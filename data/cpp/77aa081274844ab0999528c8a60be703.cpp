// Write a C++ function named `isBalancedTree` that takes a pointer to the root node of a binary tree (defined by the provided `BinaryTreeNode` struct) and returns a `bool` indicating whether the tree is height-balanced. A binary tree is height-balanced if, for every node, the absolute difference between the heights of its left and right subtrees is at most 1. The function must operate in a single traversal (i.e., it cannot first compute the height of every subtree separately and then check balance, as that would be O(n log n) on unbalanced trees). The tree may be empty (null root), in which case it is considered balanced. The implementation must not modify the tree and must be `const`-correct. Do not include a `main` function — only provide the function and any helper functions it requires, along with the necessary `#include <utility>` or equivalent for `std::pair` or similar. Assume the `BinaryTreeNode` struct is defined as: `struct BinaryTreeNode { int m_nValue; BinaryTreeNode* m_pLeft; BinaryTreeNode* m_pRight; };` (but you may include that definition in your solution to make it self-contained).
// The classic approach is to compute the height and balance status of a subtree in one post-order traversal. For each node, we recursively check its left and right subtrees. If either subtree is unbalanced, the entire tree is unbalanced, and we can return `false` immediately (or propagate a sentinel height of -1). If both are balanced, we compare their heights; if the difference exceeds 1, it's unbalanced. Otherwise, the current node's height is `1 + max(leftHeight, rightHeight)`, and we return `true`. To avoid a second traversal, we can use a return value that packs both the balance status and height, e.g., an `int` where a negative value (like -1) indicates unbalanced, and non-negative values represent the height. Alternatively, use a reference parameter for the height and return a `bool`. The edge cases include: null root (balanced, height 0), a single node (balanced, height 1), a left-heavy chain (unbalanced at root), and a tree that is balanced at the root but unbalanced deeper (e.g., a tree where the left subtree is balanced but has a deep right child causing imbalance below). Time complexity is O(n) because each node is visited once. Space complexity is O(h) for the recursion stack, where h is the tree height (worst-case O(n) for a skewed tree).
#include <algorithm>

struct BinaryTreeNode {
    int m_nValue;
    BinaryTreeNode* m_pLeft;
    BinaryTreeNode* m_pRight;
    BinaryTreeNode(int value) : m_nValue(value), m_pLeft(nullptr), m_pRight(nullptr) {}
};

// Helper: returns true if balanced, and sets *pDepth to the height of the subtree.
bool isBalancedHelper(const BinaryTreeNode* pRoot, int* pDepth) {
    if (pRoot == nullptr) {
        *pDepth = 0;
        return true;
    }

    int leftHeight, rightHeight;
    if (isBalancedHelper(pRoot->m_pLeft, &leftHeight) 
        && isBalancedHelper(pRoot->m_pRight, &rightHeight)) {
        int diff = leftHeight - rightHeight;
        if (diff <= 1 && diff >= -1) {
            *pDepth = 1 + std::max(leftHeight, rightHeight);
            return true;
        }
    }
    return false;
}

// Public function: returns true if the binary tree is height-balanced.
bool isBalancedTree(const BinaryTreeNode* pRoot) {
    int depth = 0;
    return isBalancedHelper(pRoot, &depth);
}
#include <cassert>

int main() {
    // Test 1: Empty tree
    assert(isBalancedTree(nullptr) == true);

    // Test 2: Single node
    BinaryTreeNode* n1 = new BinaryTreeNode(1);
    assert(isBalancedTree(n1) == true);
    delete n1;

    // Test 3: Perfect binary tree (balanced)
    BinaryTreeNode* root = new BinaryTreeNode(1);
    root->m_pLeft = new BinaryTreeNode(2);
    root->m_pRight = new BinaryTreeNode(3);
    root->m_pLeft->m_pLeft = new BinaryTreeNode(4);
    root->m_pLeft->m_pRight = new BinaryTreeNode(5);
    assert(isBalancedTree(root) == true);

    // Test 4: Unbalanced at root (left height 3, right height 1)
    BinaryTreeNode* root2 = new BinaryTreeNode(1);
    root2->m_pLeft = new BinaryTreeNode(2);
    root2->m_pLeft->m_pLeft = new BinaryTreeNode(3);
    root2->m_pLeft->m_pLeft->m_pLeft = new BinaryTreeNode(4);
    root2->m_pRight = new BinaryTreeNode(5);
    assert(isBalancedTree(root2) == false);

    // Test 5: Balanced at root but unbalanced deeper (right-left)
    BinaryTreeNode* root3 = new BinaryTreeNode(1);
    root3->m_pLeft = new BinaryTreeNode(2);
    root3->m_pRight = new BinaryTreeNode(3);
    root3->m_pRight->m_pRight = new BinaryTreeNode(4);
    root3->m_pRight->m_pRight->m_pRight = new BinaryTreeNode(5);
    assert(isBalancedTree(root3) == false);

    // Test 6: Balanced but not perfect (second example from snippet)
    BinaryTreeNode* root4 = new BinaryTreeNode(1);
    root4->m_pLeft = new BinaryTreeNode(2);
    root4->m_pRight = new BinaryTreeNode(3);
    root4->m_pLeft->m_pLeft = new BinaryTreeNode(4);
    root4->m_pLeft->m_pRight = new BinaryTreeNode(5);
    root4->m_pRight->m_pRight = new BinaryTreeNode(6);
    root4->m_pLeft->m_pRight->m_pLeft = new BinaryTreeNode(7);
    assert(isBalancedTree(root4) == true);

    // Cleanup
    // (Implementation of tree deletion omitted for brevity, but in real code would recursively delete)
    delete root4->m_pLeft->m_pRight->m_pLeft;
    delete root4->m_pLeft->m_pRight;
    delete root4->m_pLeft->m_pLeft;
    delete root4->m_pLeft;
    delete root4->m_pRight->m_pRight;
    delete root4->m_pRight;
    delete root4;

    delete root2->m_pLeft->m_pLeft->m_pLeft;
    delete root2->m_pLeft->m_pLeft;
    delete root2->m_pLeft;
    delete root2->m_pRight;
    delete root2;

    delete root3->m_pRight->m_pRight->m_pRight;
    delete root3->m_pRight->m_pRight;
    delete root3->m_pRight;
    delete root3->m_pLeft;
    delete root3;

    delete root->m_pLeft->m_pLeft;
    delete root->m_pLeft->m_pRight;
    delete root->m_pLeft;
    delete root->m_pRight;
    delete root;

    return 0;
}
