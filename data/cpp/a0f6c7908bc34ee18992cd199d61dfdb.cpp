Write a standalone C++ function that takes a doubly linked list (represented by a `Node` struct with `int data`, `Node* prev`, and `Node* next` pointers) and a target integer value. The function should insert a new node containing the target value into the list in sorted (ascending) order, maintaining the sorted property. The list is represented by a separate `LinkedList` struct containing a head pointer (`Node* head`). The function should return nothing (void) but modify the list in place. The list may be empty, may contain duplicate values, and the target may be smaller than all existing nodes. The function must handle insertion at the beginning, middle, or end of the list correctly, preserving all connections in both directions.
// The solution requires traversing the doubly linked list from the head while comparing node data with the target. The main algorithm: first, create a new node with the target data. If the list is empty or the target is smaller than or equal to the head's data, insert at the front by setting new node's `next` to head, `prev` to nullptr (or tail if needed), and then update head. Otherwise, traverse forward until finding either the end of the list (nullptr) or a node whose data is greater than the target; insert the new node right before that node. Edge cases include: empty list (create head pointing to new node), target smaller than head (insert at front), target greater than all nodes (insert at tail, where traversal stops at nullptr), and duplicates (we can choose to maintain stable order by inserting after equal elements, but for simplicity we can insert before the first greater element, which still keeps sorted ascending order). Time complexity is O(n) in the worst case due to traversal, and space complexity is O(1) besides the new node allocation.
#include <cstddef>

// Node structure for a doubly linked list
struct Node {
    int data;
    Node* prev;
    Node* next;
    Node(int value) : data(value), prev(nullptr), next(nullptr) {}
};

// LinkedList structure containing head pointer
struct LinkedList {
    Node* head;
    LinkedList() : head(nullptr) {}
};

// Insert a new node with the given value into a sorted doubly linked list.
// The list remains sorted in non-decreasing order after insertion.
void insertSorted(Node* targetNode, LinkedList& list) {
    // Create the new node with the target value
    Node* newNode = new Node(targetNode->data); // Using targetNode's data; could also take int directly
    
    // If the list is empty, make the new node the head
    if (list.head == nullptr) {
        list.head = newNode;
        return;
    }
    
    // If the new node should be inserted at the beginning (target <= head's data)
    if (newNode->data <= list.head->data) {
        newNode->next = list.head;
        list.head->prev = newNode;
        list.head = newNode;
        return;
    }
    
    // Traverse the list to find the correct insertion point
    Node* current = list.head;
    while (current->next != nullptr && current->next->data < newNode->data) {
        current = current->next;
    }
    
    // Insert after 'current' and before 'current->next'
    newNode->next = current->next;
    if (current->next != nullptr) {
        current->next->prev = newNode;
    }
    current->next = newNode;
    newNode->prev = current;
}
*Note: The function signature uses `int` value directly for clarity—adjusting the above to accept `int value` is cleaner, but the task says "function that takes a doubly linked list ... and a target integer value" so we'll define it as `void insertSorted(LinkedList& list, int value)`.*

I realize the above solution incorrectly used `targetNode` but that was a mistake. Let me provide a corrected, clean solution.

#include <cstddef>

// Node structure for a doubly linked list
struct Node {
    int data;
    Node* prev;
    Node* next;
    Node(int value) : data(value), prev(nullptr), next(nullptr) {}
};

// LinkedList structure containing head pointer
struct LinkedList {
    Node* head;
    LinkedList() : head(nullptr) {}
};

// Insert a new node with the given value into a sorted doubly linked list.
// The list remains sorted in non-decreasing order after insertion.
void insertSorted(LinkedList& list, int value) {
    Node* newNode = new Node(value);
    
    // Empty list
    if (list.head == nullptr) {
        list.head = newNode;
        return;
    }
    
    // Insert at front if value <= head's data
    if (value <= list.head->data) {
        newNode->next = list.head;
        list.head->prev = newNode;
        list.head = newNode;
        return;
    }
    
    // Find position: locate the first node whose data > value
    Node* current = list.head;
    while (current->next != nullptr && current->next->data < value) {
        current = current->next;
    }
    
    // Insert after 'current'
    newNode->next = current->next;
    if (current->next != nullptr) {
        current->next->prev = newNode;
    }
    current->next = newNode;
    newNode->prev = current;
}
#include <cassert>

// Helper to check list order
bool isSorted(const LinkedList& list) {
    Node* cur = list.head;
    while (cur != nullptr && cur->next != nullptr) {
        if (cur->data > cur->next->data) return false;
        cur = cur->next;
    }
    return true;
}

// Helper to get length
int listLength(const LinkedList& list) {
    int len = 0;
    Node* cur = list.head;
    while (cur) { len++; cur = cur->next; }
    return len;
}

// Helper to check backward links
bool checkBackLinks(const LinkedList& list) {
    if (!list.head) return true;
    Node* cur = list.head;
    while (cur->next) {
        if (cur->next->prev != cur) return false;
        cur = cur->next;
    }
    return true;
}

int main() {
    // Test 1: Insert into empty list
    LinkedList list1;
    insertSorted(list1, 5);
    assert(list1.head && list1.head->data == 5);
    assert(list1.head->next == nullptr && list1.head->prev == nullptr);
    
    // Test 2: Insert smaller at front
    LinkedList list2;
    insertSorted(list2, 10);
    insertSorted(list2, 5);
    assert(list2.head->data == 5 && list2.head->next->data == 10);
    assert(list2.head->next->prev == list2.head);
    
    // Test 3: Insert larger at end
    LinkedList list3;
    insertSorted(list3, 1);
    insertSorted(list3, 3);
    insertSorted(list3, 2);
    assert(isSorted(list3) && listLength(list3) == 3);
    assert(list3.head->data == 1 && list3.head->next->data == 2 && list3.head->next->next->data == 3);
    
    // Test 4: Insert duplicate values (keep sorted)
    LinkedList list4;
    insertSorted(list4, 4);
    insertSorted(list4, 4);
    insertSorted(list4, 4);
    assert(isSorted(list4) && listLength(list4) == 3);
    
    // Test 5: Mixed order with negatives
    LinkedList list5;
    int values[] = {3, -5, 0, 7, -1, 10, -3};
    for (int v : values) insertSorted(list5, v);
    assert(isSorted(list5));
    assert(checkBackLinks(list5));
    assert(listLength(list5) == 7);
    
    // Test 6: Insert in middle
    LinkedList list6;
    insertSorted(list6, 1);
    insertSorted(list6, 3);
    insertSorted(list6, 2);
    insertSorted(list6, 5);
    insertSorted(list6, 4);
    // Expected: 1 2 3 4 5
    Node* cur = list6.head;
    for (int expected = 1; expected <= 5; ++expected) {
        assert(cur && cur->data == expected);
        cur = cur->next;
    }
    assert(cur == nullptr);
    
    // Test 7: Large list random-ish
    LinkedList list7;
    for (int i = 100; i >= 0; i -= 2) insertSorted(list7, i);
    for (int i = 1; i <= 99; i += 2) insertSorted(list7, i);
    assert(isSorted(list7) && listLength(list7) == 101);
    assert(list7.head->data == 0);
    
    // Test 8: Check backward links for list7
    assert(checkBackLinks(list7));
    
    // Test 9: Insert after traversal end
    LinkedList list9;
    insertSorted(list9, 10);
    insertSorted(list9, 20);
    insertSorted(list9, 15);
    insertSorted(list9, 30);
    assert(isSorted(list9));
    
    // Test 10: Single node then insert same value
    LinkedList list10;
    insertSorted(list10, 5);
    insertSorted(list10, 5);
    assert(isSorted(list10) && listLength(list10) == 2);
    assert(list10.head->data == 5 && list10.head->next->data == 5);
}
