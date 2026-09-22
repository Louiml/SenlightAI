// Write a C++ function named `reverseLinkedList` that takes a pointer to the head of a singly linked list and returns a pointer to the new head of the reversed list. The list nodes are defined by the provided `ListNode` struct (with `val`, `next`, and three constructors). The function must not allocate new nodes; it must reverse the list in-place by changing the `next` pointers. The function should correctly handle an empty list (return `nullptr`) and a list with exactly one node (return the same node). After the function returns, the list should be traversable from the new head through the original tail, with all links reversed.

The solution uses an iterative three-pointer approach. We maintain three pointers: `current` (starting at `head`), `previous` (initially `nullptr`), and `next` (initially `head->next`). In each iteration, we save the next node before modifying the current node's pointer, then set `current->next` to `previous`. We then shift all three pointers forward: `previous` becomes `current`, `current` becomes the saved `next`, and `next` advances to `next->next` if it exists (guarding against null dereference). The loop continues while `current` is non-null. When the loop ends, `previous` points to the original tail, which is now the new head. Edge cases: empty list returns `nullptr` immediately; single-node list makes the loop run once, sets `current->next` to `nullptr`, and returns the same node. Time complexity is `O(n)` for `n` nodes, and space complexity is `O(1)` auxiliary (no recursion, no extra allocations). The implementation uses `const` correctness on function parameters where appropriate, but since the function modifies the list, the head pointer itself is passed by value and not modified externally.

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Reverses a singly linked list in-place and returns the new head.
// Empty list returns nullptr. Single-node list returns the same node.
ListNode* reverseLinkedList(ListNode* head) {
    if (head == nullptr) {
        return nullptr;
    }
    ListNode* current = head;
    ListNode* previous = nullptr;
    ListNode* next = head->next;
    while (current != nullptr) {
        current->next = previous;  // Reverse the link
        previous = current;        // Move previous forward
        current = next;            // Move current to saved next
        if (next != nullptr) {
            next = next->next;     // Advance next only if it exists
        }
    }
    return previous; // New head is the old tail
}

#include <cassert>

// Helper to create a list from an initializer list
ListNode* createList(std::initializer_list<int> vals) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int v : vals) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper to free all nodes
void deleteList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// Helper to compare list values
bool listEqual(ListNode* a, ListNode* b) {
    while (a && b) {
        if (a->val != b->val) return false;
        a = a->next;
        b = b->next;
    }
    return a == nullptr && b == nullptr;
}

int main() {
    // Test empty list
    assert(reverseLinkedList(nullptr) == nullptr);

    // Test single node
    ListNode* single = new ListNode(5);
    assert(reverseLinkedList(single) == single);
    assert(single->next == nullptr);
    delete single;

    // Test two nodes
    ListNode* two = createList({1, 2});
    ListNode* twoRev = reverseLinkedList(two);
    assert(twoRev != nullptr);
    assert(twoRev->val == 2 && twoRev->next->val == 1 && twoRev->next->next == nullptr);
    deleteList(twoRev);

    // Test multiple nodes
    ListNode* multi = createList({1, 2, 3, 4, 5});
    ListNode* multiRev = reverseLinkedList(multi);
    assert(listEqual(multiRev, createList({5, 4, 3, 2, 1})));
    deleteList(multiRev);
    // Note: The original 'multi' pointer is now part of reversed list, do not delete twice.

    // Test negative values and zeros
    ListNode* neg = createList({-3, 0, 7, -8});
    ListNode* negRev = reverseLinkedList(neg);
    assert(listEqual(negRev, createList({-8, 7, 0, -3})));
    deleteList(negRev);

    return 0;
}
