/*
Write a C++ function `int maxPathSumInBinaryTree(TreeNode* root)` that returns the maximum path sum in a binary tree, where a path is defined as any sequence of nodes from some starting node to any node in the tree along parent-child connections. The path must contain at least one node and does not need to pass through the root. Each node contains an integer value (which may be negative). You must handle empty trees (return 0) and trees with all negative values correctly. Implement the function as a free function, and assume the `TreeNode` structure is defined exactly as in the provided snippet (with `int val`, `TreeNode* left`, `TreeNode* right`, and a constructor `TreeNode(int x) : val(x), left(NULL), right(NULL) {}`). Use the standard library for `INT_MIN` and `max`.
*/

#include <algorithm>
#include <climits>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper function that returns the maximum single-branch path sum starting at node
// and updates the global maximum path sum (which may go through node).
int maxSingleBranch(TreeNode* node, int& globalMax) {
    if (!node) return 0;
    int leftBest = maxSingleBranch(node->left, globalMax);
    int rightBest = maxSingleBranch(node->right, globalMax);
    
    // Best path that goes through this node (can branch into both children)
    int throughHere = node->val + std::max(0, leftBest) + std::max(0, rightBest);
    globalMax = std::max(globalMax, throughHere);
    
    // Best single-branch path starting at this node (goes into at most one child)
    int singleBranch = node->val + std::max(0, std::max(leftBest, rightBest));
    return singleBranch;
}

// Public function that returns the maximum path sum in the binary tree.
int maxPathSumInBinaryTree(TreeNode* root) {
    if (!root) return 0;
    int globalMax = INT_MIN;
    maxSingleBranch(root, globalMax);
    return globalMax;
}

#include <cassert>

int main() {
    // Test 1: empty tree
    assert(maxPathSumInBinaryTree(nullptr) == 0);
    
    // Test 2: single node
    TreeNode* n1 = new TreeNode(5);
    assert(maxPathSumInBinaryTree(n1) == 5);
    delete n1;
    
    // Test 3: all negative values, tree: -10 -> left -20, right -30
    TreeNode* negRoot = new TreeNode(-10);
    negRoot->left = new TreeNode(-20);
    negRoot->right = new TreeNode(-30);
    assert(maxPathSumInBinaryTree(negRoot) == -10); // best is single node
    delete negRoot->left; delete negRoot->right; delete negRoot;
    
    // Test 4: mixed values, tree: 1 -> left 2, right 3
    TreeNode* mixed = new TreeNode(1);
    mixed->left = new TreeNode(2);
    mixed->right = new TreeNode(3);
    assert(maxPathSumInBinaryTree(mixed) == 6); // 2+1+3
    delete mixed->left; delete mixed->right; delete mixed;
    
    // Test 5: complex tree: root -2, left 1, right 3, left's left -1
    TreeNode* complex = new TreeNode(-2);
    complex->left = new TreeNode(1);
    complex->right = new TreeNode(3);
    complex->left->left = new TreeNode(-1);
    // Best path: 1 (left child) alone? Actually 3 (right child) alone =3, or 1+(-2)+3=2, or -1+1=0, so best is 3
    assert(maxPathSumInBinaryTree(complex) == 3);
    delete complex->left->left; delete complex->left; delete complex->right; delete complex;
    
    // Test 6: negative root but positive child branch: root -5, left 10, right -2
    TreeNode* negRootPos = new TreeNode(-5);
    negRootPos->left = new TreeNode(10);
    negRootPos->right = new TreeNode(-2);
    // Best path: 10 alone =10, or 10+(-5)+(-2)=3, so best is 10
    assert(maxPathSumInBinaryTree(negRootPos) == 10);
    delete negRootPos->left; delete negRootPos->right; delete negRootPos;
    
    // Test 7: long chain: 1 -> 2 -> 3 -> 4 (right children only)
    TreeNode* chain = new TreeNode(1);
    chain->right = new TreeNode(2);
    chain->right->right = new TreeNode(3);
    chain->right->right->right = new TreeNode(4);
    assert(maxPathSumInBinaryTree(chain) == 10); // 1+2+3+4
    delete chain->right->right->right; delete chain->right->right; delete chain->right; delete chain;
    
    // Test 8: left-only chain with negative in middle: 5 -> left -1 -> left 2
    TreeNode* leftChain = new TreeNode(5);
    leftChain->left = new TreeNode(-1);
    leftChain->left->left = new TreeNode(2);
    // Best path: 5+(-1)+2 =6, or 2 alone, or 5 alone. Best =6
    assert(maxPathSumInBinaryTree(leftChain) == 6);
    delete leftChain->left->left; delete leftChain->left; delete leftChain;
    
    return 0;
}

// The core idea is a post-order traversal. For each node, we compute the maximum "single-branch" path sum starting at that node and going down through either its left or right child (or just the node itself if all children contributions are negative). This value is then stored temporarily and used by the parent. At each node, we also consider the maximum path that goes through this node, which is the sum of the node's value plus the best left-branch contribution (if any) plus the best right-branch contribution (if any). We update a global maximum with this candidate. The recursion returns the best single-branch value upward so that ancestors can incorporate it without branching (since paths cannot fork twice). Edge cases: (1) empty tree returns 0; (2) leaf nodes are handled naturally; (3) negative child branches are ignored by using `max(0, branch)` only for the through-path calculation, but for the single-branch return we must not force a zero—the returned value should be the maximum of (node value alone) and (node value plus best positive child branch), because a path must include the node; (4) when both children exist, the best through-path is node + max(0, leftBest) + max(0, rightBest), but the single-branch return is node + max(0, leftBest, rightBest). Time complexity is O(N) with N nodes, since each node is visited once. Space complexity is O(H) for recursion stack, where H is the tree height (worst-case O(N) for a skewed tree).
