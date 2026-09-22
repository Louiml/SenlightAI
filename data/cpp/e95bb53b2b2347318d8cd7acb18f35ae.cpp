// Write a C++ function `reorderToRightOnly(TreeNode* root)` that takes the root of a binary search tree (BST) and returns a new tree pointer where the tree is rearranged into a "right-skewed" tree: each node has at most one child, and that child is always the `right` child. The resulting tree must preserve the original BST's in-order traversal order (i.e., a left-to-right inorder traversal of the original tree becomes the top-to-bottom order of the right-skewed chain). The `left` pointers of all nodes in the result must be `nullptr`. You may modify the original tree nodes in place (no need to allocate new nodes). The function must handle an empty tree (return `nullptr`) and a tree with only a single node (return the same node). Your implementation must be recursive and must not use any auxiliary containers like stacks or queues. The `TreeNode` struct is defined as: `struct TreeNode { int val; TreeNode *left; TreeNode *right; TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} };`
The core idea is to recursively process the left subtree first (since inorder order requires left nodes before the root), then attach the root, then the right subtree. For a given node, the returned "head" of the reordered subtree is the leftmost node in that subtree (because in the right-skewed chain, the first node encountered in inorder traversal is the leftmost). The algorithm works as follows:  
- If the node is `nullptr`, return `nullptr`.  
- Recursively reorder the left subtree. If the left subtree is non-empty, its returned head becomes the new head of the whole subtree. We then find the rightmost node of that reordered left chain (which will be the last node in inorder order of the left subtree) and set its `right` pointer to the current node. The current node's `left` is set to `nullptr`.  
- Recursively reorder the right subtree (if any) and set the current node's `right` to that result. The current node's `left` is already `nullptr` after the left-handling step.  
- Finally, return the head of the whole reordered subtree (which is the head from the left subtree if it exists, otherwise the current node).  
Edge cases: empty tree, single-node tree, left subtree missing, right subtree missing, and a completely left-skewed original tree. In every case, the recursion ensures that after processing, all left pointers are null and the right chain follows inorder order.  
Time complexity is O(n) because each node is visited once (plus the traversal to find the rightmost node of the left chain, which in total across all recursions is also O(n) because each edge is traversed once). Space complexity is O(h) for the recursion stack, where h is the tree height (worst-case O(n) for a skewed tree).
#include <cstddef> // for nullptr

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Reorders a BST in-place into a right-skewed tree preserving inorder order.
// Returns the new root (leftmost node in original inorder).
TreeNode* reorderToRightOnly(TreeNode* root) {
    if (root == nullptr) return nullptr;

    // Recursively reorder the left subtree first (if any).
    TreeNode* leftHead = nullptr;
    if (root->left != nullptr) {
        leftHead = reorderToRightOnly(root->left);
        // Find the rightmost node in the reordered left chain.
        TreeNode* leftTail = leftHead;
        while (leftTail->right != nullptr) {
            leftTail = leftTail->right;
        }
        // Attach the current root to the end of the left chain.
        leftTail->right = root;
        root->left = nullptr;
    }

    // Recursively reorder the right subtree and attach it.
    if (root->right != nullptr) {
        root->right = reorderToRightOnly(root->right);
    }

    // The new head is the leftmost node (the head of the left chain if it exists).
    return (leftHead != nullptr) ? leftHead : root;
}
#include <cassert>

// Helper to build a BST from a simple array (for testing only).
TreeNode* buildBST(const std::vector<int>& vals, int& idx, int min, int max) {
    if (idx >= (int)vals.size()) return nullptr;
    int val = vals[idx];
    if (val < min || val > max) return nullptr;
    idx++;
    TreeNode* node = new TreeNode(val);
    node->left = buildBST(vals, idx, min, val - 1);
    node->right = buildBST(vals, idx, val + 1, max);
    return node;
}

// Helper to check if a tree is right-skewed and inorder matches expected vector.
void checkResult(TreeNode* root, const std::vector<int>& expected) {
    std::vector<int> actual;
    while (root != nullptr) {
        assert(root->left == nullptr);
        actual.push_back(root->val);
        root = root->right;
    }
    assert(actual == expected);
}

int main() {
    // Test 1: Empty tree
    assert(reorderToRightOnly(nullptr) == nullptr);

    // Test 2: Single node
    TreeNode* single = new TreeNode(5);
    TreeNode* res = reorderToRightOnly(single);
    assert(res == single);
    assert(res->left == nullptr && res->right == nullptr);

    // Test 3: Simple tree: 2 (left:1, right:3) -> inorder 1,2,3
    TreeNode* n2 = new TreeNode(2);
    n2->left = new TreeNode(1);
    n2->right = new TreeNode(3);
    res = reorderToRightOnly(n2);
    checkResult(res, {1,2,3});

    // Test 4: Left-skewed tree: 3->2->1 (all left children) -> inorder 1,2,3
    TreeNode* n3 = new TreeNode(3);
    n3->left = new TreeNode(2);
    n3->left->left = new TreeNode(1);
    res = reorderToRightOnly(n3);
    checkResult(res, {1,2,3});

    // Test 5: Right-skewed tree: 1->2->3 (all right children) -> inorder 1,2,3
    TreeNode* r1 = new TreeNode(1);
    r1->right = new TreeNode(2);
    r1->right->right = new TreeNode(3);
    res = reorderToRightOnly(r1);
    checkResult(res, {1,2,3});

    // Test 6: Complex tree built from known BST array
    std::vector<int> vals = {4,2,1,3,6,5,7};
    int idx = 0;
    TreeNode* complex = buildBST(vals, idx, INT_MIN, INT_MAX);
    res = reorderToRightOnly(complex);
    checkResult(res, {1,2,3,4,5,6,7});

    return 0;
}
