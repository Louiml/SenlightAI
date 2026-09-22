Write a C++ function `bool hasLoop(Node* head)` that detects whether a singly linked list contains a cycle (loop). The list is composed of `Node` objects, where each node has an integer `data` and a `next` pointer. The function should return `true` if there is a cycle (i.e., some node's `next` points back to a previously visited node, directly or indirectly), and `false` otherwise. The list may be empty, in which case the function should return `false`. The function must not modify the list or use extra memory proportional to the list length. Assume the `Node` class and the `getLength` and `print` helper functions are already defined as provided in the snippet. Your implementation should be robust against lists with no cycle, lists with a cycle starting at any position, and lists of length 1 (both with and without a self-loop).

The solution uses Floyd's cycle-finding algorithm (also known as the tortoise and hare algorithm). Initialize two pointers, `slow` and `fast`, both pointing to `head`. In each iteration, move `slow` one step forward and `fast` two steps forward, checking after moving that `fast` is not `NULL` before advancing. If at any point `slow == fast`, a cycle is confirmed and the function returns `true`. If the loop exits because `fast` becomes `NULL` (or `fast->next` is `NULL`), the list has no cycle and the function returns `false`. Edge cases: empty list (head is `NULL`) → return `false`; single node with `next == NULL` → returns `false` after first loop iteration (since `fast` becomes `NULL`); single node with `next` pointing to itself (self-loop) → after moving `fast` two steps, `fast` becomes `head` again, `slow` becomes `head`, and they are equal → return `true`. Time complexity is O(n) where n is the number of nodes in the list (for lists without cycles, it's O(n) to reach the end; with a cycle, the pointers meet within O(n) steps). Space complexity is O(1) as only two pointers are used.

#include <cstddef>

class Node {
public:
    int data;
    Node* next;
    Node(int value) : data(value), next(nullptr) {}
};

// Returns true if the singly linked list starting at 'head' contains a cycle.
bool hasLoop(Node* head) {
    if (head == nullptr) {
        return false;
    }
    Node* slow = head;
    Node* fast = head;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            return true;
        }
    }
    return false;
}

#include <cassert>

int main() {
    // Test 1: Empty list
    Node* head = nullptr;
    assert(hasLoop(head) == false);

    // Test 2: Single node, no loop
    Node* single = new Node(1);
    assert(hasLoop(single) == false);

    // Test 3: Single node, self-loop
    single->next = single;
    assert(hasLoop(single) == true);
    single->next = nullptr;

    // Test 4: Two nodes, no loop
    Node* a = new Node(10);
    Node* b = new Node(20);
    a->next = b;
    assert(hasLoop(a) == false);

    // Test 5: Two nodes, cycle back from b to a
    b->next = a;
    assert(hasLoop(a) == true);
    a->next = nullptr;

    // Test 6: Long list without cycle (10 nodes)
    Node* head2 = new Node(1);
    Node* temp = head2;
    for (int i = 2; i <= 10; ++i) {
        temp->next = new Node(i);
        temp = temp->next;
    }
    assert(hasLoop(head2) == false);

    // Test 7: Long list with cycle at the 5th node
    Node* cycleStart = head2;
    for (int i = 0; i < 4; ++i) cycleStart = cycleStart->next; // node with data 5
    temp = head2;
    for (int i = 0; i < 9; ++i) temp = temp->next; // node with data 10
    temp->next = cycleStart; // create cycle
    assert(hasLoop(head2) == true);
    temp->next = nullptr; // break cycle

    // Test 8: Cycle immediately from head to itself (head->next = head)
    Node* head3 = new Node(42);
    head3->next = head3;
    assert(hasLoop(head3) == true);

    return 0;
}
