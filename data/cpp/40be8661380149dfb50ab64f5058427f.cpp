/*
Write a C++ function `ListNode* findIntersection(ListNode* headA, ListNode* headB)` that takes two singly linked lists and returns a pointer to the node where the two lists intersect, or `nullptr` if they do not intersect. The lists may have different lengths before the intersection point, and the intersection is defined by shared nodes (same address), not just equal values. The input lists are guaranteed to contain no cycles. The function must operate in linear time and constant extra space (only two pointer variables allowed for traversal). The `ListNode` structure is predefined as: `struct ListNode { int val; ListNode* next; ListNode(int x) : val(x), next(nullptr) {} };`.
*/

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Return pointer to the intersection node of two linked lists, or nullptr if none.
// Uses two pointers that switch lists at the end to equalize traversal lengths.
ListNode* findIntersection(ListNode* headA, ListNode* headB) {
    if (headA == nullptr || headB == nullptr) {
        return nullptr;
    }

    const ListNode* pA = headA;
    const ListNode* pB = headB;

    // Continue until both pointers meet or both become nullptr.
    while (pA != pB) {
        pA = (pA == nullptr) ? headB : pA->next;
        pB = (pB == nullptr) ? headA : pB->next;
    }

    // Returned as non-const pointer; the caller may modify nodes after intersection.
    return const_cast<ListNode*>(pA);
}

#include <cassert>

int main() {
    // Test 1: Intersection at node with value 8
    ListNode* common = new ListNode(8);
    common->next = new ListNode(10);

    ListNode* a1 = new ListNode(3);
    a1->next = new ListNode(7);
    a1->next->next = common;

    ListNode* b1 = new ListNode(1);
    b1->next = new ListNode(2);
    b1->next->next = common;

    assert(findIntersection(a1, b1) == common);

    // Test 2: No intersection
    ListNode* c1 = new ListNode(1);
    c1->next = new ListNode(2);
    ListNode* d1 = new ListNode(3);
    d1->next = new ListNode(4);
    assert(findIntersection(c1, d1) == nullptr);

    // Test 3: One list empty
    assert(findIntersection(nullptr, d1) == nullptr);
    assert(findIntersection(c1, nullptr) == nullptr);

    // Test 4: Identical lists (same head)
    ListNode* e = new ListNode(5);
    e->next = new ListNode(6);
    assert(findIntersection(e, e) == e);

    // Test 5: Intersection at head of both lists (same node)
    ListNode* head = new ListNode(9);
    assert(findIntersection(head, head) == head);

    // Test 6: Intersection after different lengths (list A longer)
    ListNode* common2 = new ListNode(11);
    ListNode* a2 = new ListNode(1);
    a2->next = new ListNode(2);
    a2->next->next = common2;
    ListNode* b2 = new ListNode(3);
    b2->next = common2;
    assert(findIntersection(a2, b2) == common2);

    // Test 7: Intersection after different lengths (list B longer)
    ListNode* common3 = new ListNode(13);
    ListNode* a3 = new ListNode(4);
    a3->next = common3;
    ListNode* b3 = new ListNode(5);
    b3->next = new ListNode(6);
    b3->next->next = new ListNode(7);
    b3->next->next->next = common3;
    assert(findIntersection(a3, b3) == common3);

    // Clean up (not necessary for assertion correctness, but good practice)
    // ... (allocation cleanup omitted for brevity in this test snippet)
}

// The classic approach uses two pointers that traverse both lists simultaneously. Start pointer `pA` at `headA` and `pB` at `headB`. While `pA != pB`, advance each pointer one step. When a pointer reaches `nullptr`, redirect it to the head of the other list. This works because after redirecting, both pointers have traversed the same total number of nodes (sum of the two list lengths) by the time they meet, either at the intersection node or at `nullptr` if there is no intersection. Edge cases: if either list is empty, return `nullptr` immediately. If both lists are identical (fully overlapping), the pointers meet at the first node. If there is no intersection, both pointers become `nullptr` after traversing both lists fully. Time complexity is O(m + n) where m and n are list lengths, and space complexity is O(1) beyond the two pointers.
