// Write a C++ function `mergeSortedLinkedLists` that takes two heads of singly linked lists, each sorted in non-decreasing order, and returns the head of a new singly linked list containing all nodes from both input lists merged in sorted order. The function must not allocate new nodes for data; it should rearrange the existing nodes by updating their `next` pointers. The input lists may be empty (nullptr heads), may have duplicate values, and may have different lengths. The function should preserve the relative order of equal elements (stable merge) and should not modify the data values of any node. The linked list node structure is defined as `struct Node { int data; Node* next; Node(int value); };`. The function signature must be `Node* mergeSortedLinkedLists(Node* head1, Node* head2);`.
The solution uses a standard merge algorithm for two sorted sequences. Create a dummy node with data `0` and `next = nullptr` as a temporary placeholder to simplify handling of the new list’s head. Maintain a pointer `tail` that points to the last node of the merged list so far. Iterate while both input pointers are non-null: compare `head1->data` and `head2->data`; attach the smaller (or equal, for stability) node to `tail->next`, advance the corresponding input pointer, and then advance `tail` to its new last node. After the loop, one of the input lists may still have remaining nodes; attach the remainder directly to `tail->next`. Return `dummy->next` (skipping the dummy). Edge cases: if both heads are null, return null; if one is null, return the other. No new nodes are created except the dummy, which is not part of the returned list. Time complexity is O(n + m) where n and m are lengths of the input lists, because each node is visited once. Space complexity is O(1) auxiliary (not counting the dummy node, which is constant).
#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int value) : data(value), next(nullptr) {}
};

// Merges two sorted linked lists into one sorted linked list.
// Returns the head of the merged list, reusing existing nodes.
Node* mergeSortedLinkedLists(Node* head1, Node* head2) {
    if (head1 == nullptr) return head2;
    if (head2 == nullptr) return head1;

    Node dummy(0);          // Dummy node to simplify pointer manipulation
    Node* tail = &dummy;

    while (head1 != nullptr && head2 != nullptr) {
        if (head1->data <= head2->data) {
            tail->next = head1;
            head1 = head1->next;
        } else {
            tail->next = head2;
            head2 = head2->next;
        }
        tail = tail->next;
    }

    // Attach any remaining nodes from either list
    tail->next = (head1 != nullptr) ? head1 : head2;

    return dummy.next;
}
#include <cassert>

// Helper to create a list from a vector-like initializer list (simplified for tests)
Node* buildList(std::initializer_list<int> values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : values) {
        Node* n = new Node(v);
        if (head == nullptr) { head = n; tail = n; }
        else { tail->next = n; tail = n; }
    }
    return head;
}

// Helper to compare list contents to an expected sequence
bool listEquals(Node* head, std::initializer_list<int> expected) {
    const int* e = expected.begin();
    while (head != nullptr && e != expected.end()) {
        if (head->data != *e) return false;
        head = head->next;
        ++e;
    }
    return (head == nullptr) && (e == expected.end());
}

// Helper to free list memory (for cleanup, not required for asserts)
void freeList(Node* head) {
    while (head) {
        Node* t = head->next;
        delete head;
        head = t;
    }
}

int main() {
    // Both lists non-empty, distinct values
    Node* a1 = buildList({1, 3, 5});
    Node* b1 = buildList({2, 4, 6});
    Node* merged1 = mergeSortedLinkedLists(a1, b1);
    assert(listEquals(merged1, {1, 2, 3, 4, 5, 6}));
    freeList(merged1);

    // One list empty
    Node* a2 = buildList({10, 20});
    Node* merged2 = mergeSortedLinkedLists(a2, nullptr);
    assert(listEquals(merged2, {10, 20}));
    freeList(merged2);

    // Both empty
    Node* merged3 = mergeSortedLinkedLists(nullptr, nullptr);
    assert(merged3 == nullptr);

    // Duplicates across lists
    Node* a4 = buildList({1, 2, 2, 3});
    Node* b4 = buildList({2, 3, 3, 4});
    Node* merged4 = mergeSortedLinkedLists(a4, b4);
    assert(listEquals(merged4, {1, 2, 2, 2, 3, 3, 3, 4}));
    freeList(merged4);

    // All equal values, both lists same length
    Node* a5 = buildList({5, 5, 5});
    Node* b5 = buildList({5, 5, 5});
    Node* merged5 = mergeSortedLinkedLists(a5, b5);
    assert(listEquals(merged5, {5, 5, 5, 5, 5, 5}));
    freeList(merged5);

    // One list much longer, first list exhausted quickly
    Node* a6 = buildList({1});
    Node* b6 = buildList({2, 3, 4, 5});
    Node* merged6 = mergeSortedLinkedLists(a6, b6);
    assert(listEquals(merged6, {1, 2, 3, 4, 5}));
    freeList(merged6);

    // Negative and zero values
    Node* a7 = buildList({-5, -1, 0});
    Node* b7 = buildList({-3, 2});
    Node* merged7 = mergeSortedLinkedLists(a7, b7);
    assert(listEquals(merged7, {-5, -3, -1, 0, 2}));
    freeList(merged7);

    return 0;
}
