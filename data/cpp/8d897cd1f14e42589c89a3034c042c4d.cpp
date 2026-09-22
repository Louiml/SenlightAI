// Write a C++ function that takes the head of a singly linked list and two integers `left` and `right` (where `1 <= left <= right <= number of nodes`) and reverses the nodes from position `left` to position `right` (1-indexed), returning the head of the modified list. The function must operate in a single pass over the list (O(n) time) and use O(1) extra space beyond a few pointers. Handle edge cases like reversing the entire list, reversing only a single node, or reversing a sublist that starts at the head. Do not allocate new nodes; modify the existing list in place and return the original head (or a new head if the first node is part of the reversed segment).
// The solution uses a dummy node that points to the original head. This simplifies handling when `left == 1` (reversal starts at the head), since the dummy acts as the node before the reversed segment. We traverse the list until we reach the node just before position `left` (call it `pre`). Then we set `start` to `pre->next` (the first node to be reversed) and `then` to `start->next` (the node that will be moved). The core idea is to iteratively move the node `then` to the front of the reversed segment, one at a time, for `right - left` iterations. In each iteration, we perform the following pointer updates:
// - `start->next = then->next` (skip `then` in the original order)
// - `then->next = pre->next` (insert `then` at the beginning of the reversed segment)
// - `pre->next = then` (update the predecessor to point to the new front)
// - `then = start->next` (move to the next node to be moved)
// After the loop, the segment from `left` to `right` is reversed. The dummy node's `next` is the new head, which we return. Edge cases: when `left == right`, the loop runs zero times, and the list is unchanged. When `left == 1`, the dummy handles the head change. When the list has one node, the function works correctly. Time complexity is O(n) because we traverse the list once. Space complexity is O(1) since we only use a few pointers.
#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Reverse the nodes from position left to right (1-indexed) in the given list.
ListNode* reverseBetween(ListNode* head, int left, int right) {
    if (!head || left == right) {
        return head;
    }

    ListNode dummy(0, head);
    ListNode* pre = &dummy;

    // Move pre to the node just before the reversal segment.
    for (int i = 1; i < left; ++i) {
        pre = pre->next;
    }

    ListNode* start = pre->next;
    ListNode* then = start->next;

    // Move 'then' to the front of the segment (right - left) times.
    for (int i = 0; i < right - left; ++i) {
        start->next = then->next;       // Remove 'then' from its current position.
        then->next = pre->next;         // Insert 'then' at the start of the segment.
        pre->next = then;               // Update the segment's predecessor.
        then = start->next;             // Move to the next node to be moved.
    }

    return dummy.next;
}
#include <cassert>
#include <vector>

// Helper to build a list from a vector.
ListNode* buildList(const std::vector<int>& vals) {
    ListNode dummy(0);
    ListNode* cur = &dummy;
    for (int v : vals) {
        cur->next = new ListNode(v);
        cur = cur->next;
    }
    return dummy.next;
}

// Helper to convert a list to a vector for comparison.
std::vector<int> listToVector(ListNode* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// Helper to delete the list to avoid leaks.
void deleteList(ListNode* head) {
    while (head) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: Standard reversal in the middle.
    ListNode* h1 = buildList({1,2,3,4,5});
    h1 = reverseBetween(h1, 2, 4);
    assert(listToVector(h1) == std::vector<int>({1,4,3,2,5}));
    deleteList(h1);

    // Test 2: Reversing the entire list.
    ListNode* h2 = buildList({1,2,3,4,5});
    h2 = reverseBetween(h2, 1, 5);
    assert(listToVector(h2) == std::vector<int>({5,4,3,2,1}));
    deleteList(h2);

    // Test 3: Reversing a single node (no change).
    ListNode* h3 = buildList({5});
    h3 = reverseBetween(h3, 1, 1);
    assert(listToVector(h3) == std::vector<int>({5}));
    deleteList(h3);

    // Test 4: Reversal starting at head with right in the middle.
    ListNode* h4 = buildList({1,2,3,4,5});
    h4 = reverseBetween(h4, 1, 3);
    assert(listToVector(h4) == std::vector<int>({3,2,1,4,5}));
    deleteList(h4);

    // Test 5: Reversal ending at tail.
    ListNode* h5 = buildList({1,2,3,4,5});
    h5 = reverseBetween(h5, 3, 5);
    assert(listToVector(h5) == std::vector<int>({1,2,5,4,3}));
    deleteList(h5);

    // Test 6: Adjacent nodes (left+1 == right).
    ListNode* h6 = buildList({1,2,3,4});
    h6 = reverseBetween(h6, 2, 3);
    assert(listToVector(h6) == std::vector<int>({1,3,2,4}));
    deleteList(h6);

    // Test 7: Two nodes, swap both.
    ListNode* h7 = buildList({1,2});
    h7 = reverseBetween(h7, 1, 2);
    assert(listToVector(h7) == std::vector<int>({2,1}));
    deleteList(h7);

    // Test 8: Negative values.
    ListNode* h8 = buildList({-1,-2,-3,-4});
    h8 = reverseBetween(h8, 2, 3);
    assert(listToVector(h8) == std::vector<int>({-1,-3,-2,-4}));
    deleteList(h8);

    return 0;
}
