// Write a C++ function `reverseList` that takes a singly linked list (defined by a `Node` struct with an `int data` and a `Node* next` pointer) and returns the head pointer of the reversed list. The function must reverse the list in-place by manipulating the `next` pointers, without creating any new nodes. The input list may be empty (nullptr), contain a single node, or contain many nodes. Your function must be const-correct where applicable (i.e., it does not modify the data values, only the pointers). Do not use any standard library containers beyond the node definition. The function should be free-standing (no `main`), and the node structure must be defined inside the same code block.

// The reversal is done iteratively using three pointers: `previous`, `current`, and `next`. Starting with `previous` as `nullptr` and `current` as the head, we repeatedly save `current->next` into `next`, then set `current->next` to `previous`, advance `previous` to `current`, and `current` to `next`. This continues until `current` becomes `nullptr`, at which point `previous` points to the new head, which is returned. Edge cases: an empty list (head is `nullptr`) immediately returns `nullptr`; a single-node list returns the same node since the loop just sets its `next` to `nullptr` and returns it. The algorithm runs in O(n) time, where n is the number of nodes, and uses O(1) auxiliary space because only a few pointers are used. No new nodes are allocated, and the original nodes are reused. The function does not modify `data` members, so the node's data can be considered `const`-qualified in the read-only sense, but since we need to change `next`, the pointer must be non-const.

#include <cstddef>  // for nullptr

// Node structure for a singly linked list.
struct Node {
    int data;
    Node* next;
    
    explicit Node(int val) : data(val), next(nullptr) {}
};

// Reverse a singly linked list in-place and return the new head.
// The function takes a head pointer by value, modifies the links,
// and returns the head of the reversed list.
Node* reverseList(Node* head) {
    Node* previous = nullptr;
    Node* current = head;
    
    while (current != nullptr) {
        Node* nextNode = current->next;  // save the next node
        current->next = previous;        // reverse the link
        previous = current;              // move previous forward
        current = nextNode;              // move current forward
    }
    
    return previous;  // new head of the reversed list
}

#include <cassert>

// Helper functions for testing (not part of the solution).
Node* buildList(std::initializer_list<int> values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : values) {
        Node* n = new Node(v);
        if (tail) {
            tail->next = n;
        } else {
            head = n;
        }
        tail = n;
    }
    return head;
}

void deleteList(Node* head) {
    while (head) {
        Node* temp = head->next;
        delete head;
        head = temp;
    }
}

bool listEquals(Node* head, std::initializer_list<int> values) {
    Node* temp = head;
    for (int v : values) {
        if (!temp || temp->data != v) return false;
        temp = temp->next;
    }
    return temp == nullptr;
}

int main() {
    // Test empty list
    Node* empty = nullptr;
    assert(reverseList(empty) == nullptr);

    // Test single node
    Node* single = new Node(42);
    Node* reversedSingle = reverseList(single);
    assert(reversedSingle == single);
    assert(reversedSingle->data == 42);
    assert(reversedSingle->next == nullptr);
    delete single;

    // Test list with multiple nodes
    Node* list1 = buildList({1, 2, 3, 4});
    Node* rev1 = reverseList(list1);
    assert(listEquals(rev1, {4, 3, 2, 1}));
    deleteList(rev1);

    // Test list with two nodes
    Node* list2 = buildList({5, 6});
    Node* rev2 = reverseList(list2);
    assert(listEquals(rev2, {6, 5}));
    deleteList(rev2);

    // Test list with duplicate values
    Node* list3 = buildList({1, 1, 2});
    Node* rev3 = reverseList(list3);
    assert(listEquals(rev3, {2, 1, 1}));
    deleteList(rev3);

    return 0;
}
