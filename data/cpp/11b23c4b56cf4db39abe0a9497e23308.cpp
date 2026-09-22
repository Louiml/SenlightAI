Write a C++ function that takes a sorted vector of distinct integers (non-decreasing order) and returns the root pointer of a height-balanced binary search tree (BST) constructed from those values. A height-balanced BST is one where for every node, the depths of the left and right subtrees differ by at most 1. The function must build the tree recursively by always selecting the middle element of the current subarray as the root, then recursively building the left subtree from the left half and the right subtree from the right half. Assume the vector is non-empty and contains unique, sorted values. Provide a free function (not a class method) named `sortedArrayToBalancedBST` that accepts a `const std::vector<int>&` and returns a pointer to a `TreeNode` structure (which you must define in your solution). The function must be `const`-correct and must not leak memory.
#include <cassert>
#include <vector>
#include <cmath>

// Helper to compute height of a tree (for validation only)
int treeHeight(TreeNode* node) {
    if (node == nullptr) return 0;
    return 1 + std::max(treeHeight(node->left), treeHeight(node->right));
}

// Helper to check if a tree is height-balanced
bool isBalanced(TreeNode* node) {
    if (node == nullptr) return true;
    int leftH = treeHeight(node->left);
    int rightH = treeHeight(node->right);
    return std::abs(leftH - rightH) <= 1 && isBalanced(node->left) && isBalanced(node->right);
}

// Helper to perform in-order traversal and verify sortedness
void inorderCollect(TreeNode* node, std::vector<int>& result) {
    if (node == nullptr) return;
    inorderCollect(node->left, result);
    result.push_back(node->val);
    inorderCollect(node->right, result);
}

int main() {
    // Test 1: single element
    std::vector<int> v1 = {1};
    TreeNode* t1 = sortedArrayToBalancedBST(v1);
    assert(t1 != nullptr && t1->val == 1 && t1->left == nullptr && t1->right == nullptr);
    assert(isBalanced(t1));
    delete t1;

    // Test 2: two elements
    std::vector<int> v2 = {1, 2};
    TreeNode* t2 = sortedArrayToBalancedBST(v2);
    assert(t2 != nullptr && t2->val == 1);
    assert(t2->right != nullptr && t2->right->val == 2);
    assert(isBalanced(t2));
    // cleanup
    delete t2->right;
    delete t2;

    // Test 3: three elements
    std::vector<int> v3 = {1, 2, 3};
    TreeNode* t3 = sortedArrayToBalancedBST(v3);
    assert(t3 != nullptr && t3->val == 2);
    assert(t3->left != nullptr && t3->left->val == 1);
    assert(t3->right != nullptr && t3->right->val == 3);
    assert(isBalanced(t3));
    // cleanup
    delete t3->left;
    delete t3->right;
    delete t3;

    // Test 4: seven elements (perfect tree)
    std::vector<int> v4 = {1, 2, 3, 4, 5, 6, 7};
    TreeNode* t4 = sortedArrayToBalancedBST(v4);
    assert(treeHeight(t4) == 3);
    assert(isBalanced(t4));
    std::vector<int> inorder4;
    inorderCollect(t4, inorder4);
    assert(inorder4 == v4);
    // cleanup: simple recursive delete (not required by task, but for memory safety in test)
    // (Multiple deletes omitted for brevity; assert-only is acceptable here.)

    // Test 5: empty vector
    std::vector<int> v5;
    TreeNode* t5 = sortedArrayToBalancedBST(v5);
    assert(t5 == nullptr);

    // Test 6: large sorted vector
    std::vector<int> v6;
    for (int i = 0; i < 1000; ++i) v6.push_back(i * 2);
    TreeNode* t6 = sortedArrayToBalancedBST(v6);
    assert(isBalanced(t6));
    std::vector<int> inorder6;
    inorderCollect(t6, inorder6);
    assert(inorder6 == v6);

    return 0;
}
#include <vector>
#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper function to recursively build the balanced BST from a sorted array.
TreeNode* buildBST(const std::vector<int>& nums, int low, int high) {
    if (low > high) {
        return nullptr;
    }
    int mid = low + (high - low) / 2;
    TreeNode* root = new TreeNode(nums[mid]);
    root->left = buildBST(nums, low, mid - 1);
    root->right = buildBST(nums, mid + 1, high);
    return root;
}

// Public function: constructs a height-balanced BST from a sorted vector.
TreeNode* sortedArrayToBalancedBST(const std::vector<int>& nums) {
    if (nums.empty()) {
        return nullptr;
    }
    return buildBST(nums, 0, static_cast<int>(nums.size()) - 1);
}
// The algorithm follows the divide-and-conquer strategy. For a sorted array segment defined by indices `[low, high]`, compute the middle index `mid = low + (high - low) / 2` (using this formulation avoids potential overflow). Create a new `TreeNode` with value `nums[mid]`. Recursively build the left subtree from indices `[low, mid-1]` and the right subtree from indices `[mid+1, high]`. The base case is when `low > high`, in which case return `nullptr`. This construction guarantees a balanced tree because each recursive step divides the range roughly in half, so the height is `O(log n)` for `n` elements. Edge cases include single-element arrays (return a leaf node) and arrays of length two (the middle index picks the first element, and the right subtree gets the second element, resulting in a valid balanced tree). Time complexity is `O(n)` because each node is created exactly once and each recursive call handles one node. Space complexity is `O(log n)` for the recursion stack in a balanced tree, though the function itself does not allocate extra memory beyond the nodes.
