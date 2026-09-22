/*
Given a binary tree whose nodes contain integer values, write a C++ function that counts the number of “good” nodes. A node is considered good if its value is greater than or equal to the maximum value seen along the path from the root to that node (including the node itself). The root is always good. The function should return the total number of good nodes in the tree. Handle empty trees (return 0) and trees with negative or duplicate values correctly.
*/

#include <climits>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Count the number of good nodes in a binary tree.
// A node is good if its value >= maximum value on the path from root to it.
int countGoodNodes(const TreeNode* root) {
    int goodCount = 0;
    // Recursive helper that traverses the tree and counts good nodes.
    // Uses a reference to goodCount so all recursive calls update the same counter.
    const auto dfs = [&](const TreeNode* node, int maxSoFar) -> void {
        if (node == nullptr) {
            return;
        }
        if (node->val >= maxSoFar) {
            ++goodCount;
        }
        const int newMax = (node->val > maxSoFar) ? node->val : maxSoFar;
        dfs(node->left, newMax);
        dfs(node->right, newMax);
    };
    dfs(root, INT_MIN);
    return goodCount;
}

#include <cassert>

int main() {
    // Test 1: Empty tree
    assert(countGoodNodes(nullptr) == 0);

    // Test 2: Single node (root is always good)
    TreeNode* root1 = new TreeNode(5);
    assert(countGoodNodes(root1) == 1);

    // Test 3: Simple tree: 3 (left 1, right 4) -> all nodes good
    TreeNode* root2 = new TreeNode(3);
    root2->left = new TreeNode(1);
    root2->right = new TreeNode(4);
    assert(countGoodNodes(root2) == 3);

    // Test 4: Right-skewed descending: 5 -> 4 -> 3 -> only root good
    TreeNode* root3 = new TreeNode(5);
    root3->right = new TreeNode(4);
    root3->right->right = new TreeNode(3);
    assert(countGoodNodes(root3) == 1);

    // Test 5: Negative values and duplicates: 2 (left -1, right 2) -> all good
    TreeNode* root4 = new TreeNode(2);
    root4->left = new TreeNode(-1);
    root4->right = new TreeNode(2);
    assert(countGoodNodes(root4) == 3);

    // Test 6: Mixed: 3 (left 1 with right 2, right 4 with left 3) -> good nodes: 3,4,3 (1 and 2 not)
    TreeNode* root5 = new TreeNode(3);
    root5->left = new TreeNode(1);
    root5->left->right = new TreeNode(2);
    root5->right = new TreeNode(4);
    root5->right->left = new TreeNode(3);
    assert(countGoodNodes(root5) == 3);

    // Test 7: Large negative root, positive children: -10 (left 0, right 5) -> all good
    TreeNode* root6 = new TreeNode(-10);
    root6->left = new TreeNode(0);
    root6->right = new TreeNode(5);
    assert(countGoodNodes(root6) == 3);

    // Test 8: Complex tree: 1 (left 2 with left 3, right 4), right 0 -> good: 1,2,3,4 (0 not)
    TreeNode* root7 = new TreeNode(1);
    root7->left = new TreeNode(2);
    root7->left->left = new TreeNode(3);
    root7->left->right = new TreeNode(4);
    root7->right = new TreeNode(0);
    assert(countGoodNodes(root7) == 4);

    // Test 9: Left-skewed with equal values: 0 -> 0 -> 0, all good
    TreeNode* root8 = new TreeNode(0);
    root8->left = new TreeNode(0);
    root8->left->left = new TreeNode(0);
    assert(countGoodNodes(root8) == 3);

    // Test 10: Balanced tree: 5 (left 3 with right 4, right 6 with left 7) -> good: 5,6,7 (3,4 not)
    TreeNode* root9 = new TreeNode(5);
    root9->left = new TreeNode(3);
    root9->left->right = new TreeNode(4);
    root9->right = new TreeNode(6);
    root9->right->left = new TreeNode(7);
    assert(countGoodNodes(root9) == 3);

    // Cleanup (optional, not required for correctness in this test snippet)
    // For simplicity, memory is not freed in this testing snippet.
}

// The solution uses a depth-first traversal (preorder). At each node, we track the maximum value encountered so far along the current path, initially set to the smallest possible integer (`INT_MIN`). If the current node’s value is greater than or equal to that running maximum, it is a good node, and we increment a counter. Then we update the running maximum to be the larger of the previous maximum and the current node’s value, and recursively process the left and right subtrees. This approach naturally handles edge cases: an empty tree returns 0; duplicate values count only when they do not exceed the prior maximum; negative values are handled because the initial maximum is `INT_MIN`. The time complexity is O(N) where N is the number of nodes, since each node is visited exactly once. The auxiliary space complexity is O(H) due to the recursion stack, where H is the height of the tree (O(N) in the worst case for a skewed tree, O(log N) for a balanced tree). No additional data structures are needed.
