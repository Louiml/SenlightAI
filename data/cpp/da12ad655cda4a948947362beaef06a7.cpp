// Write a C++ free function named `getLinkedListLength` that accepts a pointer to the head node of a singly linked list (where each `Node` has an `int data` and a `Node* next`) and returns the number of nodes in the list as an `int`. The function must work for an empty list (head pointer is `nullptr`) by returning 0, and must correctly count nodes for a list of arbitrary length. The function should be `const`-correct, meaning the head pointer parameter and the nodes it traverses should be treated as read-only (no modification of list structure or node data). The function must be self-contained, include the necessary `Node` structure definition, and not rely on any global variables or input/output operations.
// The solution is a straightforward linear traversal of the linked list. Start with a temporary pointer initialized to the given head, and a counter set to 0. While the temporary pointer is not `nullptr`, increment the counter by 1 and advance the temporary pointer to its `next` field. This loop terminates when reaching the end of the list (i.e., when the pointer becomes `nullptr`). Edge cases include an empty list (head is `nullptr`; the loop does not execute and returns 0) and a list with a single node (loop runs exactly once, returns 1). The algorithm does not modify any data, so the head pointer can be passed as `const Node*` to enforce read-only access. Time complexity is O(n) where n is the number of nodes, because each node is visited exactly once. Space complexity is O(1) since only a constant number of local variables are used, regardless of list size.
#include <cstddef>

// Definition of a singly linked list node.
struct Node {
    int data;
    Node* next;
    explicit Node(int value) : data(value), next(nullptr) {}
};

// Returns the number of nodes in a singly linked list given the head pointer.
// Works for an empty list (nullptr head) and does not modify the list.
int getLinkedListLength(const Node* head) {
    int count = 0;
    const Node* current = head;
    while (current != nullptr) {
        ++count;
        current = current->next;
    }
    return count;
}
#include <cassert>

// Node definition must match the solution's definition (placed here for compilation).
struct Node {
    int data;
    Node* next;
    explicit Node(int value) : data(value), next(nullptr) {}
};

// Function under test (declared here or included from the solution).
int getLinkedListLength(const Node* head);

int main() {
    // Test empty list.
    assert(getLinkedListLength(nullptr) == 0);

    // Test single-node list.
    Node a(10);
    assert(getLinkedListLength(&a) == 1);

    // Test three-node list.
    Node b(20), c(30);
    a.next = &b;
    b.next = &c;
    assert(getLinkedListLength(&a) == 3);

    // Test list of five nodes.
    Node d(40), e(50);
    c.next = &d;
    d.next = &e;
    assert(getLinkedListLength(&a) == 5);

    // Test const pointer usage (head can be const).
    const Node* constHead = &a;
    assert(getLinkedListLength(constHead) == 5);

    return 0;
}
