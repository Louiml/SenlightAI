/*
Given a singly linked list, write a C++ function `bool hasCycle(const ListNode* head)` that determines whether the list contains a cycle. The function must not modify the list, must not use extra memory proportional to the list length (i.e., no hash set), and must detect cycles even for very large lists (up to 10,000 nodes). The list nodes are defined as: `struct ListNode { int val; ListNode* next; ListNode(int x) : val(x), next(nullptr) {} };`. The function should return `true` if there is a cycle, `false` otherwise. You may use the classic "tortoise and hare" approach, but the given code snippet uses a fixed-iteration trick: because the problem spec guarantees at most 10,000 nodes in any valid test, stepping exactly 10,001 nodes forward either reaches a `nullptr` (no cycle) or proves a cycle exists (since you cannot traverse more nodes than exist in an acyclic list). Implement that strategy.
*/

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Determine if a singly linked list contains a cycle.
// The list is guaranteed to have at most 10,000 nodes.
// Returns true if there is a cycle, false otherwise.
bool hasCycle(const ListNode* head) {
    constexpr int kMaxNodes = 10001;  // one more than the maximum possible nodes

    for (int step = 0; step < kMaxNodes; ++step) {
        if (head == nullptr) {
            return false;  // Reached end of acyclic list
        }
        head = head->next;
    }

    return true;  // Survived all steps => must be a cycle
}

#include <cassert>

int main() {
    // Test 1: Empty list
    assert(hasCycle(nullptr) == false);

    // Test 2: Single node, no cycle
    ListNode a(1);
    assert(hasCycle(&a) == false);

    // Test 3: Two nodes, no cycle
    ListNode b(2);
    a.next = &b;
    assert(hasCycle(&a) == false);

    // Test 4: Cycle in middle (3 nodes, cycle back to node 2)
    ListNode c(3);
    b.next = &c;
    c.next = &b;  // create cycle
    assert(hasCycle(&a) == true);

    // Test 5: Self-cycle
    ListNode d(4);
    d.next = &d;
    assert(hasCycle(&d) == true);

    // Test 6: Long acyclic list of 10000 nodes (no cycle)
    // Build a chain of 10000 nodes
    ListNode nodes[10000];
    for (int i = 0; i < 9999; ++i) {
        nodes[i].next = &nodes[i + 1];
    }
    nodes[9999].next = nullptr;
    assert(hasCycle(&nodes[0]) == false);

    // Test 7: Long cycle of 10000 nodes (cycle at the very end)
    nodes[9999].next = &nodes[5000];  // cycle back
    assert(hasCycle(&nodes[0]) == true);

    // Test 8: Just after maximum acyclic length (10001 nodes) – not needed, but verify logic
    // Build a chain of 10001 nodes (should still be acyclic)
    ListNode nodes2[10001];
    for (int i = 0; i < 10000; ++i) {
        nodes2[i].next = &nodes2[i + 1];
    }
    nodes2[10000].next = nullptr;
    assert(hasCycle(&nodes2[0]) == false);

    // Test 9: Cycle introduced in the 10001-node list
    nodes2[10000].next = &nodes2[10000]; // self-cycle at last node
    assert(hasCycle(&nodes2[0]) == true);

    return 0;
}

// The core idea is that in a singly linked list without a cycle, you can traverse at most the total number of nodes before reaching `nullptr`. If the list has `n` nodes and no cycle, the `n`-th step leads to `nullptr`, and every further step would be invalid. Since the problem guarantees that the list size is at most 10,000, we can safely step exactly 10,001 times. If during any step we encounter `nullptr`, the list is acyclic and we return `false`. If we successfully complete all 10,001 steps without hitting `nullptr`, then the list must have a cycle (because an acyclic list would have ended earlier). Edge cases: an empty list (`head == nullptr`) returns `false` immediately. A single-node list with no cycle returns `false` after the first step. A cycle at the head or anywhere else is detected because the loop never terminates. Time complexity is O(1) effectively—constant 10,001 steps—but mathematically O(L) where L is the number of steps taken, capped at 10,001. Space complexity is O(1). The function should take `const ListNode*` and not modify anything.
