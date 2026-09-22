// Write a C++ function that takes the root of a binary tree and removes all "half nodes" from it, where a half node is defined as a node that has exactly one child (either a left child or a right child, but not both). After removal, the tree should be restructured so that the child of a removed half node takes its place, preserving the relative order of nodes in the original tree. The function should return the root pointer of the modified tree. For example, if a node has only a left child, that left child replaces the node; similarly, if a node has only a right child, that right child replaces it. Nodes with zero or two children remain unchanged. The function must handle empty trees (returning null), leaf nodes (which are not half nodes and therefore kept), and chains of half nodes (where multiple removals may cascade). The input tree is not necessarily balanced, and node values are integers. The function signature should be `Node* removeHalfNodes(Node* root)` where `Node` is a struct with fields `int data`, `Node* left`, `Node* right`, and a constructor `Node(int val)`. The solution must modify the tree in place and avoid memory leaks by properly deallocating removed nodes.

#include <cassert>
#include <bits/stdc++.h>
using namespace std;

// struct Node and removeHalfNodes function (included from above) go here

// Helper to build a simple binary tree manually (not using string parsing)
Node* buildExampleTree() {
    // Tree structure:
    //        1
    //       / \
    //      2   3
    //     /   / \
    //    4   5   6
    //     \       /
    //      7     8
    Node* root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->right->left = new Node(5);
    root->right->right = new Node(6);
    root->left->left->right = new Node(7); // 4 has only right child (half node)
    root->right->right->left = new Node(8); // 6 has only left child (half node)
    return root;
}

// Helper to collect inorder traversal into a vector
void inorderCollect(Node* node, vector<int>& out) {
    if (node == nullptr) return;
    inorderCollect(node->left, out);
    out.push_back(node->data);
    inorderCollect(node->right, out);
}

int main() {
    // Test 1: Empty tree
    assert(removeHalfNodes(nullptr) == nullptr);

    // Test 2: Single node (leaf, not half)
    Node* single = new Node(10);
    Node* result = removeHalfNodes(single);
    vector<int> vals;
    inorderCollect(result, vals);
    assert(vals.size() == 1 && vals[0] == 10);
    delete result; // cleanup

    // Test 3: Root with only a right child (root is half node)
    Node* root3 = new Node(1);
    root3->right = new Node(2);
    Node* result3 = removeHalfNodes(root3);
    assert(result3->data == 2 && result3->left == nullptr && result3->right == nullptr);
    delete result3;

    // Test 4: Root with only a left child (root is half node)
    Node* root4 = new Node(1);
    root4->left = new Node(2);
    Node* result4 = removeHalfNodes(root4);
    assert(result4->data == 2 && result4->left == nullptr && result4->right == nullptr);
    delete result4;

    // Test 5: Chain of half nodes: 1 -> right 2 -> right 3 (all half except leaf)
    Node* chain = new Node(1);
    chain->right = new Node(2);
    chain->right->right = new Node(3);
    Node* resultChain = removeHalfNodes(chain);
    assert(resultChain->data == 3 && resultChain->left == nullptr && resultChain->right == nullptr);
    delete resultChain;

    // Test 6: Complex tree with half nodes and full nodes
    Node* complex = buildExampleTree();
    Node* resultComplex = removeHalfNodes(complex);
    vector<int> inorderResult;
    inorderCollect(resultComplex, inorderResult);
    // Original inorder before removal: 4 (with right 7) 2 1 5 3 6 (with left 8)
    // Original inorder: 4, 7, 2, 1, 5, 3, 8, 6? Let's compute: 
    // Original tree: 4 has right 7, 6 has left 8. Inorder: left subtree of 4 is none, so 4 then 7, then 2, then 1, then 5, then 3, then 8, then 6.
    // Actually inorder: 4,7,2,1,5,3,8,6.
    // After removing half nodes (4 and 6), replacement: 7 replaces 4, 8 replaces 6.
    // New inorder: 7,2,1,5,3,8 -> values are 7,2,1,5,3,8.
    vector<int> expected = {7, 2, 1, 5, 3, 8};
    assert(inorderResult == expected);
    delete resultComplex;

    // Cleanup remaining test trees
    // (All other nodes were deleted during removal or manually)

    cout << "All tests passed!" << endl;
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Remove all half nodes (nodes with exactly one child) from the binary tree.
// Returns the root of the modified tree. The tree is modified in place.
Node* removeHalfNodes(Node* root) {
    if (root == nullptr) return nullptr;

    // Post-order: process children first
    root->left = removeHalfNodes(root->left);
    root->right = removeHalfNodes(root->right);

    // Check if current node is a half node (exactly one child)
    if ((root->left == nullptr || root->right == nullptr) && 
        (root->left != nullptr || root->right != nullptr)) {
        // Node has exactly one child: replace it with that child
        Node* replacement = root->left ? root->left : root->right;
        delete root; // free memory to avoid leaks
        return replacement;
    }

    // Node is either a leaf (both children null) or has both children: keep it
    return root;
}

// The core algorithm uses a post-order traversal: recursively process the left subtree first, then the right subtree, and finally handle the current node. After both children have been processed, the current node may have been replaced by its only child (if it was a half node), so we need to check the current node's state after the recursive calls. Specifically, after recursing on left and right, the function examines the current `root`. If `root` has exactly one non-null child (i.e., one of `left` or `right` is null and the other is not), then the node is a half node and should be removed. The removal is done by storing the non-null child in a temporary pointer, deleting the current node (to free memory), and returning the child pointer to be used as the replacement in the parent's recursion. If the node is not a half node (either both children are null or both are non-null), it is returned unchanged. The process naturally handles cascading removals because after the child replaces a half node, the parent's recursive call returns the new subtree root, which may itself be a half node if the original child had only one child—but since the recursion already processed that child's subtree, the child's own half nodes have been removed, and the child now has either zero or two children, so it will not be a half node anymore. Edge cases include: an empty tree (returns null); a single node (not a half node, returns it unchanged); a root that is a half node with a left or right child that is a leaf (the child replaces the root and becomes the new root); and a chain like `1->2->3` where 1 has only a right child 2, and 2 has only a right child 3—the recursion removes 2 and then removes 1, leaving 3 as the final root. The algorithm runs in O(n) time and O(h) auxiliary space (due to recursion stack), where n is the number of nodes and h is the tree height. In the worst case (skewed tree), h = n; in a balanced tree, h = log n.
