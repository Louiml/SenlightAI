/*
Write a C++ function named `cleanList` that takes a linked list built from the provided `Node` structure (with `int val` and `Node* next` members) and an integer `target`. The function should remove all nodes whose value is divisible by `target` (i.e., `val % target == 0`), maintaining the relative order of the remaining nodes. If the list becomes empty or the head is null, the function should do nothing. The function must modify the list in-place and return a pointer to the new head of the list. Assume `target` is non-zero. The function should handle deletion properly by freeing memory of removed nodes to avoid leaks, and the head pointer must be updated appropriately if the original head is removed. For example, given a list `[1, 2, 3, 4, 5]` and `target = 2`, the output list should be `[1, 3, 5]`.
*/

#include <cstddef>

struct Node {
    int val;
    Node* next;
};

// Remove all nodes whose value is divisible by target (non-zero).
// Returns the new head of the list, which may be null.
Node* cleanList(Node* head, int target) {
    // Handle empty list or null target (though target is guaranteed non-zero).
    if (head == nullptr || target == 0) {
        return head;
    }

    // Remove nodes at the head that are divisible by target.
    while (head != nullptr && head->val % target == 0) {
        Node* toDelete = head;
        head = head->next;
        delete toDelete;
    }

    // Now process the rest of the list.
    Node* current = head;
    while (current != nullptr && current->next != nullptr) {
        if (current->next->val % target == 0) {
            Node* toDelete = current->next;
            current->next = toDelete->next;
            delete toDelete;
        } else {
            current = current->next;
        }
    }

    return head;
}

#include <cassert>
#include <iostream>

// Node and cleanList definitions should be here (in the same translation unit).

// Helper to build a list from an array.
Node* makeList(const int* arr, int size) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int i = 0; i < size; ++i) {
        Node* n = new Node{arr[i], nullptr};
        if (head == nullptr) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
    }
    return head;
}

// Helper to convert list to vector-like check.
bool listEquals(Node* head, const int* expected, int size) {
    Node* curr = head;
    int idx = 0;
    while (curr != nullptr && idx < size) {
        if (curr->val != expected[idx]) return false;
        curr = curr->next;
        idx++;
    }
    return (curr == nullptr && idx == size);
}

// Helper to free list.
void freeList(Node* head) {
    while (head != nullptr) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: remove evens from 1..5 -> [1,3,5]
    {
        int init[] = {1, 2, 3, 4, 5};
        Node* head = makeList(init, 5);
        head = cleanList(head, 2);
        int expected[] = {1, 3, 5};
        assert(listEquals(head, expected, 3));
        freeList(head);
    }

    // Test 2: all multiples of 3 -> becomes empty
    {
        int init[] = {3, 6, 9};
        Node* head = makeList(init, 3);
        head = cleanList(head, 3);
        assert(head == nullptr);
        freeList(head);
    }

    // Test 3: head removed, then rest kept
    {
        int init[] = {4, 7, 8, 5};
        Node* head = makeList(init, 4);
        head = cleanList(head, 2);
        int expected[] = {7, 5};
        assert(listEquals(head, expected, 2));
        freeList(head);
    }

    // Test 4: single element not divisible
    {
        int init[] = {5};
        Node* head = makeList(init, 1);
        head = cleanList(head, 2);
        int expected[] = {5};
        assert(listEquals(head, expected, 1));
        freeList(head);
    }

    // Test 5: single element divisible
    {
        int init[] = {4};
        Node* head = makeList(init, 1);
        head = cleanList(head, 2);
        assert(head == nullptr);
        freeList(head);
    }

    // Test 6: empty list
    {
        Node* head = nullptr;
        head = cleanList(head, 2);
        assert(head == nullptr);
    }

    // Test 7: large target (no removals)
    {
        int init[] = {1, 2, 3};
        Node* head = makeList(init, 3);
        head = cleanList(head, 10);
        int expected[] = {1, 2, 3};
        assert(listEquals(head, expected, 3));
        freeList(head);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The core algorithm is a single-pass traversal with two pointers: `current` (the node being examined) and `previous` (the last node that was kept). Iterate from the head. For each node, check if `current->val % target == 0`. If yes, unlink the node by adjusting `previous->next` to skip it, save its next pointer, delete the node, and move `current` to the saved next. If no, advance both `previous` and `current`. The head requires special handling: if the head is removed, update the head pointer to the next node before continuing. Edge cases include an empty list (return null), a single node that is removed (head becomes null), and a list where all nodes are removed. Time complexity is O(n) where n is the number of nodes, because each node is visited exactly once. Space complexity is O(1) aside from the input list, since only a few pointers are used. Memory management is explicit via `delete` for removed nodes.
