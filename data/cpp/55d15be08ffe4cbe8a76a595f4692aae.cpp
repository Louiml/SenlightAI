// Given a singly linked list that may contain zero or more nodes (each node stores an integer value and a pointer to the next node), write a C++ function `ListNode* kthFromEnd(ListNode* head, int k)` that returns a pointer to the `k`-th node from the end of the list (where `k` is 1-based: `k = 1` returns the last node). It is guaranteed that `k` is always valid, i.e., `1 ≤ k ≤ length of the list`. The function must not modify the list. Provide a self-contained implementation that includes the `ListNode` struct definition exactly as given in the snippet.

#include <cassert>

int main() {
    // Helper to build a list from an array.
    ListNode* buildList(int arr[], int n) {
        if (n == 0) return nullptr;
        ListNode* head = new ListNode(arr[0]);
        ListNode* tail = head;
        for (int i = 1; i < n; ++i) {
            tail->next = new ListNode(arr[i]);
            tail = tail->next;
        }
        return head;
    }

    // Test 1: single node.
    int a1[] = {5};
    ListNode* l1 = buildList(a1, 1);
    assert(kthFromEnd(l1, 1) == l1);
    assert(kthFromEnd(l1, 1)->val == 5);

    // Test 2: k equals length -> head.
    int a2[] = {1, 2, 3, 4};
    ListNode* l2 = buildList(a2, 4);
    assert(kthFromEnd(l2, 4) == l2);

    // Test 3: k = 1 -> last node.
    assert(kthFromEnd(l2, 1)->val == 4);

    // Test 4: middle node.
    assert(kthFromEnd(l2, 2)->val == 3);
    assert(kthFromEnd(l2, 3)->val == 2);

    // Test 5: list with duplicate values.
    int a3[] = {7, 7, 7};
    ListNode* l3 = buildList(a3, 3);
    assert(kthFromEnd(l3, 2) == l3->next);
    assert(kthFromEnd(l3, 2)->val == 7);

    return 0;
}

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

// Return the k-th node from the end of the list (1-based).
// The list must have at least k nodes.
ListNode* kthFromEnd(ListNode* head, int k) {
    // Compute the total length of the list.
    int length = 0;
    for (ListNode* current = head; current != nullptr; current = current->next) {
        ++length;
    }

    // Move forward length - k steps to reach the desired node.
    ListNode* result = head;
    for (int i = 0; i < length - k; ++i) {
        result = result->next;
    }
    return result;
}

// The simplest and most direct approach is to first traverse the entire list once to compute its length `len`. If `k == len`, the desired node is the head, so return it immediately. Otherwise, we need to move forward exactly `len - k` steps from the head to reach the `k`-th node from the end (because the last node is at offset `len-1` from head, so the node at distance `len - k` from head corresponds to the `k`-th from the end). This algorithm does two passes over the list but is straightforward and handles edge cases: an empty list is not expected because `k` is valid, but if `head == nullptr` the code would crash on dereferencing; however, the constraints guarantee validity. No special handling for `k == 1` is required because the loop naturally stops at the last node. The time complexity is O(len) because we traverse the list twice (once for length, once for advancing), and space complexity is O(1) constant extra space.
