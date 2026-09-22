// Write a C++ function `ListNode* reverseSublist(ListNode* head, int left, int right)` that reverses the nodes of a singly linked list from position `left` to position `right` (1-indexed), leaving all other nodes in their original order. The function must handle edge cases such as an empty list, a list with a single node, `left == right` (no reversal needed), and reversal at the very beginning of the list (so the head changes). Assume `left` and `right` are valid (1 ≤ left ≤ right ≤ length of the list). The function should return the new head of the list after the reversal. Do not allocate new nodes; modify the existing list in place.
The solution requires a single traversal of the list. First, move a `prev` pointer to the node immediately before the sublist to be reversed, and a `cur` pointer to the first node of the sublist. If `left == 1`, then `prev` remains `nullptr` (since there is no node before the head). We then reverse the sublist by iterating `right - left + 1` nodes: for each step, we temporarily store the next pointer, reverse the current node's next pointer to point to the previous node, and advance both `prev` and `cur` pointers. After the loop, `prev` points to the last node of the reversed sublist (originally the node at position `right`), and `cur` points to the node immediately after the sublist. Finally, we connect the node before the sublist (if any) to the new head of the reversed segment, and the tail of the reversed segment to `cur`. If there was no node before the sublist, we update `head` to `prev`. Edge cases include an empty list or a single-node list (return immediately), and `left == right` (the reversal loop does nothing, and we simply return the original head). Time complexity is O(n) where n is the length of the list (we traverse at most up to `right`), and space complexity is O(1) aside from the input list itself.
#include <cstddef>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Reverse the sublist from positions left to right (1-indexed) in a singly linked list.
ListNode* reverseSublist(ListNode* head, int left, int right) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    ListNode* prev = nullptr;
    ListNode* cur = head;
    
    // Advance cur to the left-th node, and prev to the node before it.
    int steps = left;
    while (--steps > 0) {
        prev = cur;
        cur = cur->next;
    }

    ListNode* beforeSublist = prev;  // nullptr if left == 1
    ListNode* sublistTail = cur;     // will become the tail after reversal
    ListNode* next = nullptr;

    // Reverse exactly (right - left + 1) nodes.
    int count = right - left + 1;
    while (count-- > 0) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }

    // Connect the reversed segment back to the rest of the list.
    if (beforeSublist != nullptr) {
        beforeSublist->next = prev;
    } else {
        head = prev;
    }
    sublistTail->next = cur;

    return head;
}
#include <cassert>

// Helper to create a list from an initializer list (for testing)
ListNode* createList(std::initializer_list<int> vals) {
    ListNode dummy;
    ListNode* tail = &dummy;
    for (int v : vals) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper to compare a list to a vector of values
bool listEquals(ListNode* head, std::initializer_list<int> vals) {
    ListNode* cur = head;
    for (int v : vals) {
        if (cur == nullptr || cur->val != v) return false;
        cur = cur->next;
    }
    return cur == nullptr;
}

int main() {
    // Test 1: Reverse middle portion
    ListNode* list1 = createList({1,2,3,4,5});
    list1 = reverseSublist(list1, 2, 4);
    assert(listEquals(list1, {1,4,3,2,5}));

    // Test 2: Reverse entire list (left=1, right=5)
    ListNode* list2 = createList({1,2,3,4,5});
    list2 = reverseSublist(list2, 1, 5);
    assert(listEquals(list2, {5,4,3,2,1}));

    // Test 3: Reverse only head (left=1, right=1) → no change
    ListNode* list3 = createList({1,2,3});
    list3 = reverseSublist(list3, 1, 1);
    assert(listEquals(list3, {1,2,3}));

    // Test 4: Reverse tail portion (left=3, right=5)
    ListNode* list4 = createList({1,2,3,4,5});
    list4 = reverseSublist(list4, 3, 5);
    assert(listEquals(list4, {1,2,5,4,3}));

    // Test 5: Empty list
    ListNode* list5 = nullptr;
    list5 = reverseSublist(list5, 1, 1);
    assert(list5 == nullptr);

    // Test 6: Single node list
    ListNode* list6 = new ListNode(42);
    list6 = reverseSublist(list6, 1, 1);
    assert(list6 != nullptr && list6->val == 42 && list6->next == nullptr);

    // Test 7: Reversal in middle with left=right (no change)
    ListNode* list7 = createList({1,2,3,4});
    list7 = reverseSublist(list7, 2, 2);
    assert(listEquals(list7, {1,2,3,4}));

    // Test 8: Two-node list, reverse both
    ListNode* list8 = createList({1,2});
    list8 = reverseSublist(list8, 1, 2);
    assert(listEquals(list8, {2,1}));

    // Test 9: Reversal starting at head but not entire list (left=1, right=2 of 5)
    ListNode* list9 = createList({1,2,3,4,5});
    list9 = reverseSublist(list9, 1, 2);
    assert(listEquals(list9, {2,1,3,4,5}));

    // Test 10: Reversal where left=2, right=4 of 4 (tail includes last)
    ListNode* list10 = createList({1,2,3,4});
    list10 = reverseSublist(list10, 2, 4);
    assert(listEquals(list10, {1,4,3,2}));

    // Cleanup (not strictly necessary for assert tests, but good practice)
    return 0;
}
