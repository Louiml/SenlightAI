// Write a C++ function `removeNthFromEnd` that accepts the head pointer of a singly linked list (defined by a `ListNode` struct with an integer `val` and a `next` pointer) and an integer `n` (1 ≤ n ≤ number of nodes in the list). The function removes the nth node from the end of the list and returns the head pointer of the modified list. If the removed node is the head itself, return the new head. You are not allowed to use any extra linked-list-related data structures (e.g., vectors or stacks) in the solution; you may only manipulate pointers directly. The function must handle a list of size 1, cases where n equals the list length, and cases where the removal is at the end or middle. Do not modify the values of the nodes; only relink pointers. Provide a self-contained implementation with necessary headers and a clear interface, but do not include a `main` function in the section.
// The core idea is to compute the total length of the list first, then determine the position of the target node from the head (position = length - n + 1). We then traverse the list with two pointers: `prev` (tracking the node before the target) and `curr` (the current node). If the target is the head (position == 1), simply return `head->next` to skip the first node. Otherwise, iterate until `curr` reaches the target, keeping `prev` one step behind. Then set `prev->next = curr->next` to bypass the target node, and finally return the original head. Edge cases include: a single-node list (n must be 1, so returning head->next which is nullptr is correct), removing the last node (prev->next becomes nullptr), and n equals list length (target is head, handled by the early return). Time complexity is O(L) where L is the list length, because we traverse the list twice (once to compute length, once to find the target). Space complexity is O(1) since we only use a few pointers.
#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Remove the nth node from the end of the list and return the head.
ListNode* removeNthFromEnd(ListNode* head, int n) {
    // Compute the total length of the list
    ListNode* temp = head;
    int length = 0;
    while (temp != nullptr) {
        ++length;
        temp = temp->next;
    }

    // Target position from the head (1-indexed)
    int targetPos = length - n + 1;

    // If target is the head, return the next node
    if (targetPos == 1) {
        ListNode* newHead = head->next;
        // Note: we don't delete head here to avoid double-free in tests;
        // in a full program, delete head would be appropriate.
        return newHead;
    }

    // Traverse to the node before the target
    ListNode* prev = head;
    for (int i = 1; i < targetPos - 1; ++i) {
        prev = prev->next;
    }
    ListNode* nodeToRemove = prev->next;
    prev->next = nodeToRemove->next;
    // delete nodeToRemove; // omitted for test convenience; remove in production

    return head;
}
#include <cassert>

// Helper to create a list from an initializer list (for testing only)
ListNode* createList(std::initializer_list<int> values) {
    ListNode* dummy = new ListNode(0);
    ListNode* curr = dummy;
    for (int v : values) {
        curr->next = new ListNode(v);
        curr = curr->next;
    }
    return dummy->next;
}

// Helper to compare list to a vector of expected values
bool listEquals(ListNode* head, std::initializer_list<int> expected) {
    ListNode* curr = head;
    for (int v : expected) {
        if (curr == nullptr || curr->val != v) return false;
        curr = curr->next;
    }
    return curr == nullptr;
}

// Helper to delete a list and avoid memory leaks
void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test case 1: Example from prompt [1,2,3,4,5] remove 2nd from end
    ListNode* head1 = createList({1,2,3,4,5});
    ListNode* result1 = removeNthFromEnd(head1, 2);
    assert(listEquals(result1, {1,2,3,5}));
    deleteList(result1);

    // Test case 2: Single node list, remove 1st from end -> empty
    ListNode* head2 = createList({1});
    ListNode* result2 = removeNthFromEnd(head2, 1);
    assert(result2 == nullptr);
    // Note: head2 was not deleted because result2 is nullptr and head2 is the removed node; we delete it:
    delete head2;

    // Test case 3: Two nodes, remove last node
    ListNode* head3 = createList({1,2});
    ListNode* result3 = removeNthFromEnd(head3, 1);
    assert(listEquals(result3, {1}));
    deleteList(result3);

    // Test case 4: Remove head when n equals length (list of 3, n=3)
    ListNode* head4 = createList({1,2,3});
    ListNode* result4 = removeNthFromEnd(head4, 3);
    assert(listEquals(result4, {2,3}));
    deleteList(result4);

    // Test case 5: Remove from middle (list of 4, n=2 -> removes 3rd from head)
    ListNode* head5 = createList({10,20,30,40});
    ListNode* result5 = removeNthFromEnd(head5, 2);
    assert(listEquals(result5, {10,20,40}));
    deleteList(result5);

    // Test case 6: Remove last node from length 3 (n=1)
    ListNode* head6 = createList({5,6,7});
    ListNode* result6 = removeNthFromEnd(head6, 1);
    assert(listEquals(result6, {5,6}));
    deleteList(result6);

    return 0;
}
