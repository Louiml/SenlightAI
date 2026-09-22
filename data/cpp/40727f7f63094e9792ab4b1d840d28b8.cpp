// Write a C++ function `reverseLinkedList` that takes a singly linked list with a sentinel head node (the head node stores a value but is not part of the logical list; the logical list starts at `head->next`), and reverses the order of the logical nodes in place. The function should return a pointer to the sentinel head node, whose `next` pointer now points to the node that was previously the last logical node. After reversal, the logical order is reversed, but the sentinel head remains at the front. You may assume the list has at least one logical node (i.e., `head->next != nullptr`). The list nodes are defined as:
// ```cpp
// struct ListNode {
//     int value;
//     ListNode* next;
// };
// ```
// Do not allocate new nodes; only rearrange pointers. The function should handle lists of any length ≥ 1 correctly, including length 1 where reversing has no effect on the sequence.

// The key idea is to iterate through the logical nodes starting from `head->next` and successively move each node to the position right after the sentinel head, thereby building the reversed list in place. Initialize `current = head->next` and `nextNode = current->next`. First, set `current->next = nullptr` (since the first logical node becomes the last in reversed order). Then set `head->next = current`. Now iterate with `current = nextNode`. In each iteration, save `nextNode = current->next`, set `current->next = head->next` (linking the current node before the previously built reversed part), then set `head->next = current`, and finally advance `current = nextNode`. The loop continues until `current` is null. This reverses the list in a single pass. Edge case: if the list has exactly one logical node, the initial `current->next = nullptr` and `head->next = current` already produce the correct result, and the loop does not execute because `current` becomes null immediately. Time complexity is O(n) where n is the number of logical nodes, and space complexity is O(1) since only a few temporary pointers are used.

#include <cstddef>

struct ListNode {
    int value;
    ListNode* next;
};

// Reverses the logical nodes of a singly linked list with a sentinel head.
// The sentinel head is not part of the logical list; it remains at the front.
// Returns a pointer to the same sentinel head after reversal.
ListNode* reverseLinkedList(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return head; // Nothing to reverse (empty list or only sentinel).
    }

    ListNode* current = head->next;
    ListNode* nextNode = current->next;

    // The first logical node becomes the last node in reversed order.
    current->next = nullptr;
    head->next = current;

    current = nextNode;
    while (current != nullptr) {
        nextNode = current->next;          // Save the next node before relinking.
        current->next = head->next;        // Link current before the already reversed part.
        head->next = current;              // Move the sentinel's next to current.
        current = nextNode;                // Advance to the original next node.
    }

    return head;
}

#include <cassert>

// Helper to create a list with a sentinel head containing values from an array.
ListNode* createList(const int* values, int size) {
    ListNode* head = new ListNode{0, nullptr};
    ListNode* tail = head;
    for (int i = 0; i < size; ++i) {
        tail->next = new ListNode{values[i], nullptr};
        tail = tail->next;
    }
    return head;
}

// Helper to free the entire list (including sentinel).
void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* temp = head->next;
        delete head;
        head = temp;
    }
}

int main() {
    // Test 1: Reverse a list of 5 elements.
    int vals1[] = {1, 2, 3, 4, 5};
    ListNode* head1 = createList(vals1, 5);
    ListNode* rev1 = reverseLinkedList(head1);
    ListNode* p = rev1->next;
    int expected1[] = {5, 4, 3, 2, 1};
    for (int i = 0; i < 5; ++i) {
        assert(p != nullptr && p->value == expected1[i]);
        p = p->next;
    }
    assert(p == nullptr); // End of list reached.
    deleteList(head1);

    // Test 2: Reverse a single-element list (no change in values).
    int vals2[] = {42};
    ListNode* head2 = createList(vals2, 1);
    ListNode* rev2 = reverseLinkedList(head2);
    assert(rev2->next != nullptr && rev2->next->value == 42);
    assert(rev2->next->next == nullptr);
    deleteList(head2);

    // Test 3: Reverse a list of 10 elements (stress check).
    int vals3[10];
    for (int i = 0; i < 10; ++i) vals3[i] = i; // 0..9
    ListNode* head3 = createList(vals3, 10);
    ListNode* rev3 = reverseLinkedList(head3);
    p = rev3->next;
    for (int i = 9; i >= 0; --i) {
        assert(p != nullptr && p->value == i);
        p = p->next;
    }
    assert(p == nullptr);
    deleteList(head3);

    // Test 4: Reverse a list with duplicate values.
    int vals4[] = {7, 7, 7};
    ListNode* head4 = createList(vals4, 3);
    ListNode* rev4 = reverseLinkedList(head4);
    p = rev4->next;
    assert(p != nullptr && p->value == 7);
    p = p->next;
    assert(p != nullptr && p->value == 7);
    p = p->next;
    assert(p != nullptr && p->value == 7);
    assert(p->next == nullptr);
    deleteList(head4);

    // Test 5: Reverse a list of two elements.
    int vals5[] = {1, 2};
    ListNode* head5 = createList(vals5, 2);
    ListNode* rev5 = reverseLinkedList(head5);
    assert(rev5->next != nullptr && rev5->next->value == 2);
    assert(rev5->next->next != nullptr && rev5->next->next->value == 1);
    assert(rev5->next->next->next == nullptr);
    deleteList(head5);

    return 0;
}
