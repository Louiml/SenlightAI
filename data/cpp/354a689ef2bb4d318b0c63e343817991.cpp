Write a C++ function `buildBalancedBST` that takes a sorted vector of unique integers in ascending order and returns a pointer to the root of a balanced binary search tree (BST) built from those values. The tree must be height-balanced: for every node, the heights of its left and right subtrees differ by at most 1. The function should not assume the input vector is non-empty; if it is empty, it must return `nullptr`. Use a recursive divide-and-conquer approach, where the middle element of each subarray becomes the root, and the left and right halves recursively form the left and right subtrees. The function must be `const`-correct and not modify the input vector. Provide a helper structure `TreeNode` with `int val`, `TreeNode* left`, and `TreeNode* right`, along with a constructor. Also implement a helper function `height` (non-public, but can be static or in an anonymous namespace) to compute the height of a tree, and a function `isBalanced` (non-public) to verify the balance property for testing. The main deliverable is `buildBalancedBST`.
The core idea is that to ensure balance, we always pick the middle element of the current sorted subarray (or any position between `low` and `high` that splits evenly) as the root. This guarantees that the number of nodes in the left and right subtrees differ by at most 1, which directly yields a height-balanced BST. We use recursion: base case when `low > high` returns `nullptr`. For the recursive step, compute `mid = low + (high - low) / 2` (careful to avoid overflow), create a new `TreeNode` with `arr[mid]`, then recursively build left subtree from `arr[low..mid-1]` and right subtree from `arr[mid+1..high]`. Attach them to the root. Edge cases: empty vector returns `nullptr`; single element returns a leaf. Duplicate values are not expected per problem statement. Time complexity: each node is created once, and the recursive partition visits each element exactly once, so O(n) where n is the number of elements. Space complexity: O(log n) for the recursion stack (due to balanced tree height), plus O(n) for storing the tree itself. The `height` function runs in O(n) per call, but for verifying balance we can call it recursively in O(n^2) worst case, though we only use it for testing. In the reference solution we implement `height` and `isBalanced` as static helper functions inside a namespace or as free functions in the solution code.
#include <vector>
#include <cstddef>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    explicit TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// Helper: compute height of a tree (or -1 for nullptr)
static int treeHeight(const TreeNode* root) {
    if (!root) return -1;
    return 1 + std::max(treeHeight(root->left), treeHeight(root->right));
}

// Helper: check if the tree rooted at 'root' is height-balanced
static bool isBalancedHelper(const TreeNode* root) {
    if (!root) return true;
    int leftH = treeHeight(root->left);
    int rightH = treeHeight(root->right);
    if (std::abs(leftH - rightH) > 1) return false;
    return isBalancedHelper(root->left) && isBalancedHelper(root->right);
}

// Recursive helper to build balanced BST from sorted subarray arr[low..high]
static TreeNode* buildHelper(const std::vector<int>& arr, int low, int high) {
    if (low > high) return nullptr;
    int mid = low + (high - low) / 2;
    TreeNode* root = new TreeNode(arr[mid]);
    root->left = buildHelper(arr, low, mid - 1);
    root->right = buildHelper(arr, mid + 1, high);
    return root;
}

// Public function: build a height-balanced BST from a sorted vector
TreeNode* buildBalancedBST(const std::vector<int>& sortedValues) {
    if (sortedValues.empty()) return nullptr;
    return buildHelper(sortedValues, 0, static_cast<int>(sortedValues.size()) - 1);
}
#include <cassert>
#include <vector>
#include <algorithm>

// (Declarations for helpers are not needed; they are in solution)
int main() {
    // Test 1: empty vector
    std::vector<int> empty;
    assert(buildBalancedBST(empty) == nullptr);

    // Test 2: single element
    std::vector<int> single = {42};
    TreeNode* root1 = buildBalancedBST(single);
    assert(root1 != nullptr);
    assert(root1->val == 42);
    assert(root1->left == nullptr && root1->right == nullptr);
    assert(isBalancedHelper(root1));

    // Test 3: small even-sized
    std::vector<int> arr2 = {1,2,3,4};
    TreeNode* root2 = buildBalancedBST(arr2);
    // In-order traversal should yield sorted values
    std::vector<int> traversal;
    std::function<void(TreeNode*)> inOrder = [&](TreeNode* node) {
        if (!node) return;
        inOrder(node->left);
        traversal.push_back(node->val);
        inOrder(node->right);
    };
    inOrder(root2);
    assert(traversal == arr2);
    assert(isBalancedHelper(root2));

    // Test 4: odd-sized with negative numbers and zero
    std::vector<int> arr3 = {-10,-3,0,5,9};
    TreeNode* root3 = buildBalancedBST(arr3);
    // Check root is the middle element (third element, index 2)
    assert(root3->val == 0);
    assert(isBalancedHelper(root3));

    // Test 5: larger array with duplicates not allowed, but test size 7
    std::vector<int> arr4 = {1,2,3,4,5,6,7};
    TreeNode* root4 = buildBalancedBST(arr4);
    assert(root4->val == 4);
    assert(isBalancedHelper(root4));
    // Verify height property explicitly: height of left and right differ by <=1
    assert(std::abs(treeHeight(root4->left) - treeHeight(root4->right)) <= 1);

    // Cleanup: delete all nodes (simple recursive delete)
    std::function<void(TreeNode*)> deleteTree = [&](TreeNode* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    };
    deleteTree(root1);
    deleteTree(root2);
    deleteTree(root3);
    deleteTree(root4);

    return 0;
}
