// Write a C++ function named `sortLinkedList` that takes a singly linked list (represented by a pointer to its head node) and sorts it in ascending order using a single pass of adjacent swaps, according to the behavior shown in the given code snippet. The function should modify the list in place and return nothing. The linked list nodes must be defined as a struct with an integer `data` field and a pointer to the next node. The input list may be empty or contain arbitrary integers (including duplicates). The sorting behavior should match the snippet's `bubblesort` method: only one full pass from head to tail is performed, swapping adjacent elements when the current one is greater than the next. This does not fully sort the list in general, but the task is to replicate that exact algorithm. Handle edge cases: if the list is empty or has only one node, do nothing. Provide a robust implementation that avoids memory leaks (but since no allocation happens in the sort, just ensure proper traversal).

The solution must exactly replicate the given code's `bubblesort` logic. The algorithm is a single pass bubble sort: starting from the head, compare each node's data with the next node's data; if the current is greater, swap the values. Then advance to the next node. This pass runs until the second-to-last node is compared with the last node (i.e., while the current node has a next). Important edge cases: an empty list (head is `nullptr`) or a list with only one node should simply return without any action. Duplicate values are handled naturally because no swap occurs when values are equal. Time complexity is \(O(n)\) for a single pass over \(n\) nodes, and space complexity is \(O(1)\) as only a temporary pointer and integer are used. Note: This single pass does not fully sort the list in general, but the task explicitly asks to replicate that behavior, so we must not implement a full bubble sort with multiple passes.

#include <cstddef>

struct Node {
    int data;
    Node* next;
};

// Perform a single pass of adjacent swaps to partially sort the linked list.
// Matches the behavior of the original snippet's bubblesort method.
void sortLinkedList(Node* head) {
    // If the list is empty or has only one node, nothing to do.
    if (head == nullptr || head->next == nullptr) {
        return;
    }

    Node* current = head;
    // Traverse until the second-to-last node.
    while (current->next != nullptr) {
        // If current node's data is greater than next node's data, swap values.
        if (current->data > current->next->data) {
            int temp = current->data;
            current->data = current->next->data;
            current->next->data = temp;
        }
        current = current->next;
    }
}

#include <cassert>

// Helper function to create a list from an initializer list for testing.
#include <initializer_list>

Node* createList(std::initializer_list<int> values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int val : values) {
        Node* newNode = new Node{val, nullptr};
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

// Helper function to delete the entire list.
void deleteList(Node* head) {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

// Helper function to check if the list matches an expected sequence.
bool listEquals(Node* head, std::initializer_list<int> expected) {
    const int* ptr = expected.begin();
    while (head != nullptr && ptr != expected.end()) {
        if (head->data != *ptr) return false;
        head = head->next;
        ptr++;
    }
    return (head == nullptr && ptr == expected.end());
}

int main() {
    // Test 1: Example from the snippet: 3 1 7 0 9 -> after one pass: 1 3 0 7 9
    {
        Node* list = createList({3, 1, 7, 0, 9});
        sortLinkedList(list);
        assert(listEquals(list, {1, 3, 0, 7, 9}));
        deleteList(list);
    }

    // Test 2: Already sorted list remains unchanged.
    {
        Node* list = createList({1, 2, 3, 4});
        sortLinkedList(list);
        assert(listEquals(list, {1, 2, 3, 4}));
        deleteList(list);
    }

    // Test 3: List in descending order gets one swap: 4 3 2 1 -> 3 4 2 1
    {
        Node* list = createList({4, 3, 2, 1});
        sortLinkedList(list);
        assert(listEquals(list, {3, 4, 2, 1}));
        deleteList(list);
    }

    // Test 4: Single element list unchanged.
    {
        Node* list = createList({5});
        sortLinkedList(list);
        assert(listEquals(list, {5}));
        deleteList(list);
    }

    // Test 5: Empty list (nullptr) should not crash.
    {
        Node* list = nullptr;
        sortLinkedList(list);
        assert(list == nullptr);
    }

    // Test 6: Duplicate values: 2 2 1 -> after one pass: 2 1 2
    {
        Node* list = createList({2, 2, 1});
        sortLinkedList(list);
        assert(listEquals(list, {2, 1, 2}));
        deleteList(list);
    }

    // Test 7: Two elements: 5 3 -> 3 5
    {
        Node* list = createList({5, 3});
        sortLinkedList(list);
        assert(listEquals(list, {3, 5}));
        deleteList(list);
    }

    // Test 8: Negative numbers: -1 -5 0 -> -5 -1 0
    {
        Node* list = createList({-1, -5, 0});
        sortLinkedList(list);
        assert(listEquals(list, {-5, -1, 0}));
        deleteList(list);
    }

    return 0;
}
