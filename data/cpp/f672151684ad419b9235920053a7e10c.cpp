/*
Write a C++ function `ListNode* findCycleStart(ListNode* head)` that, given the head of a singly-linked list, returns a pointer to the first node where a cycle begins. If the list has no cycle, return `nullptr`. The list may be empty, may have exactly one node, or may be very long (e.g., up to 10^5 nodes). The struct `ListNode` is defined as `{ int val; ListNode *next; ListNode(int x) : val(x), next(nullptr) {} }`. You must not modify the list. Implement an efficient algorithm and provide a complexity analysis in comments.
*/

#include <cstddef> // for nullptr

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Returns the first node of the cycle if present, otherwise nullptr.
// Uses Floyd's cycle detection algorithm (tortoise and hare).
// Time: O(n) average, O(1) extra space.
ListNode* findCycleStart(ListNode* head) {
    if (!head) return nullptr;

    ListNode* slow = head;
    ListNode* fast = head;

    // Phase 1: Detect if a cycle exists.
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            // Phase 2: Find the start of the cycle.
            fast = head;
            while (fast != slow) {
                slow = slow->next;
                fast = fast->next;
            }
            return slow;
        }
    }

    return nullptr; // no cycle
}

#include <cassert>

int main() {
    // Test 1: empty list
    assert(findCycleStart(nullptr) == nullptr);

    // Test 2: single node, no cycle
    ListNode* a = new ListNode(1);
    assert(findCycleStart(a) == nullptr);

    // Test 3: single node with self-loop
    a->next = a;
    assert(findCycleStart(a) == a);

    // Test 4: two nodes, no cycle
    ListNode* b = new ListNode(2);
    a->next = b; // a now points to b, but a->next was a before; fix:
    a->next = b;
    b->next = nullptr;
    assert(findCycleStart(a) == nullptr);

    // Test 5: two nodes, cycle at second (b->next = b)
    b->next = b;
    assert(findCycleStart(a) == b);

    // Test 6: long list with cycle in middle
    // Create 1->2->3->4->5->3 (cycle starts at 3)
    ListNode* n1 = new ListNode(1);
    ListNode* n2 = new ListNode(2);
    ListNode* n3 = new ListNode(3);
    ListNode* n4 = new ListNode(4);
    ListNode* n5 = new ListNode(5);
    n1->next = n2; n2->next = n3; n3->next = n4; n4->next = n5; n5->next = n3;
    assert(findCycleStart(n1) == n3);

    // Test 7: cycle starts at head
    n5->next = n1; // now cycle starts at n1
    assert(findCycleStart(n1) == n1);

    // Clean up (for memory, but not required for assertion)
    // Avoid delete due to cycles; in practice, use a proper memory manager.

    return 0;
}

// The problem is classical cycle detection in a linked list. A naive approach using a hash set (unordered_set) of visited pointers is straightforward but uses O(n) extra space. The optimal solution uses Floyd’s Tortoise and Hare algorithm: move a `slow` pointer one step and a `fast` pointer two steps. If they ever meet, a cycle exists. Once they meet, reset `fast` to the head, then move both `slow` and `fast` one step at a time; the node where they meet again is exactly the start of the cycle. This works because the distance from the meeting point to the cycle start equals the distance from the head to the cycle start (modulo cycle length). Edge cases: empty list (return nullptr), single node with no cycle (fast will exit loop), single node with self-loop (fast and slow meet immediately after first move, then algorithm returns that node). Time complexity is O(n) time and O(1) extra space. The implementation must use `const` correctly—the input pointer is not modified, but we cannot mark it `const ListNode*` because we need to move it; we can use a local `const` qualifier on the parameter? Actually, we keep the parameter as `ListNode*` but never modify the list, so no `const` on the pointer itself is required for correctness; we can use `const ListNode*` for the head to indicate we won’t modify the list nodes, but we need to traverse, so use `const ListNode*` and cast? Better to keep the signature as `ListNode*` because the problem expects to return a mutable pointer. But for const-correctness, we can make the parameter `const ListNode*` and return `ListNode*`? That would require a const_cast. Simpler: keep it as `ListNode*` and add a comment that we do not modify the list. The task says "apply appropriate const correctness" — we can make the parameter `ListNode* const` (const pointer) to indicate we won’t reassign it? Actually, we do reassign it. So we should not use const on the pointer. Instead, we can declare local pointers as `ListNode*` and not modify the list. For brevity, we'll just not apply const to the function parameter but note that the list is read-only.
