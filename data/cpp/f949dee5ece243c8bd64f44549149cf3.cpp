Write a C++ function that takes a singly linked list head pointer and an integer target, and returns a new singly linked list containing all nodes from the original list whose values are strictly greater than the target, preserving their original relative order. The function should return a pointer to the head of the new list, and it must not modify the original list. If no node qualifies, return `nullptr`. Define a minimal `ListNode` struct with an `int val` and a `ListNode* next`. The function must handle an empty input list (`head == nullptr`) and correctly manage memory by allocating new nodes for the result (the original nodes must not be shared with the result).
int main() {
    // Helper to build a list from a vector-like initializer list (basic).
    // For simplicity, we construct manually.

    // Test 1: Empty list.
    ListNode* empty = nullptr;
    assert(filterGreaterThan(empty, 5) == nullptr);

    // Test 2: No qualifying nodes.
    ListNode* a1 = new ListNode(1);
    ListNode* a2 = new ListNode(2);
    a1->next = a2;
    assert(filterGreaterThan(a1, 3) == nullptr);

    // Test 3: All qualifying.
    ListNode* b1 = new ListNode(4);
    ListNode* b2 = new ListNode(6);
    b1->next = b2;
    ListNode* res3 = filterGreaterThan(b1, 2);
    assert(res3 != nullptr);
    assert(res3->val == 4);
    assert(res3->next != nullptr);
    assert(res3->next->val == 6);
    assert(res3->next->next == nullptr);

    // Test 4: Mixed, preserving order and skipping others.
    ListNode* c1 = new ListNode(3);
    ListNode* c2 = new ListNode(8);
    ListNode* c3 = new ListNode(1);
    ListNode* c4 = new ListNode(9);
    c1->next = c2; c2->next = c3; c3->next = c4;
    ListNode* res4 = filterGreaterThan(c1, 4);
    assert(res4 != nullptr);
    assert(res4->val == 8);
    assert(res4->next != nullptr);
    assert(res4->next->val == 9);
    assert(res4->next->next == nullptr);

    // Test 5: Duplicate values, only > target selected.
    ListNode* d1 = new ListNode(5);
    ListNode* d2 = new ListNode(5);
    ListNode* d3 = new ListNode(6);
    d1->next = d2; d2->next = d3;
    ListNode* res5 = filterGreaterThan(d1, 5);
    assert(res5 != nullptr);
    assert(res5->val == 6);
    assert(res5->next == nullptr);

    // Clean up manually for brevity (not required for tests).
    // In a real program, you'd delete all allocated nodes.
}
#include <cstddef>

// Minimal singly linked list node.
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Returns a new list containing nodes with val > target, preserving order.
// Does not modify the original list. Returns nullptr if no qualifying nodes.
ListNode* filterGreaterThan(ListNode* head, int target) {
    ListNode dummy(0);          // Dummy head for the result list.
    ListNode* tail = &dummy;    // Tracks last node of result.

    while (head != nullptr) {
        if (head->val > target) {
            tail->next = new ListNode(head->val);
            tail = tail->next;
        }
        head = head->next;
    }

    return dummy.next;  // May be nullptr if no nodes qualify.
}
// The solution traverses the original linked list once from head to tail. For each node, we check if its `val` is greater than the target. If so, we create a new node with the same value and append it to the tail of the result list. To append efficiently, we maintain a `tail` pointer that tracks the last node of the result list, so appending is O(1) per new node. We also use a dummy head node (`dummy`) to simplify the case when the result list is empty initially; the dummy node’s `next` becomes the actual result head. Edge cases: empty input list returns `nullptr`; if no node qualifies, the dummy’s `next` remains `nullptr`, so we return `nullptr`. Time complexity is O(n), where n is the number of nodes in the original list, because we traverse each node exactly once. Space complexity is O(m), where m is the number of nodes in the result list (i.e., the number of nodes with value > target), because we allocate one new node per qualifying original node. We do not allocate for the dummy node as part of the result; it is a local variable on the stack. The original list is never modified; we only read `val` and `next`.
