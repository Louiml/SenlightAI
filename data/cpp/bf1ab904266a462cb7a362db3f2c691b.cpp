// Write a C++ function `bool containsKey(const Node* head, int key)` that takes a pointer to the head of a singly linked list and an integer key. The function must return `true` if any node in the list holds `key`, and `false` otherwise. The list may be empty (head is `nullptr`), contain duplicate values, or contain any integer (including negative values). The function must not modify the list. Define a `Node` struct with an `int data` and a `Node* next` pointer, with a constructor initializing both fields.

The solution uses a simple linear traversal of the linked list. Starting from the head, we check each node's data value; if it equals the key, we immediately return `true`. If we reach the end of the list (a `nullptr` next pointer) without finding the key, we return `false`. Edge cases: an empty list (head is `nullptr`) returns `false` immediately; duplicate values do not affect correctness; negative key values work as expected since comparisons are straightforward. The algorithm operates in \(O(n)\) time, where \(n\) is the number of nodes, and uses \(O(1)\) auxiliary space, since no extra data structures are employed. The function is marked `const`-correct by taking a `const Node*` to guarantee the list is not modified.

#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int x) : data(x), next(nullptr) {}
};

// Returns true if the linked list contains the given key; false otherwise.
bool containsKey(const Node* head, int key) {
    const Node* current = head;
    while (current != nullptr) {
        if (current->data == key) {
            return true;
        }
        current = current->next;
    }
    return false;
}

#include <cassert>

int main() {
    // Test empty list
    assert(containsKey(nullptr, 5) == false);

    // Test single node with matching key
    Node* a = new Node(7);
    assert(containsKey(a, 7) == true);
    assert(containsKey(a, 8) == false);

    // Test multi-node list
    Node* b = new Node(3);
    a->next = b;
    Node* c = new Node(-4);
    b->next = c;
    Node* d = new Node(9);
    c->next = d;

    assert(containsKey(a, 3) == true);
    assert(containsKey(a, -4) == true);
    assert(containsKey(a, 9) == true);
    assert(containsKey(a, 0) == false);
    assert(containsKey(a, 10) == false);

    // Test duplicates
    Node* e = new Node(3);
    d->next = e;
    assert(containsKey(a, 3) == true);

    // Clean up (not strictly required for test, but good practice)
    delete e; delete d; delete c; delete b; delete a;
    return 0;
}
