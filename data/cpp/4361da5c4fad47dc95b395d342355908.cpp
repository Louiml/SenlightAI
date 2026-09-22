// Given a singly linked list where each node contains an integer value, write a C++ function `ListNode* findCycleStart(ListNode* head)` that returns a pointer to the first node of the cycle if the list contains a cycle, or `nullptr` if it is acyclic. The `ListNode` structure is defined as `struct ListNode { int val; ListNode *next; ListNode(int x) : val(x), next(nullptr) {} };`. You must not modify the list or use extra memory beyond a few pointer variables. The input list may be empty, may have a cycle starting at any node (including the head), or may be a simple acyclic list. Your function must correctly identify the cycle entry node in all cases.

The solution uses Floyd's cycle detection algorithm (tortoise and hare). Two pointers start at the head: `slow` moves one step and `fast` moves two steps per iteration. If the list has no cycle, `fast` will reach the end (`nullptr`) and we return `nullptr`. If a cycle exists, `slow` and `fast` will eventually meet inside the cycle. Once they meet, the mathematical property of the algorithm guarantees that the distance from the meeting point to the cycle start is equal to the distance from the head to the cycle start. Therefore, we reset one pointer to the head and keep the other at the meeting point, then advance both one step at a time. The node where they meet again is the cycle's starting node. Edge cases: empty list, single node without cycle, single node whose `next` points to itself (cycle of length 1), and cycles that begin at the head. Time complexity is O(n) where n is the number of nodes, and space complexity is O(1) since only two pointers are used.

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Return the first node of the cycle if one exists, otherwise nullptr.
ListNode* findCycleStart(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;

    // Phase 1: detect if a cycle exists
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            // Phase 2: find the cycle entry point
            ListNode* entry = head;
            while (entry != slow) {
                entry = entry->next;
                slow = slow->next;
            }
            return entry;
        }
    }
    return nullptr;
}

#include <cassert>

int main() {
    // Helper to build a list with optional cycle
    auto buildList = [](int n, int cycleStart) {
        if (n == 0) return static_cast<ListNode*>(nullptr);
        ListNode* head = new ListNode(0);
        ListNode* prev = head;
        ListNode* cycleNode = nullptr;
        for (int i = 1; i < n; ++i) {
            prev->next = new ListNode(i);
            prev = prev->next;
            if (i == cycleStart) cycleNode = prev;
        }
        if (cycleStart != -1) {
            if (cycleStart == 0) cycleNode = head;
            prev->next = cycleNode;
        }
        return head;
    };

    // Empty list
    assert(findCycleStart(nullptr) == nullptr);

    // Single node, no cycle
    ListNode* single = new ListNode(5);
    assert(findCycleStart(single) == nullptr);

    // Single node, self-cycle
    single->next = single;
    assert(findCycleStart(single) == single);

    // Linear list of 5 nodes, no cycle
    ListNode* linear = buildList(5, -1);
    assert(findCycleStart(linear) == nullptr);

    // Cycle starting at head (node 0) with 5 nodes
    ListNode* headCycle = buildList(5, 0);
    assert(findCycleStart(headCycle) == headCycle);

    // Cycle starting at node index 2 in a 5-node list
    ListNode* midCycle = buildList(5, 2);
    ListNode* expected = midCycle;
    for (int i = 0; i < 2; ++i) expected = expected->next;
    assert(findCycleStart(midCycle) == expected);

    // Cycle starting at last node (index 4) in a 5-node list
    ListNode* tailCycle = buildList(5, 4);
    expected = tailCycle;
    for (int i = 0; i < 4; ++i) expected = expected->next;
    assert(findCycleStart(tailCycle) == expected);

    // Memory cleanup (only for acyclic parts; cycles leak by design for test)
    // Not performed fully here to keep test concise.
    return 0;
}
