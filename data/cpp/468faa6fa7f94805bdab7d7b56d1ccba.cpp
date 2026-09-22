Write a C++ function `int findIntersectionValue(ListNode* headA, ListNode* headB)` that takes two singly linked lists whose nodes are of type `struct ListNode { int val; ListNode* next; ListNode(int x) : val(x), next(nullptr) {} };`. The two lists may share a common suffix (i.e., they merge at some node and then continue together). If they intersect, return the integer value stored at the first common node. If they do not intersect, return `-1`. The lists may be empty, may be identical, or one may be a sublist of the other. Your function must not modify the lists in any way, must not use extra containers like vectors or sets, and must run in linear time with constant extra space. You may assume all node values are non-negative for simplicity, except that `-1` is used solely to indicate no intersection.
The standard approach for finding the intersection of two singly linked lists is two-pointer traversal with cycle simulation. Initialize two pointers `a = headA` and `b = headB`. While `a` and `b` are not equal and neither has finished both lists, advance each pointer: if `a` reaches `nullptr`, redirect it to `headB`; if `b` reaches `nullptr`, redirect it to `headA`. If the lists intersect, both pointers will meet at the first common node after at most `lenA + lenB` steps; if they do not intersect, both will become `nullptr` after the same number of steps. To distinguish between no intersection and a valid intersection value, we must capture the meeting node pointer. The simplest correct method is: run the loop while `a != b`. When `a == b` and `a != nullptr`, return `a->val`; if `a == nullptr` (meaning both are null), return `-1`. However, the classic two-pointer trick with `nullptr` redirection alone can cause an infinite loop if the lists are disjoint? No—it terminates because after redirecting both pointers once to the opposite list, they will both eventually reach `nullptr` at the same time (they traverse `lenA + lenB` steps). Edge cases: one list empty (return -1), identical lists (first node), one list fully contained in another (meet at start of the longer list’s tail). Time complexity is O(lenA + lenB), space O(1). We must be careful to avoid dereferencing null when checking `a->next`; use while loop condition `while (a != b)` and inside check if `a == nullptr` then `a = headB` else `a = a->next`, similarly for `b`. This way, after the loop, if `a == nullptr` return -1 else return a->val.
#include <cstddef>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Returns the value at the first common node, or -1 if no intersection exists.
int findIntersectionValue(ListNode* headA, ListNode* headB) {
    if (headA == nullptr || headB == nullptr) return -1;

    ListNode* a = headA;
    ListNode* b = headB;

    // Traverse both lists, redirecting to the other list upon reaching the end.
    // If they intersect, a and b will meet at the intersection node.
    // If they don't intersect, both will become nullptr after lenA+lenB steps.
    while (a != b) {
        a = (a == nullptr) ? headB : a->next;
        b = (b == nullptr) ? headA : b->next;
    }

    // If a is nullptr, then b is also nullptr -> no intersection.
    return (a == nullptr) ? -1 : a->val;
}
#include <cassert>

int main() {
    // Helper to build lists with a given tail.
    // Test 1: Two lists that merge at node with value 8.
    ListNode* common = new ListNode(8);
    common->next = new ListNode(9);
    common->next->next = new ListNode(10);

    ListNode* headA = new ListNode(1);
    headA->next = new ListNode(2);
    headA->next->next = common;

    ListNode* headB = new ListNode(3);
    headB->next = new ListNode(4);
    headB->next->next = common;

    assert(findIntersectionValue(headA, headB) == 8);

    // Test 2: No intersection (disjoint lists).
    ListNode* c1 = new ListNode(5);
    ListNode* c2 = new ListNode(6);
    assert(findIntersectionValue(c1, c2) == -1);

    // Test 3: Identical single-node lists (same pointer).
    ListNode* single = new ListNode(7);
    assert(findIntersectionValue(single, single) == 7);

    // Test 4: One list empty.
    assert(findIntersectionValue(nullptr, headA) == -1);
    assert(findIntersectionValue(headA, nullptr) == -1);

    // Test 5: One list contains the other (longer list includes shorter entirely).
    ListNode* longHead = new ListNode(0);
    longHead->next = headA; // headA starts with 1->2->8->9->10
    assert(findIntersectionValue(longHead, headA) == 1);

    // Test 6: Both lists are exactly the same long list (intersection at first node).
    ListNode* sameList = new ListNode(100);
    sameList->next = new ListNode(101);
    assert(findIntersectionValue(sameList, sameList) == 100);

    // Test 7: Lists that intersect at the very last node.
    ListNode* tail = new ListNode(42);
    ListNode* la = new ListNode(1);
    la->next = new ListNode(2);
    la->next->next = tail;
    ListNode* lb = new ListNode(3);
    lb->next = new ListNode(4);
    lb->next->next = tail;
    assert(findIntersectionValue(la, lb) == 42);

    // Test 8: No intersection, but same values in different nodes.
    ListNode* x1 = new ListNode(1);
    ListNode* x2 = new ListNode(1);
    assert(findIntersectionValue(x1, x2) == -1);

    // Test 9: Both empty.
    assert(findIntersectionValue(nullptr, nullptr) == -1);

    return 0;
}
