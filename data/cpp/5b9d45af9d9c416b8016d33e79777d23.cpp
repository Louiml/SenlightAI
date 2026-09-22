Given two binary trees represented by `TreeNode` structures, write a C++ function `bool isSameTree(TreeNode* p, TreeNode* q)` that returns `true` if the trees are structurally identical (i.e., same shape and same node values at corresponding positions), and `false` otherwise. The function must handle `nullptr` roots, empty trees, missing left/right children, and negative values without relying on sentinel values. The solution should use a level-order (BFS) traversal for both trees simultaneously to detect mismatches early, avoiding the need for separate vector storage.
// The most straightforward approach is to perform a synchronous BFS on both trees using two queues. We start by pushing both root pointers onto their respective queues. At each step, we pop the front of both queues. If both are `nullptr`, we continue; if exactly one is `nullptr`, the trees differ and we return `false`. If both are non-null, we first compare their values; if they differ, return `false`. Then we push the left children of both nodes (which may be `nullptr`) onto the respective queues, followed by the right children. This ensures that we compare the structure level by level, including missing children, because the order of pushes preserves the shape of each tree. We also check `if (p == nullptr || q == nullptr) return false` explicitly only when one of them is null at the beginning. Edge cases include: both trees empty (return `true`), one empty (return `false`), trees with same values but different shapes (e.g., a left child vs. a right child), and trees with negative values (no special handling needed). Time complexity is O(n) where n is the number of nodes in the smaller tree, because we stop as soon as a mismatch is found. Space complexity is O(w) where w is the maximum width of the trees (number of nodes at the widest level), due to the queue usage.
#include <queue>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Checks whether two binary trees are structurally identical and have equal values.
bool isSameTree(TreeNode* p, TreeNode* q) {
    std::queue<TreeNode*> q1, q2;
    q1.push(p);
    q2.push(q);

    while (!q1.empty() && !q2.empty()) {
        TreeNode* node1 = q1.front(); q1.pop();
        TreeNode* node2 = q2.front(); q2.pop();

        // If both are null, they match at this position.
        if (node1 == nullptr && node2 == nullptr) {
            continue;
        }

        // If one is null and the other isn't, trees differ.
        if (node1 == nullptr || node2 == nullptr) {
            return false;
        }

        // Values must be equal.
        if (node1->val != node2->val) {
            return false;
        }

        // Push children in fixed order (left then right) to preserve shape.
        q1.push(node1->left);
        q1.push(node1->right);
        q2.push(node2->left);
        q2.push(node2->right);
    }

    // If one queue is empty and the other is not, sizes differ.
    return q1.empty() && q2.empty();
}
#include <cassert>

int main() {
    // Test 1: Both empty trees.
    assert(isSameTree(nullptr, nullptr) == true);

    // Test 2: One empty, one not.
    TreeNode* tree1 = new TreeNode(1);
    assert(isSameTree(tree1, nullptr) == false);
    assert(isSameTree(nullptr, tree1) == false);

    // Test 3: Both single node, same value.
    TreeNode* tree2 = new TreeNode(1);
    assert(isSameTree(tree1, tree2) == true);

    // Test 4: Single nodes with different values.
    TreeNode* tree3 = new TreeNode(2);
    assert(isSameTree(tree1, tree3) == false);

    // Test 5: Structurally same but different values.
    TreeNode* tree4 = new TreeNode(1);
    tree4->left = new TreeNode(2);
    tree4->right = new TreeNode(3);
    TreeNode* tree5 = new TreeNode(1);
    tree5->left = new TreeNode(2);
    tree5->right = new TreeNode(4);
    assert(isSameTree(tree4, tree5) == false);

    // Test 6: Same values but different shapes (left vs. right child).
    TreeNode* tree6 = new TreeNode(1);
    tree6->left = new TreeNode(2);
    TreeNode* tree7 = new TreeNode(1);
    tree7->right = new TreeNode(2);
    assert(isSameTree(tree6, tree7) == false);

    // Test 7: Negative values and deeper structure.
    TreeNode* tree8 = new TreeNode(-1);
    tree8->left = new TreeNode(-2);
    tree8->right = new TreeNode(-3);
    tree8->left->right = new TreeNode(-4);
    TreeNode* tree9 = new TreeNode(-1);
    tree9->left = new TreeNode(-2);
    tree9->right = new TreeNode(-3);
    tree9->left->right = new TreeNode(-4);
    assert(isSameTree(tree8, tree9) == true);

    // Test 8: Missing right child at deeper level.
    TreeNode* tree10 = new TreeNode(1);
    tree10->left = new TreeNode(2);
    tree10->left->left = new TreeNode(3);
    TreeNode* tree11 = new TreeNode(1);
    tree11->left = new TreeNode(2);
    tree11->left->right = new TreeNode(3);
    assert(isSameTree(tree10, tree11) == false);

    // Clean up manually (for simplicity in a small test).
    // In practice, use smart pointers or a tree destructor.
    delete tree1;
    delete tree2;
    delete tree3;
    delete tree4;
    delete tree5;
    delete tree6;
    delete tree7;
    delete tree8;
    delete tree9;
    delete tree10;
    delete tree11;
}
