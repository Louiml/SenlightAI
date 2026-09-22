// Write a C++ function named `sortedArrayToBST` that takes a constant reference to a `std::vector<int>` containing integers in strictly ascending order and returns a pointer to the root of a height-balanced binary search tree (BST) constructed from those values. The tree must satisfy the definition of height-balanced: for every node, the depths of its left and right subtrees differ by at most 1. The function should handle an empty input vector by returning `nullptr`. You may define a `struct TreeNode` with `int val`, `TreeNode* left`, and `TreeNode* right` members (with a constructor initializing `val` and setting children to `nullptr`). The function must not modify the input vector. Ensure the solution works correctly for vectors with duplicate values? (Note: the problem states ascending order, but duplicates are allowed; if present, the tree may not be strictly a BST by the usual definition, but the task only requires height-balanced tree construction from the sorted sequence.) Focus on correct recursive construction and proper memory allocation.
#include <cassert>
#include <vector>

// Include the TreeNode and sortedArrayToBST definitions here (from the Solution section)

// Helper to check height-balance: returns height, or -1 if not balanced
int checkBalanced(TreeNode* node) {
    if (!node) return 0;
    int leftH = checkBalanced(node->left);
    if (leftH < 0) return -1;
    int rightH = checkBalanced(node->right);
    if (rightH < 0) return -1;
    if (std::abs(leftH - rightH) > 1) return -1;
    return std::max(leftH, rightH) + 1;
}

// Helper to perform in-order traversal to verify sorting
void inOrder(TreeNode* node, std::vector<int>& out) {
    if (!node) return;
    inOrder(node->left, out);
    out.push_back(node->val);
    inOrder(node->right, out);
}

int main() {
    // Test 1: Empty vector
    std::vector<int> empty;
    assert(sortedArrayToBST(empty) == nullptr);

    // Test 2: Single element
    std::vector<int> single = {5};
    TreeNode* t1 = sortedArrayToBST(single);
    assert(t1 != nullptr);
    assert(t1->val == 5);
    assert(t1->left == nullptr && t1->right == nullptr);
    delete t1;

    // Test 3: Odd-length sorted vector from example
    std::vector<int> odd = {-10, -3, 0, 5, 9};
    TreeNode* t2 = sortedArrayToBST(odd);
    assert(t2 != nullptr);
    assert(checkBalanced(t2) >= 0);  // must be height-balanced
    std::vector<int> in1;
    inOrder(t2, in1);
    assert(in1 == odd);  // in-order traversal yields sorted order
    // Cleanup (not exhaustive, but delete root for demonstration; proper cleanup would require recursive delete)
    delete t2->left->left;  // manually clean to avoid leaks in test
    delete t2->left;
    delete t2->right->left;
    delete t2->right;
    delete t2;

    // Test 4: Even-length sorted vector
    std::vector<int> even = {1, 2, 3, 4};
    TreeNode* t3 = sortedArrayToBST(even);
    assert(t3 != nullptr);
    assert(checkBalanced(t3) >= 0);
    std::vector<int> in2;
    inOrder(t3, in2);
    assert(in2 == even);
    // Cleanup
    delete t3->left;
    delete t3->right->left;
    delete t3->right;
    delete t3;

    // Test 5: Duplicate values (sorted but not strictly increasing)
    std::vector<int> dup = {2, 2, 2};
    TreeNode* t4 = sortedArrayToBST(dup);
    assert(t4 != nullptr);
    assert(checkBalanced(t4) >= 0);
    std::vector<int> in3;
    inOrder(t4, in3);
    assert(in3 == dup);
    // Cleanup
    delete t4->left;
    delete t4->right;
    delete t4;

    // Test 6: Larger vector to ensure balance
    std::vector<int> large;
    for (int i = 0; i < 15; ++i) large.push_back(i * 2);
    TreeNode* t5 = sortedArrayToBST(large);
    assert(t5 != nullptr);
    assert(checkBalanced(t5) >= 0);
    std::vector<int> in4;
    inOrder(t5, in4);
    assert(in4 == large);
    // Cleanup omitted for brevity (would require recursive delete)

    return 0;
}
#include <vector>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// Helper function that recursively constructs a BST from nums[begin, end)
TreeNode* buildSubtree(const std::vector<int>& nums, int begin, int end) {
    if (begin >= end) {
        return nullptr;
    }
    int mid = begin + (end - begin) / 2;  // avoid overflow and choose lower-middle
    TreeNode* node = new TreeNode(nums[mid]);
    node->left = buildSubtree(nums, begin, mid);
    node->right = buildSubtree(nums, mid + 1, end);
    return node;
}

// Public function: converts a sorted ascending vector to a height-balanced BST
TreeNode* sortedArrayToBST(const std::vector<int>& nums) {
    return buildSubtree(nums, 0, static_cast<int>(nums.size()));
}
// The optimal approach is to simulate building the tree recursively by always selecting the middle element of the current subarray as the root node. Because the input array is sorted, the middle element naturally partitions the remaining elements into left (smaller values) and right (larger values) subarrays of roughly equal size, which guarantees the height-balanced property. Specifically, if the subarray spans indices `[begin, end)` (with `end` exclusive), compute `mid = (begin + end) / 2` (or `begin + (end - begin) / 2` to avoid overflow). Create a `TreeNode` with `nums[mid]`, then recursively build the left subtree from `[begin, mid)` and the right subtree from `[mid+1, end)`. The base case occurs when `begin >= end`, returning `nullptr`. This naturally handles the empty vector (initial call with `{0, 0}`) and single-element vectors. For time complexity, each node is visited exactly once, and each level processes a combined O(n) work, so it is O(n). The recursion stack depth is O(log n) for a balanced tree, so auxiliary space (excluding the tree itself) is O(log n) due to recursion; the input vector itself is not copied if we pass indices. Edge cases include: empty input (returns nullptr), single-element input (returns a leaf node), and vectors with even length (two middle candidates—choosing the lower index or upper index both work; we choose the lower index via integer division). Duplicate values are handled naturally as they simply occupy adjacent positions; the tree remains height-balanced.
