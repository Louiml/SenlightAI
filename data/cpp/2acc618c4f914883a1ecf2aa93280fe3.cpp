Write a C++ function that takes the head of a singly linked list and returns a pointer to the node where a cycle begins, or `nullptr` if there is no cycle. The linked list nodes are defined as `struct ListNode { int val; ListNode *next; ListNode(int x) : val(x), next(nullptr) {} };`. Your function should detect whether a cycle exists and, if so, locate the exact starting node of the cycle. Handle edge cases including an empty list, a single node with no cycle, a single node that points to itself, and cycles that start at the head node. Do not modify the list in any way.

#include <cassert>

int main() {
    // Test 1: empty list
    ListNode* empty = nullptr;
    assert(findCycleStart(empty) == nullptr);

    // Test 2: single node, no cycle
    ListNode* single = new ListNode(1);
    assert(findCycleStart(single) == nullptr);

    // Test 3: list with no cycle
    ListNode* list1 = new ListNode(1);
    list1->next = new ListNode(2);
    list1->next->next = new ListNode(3);
    assert(findCycleStart(list1) == nullptr);

    // Test 4: single node pointing to itself
    ListNode* selfCycle = new ListNode(1);
    selfCycle->next = selfCycle;
    assert(findCycleStart(selfCycle) == selfCycle);

    // Test 5: cycle starts at head (head->next = head)
    ListNode* headCycle = new ListNode(1);
    headCycle->next = headCycle;
    assert(findCycleStart(headCycle) == headCycle);

    // Test 6: cycle starts at a middle node
    ListNode* list2 = new ListNode(1);
    list2->next = new ListNode(2);
    list2->next->next = new ListNode(3);
    list2->next->next->next = new ListNode(4);
    list2->next->next->next->next = list2->next; // cycle at node with value 2
    assert(findCycleStart(list2) == list2->next);

    // Test 7: cycle starts at tail (tail points to itself)
    ListNode* list3 = new ListNode(1);
    list3->next = new ListNode(2);
    list3->next->next = new ListNode(3);
    list3->next->next->next = list3->next->next; // cycle at node with value 3
    assert(findCycleStart(list3) == list3->next->next);

    // Clean up (only for acyclic lists; cyclic lists are intentionally not deleted)
    delete single;
    delete list1->next->next;
    delete list1->next;
    delete list1;

    return 0;
}

#include <cstddef>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Returns the node where the cycle begins, or nullptr if no cycle exists.
ListNode* findCycleStart(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return nullptr;
    }

    ListNode* slow = head;
    ListNode* fast = head;

    // Phase 1: detect cycle using Floyd's algorithm
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            // Phase 2: find the start of the cycle
            slow = head;
            while (slow != fast) {
                slow = slow->next;
                fast = fast->next;
            }
            return slow;
        }
    }

    return nullptr; // no cycle
}

// The solution uses Floyd's cycle detection algorithm (tortoise and hare). A slow pointer moves one step at a time, and a fast pointer moves two steps at a time. If they ever meet, a cycle exists. After detecting a cycle, reset the slow pointer to the head and advance both pointers one step at a time; the point where they meet again is the cycle's start. This works because the distance from the head to the cycle start equals the distance from the meeting point to the cycle start when walked at one-step increments. Important edge cases: an empty list (`head == nullptr`), a list with no cycle (fast pointer reaches `nullptr`), and a cycle that starts at the head (the second loop immediately terminates because slow and fast both start at head). Time complexity is O(n) where n is the number of nodes (or the cycle length if cyclic), and space complexity is O(1) since only two pointers are used.
