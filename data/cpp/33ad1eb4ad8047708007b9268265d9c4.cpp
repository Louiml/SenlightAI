Implement a C++ function `int getRootAfterInsertions(const std::vector<int>& values)` that builds an AVL tree (self-balancing binary search tree) by inserting all values from the input vector in the given order, and returns the value stored at the root of the final tree. The tree must maintain the AVL balance property after every insertion, using the standard rotations (single left, single right, double left-right, double right-left) described in the provided snippet. The function must handle duplicate values by inserting them into the right subtree (as in the snippet’s `else` branch). The input vector is non-empty, and all values are integers within the range of `int`. The function should not modify the input vector.

The solution replicates the AVL insertion logic from the snippet. Each inserted node starts with height 1. After every recursive insertion, the height of the current node is updated as `1 + max(height(left), height(right))`, and the balance factor is computed as `height(left) - height(right)`. If an imbalance occurs (absolute balance factor 2), the appropriate rotation is applied: for a left imbalance (factor +2), if the left child has factor +1, do a single right rotation; if the left child has factor -1, do a left rotation on the left child followed by a right rotation on the current node. For a right imbalance (factor -2), symmetric rotations apply. A node is represented as a struct with `value`, `height`, and pointers to left and right children. The root pointer is passed by reference during insertions so rotations update the actual root. The algorithm runs in O(n log n) time for n insertions (each insertion is O(log n) due to balancing) and O(n) space for the tree. Edge cases include duplicate values (inserted to the right), a single element (the root is that element), and sequences that trigger all four rotation types. After all insertions, return `root->value`.

#include <vector>
#include <algorithm>

struct AVLNode {
    int value;
    int height;
    AVLNode* left;
    AVLNode* right;
    AVLNode(int v) : value(v), height(1), left(nullptr), right(nullptr) {}
};

int getHeight(AVLNode* node) {
    return node ? node->height : 0;
}

void updateHeight(AVLNode* node) {
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
}

int getBalanceFactor(AVLNode* node) {
    return getHeight(node->left) - getHeight(node->right);
}

void rotateLeft(AVLNode*& node) {
    AVLNode* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;
    updateHeight(node);
    updateHeight(newRoot);
    node = newRoot;
}

void rotateRight(AVLNode*& node) {
    AVLNode* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;
    updateHeight(node);
    updateHeight(newRoot);
    node = newRoot;
}

void insertNode(AVLNode*& node, int value) {
    if (!node) {
        node = new AVLNode(value);
        return;
    }
    if (value < node->value) {
        insertNode(node->left, value);
        updateHeight(node);
        if (getBalanceFactor(node) == 2) {
            if (getBalanceFactor(node->left) == 1) {
                rotateRight(node);
            } else if (getBalanceFactor(node->left) == -1) {
                rotateLeft(node->left);
                rotateRight(node);
            }
        }
    } else {
        insertNode(node->right, value);
        updateHeight(node);
        if (getBalanceFactor(node) == -2) {
            if (getBalanceFactor(node->right) == -1) {
                rotateLeft(node);
            } else if (getBalanceFactor(node->right) == 1) {
                rotateRight(node->right);
                rotateLeft(node);
            }
        }
    }
}

// Build an AVL tree from the given values and return the value at the root.
int getRootAfterInsertions(const std::vector<int>& values) {
    AVLNode* root = nullptr;
    for (int v : values) {
        insertNode(root, v);
    }
    int result = root->value;
    // Clean up memory (not strictly required but good practice)
    // For simplicity, we omit deletion here; in a real program use a destructor or delete recursively.
    return result;
}

#include <cassert>
#include <vector>

// The solution function is declared here (or included from the header if separate).
// For this test, we assume the function is defined above.
int main() {
    // Single element
    assert(getRootAfterInsertions({42}) == 42);
    // Simple increasing sequence: LL rotation needed
    assert(getRootAfterInsertions({1, 2, 3}) == 2);
    // Simple decreasing sequence: RR rotation needed
    assert(getRootAfterInsertions({3, 2, 1}) == 2);
    // LR rotation: left child has right heavy
    assert(getRootAfterInsertions({3, 1, 2}) == 2);
    // RL rotation: right child has left heavy
    assert(getRootAfterInsertions({1, 3, 2}) == 2);
    // Duplicate values: goes to right subtree, no imbalance if equal
    assert(getRootAfterInsertions({5, 5, 5}) == 5);
    // Larger mixed sequence, root should be a median
    assert(getRootAfterInsertions({10, 20, 30, 40, 50, 25}) == 30);
    // Negative values
    assert(getRootAfterInsertions({-10, -20, -5}) == -10);
    // Many values, check root is still correct via known AVL property (value of root is median-ish)
    std::vector<int> many = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    assert(getRootAfterInsertions(many) == 8);
    return 0;
}
