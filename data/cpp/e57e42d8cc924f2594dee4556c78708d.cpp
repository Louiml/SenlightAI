// Write a C++ function `ListNode* insertionSortList(ListNode* head)` that sorts a singly linked list in ascending order using the insertion sort algorithm. The function must operate in-place, meaning it should not create new nodes, only modify the `next` pointers of existing nodes. The input list may be empty, may contain duplicate values, and may already be sorted. The function should return a pointer to the head of the sorted list. You are not allowed to copy node values into arrays/vectors or use any other container; only pointer manipulation is permitted.
The solution maintains a sorted prefix and an unsorted suffix. We use a dummy head node to simplify insertion at the front. We track `lastSorted`, the tail of the sorted portion, and `curr`, the next node to insert. If `curr`'s value is >= `lastSorted`'s value, the sorted portion extends by moving `lastSorted` forward. Otherwise, we search from the dummy head forward to find the correct insertion position (the first node whose next value is greater than `curr`'s value). Then we remove `curr` from its current position and insert it after the found predecessor. After insertion, `curr` moves to the next unsorted node (which is `lastSorted->next`). Edge cases include an empty list (return immediately), a single node, all nodes already sorted, all nodes in reverse order, and duplicates. Time complexity is O(n²) in the worst case (reverse-sorted input) and O(n) in the best case (already sorted). Space complexity is O(1) auxiliary, besides the dummy node allocated.
#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Sort a singly linked list using in-place insertion sort.
// Returns the head of the sorted list.
ListNode* insertionSortList(ListNode* head) {
    if (head == nullptr) return head;

    ListNode* dummyHead = new ListNode(0, head);
    ListNode* lastSorted = head;
    ListNode* curr = head->next;

    while (curr != nullptr) {
        if (lastSorted->val <= curr->val) {
            // Already in correct order relative to the sorted prefix.
            lastSorted = lastSorted->next;
        } else {
            // Find insertion point from the beginning.
            ListNode* prev = dummyHead;
            while (prev->next->val <= curr->val) {
                prev = prev->next;
            }
            // Remove curr from its current position.
            lastSorted->next = curr->next;
            // Insert curr after prev.
            curr->next = prev->next;
            prev->next = curr;
        }
        // Move to the next unsorted node.
        curr = lastSorted->next;
    }

    ListNode* result = dummyHead->next;
    delete dummyHead;
    return result;
}
#include <cassert>

// Helper to build a list from an initializer list (for testing only).
ListNode* createList(std::initializer_list<int> values) {
    ListNode* dummy = new ListNode(0);
    ListNode* tail = dummy;
    for (int v : values) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    ListNode* head = dummy->next;
    delete dummy;
    return head;
}

// Helper to compare two lists for equality (for testing only).
bool listsEqual(ListNode* a, ListNode* b) {
    while (a && b) {
        if (a->val != b->val) return false;
        a = a->next;
        b = b->next;
    }
    return (a == nullptr && b == nullptr);
}

// Helper to free list memory (for testing only).
void deleteList(ListNode* head) {
    while (head) {
        ListNode* tmp = head;
        head = head->next;
        delete tmp;
    }
}

int main() {
    // Test 1: empty list -> empty
    {
        ListNode* head = nullptr;
        ListNode* sorted = insertionSortList(head);
        assert(sorted == nullptr);
    }

    // Test 2: single node
    {
        ListNode* head = new ListNode(5);
        ListNode* sorted = insertionSortList(head);
        assert(sorted->val == 5);
        assert(sorted->next == nullptr);
        delete sorted;
    }

    // Test 3: already sorted
    {
        ListNode* head = createList({1, 2, 3, 4});
        ListNode* sorted = insertionSortList(head);
        assert(listsEqual(sorted, createList({1, 2, 3, 4})));
        deleteList(sorted);
    }

    // Test 4: reverse sorted
    {
        ListNode* head = createList({4, 3, 2, 1});
        ListNode* sorted = insertionSortList(head);
        assert(listsEqual(sorted, createList({1, 2, 3, 4})));
        deleteList(sorted);
    }

    // Test 5: unsorted with duplicates
    {
        ListNode* head = createList({3, 1, 2, 1, 3});
        ListNode* sorted = insertionSortList(head);
        assert(listsEqual(sorted, createList({1, 1, 2, 3, 3})));
        deleteList(sorted);
    }

    // Test 6: negative and zero values
    {
        ListNode* head = createList({0, -5, 2, -1});
        ListNode* sorted = insertionSortList(head);
        assert(listsEqual(sorted, createList({-5, -1, 0, 2})));
        deleteList(sorted);
    }

    // Test 7: large list (100 descending)
    {
        ListNode* head = nullptr;
        for (int i = 100; i >= 1; i--) {
            ListNode* newNode = new ListNode(i);
            newNode->next = head;
            head = newNode;
        }
        ListNode* sorted = insertionSortList(head);
        ListNode* cur = sorted;
        for (int i = 1; i <= 100; i++) {
            assert(cur->val == i);
            cur = cur->next;
        }
        deleteList(sorted);
    }

    return 0;
}
