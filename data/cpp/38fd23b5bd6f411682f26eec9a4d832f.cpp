Given a non-empty sequence of integers terminated by `-1` and stored as a singly linked list, write a C++ function that returns the integer value located at a given 0-based position in the linked list. If the position is out of bounds (negative or greater than or equal to the list length), or if the list is empty, the function must return `-1` as a sentinel. The function must only traverse the list once (no precomputing the length) and must not modify the list. The linked list nodes are predefined as a `Node` structure with `int data` and `Node* next`. The function signature must be `int getKthElement(const Node* head, int pos)`. You may assume the list has at least one node (i.e., head is non‑null) and that node values are integers in the range that does not include `-1` as a valid element (since `-1` is the terminator).
#include <cassert>

int main() {
    // Build a linked list: 10 -> 20 -> 30 -> 40 -> 50 (no -1 sentinel; null-terminated)
    Node n0(10), n1(20), n2(30), n3(40), n4(50);
    n0.next = &n1;
    n1.next = &n2;
    n2.next = &n3;
    n3.next = &n4;

    const Node* head = &n0;

    // Valid positions
    assert(getKthElement(head, 0) == 10);
    assert(getKthElement(head, 1) == 20);
    assert(getKthElement(head, 4) == 50);

    // Out of bounds
    assert(getKthElement(head, 5) == -1);
    assert(getKthElement(head, 100) == -1);
    assert(getKthElement(head, -1) == -1);
    assert(getKthElement(head, -100) == -1);

    // Empty list (though spec says non-empty, test safety)
    Node* emptyHead = nullptr;
    assert(getKthElement(emptyHead, 0) == -1);

    // Single-node list
    Node single(99);
    assert(getKthElement(&single, 0) == 99);
    assert(getKthElement(&single, 1) == -1);

    return 0;
}
#include <cstddef>

struct Node {
    int data;
    Node* next;
    explicit Node(int value) : data(value), next(nullptr) {}
};

// Return the data at 0-based position pos, or -1 if pos is invalid or out of range.
int getKthElement(const Node* head, int pos) {
    if (pos < 0 || head == nullptr) {
        return -1;
    }
    const Node* current = head;
    int index = 0;
    while (current != nullptr && index < pos) {
        current = current->next;
        ++index;
    }
    // If we reached exactly position pos, current is non-null; else we ran off the end.
    if (current != nullptr) {
        return current->data;
    }
    return -1;
}
// The core algorithm is a simple linear scan: start a `const Node*` pointer at the head and a counter at 0. While the pointer is not null and the counter is less than the target position, advance the pointer and increment the counter. After the loop, if the pointer is still non‑null (meaning we reached the exact position), return its `data`; otherwise, return `-1`. Because the position is checked before advancing, negative positions immediately cause the loop to skip and the pointer remains at head—if the list is non‑empty, that would incorrectly return the head’s data. Therefore, we must handle `pos < 0` explicitly at the start and return `-1`. Similarly, if the list becomes null before reaching the desired position, return `-1`. This approach traverses the list at most `min(pos+1, n)` nodes, where `n` is the list length, so the worst-case time is `O(n)` and auxiliary space is `O(1)`. Edge cases include: empty list (though the spec says non‑empty, still safe to handle), negative position, position equal to length, and position beyond length.
