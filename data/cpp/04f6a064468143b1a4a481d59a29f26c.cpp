// Write a C++ function `int findLargestInTree(const node* root)` that, given a non-empty binary tree where each node stores an integer value, returns the maximum integer value present in the entire tree. The tree is defined by the provided `node` class (which contains `data`, `left`, and `right` pointers). The function must not modify the tree, must handle trees with only one node, and must recursively compare the current node's value with the maximums found in its left and right subtrees (if they exist). You cannot use any external libraries beyond the standard ones, and you must implement the logic manually without relying on built-in tree traversal functions.
// The solution recursively explores the entire tree. For each node, the maximum value is the largest among three candidates: the node's own `data`, the maximum of its left subtree (if it exists), and the maximum of its right subtree (if it exists). Base case: if the node is a leaf (both children are null), return its `data`. Otherwise, recursively compute the left and right subtree maxima (or use the current node's data as a fallback if a subtree is missing), then combine all three using a three-way maximum. Edge cases: single-node tree returns that node's value; skewed trees still work because recursion handles missing children by using the current node's data as a placeholder. Complexity: each node is visited exactly once, so time is O(n) where n is the number of nodes. Space complexity is O(h) due to recursion stack, where h is the tree height (up to O(n) for skewed trees). The function is `const`-correct because it does not modify the tree.
#include <algorithm>

class node {
public:
    int data;
    node* left;
    node* right;
    node(int d) : data(d), left(nullptr), right(nullptr) {}
};

// Returns the largest integer stored in the non-empty binary tree.
int findLargestInTree(const node* root) {
    // If leaf node, the maximum is itself.
    if (root->left == nullptr && root->right == nullptr) {
        return root->data;
    }

    int leftMax = root->data;
    int rightMax = root->data;

    if (root->left != nullptr) {
        leftMax = findLargestInTree(root->left);
    }

    if (root->right != nullptr) {
        rightMax = findLargestInTree(root->right);
    }

    return std::max({root->data, leftMax, rightMax});
}
#include <cassert>

int main() {
    // Test 1: Single node.
    node* single = new node(42);
    assert(findLargestInTree(single) == 42);

    // Test 2: Small tree with left larger.
    node* root2 = new node(5);
    root2->left = new node(10);
    root2->right = new node(3);
    assert(findLargestInTree(root2) == 10);

    // Test 3: Small tree with right larger.
    node* root3 = new node(1);
    root3->left = new node(2);
    root3->right = new node(9);
    root3->right->left = new node(6);
    assert(findLargestInTree(root3) == 9);

    // Test 4: Large value in left subtree.
    node* root4 = new node(7);
    root4->left = new node(7);
    root4->left->left = new node(7);
    root4->left->left->left = new node(100);
    assert(findLargestInTree(root4) == 100);

    // Test 5: Negative values.
    node* root5 = new node(-1);
    root5->left = new node(-5);
    root5->right = new node(-3);
    assert(findLargestInTree(root5) == -1);

    // Test 6: Root is the maximum.
    node* root6 = new node(20);
    root6->left = new node(10);
    root6->right = new node(15);
    root6->right->right = new node(17);
    assert(findLargestInTree(root6) == 20);

    // Test 7: Right skewed tree.
    node* root7 = new node(1);
    root7->right = new node(2);
    root7->right->right = new node(3);
    assert(findLargestInTree(root7) == 3);

    // Test 8: Left skewed tree.
    node* root8 = new node(-2);
    root8->left = new node(-4);
    root8->left->left = new node(-6);
    assert(findLargestInTree(root8) == -2);

    // Cleanup (not strictly necessary for assert tests but good practice)
    // For brevity, cleanup is omitted.

    return 0;
}
