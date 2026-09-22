// Write a C++ function `int sumOfDeepestLeaves(TreeNode* root)` that, given a binary tree, returns the sum of the values of all leaves at the greatest depth. The tree is not necessarily balanced, may be empty, and may contain any integer values (positive, negative, or zero). A leaf is defined as a node with both left and right children equal to `nullptr`. For an empty tree, return 0. For a tree with only a root, return the root’s value. You may assume the tree node structure is already defined as in the provided snippet; do not include that definition in your solution—only provide the free function.
// The solution requires two passes over the tree. First, compute the maximum depth (height) of the tree using a recursive depth-first traversal. In this pass, track the current depth from the root (root is at depth 0). Update the maximum depth whenever a deeper node is found. Since we recurse into both children, we visit every node exactly once, ensuring the maximum depth is correct. Second, perform another recursive traversal to sum the values of all leaves whose depth equals the previously computed maximum depth. In this pass, if a node is a leaf (both children null) and its current depth equals the maximum depth, add its value to the answer. Continue recursion into children if they exist. Edge cases: empty tree returns 0 without recursion; a single-node tree returns its value directly as both the only leaf and the maximum depth is 0. Both traversals run in O(n) time, where n is the number of nodes, and use O(h) auxiliary space for the recursion stack, where h is the height of the tree (worst-case O(n) for a skewed tree, O(log n) for a balanced tree). The algorithm is straightforward and does not require any additional data structures.
#include <algorithm>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper to compute the maximum depth of the tree.
void computeMaxDepth(const TreeNode* node, int currentDepth, int& maxDepth) {
    if (node == nullptr) {
        return;
    }
    maxDepth = std::max(maxDepth, currentDepth);
    computeMaxDepth(node->left, currentDepth + 1, maxDepth);
    computeMaxDepth(node->right, currentDepth + 1, maxDepth);
}

// Helper to sum leaf values at the given target depth.
void sumLeavesAtDepth(const TreeNode* node, int currentDepth, int targetDepth, int& sum) {
    if (node == nullptr) {
        return;
    }
    if (node->left == nullptr && node->right == nullptr) {
        if (currentDepth == targetDepth) {
            sum += node->val;
        }
        return;
    }
    sumLeavesAtDepth(node->left, currentDepth + 1, targetDepth, sum);
    sumLeavesAtDepth(node->right, currentDepth + 1, targetDepth, sum);
}

// Returns the sum of all leaves at the greatest depth.
int sumOfDeepestLeaves(const TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    int maxDepth = 0;
    computeMaxDepth(root, 0, maxDepth);
    int result = 0;
    sumLeavesAtDepth(root, 0, maxDepth, result);
    return result;
}
#include <cassert>

int main() {
    // Test 1: Empty tree
    assert(sumOfDeepestLeaves(nullptr) == 0);

    // Test 2: Single-node tree
    TreeNode n1(5);
    assert(sumOfDeepestLeaves(&n1) == 5);

    // Test 3: Simple two-level tree: root(1) with left(2) and right(3)
    TreeNode n3(3);
    TreeNode n2(2);
    TreeNode n1v2(1, &n2, &n3);
    assert(sumOfDeepestLeaves(&n1v2) == 5); // 2+3=5

    // Test 4: Deeper leaves on one side: root(10) -> left(1) -> left(2), root->right(3)
    TreeNode leaf2(2);
    TreeNode leftChild(1, &leaf2, nullptr);
    TreeNode rightChild(3);
    TreeNode root(10, &leftChild, &rightChild);
    // Deepest leaves are 2 and 3 (depth 2), sum = 5
    assert(sumOfDeepestLeaves(&root) == 5);

    // Test 5: All leaves at different depths: root(1) with left tree of depth 2 and right leaf
    TreeNode leafDeep1(11);
    TreeNode leafDeep2(12);
    TreeNode mid(5, &leafDeep1, &leafDeep2);
    TreeNode rightLeaf(7);
    TreeNode root2(1, &mid, &rightLeaf);
    // Deepest leaves are 11 and 12 (depth 2), sum = 23
    assert(sumOfDeepestLeaves(&root2) == 23);

    // Test 6: Negative values
    TreeNode negLeaf(-4);
    TreeNode posLeaf(6);
    TreeNode negRoot(0, &negLeaf, &posLeaf);
    // Both leaves depth 1, sum = 2
    assert(sumOfDeepestLeaves(&negRoot) == 2);

    // Test 7: Skewed tree: root -> right -> right
    TreeNode deep1(1);
    TreeNode deep2(2, nullptr, &deep1);
    TreeNode deep3(3, nullptr, &deep2);
    // Only one leaf at depth 2, sum = 1
    assert(sumOfDeepestLeaves(&deep3) == 1);
}
