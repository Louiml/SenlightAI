/*
Write a C++ function `analyzeBinaryTree` that takes a binary tree node pointer and computes the following four statistics in one traversal: the total number of nodes, the sum of all node data values, the height of the tree (defined as the number of nodes on the longest path from root to a leaf, with a single-node tree having height 1), and the diameter of the tree (defined as the number of nodes on the longest path between any two nodes in the tree, which may or may not pass through the root). The function should return these four values in a `struct` or `std::tuple`. The tree is defined with a `Node` struct that has `int data`, `Node* left`, `Node* right`, and a constructor accepting an integer value. The function must handle an empty tree (null root) by returning all zeros. All node data values are non-negative integers, and the tree may be degenerate (a linked list) or balanced. The function should be efficient, computing all four statistics in a single depth-first traversal without additional tree modifications.
*/

#include <algorithm>
#include <cstddef>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

struct TreeStats {
    int nodeCount;
    int sum;
    int height;
    int diameter;
};

TreeStats analyzeBinaryTree(const Node* root) {
    if (root == nullptr) {
        return {0, 0, 0, 0};
    }
    TreeStats left = analyzeBinaryTree(root->left);
    TreeStats right = analyzeBinaryTree(root->right);
    TreeStats current;
    current.nodeCount = left.nodeCount + right.nodeCount + 1;
    current.sum = left.sum + right.sum + root->data;
    current.height = std::max(left.height, right.height) + 1;
    current.diameter = std::max({left.diameter, right.diameter, left.height + right.height + 1});
    return current;
}

#include <cassert>

int main() {
    // Test 1: Empty tree
    TreeStats empty = analyzeBinaryTree(nullptr);
    assert(empty.nodeCount == 0);
    assert(empty.sum == 0);
    assert(empty.height == 0);
    assert(empty.diameter == 0);

    // Test 2: Single node tree
    Node* single = new Node(42);
    TreeStats singleStats = analyzeBinaryTree(single);
    assert(singleStats.nodeCount == 1);
    assert(singleStats.sum == 42);
    assert(singleStats.height == 1);
    assert(singleStats.diameter == 1);

    // Test 3: Full binary tree (like the snippet: 1-2-3 with 4,5,6,7 leaves)
    Node* root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->left->right = new Node(5);
    root->right->left = new Node(6);
    root->right->right = new Node(7);
    TreeStats full = analyzeBinaryTree(root);
    assert(full.nodeCount == 7);
    assert(full.sum == 28);
    assert(full.height == 3);
    assert(full.diameter == 5); // path 4-2-1-3-7 (or similar)

    // Test 4: Degenerate left-skewed tree (linked list)
    Node* skew = new Node(10);
    skew->left = new Node(20);
    skew->left->left = new Node(30);
    skew->left->left->left = new Node(40);
    TreeStats skewStats = analyzeBinaryTree(skew);
    assert(skewStats.nodeCount == 4);
    assert(skewStats.sum == 100);
    assert(skewStats.height == 4);
    assert(skewStats.diameter == 4); // path from root to deepest leaf

    // Test 5: Tree with only right children
    Node* rightOnly = new Node(5);
    rightOnly->right = new Node(15);
    rightOnly->right->right = new Node(25);
    TreeStats rightStats = analyzeBinaryTree(rightOnly);
    assert(rightStats.nodeCount == 3);
    assert(rightStats.sum == 45);
    assert(rightStats.height == 3);
    assert(rightStats.diameter == 3);

    // Cleanup (not necessary for assert but good practice)
    delete single;
    delete root->left->left;
    delete root->left->right;
    delete root->right->left;
    delete root->right->right;
    delete root->left;
    delete root->right;
    delete root;
    delete skew->left->left->left;
    delete skew->left->left;
    delete skew->left;
    delete skew;
    delete rightOnly->right->right;
    delete rightOnly->right;
    delete rightOnly;

    return 0;
}

// The solution requires a single recursive traversal that simultaneously computes all four metrics. We define a helper struct `TreeStats` with fields `nodeCount`, `sum`, `height`, and `diameter`. For a null root, we return `{0,0,0,0}`. For a non-null node, we recursively compute left and right subtree stats. The current node's `nodeCount` is left.nodeCount + right.nodeCount + 1; `sum` is left.sum + right.sum + node->data; `height` is max(left.height, right.height) + 1; and `diameter` is max(left.diameter, right.diameter, left.height + right.height + 1). The key insight is that the diameter through the current node is the height of left subtree plus the height of right subtree plus one, because the longest path through this node goes from the deepest leaf in the left subtree to the deepest leaf in the right subtree, passing through the node itself. The overall diameter is the maximum of the left and right diameters and the path through the current node. This single post-order traversal ensures each node is visited once. Edge cases: empty tree (all zeros), single node (count=1, sum=data, height=1, diameter=1 since the path from a node to itself is 1 node), degenerate left-only tree where heights are large but right subtrees are null. Time complexity is O(n) where n is the number of nodes, and space complexity is O(h) for the recursion stack, with h being the tree height (worst-case O(n) for a degenerate tree). The function is const-correct by taking `const Node* root` since it only reads, not modifies, the tree.
