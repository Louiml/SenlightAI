/*
Write a C++ function that takes the root of a binary tree (defined below) and returns a `std::vector<int>` containing the left view of the tree, i.e., the values of the nodes that are visible when the tree is viewed from the left side, ordered from top to bottom. The left view consists of the leftmost node at each depth level. You may assume the tree contains non-negative integers. If the tree is empty, return an empty vector. Implement the function with an iterative or recursive approach, and ensure it works for skewed, balanced, and degenerate trees.
*/
#include <vector>

// Definition for a binary tree node.
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Helper function to perform recursive traversal and collect left view.
void collectLeftView(Node* node, int level, std::vector<int>& result) {
    if (node == nullptr) {
        return;
    }
    // If this is the first node encountered at this level, add it.
    if (result.size() == static_cast<size_t>(level)) {
        result.push_back(node->data);
    }
    // Traverse left first, then right.
    collectLeftView(node->left, level + 1, result);
    collectLeftView(node->right, level + 1, result);
}

// Return the left view of the binary tree as a vector.
std::vector<int> leftView(const Node* root) {
    std::vector<int> result;
    // The function modifies the vector but not the tree, so we use const_cast carefully.
    // Alternatively, we can create a non-const helper, but since we don't modify the tree,
    // we can keep the const interface and use a helper that takes a non-const pointer.
    // For simplicity, we cast away constness internally (safe because we never modify the tree).
    collectLeftView(const_cast<Node*>(root), 0, result);
    return result;
}
#include <cassert>
#include <vector>

// Node and leftView function as defined above.

int main() {
    // Test 1: Empty tree
    Node* empty = nullptr;
    assert(leftView(empty).empty());

    // Test 2: Single node
    Node* single = new Node(5);
    assert(leftView(single) == std::vector<int>{5});

    // Test 3: Left-skewed tree: 1 -> 2 -> 3 (all left children)
    Node* leftSkewed = new Node(1);
    leftSkewed->left = new Node(2);
    leftSkewed->left->left = new Node(3);
    assert(leftView(leftSkewed) == std::vector<int>({1, 2, 3}));

    // Test 4: Right-skewed tree: 1 -> 2 -> 3 (all right children)
    Node* rightSkewed = new Node(1);
    rightSkewed->right = new Node(2);
    rightSkewed->right->right = new Node(3);
    assert(leftView(rightSkewed) == std::vector<int>({1}));

    // Test 5: Balanced tree:
    //        1
    //       / \
    //      2   3
    //     / \   \
    //    4   5   6
    Node* balanced = new Node(1);
    balanced->left = new Node(2);
    balanced->right = new Node(3);
    balanced->left->left = new Node(4);
    balanced->left->right = new Node(5);
    balanced->right->right = new Node(6);
    assert(leftView(balanced) == std::vector<int>({1, 2, 4}));

    // Test 6: Tree where leftmost nodes are not all from left subtree:
    //        1
    //       / \
    //      2   3
    //           \
    //            4
    // Expected: 1, 2, 4 (at level 2, the leftmost is 4 because 2 has no children at that level)
    Node* mixed = new Node(1);
    mixed->left = new Node(2);
    mixed->right = new Node(3);
    mixed->right->right = new Node(4);
    assert(leftView(mixed) == std::vector<int>({1, 2, 4}));

    // Cleanup (optional but good practice)
    delete leftSkewed->left->left;
    delete leftSkewed->left;
    delete leftSkewed;
    delete rightSkewed->right->right;
    delete rightSkewed->right;
    delete rightSkewed;
    delete balanced->left->left;
    delete balanced->left->right;
    delete balanced->left;
    delete balanced->right->right;
    delete balanced->right;
    delete balanced;
    delete mixed->left;
    delete mixed->right->right;
    delete mixed->right;
    delete mixed;
    delete single;

    return 0;
}
// The left view can be obtained by performing a depth-first traversal where we visit the left subtree before the right subtree. We maintain a `level` counter that starts at 0 for the root. We also maintain a result vector. At each node, if the current `level` equals the size of the result vector, it means this is the first node we've encountered at this depth, so we push its value. Because we traverse left first, this first node at each level is exactly the leftmost node. Then we recursively process the left child (level+1) and then the right child (level+1). This ensures that at each depth, the leftmost node is added before any other node at that depth. Edge cases: empty tree (return empty vector), a single node (return that node), a left-skewed tree (all nodes on the left path appear), a right-skewed tree (only the root appears because the left view only includes the leftmost at each level, and the root is the only leftmost node across all depths). Time complexity is O(n) where n is the number of nodes, because we visit every node exactly once. Space complexity is O(h) for the recursion stack in the worst case (skewed tree), plus O(d) for the result vector, where d is the number of levels (height of tree). Overall auxiliary space is O(h).
