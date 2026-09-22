/*
Write a C++ function named `insertSortedDescending` that takes a `List` structure (defined as having a `Node* head` and `Node* tail`, where `Node` stores an integer `value` and pointers to `next` and `prev`) and an integer `value`. The function must insert the value into the doubly linked list so that the list remains sorted in **descending** order (largest to smallest). The `List` may be empty; duplicate values are allowed and should be inserted after any existing equal values (stable insertion). The function must correctly maintain both the `head` and `tail` pointers and all `prev`/`next` links. The function should not print anything and must handle memory allocation for the new node. Implement the function in a self-contained manner with necessary includes, and ensure it is const-correct where applicable (e.g., if a helper to traverse exists, it should take `const Node*`).
*/
#include <cstddef>

struct Node {
    int value;
    Node* next;
    Node* prev;
};

struct List {
    Node* head = nullptr;
    Node* tail = nullptr;
};

// Helper: create a new node with given value, next/prev set to nullptr.
Node* createNode(int value) {
    Node* newNode = new Node;
    newNode->value = value;
    newNode->next = nullptr;
    newNode->prev = nullptr;
    return newNode;
}

// Insert value into the list so it stays sorted in descending order.
// Stable: duplicates are inserted after existing equal values.
// Maintains head and tail pointers correctly.
void insertSortedDescending(List& list, int value) {
    Node* newNode = createNode(value);

    // Case 1: empty list
    if (list.head == nullptr) {
        list.head = newNode;
        list.tail = newNode;
        return;
    }

    // Case 2: insert at front (new value is strictly greater than head)
    if (value > list.head->value) {
        newNode->next = list.head;
        list.head->prev = newNode;
        list.head = newNode;
        return;
    }

    // Case 3: insert at tail (new value is strictly less than tail)
    if (value < list.tail->value) {
        newNode->prev = list.tail;
        list.tail->next = newNode;
        list.tail = newNode;
        return;
    }

    // Case 4: value is between head and tail (or equal to some middle values)
    // Traverse while the next node's value is greater than the new value.
    // Because list is descending, this stops when next value <= new value.
    Node* current = list.head;
    while (current->next != nullptr && current->next->value > value) {
        current = current->next;
    }

    // Insert after current, before current->next (if exists)
    newNode->next = current->next;
    if (current->next != nullptr) {
        current->next->prev = newNode;
    } else {
        // If current is the tail, update tail
        list.tail = newNode;
    }
    current->next = newNode;
    newNode->prev = current;
}
#include <cassert>

// Assume the solution function and structs are defined above (or included here).

int main() {
    // Helper to check list from head to tail
    auto checkList = [](const List& list, const std::initializer_list<int>& expected) {
        Node* cur = list.head;
        auto it = expected.begin();
        for (; cur != nullptr && it != expected.end(); ++cur, ++it) {
            assert(cur->value == *it);
        }
        assert(cur == nullptr && it == expected.end()); // both ended together
        // Also check tail link backwards
        if (list.tail != nullptr) {
            Node* back = list.tail;
            auto rit = expected.end();
            --rit;
            for (; back != nullptr && rit != expected.begin() - 1; back = back->prev, --rit) {
                assert(back->value == *rit);
            }
            assert(back == nullptr);
        }
    };

    // Test 1: empty list insertion
    List l1;
    insertSortedDescending(l1, 5);
    checkList(l1, {5});
    assert(l1.head == l1.tail);

    // Test 2: multiple insertions (descending)
    List l2;
    insertSortedDescending(l2, 3);
    insertSortedDescending(l2, 10);
    insertSortedDescending(l2, 7);
    insertSortedDescending(l2, 1);
    checkList(l2, {10, 7, 3, 1});

    // Test 3: duplicates stable (insert equal after existing)
    List l3;
    insertSortedDescending(l3, 5);
    insertSortedDescending(l3, 5);
    insertSortedDescending(l3, 5);
    checkList(l3, {5, 5, 5});

    // Test 4: duplicates with other numbers
    List l4;
    insertSortedDescending(l4, 9);
    insertSortedDescending(l4, 9);
    insertSortedDescending(l4, 4);
    insertSortedDescending(l4, 9);
    insertSortedDescending(l4, 7);
    checkList(l4, {9, 9, 9, 7, 4});

    // Test 5: all equal, insertion works
    List l5;
    insertSortedDescending(l5, 2);
    insertSortedDescending(l5, 2);
    insertSortedDescending(l5, 2);
    checkList(l5, {2, 2, 2});

    // Test 6: large number at end, small at front
    List l6;
    insertSortedDescending(l6, 100);
    insertSortedDescending(l6, -5);
    insertSortedDescending(l6, 50);
    checkList(l6, {100, 50, -5});

    // Test 7: negative numbers
    List l7;
    insertSortedDescending(l7, -3);
    insertSortedDescending(l7, -10);
    insertSortedDescending(l7, -1);
    checkList(l7, {-1, -3, -10});

    // Test 8: only two elements
    List l8;
    insertSortedDescending(l8, 1);
    insertSortedDescending(l8, 2);
    checkList(l8, {2, 1});

    // Test 9: insertion at front then middle then tail
    List l9;
    insertSortedDescending(l9, 10);
    insertSortedDescending(l9, 1);
    insertSortedDescending(l9, 5);
    insertSortedDescending(l9, 3);
    checkList(l9, {10, 5, 3, 1});

    // Test 10: extensive random-like sequence
    List l10;
    int values[] = {4, 7, 1, 9, 3, 9, 2, 7, 8, 5};
    for (int v : values) insertSortedDescending(l10, v);
    checkList(l10, {9, 9, 8, 7, 7, 5, 4, 3, 2, 1});

    // No memory leak check in this simple test, but for completeness,
    // we could delete nodes (not required for assert test).

    return 0;
}
// The main algorithm is similar to a sorted insertion in a doubly linked list, but with reversed comparison logic. We first create a new node with `initialize` (which sets `value`, `next`, and `prev` to `nullptr`). Then we handle four cases:
//
// 1. **Empty list**: If `list.head` is `nullptr`, set both `head` and `tail` to the new node.
// 2. **Insert at front**: If the new value is **greater than or equal** to `list.head->value` (since descending order, larger values go first), set `newNode->next = list.head`, set `list.head->prev = newNode`, and update `list.head = newNode`. Because we want stable insertion for duplicates, we use `>` instead of `>=` when checking if it should go strictly before the head; that is, if `value > head->value`, insert at front; if `value == head->value`, we should insert after all equal values, so we don't use >=. However, in descending order, equal values are inserted after existing ones, so we only insert at front if `value > head->value`.
// 3. **Insert at tail**: If the new value is **less than** `list.tail->value`, insert at the end: set `newNode->prev = list.tail`, set `list.tail->next = newNode`, then update `list.tail = newNode`. Again, we use `<` strictly; equal values should not go to the tail if there are existing equal values (they go after them, which may be not at the tail if there are smaller values later).
// 4. **Insert in middle**: Otherwise, traverse from `head` forward while the next node's value is **greater than** the new value (i.e., `current->next->value > value`). Stop when we find a node whose next value is less than or equal to the new value (or we reach the tail area). Actually, for descending order, we want to keep going while the next node's value is greater than the new value, because we want to insert before the first smaller or equal value. Then insert `newNode` between `current` and `current->next`: set `newNode->next = current->next`, `current->next->prev = newNode`, `current->next = newNode`, `newNode->prev = current`.
//
// Edge cases: empty list, single element, duplicate values (must be inserted after existing duplicates), and values that are larger than head or smaller than tail. The algorithm runs in O(n) worst-case time (traversal) and O(1) auxiliary space. Memory: one new node allocation.
