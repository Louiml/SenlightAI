/*
Write a C++ function that takes the head of a singly linked list and an integer `n` (1 ≤ n ≤ list length), and removes the nth node from the end of the list, returning the head of the modified list. The list nodes are defined as `struct ListNode { int val; ListNode *next; }` with constructors. The function must handle lists of length 1, remove the head when applicable, and preserve the relative order of remaining nodes. You may assume the input list is non-empty and `n` is always valid.
*/
#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Removes the n-th node from the end of the list and returns the new head.
ListNode* removeNthFromEnd(ListNode* head, int n) {
    if (head->next == nullptr) {
        // Only one node, so removing it leaves an empty list.
        return nullptr;
    }

    ListNode* fast = head;
    ListNode* slow = head;

    // Advance fast by n steps.
    for (int i = 0; i < n; ++i) {
        fast = fast->next;
    }

    // If fast is null, the head is the node to remove.
    if (fast == nullptr) {
        ListNode* newHead = head->next;
        return newHead;
    }

    // Move both pointers until fast reaches the last node.
    while (fast->next != nullptr) {
        fast = fast->next;
        slow = slow->next;
    }

    // slow now points to the node before the target.
    slow->next = slow->next->next;
    return head;
}
#include <cassert>

// Helper to build list from vector-like initializer list.
ListNode* buildList(std::initializer_list<int> vals) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int v : vals) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper to convert list to vector-like for comparison.
bool listEqual(ListNode* head, std::initializer_list<int> vals) {
    ListNode* cur = head;
    for (int v : vals) {
        if (!cur || cur->val != v) return false;
        cur = cur->next;
    }
    return cur == nullptr;
}

// Helper to free list memory.
void freeList(ListNode* head) {
    while (head) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Case 1: Single node, remove it.
    ListNode* l1 = buildList({5});
    l1 = removeNthFromEnd(l1, 1);
    assert(l1 == nullptr);

    // Case 2: Remove head from [1,2,3,4,5] with n=5.
    ListNode* l2 = buildList({1,2,3,4,5});
    l2 = removeNthFromEnd(l2, 5);
    assert(listEqual(l2, {2,3,4,5}));
    freeList(l2);

    // Case 3: Remove last node [1,2,3] with n=1.
    ListNode* l3 = buildList({1,2,3});
    l3 = removeNthFromEnd(l3, 1);
    assert(listEqual(l3, {1,2}));
    freeList(l3);

    // Case 4: Remove middle node [1,2,3,4] with n=2.
    ListNode* l4 = buildList({1,2,3,4});
    l4 = removeNthFromEnd(l4, 2);
    assert(listEqual(l4, {1,2,4}));
    freeList(l4);

    // Case 5: Two-node list, remove head [10,20] with n=2.
    ListNode* l5 = buildList({10,20});
    l5 = removeNthFromEnd(l5, 2);
    assert(listEqual(l5, {20}));
    freeList(l5);

    // Case 6: Two-node list, remove tail [10,20] with n=1.
    ListNode* l6 = buildList({10,20});
    l6 = removeNthFromEnd(l6, 1);
    assert(listEqual(l6, {10}));
    freeList(l6);

    // Case 7: Duplicate values [7,7,7] remove middle n=2.
    ListNode* l7 = buildList({7,7,7});
    l7 = removeNthFromEnd(l7, 2);
    assert(listEqual(l7, {7,7}));
    freeList(l7);
}
// The optimal solution uses a two-pointer (fast/slow) technique with a single pass. Initialize two pointers, `fast` and `slow`, both pointing to `head`. First, advance `fast` by `n` steps. If `fast` becomes `nullptr` after this, it means the node to remove is the head, so simply return `head->next`. Otherwise, advance both pointers simultaneously until `fast` reaches the last node (i.e., `fast->next == nullptr`). At that point, `slow` points to the node just before the one to remove. Then set `slow->next = slow->next->next` to unlink the target node. Edge cases: a single-node list (n=1) returns `nullptr` directly; removing the head (when fast becomes null after the initial advance) is handled separately. Time complexity is O(L) where L is list length, and space complexity is O(1) auxiliary.
