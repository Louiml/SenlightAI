// Given a singly linked list of integers and a non-negative integer `k`, write a C++ function `ListNode* rotateRight(ListNode* head, int k)` that rotates the list to the right by `k` positions. A right rotation by one position moves the last node to the front, and subsequent rotations are applied to the resulting list. If the list is empty or contains only one node, return the head unchanged. The value of `k` may be much larger than the length of the list, so you must handle that by taking the modulo of `k` with the list length. The function should modify the original list (not create a new one) and return the new head pointer. You may assume the list is singly linked and each node contains an integer value and a `next` pointer.

// The solution first computes the length of the list by traversing it once. If the length is 0 or 1, return `head` immediately because rotation has no effect. Otherwise, set `k = k % length` to reduce the number of effective rotations since rotating by the full length returns the list to its original form. If `k` becomes 0 after modulo, return `head` unchanged. Next, find the new tail: the node at position `length - k - 1` (0-indexed from the head); its `next` becomes the new head. Then, traverse to the original tail (the last node) and set its `next` to the original head, forming a cycle temporarily. Finally, set the new tail's `next` to `nullptr` and return the new head pointer. Edge cases include empty list, single node, `k == 0`, `k` being a multiple of the length, and `k` larger than the length. Time complexity is O(n) for two passes (length computation and tail/new-head find), and space complexity is O(1) since no extra data structures are used.

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

// Rotate the linked list to the right by k positions.
// Returns the new head of the rotated list.
ListNode* rotateRight(ListNode* head, int k) {
    if (head == NULL || head->next == NULL) {
        return head;
    }

    // Compute length of the list.
    int length = 0;
    ListNode* current = head;
    while (current != NULL) {
        current = current->next;
        ++length;
    }

    // Reduce k modulo length to avoid unnecessary rotations.
    k = k % length;
    if (k == 0) {
        return head;
    }

    // Find the new tail: node at position length - k - 1.
    int stepsToNewTail = length - k - 1;
    ListNode* newTail = head;
    for (int i = 0; i < stepsToNewTail; ++i) {
        newTail = newTail->next;
    }

    ListNode* newHead = newTail->next;

    // Find the original tail and link it to the original head.
    ListNode* originalTail = newHead;
    while (originalTail->next != NULL) {
        originalTail = originalTail->next;
    }
    originalTail->next = head;

    // Break the list at the new tail.
    newTail->next = NULL;

    return newHead;
}

#include <cassert>

// Helper to build a list from an initializer_list-like array.
ListNode* buildList(const std::initializer_list<int>& values) {
    ListNode* head = NULL;
    ListNode** tail = &head;
    for (int v : values) {
        *tail = new ListNode(v);
        tail = &((*tail)->next);
    }
    return head;
}

// Helper to compare list contents to an expected vector.
void assertListEquals(ListNode* head, const std::initializer_list<int>& expected) {
    ListNode* current = head;
    for (int e : expected) {
        assert(current != NULL);
        assert(current->val == e);
        current = current->next;
    }
    assert(current == NULL);
}

// Helper to free memory.
void freeList(ListNode* head) {
    while (head != NULL) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Single node
    ListNode* l1 = buildList({5});
    assert(rotateRight(l1, 1) == l1);
    assert(rotateRight(l1, 10) == l1);
    freeList(l1);

    // k = 0
    ListNode* l2 = buildList({1,2,3});
    ListNode* r2 = rotateRight(l2, 0);
    assertListEquals(r2, {1,2,3});
    freeList(r2);

    // k mod length = 0
    ListNode* l3 = buildList({1,2,3});
    ListNode* r3 = rotateRight(l3, 3);
    assertListEquals(r3, {1,2,3});
    freeList(r3);

    // Simple rotation by 1
    ListNode* l4 = buildList({1,2,3,4,5});
    ListNode* r4 = rotateRight(l4, 1);
    assertListEquals(r4, {5,1,2,3,4});
    freeList(r4);

    // Rotation by large k
    ListNode* l5 = buildList({1,2,3,4,5});
    ListNode* r5 = rotateRight(l5, 7); // 7 % 5 = 2
    assertListEquals(r5, {4,5,1,2,3});
    freeList(r5);

    // Rotation by length-1
    ListNode* l6 = buildList({10,20,30});
    ListNode* r6 = rotateRight(l6, 2);
    assertListEquals(r6, {20,30,10});
    freeList(r6);

    // Two nodes
    ListNode* l7 = buildList({1,2});
    ListNode* r7 = rotateRight(l7, 2); // k%2=0
    assertListEquals(r7, {1,2});
    freeList(r7);

    ListNode* l8 = buildList({1,2});
    ListNode* r8 = rotateRight(l8, 1);
    assertListEquals(r8, {2,1});
    freeList(r8);

    // Empty list
    ListNode* l9 = NULL;
    assert(rotateRight(l9, 5) == NULL);

    return 0;
}
