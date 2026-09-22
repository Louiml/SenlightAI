// Given a singly linked list whose nodes contain integer values, write a C++ function named `middleNodeValue` that takes a `const ListNode*` representing the head of a non-empty list and returns the integer value stored in the middle node. If the list has an even number of nodes, return the value of the second middle node (i.e., for a list of length 2, return the second node; for length 4, return the third node, etc.). The function must not modify the list, must use constant time auxiliary space, and must handle lists of length 1 correctly.

The classic two-pointer (tortoise and hare) technique solves this in a single pass. Initialize both `slow` and `fast` pointers to the head. Advance `slow` by one node and `fast` by two nodes in each iteration, continuing as long as `fast` is not null and `fast->next` is not null. When the loop terminates, `slow` points to the middle node: for odd-length lists it is the exact middle; for even-length lists it is the second middle node because `fast` reaches the end (null) only after the `slow` has moved one extra time. Edge cases: a single-node list (loop never runs, `slow` is head) and an empty list (though the task specifies non-empty, we can guard for safety by returning a sentinel or using a pre-condition; here we assume non-empty per spec). Time complexity is O(n) with n being the number of nodes, and space complexity is O(1) since no extra data structures are used.

#include <cstddef> // for nullptr

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Return the value of the middle node of a non-empty linked list.
// For even-length lists, returns the second middle node.
int middleNodeValue(const ListNode* head) {
    if (head == nullptr) {
        // Pre-condition: list is non-empty. Fallback for safety.
        return -1;
    }

    const ListNode* slow = head;
    const ListNode* fast = head;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow->val;
}

#include <cassert>

int main() {
    // Test 1: Single node
    ListNode a(5);
    assert(middleNodeValue(&a) == 5);

    // Test 2: Two nodes -> returns second middle (node 2)
    ListNode b1(1), b2(2);
    b1.next = &b2;
    assert(middleNodeValue(&b1) == 2);

    // Test 3: Three nodes -> returns middle (node 2)
    ListNode c1(10), c2(20), c3(30);
    c1.next = &c2;
    c2.next = &c3;
    assert(middleNodeValue(&c1) == 20);

    // Test 4: Four nodes -> returns second middle (node 3)
    ListNode d1(7), d2(8), d3(9), d4(10);
    d1.next = &d2;
    d2.next = &d3;
    d3.next = &d4;
    assert(middleNodeValue(&d1) == 9);

    // Test 5: Five nodes -> returns middle (node 3)
    ListNode e1(1), e2(2), e3(3), e4(4), e5(5);
    e1.next = &e2;
    e2.next = &e3;
    e3.next = &e4;
    e4.next = &e5;
    assert(middleNodeValue(&e1) == 3);

    // Test 6: Even larger list (6 nodes) -> returns node 4
    ListNode f1(0), f2(1), f3(2), f4(3), f5(4), f6(5);
    f1.next = &f2;
    f2.next = &f3;
    f3.next = &f4;
    f4.next = &f5;
    f5.next = &f6;
    assert(middleNodeValue(&f1) == 3);

    // Test 7: Negative values
    ListNode g1(-10), g2(-20), g3(-30);
    g1.next = &g2;
    g2.next = &g3;
    assert(middleNodeValue(&g1) == -20);

    // Test 8: All same values
    ListNode h1(2), h2(2), h3(2), h4(2);
    h1.next = &h2;
    h2.next = &h3;
    h3.next = &h4;
    assert(middleNodeValue(&h1) == 2);

    return 0;
}
