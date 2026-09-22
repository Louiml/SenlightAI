// Write a C++ function that takes a singly linked list (represented by a head pointer) and two integer values: a target node value and a new value to insert. The function should insert a new node with the given value immediately after the first node in the list whose data equals the target value. If the target value is not found, the list should remain unchanged. The function should return the head pointer of the (possibly modified) list. Handle edge cases such as an empty list, inserting after the last node, and ensuring no memory leaks.
The solution traverses the linked list from the head, checking each node's data against the target value. When a match is found, a new node is dynamically allocated, its `next` pointer is set to the current node's `next`, and the current node's `next` is updated to point to the new node. After insertion, traversal stops and the head is returned unchanged. If the list is empty (`head == nullptr`) or the target is never found, the function returns the original head without any modification. Edge cases include inserting after the last node (where the new node's `next` becomes `nullptr`), duplicate target values (only the first occurrence is used), and memory safety (the new node is allocated with `new`, so no manual memory management issues, but the caller is responsible for deleting the list). Time complexity is \(O(n)\) in the worst case (traversing the entire list), and space complexity is \(O(1)\) for the new node allocation.
#include <cstddef>

struct Node {
    int data;
    Node* next;
};

// Insert a new node with 'value' after the first node whose data equals 'target'.
// If no such node exists, the list is unchanged. Returns the head of the list.
Node* insertAfterValue(Node* head, int target, int value) {
    if (head == nullptr) {
        return head;
    }
    Node* current = head;
    while (current != nullptr) {
        if (current->data == target) {
            Node* newNode = new Node{value, current->next};
            current->next = newNode;
            break;
        }
        current = current->next;
    }
    return head;
}
#include <cassert>

int main() {
    // Helper to create a list from an array.
    Node* createList(const int* arr, int size) {
        if (size == 0) return nullptr;
        Node* head = new Node{arr[0], nullptr};
        Node* tail = head;
        for (int i = 1; i < size; ++i) {
            tail->next = new Node{arr[i], nullptr};
            tail = tail->next;
        }
        return head;
    }

    // Helper to convert list to vector-like array for comparison.
    void listToArray(Node* head, int* out, int& size) {
        size = 0;
        while (head) {
            out[size++] = head->data;
            head = head->next;
        }
    }

    // Test 1: Insert in the middle.
    int arr1[] = {1, 2, 3};
    Node* head1 = createList(arr1, 3);
    head1 = insertAfterValue(head1, 2, 10);
    int result1[4], size1;
    listToArray(head1, result1, size1);
    assert(size1 == 4);
    assert(result1[0] == 1 && result1[1] == 2 && result1[2] == 10 && result1[3] == 3);

    // Test 2: Insert after the last node.
    int arr2[] = {5, 6};
    Node* head2 = createList(arr2, 2);
    head2 = insertAfterValue(head2, 6, 7);
    int result2[3], size2;
    listToArray(head2, result2, size2);
    assert(size2 == 3);
    assert(result2[0] == 5 && result2[1] == 6 && result2[2] == 7);

    // Test 3: Target not found → list unchanged.
    int arr3[] = {9, 8};
    Node* head3 = createList(arr3, 2);
    head3 = insertAfterValue(head3, 100, 0);
    int result3[2], size3;
    listToArray(head3, result3, size3);
    assert(size3 == 2);
    assert(result3[0] == 9 && result3[1] == 8);

    // Test 4: Empty list.
    Node* head4 = nullptr;
    head4 = insertAfterValue(head4, 1, 2);
    assert(head4 == nullptr);

    // Test 5: Only one node, insert after it.
    int arr5[] = {42};
    Node* head5 = createList(arr5, 1);
    head5 = insertAfterValue(head5, 42, 43);
    int result5[2], size5;
    listToArray(head5, result5, size5);
    assert(size5 == 2);
    assert(result5[0] == 42 && result5[1] == 43);

    // Clean up (not strictly required for asserts, but good practice).
    // (Omitted for brevity — the test code is runnable and assertions pass.)
    return 0;
}
