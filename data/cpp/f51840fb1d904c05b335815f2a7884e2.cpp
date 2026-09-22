// Write a C++ function `int findPositionFromEnd(Node* head, int value)` that, given a singly linked list (where each `Node` has an `int data` and a `Node* next`), returns the 1-based position of the last occurrence of a given value in the list, counting from the end (i.e., the tail is position 1). If the value is absent, return -1. The list may contain duplicate values, and you must handle empty lists and a `value` that appears multiple times correctly.
The solution requires two passes for clarity, but a more space-efficient single-pass approach using a "runner" pointer can also be used. The simplest approach: first determine the total length `n` of the list by traversing from head to null (counting nodes). Then, traverse again and for each node, compute its position from the end as `n - currentIndex + 1` (where currentIndex is 1-based from head). When a node's data equals `value`, update the answer to this computed position. Continue traversing to ensure you find the last occurrence (closest to tail). At the end, if no match was found, return -1. Edge cases: empty list (head == nullptr) returns -1; value at the tail gives 1; duplicates: the last occurrence closest to the tail is returned. Time complexity is O(n) for the two passes (or O(n) with a single pass using a window), and space complexity is O(1) auxiliary.
#include <cstddef>

struct Node {
    int data;
    Node* next;
};

// Return 1-based position from the end of the last occurrence of `value`, or -1 if absent.
int findPositionFromEnd(Node* head, int value) {
    if (head == nullptr) {
        return -1; // empty list
    }

    // First pass: compute total length
    int length = 0;
    for (Node* curr = head; curr != nullptr; curr = curr->next) {
        ++length;
    }

    // Second pass: find last occurrence and compute its position from end
    int positionFromEnd = -1;
    int indexFromHead = 1;
    for (Node* curr = head; curr != nullptr; curr = curr->next) {
        if (curr->data == value) {
            positionFromEnd = length - indexFromHead + 1;
        }
        ++indexFromHead;
    }

    return positionFromEnd;
}
#include <cassert>

int main() {
    // Helper to build list from array (for testing only, not part of solution)
    Node* buildList(int arr[], int n) {
        if (n == 0) return nullptr;
        Node* head = new Node{arr[0], nullptr};
        Node* tail = head;
        for (int i = 1; i < n; ++i) {
            tail->next = new Node{arr[i], nullptr};
            tail = tail->next;
        }
        return head;
    }

    // Test 1: empty list
    Node* head1 = nullptr;
    assert(findPositionFromEnd(head1, 5) == -1);

    // Test 2: single node, value present
    int arr2[] = {7};
    Node* head2 = buildList(arr2, 1);
    assert(findPositionFromEnd(head2, 7) == 1);

    // Test 3: value absent
    int arr3[] = {1, 2, 3};
    Node* head3 = buildList(arr3, 3);
    assert(findPositionFromEnd(head3, 9) == -1);

    // Test 4: last occurrence at tail
    int arr4[] = {5, 5, 1, 5};
    Node* head4 = buildList(arr4, 4);
    assert(findPositionFromEnd(head4, 5) == 1);

    // Test 5: last occurrence in middle
    int arr5[] = {3, 2, 3, 1};
    Node* head5 = buildList(arr5, 4);
    assert(findPositionFromEnd(head5, 3) == 3);

    // Test 6: value at head only
    int arr6[] = {8, 0, 2};
    Node* head6 = buildList(arr6, 3);
    assert(findPositionFromEnd(head6, 8) == 3);

    // Test 7: all same values
    int arr7[] = {4, 4, 4};
    Node* head7 = buildList(arr7, 3);
    assert(findPositionFromEnd(head7, 4) == 1);

    return 0;
}
