Write a C++ function named `reverseLinkedList` that takes the head pointer of a singly-linked list (where each node stores an integer `val` and a `next` pointer) and returns the head pointer of the reversed list. The input list is non-empty, may contain any number of nodes (including negative values and duplicates), and must not allocate extra nodes; only pointer reassignment is allowed. The reversal must be performed iteratively in a single pass, preserving the original node objects but reversing their order. The function should be const-correct with respect to the input pointer (i.e., it does not modify the list before reversal), and it must handle edge cases like a list with a single node.
// The iterative reversal technique uses three pointers: `prev`, `curr`, and `nextTemp`. Initially, `prev` is `nullptr` and `curr` points to the head. In each iteration, the next node of `curr` is saved in `nextTemp`, then `curr->next` is redirected to `prev`, effectively reversing the link for that node. Then `prev` moves to `curr`, and `curr` moves to `nextTemp`. This continues until `curr` becomes `nullptr`, at which point `prev` points to the new head (the original tail). This requires no extra memory beyond a few temporary pointers, making it \(O(1)\) auxiliary space. Edge cases include an empty list (impossible per specification, but should return `nullptr` if it occurs) and a single-element list, where the loop runs once and `prev` becomes that single node, correctly returned as the new head. Time complexity is \(O(n)\) because each node is visited exactly once.
#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Reverse a singly-linked list iteratively.
// Returns the head pointer of the reversed list.
ListNode* reverseLinkedList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    while (curr != nullptr) {
        ListNode* nextTemp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextTemp;
    }
    return prev;
}
#include <cassert>

int main() {
    // Test 1: Single node
    ListNode* a = new ListNode(5);
    ListNode* result = reverseLinkedList(a);
    assert(result == a);
    assert(result->val == 5);
    assert(result->next == nullptr);

    // Test 2: Two nodes
    ListNode* b1 = new ListNode(1);
    ListNode* b2 = new ListNode(2);
    b1->next = b2;
    result = reverseLinkedList(b1);
    assert(result == b2);
    assert(result->val == 2);
    assert(result->next == b1);
    assert(result->next->next == nullptr);

    // Test 3: Three nodes with duplicates and negatives
    ListNode* c1 = new ListNode(-3);
    ListNode* c2 = new ListNode(0);
    ListNode* c3 = new ListNode(-3);
    c1->next = c2;
    c2->next = c3;
    result = reverseLinkedList(c1);
    assert(result == c3);
    assert(result->val == -3);
    assert(result->next == c2);
    assert(result->next->next == c1);
    assert(result->next->next->next == nullptr);

    // Test 4: Longer list (5 nodes)
    ListNode* d1 = new ListNode(7);
    ListNode* d2 = new ListNode(8);
    ListNode* d3 = new ListNode(9);
    ListNode* d4 = new ListNode(10);
    ListNode* d5 = new ListNode(11);
    d1->next = d2; d2->next = d3; d3->next = d4; d4->next = d5;
    result = reverseLinkedList(d1);
    assert(result == d5);
    assert(result->val == 11);
    assert(result->next == d4);
    assert(result->next->next == d3);
    assert(result->next->next->next == d2);
    assert(result->next->next->next->next == d1);
    assert(result->next->next->next->next->next == nullptr);

    // Test 5: All same values
    ListNode* e1 = new ListNode(42);
    ListNode* e2 = new ListNode(42);
    ListNode* e3 = new ListNode(42);
    e1->next = e2; e2->next = e3;
    result = reverseLinkedList(e1);
    assert(result == e3);
    assert(result->next == e2);
    assert(result->next->next == e1);
    assert(result->next->next->next == nullptr);
}
