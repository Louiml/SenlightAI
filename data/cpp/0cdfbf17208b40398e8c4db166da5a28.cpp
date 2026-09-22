/*
Write a C++ function that takes the head of a singly-linked list and returns a pointer to the node where a cycle begins, or `nullptr` if the list has no cycle. The list nodes are defined by `struct ListNode { int val; ListNode *next; ListNode(int x) : val(x), next(nullptr) {} };`. The function must detect any cycle (including self-loops and cycles that start at any position) and return the exact starting node; if no cycle exists, return `nullptr`. The list may be empty (head is `nullptr`), may contain only one node, may be acyclical with any length, or may have a cycle that starts at any node including the head. Your solution must run in \(O(n)\) time and use only \(O(1)\) extra space (no hash set, no marking nodes).
*/

#include <cstddef>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Returns a pointer to the first node of the cycle in the linked list,
// or nullptr if no cycle exists.
ListNode* cycleStart(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return nullptr;
    }

    ListNode* slow = head;
    ListNode* fast = head;

    // Phase 1: detect if a cycle exists using Floyd's algorithm.
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            break;
        }
    }

    // If fast reached the end, no cycle was found.
    if (fast == nullptr || fast->next == nullptr) {
        return nullptr;
    }

    // Phase 2: find the cycle entry point.
    slow = head;
    while (slow != fast) {
        slow = slow->next;
        fast = fast->next;
    }

    return slow;
}

#include <cassert>

int main() {
    // Test 1: Empty list.
    ListNode* empty = nullptr;
    assert(cycleStart(empty) == nullptr);

    // Test 2: Single node, no cycle.
    ListNode n1(1);
    assert(cycleStart(&n1) == nullptr);

    // Test 3: Single node, self-loop.
    n1.next = &n1;
    assert(cycleStart(&n1) == &n1);
    n1.next = nullptr; // Reset for later tests.

    // Test 4: Acyclic list of four nodes.
    ListNode a(1), b(2), c(3), d(4);
    a.next = &b; b.next = &c; c.next = &d;
    assert(cycleStart(&a) == nullptr);

    // Test 5: Cycle starting at node c (not head).
    d.next = &c; // cycle from d -> c
    assert(cycleStart(&a) == &c);

    // Test 6: Cycle starting at head (a).
    d.next = &a;
    assert(cycleStart(&a) == &a);

    // Test 7: Cycle starting at b.
    d.next = &b;
    assert(cycleStart(&a) == &b);

    // Test 8: Cycle in a two-node list, starting at head.
    ListNode x(1), y(2);
    x.next = &y;
    y.next = &x;
    assert(cycleStart(&x) == &x);

    // Test 9: Cycle in a two-node list, starting at tail.
    y.next = &y; // self-loop on y
    assert(cycleStart(&x) == &y);
}

// The standard approach uses Floyd’s cycle-detection algorithm (tortoise and hare). Use two pointers, `slow` and `fast`, both initially at `head`. In each step, advance `slow` by one node and `fast` by two nodes. If there is no cycle, `fast` will eventually reach `nullptr` (or `fast->next` becomes `nullptr` for even-length lists), and we can safely return `nullptr`. If a cycle exists, `slow` and `fast` will eventually meet inside the cycle. At that meeting point, reset `slow` to `head` while keeping `fast` at the meeting point. Then move both pointers one step at a time; the first node where they become equal is exactly the start of the cycle. This works mathematically because the distance from the head to the cycle start equals the distance from the meeting point back to the cycle start when both move at the same pace. Edge cases: empty list or single-node list with no cycle must return `nullptr`; a self-loop on a single node should return that node; a cycle starting at the head should return `head`. Time complexity is \(O(n)\) because the pointers traverse at most a constant number of times around the cycle, and space is \(O(1)\) (only two pointers). The constant factor is small.
