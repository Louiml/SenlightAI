// Write a C++ function `buildTreeFromPreorderInorder` that takes two vectors of integers, `preorder` and `inorder`, representing the preorder and inorder traversals of a binary tree with unique node values, and returns a pointer to the root of the reconstructed binary tree. The function must handle empty input (return `nullptr`), and the tree may be skewed (e.g., all nodes on one side). The node structure is `struct TreeNode { int val; TreeNode *left; TreeNode *right; TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} };`. You must not use global variables; the solution should be self-contained within the function and any helper functions you define. The function signature is `TreeNode* buildTreeFromPreorderInorder(const std::vector<int>& preorder, const std::vector<int>& inorder)`. The uniqueness of values is guaranteed.

// The problem is reconstructing a binary tree from its preorder and inorder traversals. In a preorder traversal, the first element is the root. In an inorder traversal, the elements to the left of the root's value belong to the left subtree, and those to the right belong to the right subtree. The algorithm uses a recursive approach: at each call, we take the current root value from the preorder vector (using an index that increments as we process nodes), find its position in the inorder vector using a precomputed hash map (value → index) for O(1) lookup, then recursively build the left subtree from the inorder range `[l, inorderIndex-1]` and the right subtree from `[inorderIndex+1, r]`. The recursion terminates when `l > r`, returning `nullptr`. This naturally handles skewed trees because the inorder range will shrink appropriately. The overall time complexity is O(n) since each node is processed once and hash map lookups are O(1). Space complexity is O(n) for the hash map and the recursion stack in the worst case (skewed tree). Edge cases include empty input (return nullptr) and single-node trees.

#include <vector>
#include <unordered_map>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper recursive function to build the tree.
TreeNode* buildHelper(
    const std::vector<int>& preorder,
    const std::unordered_map<int, int>& inorderMap,
    int& preIndex,
    int left,
    int right
) {
    if (left > right) {
        return nullptr;
    }
    
    // The current root value is the next in preorder.
    int rootVal = preorder[preIndex++];
    TreeNode* root = new TreeNode(rootVal);
    
    // Find the root's index in inorder to split left and right subtrees.
    int inorderIndex = inorderMap.at(rootVal);
    
    // Recursively build left and right subtrees.
    root->left = buildHelper(preorder, inorderMap, preIndex, left, inorderIndex - 1);
    root->right = buildHelper(preorder, inorderMap, preIndex, inorderIndex + 1, right);
    
    return root;
}

// Main function to build the tree from preorder and inorder traversals.
TreeNode* buildTreeFromPreorderInorder(
    const std::vector<int>& preorder,
    const std::vector<int>& inorder
) {
    if (preorder.empty() || inorder.empty()) {
        return nullptr;
    }
    
    // Build a hash map for O(1) inorder index lookup.
    std::unordered_map<int, int> inorderMap;
    for (int i = 0; i < inorder.size(); ++i) {
        inorderMap[inorder[i]] = i;
    }
    
    int preIndex = 0;
    return buildHelper(preorder, inorderMap, preIndex, 0, inorder.size() - 1);
}

#include <cassert>
#include <vector>

// Assume TreeNode definition and buildTreeFromPreorderInorder are available above.

// Helper to check tree equality or convert to vector for testing.
void inorderCollect(TreeNode* node, std::vector<int>& result) {
    if (!node) return;
    inorderCollect(node->left, result);
    result.push_back(node->val);
    inorderCollect(node->right, result);
}

void preorderCollect(TreeNode* node, std::vector<int>& result) {
    if (!node) return;
    result.push_back(node->val);
    preorderCollect(node->left, result);
    preorderCollect(node->right, result);
}

int main() {
    // Test 1: Normal tree from the problem statement.
    std::vector<int> pre1 = {3, 9, 20, 15, 7};
    std::vector<int> in1  = {9, 3, 15, 20, 7};
    TreeNode* root1 = buildTreeFromPreorderInorder(pre1, in1);
    std::vector<int> preResult1, inResult1;
    preorderCollect(root1, preResult1);
    inorderCollect(root1, inResult1);
    assert(preResult1 == pre1);
    assert(inResult1 == in1);

    // Test 2: Single node.
    std::vector<int> pre2 = {5};
    std::vector<int> in2  = {5};
    TreeNode* root2 = buildTreeFromPreorderInorder(pre2, in2);
    assert(root2 != nullptr);
    assert(root2->val == 5);
    assert(root2->left == nullptr && root2->right == nullptr);

    // Test 3: Left-skewed tree.
    std::vector<int> pre3 = {1, 2, 3};
    std::vector<int> in3  = {3, 2, 1};
    TreeNode* root3 = buildTreeFromPreorderInorder(pre3, in3);
    std::vector<int> preResult3, inResult3;
    preorderCollect(root3, preResult3);
    inorderCollect(root3, inResult3);
    assert(preResult3 == pre3);
    assert(inResult3 == in3);

    // Test 4: Right-skewed tree.
    std::vector<int> pre4 = {1, 2, 3};
    std::vector<int> in4  = {1, 2, 3};
    TreeNode* root4 = buildTreeFromPreorderInorder(pre4, in4);
    std::vector<int> preResult4, inResult4;
    preorderCollect(root4, preResult4);
    inorderCollect(root4, inResult4);
    assert(preResult4 == pre4);
    assert(inResult4 == in4);

    // Test 5: Empty input.
    TreeNode* root5 = buildTreeFromPreorderInorder({}, {});
    assert(root5 == nullptr);

    // Test 6: Larger random-like tree (balanced).
    std::vector<int> pre6 = {10, 5, 3, 7, 15, 12, 20};
    std::vector<int> in6  = {3, 5, 7, 10, 12, 15, 20};
    TreeNode* root6 = buildTreeFromPreorderInorder(pre6, in6);
    std::vector<int> preResult6, inResult6;
    preorderCollect(root6, preResult6);
    inorderCollect(root6, inResult6);
    assert(preResult6 == pre6);
    assert(inResult6 == in6);

    return 0;
}
