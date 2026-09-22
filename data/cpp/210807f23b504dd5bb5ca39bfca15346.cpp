// Given a sorted array of integers in ascending order, write a C++ function that constructs a balanced binary search tree (BST) from the array and returns the root node pointer. The BST must be height-balanced, meaning for every node, the depths of its left and right subtrees differ by at most 1. The function should recursively pick the middle element as the root, and build left and right subtrees from the subarrays. Edge cases include an empty array (return nullptr) and arrays with one or two elements. The input array is guaranteed to be sorted in non-decreasing order.

// The core algorithm is a binary search tree construction based on the divide-and-conquer approach: for a subarray defined by indices `l` and `r` (inclusive), we select the middle element `mid = (l + r) / 2` as the root of the current subtree. The left subtree is built recursively from indices `l` to `mid-1`, and the right subtree from `mid+1` to `r`. This ensures the resulting tree is height-balanced because each recursive call splits the remaining elements evenly. The base case occurs when `l > r`, meaning the subarray is empty, and we return a null pointer. Edge cases include `n = 0` (return nullptr immediately), `n = 1` (root has no children), and `n = 2` (root is the first middle element, with one child). Time complexity is O(n) because each array element is visited exactly once to create a node. Space complexity is O(log n) for the recursion stack in the balanced case, but O(n) in the worst case if the array were unsorted (though the input is sorted, the recursion depth is still O(log n) due to balanced splitting). The tree itself occupies O(n) memory for nodes.

#include <vector>

// Forward declaration to keep the solution self-contained.
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Recursively builds a height-balanced BST from a sorted subarray.
TreeNode* buildBalancedBST(const std::vector<int>& sortedArray, int left, int right) {
    if (left > right) {
        return nullptr;
    }
    int mid = left + (right - left) / 2; // Avoid overflow
    TreeNode* node = new TreeNode(sortedArray[mid]);
    node->left = buildBalancedBST(sortedArray, left, mid - 1);
    node->right = buildBalancedBST(sortedArray, mid + 1, right);
    return node;
}

// Public interface: converts a sorted vector to a balanced BST root.
TreeNode* sortedArrayToBST(const std::vector<int>& sortedArray) {
    if (sortedArray.empty()) {
        return nullptr;
    }
    return buildBalancedBST(sortedArray, 0, static_cast<int>(sortedArray.size()) - 1);
}

#include <cassert>
#include <vector>
#include <queue>
#include <algorithm>

// Helper: check if tree is height-balanced and returns height, or -1 if not.
int checkBalanced(TreeNode* root) {
    if (!root) return 0;
    int leftHeight = checkBalanced(root->left);
    if (leftHeight == -1) return -1;
    int rightHeight = checkBalanced(root->right);
    if (rightHeight == -1) return -1;
    if (std::abs(leftHeight - rightHeight) > 1) return -1;
    return std::max(leftHeight, rightHeight) + 1;
}

int main() {
    // Test 1: empty array
    std::vector<int> empty = {};
    assert(sortedArrayToBST(empty) == nullptr);

    // Test 2: single element
    std::vector<int> single = {42};
    TreeNode* root1 = sortedArrayToBST(single);
    assert(root1 != nullptr);
    assert(root1->val == 42);
    assert(root1->left == nullptr && root1->right == nullptr);
    assert(checkBalanced(root1) != -1);
    delete root1;

    // Test 3: two elements
    std::vector<int> two = {1, 2};
    TreeNode* root2 = sortedArrayToBST(two);
    assert(root2 != nullptr);
    assert(root2->val == 1);
    assert(root2->right != nullptr && root2->right->val == 2);
    assert(root2->left == nullptr);
    assert(checkBalanced(root2) != -1);
    delete root2->right;
    delete root2;

    // Test 4: odd number of elements
    std::vector<int> odd = {1, 2, 3, 4, 5};
    TreeNode* root3 = sortedArrayToBST(odd);
    assert(root3 != nullptr);
    assert(root3->val == 3);
    assert(checkBalanced(root3) != -1);
    // In-order traversal should give sorted values
    std::vector<int> result;
    std::function<void(TreeNode*)> inorder = [&](TreeNode* node) {
        if (!node) return;
        inorder(node->left);
        result.push_back(node->val);
        inorder(node->right);
    };
    inorder(root3);
    assert(result == odd);
    // Cleanup all nodes
    std::queue<TreeNode*> q;
    q.push(root3);
    while (!q.empty()) {
        TreeNode* cur = q.front(); q.pop();
        if (cur->left) q.push(cur->left);
        if (cur->right) q.push(cur->right);
        delete cur;
    }

    // Test 5: even number of elements with larger size
    std::vector<int> even = {-10, -3, 0, 5, 9, 12, 15, 20};
    TreeNode* root4 = sortedArrayToBST(even);
    assert(root4 != nullptr);
    assert(root4->val == 5); // mid = (0+7)/2 = 3 -> value 5
    assert(checkBalanced(root4) != -1);
    result.clear();
    inorder = [&](TreeNode* node) {
        if (!node) return;
        inorder(node->left);
        result.push_back(node->val);
        inorder(node->right);
    };
    inorder(root4);
    assert(result == even);
    q.push(root4);
    while (!q.empty()) {
        TreeNode* cur = q.front(); q.pop();
        if (cur->left) q.push(cur->left);
        if (cur->right) q.push(cur->right);
        delete cur;
    }

    // Test 6: all duplicates
    std::vector<int> dups = {7, 7, 7, 7};
    TreeNode* root5 = sortedArrayToBST(dups);
    assert(root5 != nullptr);
    assert(checkBalanced(root5) != -1);
    result.clear();
    inorder = [&](TreeNode* node) {
        if (!node) return;
        inorder(node->left);
        result.push_back(node->val);
        inorder(node->right);
    };
    inorder(root5);
    assert(result == dups);
    q.push(root5);
    while (!q.empty()) {
        TreeNode* cur = q.front(); q.pop();
        if (cur->left) q.push(cur->left);
        if (cur->right) q.push(cur->right);
        delete cur;
    }

    return 0;
}
