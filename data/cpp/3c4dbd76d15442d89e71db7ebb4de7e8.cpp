// Given the root of a binary search tree (BST) where each node has an integer value, write a C++ function that returns a new BST that is a "right-leaning" or "increasing" BST — meaning the original tree is reorganized so that every node's left child is null and its right child points to the next node in an in-order traversal. The new tree should preserve the in-order sequence of the original values, and it must be built as new nodes (do not modify the original tree). The function should return the root of the new tree, whose left child is always null, and whose right subtree forms a chain (each node has at most one right child, no left children). Assume the input tree is a valid BST (left subtree < node < right subtree) and may be empty (return nullptr for empty input). The function signature must be: `TreeNode* increasingBST(TreeNode* root)`.
The solution uses a standard in-order traversal of the BST, which visits nodes in ascending order. We maintain a dummy node to serve as a starting point for the new right-leaning chain, along with a tail pointer that always points to the last node added to the chain. During the recursive in-order traversal, for each visited node (in ascending order), we create a new TreeNode with that value and attach it as the right child of the current tail, then advance the tail to the newly created node. This builds the new chain without modifying the original tree. Key edge cases: an empty tree (root == nullptr) should return nullptr; a single-node tree returns a single new node; duplicate values are allowed and appear consecutively in the chain. The algorithm performs exactly one in-order traversal, visiting each node once, so time complexity is O(n) where n is the number of nodes. Auxiliary space is O(n) due to the new nodes created (which are part of the output) plus O(h) for the recursion stack, where h is the tree height (worst-case O(n) for a skewed tree, O(log n) for a balanced tree).
#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper: recursive in-order traversal that appends new nodes to the tail of the increasing chain.
void buildIncreasing(Node* root, TreeNode*& tail) {
    if (!root) return;
    buildIncreasing(root->left, tail);
    tail->right = new TreeNode(root->val);
    tail = tail->right;
    buildIncreasing(root->right, tail);
}

// Return the root of a new right-leaning BST preserving in-order order.
// Does not modify the original tree. Returns nullptr for empty input.
TreeNode* increasingBST(TreeNode* root) {
    if (!root) return nullptr;
    TreeNode dummy(0); // sentinel node, not part of result
    TreeNode* tail = &dummy;
    buildIncreasing(root, tail);
    return dummy.right;
}
#include <cassert>

// Helper to create a small test tree (not needed for test checks, but we'll build manually)
TreeNode* newNode(int val) {
    return new TreeNode(val);
}

// Helper to verify the increasing BST is a valid right-leaning chain with correct order.
bool verifyChain(TreeNode* root, const std::vector<int>& expected) {
    std::vector<int> actual;
    TreeNode* cur = root;
    while (cur) {
        if (cur->left) return false; // left must always be null
        actual.push_back(cur->val);
        cur = cur->right;
    }
    return actual == expected;
}

int main() {
    // Test 1: empty tree
    assert(increasingBST(nullptr) == nullptr);

    // Test 2: single node
    TreeNode* single = newNode(5);
    TreeNode* res = increasingBST(single);
    assert(verifyChain(res, {5}));
    delete single; // clean up original (not the new chain)
    // (No need to delete res in tests, but for clarity we skip.)

    // Test 3: simple BST:      2
    //                         / \
    //                        1   3
    TreeNode* t3 = newNode(2);
    t3->left = newNode(1);
    t3->right = newNode(3);
    TreeNode* res3 = increasingBST(t3);
    assert(verifyChain(res3, {1, 2, 3}));
    delete t3->left; delete t3->right; delete t3;

    // Test 4: skewed left tree: 3
    //                           /
    //                          2
    //                         /
    //                        1
    TreeNode* t4 = newNode(3);
    t4->left = newNode(2);
    t4->left->left = newNode(1);
    TreeNode* res4 = increasingBST(t4);
    assert(verifyChain(res4, {1, 2, 3}));
    delete t4->left->left; delete t4->left; delete t4;

    // Test 5: skewed right tree: 1
    //                             \
    //                              2
    //                               \
    //                                3
    TreeNode* t5 = newNode(1);
    t5->right = newNode(2);
    t5->right->right = newNode(3);
    TreeNode* res5 = increasingBST(t5);
    assert(verifyChain(res5, {1, 2, 3}));
    delete t5->right->right; delete t5->right; delete t5;

    // Test 6: duplicate values: 2
    //                          / \
    //                         2   3
    TreeNode* t6 = newNode(2);
    t6->left = newNode(2);
    t6->right = newNode(3);
    TreeNode* res6 = increasingBST(t6);
    assert(verifyChain(res6, {2, 2, 3}));
    delete t6->left; delete t6->right; delete t6;

    // Test 7: Large-ish tree:      4
    //                            /   \
    //                           2     6
    //                          / \   / \
    //                         1   3 5   7
    TreeNode* t7 = newNode(4);
    t7->left = newNode(2);
    t7->left->left = newNode(1);
    t7->left->right = newNode(3);
    t7->right = newNode(6);
    t7->right->left = newNode(5);
    t7->right->right = newNode(7);
    TreeNode* res7 = increasingBST(t7);
    assert(verifyChain(res7, {1, 2, 3, 4, 5, 6, 7}));
    // Clean up original tree nodes (not the new chain)
    delete t7->left->left; delete t7->left->right; delete t7->left;
    delete t7->right->left; delete t7->right->right; delete t7->right;
    delete t7;

    return 0;
}
