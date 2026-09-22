// Write a C++ function named `buildLinkedList` that reads a sequence of integers from standard input (first an integer `size`, then `size` integers) and returns the head pointer of a singly linked list created in the order the elements are entered. Each node must store an integer `data` and a pointer `next` to the following node. The last node's `next` must point to `nullptr`. The function must handle the edge case where `size` is 0 by returning `nullptr`. You may define your own node structure with the fields `data` and `next`. The function should use dynamic memory allocation (e.g., `new` in C++) for each node and must be self-contained (no external libraries beyond standard headers). Do not write a `main` function as part of the solution—only the function and the node definition.

#include <cassert>

// Helper to compare two linked lists by values.
bool equalLists(Node* a, Node* b) {
    while (a != nullptr && b != nullptr) {
        if (a->data != b->data) return false;
        a = a->next;
        b = b->next;
    }
    return a == nullptr && b == nullptr;
}

// Helper to delete a linked list.
void deleteList(Node* head) {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: Empty list (size = 0)
    {
        // Simulate input "0"
        std::stringstream ss("0");
        std::cin.rdbuf(ss.rdbuf());
        Node* result = buildLinkedList();
        assert(result == nullptr);
    }

    // Test 2: Single element
    {
        std::stringstream ss("1 42");
        std::cin.rdbuf(ss.rdbuf());
        Node* result = buildLinkedList();
        assert(result != nullptr);
        assert(result->data == 42 && result->next == nullptr);
        deleteList(result);
    }

    // Test 3: Multiple elements
    {
        std::stringstream ss("3 10 20 30");
        std::cin.rdbuf(ss.rdbuf());
        Node* result = buildLinkedList();
        Node* expected = new Node(10);
        expected->next = new Node(20);
        expected->next->next = new Node(30);
        assert(equalLists(result, expected));
        deleteList(result);
        deleteList(expected);
    }

    // Test 4: Negative and duplicate values
    {
        std::stringstream ss("4 -5 -5 7 7");
        std::cin.rdbuf(ss.rdbuf());
        Node* result = buildLinkedList();
        Node* expected = new Node(-5);
        expected->next = new Node(-5);
        expected->next->next = new Node(7);
        expected->next->next->next = new Node(7);
        assert(equalLists(result, expected));
        deleteList(result);
        deleteList(expected);
    }

    return 0;
}

#include <iostream>

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Builds a singly linked list from standard input: first an integer size, then size integers.
// Returns the head pointer of the list. Returns nullptr if size is 0.
Node* buildLinkedList() {
    int size;
    std::cin >> size;
    if (size <= 0) return nullptr;

    Node* head = nullptr;
    Node* tail = nullptr;

    for (int i = 0; i < size; ++i) {
        int value;
        std::cin >> value;
        Node* new_node = new Node(value);
        if (head == nullptr) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }
    if (tail != nullptr) {
        tail->next = nullptr;
    }
    return head;
}

// The solution involves reading the total number of elements first, then iterating exactly `size` times to read each integer and create a node. Use a `dummy` pointer (or track head and tail separately) to simplify insertion at the tail: start with `head = nullptr` and `tail = nullptr`. For each new value, allocate a node with `new Node(value)`. If the list is empty, set both `head` and `tail` to the new node; otherwise, set `tail->next = new_node` and move `tail` to `new_node`. After the loop, set `tail->next = nullptr` (or ensure it is already set). Edge case: if `size` is 0, return `nullptr` immediately without reading any further input. The time complexity is O(n) where n is the number of nodes, and space complexity is O(n) for storing the nodes. No special handling for duplicate values is needed.
