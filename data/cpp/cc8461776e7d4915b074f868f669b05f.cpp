Write a C++ function that takes the head of a singly-linked list and returns the head of the reversed list. Each node contains an integer value and a pointer to the next node. The linked list may be empty or contain any number of nodes. Do not allocate new nodes; reverse the list by changing the `next` pointers of existing nodes only. The function should handle edge cases such as an empty list and a list with a single node, and it should return the new head pointer after reversal.

#include <cassert>

int main() {
    // Test 1: Empty list
    assert(reverseLinkedList(nullptr) == nullptr);

    // Test 2: Single node
    ListNode node1(5);
    ListNode* result1 = reverseLinkedList(&node1);
    assert(result1 == &node1);
    assert(result1->next == nullptr);

    // Test 3: Two nodes
    ListNode a(1);
    ListNode b(2);
    a.next = &b;
    ListNode* result2 = reverseLinkedList(&a);
    assert(result2 == &b);
    assert(result2->next == &a);
    assert(a.next == nullptr);

    // Test 4: Three nodes
    ListNode x(10);
    ListNode y(20);
    ListNode z(30);
    x.next = &y;
    y.next = &z;
    ListNode* result3 = reverseLinkedList(&x);
    assert(result3 == &z);
    assert(result3->next == &y);
    assert(result3->next->next == &x);
    assert(x.next == nullptr);

    // Test 5: Four nodes
    ListNode p(1);
    ListNode q(2);
    ListNode r(3);
    ListNode s(4);
    p.next = &q;
    q.next = &r;
    r.next = &s;
    ListNode* result4 = reverseLinkedList(&p);
    assert(result4 == &s);
    assert(result4->next == &r);
    assert(result4->next->next == &q);
    assert(result4->next->next->next == &p);
    assert(p.next == nullptr);

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

// Reverses a singly-linked list in-place and returns the new head.
ListNode* reverseLinkedList(ListNode* head) {
    ListNode* previous = nullptr;
    ListNode* current = head;
    ListNode* nextNode = nullptr;
    
    while (current != nullptr) {
        nextNode = current->next;  // Save next node
        current->next = previous;  // Reverse the link
        previous = current;        // Move previous forward
        current = nextNode;        // Move current forward
    }
    
    return previous;  // New head after reversal
}

// The solution uses three pointers: `prev` (initially `nullptr`), `current` (pointing to the head), and `next` (to temporarily store the next node). We iterate through the list while `current` is not `nullptr`. At each step, we save `current->next` into `next`, then set `current->next` to point to `prev`, effectively reversing the link for that node. Then we advance `prev` to `current` and `current` to `next`. After the loop finishes, `prev` points to the last node of the original list, which becomes the new head; we return it. For an empty list, the loop never runs and `prev` remains `nullptr`, which is correctly returned. For a single-node list, the loop runs once, sets the node's `next` to `nullptr` (since `prev` is initially `nullptr`), and returns that node. Time complexity is O(n) where n is the number of nodes, and space complexity is O(1) because we only use a fixed number of pointers.
