// Write a standalone C++ function that takes a pointer to the root of a binary search tree (BST) where each node has `data`, `left`, `right`, and `parent` pointers, and returns a vector of integers containing the in-order traversal of the tree. The function must handle the case where the root pointer is `nullptr` (empty tree), and must not modify the tree. The BST property is guaranteed, but the tree may be unbalanced. Implement the function iteratively using the successor/predecessor logic (or a stack) to avoid recursion, and ensure it works correctly for trees with any number of nodes. The function signature should be `std::vector<int> inorderTraversal(const BinaryNode* root)`, where `BinaryNode` is a struct/class with the described members. Assume the `data` values are unique and non-negative for simplicity.
The solution uses an iterative in-order traversal that avoids recursion (which could overflow for deep trees). The algorithm starts from the leftmost node of the tree and repeatedly finds the next node using the successor logic: if the current node has a right child, go to the leftmost node of that right subtree; otherwise, climb up the parent chain while we are the right child, stopping when we are a left child or the parent is null. This visits every node exactly once in sorted order. Edge cases: empty tree returns an empty vector; a single node returns that node; a left-skewed or right-skewed tree works because the successor logic handles both cases. Time complexity is O(n) for n nodes because each node is visited once and the upward climbs are amortized O(1) per node (each edge is traversed at most twice). Space complexity is O(1) auxiliary, excluding the output vector; no recursion stack is used. The function is `const`-correct because it takes a `const BinaryNode*` and does not modify the tree.
#include <vector>

// Binary tree node definition (matches the problem's structure)
struct BinaryNode {
    int data;
    BinaryNode* left;
    BinaryNode* right;
    BinaryNode* parent;

    BinaryNode(int x) : data(x), left(nullptr), right(nullptr), parent(nullptr) {}
};

// Perform iterative in-order traversal of a BST and return the values in a vector.
std::vector<int> inorderTraversal(const BinaryNode* root) {
    std::vector<int> result;
    if (!root) {
        return result;
    }

    // Find the leftmost node to start the traversal.
    const BinaryNode* current = root;
    while (current->left) {
        current = current->left;
    }

    // Traverse using successor links iteratively.
    while (current) {
        result.push_back(current->data);

        // If there is a right child, go to the leftmost node of that subtree.
        if (current->right) {
            current = current->right;
            while (current->left) {
                current = current->left;
            }
        } else {
            // Otherwise, climb up until we are a left child or reach the root.
            const BinaryNode* next = current->parent;
            while (next && current == next->right) {
                current = next;
                next = next->parent;
            }
            current = next;
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// Include the solution here or link it.

int main() {
    // Empty tree
    assert(inorderTraversal(nullptr).empty());

    // Single node
    BinaryNode* single = new BinaryNode(10);
    assert(inorderTraversal(single) == std::vector<int>({10}));
    delete single;

    // Build the BST from the snippet: 3,5,7,10,12,15,18
    BinaryNode* root = new BinaryNode(10);
    BinaryNode* n5 = new BinaryNode(5);
    BinaryNode* n15 = new BinaryNode(15);
    BinaryNode* n3 = new BinaryNode(3);
    BinaryNode* n7 = new BinaryNode(7);
    BinaryNode* n12 = new BinaryNode(12);
    BinaryNode* n18 = new BinaryNode(18);

    // Manually link as per the snippet's insert operations
    root->left = n5; n5->parent = root;
    root->right = n15; n15->parent = root;
    n5->left = n3; n3->parent = n5;
    n5->right = n7; n7->parent = n5;
    n15->left = n12; n12->parent = n15;
    n15->right = n18; n18->parent = n15;

    std::vector<int> expected = {3, 5, 7, 10, 12, 15, 18};
    assert(inorderTraversal(root) == expected);

    // Left-skewed tree: 20,10,5,1
    BinaryNode* leftRoot = new BinaryNode(20);
    leftRoot->left = new BinaryNode(10); leftRoot->left->parent = leftRoot;
    leftRoot->left->left = new BinaryNode(5); leftRoot->left->left->parent = leftRoot->left;
    leftRoot->left->left->left = new BinaryNode(1); leftRoot->left->left->left->parent = leftRoot->left->left;
    assert(inorderTraversal(leftRoot) == std::vector<int>({1, 5, 10, 20}));
    // Cleanup (not strictly necessary for the test but good practice)
    // In a real test you'd delete all nodes; here omitted for brevity.

    // Right-skewed tree: 1,2,3,4
    BinaryNode* rightRoot = new BinaryNode(1);
    rightRoot->right = new BinaryNode(2); rightRoot->right->parent = rightRoot;
    rightRoot->right->right = new BinaryNode(3); rightRoot->right->right->parent = rightRoot->right;
    rightRoot->right->right->right = new BinaryNode(4); rightRoot->right->right->right->parent = rightRoot->right->right;
    assert(inorderTraversal(rightRoot) == std::vector<int>({1, 2, 3, 4}));

    return 0;
}
