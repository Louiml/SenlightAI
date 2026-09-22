// Write a C++ function that, given a root pointer of a binary search tree (BST) and a new value to insert, performs an AVL-style insertion that includes balancing. The function should return the new root of the tree (which may change due to rotations). The tree nodes have fields `data`, `left`, `right`, `parent`, and `bfactor` (balance factor: height of right subtree minus height of left subtree). After insertion, perform necessary rotations (LL, RR, LR, RL) to restore the AVL property (|bfactor| ≤ 1) for all nodes. The function must handle insertion into an empty tree. For simplicity, assume all values are unique. The function should update parent pointers and balance factors correctly during rotations. The balancing logic should be based on the inserted node's path from the root, but the task is to implement a single function `insertAVL(Node* root, int value)` that creates a new node, inserts it as in a BST, then walks up from the inserted node to find the first unbalanced node and performs the correct rotation (or double rotation) based on the direction of insertion relative to that node's child. After rotation, return the new root (which might be the rotated subtree root if it becomes the tree root). For clarity, implement helper rotations as separate functions if needed, but the main solution function must be the one described. Do not include a main function in the solution; just the function and any helper functions.

// The algorithm starts with a standard BST insertion: traverse from root to find the correct leaf position, create the new node with `bfactor=0`, link it to its parent, and then update balance factors along the path back to the root. However, a simpler approach: after insertion, walk from the new node upward. For each ancestor, recompute its balance factor as the difference between heights of right and left subtrees (or approximate using existing child bfactors). When we find a node whose balance factor is 2 or -2, that is the first unbalanced node. Then determine the case: if the inserted value is in the right subtree of that node's right child, it's an LL rotation (left-left imbalance from the node's perspective, actually a single left rotation on the node's right child and then left rotation on the node – but the snippet names are confusing). Actually, standard terminology: if the imbalance is in the left subtree of the left child, perform a left rotation (called RR in the snippet?); better to define clearly: for a node X with balance factor 2 (right-heavy), if the new value is in the right subtree of X->right, perform a single left rotation on X. If it's in the left subtree of X->right, perform a double rotation (right-left). Similarly for -2. The provided snippet mislabels rotations; we should implement standard AVL rotations. We must also update parent pointers and balance factors during rotations. After rotation, update the root if the rotated subtree root becomes the tree root. Edge cases: inserting into empty tree returns new node as root. Also handle case where unbalanced node is the root itself (its parent is NULL). Time complexity: O(log n) average for insertion plus O(log n) for balancing walk, so O(log n) total. Space O(1) auxiliary (excluding recursion if iterative). The solution below uses iterative insertion and an upward walk, and helper functions for single and double rotations.

#include <cstddef>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node* parent;
    int bfactor; // height(right) - height(left)
};

// Left rotation (right child becomes parent)
Node* rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent) {
        if (x->parent->left == x) x->parent->left = y;
        else x->parent->right = y;
    }
    x->parent = y;
    y->left = x;
    // Update balance factors: after rotation, both x and y are balanced
    x->bfactor = 0;
    y->bfactor = 0;
    return y;
}

// Right rotation (left child becomes parent)
Node* rotateRight(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    if (x->right) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent) {
        if (y->parent->left == y) y->parent->left = x;
        else y->parent->right = x;
    }
    y->parent = x;
    x->right = y;
    y->bfactor = 0;
    x->bfactor = 0;
    return x;
}

// Insert a value into AVL tree and return new root
Node* insertAVL(Node* root, int value) {
    Node* newNode = new Node();
    newNode->data = value;
    newNode->left = newNode->right = newNode->parent = nullptr;
    newNode->bfactor = 0;

    if (root == nullptr) {
        return newNode;
    }

    // Standard BST insertion
    Node* cur = root;
    while (cur) {
        if (value < cur->data) {
            if (cur->left == nullptr) {
                cur->left = newNode;
                newNode->parent = cur;
                break;
            }
            cur = cur->left;
        } else {
            if (cur->right == nullptr) {
                cur->right = newNode;
                newNode->parent = cur;
                break;
            }
            cur = cur->right;
        }
    }

    // Walk up to find first unbalanced node
    Node* unbalanced = nullptr;
    Node* child = newNode;
    Node* parent = newNode->parent;
    while (parent) {
        // Compute height difference manually (simple subtree heights)
        // For simplicity, we can compute balance factor via heights
        auto height = [](Node* n) {
            if (!n) return 0;
            int lh = n->left ? height(n->left) : 0;
            int rh = n->right ? height(n->right) : 0;
            return 1 + (lh > rh ? lh : rh);
        };
        parent->bfactor = height(parent->right) - height(parent->left);
        if (parent->bfactor > 1 || parent->bfactor < -1) {
            unbalanced = parent;
            break;
        }
        child = parent;
        parent = parent->parent;
    }

    if (unbalanced) {
        // Determine rotation case
        Node* newRoot = nullptr;
        if (unbalanced->bfactor > 1) { // right heavy
            if (value > unbalanced->right->data) {
                // Right-Right -> left rotation
                newRoot = rotateLeft(unbalanced);
            } else {
                // Right-Left -> right-left double rotation
                rotateRight(unbalanced->right);
                newRoot = rotateLeft(unbalanced);
            }
        } else { // left heavy
            if (value < unbalanced->left->data) {
                // Left-Left -> right rotation
                newRoot = rotateRight(unbalanced);
            } else {
                // Left-Right -> left-right double rotation
                rotateLeft(unbalanced->left);
                newRoot = rotateRight(unbalanced);
            }
        }
        if (newRoot->parent == nullptr) {
            root = newRoot;
        } else {
            // If unbalanced was not root, root remains unchanged, but newRoot already linked correctly
            // Find actual root
            Node* temp = newRoot;
            while (temp->parent) temp = temp->parent;
            root = temp;
        }
    }

    return root;
}

#include <cassert>

// Helper to compute height for testing
int ht(Node* n) {
    if (!n) return 0;
    int l = ht(n->left);
    int r = ht(n->right);
    return 1 + (l > r ? l : r);
}

// Verify AVL property
bool isAVL(Node* n) {
    if (!n) return true;
    int bf = ht(n->right) - ht(n->left);
    if (bf < -1 || bf > 1) return false;
    return isAVL(n->left) && isAVL(n->right);
}

int main() {
    // Test 1: Insert into empty tree
    Node* root = nullptr;
    root = insertAVL(root, 10);
    assert(root != nullptr && root->data == 10 && root->left == nullptr && root->right == nullptr);
    assert(isAVL(root));

    // Test 2: Insert increasing values (causes left rotations)
    root = insertAVL(root, 20);
    root = insertAVL(root, 30);
    assert(root->data == 20); // root becomes 20 after rotation
    assert(root->left->data == 10);
    assert(root->right->data == 30);
    assert(isAVL(root));

    // Test 3: Insert causing right-left rotation
    root = insertAVL(root, 25);
    assert(isAVL(root));
    // Verify inorder is sorted
    // Inorder: 10,20,25,30
    assert(root->data == 20); // after insertion, tree might rebalance
    assert(isAVL(root));

    // Test 4: Insert causing left-right rotation
    Node* root2 = nullptr;
    root2 = insertAVL(root2, 30);
    root2 = insertAVL(root2, 10);
    root2 = insertAVL(root2, 20);
    assert(isAVL(root2));
    assert(root2->data == 20);

    // Test 5: Many inserts, check AVL property
    Node* root3 = nullptr;
    for (int i = 1; i <= 100; i++) {
        root3 = insertAVL(root3, i * 7 % 101); // pseudo-random but unique
    }
    assert(isAVL(root3));

    // Test 6: Check parent pointers
    root = insertAVL(root, 5);
    Node* node5 = root;
    while (node5->left) node5 = node5->left;
    assert(node5->data == 5);
    if (node5->parent) assert(node5->parent->left == node5 || node5->parent->right == node5);
    assert(isAVL(root));

    // Test 7: Duplicate handling? Not required, but ensure no crash
    // (we assume unique values per task, but test same value returns root)
    Node* before = root;
    root = insertAVL(root, 20); // duplicate
    assert(root != nullptr);
    assert(isAVL(root));

    // Test 8: Delete all nodes to avoid memory leak (optional)
    // Not necessary for test, but we can free dynamic memory in a real program.

    return 0;
}
