// Given a perfect binary tree where each node has a `next` pointer initially set to `nullptr`, write a C++ function `connectPerfectBinaryTree(Node* root)` that populates each node's `next` pointer to point to its right sibling at the same depth. If a node has no right sibling (i.e., it is the rightmost node at its level), its `next` pointer must remain `nullptr`. The tree is guaranteed to be perfect (all leaves at the same depth, every internal node has exactly two children). The function must operate in constant auxiliary space (excluding recursion stack and output modifications) and must not use any additional data structures like queues or vectors. The function should return `void` and mutate the tree in place. Handle the case where `root` is `nullptr` gracefully by doing nothing.

#include <cassert>

// Helper to create a perfect binary tree of given height (1 = just root).
Node* createPerfectTree(int height, int& value) {
    if (height == 0) {
        return nullptr;
    }
    Node* node = new Node(value++);
    if (height > 1) {
        node->left = createPerfectTree(height - 1, value);
        node->right = createPerfectTree(height - 1, value);
    }
    return node;
}

// Helper to verify next pointers for a perfect tree.
void verifyNextPointers(Node* root, int& expected) {
    if (root == nullptr) {
        return;
    }
    // For each level, traverse via next and check values are consecutive.
    Node* levelStart = root;
    while (levelStart != nullptr) {
        Node* current = levelStart;
        while (current != nullptr) {
            assert(current->val == expected);
            expected++;
            current = current->next;
        }
        // The last node's next should be null.
        // Check by traversing to end of current level.
        current = levelStart;
        while (current->next != nullptr) {
            current = current->next;
        }
        assert(current->next == nullptr);
        // Move to next level (leftmost child of levelStart).
        levelStart = levelStart->left;
    }
}

// Helper to free the tree.
void deleteTree(Node* root) {
    if (root == nullptr) {
        return;
    }
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    // Test 1: Empty tree
    Node* empty = nullptr;
    connectPerfectBinaryTree(empty); // Should do nothing

    // Test 2: Single node tree
    int val = 1;
    Node* single = new Node(val);
    connectPerfectBinaryTree(single);
    assert(single->next == nullptr);
    delete single;

    // Test 3: Height 2 (3 nodes)
    val = 1;
    Node* tree2 = createPerfectTree(2, val);
    connectPerfectBinaryTree(tree2);
    int expected2 = 1;
    verifyNextPointers(tree2, expected2);
    deleteTree(tree2);

    // Test 4: Height 3 (7 nodes)
    val = 1;
    Node* tree3 = createPerfectTree(3, val);
    connectPerfectBinaryTree(tree3);
    int expected3 = 1;
    verifyNextPointers(tree3, expected3);
    deleteTree(tree3);

    // Test 5: Height 4 (15 nodes)
    val = 1;
    Node* tree4 = createPerfectTree(4, val);
    connectPerfectBinaryTree(tree4);
    int expected4 = 1;
    verifyNextPointers(tree4, expected4);
    deleteTree(tree4);

    // Test 6: Verify rightmost nodes have next == nullptr
    val = 1;
    Node* tree5 = createPerfectTree(3, val);
    connectPerfectBinaryTree(tree5);
    // Check bottom-right node (should have value 7)
    Node* bottomRight = tree5->right->right->right;
    assert(bottomRight->next == nullptr);
    // Check middle-right node (value 3)
    assert(tree5->right->next == nullptr);
    deleteTree(tree5);

    return 0;
}

#include <cstddef>

// Node structure for a binary tree with a next pointer.
struct Node {
    int val;
    Node* left;
    Node* right;
    Node* next;

    Node(int x) : val(x), left(nullptr), right(nullptr), next(nullptr) {}
};

// Populate each node's next pointer to point to its right sibling.
// The tree is perfect, so every internal node has two children.
// Uses constant auxiliary space.
void connectPerfectBinaryTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    Node* levelStart = root;

    // Traverse levels from top to bottom.
    while (levelStart->left != nullptr) {
        Node* current = levelStart;
        Node dummy(0);  // Dummy head to build the next level's linked list.
        Node* tail = &dummy;

        // Traverse all nodes in the current level.
        while (current != nullptr) {
            // Connect left child to right child.
            tail->next = current->left;
            tail = tail->next;
            tail->next = current->right;
            tail = tail->next;

            // Move to the next node in the current level.
            current = current->next;
        }

        // Move down to the leftmost child of the next level.
        levelStart = levelStart->left;
    }
}

// The solution processes the tree level by level using two pointers: `level` tracks the current leftmost node of the level being processed, and `node` traverses all nodes in that level. For each node in the current level, we connect its left child to its right child (they are siblings), and then if the current node has a `next` pointer, we connect its right child to the next node's left child. This is done by maintaining a `dummy` head node to simplify the linking of the next level's chain. After finishing a level, we move `level` to its left child, which starts the next level. This works because the tree is perfect, so every node has both children, and we only need to traverse down the leftmost path to reach the next level. The time complexity is \(O(n)\) where \(n\) is the number of nodes, since each node is visited exactly once. The auxiliary space is \(O(1)\), ignoring the input tree itself, because we only use a fixed number of pointers. Edge cases include an empty tree (do nothing) and a tree with only one node (no children, so nothing to connect).
