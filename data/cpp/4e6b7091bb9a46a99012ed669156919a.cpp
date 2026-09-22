Write a C++ function `TreeNode* constructMaximumBinaryTree(const std::vector<int>& nums)` that, given a non-empty vector of integers with no duplicates, constructs and returns the *maximum binary tree* defined by the following recursive rule: The root of the tree is the maximum element in the array. The left subtree is the maximum binary tree built from the elements to the left of that maximum, and the right subtree is the maximum binary tree built from the elements to the right. The function must take the vector by const reference, not modify the input, and return a dynamically allocated `TreeNode` (with `val`, `left`, and `right` members, defaulting to null). You may assume the vector has at least one element and contains distinct integers, but your implementation should still handle a vector of size 1 correctly (returning a single-node tree). Avoid copying the input vector or subvectors in your solution to keep it efficient. You must define the `TreeNode` struct yourself within the solution (with constructors) as shown in the snippet. Provide only the free function and struct definition.
#include <cassert>
#include <vector>

// TreeNode definition and solution function are assumed to be available.
// (In actual test, include the solution above.)

// Helper to check if a tree equals the expected maximum tree.
bool isSameTree(TreeNode* a, TreeNode* b) {
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;
    return (a->val == b->val) && isSameTree(a->left, b->left) && isSameTree(a->right, b->right);
}

// Helper to build a tree from a vector with -1 representing null (for testing).
TreeNode* buildTreeFromVector(const std::vector<int>& vals, int idx) {
    if (idx >= static_cast<int>(vals.size()) || vals[idx] == -1) return nullptr;
    TreeNode* root = new TreeNode(vals[idx]);
    root->left = buildTreeFromVector(vals, 2 * idx + 1);
    root->right = buildTreeFromVector(vals, 2 * idx + 2);
    return root;
}

int main() {
    // Test 1: Single element
    {
        std::vector<int> nums = {5};
        TreeNode* result = constructMaximumBinaryTree(nums);
        assert(result != nullptr);
        assert(result->val == 5);
        assert(result->left == nullptr && result->right == nullptr);
        delete result;
    }

    // Test 2: Simple [3,2,1]
    {
        std::vector<int> nums = {3,2,1};
        TreeNode* result = constructMaximumBinaryTree(nums);
        // Expected tree: root=3, left=null, right=2 (whose right=1)
        assert(result->val == 3);
        assert(result->left == nullptr);
        assert(result->right != nullptr && result->right->val == 2);
        assert(result->right->left == nullptr);
        assert(result->right->right != nullptr && result->right->right->val == 1);
        delete result->right->right;
        delete result->right;
        delete result;
    }

    // Test 3: [1,3,2] (max in middle)
    {
        std::vector<int> nums = {1,3,2};
        TreeNode* result = constructMaximumBinaryTree(nums);
        // Expected: root=3, left=1, right=2
        assert(result->val == 3);
        assert(result->left != nullptr && result->left->val == 1);
        assert(result->right != nullptr && result->right->val == 2);
        assert(result->left->left == nullptr && result->left->right == nullptr);
        assert(result->right->left == nullptr && result->right->right == nullptr);
        delete result->left;
        delete result->right;
        delete result;
    }

    // Test 4: [3,1,2] (max at start, then right subtree)
    {
        std::vector<int> nums = {3,1,2};
        TreeNode* result = constructMaximumBinaryTree(nums);
        // Expected: root=3, left=null, right subtree from [1,2] => root=2 left=1
        assert(result->val == 3);
        assert(result->left == nullptr);
        assert(result->right != nullptr && result->right->val == 2);
        assert(result->right->left != nullptr && result->right->left->val == 1);
        assert(result->right->right == nullptr);
        delete result->right->left;
        delete result->right;
        delete result;
    }

    // Test 5: [2,1,3] (max at end, left subtree)
    {
        std::vector<int> nums = {2,1,3};
        TreeNode* result = constructMaximumBinaryTree(nums);
        // Expected: root=3, left subtree from [2,1] => root=2 right=1, right=null
        assert(result->val == 3);
        assert(result->right == nullptr);
        assert(result->left != nullptr && result->left->val == 2);
        assert(result->left->left == nullptr);
        assert(result->left->right != nullptr && result->left->right->val == 1);
        delete result->left->right;
        delete result->left;
        delete result;
    }

    // Test 6: Larger array [3,2,1,6,0,5]
    {
        std::vector<int> nums = {3,2,1,6,0,5};
        TreeNode* result = constructMaximumBinaryTree(nums);
        // Expected maximum tree (level order): root=6, left=3, right=5, left's children: null,2? Actually let's build manually:
        // nums=[3,2,1,6,0,5], max=6 at index 3, left=[3,2,1], right=[0,5].
        // Left: max=3 at index0, left empty, right=[2,1] -> root=2, right=1.
        // Right: max=5 at index5, left=[0], right empty -> root=5, left=0.
        // So tree: 6 (left: 3 (left null, right: 2 (left null, right:1)), right: 5 (left:0, right null))
        assert(result->val == 6);
        assert(result->left != nullptr && result->left->val == 3);
        assert(result->right != nullptr && result->right->val == 5);
        // left subtree of 3
        assert(result->left->left == nullptr);
        assert(result->left->right != nullptr && result->left->right->val == 2);
        assert(result->left->right->left == nullptr);
        assert(result->left->right->right != nullptr && result->left->right->right->val == 1);
        // right subtree of 5
        assert(result->right->left != nullptr && result->right->left->val == 0);
        assert(result->right->right == nullptr);
        // Cleanup (simple recursive delete)
        // (Note: In production code, you'd use a proper delete function, but for test we can manually delete leaves upward)
        delete result->left->right->right;
        delete result->left->right;
        delete result->left;
        delete result->right->left;
        delete result->right;
        delete result;
    }

    // Test 7: Already sorted ascending [1,2,3,4] -> skewed right
    {
        std::vector<int> nums = {1,2,3,4};
        TreeNode* result = constructMaximumBinaryTree(nums);
        assert(result->val == 4);
        assert(result->left != nullptr && result->left->val == 3);
        assert(result->left->left != nullptr && result->left->left->val == 2);
        assert(result->left->left->left != nullptr && result->left->left->left->val == 1);
        assert(result->left->left->left->left == nullptr && result->left->left->left->right == nullptr);
        delete result->left->left->left;
        delete result->left->left;
        delete result->left;
        delete result;
    }

    // Test 8: Already sorted descending [4,3,2,1] -> skewed left
    {
        std::vector<int> nums = {4,3,2,1};
        TreeNode* result = constructMaximumBinaryTree(nums);
        assert(result->val == 4);
        assert(result->right != nullptr && result->right->val == 3);
        assert(result->right->right != nullptr && result->right->right->val == 2);
        assert(result->right->right->right != nullptr && result->right->right->right->val == 1);
        assert(result->right->right->right->right == nullptr && result->right->right->right->left == nullptr);
        delete result->right->right->right;
        delete result->right->right;
        delete result->right;
        delete result;
    }

    // Test 9: Two elements [2,1]
    {
        std::vector<int> nums = {2,1};
        TreeNode* result = constructMaximumBinaryTree(nums);
        assert(result->val == 2);
        assert(result->left == nullptr);
        assert(result->right != nullptr && result->right->val == 1);
        delete result->right;
        delete result;
    }

    return 0;
}
#include <vector>
#include <algorithm>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Recursive helper: build tree from nums[left..right-1].
TreeNode* buildMaxTree(const std::vector<int>& nums, int left, int right) {
    if (left >= right) {
        return nullptr;
    }
    // Find index of maximum element in [left, right).
    int maxIndex = left;
    for (int i = left + 1; i < right; ++i) {
        if (nums[i] > nums[maxIndex]) {
            maxIndex = i;
        }
    }
    // Create root with maximum value.
    TreeNode* root = new TreeNode(nums[maxIndex]);
    // Recursively build left and right subtrees from subarrays around max.
    root->left = buildMaxTree(nums, left, maxIndex);
    root->right = buildMaxTree(nums, maxIndex + 1, right);
    return root;
}

// Construct the maximum binary tree from the given nums (non-empty, distinct values).
TreeNode* constructMaximumBinaryTree(const std::vector<int>& nums) {
    return buildMaxTree(nums, 0, static_cast<int>(nums.size()));
}
// The core idea is a recursive divide-and-conquer approach using index bounds rather than creating new vectors. The helper function `traversal` takes the original vector (`nums`) and a half-open interval `[left, right)` representing the current subarray. If `left >= right`, the subarray is empty, so return `nullptr`. Otherwise, scan from `left` to `right-1` to find the index `maxIndex` of the maximum value in that subarray. Create a new `TreeNode` with that value. Then recursively build the left subtree from `[left, maxIndex)` and the right subtree from `[maxIndex+1, right)`, assigning the results to `root->left` and `root->right`. After recursion completes, return the root. Important edge cases: when the subarray has size 1, the loop finds that sole element as max, and both recursive calls receive empty intervals, resulting in a leaf node. When the maximum is at the far left or far right, one recursive call gets an empty interval and returns null, correctly leaving that child as a null pointer. Time complexity is O(n²) in the worst case (e.g., sorted array, where each level scans a subarray of decreasing size), but O(n log n) on average for random arrays. Space complexity is O(n) for the recursion stack in the worst case (skewed tree) and O(n) for the tree nodes themselves (since each element becomes a node exactly once). Since we only pass indices and a reference to the original vector, no extra space is used for subarray copies.
