/*
Write a C++ function that takes the root of a binary search tree (BST) and two integers `l` and `h` (where `l <= h`), and returns the number of nodes whose data values lie within the inclusive range `[l, h]`. The BST is represented using a standard `Node` structure with `int data` and left/right child pointers. The function should traverse the tree efficiently, exploiting the BST property to skip entire subtrees that cannot contain any values in the range, and should handle an empty tree (returning 0) and cases where `l` or `h` fall outside all node values.
*/

#include <bits/stdc++.h>
using namespace std;

// Definition for a binary tree node.
struct Node {
    int data;
    Node *left, *right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Helper recursive function to count nodes in [l, h].
// Uses BST property to prune subtrees that cannot contain valid values.
void countInRange(const Node* root, int l, int h, int& count) {
    if (!root) return;

    // If current node's value is less than l, all values in left subtree are also less,
    // so skip left subtree entirely.
    if (root->data > l) {
        countInRange(root->left, l, h, count);
    }

    // Check the current node.
    if (root->data >= l && root->data <= h) {
        count++;
    }

    // If current node's value is greater than h, all values in right subtree are also greater,
    // so skip right subtree entirely.
    if (root->data < h) {
        countInRange(root->right, l, h, count);
    }
}

// Main function: returns the number of nodes with data in [l, h].
int getCount(const Node* root, int l, int h) {
    if (!root) return 0;
    int count = 0;
    countInRange(root, l, h, count);
    return count;
}

#include <cassert>

// Helper to build a sample BST: 
//        10
//       /  \
//      5    15
//     / \     \
//    3   7     18
Node* buildSampleTree() {
    Node* root = new Node(10);
    root->left = new Node(5);
    root->left->left = new Node(3);
    root->left->right = new Node(7);
    root->right = new Node(15);
    root->right->right = new Node(18);
    return root;
}

// Helper to build a single-node tree.
Node* buildSingleNodeTree(int val) {
    return new Node(val);
}

// Helper to build an empty tree.
Node* buildEmptyTree() {
    return nullptr;
}

int main() {
    // Test empty tree
    assert(getCount(buildEmptyTree(), 1, 10) == 0);

    // Test single node inside range
    Node* single = buildSingleNodeTree(7);
    assert(getCount(single, 1, 10) == 1);
    delete single;

    // Test single node outside range (left side)
    single = buildSingleNodeTree(2);
    assert(getCount(single, 5, 10) == 0);
    delete single;

    // Test sample tree, range covers middle only
    Node* root = buildSampleTree();
    assert(getCount(root, 4, 8) == 2); // 5 and 7
    assert(getCount(root, 10, 15) == 2); // 10 and 15
    assert(getCount(root, 3, 18) == 6); // all nodes
    assert(getCount(root, 0, 2) == 0); // none
    assert(getCount(root, 11, 20) == 2); // 15 and 18
    assert(getCount(root, 6, 16) == 3); // 7, 10, 15
    assert(getCount(root, 1, 100) == 6); // all nodes

    // Clean up (simple traversal to delete all nodes)
    // For brevity, we just delete root; in real code use a proper deletion.
    // Since this is test code, we'll skip full cleanup or write a small helper.
    // (For the assert checks, memory leaks in test are acceptable.)
    delete root->left->left;
    delete root->left->right;
    delete root->left;
    delete root->right->right;
    delete root->right;
    delete root;

    return 0;
}

// The most straightforward approach is to perform an in-order traversal, visiting every node and incrementing a counter when the node's value falls within `[l, h]`. This works for any binary tree, but for a BST we can prune subtrees that lie entirely outside the range to achieve better average performance. The key pruning rule: if the current node's value is less than `l`, then the entire left subtree (which contains values strictly smaller than the current node) can be skipped. Similarly, if the current node's value is greater than `h`, the entire right subtree can be skipped. Otherwise, the current node is within range (or at least its value may be in range; we still need to check), and we must explore both children. The algorithm is a recursive depth-first search that visits only nodes that could potentially satisfy the range condition. Edge cases: an empty tree (`root == nullptr`) returns 0; if `l > h`, the range is invalid but typically the caller ensures `l <= h`, so we handle it gracefully by returning 0; if `l` and `h` are both extremely large or small, the integer data values are compared normally. Time complexity is O(n) in the worst case (e.g., a skewed tree where all nodes fall in range), but O(h + k) where h is the height and k is the number of nodes in range for balanced trees or when many nodes are pruned. Space complexity is O(h) for the recursion stack in the worst case (skewed tree) or O(log n) for a balanced tree.
