Write a C++ function named `largestValuesPerLevel` that takes a binary tree's root node (defined by the provided `TreeNode` struct) and returns a `std::vector<int>` where each element at index `i` represents the maximum value among all nodes at depth `i` (with the root at depth 0). The tree may be empty (root is `nullptr`), contain negative values, and may be unbalanced. The function should not modify the tree and must be `const`-correct, meaning it should accept a pointer to a `const TreeNode` or a `const TreeNode*` (the original snippet uses non-const, but for a standalone task, apply `const` correctness to the tree nodes). Implement the traversal iteratively or recursively, but ensure it is efficient in both time and space. The function must compile with standard C++ headers and not rely on global variables or extra parameters beyond the root.

// The core task is to compute the maximum value at each tree level (depth). The optimal approach is a depth-first traversal where we track the current depth (level) as we recurse. At each node, we either append its value to the result vector if we are visiting a new depth for the first time (since the vector's current size equals the depth index), or update the existing maximum at that index if the node's value is larger. This works because DFS visits nodes in a depth-first order, but the result vector is indexed by depth, so any order among nodes at the same depth is fine. Edge cases: (1) Empty tree → return an empty vector. (2) Negative values → must be handled correctly by initializing the max with the first node value at each level, not with a default like 0 or INT_MIN (though using INT_MIN works, it's safer to initialize from the first encountered node). (3) Unbalanced tree → some levels may have only one node; the algorithm still correctly initializes that level's entry. Time complexity is O(N) where N is the number of nodes, since each node is visited once. Space complexity is O(H) for the recursion stack in the worst case (H = tree height), plus O(L) for the result vector where L is the number of levels, but asymptotically this is O(N) in the worst case for a skewed tree (H = N) and O(N) for the result vector in the worst case (all nodes on distinct levels? Actually L ≤ N, so O(N) total). For the solution, we'll write a recursive helper that takes a `const TreeNode*` and a reference to the result vector and the current level, and we'll apply `const` correctly.

#include <vector>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper function that recursively traverses the tree and updates per-level maximums.
static void collectLevelMax(const TreeNode* node, std::vector<int>& result, size_t depth) {
    if (node == nullptr) {
        return;
    }
    if (result.size() <= depth) {
        // First time visiting this depth: initialize with current node's value.
        result.push_back(node->val);
    } else {
        // Update the maximum for this depth if current node's value is larger.
        if (result[depth] < node->val) {
            result[depth] = node->val;
        }
    }
    // Recurse into children at the next depth.
    collectLevelMax(node->left, result, depth + 1);
    collectLevelMax(node->right, result, depth + 1);
}

// Public function: returns the maximum value at each tree level.
std::vector<int> largestValuesPerLevel(const TreeNode* root) {
    std::vector<int> levelMax;
    collectLevelMax(root, levelMax, 0);
    return levelMax;
}

#include <cassert>
#include <vector>

// Assume the TreeNode struct and largestValuesPerLevel function are already defined above.

// Helper to manually build and delete a tree for testing.
TreeNode* createNode(int val) { return new TreeNode(val); }
void deleteTree(TreeNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

int main() {
    // Test 1: Empty tree
    assert(largestValuesPerLevel(nullptr).empty());

    // Test 2: Single node
    TreeNode* t1 = createNode(5);
    std::vector<int> res1 = largestValuesPerLevel(t1);
    assert(res1.size() == 1 && res1[0] == 5);
    deleteTree(t1);

    // Test 3: Balanced tree with mixed values
    //       1
    //      / \
    //     3   2
    //    / \   \
    //   5   3   9
    TreeNode* t2 = createNode(1);
    t2->left = createNode(3);
    t2->right = createNode(2);
    t2->left->left = createNode(5);
    t2->left->right = createNode(3);
    t2->right->right = createNode(9);
    std::vector<int> res2 = largestValuesPerLevel(t2);
    assert(res2.size() == 3);
    assert(res2[0] == 1);
    assert(res2[1] == 3); // max of {3,2} is 3
    assert(res2[2] == 9); // max of {5,3,9} is 9
    deleteTree(t2);

    // Test 4: Unbalanced tree (left skew) with negative values
    //    -2
    //    /
    //  -5
    //  /
    // -1
    TreeNode* t3 = createNode(-2);
    t3->left = createNode(-5);
    t3->left->left = createNode(-1);
    std::vector<int> res3 = largestValuesPerLevel(t3);
    assert(res3.size() == 3);
    assert(res3[0] == -2);
    assert(res3[1] == -5);
    assert(res3[2] == -1);
    deleteTree(t3);

    // Test 5: Right skew with all same values
    //  7
    //   \
    //    7
    //     \
    //      7
    TreeNode* t4 = createNode(7);
    t4->right = createNode(7);
    t4->right->right = createNode(7);
    std::vector<int> res4 = largestValuesPerLevel(t4);
    assert(res4.size() == 3);
    assert(res4[0] == 7 && res4[1] == 7 && res4[2] == 7);
    deleteTree(t4);

    return 0;
}
