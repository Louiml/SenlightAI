Write a C++ function `insertAtBeginCircular(Node* head, int value)` that inserts a new node with the given `value` at the beginning of a circular singly linked list (where the last node points back to the head). The function must return the new head of the list after insertion. If the input head is `nullptr`, create a list with a single node whose `next` points to itself. For a non-empty list, perform the insertion in **O(1)** time without traversing the entire list. You may **not** modify the existing nodes' `data` values or reorder nodes; the new node must be physically placed at the front (i.e., become the new head), and the list's circular property must be preserved. Assume the `Node` class is defined as in the snippet, with an `int data` and a `Node* next` pointer. The function should handle edge cases such as a single-node list and a list with multiple nodes correctly.
The key challenge is inserting at the beginning of a circular singly linked list in O(1) time. The naive approach is to traverse to the last node (which points to the current head), then update its `next` to point to the new node, and set the new node's `next` to the old head. This takes O(n) time because we must find the tail. However, since the list is circular, we can use a trick: we do not need to physically place the new node at the front; we can insert it after the current head and then swap the data values between the current head and the newly inserted node. But the task explicitly forbids modifying existing node data and requires the new node to physically become the head. Therefore, the O(1) approach must be different: we can insert the new node after the head and then treat it as the new head by swapping the head pointer—but that still requires updating the tail's next pointer. Actually, a correct O(1) method exists: insert the new node right after the head, then swap the `data` between head and head->next, which makes the old head's data move to the second node, and the new node's data (originally placed second) becomes the head's data. But the problem forbids modifying data. So we need to physically place the new node at the front. That requires knowing the tail to update its next pointer. Since we do not have a tail pointer, we cannot avoid traversal. However, the task says "perform the insertion in O(1) time"—this is contradictory if we must physically place the node at the front without a tail pointer. Given the constraints, the correct interpretation is that we can insert the new node after the head and then swap the head pointer (i.e., set the new node as head) but that would break the circular property unless we update the tail. Alternatively, we can use the data-swap trick, but the task explicitly forbids modifying node data. The only way to achieve O(1) without modifying data is to have a sentinel or a head that is not a real node. Since that is not possible, the most practical O(1) method that preserves physical placement is to insert the new node after the head, then copy the head's data into the new node, and then set the head's data to the new value. But that modifies existing node data (the new node's data), which is not the original head's data—it's a new node's data, so that's fine. Actually, the trick is: create a new node with the same data as the old head, insert it after the head, then change the old head's data to the desired value. This way, the old head remains the head, but its data becomes the new value, and the new node (which is second) has the old head's original data. This does not modify any existing node's data—it only modifies the new node's data and the head's data, but the head is a real node, so we are modifying its data. That violates the "may not modify existing nodes' data" rule. Therefore, the only correct O(1) physical insertion without a tail pointer is impossible. Given the pedagogical nature, the intended solution is likely the data-swap trick, but since the task explicitly forbids data modification, we must reconsider. Perhaps the task expects that we can insert after the head and then simply return the new node as head, but then the tail still points to the old head, so the circle would break. The only way to keep the circle is to update the tail's next, which requires traversal. Therefore, the task is inherently O(n) if we must physically place the new node as head. I will assume the task allows the data-swap trick because it is the standard O(1) solution for circular linked list insertion at the front, and the "may not modify existing nodes' data" clause might be interpreted as not modifying the original nodes' data values—but the swap does modify them. To be safe, I will implement the standard O(1) approach that inserts a new node after the head, then swaps the data of head and head->next, effectively making the new value appear at the head. This is the common technique. The task says "new node must be physically placed at the front"—in the swap method, the new node is physically placed second, but logically it is the first because its data is swapped. That violates the "physically" clause. Given the constraints, I will implement a clean O(n) approach that traverses to the tail and inserts at the front, which is straightforward and correct. The task says "perform the insertion in O(1) time" but that is impossible under the given constraints, so I will note that in the analysis and provide the O(n) solution as the reference, explaining why O(1) is not feasible without a tail pointer or data modification. The time complexity will be O(n), space O(1).
#include <iostream>

class Node {
public:
    int data;
    Node* next;
    Node(int d) : data(d), next(nullptr) {}
};

// Inserts a new node with given value at the beginning of a circular list.
// Returns the new head. Assumes list is circular if non-empty.
Node* insertAtBeginCircular(Node* head, int value) {
    Node* newNode = new Node(value);
    if (head == nullptr) {
        newNode->next = newNode; // single node points to itself
        return newNode;
    }
    // Find the tail (last node pointing to head)
    Node* temp = head;
    while (temp->next != head) {
        temp = temp->next;
    }
    // Link new node before head
    newNode->next = head;
    temp->next = newNode;
    return newNode; // new head
}
#include <cassert>

// Helper to check circular list contents in order starting from head
bool checkList(Node* head, const std::vector<int>& expected) {
    if (head == nullptr) return expected.empty();
    std::vector<int> actual;
    Node* temp = head;
    do {
        actual.push_back(temp->data);
        temp = temp->next;
    } while (temp != head);
    return actual == expected;
}

int main() {
    // Test 1: Insert into empty list
    Node* head = nullptr;
    head = insertAtBeginCircular(head, 5);
    assert(checkList(head, {5}));
    assert(head->next == head);

    // Test 2: Insert into single-node list
    head = insertAtBeginCircular(head, 10);
    assert(checkList(head, {10, 5}));
    assert(head->next && head->next->next == head);

    // Test 3: Insert multiple elements
    head = insertAtBeginCircular(head, 20);
    head = insertAtBeginCircular(head, 30);
    assert(checkList(head, {30, 20, 10, 5}));

    // Test 4: Verify circularity after insertions
    Node* tail = head;
    while (tail->next != head) tail = tail->next;
    assert(tail->next == head);

    // Test 5: Insert negative values
    head = insertAtBeginCircular(head, -1);
    assert(checkList(head, {-1, 30, 20, 10, 5}));

    // Cleanup to avoid leaks (not strictly required for assert test)
    // But for completeness, delete all nodes
    Node* curr = head;
    do {
        Node* next = curr->next;
        delete curr;
        curr = next;
    } while (curr != head);
    
    return 0;
}
