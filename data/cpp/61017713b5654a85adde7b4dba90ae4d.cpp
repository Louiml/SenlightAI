// Write a C++ function that takes the head of a singly linked list (where each node stores an `int` data value and a `next` pointer) and returns a `bool` indicating whether the linked list contains a cycle (i.e., a node's `next` pointer eventually leads back to a previously visited node, creating an infinite loop when traversed). The function must handle an empty list, a list with a single node, and lists that are already cyclic. You may not use any auxiliary data structures or mark visited nodes; you must use the Floyd’s cycle-finding algorithm (tortoise and hare). The function should be `const`-correct where applicable and should not modify the list.
// The solution uses Floyd’s cycle-detection algorithm with two pointers moving at different speeds. Initialize both `slow` and `fast` to the head. In each iteration, move `slow` one step forward and `fast` two steps forward. If at any point `slow == fast`, a cycle exists because the faster pointer has caught up to the slower one inside a loop. If `fast` or `fast->next` becomes `nullptr`, the list has a boundary and is acyclic. The algorithm correctly handles edge cases: an empty list returns `false` immediately; a single-node list with `next = nullptr` returns `false`; a single-node list where `next` points to itself is detected because in the first iteration `slow` and `fast` both move to the same node (after `fast->next` is itself) and become equal. Time complexity is O(n) where n is the number of nodes (or O(n + k) with k being the cycle length, but worst-case O(n)), and space complexity is O(1) since only two pointers are used.
#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Return true if the singly linked list contains a cycle.
bool hasCycle(const ListNode* head) {
    if (head == nullptr) {
        return false;
    }

    const ListNode* slow = head;
    const ListNode* fast = head;

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
    ListNode* head1 = nullptr;
    assert(hasCycle(head1) == false);

    // Test 2: Single node, no cycle
    ListNode* node2 = new ListNode(1);
    assert(hasCycle(node2) == false);

    // Test 3: Single node, self-cycle
    node2->next = node2;
    assert(hasCycle(node2) == true);
    node2->next = nullptr;

    // Test 4: Two nodes, no cycle
    ListNode* node3 = new ListNode(2);
    node2->next = node3;
    assert(hasCycle(node2) == false);

    // Test 5: Two nodes, cycle back to head
    node3->next = node2;
    assert(hasCycle(node2) == true);
    node3->next = nullptr;

    // Test 6: Longer list, cycle in middle
    ListNode* n1 = new ListNode(1);
    ListNode* n2 = new ListNode(2);
    ListNode* n3 = new ListNode(3);
    ListNode* n4 = new ListNode(4);
    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n2; // cycle: 4 -> 2
    assert(hasCycle(n1) == true);

    // Test 7: Long acyclic list
    ListNode* m1 = new ListNode(1);
    ListNode* m2 = new ListNode(2);
    ListNode* m3 = new ListNode(3);
    m1->next = m2;
    m2->next = m3;
    assert(hasCycle(m1) == false);

    // Cleanup (not necessary for assert tests, but good practice)
    // In real code, implement a proper destructor or free list nodes carefully.
    return 0;
}
