// Write a C++ function that, given the root of a binary tree where each node stores an integer value (assume tree values fit within `int`), returns the number of subtrees (including the entire tree itself) for which the average value of that subtree’s nodes is equal to the value stored at the subtree’s root. The average is computed using integer division (truncating toward zero), and the function must handle empty trees (returning 0) and trees with possibly negative values. Use a simple `TreeNode` structure with fields `val`, `left`, and `right`. The function should be self-contained (no global state) and not modify the tree.

// The core idea is to perform a post-order traversal (left, right, root) to compute for each subtree two pieces of information: the sum of node values in that subtree and the number of nodes in that subtree. For a given node, after obtaining the sum and count for its left and right children, combine them with the node's own value to get the subtree's total sum and count. Then compute `sum / count` using integer division (which truncates toward zero in C++). Compare this result to the node's value; if equal, increment a counter. The traversal returns a pair `{sum, count}` for each subtree, which is used by the parent. Edge cases include: the root being `nullptr` (return 0), leaf nodes (sum/count = value/1 always matches, so every leaf counts), negative values (integer division truncation works correctly for negative numbers, e.g., `-7/2 = -3`), and large sums that could overflow `int` in pathological trees—so use `long long` for sums internally to be safe. Time complexity is O(N) where N is the number of nodes, because each node is visited exactly once. Space complexity is O(H) for the recursion stack, where H is the tree height (O(N) in worst-case skewed tree, O(log N) for balanced).

#include <utility> // for std::pair

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper function that returns {sum, count} for the subtree rooted at node.
// Also increments the result counter when a subtree's average matches its root value.
static std::pair<long long, int> averageSubtreeHelper(TreeNode* node, int& result) {
    if (node == nullptr) {
        return {0, 0};
    }

    auto left = averageSubtreeHelper(node->left, result);
    auto right = averageSubtreeHelper(node->right, result);

    long long totalSum = left.first + right.first + node->val;
    int totalCount = left.second + right.second + 1;

    if (totalSum / totalCount == node->val) {
        ++result;
    }

    return {totalSum, totalCount};
}

// Returns the number of subtrees whose integer average equals the subtree root's value.
int countSubtreesWithAverageEqualRoot(TreeNode* root) {
    int result = 0;
    averageSubtreeHelper(root, result);
    return result;
}

#include <cassert>

int main() {
    // Test 1: Empty tree
    assert(countSubtreesWithAverageEqualRoot(nullptr) == 0);

    // Test 2: Single node
    TreeNode* one = new TreeNode(5);
    assert(countSubtreesWithAverageEqualRoot(one) == 1); // leaf always matches

    // Test 3: Left child only, average matches root? Tree: 2 -> left 2
    TreeNode* root3 = new TreeNode(2);
    root3->left = new TreeNode(2);
    // Subtrees: leaf (2) matches, root avg = (2+2)/2 = 2 matches -> total 2
    assert(countSubtreesWithAverageEqualRoot(root3) == 2);

    // Test 4: Right child only, negative values. Tree: -3 -> right -3
    TreeNode* root4 = new TreeNode(-3);
    root4->right = new TreeNode(-3);
    // leaf matches, root avg = (-6)/2 = -3 matches -> total 2
    assert(countSubtreesWithAverageEqualRoot(root4) == 2);

    // Test 5: More complex tree: root=1, left=1, right=1, left's right=2
    TreeNode* root5 = new TreeNode(1);
    root5->left = new TreeNode(1);
    root5->right = new TreeNode(1);
    root5->left->right = new TreeNode(2);
    // Subtrees:
    // leaf 2: avg=2 matches -> +1
    // leaf 1 (left's left? none, but left node has left=null, right=2) -> subtree of left node: sum=1+2=3, count=2, avg=1 (3/2=1) matches -> +1
    // leaf 1 (right) -> +1
    // whole root: sum=1+1+1+2=5, count=4, avg=1 (5/4=1) matches -> +1
    // total = 4
    assert(countSubtreesWithAverageEqualRoot(root5) == 4);

    // Test 6: Tree where root does not match but children do: root=3, left=2, left's left=2
    TreeNode* root6 = new TreeNode(3);
    root6->left = new TreeNode(2);
    root6->left->left = new TreeNode(2);
    // leaf 2 matches (+1)
    // subtree left: sum=4, count=2, avg=2 matches (+1)
    // root: sum=7, count=3, avg=2 (7/3=2) does NOT equal 3 -> no
    // total = 2
    assert(countSubtreesWithAverageEqualRoot(root6) == 2);

    // Test 7: Negative and positive mixed, root does not match
    TreeNode* root7 = new TreeNode(-1);
    root7->left = new TreeNode(3);
    root7->right = new TreeNode(-3);
    // leaf 3 matches (+1), leaf -3 matches (+1)
    // root: sum=-1, count=3, avg=-1 (truncates toward zero, -1/3 = 0) does NOT equal -1 -> no
    // total = 2
    assert(countSubtreesWithAverageEqualRoot(root7) == 2);

    // Test 8: Root only counts when avg matches. Tree: root=0, left=0, right=0
    TreeNode* root8 = new TreeNode(0);
    root8->left = new TreeNode(0);
    root8->right = new TreeNode(0);
    // all three subtrees have avg 0, so 3 matches
    assert(countSubtreesWithAverageEqualRoot(root8) == 3);

    // Clean up (not strictly necessary for asserts but good practice)
    // Since this is test code, we omit full deletion for brevity.

    return 0;
}
