// Write a C++ function `ListNode* removeAll(ListNode* head, int target)` that takes the head of a singly-linked list (where each node stores an integer `val` and a pointer `next`) and an integer `target`, and returns the head of the list after removing **all** nodes whose value equals `target`. The function must correctly handle empty lists, lists where the head itself needs removal, consecutive deletions, and must free the memory of deleted nodes to avoid leaks. You may define the `ListNode` struct exactly as provided, but you are not allowed to modify it.
#include <cassert>

int main() {
    // Test 1: Empty list
    ListNode* empty = nullptr;
    assert(removeAll(empty, 5) == nullptr);

    // Test 2: Remove head only
    ListNode* n1 = new ListNode(3);
    ListNode* n2 = new ListNode(1);
    n1->next = n2;
    ListNode* res2 = removeAll(n1, 3);
    assert(res2 == n2);
    assert(res2->val == 1);
    assert(res2->next == nullptr);
    delete res2;

    // Test 3: Remove all nodes
    ListNode* a1 = new ListNode(2);
    ListNode* a2 = new ListNode(2);
    a1->next = a2;
    ListNode* res3 = removeAll(a1, 2);
    assert(res3 == nullptr);

    // Test 4: Remove multiple non-consecutive values
    ListNode* b1 = new ListNode(7);
    ListNode* b2 = new ListNode(8);
    ListNode* b3 = new ListNode(7);
    b1->next = b2;
    b2->next = b3;
    ListNode* res4 = removeAll(b1, 7);
    assert(res4 == b2);
    assert(res4->val == 8);
    assert(res4->next == nullptr);
    delete res4;

    // Test 5: No removal needed
    ListNode* c1 = new ListNode(1);
    ListNode* c2 = new ListNode(2);
    c1->next = c2;
    ListNode* res5 = removeAll(c1, 3);
    assert(res5 == c1);
    assert(res5->val == 1);
    assert(res5->next == c2);
    assert(c2->val == 2);
    assert(c2->next == nullptr);
    delete c1;
    delete c2;

    return 0;
}
#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Removes all nodes with value equal to target and returns the new head.
// Deletes the removed nodes to prevent memory leaks.
ListNode* removeAll(ListNode* head, int target) {
    ListNode dummy(0, head);  // sentinel node
    ListNode* prev = &dummy;
    ListNode* curr = head;

    while (curr != nullptr) {
        if (curr->val == target) {
            prev->next = curr->next;
            delete curr;           // free the removed node
            curr = prev->next;     // move to the next candidate
        } else {
            prev = curr;
            curr = curr->next;
        }
    }

    return dummy.next;
}
// The main challenge is removing nodes that may be at the beginning of the list, because that requires updating the head pointer. Using a dummy (sentinel) node that points to the original head solves this cleanly: we maintain a `prev` pointer that always points to the last node that is *not* removed, and a `curr` pointer that scans forward. If `curr->val == target`, we unlink `curr` by setting `prev->next = curr->next`, delete `curr` to free memory, and then move `curr` to the new `prev->next`. If the value does not match, we advance both `prev` and `curr`. At the end, the new head is `dummy->next`; we delete the dummy node and return it. Edge cases: an empty list returns `nullptr`; if all nodes match, the result is `nullptr` after deleting each node; the dummy node also ensures we never dereference `nullptr` when the first node is removed. Time complexity is O(n) where n is the number of nodes, because each node is visited exactly once. Space complexity is O(1) auxiliary, excluding the input list and freed memory.
