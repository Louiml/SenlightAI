/*
Write a C++ function named `insertSorted` that inserts a new integer value into a **sorted** singly linked list (ascending order) while maintaining the sorted order. The list is represented by a `Node` struct containing an `int data` and a `Node* next` pointer. The function should take the head pointer of the list (by reference) and the new integer value, insert the value in the correct sorted position, and return the head pointer. If the list is empty, the new node becomes the head. Handle duplicate values by inserting the new node **after** all existing nodes with the same value (stable insertion). Do not use any standard library containers (e.g., `std::list`, `std::vector`) — only raw pointers and dynamic allocation.
*/
#include <iostream>

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Inserts a value into a sorted (ascending) singly linked list while maintaining sort order.
// For duplicate values, the new node is placed after all existing nodes with the same value.
// Returns the (possibly new) head of the list.
Node* insertSorted(Node* head, int value) {
    Node* newNode = new Node(value);

    // Case: empty list or value smaller than the head's data
    if (head == nullptr || head->data > value) {
        newNode->next = head;
        return newNode;
    }

    // Traverse to find the correct insertion point.
    // `prev` will be the node after which we insert (last node with data <= value).
    Node* prev = head;
    Node* curr = head->next;
    while (curr != nullptr && curr->data <= value) {
        prev = curr;
        curr = curr->next;
    }

    // Insert new node between prev and curr
    newNode->next = curr;
    prev->next = newNode;

    return head;
}
#include <cassert>

int main() {
    // Helper to create a list from an array (for testing)
    Node* makeList(int arr[], int size) {
        if (size == 0) return nullptr;
        Node* head = new Node(arr[0]);
        Node* tail = head;
        for (int i = 1; i < size; ++i) {
            tail->next = new Node(arr[i]);
            tail = tail->next;
        }
        return head;
    }

    // Helper to convert list to vector (for comparison)
    std::vector<int> toVector(Node* head) {
        std::vector<int> v;
        while (head) {
            v.push_back(head->data);
            head = head->next;
        }
        return v;
    }

    // Test 1: Insert into empty list
    Node* head = nullptr;
    head = insertSorted(head, 5);
    assert(toVector(head) == std::vector<int>({5}));

    // Test 2: Insert at beginning
    head = insertSorted(head, 1);
    assert(toVector(head) == std::vector<int>({1, 5}));

    // Test 3: Insert in middle
    head = insertSorted(head, 3);
    assert(toVector(head) == std::vector<int>({1, 3, 5}));

    // Test 4: Insert at end
    head = insertSorted(head, 10);
    assert(toVector(head) == std::vector<int>({1, 3, 5, 10}));

    // Test 5: Insert duplicate (should go after existing same values)
    head = insertSorted(head, 3);
    assert(toVector(head) == std::vector<int>({1, 3, 3, 5, 10}));

    // Test 6: Insert another duplicate at start
    head = insertSorted(head, 1);
    assert(toVector(head) == std::vector<int>({1, 1, 3, 3, 5, 10}));

    // Test 7: Insert negative numbers
    head = insertSorted(head, -2);
    assert(toVector(head) == std::vector<int>({-2, 1, 1, 3, 3, 5, 10}));

    // Test 8: Insert into list with all duplicates
    Node* dupHead = makeList(new int[]{7, 7, 7}, 3);
    dupHead = insertSorted(dupHead, 7);
    assert(toVector(dupHead) == std::vector<int>({7, 7, 7, 7}));

    // Test 9: Insert multiple values sequentially
    Node* head2 = nullptr;
    head2 = insertSorted(head2, 4);
    head2 = insertSorted(head2, 2);
    head2 = insertSorted(head2, 6);
    head2 = insertSorted(head2, 2);
    assert(toVector(head2) == std::vector<int>({2, 2, 4, 6}));

    // Test 10: Insert value that is smaller than all but not at head (already covered by negative test)

    return 0;
}
// The solution traverses the singly linked list to find the correct insertion point. Since the list is sorted ascending, we need to locate the node **after which** the new value should be inserted — specifically, the last node whose `data` is **less than or equal** to the new value (to ensure stability for duplicates). We maintain two pointers: `current` (the node being examined) and `previous` (the node before `current`). Start with `previous = nullptr` and `current = head`. While `current != nullptr` and `current->data <= value`, move both pointers forward. After the loop, `previous` is the node after which we insert (or `nullptr` if we insert at the head). Create a new node with the value, set its `next` to `current`, and if `previous` is null, update the head; otherwise set `previous->next` to the new node. Edge cases: empty list (head becomes new node), insertion at the beginning (value smaller than all existing), insertion at the end (value greater than all existing), and duplicates (loop continues past equal values, so new node goes after them). Time complexity is O(n) in the worst case (traversal) and O(1) time to insert once the position is found; space complexity is O(1) for the new node.
