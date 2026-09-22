/*
Given a circular linked list with at least one node, write a C++ function `void insertInCircularList(Node*& tail, int element, int data)` that inserts a new node with value `data` after the first node in the circular list whose value equals `element`. If `tail` is `nullptr`, create a circular list with a single node containing `data`. If no node with value `element` exists, append the new node after the current `tail` node (so the new node becomes the new tail). The list must remain circular and singly linked after every insertion. The function should handle duplicate values by only considering the first match encountered when traversing from `tail`. The function must work with the `Node` struct that has an `int data` member and a `Node* next` pointer, and that includes a destructor that recursively deletes the list when the last node is deleted (do not call delete inside this function yourself).
*/
#include <cstddef>

// Node structure for a singly linked circular list
struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
    ~Node() {
        if (next != nullptr) {
            delete next;
            next = nullptr;
        }
    }
};

// Insert a new node with value 'data' after the first node equal to 'element'
// If tail is null, create a single-node circular list.
// If element not found, insert after tail (new node becomes the new tail).
void insertInCircularList(Node*& tail, int element, int data) {
    if (tail == nullptr) {
        Node* newNode = new Node(data);
        newNode->next = newNode;
        tail = newNode;
        return;
    }

    Node* curr = tail;
    bool found = false;

    // Traverse the circular list at least once, stopping when we return to tail
    do {
        if (curr->data == element) {
            found = true;
            break;
        }
        curr = curr->next;
    } while (curr != tail);

    // If not found, curr is tail; we should insert after tail
    if (!found) {
        // curr is still tail
    }

    Node* newNode = new Node(data);
    newNode->next = curr->next;
    curr->next = newNode;

    // If we inserted right after the old tail, the new node becomes the new tail
    if (curr == tail) {
        tail = newNode;
    }
}
#include <cassert>

// Global main for testing
int main() {
    // Test 1: Insert into empty list
    Node* tail = nullptr;
    insertInCircularList(tail, 5, 3);
    assert(tail != nullptr);
    assert(tail->data == 3);
    assert(tail->next == tail);

    // Test 2: Insert after existing element (element found)
    insertInCircularList(tail, 3, 7);
    // List should be: 3 -> 7 -> (back to 3), tail is now 7
    assert(tail->data == 7);
    assert(tail->next->data == 3);
    assert(tail->next->next == tail);

    // Test 3: Insert after element not found -> appends after current tail
    insertInCircularList(tail, 99, 10);
    // List: 3 -> 7 -> 10 -> (back to 3), tail is now 10
    assert(tail->data == 10);
    assert(tail->next->data == 3);
    assert(tail->next->next->data == 7);
    assert(tail->next->next->next == tail);

    // Test 4: Insert after an element that appears later in traversal from tail
    // Current list from tail (10): 10 -> 3 -> 7 -> (back to 10)
    insertInCircularList(tail, 3, 5);
    // After 3, new node 5: list becomes 10 -> 3 -> 5 -> 7 -> (back to 10), tail stays 10
    assert(tail->data == 10);
    assert(tail->next->data == 3);
    assert(tail->next->next->data == 5);
    assert(tail->next->next->next->data == 7);
    assert(tail->next->next->next->next == tail);

    // Test 5: Insert after element equal to tail's data (found at tail)
    insertInCircularList(tail, 10, 20);
    // After 10, new node 20, tail becomes 20 because we insert right after old tail
    assert(tail->data == 20);
    assert(tail->next->data == 10);
    assert(tail->next->next->data == 3);
    assert(tail->next->next->next->data == 5);
    assert(tail->next->next->next->next->data == 7);
    assert(tail->next->next->next->next->next == tail);

    // Test 6: Insert element that matches first encountered from tail after a full cycle
    // Current list from tail (20): 20 -> 10 -> 3 -> 5 -> 7 -> (back to 20)
    insertInCircularList(tail, 5, 6);
    // First 5 found at position after 3, insert 6 after 5, tail remains 20
    assert(tail->data == 20);
    assert(tail->next->data == 10);
    assert(tail->next->next->data == 3);
    assert(tail->next->next->next->data == 5);
    assert(tail->next->next->next->next->data == 6);
    assert(tail->next->next->next->next->next->data == 7);
    assert(tail->next->next->next->next->next->next == tail);

    // Clean up to avoid memory leak; since we use recursive destructor, delete tail once
    delete tail;

    return 0;
}
// The main algorithm uses a single traversal of the circular list starting from the `tail` pointer. First, check if `tail` is `nullptr`; if so, create a new node, set its `next` to itself, assign it as the new `tail`, and return. Otherwise, traverse using a `curr` pointer initialized to `tail`. In each step, check if `curr->data == element`; if found, create the new node, link it after `curr` (set `newNode->next = curr->next` then `curr->next = newNode`), and if `curr` was the tail, the new node becomes the new tail (because the new node is now the last in the circular order). If the entire list is traversed without finding `element`, the loop will eventually return to `tail`; at that point, we insert the new node right after `tail` (i.e., before what was originally `tail->next`), making the new node the new tail. A key edge case is that when the list contains only one node and `element` equals that node's data, insertion after that node is straightforward; if the list has exactly one node and `element` is not found, we still insert after `tail` and the new node becomes the new tail, leaving the list with two nodes in a circular order. Another edge case is when `element` matches the tail's data but also appears earlier; since we stop at the first match from `tail`, the behavior is deterministic. Time complexity is O(n) per insertion in the worst case where n is the number of nodes (when scanning the entire circular list). Space complexity is O(1) extra space beyond the new node.
