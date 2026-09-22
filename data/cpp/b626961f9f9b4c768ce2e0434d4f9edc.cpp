// Write a C++ function that rotates a singly linked list to the right by `k` positions, where `k` is a non-negative integer. The function should take the head pointer of the list and the integer `k`, and return the new head pointer after the rotation. If the list is empty or `k` is a multiple of the list length, return the original head unchanged. You must implement the function as a free function (not a class method) and use a provided `ListNode` struct with `val` and `next` members. Handle the case where `k` is larger than the list length by taking the modulo. Do not allocate any new nodes; only rearrange pointers. Assume the list is a standard singly linked list with no cycles.

// The key observation is that rotating a list of length `n` to the right by `k` positions is equivalent to moving the first `n - (k % n)` nodes to the end of the list. First, handle the edge case of an empty list by returning `nullptr`. Compute the length `n` by traversing to the tail, also keeping a pointer to the tail. Let `k = k % n`; if the result is 0, no rotation is needed, so return the original head. Otherwise, find the node at position `n - k - 1` (0-indexed), which will become the new tail. Set its `next` to `nullptr`, then set the old tail's `next` to the original head, and return the node after the new tail as the new head. This requires exactly one full traversal to compute length and another partial traversal to find the split point, giving O(n) time and O(1) auxiliary space. Edge cases include empty list, `k` being 0, `k` being exactly a multiple of `n`, and rotations where the split point is the last node (i.e., `k == n - 1`).

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Rotate the linked list to the right by k positions.
// Returns the new head of the list.
ListNode* rotateRight(ListNode* head, int k) {
    if (head == nullptr) return nullptr;

    // Compute length and find tail.
    int n = 1;
    ListNode* tail = head;
    while (tail->next != nullptr) {
        tail = tail->next;
        ++n;
    }

    // Effective rotation steps.
    k = k % n;
    if (k == 0) return head;

    // Find the new tail: node at index (n - k - 1).
    int stepsToNewTail = n - k - 1;
    ListNode* newTail = head;
    for (int i = 0; i < stepsToNewTail; ++i) {
        newTail = newTail->next;
    }

    // Perform rotation.
    ListNode* newHead = newTail->next;
    newTail->next = nullptr;
    tail->next = head;

    return newHead;
}

#include <cassert>

// Helper to build a list from initializer list for testing.
ListNode* buildList(std::initializer_list<int> vals) {
    ListNode dummy(0);
    ListNode* cur = &dummy;
    for (int v : vals) {
        cur->next = new ListNode(v);
        cur = cur->next;
    }
    return dummy.next;
}

// Helper to compare list contents with a vector.
bool listEquals(ListNode* head, std::initializer_list<int> vals) {
    ListNode* cur = head;
    for (int v : vals) {
        if (cur == nullptr || cur->val != v) return false;
        cur = cur->next;
    }
    return cur == nullptr;
}

// Helper to free list memory.
void deleteList(ListNode* head) {
    while (head) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: Basic rotation.
    ListNode* l1 = buildList({1,2,3,4,5});
    l1 = rotateRight(l1, 2);
    assert(listEquals(l1, {4,5,1,2,3}));
    deleteList(l1);

    // Test 2: k equals length (no rotation).
    ListNode* l2 = buildList({1,2,3});
    l2 = rotateRight(l2, 3);
    assert(listEquals(l2, {1,2,3}));
    deleteList(l2);

    // Test 3: k larger than length.
    ListNode* l3 = buildList({1,2,3,4});
    l3 = rotateRight(l3, 6); // 6 % 4 = 2
    assert(listEquals(l3, {3,4,1,2}));
    deleteList(l3);

    // Test 4: Empty list.
    ListNode* l4 = rotateRight(nullptr, 5);
    assert(l4 == nullptr);

    // Test 5: Single node, any k.
    ListNode* l5 = buildList({42});
    l5 = rotateRight(l5, 100);
    assert(listEquals(l5, {42}));
    deleteList(l5);

    // Test 6: k = 0.
    ListNode* l6 = buildList({1,2,3});
    l6 = rotateRight(l6, 0);
    assert(listEquals(l6, {1,2,3}));
    deleteList(l6);

    // Test 7: rotate by 1 on a list.
    ListNode* l7 = buildList({1,2,3,4});
    l7 = rotateRight(l7, 1);
    assert(listEquals(l7, {4,1,2,3}));
    deleteList(l7);

    return 0;
}
