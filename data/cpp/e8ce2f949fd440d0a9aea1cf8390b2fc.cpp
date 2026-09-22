// Given a sorted (in ascending order) vector of distinct integers, write a C++ function `sortedArrayToBST` that returns the root node of a height-balanced binary search tree (BST) constructed from the array. A height-balanced BST is one where for every node, the absolute difference between the heights of its left and right subtrees is at most 1. The function should take a `const std::vector<int>&` as input, and you must define your own `TreeNode` struct with integer value and left/right pointers. The returned tree must satisfy the BST property (left subtree values < current value < right subtree values) and the height-balance requirement.
The core idea is to use the divide-and-conquer approach via recursion. Since the array is sorted, the middle element naturally becomes the root of a BST subtree: all elements to its left are smaller and belong in the left subtree, and all elements to its right are larger and belong in the right subtree. By always picking the middle index (using `low + (high - low) / 2` to avoid potential overflow), we guarantee that the left and right halves have sizes differing by at most one, which ensures the tree is height-balanced. The recursion proceeds by solving the left half (`[low, mid-1]`) and the right half (`[mid+1, high]`) independently, and attaching the resulting roots as children. The base case occurs when `low > high`, indicating an empty subarray, and we return `nullptr`. Edge cases include an empty input vector (return `nullptr`) and a single-element vector (the root has no children). The algorithm visits each element exactly once, so the time complexity is O(n). The recursion depth is O(log n) for a balanced construction, so the auxiliary space (ignoring the output tree) is O(log n) due to the call stack. The output tree itself has n nodes.
#include <vector>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper function that recursively builds the BST from a sorted subarray.
TreeNode* buildBST(const std::vector<int>& nums, int low, int high) {
    if (low > high) {
        return nullptr;
    }
    // Use (low + high) / 2 but avoid overflow by writing low + (high - low) / 2.
    int mid = low + (high - low) / 2;
    TreeNode* node = new TreeNode(nums[mid]);
    node->left = buildBST(nums, low, mid - 1);
    node->right = buildBST(nums, mid + 1, high);
    return node;
}

// Public function that converts a sorted vector to a height-balanced BST.
TreeNode* sortedArrayToBST(const std::vector<int>& nums) {
    return buildBST(nums, 0, static_cast<int>(nums.size()) - 1);
}
#include <cassert>
#include <vector>
#include <cmath>

// Helper to check height and balance recursively.
int checkHeight(TreeNode* node, bool& balanced) {
    if (node == nullptr) return 0;
    int leftH = checkHeight(node->left, balanced);
    int rightH = checkHeight(node->right, balanced);
    if (std::abs(leftH - rightH) > 1) balanced = false;
    return 1 + std::max(leftH, rightH);
}

// Helper to verify BST property in-order traversal.
void checkBSTOrder(TreeNode* node, std::vector<int>& out) {
    if (node == nullptr) return;
    checkBSTOrder(node->left, out);
    out.push_back(node->val);
    checkBSTOrder(node->right, out);
}

int main() {
    // Test 1: empty input
    std::vector<int> empty;
    TreeNode* rootEmpty = sortedArrayToBST(empty);
    assert(rootEmpty == nullptr);

    // Test 2: single element
    std::vector<int> single = {42};
    TreeNode* rootSingle = sortedArrayToBST(single);
    assert(rootSingle != nullptr);
    assert(rootSingle->val == 42);
    assert(rootSingle->left == nullptr);
    assert(rootSingle->right == nullptr);
    bool balSingle = true;
    assert(checkHeight(rootSingle, balSingle) == 1 && balSingle);

    // Test 3: sorted array of length 3
    std::vector<int> arr3 = {-10, -3, 0};
    TreeNode* root3 = sortedArrayToBST(arr3);
    bool bal3 = true;
    checkHeight(root3, bal3);
    assert(bal3);
    std::vector<int> order3;
    checkBSTOrder(root3, order3);
    assert(order3 == arr3);

    // Test 4: sorted array of length 6
    std::vector<int> arr6 = {-10, -3, 0, 5, 9, 11};
    TreeNode* root6 = sortedArrayToBST(arr6);
    bool bal6 = true;
    checkHeight(root6, bal6);
    assert(bal6);
    std::vector<int> order6;
    checkBSTOrder(root6, order6);
    assert(order6 == arr6);

    // Test 5: long sorted array to stress balance
    std::vector<int> arrBig;
    for (int i = 0; i < 100; ++i) arrBig.push_back(i * 2);
    TreeNode* rootBig = sortedArrayToBST(arrBig);
    bool balBig = true;
    int heightBig = checkHeight(rootBig, balBig);
    assert(balBig);
    // For 100 nodes, height should be at most ceil(log2(101)) ≈ 7
    assert(heightBig <= 7);
    std::vector<int> orderBig;
    checkBSTOrder(rootBig, orderBig);
    assert(orderBig == arrBig);

    // Clean up memory (not strictly required for testing but good practice)
    // Note: For brevity in this exercise, we skip full deletion.

    return 0;
}
