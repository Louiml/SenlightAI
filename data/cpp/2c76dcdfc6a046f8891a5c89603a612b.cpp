// Write a C++ function that takes a reference to the head pointer of a singly linked list and a key value, and returns a new linked list where all nodes containing the given key are removed, while preserving the relative order of the remaining nodes. The function should accept the head pointer by reference so that it can update it if the first node is removed, and return the (possibly new) head pointer. If the linked list becomes empty after removal, return `nullptr`. The function must work for empty lists, lists where the key appears multiple times, and lists where the key does not appear at all.
The main approach is to iterate through the linked list while maintaining a pointer to the previous node, so that when a node matches the key, we can bypass it by updating the previous node’s next pointer. A critical edge case is when the head node itself contains the key: we must update the head pointer to point to the next node. This is handled by processing the head separately in a loop that advances the head until it no longer matches the key or the list is empty. After that, we traverse the rest of the list with a previous pointer, and whenever `current->data == key`, we unlink the current node and delete it to avoid memory leaks; otherwise, we move both previous and current forward. Time complexity is O(n) because each node is visited exactly once. Space complexity is O(1) beyond the nodes themselves, since only a few pointer variables are used.
#include <cstddef>

// Definition for singly-linked list node.
struct Node {
    int data;
    Node* next;
    Node(int value) : data(value), next(nullptr) {}
};

// Removes all nodes with the given key from the linked list.
// Updates the head pointer if necessary and returns the new head.
Node* removeAllOccurrences(Node*& head, int key) {
    // Remove leading nodes that match the key.
    while (head != nullptr && head->data == key) {
        Node* toDelete = head;
        head = head->next;
        delete toDelete;
    }
    
    if (head == nullptr) {
        return nullptr;
    }
    
    // Remove matching nodes from the rest of the list.
    Node* previous = head;
    Node* current = head->next;
    
    while (current != nullptr) {
        if (current->data == key) {
            previous->next = current->next;
            delete current;
            current = previous->next;
        } else {
            previous = current;
            current = current->next;
        }
    }
    
    return head;
}
#include <cassert>
#include <iostream>

// Node definition is provided in the solution; for testing, include it here.
struct Node {
    int data;
    Node* next;
    Node(int value) : data(value), next(nullptr) {}
};

// Helper to build a list from a vector-like initializer list.
Node* buildList(std::initializer_list<int> values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : values) {
        Node* n = new Node(v);
        if (head == nullptr) {
            head = n;
            tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
    }
    return head;
}

// Helper to convert list to a vector for easy comparison.
std::vector<int> toVector(Node* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

// Helper to delete entire list.
void deleteList(Node* head) {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: Remove from middle
    Node* head = buildList({1, 2, 3, 2, 4});
    head = removeAllOccurrences(head, 2);
    assert(toVector(head) == std::vector<int>({1, 3, 4}));
    deleteList(head);

    // Test 2: Remove from head and tail
    head = buildList({5, 1, 5, 3, 5});
    head = removeAllOccurrences(head, 5);
    assert(toVector(head) == std::vector<int>({1, 3}));
    deleteList(head);

    // Test 3: Remove all nodes
    head = buildList({7, 7, 7});
    head = removeAllOccurrences(head, 7);
    assert(head == nullptr);
    // no deletion needed since head is null

    // Test 4: Key not present
    head = buildList({1, 2, 3});
    head = removeAllOccurrences(head, 4);
    assert(toVector(head) == std::vector<int>({1, 2, 3}));
    deleteList(head);

    // Test 5: Empty list
    head = nullptr;
    head = removeAllOccurrences(head, 1);
    assert(head == nullptr);

    // Test 6: Single node not matching
    head = buildList({42});
    head = removeAllOccurrences(head, 1);
    assert(toVector(head) == std::vector<int>({42}));
    deleteList(head);

    // Test 7: Single node matching
    head = buildList({99});
    head = removeAllOccurrences(head, 99);
    assert(head == nullptr);

    std::cout << "All tests passed.\n";
    return 0;
}
