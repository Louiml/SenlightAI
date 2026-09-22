Write a C++ function `TreeNode* balanceBST(TreeNode* root)` that takes the root of a binary search tree (BST) and returns the root of a new balanced BST containing the same values. The balanced BST must have the property that for every node, the heights of its left and right subtrees differ by at most one (i.e., it is height-balanced). The function should reconstruct the tree from the sorted sequence of its nodes without modifying the input tree. You may assume the input tree is a valid BST with distinct integer values, and the number of nodes is between 1 and 10^4.
#include <cassert>
#include <vector>
#include <queue>

// Helper to check if a tree is height-balanced and returns its height.
int checkBalanced(TreeNode* node, bool& balanced) {
    if (node == nullptr) return 0;
    int leftH = checkBalanced(node->left, balanced);
    int rightH = checkBalanced(node->right, balanced);
    if (std::abs(leftH - rightH) > 1) balanced = false;
    return 1 + std::max(leftH, rightH);
}

// Helper to collect in-order values from a tree.
void inorderValues(TreeNode* node, std::vector<int>& out) {
    if (node == nullptr) return;
    inorderValues(node->left, out);
    out.push_back(node->val);
    inorderValues(node->right, out);
}

// Helper to delete tree nodes (to avoid memory leaks in tests).
void deleteTree(TreeNode* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

int main() {
    // Test 1: Already balanced tree (single node)
    TreeNode* t1 = new TreeNode(1);
    TreeNode* b1 = balanceBST(t1);
    std::vector<int> v1;
    inorderValues(b1, v1);
    assert(v1.size() == 1 && v1[0] == 1);
    bool ok1 = true;
    checkBalanced(b1, ok1);
    assert(ok1);
    deleteTree(t1);
    deleteTree(b1);

    // Test 2: Skewed tree (1->2->3->4)
    TreeNode* t2 = new TreeNode(1, nullptr, new TreeNode(2, nullptr, new TreeNode(3, nullptr, new TreeNode(4))));
    TreeNode* b2 = balanceBST(t2);
    std::vector<int> v2;
    inorderValues(b2, v2);
    assert(v2.size() == 4);
    for (int i = 0; i < 4; ++i) assert(v2[i] == i + 1);
    bool ok2 = true;
    checkBalanced(b2, ok2);
    assert(ok2);
    deleteTree(t2);
    deleteTree(b2);

    // Test 3: Left-skewed tree (4->3->2->1)
    TreeNode* t3 = new TreeNode(4, new TreeNode(3, new TreeNode(2, new TreeNode(1), nullptr), nullptr), nullptr);
    TreeNode* b3 = balanceBST(t3);
    std::vector<int> v3;
    inorderValues(b3, v3);
    assert(v3.size() == 4);
    for (int i = 0; i < 4; ++i) assert(v3[i] == i + 1);
    bool ok3 = true;
    checkBalanced(b3, ok3);
    assert(ok3);
    deleteTree(t3);
    deleteTree(b3);

    // Test 4: Unbalanced tree with 7 nodes (like a chain with a branch)
    TreeNode* t4 = new TreeNode(5);
    t4->left = new TreeNode(3);
    t4->left->right = new TreeNode(4);
    t4->left->left = new TreeNode(2);
    t4->left->left->left = new TreeNode(1);
    t4->right = new TreeNode(7);
    t4->right->right = new TreeNode(8);
    TreeNode* b4 = balanceBST(t4);
    std::vector<int> v4;
    inorderValues(b4, v4);
    std::vector<int> expected = {1,2,3,4,5,7,8};
    assert(v4 == expected);
    bool ok4 = true;
    checkBalanced(b4, ok4);
    assert(ok4);
    deleteTree(t4);
    deleteTree(b4);

    // Test 5: Empty tree (should return nullptr)
    TreeNode* b5 = balanceBST(nullptr);
    assert(b5 == nullptr);

    return 0;
}
#include <vector>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper: in-order traversal to collect node values in sorted order.
void collectInorder(TreeNode* root, std::vector<int>& values) {
    if (root == nullptr) return;
    collectInorder(root->left, values);
    values.push_back(root->val);
    collectInorder(root->right, values);
}

// Helper: recursively build a balanced BST from a sorted vector slice [start, end].
TreeNode* buildBalanced(const std::vector<int>& values, int start, int end) {
    if (start > end) return nullptr;
    int mid = start + (end - start) / 2;
    TreeNode* node = new TreeNode(values[mid]);
    node->left = buildBalanced(values, start, mid - 1);
    node->right = buildBalanced(values, mid + 1, end);
    return node;
}

// Main function: balance a given BST and return the new balanced root.
TreeNode* balanceBST(TreeNode* root) {
    if (root == nullptr) return nullptr;
    std::vector<int> values;
    collectInorder(root, values);
    return buildBalanced(values, 0, static_cast<int>(values.size()) - 1);
}
// The main algorithm is straightforward: first perform an in-order traversal of the given BST to collect all node values into a sorted vector. Since the input is a valid BST, an in-order traversal yields the values in non-decreasing order. Then, recursively build a height‑balanced BST from the sorted vector by choosing the middle element as the root for the current subarray (using `mid = start + (end - start) / 2` to avoid overflow). The left half becomes the left subtree, and the right half becomes the right subtree. The recursion terminates when the subarray is empty (start > end) or contains a single element (start == end). Key edge cases: an empty tree (though the task guarantees non‑empty, the function should handle it gracefully), a tree with only one node (directly returns a new node with the same value), and trees where the middle element is not exactly the median due to even counts (any middle selection works as long as it keeps the tree balanced). Time complexity: O(n) for the traversal and O(n) for building the tree, overall O(n) since each node is visited exactly once in each phase. Space complexity: O(n) for the vector and O(log n) recursion depth for building the tree (plus O(n) recursion depth for the traversal in the worst case if the original tree is skewed, but the problem's typical constraints allow this; we can note that the auxiliary vector dominates the extra space, making it O(n) overall).
