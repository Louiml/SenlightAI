Write a C++ function that searches a binary search tree (BST) for a node with a given value. The function should take as input a pointer to the root node of the BST and an integer `val`. It must return a pointer to the node whose value equals `val`, or `nullptr` if no such node exists. The BST is defined by the `TreeNode` structure with `int val`, `TreeNode *left`, and `TreeNode *right`. The function should be recursive and must not rely on the BST property for pruning (i.e., it should search both left and right subtrees in a general binary-tree fashion), but the input is guaranteed to be a valid BST. Handle empty trees (root = nullptr) and values not present in the tree correctly.

// The main algorithm is a recursive depth-first search over the tree. Starting at the root, check if the current node is `nullptr` — if so, return `nullptr` immediately. If the current node's value equals the target, return that node. Otherwise, recursively search the left subtree first; if that returns a non-null pointer, propagate it upward. If the left subtree search fails (returns `nullptr`), then search the right subtree and return its result regardless of whether it is null or not. This approach works for any binary tree, not just BSTs, and does not use the ordering property; it simply explores all nodes until a match is found. Edge cases include: empty tree (root = nullptr → return nullptr), single-node tree with matching value (return root), single-node tree with non-matching value (return nullptr after checking both children which are null). Time complexity is O(n) in the worst case (when the tree is skewed and the target is absent or at the deepest leaf), where n is the number of nodes. Space complexity is O(h) due to recursion stack, where h is the tree height (O(n) for skewed trees, O(log n) for balanced trees).

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

// Recursively search for a node with the given value in the binary search tree.
// Returns a pointer to the node if found, otherwise nullptr.
TreeNode* searchInBST(TreeNode* root, int val) {
    if (root == nullptr) {
        return nullptr;
    }
    if (root->val == val) {
        return root;
    }
    TreeNode* leftResult = searchInBST(root->left, val);
    if (leftResult != nullptr) {
        return leftResult;
    }
    return searchInBST(root->right, val);
}

#include <cassert>

int main() {
    // Test 1: Empty tree
    assert(searchInBST(nullptr, 5) == nullptr);

    // Test 2: Single node, value present
    TreeNode n5(5);
    assert(searchInBST(&n5, 5) == &n5);

    // Test 3: Single node, value absent
    assert(searchInBST(&n5, 3) == nullptr);

    // Test 4: Tree with multiple nodes, target in left subtree
    TreeNode n2(2);
    TreeNode n7(7);
    TreeNode n4(4, &n2, &n7);
    TreeNode n13(13);
    TreeNode n15(15, nullptr, &n13);
    TreeNode n10(10, &n4, &n15);
    assert(searchInBST(&n10, 7) == &n7);

    // Test 5: Target in right subtree
    assert(searchInBST(&n10, 15) == &n15);

    // Test 6: Target is root
    assert(searchInBST(&n10, 10) == &n10);

    // Test 7: Target absent
    assert(searchInBST(&n10, 99) == nullptr);

    // Test 8: Target in deep left leaf
    TreeNode n1(1);
    TreeNode n3(3, &n1, nullptr);
    TreeNode n6(6);
    TreeNode n8(8);
    TreeNode n7b(7, &n6, &n8);
    TreeNode n5b(5, &n3, &n7b);
    assert(searchInBST(&n5b, 1) == &n1);

    // Test 9: Skewed tree, target at leaf
    TreeNode a(1);
    TreeNode b(2, &a, nullptr);
    TreeNode c(3, &b, nullptr);
    TreeNode d(4, &c, nullptr);
    assert(searchInBST(&d, 1) == &a);
    assert(searchInBST(&d, 5) == nullptr);
}
