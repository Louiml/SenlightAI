/*
Write a C++ function `void treeToDoublyLinkedList(Node* root, Node** headRef)` that converts a given binary search tree (BST) into a sorted doubly linked list in-place, where the `left` pointer of each node is used as the `prev` pointer and the `right` pointer is used as the `next` pointer. The function must set `*headRef` to point to the head (smallest element) of the resulting doubly linked list. The BST may contain duplicate values and may be unbalanced. The conversion must not allocate any new nodes; it must reuse the existing tree nodes. The resulting list must be traversable both forward (via `right`) and backward (via `left`) starting from the head. After conversion, the original tree structure is destroyed (the tree becomes a doubly linked list). The BST may contain up to 10^5 nodes, and each node's `data` is a non-negative integer within the range [0, 10^9]. The function must handle an empty tree (root == NULL) gracefully by setting `*headRef` to NULL.
*/
#include <cstddef>

// Node structure for the binary search tree / doubly linked list.
struct Node {
    int data;
    Node* left;   // used as prev in DLL
    Node* right;  // used as next in DLL
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Helper function that performs in-order traversal and links the nodes.
void inOrderLink(Node* current, Node*& prevNode, Node** headRef) {
    if (current == nullptr) {
        return;
    }

    // Traverse left subtree.
    inOrderLink(current->left, prevNode, headRef);

    // If prevNode is null, this is the leftmost node (smallest) => head.
    if (prevNode == nullptr) {
        *headRef = current;
    } else {
        // Link the previous node and current node.
        prevNode->right = current;
        current->left = prevNode;
    }

    // Update prevNode to the current node.
    prevNode = current;

    // Traverse right subtree.
    inOrderLink(current->right, prevNode, headRef);
}

// Public function to convert BST to sorted doubly linked list.
void treeToDoublyLinkedList(Node* root, Node** headRef) {
    *headRef = nullptr;
    Node* prevNode = nullptr;
    inOrderLink(root, prevNode, headRef);
}
#include <cassert>
#include <iostream>

// Node structure for the binary search tree / doubly linked list.
struct Node {
    int data;
    Node* left;   // used as prev in DLL
    Node* right;  // used as next in DLL
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Helper function that performs in-order traversal and links the nodes.
void inOrderLink(Node* current, Node*& prevNode, Node** headRef) {
    if (current == nullptr) {
        return;
    }

    // Traverse left subtree.
    inOrderLink(current->left, prevNode, headRef);

    // If prevNode is null, this is the leftmost node (smallest) => head.
    if (prevNode == nullptr) {
        *headRef = current;
    } else {
        // Link the previous node and current node.
        prevNode->right = current;
        current->left = prevNode;
    }

    // Update prevNode to the current node.
    prevNode = current;

    // Traverse right subtree.
    inOrderLink(current->right, prevNode, headRef);
}

// Public function to convert BST to sorted doubly linked list.
void treeToDoublyLinkedList(Node* root, Node** headRef) {
    *headRef = nullptr;
    Node* prevNode = nullptr;
    inOrderLink(root, prevNode, headRef);
}

// Helper to build a small BST from an initializer list (for testing only).
// This function is simplified: it inserts nodes manually for the tests below.
int main() {
    // Test 1: Empty tree.
    Node* root1 = nullptr;
    Node* head1 = nullptr;
    treeToDoublyLinkedList(root1, &head1);
    assert(head1 == nullptr);

    // Test 2: Single node.
    Node* root2 = new Node(5);
    Node* head2 = nullptr;
    treeToDoublyLinkedList(root2, &head2);
    assert(head2 == root2);
    assert(head2->left == nullptr);
    assert(head2->right == nullptr);

    // Test 3: Balanced BST (3 nodes: 2, 1, 3).
    Node* root3 = new Node(2);
    root3->left = new Node(1);
    root3->right = new Node(3);
    Node* head3 = nullptr;
    treeToDoublyLinkedList(root3, &head3);
    assert(head3->data == 1);
    assert(head3->right->data == 2);
    assert(head3->right->right->data == 3);
    assert(head3->right->left == head3);
    assert(head3->right->right->left == head3->right);
    assert(head3->left == nullptr);
    assert(head3->right->right->right == nullptr);

    // Test 4: Skewed left tree (3, 2, 1).
    Node* root4 = new Node(3);
    root4->left = new Node(2);
    root4->left->left = new Node(1);
    Node* head4 = nullptr;
    treeToDoublyLinkedList(root4, &head4);
    assert(head4->data == 1);
    assert(head4->right->data == 2);
    assert(head4->right->right->data == 3);
    assert(head4->right->left == head4);
    assert(head4->right->right->left == head4->right);

    // Test 5: Duplicate values (2, 2, 1).
    Node* root5 = new Node(2);
    root5->left = new Node(1);
    root5->right = new Node(2);
    Node* head5 = nullptr;
    treeToDoublyLinkedList(root5, &head5);
    assert(head5->data == 1);
    assert(head5->right->data == 2);
    assert(head5->right->right->data == 2);
    assert(head5->right->left == head5);
    assert(head5->right->right->left == head5->right);

    // Test 6: Larger tree (5 nodes: 4,2,5,1,3).
    Node* root6 = new Node(4);
    root6->left = new Node(2);
    root6->right = new Node(5);
    root6->left->left = new Node(1);
    root6->left->right = new Node(3);
    Node* head6 = nullptr;
    treeToDoublyLinkedList(root6, &head6);
    int expected[] = {1,2,3,4,5};
    Node* cur = head6;
    for (int i = 0; i < 5; ++i) {
        assert(cur->data == expected[i]);
        if (i > 0) {
            assert(cur->left->data == expected[i-1]);
        }
        if (i < 4) {
            assert(cur->right->data == expected[i+1]);
        }
        cur = cur->right;
    }
    assert(cur == nullptr);
    // Traverse backward.
    cur = head6;
    while (cur->right != nullptr) cur = cur->right;
    for (int i = 4; i >= 0; --i) {
        assert(cur->data == expected[i]);
        cur = cur->left;
    }
    assert(cur == nullptr);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The solution uses an in-order traversal of the binary search tree because in-order traversal of a BST produces nodes in non-decreasing sorted order. We maintain a global/static pointer `prevNode` that tracks the previously visited node during the traversal. When we visit a node for the first time (when `*headRef` is still NULL), we set `*headRef` to that node, which becomes the smallest element (the head of the DLL). For every subsequent node, we link the current node’s `left` pointer to `prevNode` and the `prevNode`’s `right` pointer to the current node. Then we update `prevNode` to the current node. After the traversal, the doubly linked list is correctly wired from head to tail. Important edge cases: (1) Empty tree (`root == NULL`) — directly set `*headRef` to NULL. (2) Single-node BST — the node becomes both head and tail, with `left` and `right` already NULL. (3) Duplicate values — they are ordered arbitrarily, but the links still form a valid sorted (non-decreasing) list. (4) The static pointer must be reset for each call when the function is invoked on a new tree; to avoid issues with repeated calls, we initialize `prevNode` to NULL at the start of the public function and pass it by reference to a recursive helper (or use a local static with reset logic). The time complexity is O(n), where n is the number of nodes, because we visit each node exactly once. The space complexity is O(h) for the recursion stack, where h is the height of the BST (worst case O(n) for a skewed tree, but typically O(log n) for a balanced tree). No extra dynamic memory is used other than the recursion stack.
