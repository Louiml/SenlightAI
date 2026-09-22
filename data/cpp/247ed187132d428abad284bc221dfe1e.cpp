/*
Implement a C++ function that takes the head of a singly linked list of integers and an integer `n`, and returns the value of the nth node from the end of the list (1-indexed, so n=1 means the last node). If the list has fewer than `n` nodes, return -1. The function must operate in a single pass using only two pointers, and should not modify the list. Assume the list nodes have structure `struct Node { int data; Node* next; };`.
*/

#include <cstddef> // for nullptr

// Node definition for a singly linked list
struct Node {
    int data;
    Node* next;
    explicit Node(int value) : data(value), next(nullptr) {}
};

// Returns the value of the nth node from the end (1-indexed).
// Returns -1 if the list is empty, n is not positive, or n exceeds list length.
int getNthFromLast(const Node* head, int n) {
    if (head == nullptr || n <= 0) {
        return -1;
    }

    const Node* first = head;
    const Node* second = head;

    // Advance 'first' by n-1 steps.
    for (int i = 1; i <= n - 1; ++i) {
        first = first->next;
        if (first == nullptr) {
            return -1; // List too short
        }
    }

    // Move both pointers until 'first' reaches the last node.
    while (first->next != nullptr) {
        first = first->next;
        second = second->next;
    }

    return second->data;
}

#include <cassert>

int main() {
    // Helper to build a list from an initializer list (for testing only)
    Node* buildList(std::initializer_list<int> values) {
        Node* head = nullptr;
        Node** current = &head;
        for (int val : values) {
            *current = new Node(val);
            current = &((*current)->next);
        }
        return head;
    }

    // Helper to delete list
    void deleteList(Node* head) {
        while (head) {
            Node* next = head->next;
            delete head;
            head = next;
        }
    }

    // Test 1: normal list, n=1 should return last element
    {
        Node* head = buildList({10, 20, 30, 40});
        assert(getNthFromLast(head, 1) == 40);
        assert(getNthFromLast(head, 2) == 30);
        assert(getNthFromLast(head, 3) == 20);
        assert(getNthFromLast(head, 4) == 10);
        deleteList(head);
    }

    // Test 2: n equal to list length
    {
        Node* head = buildList({5, 15});
        assert(getNthFromLast(head, 2) == 5);
        deleteList(head);
    }

    // Test 3: n greater than length -> -1
    {
        Node* head = buildList({1, 2, 3});
        assert(getNthFromLast(head, 4) == -1);
        deleteList(head);
    }

    // Test 4: empty list -> -1
    {
        assert(getNthFromLast(nullptr, 1) == -1);
    }

    // Test 5: n <= 0 -> -1
    {
        Node* head = buildList({7, 8});
        assert(getNthFromLast(head, 0) == -1);
        assert(getNthFromLast(head, -3) == -1);
        deleteList(head);
    }

    // Test 6: single node, n=1
    {
        Node* head = buildList({42});
        assert(getNthFromLast(head, 1) == 42);
        deleteList(head);
    }

    // Test 7: negative values in list
    {
        Node* head = buildList({-1, -2, -3});
        assert(getNthFromLast(head, 1) == -3);
        assert(getNthFromLast(head, 3) == -1);
        deleteList(head);
    }

    return 0;
}

// The standard two-pointer technique is used: initialize both `first` and `second` pointers to the head. Advance `first` by `n-1` steps. If during this advancement `first` becomes null, the list is too short — return -1. Then move both pointers simultaneously (`first` and `second`) one step at a time until `first->next` is null. At that point, `second` points to the nth node from the end. Edge cases: n ≤ 0 (though typically n is positive, we can return -1 for clarity), n equal to the list length (should return head’s data), n greater than the list length (return -1), empty list (head null) — in that case immediately return -1. Time complexity is O(L) where L is the list length, because each pointer traverses at most L nodes. Space complexity is O(1) for the two pointers.
