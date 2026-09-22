// Write a C++ function that detects whether a singly-linked list contains a cycle. The list nodes are defined by the standard `ListNode` structure with an `int val` and a `next` pointer. The function must return `true` if there is a cycle (i.e., some node's `next` points to an earlier node, forming a loop) and `false` otherwise. The input may be an empty list (`nullptr`), a list with a single node, or a list with or without a cycle. The function must not modify the original list’s values or structure and must be const-correct (taking a `const ListNode*`). It should work for lists containing arbitrary integer values (including negative numbers, zero, and very large values). The solution must not use additional memory beyond a constant number of pointers/state, and must handle the edge case where the list has a cycle at the very last node (pointing back to the head). Provide the function `bool hasCycle(const ListNode* head)`.
The classic efficient approach is **Floyd’s cycle-detection algorithm** (tortoise and hare). Use two pointers, `slow` and `fast`, both initially pointing to `head`. Advance `slow` by one node and `fast` by two nodes in each iteration. If there is a cycle, `fast` will eventually “lap” `slow` and they will meet at some node; if the list ends (i.e., `fast` or `fast->next` becomes `nullptr`), then there is no cycle. This works for any valid singly-linked list because in a cycle, the `fast` pointer cannot exit the loop and will catch up to `slow`; in a linear list, the `fast` pointer reaches the end. Edge cases: empty list (`head == nullptr`) immediately returns `false`; single-node list with no cycle returns `false` since `fast->next` is `nullptr`; single-node list pointing to itself (cycle) will cause `fast` to equal `slow` after one step (both point to same node). The algorithm does not modify the list, so const-correctness is maintained. Time complexity is O(n) for acyclic lists (where n is the number of nodes) and O(λ + μ) for cyclic lists (λ = tail length, μ = cycle length), which is still O(n). Space complexity is O(1) as only two pointers are used.
#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

// Detect if a singly-linked list has a cycle.
// Uses Floyd's tortoise-and-hare algorithm.
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
            return true; // Cycle detected
        }
    }

    return false; // Reached the end, no cycle
}
#include <cassert>

// Helper to create a list with a cycle for testing.
ListNode* createCyclicList(int values[], int size, int pos) {
    if (size == 0) return nullptr;
    ListNode* head = new ListNode(values[0]);
    ListNode* tail = head;
    ListNode* cycleNode = nullptr;
    for (int i = 1; i < size; ++i) {
        tail->next = new ListNode(values[i]);
        tail = tail->next;
        if (i == pos) cycleNode = tail;
    }
    if (pos >= 0) {
        tail->next = cycleNode;
    }
    return head;
}

// Helper to create a linear (acyclic) list.
ListNode* createLinearList(int values[], int size) {
    if (size == 0) return nullptr;
    ListNode* head = new ListNode(values[0]);
    ListNode* tail = head;
    for (int i = 1; i < size; ++i) {
        tail->next = new ListNode(values[i]);
        tail = tail->next;
    }
    return head;
}

int main() {
    // Test 1: empty list
    assert(hasCycle(nullptr) == false);

    // Test 2: single node, no cycle
    ListNode* single = new ListNode(1);
    assert(hasCycle(single) == false);

    // Test 3: single node, cycle to itself
    single->next = single;
    assert(hasCycle(single) == true);
    single->next = nullptr; // clean up

    // Test 4: linear list with multiple nodes
    int linearVals[] = {1, 2, 3, 4, 5};
    ListNode* linear = createLinearList(linearVals, 5);
    assert(hasCycle(linear) == false);

    // Test 5: cycle in middle
    int cycleMidVals[] = {1, 2, 3, 4, 5};
    ListNode* cycleMid = createCyclicList(cycleMidVals, 5, 2); // cycle at node index 2 (value 3)
    assert(hasCycle(cycleMid) == true);

    // Test 6: cycle back to head
    int cycleHeadVals[] = {10, 20, 30};
    ListNode* cycleHead = createCyclicList(cycleHeadVals, 3, 0); // cycle at head
    assert(hasCycle(cycleHead) == true);

    // Test 7: long list without cycle (stress test)
    int longLinearVals[1000];
    for (int i = 0; i < 1000; ++i) longLinearVals[i] = i;
    ListNode* longLinear = createLinearList(longLinearVals, 1000);
    assert(hasCycle(longLinear) == false);

    // Test 8: negative values in acyclic list
    int negVals[] = {-1, -2, -3};
    ListNode* negList = createLinearList(negVals, 3);
    assert(hasCycle(negList) == false);

    // Test 9: large values acyclic
    int largeVals[] = {100000, 200000, 300000};
    ListNode* largeList = createLinearList(largeVals, 3);
    assert(hasCycle(largeList) == false);

    // Test 10: large values with cycle
    int largeCycleVals[] = {100000, 200000, 300000, 400000};
    ListNode* largeCycle = createCyclicList(largeCycleVals, 4, 1);
    assert(hasCycle(largeCycle) == true);

    return 0;
}
