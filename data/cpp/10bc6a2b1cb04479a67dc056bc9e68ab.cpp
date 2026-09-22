Write a C++ function `insertSortedLinkedList` that takes the head pointer of a sorted singly linked list (non-decreasing order, potentially empty) and an integer value `x`, inserts `x` into the correct position to maintain sorted order, and returns the (possibly new) head pointer. The function must create a new node dynamically, handle insertion at the head, middle, and tail correctly, and not modify any other part of the list. Assume the input list is already sorted; do not sort or rearrange it yourself. Also write a helper `printLinkedList` that prints the list elements separated by spaces, and handle memory cleanup only for the newly created node if needed—do not free the rest of the list.
#include <cassert>
#include <iostream>

// Node and function definitions are assumed to be included before this.
// For standalone testing, paste the code here.

void printList(SinglyLinkedListNode* head) {
    while (head) {
        std::cout << head->data << " ";
        head = head->next;
    }
    std::cout << std::endl;
}

void deleteList(SinglyLinkedListNode* head) {
    while (head) {
        SinglyLinkedListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: Insert into empty list
    SinglyLinkedListNode* head = nullptr;
    head = insertSortedLinkedList(head, 5);
    assert(head != nullptr && head->data == 5 && head->next == nullptr);
    deleteList(head);

    // Test 2: Insert at head (smaller than first)
    head = new SinglyLinkedListNode(10);
    head = insertSortedLinkedList(head, 3);
    assert(head->data == 3 && head->next->data == 10);
    deleteList(head);

    // Test 3: Insert in middle
    head = new SinglyLinkedListNode(1);
    head->next = new SinglyLinkedListNode(5);
    head->next->next = new SinglyLinkedListNode(9);
    head = insertSortedLinkedList(head, 7);
    assert(head->data == 1 && head->next->data == 5 && head->next->next->data == 7 && head->next->next->next->data == 9);
    deleteList(head);

    // Test 4: Insert at tail
    head = new SinglyLinkedListNode(2);
    head->next = new SinglyLinkedListNode(4);
    head = insertSortedLinkedList(head, 8);
    assert(head->data == 2 && head->next->data == 4 && head->next->next->data == 8);
    deleteList(head);

    // Test 5: Insert equal value (non-decreasing, goes before equal)
    head = new SinglyLinkedListNode(5);
    head->next = new SinglyLinkedListNode(5);
    head = insertSortedLinkedList(head, 5);
    assert(head->data == 5 && head->next->data == 5 && head->next->next->data == 5);
    deleteList(head);

    // Test 6: Multiple insertions maintain order
    head = nullptr;
    head = insertSortedLinkedList(head, 4);
    head = insertSortedLinkedList(head, 1);
    head = insertSortedLinkedList(head, 9);
    head = insertSortedLinkedList(head, 3);
    int expected[] = {1, 3, 4, 9};
    SinglyLinkedListNode* node = head;
    for (int i = 0; i < 4; i++) {
        assert(node != nullptr);
        assert(node->data == expected[i]);
        node = node->next;
    }
    assert(node == nullptr);
    deleteList(head);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <iostream>

class SinglyLinkedListNode {
public:
    int data;
    SinglyLinkedListNode* next;
    SinglyLinkedListNode(int node_data) : data(node_data), next(nullptr) {}
};

// Insert x into a sorted singly linked list (non-decreasing order).
// Returns the new head pointer. The list may be empty.
SinglyLinkedListNode* insertSortedLinkedList(SinglyLinkedListNode* head, int x) {
    SinglyLinkedListNode* current = head;
    SinglyLinkedListNode* previous = nullptr;
    SinglyLinkedListNode* newNode = new SinglyLinkedListNode(x);

    // Find insertion point: stop when current is null or current->data >= x
    while (current != nullptr && current->data < x) {
        previous = current;
        current = current->next;
    }

    // Insert at head if previous is null
    if (previous == nullptr) {
        newNode->next = head;
        return newNode;
    }

    // Insert after previous (middle or tail)
    previous->next = newNode;
    newNode->next = current;
    return head;
}
// The algorithm uses two pointers: `p` (current) and `q` (previous). Start with `p = head` and `q = nullptr`. Traverse the list while `p` is not null and `p->data < x`. During traversal, move `q` to `p` and `p` to `p->next`. After the loop, we have found the insertion point. Create a new node with value `x`. If `q` is null, this means `x` is smaller than or equal to the first element's data, so insert at the head: set `newNode->next = p` and return `newNode` as the new head. Otherwise, insert after `q`: set `newNode->next = p` and `q->next = newNode`, then return the original head. Edge cases: empty list (head is null) — the loop doesn't run, `q` remains null, so we insert as head and return the new node. Insertion at tail happens when `p` becomes null after traversing all nodes; then `q` points to the tail, we set `q->next = newNode` and `newNode->next = nullptr` (already null from constructor). Insertion at head when `x` is smaller than or equal to the first element—our loop condition uses `<`, so if `p->data == x`, we stop and insert before that node, which is valid for non-decreasing order but may place equal values before existing ones; that's acceptable because order is not strictly ascending, just non-decreasing. Time complexity is O(n) in the worst case (traverse to tail), O(1) average if inserting early. Space complexity is O(1) auxiliary (only one new node). The function must be const-correct with respect to not modifying the list structure except for the insertion itself—it only modifies the `next` pointers of surrounding nodes.
