// Write a C++ function `removeNthFromEnd(ListNode* head, int n)` that removes the `n`-th node from the end of a singly linked list and returns the head of the modified list. The list is non-empty, and `n` is guaranteed to be valid (i.e., `1 <= n <= list length`). The function must handle edge cases such as removing the only node, removing the head, and removing the last node. Assume the `ListNode` structure is defined with `val` and `next` as shown in the snippet. Do not use extra data structures; the solution should operate in-place with O(1) additional memory.
The standard two-pass approach is used: first, traverse the entire list once to count its length `L`. Then compute the index from the start: `target = L - n`. If `target == 0`, the node to remove is the head, so we simply return `head->next`. Otherwise, traverse again from the head for `target - 1` steps to reach the node just before the target, then update its `next` pointer to skip the target node (`temp->next = temp->next->next`). This works because removing the `n`-th from the end is equivalent to removing the `(L - n + 1)`-th from the start. Edge cases include: when `n == L` (removing the head), when `n == 1` (removing the tail, where `temp->next->next` is `nullptr`), and when the list has only one node (then `n == 1` and `L == 1`, so `target == 0` and we return `head->next` which is `nullptr`). Time complexity is O(L) for the two traversals; space complexity is O(1) auxiliary. No `const` correctness is applied to the function parameter because the list is mutable.
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
    // First pass: compute length
    ListNode* temp = head;
    int length = 0;
    while (temp) {
        ++length;
        temp = temp->next;
    }

    // If removing the head node
    if (length == n) {
        ListNode* newHead = head->next;
        delete head; // optional, for clean memory management
        return newHead;
    }

    // Second pass: reach node before the target
    int steps = length - n - 1;
    temp = head;
    for (int i = 0; i < steps; ++i) {
        temp = temp->next;
    }

    // Skip the target node
    ListNode* toDelete = temp->next;
    temp->next = toDelete->next;
    delete toDelete; // optional

    return head;
}
#include <cassert>

int main() {
    // Helper to create list from initializer list (for testing)
    auto createList = [](std::initializer_list<int> vals) {
        ListNode* head = nullptr;
        ListNode* tail = nullptr;
        for (int v : vals) {
            ListNode* node = new ListNode(v);
            if (!head) head = node;
            else tail->next = node;
            tail = node;
        }
        return head;
    };

    // Helper to compare list values
    auto listEqual = [](ListNode* head, std::initializer_list<int> expected) {
        ListNode* cur = head;
        for (int v : expected) {
            if (!cur || cur->val != v) return false;
            cur = cur->next;
        }
        return cur == nullptr;
    };

    // Test 1: remove middle
    ListNode* l1 = createList({1,2,3,4,5});
    l1 = removeNthFromEnd(l1, 2);
    assert(listEqual(l1, {1,2,3,5}));

    // Test 2: remove head
    ListNode* l2 = createList({1,2,3});
    l2 = removeNthFromEnd(l2, 3);
    assert(listEqual(l2, {2,3}));

    // Test 3: remove tail
    ListNode* l3 = createList({1,2,3});
    l3 = removeNthFromEnd(l3, 1);
    assert(listEqual(l3, {1,2}));

    // Test 4: single element
    ListNode* l4 = createList({7});
    l4 = removeNthFromEnd(l4, 1);
    assert(l4 == nullptr);

    // Test 5: two elements, remove head
    ListNode* l5 = createList({1,2});
    l5 = removeNthFromEnd(l5, 2);
    assert(listEqual(l5, {2}));

    // Test 6: two elements, remove tail
    ListNode* l6 = createList({1,2});
    l6 = removeNthFromEnd(l6, 1);
    assert(listEqual(l6, {1}));

    // Clean up (optional but good practice)
    // For simplicity, we skip full deletion in tests.
    return 0;
}
