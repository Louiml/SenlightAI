/*
Implement a C++ function `bool isBalancedAVL(const std::vector<int>& values)` that takes a vector of integers and returns `true` if inserting the values in the given order into an initially empty AVL tree (as defined by the provided code’s `AVL` class) results in a correct AVL tree at every step, and `false` otherwise. The function should simulate the insertion process exactly as the provided `AVL` class does (including its specific rotation logic) and verify that after every insertion the tree is balanced (balance factor in {-1,0,1} for every node) and satisfies the BST property (left subtree values < node value ≤ right subtree values). This is a self-contained task; you cannot use the provided `AVL` class directly, but you must replicate its behavior faithfully—including the `maintainAVL` logic—in your own implementation.
*/

#include <vector>
#include <algorithm>

struct AVLNode {
    int value;
    int height;
    AVLNode* left;
    AVLNode* right;
    AVLNode* parent;
    AVLNode(int v) : value(v), height(0), left(nullptr), right(nullptr), parent(nullptr) {}
};

// Helper: get height of node, treating null as -1
int getHeight(AVLNode* n) {
    return n ? n->height : -1;
}

// Update height of a node based on children heights
void updateHeightNode(AVLNode* n) {
    if (!n) return;
    n->height = 1 + std::max(getHeight(n->left), getHeight(n->right));
}

// Update heights bottom-up from a node to root
void updateHeightsToRoot(AVLNode* n) {
    while (n) {
        updateHeightNode(n);
        n = n->parent;
    }
}

// Left rotation
AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    AVLNode* p = x->parent;

    y->left = x;
    x->right = T2;
    if (T2) T2->parent = x;
    x->parent = y;
    y->parent = p;
    if (p) {
        if (p->left == x) p->left = y;
        else p->right = y;
    }
    updateHeightNode(x);
    updateHeightNode(y);
    return y;
}

// Right rotation
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    AVLNode* p = y->parent;

    x->right = y;
    y->left = T2;
    if (T2) T2->parent = y;
    y->parent = x;
    x->parent = p;
    if (p) {
        if (p->left == y) p->left = x;
        else p->right = x;
    }
    updateHeightNode(y);
    updateHeightNode(x);
    return x;
}

// Adjust balance after insertion, mimicking the original maintainAVL logic
void maintainAVL(AVLNode* inserted, AVLNode*& root) {
    AVLNode* tempu = inserted;
    while (tempu) {
        AVLNode* temp = tempu;
        // Compute balance factor using stored heights, null as -1
        int leftH = getHeight(temp->left);
        int rightH = getHeight(temp->right);
        int balance = leftH - rightH;

        if (balance < -1) {
            // Right-heavy case
            AVLNode* child = temp->right;
            int childLeftH = getHeight(child->left);
            int childRightH = getHeight(child->right);
            if (childLeftH > childRightH) {
                // Right-left rotation
                child = rightRotate(child);
                // After rotation, child's parent pointer updates; we need to reconnect to temp
                // But rightRotate returns new subtree root, child is now temp's right child
                temp->right = child;
                child->parent = temp;
                leftRotate(temp);
            } else {
                leftRotate(temp);
            }
        } else if (balance > 1) {
            // Left-heavy case
            AVLNode* child = temp->left;
            int childLeftH = getHeight(child->left);
            int childRightH = getHeight(child->right);
            if (childLeftH < childRightH) {
                // Left-right rotation
                child = leftRotate(child);
                temp->left = child;
                child->parent = temp;
                rightRotate(temp);
            } else {
                rightRotate(temp);
            }
        }
        // After rotations, root may have changed; update root pointer if needed
        while (root->parent) root = root->parent;
        tempu = tempu->parent;
    }
}

// Insert value into BST, then rebalance; returns new root
AVLNode* insertNode(AVLNode* root, int val) {
    if (!root) {
        return new AVLNode(val);
    }
    AVLNode* curr = root;
    AVLNode* parent = nullptr;
    while (curr) {
        parent = curr;
        if (val < curr->value) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    AVLNode* newNode = new AVLNode(val);
    newNode->parent = parent;
    if (val < parent->value) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
    // Update heights for all nodes from parent up to root
    updateHeightsToRoot(parent);
    // Rebalance from the new node upward
    maintainAVL(newNode, root);
    // Ensure root is correct (could have been rotated)
    while (root->parent) root = root->parent;
    return root;
}

// Check BST ordering via in-order traversal
bool isBST(AVLNode* root, int& prev, bool& first) {
    if (!root) return true;
    if (!isBST(root->left, prev, first)) return false;
    if (!first && root->value < prev) return false;
    prev = root->value;
    first = false;
    return isBST(root->right, prev, first);
}

// Check AVL balance condition: every node's height correct and balance factor in {-1,0,1}
bool isBalancedAndHeightsCorrect(AVLNode* root) {
    if (!root) return true;
    if (!isBalancedAndHeightsCorrect(root->left)) return false;
    if (!isBalancedAndHeightsCorrect(root->right)) return false;
    int expectedHeight = 1 + std::max(getHeight(root->left), getHeight(root->right));
    if (root->height != expectedHeight) return false;
    int balance = getHeight(root->left) - getHeight(root->right);
    return balance >= -1 && balance <= 1;
}

// Main function: insert values in order and check AVL validity after each insertion
bool isBalancedAVL(const std::vector<int>& values) {
    AVLNode* root = nullptr;
    for (int v : values) {
        root = insertNode(root, v);
        // Validate after each insertion
        if (root == nullptr) return false;
        if (!isBalancedAndHeightsCorrect(root)) return false;
        int prev = 0;
        bool first = true;
        if (!isBST(root, prev, first)) return false;
    }
    // Clean up (optional, but for completeness not required for the test)
    return true;
}

#include <cassert>
#include <vector>

// Assume the solution function is declared as above
int main() {
    // Basic balanced sequences
    assert(isBalancedAVL({1,2,3}) == true);    // single left-right => double rotation
    assert(isBalancedAVL({3,2,1}) == true);    // right-left rotation
    assert(isBalancedAVL({1,2,3,4,5,6,7,8}) == true); // same as snippet input
    assert(isBalancedAVL({}) == true);         // empty input valid
    assert(isBalancedAVL({5}) == true);        // single node
    
    // Duplicates allowed (insert to right)
    assert(isBalancedAVL({1,1,1}) == true);
    
    // Unbalanced sequences? The given algorithm always balances, so all should be true
    assert(isBalancedAVL({10,20,30,40,50}) == true);
    assert(isBalancedAVL({50,40,30,20,10}) == true);
    assert(isBalancedAVL({5,2,8,1,3,7,9}) == true);
    
    // Larger random-like sequence
    assert(isBalancedAVL({15,33,10,23,41,7,52,18,30,45}) == true);
    
    return 0;
}

// The core challenge is to faithfully reproduce the AVL insertion and rebalancing algorithm from the snippet, then validate the invariant after each insertion. The approach: define a `struct Node` with `left`, `right`, `parent`, `height`, and `value`. Implement helper functions: `createNode`, `updateHeight` (post-order height update), `leftRotate`, `rightRotate`, and `maintainAVL` that walks up from the newly inserted node, computes a balance factor (using stored heights, not recomputing recursively as the commented-out version does), and performs the appropriate single or double rotation exactly as the snippet does. After each `insert`, check: (1) the BST ordering by doing an in-order traversal and verifying sorted order, and (2) every node’s height equals `1 + max(leftHeight, rightHeight)` (where null height is -1) and balance factor is -1, 0, or 1. Edge cases: empty input (should return true because no invalid state), duplicates (allowed, insert to right), single-node tree, and sequences that trigger both single and double rotations (e.g., left-right and right-left cases). Time complexity: each insert is O(log n) on average but worst-case O(n) because the provided code’s `updateHeight` is called on the entire tree after each insertion, and `maintainAVL` may traverse up to the root; overall O(k*n) for k insertions, which is fine for moderate sizes. Space: O(n) for tree nodes.
