// Write a C++ function `addOneToList(Node* head)` that takes the head of a singly linked list where each node contains a single decimal digit (0-9) and represents the digits of a non-negative integer in most-significant-digit-first order. The function must add 1 to the number formed by concatenating all node values in order and return the head of the modified linked list. If adding 1 causes an additional most-significant digit (e.g., 999 → 1000), a new node must be created at the front of the list. The function should work in-place without using extra linked-list nodes except when a new leading digit is required, and must run in O(n) time with O(1) auxiliary space.
// The main idea is to process the addition from the least significant digit to the most significant digit, since carrying only propagates backward. This can be done by first reversing the linked list so that the least significant digit becomes the head. Then traverse the reversed list with an initial carry of 1, updating each node's data to `(current_data + carry) % 10` and computing the new carry as the integer division of `(current_data + carry)` by 10. After the traversal, if there is still a remaining carry (which only happens when the entire number was 999...9), append a new node with that carry value at the end of the reversed list. Finally, reverse the list again to restore the original most-significant-digit-first order and return the head. Edge cases include an empty list (though constraints guarantee non-empty), a single-digit node such as 9 that becomes 10 (requiring a new node), and numbers with trailing zeros that are unaffected by carry (e.g., 120 → 121). Time complexity is O(n) because we traverse the list three times (reverse, add, reverse), and auxiliary space is O(1) besides the possible single new node.
#include <cstddef>

// Node structure for the linked list
struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Helper to reverse a linked list iteratively
Node* reverseList(Node* head) {
    Node* prev = nullptr;
    Node* curr = head;
    while (curr) {
        Node* nextNode = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextNode;
    }
    return prev;
}

// Add 1 to the number represented by the linked list (most-significant digit first)
Node* addOneToList(Node* head) {
    if (!head) return nullptr;

    // Reverse the list so we traverse from least significant digit
    head = reverseList(head);

    Node* curr = head;
    int carry = 1; // initial add of 1

    while (curr) {
        int sum = curr->data + carry;
        curr->data = sum % 10;
        carry = sum / 10;
        curr = curr->next;
    }

    // If carry remains after the last digit, append a new node
    if (carry) {
        // Find the tail (last node) of the reversed list
        Node* tail = head;
        while (tail->next) {
            tail = tail->next;
        }
        tail->next = new Node(carry);
    }

    // Reverse back to original order
    return reverseList(head);
}
#include <cassert>

// Helper to build a list from an array
Node* buildList(int arr[], int size) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int i = 0; i < size; ++i) {
        Node* newNode = new Node(arr[i]);
        if (!head) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

// Helper to convert list to a long value for checking
long listToNumber(Node* head) {
    long num = 0;
    while (head) {
        num = num * 10 + head->data;
        head = head->next;
    }
    return num;
}

// Helper to free list memory
void freeList(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: 4->5->6 -> 457
    {
        int arr[] = {4, 5, 6};
        Node* head = buildList(arr, 3);
        Node* result = addOneToList(head);
        assert(listToNumber(result) == 457);
        freeList(result);
    }

    // Test 2: 1->2->3 -> 124
    {
        int arr[] = {1, 2, 3};
        Node* head = buildList(arr, 3);
        Node* result = addOneToList(head);
        assert(listToNumber(result) == 124);
        freeList(result);
    }

    // Test 3: 9 -> 10
    {
        int arr[] = {9};
        Node* head = buildList(arr, 1);
        Node* result = addOneToList(head);
        assert(listToNumber(result) == 10);
        freeList(result);
    }

    // Test 4: 9->9->9 -> 1000
    {
        int arr[] = {9, 9, 9};
        Node* head = buildList(arr, 3);
        Node* result = addOneToList(head);
        assert(listToNumber(result) == 1000);
        freeList(result);
    }

    // Test 5: 1->0->0 -> 101
    {
        int arr[] = {1, 0, 0};
        Node* head = buildList(arr, 3);
        Node* result = addOneToList(head);
        assert(listToNumber(result) == 101);
        freeList(result);
    }

    // Test 6: 0 -> 1
    {
        int arr[] = {0};
        Node* head = buildList(arr, 1);
        Node* result = addOneToList(head);
        assert(listToNumber(result) == 1);
        freeList(result);
    }

    // Test 7: 1->2->9 -> 130
    {
        int arr[] = {1, 2, 9};
        Node* head = buildList(arr, 3);
        Node* result = addOneToList(head);
        assert(listToNumber(result) == 130);
        freeList(result);
    }

    return 0;
}
