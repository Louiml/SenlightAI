/*
Write a C++ function that takes the head of a singly linked list and returns the head of a new linked list obtained by swapping every two adjacent nodes (i.e., nodes at positions 0 and 1, 2 and 3, etc.). The function must operate in-place without modifying the node values, only rearranging the `next` pointers. If the list has an odd number of nodes, the last node stays in its position. The input list may be empty (head is `nullptr`). You are given the `ListNode` struct definition; implement the function as a free function named `swapAdjacentPairs` that accepts a `ListNode*` and returns the new head. Do not allocate any new `ListNode` objects; only rearrange existing nodes.
*/
#include <cstddef>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Swap every two adjacent nodes in the linked list and return the new head.
ListNode* swapAdjacentPairs(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    ListNode* prev = nullptr;
    ListNode* curr = head;
    ListNode* newHead = head->next;

    while (curr != nullptr && curr->next != nullptr) {
        ListNode* next = curr->next;
        ListNode* afterPair = next->next;

        next->next = curr;
        curr->next = afterPair;

        if (prev != nullptr) {
            prev->next = next;
        }

        prev = curr;
        curr = afterPair;
    }

    return newHead;
}
#include <cassert>

// Helper to build a list from initializer list (for tests only).
ListNode* buildList(std::initializer_list<int> vals) {
    ListNode* dummy = new ListNode(0);
    ListNode* tail = dummy;
    for (int v : vals) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy->next;
}

// Helper to compare list to a vector (for tests only).
bool listEquals(ListNode* head, std::initializer_list<int> vals) {
    ListNode* cur = head;
    for (int v : vals) {
        if (cur == nullptr || cur->val != v) return false;
        cur = cur->next;
    }
    return cur == nullptr;
}

int main() {
    // Empty list
    assert(listEquals(swapAdjacentPairs(nullptr), {}));

    // Single node
    ListNode* single = new ListNode(5);
    assert(listEquals(swapAdjacentPairs(single), {5}));

    // Two nodes
    ListNode* two = buildList({1, 2});
    ListNode* swappedTwo = swapAdjacentPairs(two);
    assert(listEquals(swappedTwo, {2, 1}));

    // Three nodes (odd length)
    ListNode* three = buildList({1, 2, 3});
    ListNode* swappedThree = swapAdjacentPairs(three);
    assert(listEquals(swappedThree, {2, 1, 3}));

    // Four nodes (even length)
    ListNode* four = buildList({1, 2, 3, 4});
    ListNode* swappedFour = swapAdjacentPairs(four);
    assert(listEquals(swappedFour, {2, 1, 4, 3}));

    // Six nodes
    ListNode* six = buildList({1, 2, 3, 4, 5, 6});
    ListNode* swappedSix = swapAdjacentPairs(six);
    assert(listEquals(swappedSix, {2, 1, 4, 3, 6, 5}));

    // Check that original nodes are rearranged (no new allocation)
    // Verify by checking a value: the first node after swap should be the second original node.
    // (Already covered by listEquals above.)

    // Clean up (optional in test, but shown for completeness)
    // For simplicity in a standalone test, we skip deep deletion.
    return 0;
}
// The solution uses an iterative pointer manipulation approach. We maintain three pointers: `prev` (the node before the current pair), `curr` (the first node of the current pair), and `next` (the second node of the current pair). For each pair, we perform the swap: set `next`’s `next` to `curr`, set `curr`’s `next` to whatever came after `next` originally, and connect `prev` to `next`. Then we advance `prev` to `curr` and `curr` to `curr->next` (the node after the pair). We must handle the head case separately: if the list is empty or has only one node, return as is. For the first pair, `prev` is `nullptr`, so we update the head to `next`. The loop continues while both `curr` and `curr->next` exist. Edge cases: empty list, single node, even-length list, odd-length list (last node remains). Time complexity is O(n) where n is the number of nodes, and space complexity is O(1) auxiliary (only a few pointer variables).
