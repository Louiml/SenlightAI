// Write a C++ function `reverseLinkedList` that takes a pointer to the head of a singly linked list and reverses the list iteratively, returning the new head. The list nodes store integer values. The function must handle an empty list (returning `nullptr`), a single-node list (returning the same node), and a general case. Do not modify the node structure; use only pointer manipulation. The function must be `const`-correct regarding the input pointer (it may take a pointer to non-const nodes but not modify the nodes' data), and it must not allocate memory. Additionally, provide a helper function `printList` that prints the list in the format `value->value->...` without trailing arrow, but that helper is not required for the solution function itself. Focus only on the reverse function in the solution section.

The solution uses the standard iterative reversal algorithm with three pointers: `prev`, `current`, and `next`. Initialize `prev` to `nullptr` and `current` to the input head. While `current` is not `nullptr`, save `current->next` to `next`, set `current->next` to `prev`, then move `prev` to `current` and `current` to `next`. At the end, `prev` is the new head. Edge cases: an empty list immediately returns `nullptr` because the loop condition fails; a single-node list returns the same node after one iteration where `next` is `nullptr`. Time complexity is O(n) where n is the number of nodes, and auxiliary space is O(1). The function should be declared `Node* reverseLinkedList(Node* head)` and must handle the case where the input pointer is `nullptr` without dereferencing it.

#include <cstddef>

// Node structure for singly linked list.
struct Node {
    int data;
    Node* next;
    explicit Node(int value) : data(value), next(nullptr) {}
};

// Reverses a singly linked list iteratively and returns the new head.
// Handles empty list (nullptr) and single-node list without special cases.
Node* reverseLinkedList(Node* head) {
    Node* prev = nullptr;
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        current->next = prev;
        prev = current;
        current = nextNode;
    }
    return prev; // prev is the new head (or nullptr if input was empty)
}

#include <cassert>

// Helper to create a list from an initializer list for testing.
Node* createList(std::initializer_list<int> values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int val : values) {
        Node* newNode = new Node(val);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

// Helper to free list memory.
void deleteList(Node* head) {
    while (head) {
        Node* temp = head->next;
        delete head;
        head = temp;
    }
}

// Helper to compare two lists (by value) for equality.
bool listsEqual(Node* a, Node* b) {
    while (a && b) {
        if (a->data != b->data) return false;
        a = a->next;
        b = b->next;
    }
    return (a == nullptr && b == nullptr);
}

int main() {
    // Test empty list
    Node* empty = nullptr;
    assert(reverseLinkedList(empty) == nullptr);

    // Test single node
    Node* single = new Node(5);
    Node* revSingle = reverseLinkedList(single);
    assert(revSingle == single && revSingle->next == nullptr);
    deleteList(revSingle);

    // Test multiple nodes
    Node* list1 = createList({1, 2, 3, 4, 5});
    Node* rev1 = reverseLinkedList(list1);
    assert(listsEqual(rev1, createList({5, 4, 3, 2, 1})) ? (deleteList(rev1), true) : (deleteList(rev1), false));

    // Test two nodes
    Node* list2 = createList({10, 20});
    Node* rev2 = reverseLinkedList(list2);
    assert(rev2->data == 20 && rev2->next->data == 10 && rev2->next->next == nullptr);
    deleteList(rev2);

    // Test larger list with duplicate values
    Node* list3 = createList({7, 3, 3, 8, 3});
    Node* rev3 = reverseLinkedList(list3);
    assert(listsEqual(rev3, createList({3, 8, 3, 3, 7})) ? (deleteList(rev3), true) : (deleteList(rev3), false));
}
