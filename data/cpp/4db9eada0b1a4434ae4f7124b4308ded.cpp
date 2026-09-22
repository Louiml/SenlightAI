// Write a C++ function named `removeFirstNode` that takes a singly linked list represented by a head pointer and returns the head pointer after removing the first node. The function should handle the cases where the list is empty (head is `nullptr`) and where the list has only one node (after removal, the list becomes empty). The node values are integers. Do not modify any nodes other than removing the first node, and ensure proper memory deallocation of the removed node. The function signature should be: `Node* removeFirstNode(Node* head)` where `Node` is a struct with an `int val` and a `Node* next`. You may assume the `Node` struct is defined as:  
// ```cpp
// struct Node {
//     int val;
//     Node* next;
//     Node(int v) : val(v), next(nullptr) {}
// };
// ```

#include <cassert>

int main() {
    // Test 1: empty list
    Node* head1 = nullptr;
    head1 = removeFirstNode(head1);
    assert(head1 == nullptr);

    // Test 2: single node
    Node* head2 = new Node(5);
    head2 = removeFirstNode(head2);
    assert(head2 == nullptr);

    // Test 3: multiple nodes
    Node* head3 = new Node(1);
    head3->next = new Node(2);
    head3->next->next = new Node(3);
    head3 = removeFirstNode(head3);
    assert(head3 != nullptr);
    assert(head3->val == 2);
    assert(head3->next->val == 3);
    assert(head3->next->next == nullptr);
    // Clean up
    delete head3->next;
    delete head3;

    // Test 4: multiple nodes, verify all remain after removal
    Node* head4 = new Node(10);
    head4->next = new Node(20);
    head4->next->next = new Node(30);
    head4 = removeFirstNode(head4);
    assert(head4->val == 20);
    assert(head4->next->val == 30);
    assert(head4->next->next == nullptr);
    // Clean up
    delete head4->next;
    delete head4;

    return 0;
}

#include <cstddef>

struct Node {
    int val;
    Node* next;
    Node(int v) : val(v), next(nullptr) {}
};

// Remove the first node of a linked list and return the new head.
// If the list is empty, return nullptr.
Node* removeFirstNode(Node* head) {
    if (head == nullptr) {
        return nullptr;
    }
    Node* temp = head;
    head = head->next;
    delete temp;
    return head;
}

// The solution is straightforward: To delete the first node, we need the new head to point to the second node (if any). We first check if the head is `nullptr`; if so, return `nullptr` immediately because there is nothing to remove. If the list is non-empty, store the current head in a temporary pointer, update `head` to `head->next`, delete the temporary node (to free memory), and return the new head. Edge cases: an empty list returns `nullptr` unchanged; a single-node list will have `head->next` be `nullptr`, so after deletion the returned head is `nullptr`, correctly representing an empty list. Time complexity is O(1) because we only adjust a couple of pointers. Space complexity is O(1) aside from the deleted node's memory being freed.
