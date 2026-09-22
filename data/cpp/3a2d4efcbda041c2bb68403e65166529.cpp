Given a binary tree `original` and a cloned copy of that tree `cloned`, and a reference to a target node in the original tree, write a C++ function `findCorrespondingNode` that returns a pointer to the corresponding node in the cloned tree. The cloned tree is an exact deep copy of the original, meaning each node's structure and values match but the node pointers are different. The target is guaranteed to exist in the original tree. The function should not modify either tree and should work for any binary tree, including those with only one node or skewed trees. Trees are represented using the provided `TreeNode` struct with `int val`, `TreeNode* left`, and `TreeNode* right`. You may assume the target pointer is valid and non-null. The function should be `const`-safe where appropriate (i.e., not modifying the trees). Return the pointer to the corresponding node in the cloned tree.
The solution uses a depth-first search (DFS) that traverses both trees in lockstep. Since the cloned tree mirrors the original exactly, we compare node pointers in the original tree to the target pointer. When we find the node in the original tree whose address equals the target pointer, we return the corresponding node from the cloned tree at the same traversal position. The recursion simultaneously descends into left and right children of both trees. Base case: if the current original node is null, return nullptr (or just return without updating the result). If `original == target`, we have found the match and return the cloned node. Because the target is guaranteed to exist, the recursion will eventually find it. Edge cases include a single-node tree (target is root, return cloned root immediately) and skewed trees (e.g., all left children). The time complexity is O(n) in the worst case because we may need to visit every node if the target is deep in the tree. Auxiliary space is O(h) where h is the height of the tree, due to recursion stack. For a balanced tree, O(log n); for skewed, O(n). No extra data structures are needed.
#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// Finds the node in the cloned tree that corresponds to the given target node in the original tree.
// The original and cloned trees are structurally identical. The function does not modify either tree.
const TreeNode* findCorrespondingNode(const TreeNode* original, const TreeNode* cloned, const TreeNode* target) {
    // If we have reached a null node in either tree, no match here.
    if (original == nullptr || cloned == nullptr) {
        return nullptr;
    }
    // If the current original node is the target, return the corresponding cloned node.
    if (original == target) {
        return cloned;
    }
    // Search left subtree first, then right subtree.
    const TreeNode* leftResult = findCorrespondingNode(original->left, cloned->left, target);
    if (leftResult != nullptr) {
        return leftResult;
    }
    return findCorrespondingNode(original->right, cloned->right, target);
}
#include <cassert>

int main() {
    // Test 1: Single-node tree.
    TreeNode* orig1 = new TreeNode(1);
    TreeNode* clone1 = new TreeNode(1);
    const TreeNode* result = findCorrespondingNode(orig1, clone1, orig1);
    assert(result == clone1);
    delete orig1;
    delete clone1;

    // Test 2: Simple two-level tree, target is left child.
    TreeNode* orig2 = new TreeNode(2);
    orig2->left = new TreeNode(1);
    orig2->right = new TreeNode(3);
    TreeNode* clone2 = new TreeNode(2);
    clone2->left = new TreeNode(1);
    clone2->right = new TreeNode(3);
    const TreeNode* result2 = findCorrespondingNode(orig2, clone2, orig2->left);
    assert(result2 == clone2->left);
    delete orig2->left; delete orig2->right; delete orig2;
    delete clone2->left; delete clone2->right; delete clone2;

    // Test 3: Target is right child in a larger tree.
    TreeNode* orig3 = new TreeNode(5);
    orig3->left = new TreeNode(3);
    orig3->right = new TreeNode(8);
    orig3->left->left = new TreeNode(2);
    orig3->left->right = new TreeNode(4);
    orig3->right->left = new TreeNode(7);
    orig3->right->right = new TreeNode(9);
    TreeNode* clone3 = new TreeNode(5);
    clone3->left = new TreeNode(3);
    clone3->right = new TreeNode(8);
    clone3->left->left = new TreeNode(2);
    clone3->left->right = new TreeNode(4);
    clone3->right->left = new TreeNode(7);
    clone3->right->right = new TreeNode(9);
    const TreeNode* result3 = findCorrespondingNode(orig3, clone3, orig3->right->right);
    assert(result3 == clone3->right->right);
    // Clean up: delete all nodes (simplified, actual deletion would use post-order)
    // For brevity, we skip full cleanup in this test code.

    // Test 4: Skewed tree to the left, target is deepest node.
    TreeNode* orig4 = new TreeNode(1);
    orig4->left = new TreeNode(2);
    orig4->left->left = new TreeNode(3);
    TreeNode* clone4 = new TreeNode(1);
    clone4->left = new TreeNode(2);
    clone4->left->left = new TreeNode(3);
    const TreeNode* result4 = findCorrespondingNode(orig4, clone4, orig4->left->left);
    assert(result4 == clone4->left->left);

    // Test 5: Target is root.
    TreeNode* orig5 = new TreeNode(42);
    orig5->left = new TreeNode(10);
    orig5->right = new TreeNode(20);
    TreeNode* clone5 = new TreeNode(42);
    clone5->left = new TreeNode(10);
    clone5->right = new TreeNode(20);
    const TreeNode* result5 = findCorrespondingNode(orig5, clone5, orig5);
    assert(result5 == clone5);

    return 0;
}
