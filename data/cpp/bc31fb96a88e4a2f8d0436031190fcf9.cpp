Write a C++ function `bool isValidBSTOrder(TreeNode* root)` that determines whether a given binary tree is a valid Binary Search Tree (BST). A binary tree is a valid BST if and only if, for every node, all values in its left subtree are strictly less than the node’s value, and all values in its right subtree are strictly greater than the node’s value. Additionally, the tree must have no duplicate values since the BST property requires strict ordering. The function should work on any binary tree where each node contains an integer `val` and pointers `left` and `right` that may be `nullptr`. You may assume the `TreeNode` structure is already defined as follows:
```cpp
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
```
Your solution must not modify the tree and must handle empty trees and trees with a single node correctly.
// A standard approach to validate a BST is to perform an in-order traversal (left, node, right) and collect the node values into a vector. For a valid BST, an in-order traversal yields a strictly increasing sequence. Therefore, after collecting all values, we check that each consecutive pair satisfies `arr[i] > arr[i-1]`. If any pair fails (i.e., `arr[i] <= arr[i-1]`), the tree is not a valid BST. This handles duplicates naturally because duplicates cause a non-strict increase. Edge cases: an empty tree (`root == nullptr`) is considered a valid BST (by convention), and a single-node tree is trivially valid. The recursion must carefully avoid null-pointer dereferences: if a child is `nullptr`, we simply skip the recursive call for that child but still push the current node’s value. The algorithm runs in O(n) time, where n is the number of nodes, because each node is visited exactly once during traversal, and the vector scan is linear. The auxiliary space is O(n) for the vector plus O(h) for the recursion stack, where h is the tree height (worst-case O(n) for a skewed tree). This approach is simple but not the most space-efficient; an alternative is to do an in-order traversal with a running previous value, but the vector method matches the provided snippet and is clear.
#include <vector>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper function: perform in-order traversal and collect node values into a vector.
void collectInorder(TreeNode* node, std::vector<int>& values) {
    if (node == nullptr) {
        return;
    }
    collectInorder(node->left, values);
    values.push_back(node->val);
    collectInorder(node->right, values);
}

// Check if the given binary tree is a valid BST (strictly increasing in-order sequence).
bool isValidBSTOrder(TreeNode* root) {
    std::vector<int> values;
    collectInorder(root, values);
    
    for (std::size_t i = 1; i < values.size(); ++i) {
        if (values[i] <= values[i - 1]) {
            return false; // Not strictly increasing → not a BST
        }
    }
    return true; // Empty tree (size 0) or single node (size 1) returns true
}
#include <cassert>

int main() {
    // Test 1: Empty tree is valid
    assert(isValidBSTOrder(nullptr) == true);
    
    // Test 2: Single node is valid
    TreeNode* single = new TreeNode(5);
    assert(isValidBSTOrder(single) == true);
    
    // Test 3: Simple valid BST
    TreeNode* root1 = new TreeNode(2);
    root1->left = new TreeNode(1);
    root1->right = new TreeNode(3);
    assert(isValidBSTOrder(root1) == true);
    
    // Test 4: Invalid because left child equals root (duplicate)
    TreeNode* root2 = new TreeNode(2);
    root2->left = new TreeNode(2);
    root2->right = new TreeNode(3);
    assert(isValidBSTOrder(root2) == false);
    
    // Test 5: Invalid because right child smaller than root
    TreeNode* root3 = new TreeNode(5);
    root3->left = new TreeNode(1);
    root3->right = new TreeNode(4);
    root3->right->left = new TreeNode(3);
    root3->right->right = new TreeNode(6);
    assert(isValidBSTOrder(root3) == false);
    
    // Test 6: Valid but deeper tree
    TreeNode* root4 = new TreeNode(10);
    root4->left = new TreeNode(5);
    root4->left->right = new TreeNode(7);
    root4->right = new TreeNode(15);
    root4->right->left = new TreeNode(12);
    root4->right->right = new TreeNode(20);
    assert(isValidBSTOrder(root4) == true);
    
    // Test 7: Invalid due to violation across subtrees (left subtree contains larger value)
    TreeNode* root5 = new TreeNode(10);
    root5->left = new TreeNode(5);
    root5->left->right = new TreeNode(15); // >10, invalid
    root5->right = new TreeNode(20);
    assert(isValidBSTOrder(root5) == false);
    
    // Test 8: Right child exactly equal to root (violates strict)
    TreeNode* root6 = new TreeNode(1);
    root6->right = new TreeNode(1);
    assert(isValidBSTOrder(root6) == false);
    
    // Clean up (optional but good practice)
    // For brevity, not deleting all nodes; in a real test, use smart pointers or delete.
    
    return 0;
}
