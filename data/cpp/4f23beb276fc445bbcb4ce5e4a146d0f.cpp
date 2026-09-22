Write a C++ function named `reverseList` that takes a pointer to the head of a singly linked list (where each node is a `Node` struct containing an `int val` and a `Node* next`) and returns a new head pointer to the reversed list. The function should reverse the list in place—meaning it modifies the original nodes' `next` pointers—and return the new head. The input may be an empty list (head is `nullptr`). The function must handle lists with one or many nodes. You are provided the `Node` struct definition but must write the function yourself. The reversed list should have the original tail as the new head, and the original head as the new tail (with `next` set to `nullptr`). You must not allocate any new nodes; only relink existing ones. The solution must use a free function (not a method) with the signature `Node* reverseList(Node* head)`.

The solution uses three pointers: `prev` (initialized to `nullptr`), `curr` (initialized to `head`), and `next` (temporary). Iterate through the list, at each step save the next node (`next = curr->next`), then reverse the link by setting `curr->next = prev`, then advance `prev` to `curr` and `curr` to `next`. When `curr` becomes `nullptr`, `prev` points to the new head. Edge cases: if the input is `nullptr`, the function immediately returns `nullptr`; if there is only one node, the loop runs once and returns that same node. Time complexity is O(n) visiting each node exactly once, and space complexity is O(1) using only a constant number of pointers. No new nodes are allocated, satisfying the no-allocation requirement.

#include <cstddef>

struct Node {
    int val;
    Node* next;
};

// Reverses a singly linked list in place and returns the new head.
Node* reverseList(Node* head) {
    Node* prev = nullptr;
    Node* curr = head;
    while (curr != nullptr) {
        Node* next = curr->next;  // save the next node
        curr->next = prev;        // reverse the link
        prev = curr;              // move prev forward
        curr = next;              // move curr forward
    }
    return prev; // prev is the new head (or nullptr if list was empty)
}

#include <cassert>

// Node struct is declared above in solution, but for test we need it again.
// In a real project the struct would be in a header. For this test, we rely on
// the declaration from the solution file, so we do not redeclare here.

int main() {
    // Empty list
    Node* empty = nullptr;
    assert(reverseList(empty) == nullptr);

    // Single node
    Node* single = new Node{42, nullptr};
    Node* singleReversed = reverseList(single);
    assert(singleReversed == single);
    assert(singleReversed->val == 42);
    assert(singleReversed->next == nullptr);

    // Two nodes: 1 -> 2, reverse to 2 -> 1
    Node* n1 = new Node{1, nullptr};
    Node* n2 = new Node{2, nullptr};
    n1->next = n2;
    Node* head2 = reverseList(n1);
    assert(head2 == n2);
    assert(head2->val == 2);
    assert(head2->next == n1);
    assert(head2->next->val == 1);
    assert(head2->next->next == nullptr);

    // Three nodes: 3 -> 343 -> 353, reverse to 353 -> 343 -> 3
    Node* a = new Node{3, nullptr};
    Node* b = new Node{343, nullptr};
    Node* c = new Node{353, nullptr};
    a->next = b;
    b->next = c;
    Node* head3 = reverseList(a);
    assert(head3 == c);
    assert(head3->val == 353);
    assert(head3->next == b);
    assert(head3->next->val == 343);
    assert(head3->next->next == a);
    assert(head3->next->next->val == 3);
    assert(head3->next->next->next == nullptr);

    // Fourth test: list with duplicate values 5 -> 5 -> 5
    Node* d1 = new Node{5, nullptr};
    Node* d2 = new Node{5, nullptr};
    Node* d3 = new Node{5, nullptr};
    d1->next = d2;
    d2->next = d3;
    Node* head4 = reverseList(d1);
    assert(head4 == d3);
    assert(head4->val == 5);
    assert(head4->next == d2);
    assert(head4->next->next == d1);
    assert(head4->next->next->next == nullptr);

    // Clean up memory (not required for correctness but good practice)
    delete single;
    delete n1;
    delete n2;
    delete a;
    delete b;
    delete c;
    delete d1;
    delete d2;
    delete d3;

    return 0;
}
