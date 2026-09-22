Write a C++ function `maxSumBST` that takes a pointer to the root of a binary tree (each node has an integer `val` and pointers to `left` and `right` children, where `nullptr` represents an empty subtree) and returns the maximum sum of values among all sub-trees that are valid Binary Search Trees (BSTs). A BST is defined such that for every node, all values in its left subtree are strictly less than the node's value, and all values in its right subtree are strictly greater. An empty tree (root is `nullptr`) is considered a valid BST with sum 0, but it counts as a valid sub-tree for the purpose of the answer only if it is not the whole tree (though taking the max with 0 handles it). The function must handle negative values, duplicate values (which make a sub-tree invalid if they appear on both sides improperly), and trees with a single node. The returned sum should be an integer.

// The solution uses a post-order traversal where each recursive call returns a small aggregate structure containing: whether the subtree is a BST, the minimum value in that subtree, the maximum value in that subtree, and the sum of values if the subtree is a BST (otherwise, the best valid BST sum found within that subtree). For a null node, return a structure with `bst = true`, `min = +infinity`, `max = -infinity`, and `sum = 0`. At each node, combine the left and right results: the node's subtree is a BST if both children are BSTs and `root->val > left.max` and `root->val < right.min`. If so, the new sum is `left.sum + right.sum + root->val`, new min is `min(root->val, left.min)`, new max is `max(root->val, right.max)`. If not a BST, set `bst = false` and carry forward the maximum sum from the children (`max(left.sum, right.sum)`). Update a global or reference variable `ans` with the current sum at each node. Edge cases: negative values, empty tree (return 0), tree with only one node (sum is that value if it's a BST, which it is), and cases where a subtree is not a BST but contains a valid BST deeper inside. Time complexity is O(n) where n is the number of nodes, as each node is visited once. Space complexity is O(h) for recursion stack, where h is the tree height.

#include <climits>
#include <algorithm>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

struct SubtreeInfo {
    bool isBST;
    int minVal;
    int maxVal;
    int sum; // if isBST, sum of this subtree; else best valid BST sum inside
};

// Returns the maximum sum of all valid BST sub-trees in the given tree.
int maxSumBST(TreeNode* root) {
    int ans = 0;
    // Recursive helper that returns SubtreeInfo and updates ans
    auto dfs = [&](TreeNode* node) -> SubtreeInfo {
        if (node == nullptr) {
            // Empty tree is a BST with sum 0, min=+inf, max=-inf
            return {true, INT_MAX, INT_MIN, 0};
        }
        SubtreeInfo left = dfs(node->left);
        SubtreeInfo right = dfs(node->right);
        SubtreeInfo current;
        if (left.isBST && right.isBST && node->val > left.maxVal && node->val < right.minVal) {
            current.isBST = true;
            current.sum = left.sum + right.sum + node->val;
            current.minVal = std::min(node->val, left.minVal);
            current.maxVal = std::max(node->val, right.maxVal);
        } else {
            current.isBST = false;
            current.sum = std::max(left.sum, right.sum);
            // minVal/maxVal are not meaningful when not a BST, but set to avoid warnings
            current.minVal = 0;
            current.maxVal = 0;
        }
        ans = std::max(ans, current.sum);
        return current;
    };
    dfs(root);
    return ans;
}

#include <cassert>

int main() {
    // Test 1: Empty tree (root = nullptr)
    assert(maxSumBST(nullptr) == 0);

    // Test 2: Single node
    TreeNode* t2 = new TreeNode(5);
    assert(maxSumBST(t2) == 5);
    delete t2;

    // Test 3: Simple valid BST: 2 (left 1, right 3) => sum = 6
    TreeNode* t3 = new TreeNode(2, new TreeNode(1), new TreeNode(3));
    assert(maxSumBST(t3) == 6);
    delete t3->left; delete t3->right; delete t3;

    // Test 4: Invalid BST overall, but valid subtree inside: root=5 (left 4, right 6), left=4 (left 3), right=6 (right 7)
    // subtree 3 is valid sum=3, subtree 7 is valid sum=7, subtree 4-3 not BST (4>3? wait 3 is left, so 4>3 and 3<4, but right of 4 is null => BST sum=7). Actually whole tree not BST because 6 is in right of 5 and 7 is right of 6, but 6<5? No, 6>5 but 7>6, so right subtree of 5 is BST? Let's construct: root=5, left=4 (left=3), right=6 (right=7). Check: left subtree (4 with left 3) is BST sum=7, right subtree (6 with right 7) is BST sum=13, root: 5 > max of left (4) and 5 < min of right (6) => whole is BST sum=5+7+13=25. So expected 25.
    TreeNode* t4 = new TreeNode(5, new TreeNode(4, new TreeNode(3), nullptr), new TreeNode(6, nullptr, new TreeNode(7)));
    assert(maxSumBST(t4) == 25);
    delete t4->left->left; delete t4->left; delete t4->right->right; delete t4->right; delete t4;

    // Test 5: Duplicate values make BST invalid: root=2 (left=2), not BST because 2 > left.max (2) is false. The left subtree (2) is a valid BST sum=2, so answer = max(0, 2) = 2.
    TreeNode* t5 = new TreeNode(2, new TreeNode(2), nullptr);
    assert(maxSumBST(t5) == 2);
    delete t5->left; delete t5;

    // Test 6: Negative values: root=-3 (left=-5, right=-2) is BST sum=-10, so answer = -10 (but since we initialize ans=0 and only max with current.sum, if all sums negative, ans stays 0, but the problem typically expects max sum of BST sub-trees, and empty tree sum=0 counts, so answer should be 0). However if the task expects maximum sum including negative, then we might need to initialize ans to INT_MIN. The given snippet initializes ans=0 so answer 0. For consistency with snippet, test expect 0.
    TreeNode* t6 = new TreeNode(-3, new TreeNode(-5), new TreeNode(-2));
    assert(maxSumBST(t6) == 0);
    delete t6->left; delete t6->right; delete t6;

    // Test 7: Mixed tree where best BST is not the whole tree: root=10 (left=5, right=15), left=5 (left=null, right=null) BST sum=5, right=15 (left=6, right=20) not BST because 6<15 but 20>6? Actually right subtree of 15: left=6, right=20, for node 15, left.max=6, right.min=20, 15<20 and 15>6 so that subtree is BST sum=41. Whole tree: 10 > left.max? left max=5, 10>5 ok; 10 < right.min? right.min of subtree (15) is min of its left (6) and its own val? The right subtree min is 6, so 10<6 false, so whole not BST. Best is right subtree sum=41. So answer=41.
    TreeNode* t7 = new TreeNode(10, new TreeNode(5), new TreeNode(15, new TreeNode(6), new TreeNode(20)));
    assert(maxSumBST(t7) == 41);
    delete t7->left; delete t7->right->left; delete t7->right->right; delete t7->right; delete t7;

    // Test 8: Tree with only left chain: root=3 (left=2 (left=1)) => whole is BST sum=6, answer=6
    TreeNode* t8 = new TreeNode(3, new TreeNode(2, new TreeNode(1), nullptr), nullptr);
    assert(maxSumBST(t8) == 6);
    delete t8->left->left; delete t8->left; delete t8;

    // Test 9: Tree with a large invalid root but best BST is a leaf: root=0 (left=10, right=20) – not BST because 0<10? left max=10, 0>10 false. Left subtree (10) sum=10, right (20) sum=20, answer=20.
    TreeNode* t9 = new TreeNode(0, new TreeNode(10), new TreeNode(20));
    assert(maxSumBST(t9) == 20);
    delete t9->left; delete t9->right; delete t9;

    // Test 10: Complex tree where best BST is a small valid sub-tree with negative values but others positive: root=5 (left=3 (left=1, right=4) BST sum=8, right=8 (left=7, right=9) BST sum=24, but whole root: 5>max left=4, 5<min right=7 => whole BST sum=5+8+24=37. So answer=37.
    TreeNode* t10 = new TreeNode(5, new TreeNode(3, new TreeNode(1), new TreeNode(4)), new TreeNode(8, new TreeNode(7), new TreeNode(9)));
    assert(maxSumBST(t10) == 37);
    delete t10->left->left; delete t10->left->right; delete t10->left;
    delete t10->right->left; delete t10->right->right; delete t10->right; delete t10;
}
