// Write a C++ function that takes the head of a singly linked list and an integer position `pos` (0-based), and returns the data value stored at that position. If the position is out of bounds (i.e., the list is shorter than `pos+1` nodes or the list is empty), the function should return -1 as a sentinel value. The input list is terminated by -1 when constructed from user input, but your function must not assume any specific sentinel; it should work on any valid singly linked list where each node has an `int data` and a `Node* next` pointer.
#include <cassert>

int main() {
    // Test 1: Empty list
    Node* empty = nullptr;
    assert(getNodeValue(empty, 0) == -1);
    assert(getNodeValue(empty, 5) == -1);

    // Test 2: Single-node list
    Node n1(42);
    assert(getNodeValue(&n1, 0) == 42);
    assert(getNodeValue(&n1, 1) == -1);

    // Test 3: Multi-node list: 10 -> 20 -> 30
    Node a(10), b(20), c(30);
    a.next = &b;
    b.next = &c;
    assert(getNodeValue(&a, 0) == 10);
    assert(getNodeValue(&a, 1) == 20);
    assert(getNodeValue(&a, 2) == 30);
    assert(getNodeValue(&a, 3) == -1);

    // Test 4: Negative position
    assert(getNodeValue(&a, -1) == -1);

    // Test 5: Duplicate values still work
    Node x(7), y(7), z(7);
    x.next = &y;
    y.next = &z;
    assert(getNodeValue(&x, 0) == 7);
    assert(getNodeValue(&x, 2) == 7);

    // Test 6: Position 0 on empty list
    assert(getNodeValue(empty, 0) == -1);

    return 0;
}
#include <cstddef>

// Node structure for singly linked list
struct Node {
    int data;
    Node* next;
    explicit Node(int value) : data(value), next(nullptr) {}
};

// Returns data at given 0-based position, or -1 if position is invalid
int getNodeValue(const Node* head, int pos) {
    if (pos < 0) return -1;
    int count = 0;
    const Node* current = head;
    while (current != nullptr) {
        if (count == pos) {
            return current->data;
        }
        ++count;
        current = current->next;
    }
    return -1;
}
// The main algorithm is a simple iterative traversal: initialize a counter to 0, then walk through the linked list node by node. At each node, check if the counter equals the requested position; if so, return that node's data. If the traversal reaches a null pointer before the position is found, return -1. Edge cases include: an empty list (head is nullptr) where any pos should return -1; pos=0 on a valid list returns the first element; pos beyond the list length returns -1; and negative pos should also return -1 (since counters start at 0 and will never match). The time complexity is O(n) where n is the number of nodes in the list, and space complexity is O(1) besides the input list itself. The function should use `const Node*` to enforce read-only access and accept the head by value (a pointer) for simplicity.
