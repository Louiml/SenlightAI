/*
Write a C++ function `connectSiblings(Node* root)` that, given the root of a perfect binary tree where each node has `val`, `left`, `right`, and a `next` pointer (initially `NULL`), populates each node's `next` pointer to point to its next right node on the same level. If no next right node exists (i.e., it is the rightmost node of a level), the `next` pointer should remain `NULL`. The function must return the root of the modified tree. The node structure is defined as: `struct Node { int val; Node* left; Node* right; Node* next; Node(int v) : val(v), left(nullptr), right(nullptr), next(nullptr) {} };`. You may assume the tree is perfect (every internal node has exactly two children, and all leaves are at the same depth). The input tree may be empty. Do not use any extra space beyond a constant amount (i.e., you may not use a queue or stack, and the recursion stack is acceptable but not optimal—design an iterative constant-space solution).
*/

#include <cstddef>

struct Node {
    int val;
    Node* left;
    Node* right;
    Node* next;
    Node(int v) : val(v), left(nullptr), right(nullptr), next(nullptr) {}
};

// Populate each node's next pointer to its right sibling at the same level.
Node* connectSiblings(Node* root) {
    if (root == nullptr) return nullptr;

    Node* levelStart = root; // leftmost node of the current level

    // While there is a next level (i.e., current level has children)
    while (levelStart->left != nullptr) {
        Node* walker = levelStart; // traverse current level

        while (walker != nullptr) {
            // Connect left child to right child
            walker->left->next = walker->right;

            // If there is a next node on this level, connect right child to its left child
            if (walker->next != nullptr) {
                walker->right->next = walker->next->left;
            }

            walker = walker->next; // move to next node on this level
        }

        levelStart = levelStart->left; // move to the leftmost node of the next level
    }

    return root;
}

#include <cassert>

// Helper to create a perfect tree of given height (0 = null, 1 = single node)
Node* createTree(int height, int& counter) {
    if (height == 0) return nullptr;
    Node* node = new Node(counter++);
    if (height > 1) {
        node->left = createTree(height - 1, counter);
        node->right = createTree(height - 1, counter);
    }
    return node;
}

// Helper to verify that next pointers are correctly set level by level
void verifyNext(Node* root) {
    if (root == nullptr) return;
    Node* levelStart = root;

    while (levelStart->left) {
        Node* walker = levelStart;
        while (walker) {
            assert(walker->left->next == walker->right);
            if (walker->next) {
                assert(walker->right->next == walker->next->left);
            } else {
                assert(walker->right->next == nullptr);
            }
            walker = walker->next;
        }
        levelStart = levelStart->left;
    }
}

int main() {
    // Test 1: Empty tree
    Node* empty = nullptr;
    assert(connectSiblings(empty) == nullptr);

    // Test 2: Single node
    Node* single = new Node(1);
    connectSiblings(single);
    assert(single->next == nullptr);
    delete single;

    // Test 3: Height 2 tree (3 nodes)
    int counter = 1;
    Node* h2 = createTree(2, counter);
    connectSiblings(h2);
    assert(h2->left->next == h2->right);
    assert(h2->right->next == nullptr);
    assert(h2->left->left == nullptr); // leaves have no children
    verifyNext(h2);

    // Test 4: Height 3 tree (7 nodes)
    counter = 1;
    Node* h3 = createTree(3, counter);
    connectSiblings(h3);
    verifyNext(h3);

    // Test 5: Height 4 tree (15 nodes)
    counter = 1;
    Node* h4 = createTree(4, counter);
    connectSiblings(h4);
    verifyNext(h4);

    // Manual check for height 4: leftmost leaf's next chain
    Node* leaf = h4;
    while (leaf->left) leaf = leaf->left;
    int chainCount = 0;
    Node* temp = leaf;
    while (temp) {
        chainCount++;
        temp = temp->next;
    }
    assert(chainCount == 8); // 8 leaves at depth 4

    return 0;
}

// The core idea is to traverse the tree level by level, but instead of using a queue (which would use O(n) extra space), we can exploit the `next` pointers already set in the previous level to traverse the current level. We maintain two pointers: `levelStart` (the leftmost node of the current level) and `current` (used to walk across the level). Initially, `levelStart` is the root. While `levelStart` is not null, we set a `walker` pointer to `levelStart` and, for each node in that level, we link its left child to its right child, and if the node has a `next` pointer, we link the right child to the next node's left child. After finishing the level, we move `levelStart` to `levelStart->left` (the leftmost node of the next level). This works because the tree is perfect: every node except the last on a level has a right sibling, and every node except leaf nodes has children. Edge cases: empty tree (return nullptr immediately) and single-node tree (no changes needed). Time complexity is O(n) because each node is visited once. Space complexity is O(1) auxiliary space (excluding the recursion stack if implemented recursively, but here we use iteration so it is truly O(1)).
