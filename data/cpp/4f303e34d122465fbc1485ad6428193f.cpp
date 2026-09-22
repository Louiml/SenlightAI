/*
Write a C++ function `int length(Node* head)` that recursively computes the number of nodes in a singly linked list. The `Node` class is defined as having an `int data` field and a `Node* next` pointer. The function must handle an empty list (head is `nullptr`), a single-node list, and lists with many nodes. The function should not modify the list and must use recursion rather than iteration. The function signature must use `Node* head` and return an integer count.
*/
#include <cstddef>

class Node {
public:
    int data;
    Node* next;
    Node(int data) : data(data), next(nullptr) {}
};

// Recursively count the number of nodes in a singly linked list.
// Returns 0 if head is nullptr.
int length(Node* head) {
    if (head == nullptr) {
        return 0;
    }
    return 1 + length(head->next);
}
#include <cassert>

int main() {
    // Test empty list
    Node* head = nullptr;
    assert(length(head) == 0);

    // Test single-node list
    head = new Node(5);
    assert(length(head) == 1);

    // Test three-node list
    head->next = new Node(10);
    head->next->next = new Node(15);
    assert(length(head) == 3);

    // Test list of 100 nodes
    Node* longHead = nullptr;
    Node* tail = nullptr;
    for (int i = 0; i < 100; ++i) {
        Node* newNode = new Node(i);
        if (longHead == nullptr) {
            longHead = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    assert(length(longHead) == 100);

    // Clean up memory (optional but recommended in real code)
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    while (longHead != nullptr) {
        Node* temp = longHead;
        longHead = longHead->next;
        delete temp;
    }

    return 0;
}
// The solution uses recursion by checking if the head pointer is null. If null, the length is 0 (base case). Otherwise, it returns 1 plus the length of the remaining list, which is obtained by recursively calling `length(head->next)`. This works because each recursive call processes exactly one node and delegates the rest. Edge cases include an empty list (returns 0) and a list with one node (returns 1). Since the list is not modified, there is no need for `const` on the pointer itself, but the pointer parameter is passed by value so no mutation occurs. The time complexity is O(n) where n is the number of nodes, because each node is visited exactly once. The space complexity is O(n) due to the recursion stack depth proportional to the list length, which could be a concern for very long lists, but is acceptable for typical academic exercises.
