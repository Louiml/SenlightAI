// Write a C++ function `insertSorted` that takes a reference to a singly linked list (represented by a `node*` head pointer, where `node` has an `int data` member and a `node* next` member) and an integer value, and inserts a new node containing that value into the list at the correct position so that the list remains sorted in non-decreasing order. The function must handle insertion into an empty list, insertion at the beginning, middle, and end of the list. It must not assume the list is already sorted (if it is not, the insertion should place the value in a position that maintains sorted order from that point, but you may assume the list is initially sorted for simplicity). The function should not create any memory leaks and should re-use or create new nodes appropriately. The task must be solved using only the provided structure and without using any standard library containers.

#include <cassert>

// Helper to build a list from an initializer list for testing.
node* buildList(std::initializer_list<int> vals) {
    node* head = nullptr;
    node* tail = nullptr;
    for (int v : vals) {
        node* n = new node(v);
        if (head == nullptr) { head = n; tail = n; }
        else { tail->next = n; tail = n; }
    }
    return head;
}

// Helper to convert a list to a vector for comparison.
std::vector<int> toVector(const node* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

int main() {
    // Test 1: Insert into empty list.
    node* list1 = nullptr;
    insertSorted(list1, 5);
    assert(toVector(list1) == std::vector<int>{5});

    // Test 2: Insert at head.
    node* list2 = buildList({3, 6, 9});
    insertSorted(list2, 1);
    assert(toVector(list2) == std::vector<int>{1, 3, 6, 9});

    // Test 3: Insert in middle.
    node* list3 = buildList({1, 2, 4, 5});
    insertSorted(list3, 3);
    assert(toVector(list3) == std::vector<int>{1, 2, 3, 4, 5});

    // Test 4: Insert at tail.
    node* list4 = buildList({1, 2, 3});
    insertSorted(list4, 7);
    assert(toVector(list4) == std::vector<int>{1, 2, 3, 7});

    // Test 5: Insert duplicate value (non-decreasing allowed).
    node* list5 = buildList({2, 2, 4});
    insertSorted(list5, 2);
    assert(toVector(list5) == std::vector<int>{2, 2, 2, 4});

    // Test 6: Insert into single-element list at head.
    node* list6 = buildList({10});
    insertSorted(list6, 0);
    assert(toVector(list6) == std::vector<int>{0, 10});

    // Test 7: Insert into single-element list at tail.
    node* list7 = buildList({10});
    insertSorted(list7, 20);
    assert(toVector(list7) == std::vector<int>{10, 20});

    return 0;
}

#include <iostream>

class node {
public:
    int data;
    node* next;
    node(int val) : data(val), next(nullptr) {}
};

// Insert a new node with value val into a sorted singly linked list.
// The list remains sorted in non-decreasing order after insertion.
// The head pointer is passed by reference so the head can be updated if needed.
void insertSorted(node*& head, int val) {
    node* newNode = new node(val);

    // Empty list: newNode becomes the head.
    if (head == nullptr) {
        head = newNode;
        return;
    }

    // Insert at the beginning if the new value is smaller than the head's data.
    if (val < head->data) {
        newNode->next = head;
        head = newNode;
        return;
    }

    // Traverse to find the insertion point.
    node* current = head;
    while (current->next != nullptr && current->next->data <= val) {
        current = current->next;
    }

    // Insert newNode after current (either before a larger node or at the tail).
    newNode->next = current->next;
    current->next = newNode;
}

// The solution traverses the linked list starting from the head while keeping track of the previous node. The algorithm compares the new value with each node's data. If the new value is less than or equal to the current node's data (or if we reach the end of the list), we insert the new node before the current node (or at the tail). For an empty list (head is `NULL`), we simply set head to the new node. To insert at the head, we create a new node, set its `next` to the current head, and update the head pointer. For middle and tail insertions, we traverse until we find a node whose data is greater than the new value (or `NULL`), then link the previous node to the new node and the new node to the current node. Edge cases include inserting into an empty list (no traversal needed), inserting at the head (when the new value is smaller than the first element), and inserting at the tail (when the new value is greater than all existing elements). Time complexity is O(n) where n is the number of nodes in the list because in the worst case we traverse the entire list. Space complexity is O(1) auxiliary space because we only allocate one new node.
