Write a C++ function `Node* constructTree(const std::vector<int>& inorder, const std::vector<int>& preorder)` that reconstructs a binary tree from its inorder and preorder traversals (assuming all node values are distinct) and returns a pointer to the root of the constructed tree. The function should use a helper that recursively builds the tree by picking the next element from preorder as the current root, locating its position in inorder to split the left and right subtrees, and using an unordered map for O(1) index lookup. The function must handle edge cases such as an empty input (return nullptr) and a tree with a single node. The node structure is defined as `struct Node { int data; Node* left; Node* right; Node(int x) : data(x), left(nullptr), right(nullptr) {} };`. You may include helper functions inside the main function if needed, but the primary interface is the function above. The implementation should be self-contained with all necessary headers and apply `const` correctness where appropriate. The function should not rely on global variables; instead, pass a mutable reference or pointer for the preorder index in the helper.

The key insight is that in preorder traversal, the first element is always the root. In inorder traversal, elements before the root’s position belong to the left subtree, and elements after belong to the right subtree. Recursively, we can pick the next preorder element as the root for each subtree, find its index in inorder (using a hash map for O(1) lookup), and split the inorder range accordingly. The algorithm works as follows: maintain an index `preIndex` that starts at 0 and increments each time we pick a root from preorder. For each recursive call, if the current inorder range is empty (`inStart > inEnd`), return nullptr. Otherwise, read `preorder[preIndex++]`, create a node with that value, find its index `inIndex` in inorder using the map, then recursively build the left subtree with range `[inStart, inIndex-1]` and the right subtree with `[inIndex+1, inEnd]`. This works because the preorder sequence naturally visits roots before children, and the inorder bounds correctly define left and right subtrees. Edge cases: empty input returns nullptr; a single node tree has both left and right as nullptr after creation; all values are assumed distinct, so the map lookup is unambiguous. Time complexity is O(n) because each node is processed once and each map lookup is O(1) on average; space complexity is O(n) for the map and O(h) for the recursion stack, where h is the tree height (worst case O(n) for skewed trees).

#include <vector>
#include <unordered_map>
#include <cstddef>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int x) : data(x), left(nullptr), right(nullptr) {}
};

// Helper function to recursively build the tree.
// inorder and preorder are the traversal arrays; inStart and inEnd define the current inorder range.
// preIndex is a reference to the current position in preorder; lookup maps inorder value to its index.
Node* buildHelper(const std::vector<int>& inorder, const std::vector<int>& preorder,
                  int inStart, int inEnd, int& preIndex,
                  const std::unordered_map<int, int>& lookup) {
    if (inStart > inEnd) {
        return nullptr;
    }
    int currRoot = preorder[preIndex++];
    Node* node = new Node(currRoot);
    // If this is a leaf node, return.
    if (inStart == inEnd) {
        return node;
    }
    int inIndex = lookup.at(currRoot);
    node->left = buildHelper(inorder, preorder, inStart, inIndex - 1, preIndex, lookup);
    node->right = buildHelper(inorder, preorder, inIndex + 1, inEnd, preIndex, lookup);
    return node;
}

// Constructs a binary tree from its inorder and preorder traversals.
// All node values must be distinct.
Node* constructTree(const std::vector<int>& inorder, const std::vector<int>& preorder) {
    int n = inorder.size();
    if (n == 0) {
        return nullptr;
    }
    std::unordered_map<int, int> lookup;
    for (int i = 0; i < n; ++i) {
        lookup[inorder[i]] = i;
    }
    int preIndex = 0;
    return buildHelper(inorder, preorder, 0, n - 1, preIndex, lookup);
}

#include <cassert>
#include <vector>

// Assume Node struct and constructTree are defined above (from the solution).
// Helper to compare two trees for equality (same structure and values).
bool sameTree(Node* a, Node* b) {
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;
    return (a->data == b->data) && sameTree(a->left, b->left) && sameTree(a->right, b->right);
}

// Helper to delete tree nodes (prevent memory leaks in tests).
void deleteTree(Node* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    // Test 1: Empty input
    assert(constructTree({}, {}) == nullptr);

    // Test 2: Single node
    Node* t2 = constructTree({5}, {5});
    assert(t2 != nullptr && t2->data == 5 && t2->left == nullptr && t2->right == nullptr);
    delete t2;

    // Test 3: Balanced tree: inorder [4,2,5,1,6,3,7], preorder [1,2,4,5,3,6,7]
    std::vector<int> in3 = {4,2,5,1,6,3,7};
    std::vector<int> pre3 = {1,2,4,5,3,6,7};
    Node* t3 = constructTree(in3, pre3);
    Node* expected3 = new Node(1);
    expected3->left = new Node(2);
    expected3->left->left = new Node(4);
    expected3->left->right = new Node(5);
    expected3->right = new Node(3);
    expected3->right->left = new Node(6);
    expected3->right->right = new Node(7);
    assert(sameTree(t3, expected3));
    deleteTree(t3);
    deleteTree(expected3);

    // Test 4: Left-skewed tree: inorder [4,3,2,1], preorder [1,2,3,4]
    std::vector<int> in4 = {4,3,2,1};
    std::vector<int> pre4 = {1,2,3,4};
    Node* t4 = constructTree(in4, pre4);
    Node* expected4 = new Node(1);
    expected4->left = new Node(2);
    expected4->left->left = new Node(3);
    expected4->left->left->left = new Node(4);
    assert(sameTree(t4, expected4));
    deleteTree(t4);
    deleteTree(expected4);

    // Test 5: Right-skewed tree: inorder [1,2,3,4], preorder [1,2,3,4]
    std::vector<int> in5 = {1,2,3,4};
    std::vector<int> pre5 = {1,2,3,4};
    Node* t5 = constructTree(in5, pre5);
    Node* expected5 = new Node(1);
    expected5->right = new Node(2);
    expected5->right->right = new Node(3);
    expected5->right->right->right = new Node(4);
    assert(sameTree(t5, expected5));
    deleteTree(t5);
    deleteTree(expected5);

    // Test 6: Tree with both children but not full: inorder [2,1,3], preorder [1,2,3]
    std::vector<int> in6 = {2,1,3};
    std::vector<int> pre6 = {1,2,3};
    Node* t6 = constructTree(in6, pre6);
    Node* expected6 = new Node(1);
    expected6->left = new Node(2);
    expected6->right = new Node(3);
    assert(sameTree(t6, expected6));
    deleteTree(t6);
    deleteTree(expected6);

    return 0;
}
