/*
Write a C++ function `insertionSortList(ListNode* head)` that sorts a singly linked list using the insertion sort algorithm. The input is a pointer to the head of the list, where each node contains an `int val` and a `ListNode* next`. The function must return the head of the sorted list. The list may be empty, may contain duplicate values, and may already be sorted or reverse sorted. The original node objects must be reused (no new nodes created except for a sentinel, which must be local to the function, not heap-allocated). The algorithm must explicitly mimic insertion sort by repeatedly taking the next node from the original list and inserting it into its correct position in a growing sorted portion of the list. Ensure the function handles edge cases such as empty lists, single-element lists, and lists with all equal values without crashing or leaking memory.
*/

#include <climits>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Sort a singly linked list using insertion sort.
// Returns the head of the sorted list.
ListNode* insertionSortList(ListNode* head) {
    // Sentinel node to simplify insertions at the beginning.
    ListNode dummy(INT_MIN);
    ListNode* curr = head;

    while (curr) {
        // Find the insertion position in the sorted list (starting from dummy).
        ListNode* position = &dummy;
        // Move position to the last node whose value is <= curr->val.
        while (position->next && position->next->val <= curr->val) {
            position = position->next;
        }

        // Save the next node from the original list before modifying curr.
        ListNode* temp = curr->next;

        // Insert curr after position.
        curr->next = position->next;
        position->next = curr;

        // Move to the next original node.
        curr = temp;
    }

    return dummy.next;
}

#include <cassert>

// Helper to create a list from an initializer list (for testing).
ListNode* createList(std::initializer_list<int> vals) {
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    for (int v : vals) {
        ListNode* node = new ListNode(v);
        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

// Helper to free a list (for testing).
void freeList(ListNode* head) {
    while (head) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

// Helper to check if a list is sorted and equals expected values.
bool listEquals(ListNode* head, std::initializer_list<int> expected) {
    for (int v : expected) {
        if (!head || head->val != v) return false;
        head = head->next;
    }
    return head == nullptr;
}

int main() {
    // Empty list.
    ListNode* empty = nullptr;
    assert(insertionSortList(empty) == nullptr);

    // Single element.
    ListNode* single = new ListNode(5);
    assert(listEquals(insertionSortList(single), {5}));
    freeList(single);

    // Already sorted.
    ListNode* sorted = createList({1, 2, 3});
    assert(listEquals(insertionSortList(sorted), {1, 2, 3}));
    freeList(sorted);

    // Reverse sorted.
    ListNode* reverse = createList({3, 2, 1});
    assert(listEquals(insertionSortList(reverse), {1, 2, 3}));
    freeList(reverse);

    // Duplicate values.
    ListNode* duplicates = createList({4, 2, 4, 2});
    assert(listEquals(insertionSortList(duplicates), {2, 2, 4, 4}));
    freeList(duplicates);

    // Mixed with negatives and larger numbers.
    ListNode* mixed = createList({-3, 10, -5, 0, 7, -3});
    assert(listEquals(insertionSortList(mixed), {-5, -3, -3, 0, 7, 10}));
    freeList(mixed);

    return 0;
}

// The core idea is to build a new sorted list incrementally from the original list. We use a local dummy node with value `INT_MIN` as a sentinel to simplify insertion at the beginning of the sorted list. We iterate through each node `curr` of the original list, and for each node, we find the position in the sorted list (starting from the dummy node) where the node's value should be inserted. That position is the node whose `next` has a value greater than or equal to `curr->val`, or the end of the list. We then insert `curr` after that position by adjusting pointers. Crucially, we save `curr->next` before modifying `curr`, so we can continue iterating the original list. The dummy node’s `next` becomes the sorted list’s head. Edge cases: empty list returns `nullptr` (dummy's next is null). Single-node list simply gets inserted after the dummy. Duplicate values are handled because we insert after the last node with a value ≤ `curr->val` (since the insertion position loop stops when `curr->val > node->val`). For a list already sorted, each node is inserted at the end. For reverse sorted, each node is inserted right after the dummy. Time complexity is O(n²) in the worst case (reverse-sorted) and O(n) in the best case (already sorted, but the loop still scans from the dummy each time, so it is O(n²) overall), but we note it as O(n²) average/worst, with O(1) auxiliary space excluding the input list (only a local dummy node). The solution correctly reuses existing nodes and does not allocate new memory except for the stack-allocated dummy.
