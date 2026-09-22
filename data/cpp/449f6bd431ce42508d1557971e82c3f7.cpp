/*
Write a C++ function that, given the root of a binary search tree (BST) and an integer key, returns a pointer to the in-order successor of the node containing that key, or `nullptr` if the key is absent or the node has no in-order successor. The in-order successor of a node is the node with the smallest value greater than the node’s key. If the key is not present in the BST, return `nullptr`. The BST contains unique integer values and may be empty.
*/
#include <cstddef>

struct Node {
    int data;
    Node *left;
    Node *right;
};

// Helper: return the leftmost node in a subtree.
Node* leftmost(Node* root) {
    while (root && root->left != nullptr) {
        root = root->left;
    }
    return root;
}

// Return the in-order successor of the node with the given key, or nullptr if not found or no successor.
Node* inorderSuccessor(const Node* root, int key) {
    const Node* current = root;
    Node* succ = nullptr;

    while (current != nullptr) {
        if (key == current->data) {
            if (current->right != nullptr) {
                succ = leftmost(current->right);
            }
            break;
        } else if (key < current->data) {
            succ = const_cast<Node*>(current); // potential successor ancestor
            current = current->left;
        } else {
            current = current->right;
        }
    }
    // If key not found, succ will still be nullptr because we never reached key and break.
    // But if key not found, we should return nullptr. The loop may have set succ for ancestors
    // incorrectly. So we need to re-check. Simpler: use a found flag.
    // The following corrected version:
    // We'll re-implement with a found flag to be safe.
    // However the above logic breaks on key found. If key not found, succ may be non-null
    // from ancestors but we must return nullptr. So we need to verify key exists.
    // Let's implement a cleaner version below.
    // I'll rewrite the function properly.
    return nullptr; // placeholder, actual implementation follows
}
**Correction**: The above is incomplete. Here is the final correct implementation:

#include <cstddef>

struct Node {
    int data;
    Node *left;
    Node *right;
};

// Helper: return the leftmost node in a subtree.
Node* leftmost(const Node* root) {
    while (root && root->left != nullptr) {
        root = root->left;
    }
    return const_cast<Node*>(root);
}

// Return the in-order successor of the node with the given key, or nullptr if key not present or no successor.
Node* inorderSuccessor(const Node* root, int key) {
    const Node* current = root;
    Node* succ = nullptr;
    bool found = false;

    while (current != nullptr) {
        if (key == current->data) {
            found = true;
            if (current->right != nullptr) {
                succ = leftmost(current->right);
            }
            break;
        } else if (key < current->data) {
            succ = const_cast<Node*>(current);
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (!found) {
        return nullptr;
    }
    return succ;
}
#include <cassert>

// Assume Node and inorderSuccessor are defined as above.
// Helper to create a node.
Node* makeNode(int val) {
    Node* n = new Node{val, nullptr, nullptr};
    return n;
}

// Helper to insert into BST (for testing).
Node* insert(Node* root, int val) {
    if (root == nullptr) return makeNode(val);
    if (val < root->data) root->left = insert(root->left, val);
    else if (val > root->data) root->right = insert(root->right, val);
    return root;
}

int main() {
    // Build BST: 15,10,20,8,12,17,25,6,11,16,27
    Node* root = nullptr;
    int keys[] = {15,10,20,8,12,17,25,6,11,16,27};
    for (int k : keys) root = insert(root, k);

    // Test successors
    assert(inorderSuccessor(root, 6) != nullptr && inorderSuccessor(root, 6)->data == 8);
    assert(inorderSuccessor(root, 8) != nullptr && inorderSuccessor(root, 8)->data == 10);
    assert(inorderSuccessor(root, 10) != nullptr && inorderSuccessor(root, 10)->data == 11);
    assert(inorderSuccessor(root, 11) != nullptr && inorderSuccessor(root, 11)->data == 12);
    assert(inorderSuccessor(root, 12) != nullptr && inorderSuccessor(root, 12)->data == 15);
    assert(inorderSuccessor(root, 15) != nullptr && inorderSuccessor(root, 15)->data == 16);
    assert(inorderSuccessor(root, 16) != nullptr && inorderSuccessor(root, 16)->data == 17);
    assert(inorderSuccessor(root, 17) != nullptr && inorderSuccessor(root, 17)->data == 20);
    assert(inorderSuccessor(root, 20) != nullptr && inorderSuccessor(root, 20)->data == 25);
    assert(inorderSuccessor(root, 25) != nullptr && inorderSuccessor(root, 25)->data == 27);
    assert(inorderSuccessor(root, 27) == nullptr);

    // Test key not present
    assert(inorderSuccessor(root, 99) == nullptr);
    assert(inorderSuccessor(root, 1) == nullptr);

    // Test empty tree
    assert(inorderSuccessor(nullptr, 5) == nullptr);

    // Clean up (not necessary for test, but good practice)
    // (Omitted for brevity; a full program would delete nodes)

    return 0;
}
// The standard algorithm for finding an in-order successor in a BST uses a single traversal from the root down toward the key. Initialize a pointer `succ` to `nullptr`. Start at the root and repeatedly compare the current node’s data with the key. If the current node's data equals the key, then if the right subtree exists, the successor is the leftmost node in that right subtree (obtained by repeatedly moving left). If the key is less than the current node's data, the current node is a potential ancestor successor, so set `succ` to the current node and move to its left child. If the key is greater, move to the right child without changing `succ`. Continue until reaching `nullptr`. If the key is found, the successor has already been assigned during the traversal, but if the key was found and had a right child, the `succ` pointer is overwritten by the minimum of the right subtree. Edge cases: empty tree (return `nullptr`), key not present, key is the maximum value in the tree (successor is `nullptr`), and key with a right subtree (successor is the leftmost node there). Time complexity is \(O(h)\) where \(h\) is the height of the BST, which is \(O(\log n)\) for a balanced tree and \(O(n)\) in the worst case. Space complexity is \(O(1)\) auxiliary, as only a few pointers are used.
