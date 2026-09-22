// Write a C++ function `ListNode* findCycleStart(ListNode* head)` that, given the head of a singly-linked list, returns a pointer to the first node where a cycle begins. If the list has no cycle, return `nullptr`. The list nodes are defined with `int val` and `ListNode* next`, and a constructor that initializes `next` to `nullptr`. You must detect and locate the cycle in a single pass over the list using only constant extra memory—do not use a hash set or modify the list’s structure. The function should handle edge cases: an empty list, a single node without a cycle, a single node that points to itself, and a long list with a cycle in the middle.

int main() {
    // No cycle: empty list
    ListNode* empty = nullptr;
    assert(findCycleStart(empty) == nullptr);

    // No cycle: single node
    ListNode* single = new ListNode(1);
    assert(findCycleStart(single) == nullptr);

    // No cycle: two nodes
    ListNode* two1 = new ListNode(1);
    ListNode* two2 = new ListNode(2);
    two1->next = two2;
    assert(findCycleStart(two1) == nullptr);

    // Cycle: single node self-loop
    ListNode* self = new ListNode(1);
    self->next = self;
    assert(findCycleStart(self) == self);

    // Cycle: 1 -> 2 -> 3 -> 4 -> 2 (cycle starts at node 2)
    ListNode* n1 = new ListNode(1);
    ListNode* n2 = new ListNode(2);
    ListNode* n3 = new ListNode(3);
    ListNode* n4 = new ListNode(4);
    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n2; // cycle back to n2
    assert(findCycleStart(n1) == n2);

    // Cycle: 1 -> 2 -> 3 -> 4 -> 5 -> 3 (cycle starts at node 3)
    ListNode* m1 = new ListNode(1);
    ListNode* m2 = new ListNode(2);
    ListNode* m3 = new ListNode(3);
    ListNode* m4 = new ListNode(4);
    ListNode* m5 = new ListNode(5);
    m1->next = m2;
    m2->next = m3;
    m3->next = m4;
    m4->next = m5;
    m5->next = m3; // cycle back to m3
    assert(findCycleStart(m1) == m3);

    // Cycle: long list where cycle starts at head itself
    ListNode* h1 = new ListNode(1);
    ListNode* h2 = new ListNode(2);
    ListNode* h3 = new ListNode(3);
    h1->next = h2;
    h2->next = h3;
    h3->next = h1; // cycle starts at head
    assert(findCycleStart(h1) == h1);

    // Cleanup (won't delete cycles; OK for test)
    return 0;
}

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Finds and returns the start node of a cycle in a linked list.
// Returns nullptr if no cycle exists.
ListNode* findCycleStart(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return nullptr;
    }

    ListNode* slow = head;
    ListNode* fast = head;

    // Phase 1: detect if a cycle exists
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            break; // cycle detected
        }
    }

    // No cycle if fast reached the end
    if (fast == nullptr || fast->next == nullptr) {
        return nullptr;
    }

    // Phase 2: find the start of the cycle
    slow = head;
    while (slow != fast) {
        slow = slow->next;
        fast = fast->next;
    }
    return slow;
}

// The solution uses Floyd’s cycle detection algorithm (tortoise and hare). Move a slow pointer one step and a fast pointer two steps at a time. If they meet, a cycle exists; if the fast pointer reaches a null before meeting, there is no cycle. Once a meeting point is found, reset one pointer to the head and move both pointers one step at a time. The node where they meet again is the start of the cycle. This works because the distance from the head to the cycle start is equal to the distance from the meeting point to the cycle start when traversed at the same speed. Edge cases: empty list or single node with no cycle returns nullptr immediately; a self-loop on a single node is detected because slow and fast both start at the head, move, then meet again at the same node, and the second phase returns the head. Time complexity is O(n) and space is O(1).
