/*
Given a head pointer to a singly linked list where each node contains an integer, write a C++ function `int lastNodeValue(const Node* head)` that returns the integer stored in the last node of the list. The linked list is defined by the provided `Node` class, where `Node` has an `int data` member and a `Node* next` pointer. If the list is empty (i.e., `head` is `nullptr`), the function should return `-1` to indicate an error. The function must only traverse the list; it must not modify the list or use any additional data structures. You may assume the list contains no cycles. The `Node` class and the `print` function are already provided in the original snippet; do not redefine them in your solution—just use them in the test code as needed.
*/

#include <cstddef> // for nullptr

// Return the data of the last node in a singly linked list, or -1 if empty.
int lastNodeValue(const Node* head) {
    if (head == nullptr) {
        return -1;
    }

    const Node* current = head;
    int lastValue = current->data;
    while (current->next != nullptr) {
        current = current->next;
        lastValue = current->data;
    }
    return lastValue;
}

#include <cassert>

// The Node class and print function are assumed to be defined as in the snippet.
// For testing, we re-declare them here minimally (or include the snippet's definitions).
// Since the task says not to redefine them, we assume they are provided. For a runnable test,
// we include them below for completeness, but in a real app they'd be in a header.

class Node {
public:
    int data;
    Node* next;
    Node(int data) : data(data), next(nullptr) {}
};

int main() {
    // Test empty list
    assert(lastNodeValue(nullptr) == -1);

    // Test single node
    Node* n1 = new Node(42);
    assert(lastNodeValue(n1) == 42);

    // Test multiple nodes
    Node* n2 = new Node(10);
    Node* n3 = new Node(20);
    Node* n4 = new Node(30);
    n2->next = n3;
    n3->next = n4;
    assert(lastNodeValue(n2) == 30);

    // Test negative values
    Node* n5 = new Node(-5);
    Node* n6 = new Node(-100);
    n5->next = n6;
    assert(lastNodeValue(n5) == -100);

    // Test larger list and verify original list unchanged
    Node* n7 = new Node(1);
    Node* n8 = new Node(2);
    Node* n9 = new Node(3);
    Node* n10 = new Node(4);
    n7->next = n8; n8->next = n9; n9->next = n10;
    assert(lastNodeValue(n7) == 4);
    // Verify list integrity: check order
    Node* temp = n7;
    int expected[] = {1,2,3,4};
    for (int i = 0; i < 4; ++i) {
        assert(temp->data == expected[i]);
        temp = temp->next;
    }
    assert(temp == nullptr);

    // Clean up memory (not strictly necessary but good practice)
    delete n1;
    delete n2; delete n3; delete n4;
    delete n5; delete n6;
    delete n7; delete n8; delete n9; delete n10;

    return 0;
}

// The solution is straightforward. Start with a pointer `current` initialized to `head`. Traverse the list by moving `current = current->next` in a loop that continues while `current` is not `nullptr`. Keep track of the previous node's value (or simply remember the last value visited) so that when the loop exits (because `current` is `nullptr`), the last value seen is the data of the last node. If the list is empty initially, the loop never runs, and we must return `-1`. A robust approach is to use a `bool hasValue` flag or initialize a result variable and check it after the loop. Edge case: a single-node list returns that node's data. Time complexity is O(n) where n is the number of nodes, because we visit each node exactly once. Space complexity is O(1) because we only use a few pointer and integer variables. We do not modify the list, so `const` correctness on the pointer (i.e., `const Node* head`) is appropriate; however, we must cast away const if we need to traverse? Actually, we can traverse with a `const Node*` pointer without issue because we only read `data` and `next`. The function should be declared with `const Node* head` to ensure we don't accidentally modify the list.
