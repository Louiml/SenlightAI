/*
Write a C++ function `std::string serialize(TreeNode* root)` that converts a binary tree into a compact string representation using preorder traversal, where each node's value is followed by a comma, and `x` represents a null child (also followed by a comma). Then write a companion function `TreeNode* deserialize(const std::string& data)` that reconstructs the original binary tree from this string. The input tree may contain negative integers, nodes with only one child, and must handle an empty tree (returning an empty string for serialize and `nullptr` for deserialize). Both functions should work for arbitrary binary trees without relying on any global state between calls.
*/

#include <string>
#include <cctype>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Recursive helper to serialize the tree in preorder.
void serializeHelper(TreeNode* node, std::string& result) {
    if (node == nullptr) {
        result += "x,";
        return;
    }
    result += std::to_string(node->val) + ",";
    serializeHelper(node->left, result);
    serializeHelper(node->right, result);
}

// Serialize a binary tree to a single string.
std::string serialize(TreeNode* root) {
    if (root == nullptr) {
        return "";
    }
    std::string result;
    serializeHelper(root, result);
    return result;
}

// Recursive helper to deserialize the tree from the string.
TreeNode* deserializeHelper(const std::string& data, int& idx) {
    // Parse an integer (handling negative and multi-digit numbers).
    int num = 0;
    bool isNegative = false;
    if (data[idx] == '-') {
        isNegative = true;
        ++idx;
    }
    while (idx < data.length() && std::isdigit(data[idx])) {
        num = num * 10 + (data[idx] - '0');
        ++idx;
    }
    TreeNode* node = new TreeNode(isNegative ? -num : num);
    ++idx; // Skip the comma after the value.

    // Build left child if not null.
    if (idx < data.length() && data[idx] != 'x') {
        node->left = deserializeHelper(data, idx);
    } else {
        idx += 2; // Skip "x,"
    }

    // Build right child if not null.
    if (idx < data.length() && data[idx] != 'x') {
        node->right = deserializeHelper(data, idx);
    } else {
        idx += 2; // Skip "x,"
    }

    return node;
}

// Deserialize a string back to a binary tree.
TreeNode* deserialize(const std::string& data) {
    if (data.empty()) {
        return nullptr;
    }
    int idx = 0;
    return deserializeHelper(data, idx);
}

#include <cassert>

int main() {
    // Test 1: Single node tree
    TreeNode* root1 = new TreeNode(5);
    std::string s1 = serialize(root1);
    TreeNode* t1 = deserialize(s1);
    assert(t1 != nullptr);
    assert(t1->val == 5);
    assert(t1->left == nullptr && t1->right == nullptr);
    delete t1;
    delete root1;

    // Test 2: Empty tree
    std::string s2 = serialize(nullptr);
    assert(s2 == "");
    TreeNode* t2 = deserialize(s2);
    assert(t2 == nullptr);

    // Test 3: Tree with negative values and both children
    TreeNode* root3 = new TreeNode(-1);
    root3->left = new TreeNode(2);
    root3->right = new TreeNode(-3);
    std::string s3 = serialize(root3);
    TreeNode* t3 = deserialize(s3);
    assert(t3 != nullptr);
    assert(t3->val == -1);
    assert(t3->left->val == 2);
    assert(t3->right->val == -3);
    assert(t3->left->left == nullptr && t3->left->right == nullptr);
    assert(t3->right->left == nullptr && t3->right->right == nullptr);
    delete t3->left;
    delete t3->right;
    delete t3;
    delete root3->left;
    delete root3->right;
    delete root3;

    // Test 4: Tree with only left child (right null)
    TreeNode* root4 = new TreeNode(10);
    root4->left = new TreeNode(20);
    std::string s4 = serialize(root4);
    TreeNode* t4 = deserialize(s4);
    assert(t4 != nullptr);
    assert(t4->val == 10);
    assert(t4->left->val == 20);
    assert(t4->left->left == nullptr && t4->left->right == nullptr);
    assert(t4->right == nullptr);
    delete t4->left;
    delete t4;
    delete root4->left;
    delete root4;

    // Test 5: Larger tree with balanced structure
    TreeNode* root5 = new TreeNode(1);
    root5->left = new TreeNode(2);
    root5->right = new TreeNode(3);
    root5->left->left = new TreeNode(4);
    root5->left->right = new TreeNode(5);
    std::string s5 = serialize(root5);
    TreeNode* t5 = deserialize(s5);
    assert(t5 != nullptr);
    assert(t5->val == 1);
    assert(t5->left->val == 2);
    assert(t5->right->val == 3);
    assert(t5->left->left->val == 4);
    assert(t5->left->right->val == 5);
    // Spot-check nulls
    assert(t5->right->left == nullptr && t5->right->right == nullptr);
    delete t5->left->left;
    delete t5->left->right;
    delete t5->left;
    delete t5->right;
    delete t5;
    delete root5->left->left;
    delete root5->left->right;
    delete root5->left;
    delete root5->right;
    delete root5;

    return 0;
}

// The solution uses a recursive preorder traversal (node, left subtree, right subtree) to serialize. Each node's value is converted to a string via `std::to_string` and appended with a comma. For a null child, we append `"x,"` to mark the position explicitly. This ensures the structure (including which side a child belongs to) can be reconstructed unambiguously. Deserialization uses a recursive approach with an index reference that advances through the string. At each step, we parse an integer (handling negative signs and multi-digit numbers), then increment past the comma. If the next character is not `x`, we recursively build the left subtree; otherwise we skip the `x` and comma. The same logic applies to the right subtree. Edge cases include an empty string (representing an empty tree), trees with only one child (which must be distinguished by the `x` markers), and negative values. The time complexity is O(n) for both serialize and deserialize, where n is the number of nodes, since each node is visited exactly once. Space complexity is O(n) due to the recursion stack depth (worst-case for a skewed tree) and the output string length.
