// Write a C++ function `isCompleteBinaryTree(TreeNode* root)` that takes the root of a binary tree and returns `true` if the tree is a complete binary tree, otherwise `false`. A complete binary tree has every level completely filled except possibly the last level, and in the last level, all nodes are as far left as possible. The tree may be empty (root is `nullptr`), which should be considered complete. Use the provided `TreeNode` structure with `val`, `left`, and `right` pointers. The function must be efficient and handle any valid binary tree shape.

The solution uses a level-order traversal (BFS) with a queue. The key idea is to detect the first `nullptr` node in the traversal. Once a `nullptr` is encountered, no subsequent non-null node can appear in the traversal order; otherwise, the tree is not complete. We start by pushing the root into the queue. While the queue is not empty, we pop the front node. If it is `nullptr`, we set a flag indicating that we have seen a hole. If it is non-null and the hole flag is already set, we immediately return `false`. Otherwise, we push its left and right children (even if they are null) to preserve the level-order sequence. This approach works because in a complete tree, after the first missing node, all remaining nodes in the level-order must also be missing. Edge cases: an empty tree returns `true`; a tree with only a root returns `true`; a tree with a missing left child but present right child returns `false` because the right child appears before the null left child in level-order. Time complexity is O(N) since each node is visited once. Space complexity is O(N) in the worst case (e.g., a full last level) due to the queue storing nodes.

#include <queue>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Check whether the binary tree is complete using level-order traversal.
bool isCompleteBinaryTree(TreeNode* root) {
    if (root == nullptr) {
        return true;
    }
    
    std::queue<TreeNode*> q;
    q.push(root);
    bool seenNull = false;
    
    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();
        
        if (current == nullptr) {
            seenNull = true;
        } else {
            if (seenNull) {
                return false;
            }
            q.push(current->left);
            q.push(current->right);
        }
    }
    return true;
}

#include <cassert>

int main() {
    // Test 1: Empty tree
    TreeNode* empty = nullptr;
    assert(isCompleteBinaryTree(empty) == true);

    // Test 2: Single node
    TreeNode* single = new TreeNode(1);
    assert(isCompleteBinaryTree(single) == true);

    // Test 3: Complete tree (full last level)
    //       1
    //      / \
    //     2   3
    TreeNode* full = new TreeNode(1);
    full->left = new TreeNode(2);
    full->right = new TreeNode(3);
    assert(isCompleteBinaryTree(full) == true);

    // Test 4: Complete tree (partial last level, as far left as possible)
    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5
    TreeNode* complete = new TreeNode(1);
    complete->left = new TreeNode(2);
    complete->right = new TreeNode(3);
    complete->left->left = new TreeNode(4);
    complete->left->right = new TreeNode(5);
    assert(isCompleteBinaryTree(complete) == true);

    // Test 5: Not complete - missing left child of a node, but right exists
    //       1
    //      / \
    //     2   3
    //    /
    //   4
    // But here we make 3 have a right child only
    TreeNode* notComplete1 = new TreeNode(1);
    notComplete1->left = new TreeNode(2);
    notComplete1->right = new TreeNode(3);
    notComplete1->left->left = new TreeNode(4);
    notComplete1->right->right = new TreeNode(5); // This breaks completeness
    assert(isCompleteBinaryTree(notComplete1) == false);

    // Test 6: Not complete - hole in the middle (missing left child of a node)
    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5
    // But 3 has a left child and no right child? Actually this is complete.
    // Instead, let's make node 3 have only a right child:
    TreeNode* notComplete2 = new TreeNode(1);
    notComplete2->left = new TreeNode(2);
    notComplete2->right = new TreeNode(3);
    notComplete2->left->left = new TreeNode(4);
    notComplete2->left->right = new TreeNode(5);
    notComplete2->right->right = new TreeNode(6); // Right child without left child -> not complete
    assert(isCompleteBinaryTree(notComplete2) == false);

    // Test 7: Complete tree with two levels, last level partially filled
    //       1
    //      / \
    //     2   3
    //    /
    //   4
    TreeNode* complete2 = new TreeNode(1);
    complete2->left = new TreeNode(2);
    complete2->right = new TreeNode(3);
    complete2->left->left = new TreeNode(4);
    assert(isCompleteBinaryTree(complete2) == true);

    // Clean up (optional in tests, but for completeness we could delete nodes)
    return 0;
}
