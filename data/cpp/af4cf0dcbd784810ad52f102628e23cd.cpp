Write a C++ function `sortLinkedList` that takes the head pointer of a singly linked list of integers and sorts the list in ascending order by swapping node values, not by rearranging nodes. The linked list uses the `Node` structure with an integer `value` and a `next` pointer. The function should modify the list in place and return nothing (void). The input list will be non-empty, may contain duplicate values, and will not contain the sentinel value -1 as a data node. Your function must handle lists of arbitrary length efficiently and should not allocate new nodes, only swap values between existing nodes. The function should also work correctly when the list has only one node.

#include <cassert>
#include <vector>

// Helper to create a linked list from a vector of values.
Node* makeList(const std::vector<int>& values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : values) {
        Node* newNode = new Node(v);
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

// Helper to convert linked list to a vector for easy comparison.
std::vector<int> listToVector(Node* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->value);
        head = head->next;
    }
    return result;
}

// Helper to delete the list and free memory.
void deleteList(Node* head) {
    while (head != nullptr) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: basic unsorted list
    {
        Node* head = makeList({3, 1, 2});
        sortLinkedList(head);
        assert(listToVector(head) == std::vector<int>({1, 2, 3}));
        deleteList(head);
    }
    // Test 2: already sorted descending
    {
        Node* head = makeList({5, 4, 3, 2, 1});
        sortLinkedList(head);
        assert(listToVector(head) == std::vector<int>({1, 2, 3, 4, 5}));
        deleteList(head);
    }
    // Test 3: single element
    {
        Node* head = makeList({42});
        sortLinkedList(head);
        assert(listToVector(head) == std::vector<int>({42}));
        deleteList(head);
    }
    // Test 4: duplicates
    {
        Node* head = makeList({2, 2, 1, 2, 3, 1});
        sortLinkedList(head);
        assert(listToVector(head) == std::vector<int>({1, 1, 2, 2, 2, 3}));
        deleteList(head);
    }
    // Test 5: already sorted ascending
    {
        Node* head = makeList({1, 2, 3, 4});
        sortLinkedList(head);
        assert(listToVector(head) == std::vector<int>({1, 2, 3, 4}));
        deleteList(head);
    }
    return 0;
}

#include <bits/stdc++.h>

struct Node {
    int value;
    Node *next;
    explicit Node(int val) : value(val), next(nullptr) {}
};

// Sorts a singly linked list in ascending order by swapping node values.
// The list is modified in-place. No new nodes are allocated.
void sortLinkedList(Node *head) {
    if (head == nullptr) return;  // nothing to do for an empty list

    for (Node *i = head; i != nullptr && i->next != nullptr; i = i->next) {
        for (Node *j = i->next; j != nullptr; j = j->next) {
            if (i->value > j->value) {
                std::swap(i->value, j->value);
            }
        }
    }
}

// The core idea is a simple selection-sort-like approach directly on the linked list. We use two nested loops over the pointers: an outer loop traverses each node from head to the second-to-last node, and an inner loop traverses from the next node to the end, comparing the outer node's value with each subsequent node's value. Whenever a smaller value is found, we swap the values of the outer and inner nodes. After the outer loop completes, the list is sorted in ascending order. This is the same algorithm as in the snippet but with corrected loop conditions. Edge cases: an empty list (though the task says non-empty) should do nothing; a single node needs no swaps. Complexity: The outer loop runs \(n\) times and the inner loop runs up to \(n-i\) times, so total comparisons are \(O(n^2)\). No extra space is used beyond temporary variables for swapping, so auxiliary space is \(O(1)\). The node structure is standard with no extra fields.
