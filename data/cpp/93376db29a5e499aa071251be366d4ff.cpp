// Write a C++ function `insertSorted` that inserts a given integer into a singly linked list while maintaining the list in ascending order. The function should take a pointer to the head node (by reference) and the integer value to insert. It must create a new node, place it in the correct sorted position, and return nothing (void). The list may be empty, already contain duplicate values, or require insertion at the beginning, middle, or end. The function should handle all these cases without memory leaks and without using any standard library containers—only your own `Node` struct with members `int data` and `Node* next`. The function must be robust to a null head pointer and maintain the invariant that the list remains sorted after insertion.
#include <cassert>

// Helper to check list values in order
bool listMatches(Node* head, const int* expected, int size) {
    Node* current = head;
    for (int i = 0; i < size; ++i) {
        if (current == nullptr || current->data != expected[i]) return false;
        current = current->next;
    }
    return current == nullptr;
}

// Helper to free memory
void deleteList(Node*& head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: Insert into empty list
    Node* list1 = nullptr;
    insertSorted(list1, 5);
    int exp1[] = {5};
    assert(listMatches(list1, exp1, 1));
    deleteList(list1);

    // Test 2: Insert at beginning (smaller than head)
    Node* list2 = nullptr;
    insertSorted(list2, 10);
    insertSorted(list2, 3);
    int exp2[] = {3, 10};
    assert(listMatches(list2, exp2, 2));
    deleteList(list2);

    // Test 3: Insert in middle
    Node* list3 = nullptr;
    insertSorted(list3, 1);
    insertSorted(list3, 9);
    insertSorted(list3, 5);
    int exp3[] = {1, 5, 9};
    assert(listMatches(list3, exp3, 3));
    deleteList(list3);

    // Test 4: Insert at end (largest value)
    Node* list4 = nullptr;
    insertSorted(list4, 2);
    insertSorted(list4, 4);
    insertSorted(list4, 8);
    int exp4[] = {2, 4, 8};
    assert(listMatches(list4, exp4, 3));
    deleteList(list4);

    // Test 5: Duplicate values
    Node* list5 = nullptr;
    insertSorted(list5, 7);
    insertSorted(list5, 7);
    insertSorted(list5, 7);
    int exp5[] = {7, 7, 7};
    assert(listMatches(list5, exp5, 3));
    deleteList(list5);

    // Test 6: Mixed duplicates and insertion at various positions
    Node* list6 = nullptr;
    insertSorted(list6, 3);
    insertSorted(list6, 1);
    insertSorted(list6, 3);
    insertSorted(list6, 2);
    insertSorted(list6, 4);
    int exp6[] = {1, 2, 3, 3, 4};
    assert(listMatches(list6, exp6, 5));
    deleteList(list6);

    return 0;
}
#include <iostream>

struct Node {
    int data;
    Node* next;
};

// Insert a new node with value `value` into a sorted singly linked list.
// The list is modified in place to maintain ascending order.
void insertSorted(Node*& head, int value) {
    Node* newNode = new Node{value, nullptr};

    // Case: empty list or new value should be the new head
    if (head == nullptr || head->data >= value) {
        newNode->next = head;
        head = newNode;
        return;
    }

    // Traverse to find the correct insertion point
    Node* current = head;
    while (current->next != nullptr && current->next->data < value) {
        current = current->next;
    }

    // Insert after `current`
    newNode->next = current->next;
    current->next = newNode;
}
// The solution traverses the linked list starting from the head, keeping track of the current node and the previous node. Since the list is sorted ascending, we stop when we find the first node whose data is greater than or equal to the new value (or when we reach the end). We insert the new node before that position. Edge cases: if the list is empty or the new value is smaller than the head's data, the new node becomes the new head. If duplicates exist, inserting before the first greater-or-equal node keeps the list sorted (order among equal values is irrelevant). The algorithm runs in O(n) time in the worst case (when inserting at the end) and O(1) auxiliary space. Memory is allocated for exactly one new node and freed only when the caller deletes the list. No special handling is needed for a null head except creating a new head. The `const` correctness applies only to functions that should not modify the list; here, the head reference must be modifiable because insertion changes the head when inserting at the beginning.
