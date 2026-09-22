Write a C++ function `ListNode* reorderOddEven(ListNode* head)` that takes the head of a singly linked list and rearranges its nodes so that all nodes at odd positions (1st, 3rd, 5th, …) come first in their original relative order, followed by all nodes at even positions (2nd, 4th, 6th, …) in their original relative order. The function must modify the list in-place (no copying of node values) and return the new head. For example, given the list `1 -> 2 -> 3 -> 4 -> 5 -> 6`, the result should be `1 -> 3 -> 5 -> 2 -> 4 -> 6`. The function should handle edge cases gracefully: an empty list, a single node, or two nodes should be returned unchanged. Assume `ListNode` is defined as in the snippet: with an integer `val` and a `next` pointer, plus constructors. The function must not leak memory and must not use any extra container (like `std::list` or `std::vector`) for rearrangement—only pointer manipulation is allowed.
The core idea is to split the original list into two separate chains: one chain for odd-indexed nodes and one for even-indexed nodes, then link the end of the odd chain to the head of the even chain. We maintain three pointers: `oddTail` (the last node in the odd chain, initially `head`), `evenHead` (the head of the even chain, which is `head->next` if it exists), and `evenTail` (the last node in the even chain, initially also `head->next`). Then we traverse the list starting from the third node (`head->next->next`) and, for each node, determine whether its 1-based position is odd or even. If odd, we append it to the odd chain by updating `oddTail->next` and advancing `oddTail`; if even, we append to the even chain similarly. After the traversal, we must set `evenTail->next = nullptr` to properly terminate the even chain (this is essential to avoid a cycle). Finally, we connect `oddTail->next = evenHead` and return `head`. Edge cases: if the list has fewer than three nodes, return `head` unchanged. Time complexity is O(n) with a single pass, and space complexity is O(1) auxiliary (excluding the input list). The approach is robust because it preserves the relative order of nodes within each group and works for any length.
#include <cstddef>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Reorders the list so that all odd-indexed nodes come first, then all even-indexed nodes.
// The list is modified in-place. Returns the head of the rearranged list.
ListNode* reorderOddEven(ListNode* head) {
    // Handle empty, single, or two-node lists directly.
    if (head == nullptr || head->next == nullptr || head->next->next == nullptr) {
        return head;
    }

    // Odd chain starts at the head; even chain starts at the second node.
    ListNode* oddTail = head;
    ListNode* evenHead = head->next;
    ListNode* evenTail = head->next;

    // Traverse the remaining nodes starting from the third position.
    ListNode* current = head->next->next;
    int position = 3; // 1-based index of the current node
    while (current != nullptr) {
        if (position % 2 == 1) {
            // Odd position: append to odd chain.
            oddTail->next = current;
            oddTail = oddTail->next;
        } else {
            // Even position: append to even chain.
            evenTail->next = current;
            evenTail = evenTail->next;
        }
        current = current->next;
        ++position;
    }

    // Terminate the even chain to avoid cycles.
    evenTail->next = nullptr;

    // Connect the odd chain to the even chain.
    oddTail->next = evenHead;

    return head;
}
#include <cassert>

// Helper to create a list from an initializer list-like vector for testing.
ListNode* createList(const std::vector<int>& values) {
    if (values.empty()) return nullptr;
    ListNode* head = new ListNode(values[0]);
    ListNode* tail = head;
    for (size_t i = 1; i < values.size(); ++i) {
        tail->next = new ListNode(values[i]);
        tail = tail->next;
    }
    return head;
}

// Helper to compare a list to a vector of expected values.
bool compareList(ListNode* head, const std::vector<int>& expected) {
    ListNode* p = head;
    for (int v : expected) {
        if (p == nullptr || p->val != v) return false;
        p = p->next;
    }
    return p == nullptr;
}

int main() {
    // Test 1: Standard case with 6 nodes.
    ListNode* list1 = createList({1,2,3,4,5,6});
    list1 = reorderOddEven(list1);
    assert(compareList(list1, {1,3,5,2,4,6}));

    // Test 2: Empty list.
    ListNode* list2 = nullptr;
    list2 = reorderOddEven(list2);
    assert(list2 == nullptr);

    // Test 3: Single node.
    ListNode* list3 = createList({42});
    list3 = reorderOddEven(list3);
    assert(compareList(list3, {42}));

    // Test 4: Two nodes.
    ListNode* list4 = createList({7,8});
    list4 = reorderOddEven(list4);
    assert(compareList(list4, {7,8}));

    // Test 5: Odd number of nodes (5 elements).
    ListNode* list5 = createList({10,20,30,40,50});
    list5 = reorderOddEven(list5);
    assert(compareList(list5, {10,30,50,20,40}));

    // Test 6: All nodes have the same value.
    ListNode* list6 = createList({5,5,5,5});
    list6 = reorderOddEven(list6);
    assert(compareList(list6, {5,5,5,5}));

    // Test 7: Already in odd-even order (e.g., [1,3,5,2,4]).
    ListNode* list7 = createList({1,3,5,2,4});
    list7 = reorderOddEven(list7);
    assert(compareList(list7, {1,5,4,3,2})); // Note: positions are based on original order.

    // Test 8: Large list (e.g., 1..10).
    std::vector<int> large;
    for (int i = 1; i <= 10; ++i) large.push_back(i);
    ListNode* list8 = createList(large);
    list8 = reorderOddEven(list8);
    assert(compareList(list8, {1,3,5,7,9,2,4,6,8,10}));

    // Memory cleanup (simplified: no deletion for brevity, but in real code would delete).

    return 0;
}
