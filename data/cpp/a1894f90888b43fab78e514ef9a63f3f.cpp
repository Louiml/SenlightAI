// Write a standalone C++ function that merges two sorted singly-linked lists into a single sorted linked list without allocating new nodes (reuse the existing nodes). The function must accept two pointers to the heads of the input lists (which may be empty, i.e., `nullptr`) and return a pointer to the head of the resulting merged list. The lists contain integers sorted in non-decreasing order. Your implementation must correctly handle cases where one or both input lists are empty, lists with duplicate values, and lists of different lengths. The merged list must preserve the sorted order (non-decreasing). The solution should not modify the values inside nodes and must maintain the original node objects (no copying). Provide a free function named `mergeSortedLists` that takes two `ListNode*` arguments (where `ListNode` has `int val` and `ListNode* next`) and returns `ListNode*`. You may include a `ListNode` definition in your solution if not provided by the task context. Your function must use constant auxiliary space (only a few pointers) and run in O(n + m) time, where n and m are the lengths of the two input lists.
The core idea is to use a dummy head node to simplify pointer manipulation when merging. We maintain a `tail` pointer that always points to the last node in the merged list being built. While both input pointers are non-null, we compare their current node values: the smaller (or equal) value node is appended to the tail and its corresponding input pointer advances. Because the inputs are sorted, this guarantees the merged list stays sorted. When one list becomes empty, we simply attach the remaining portion of the other list to the tail. Since we reuse existing nodes (only changing `next` pointers), no new memory is allocated, and auxiliary space is O(1). Edge cases include both lists empty (returning `nullptr`), one empty (returning the other), and duplicate values—when equal, we can choose either node to append first; here, we append from the first list when values are equal (using `<=`). The dummy head avoids special-casing the first node, and we return `dummy->next` after discarding the dummy. Time complexity is linear in the total number of nodes, dominated by traversing each list exactly once.
#include <cstddef>

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

// Merge two sorted linked lists, reusing nodes. Returns head of merged list.
ListNode* mergeSortedLists(ListNode* list1, ListNode* list2) {
    ListNode dummy;                 // dummy head to simplify insertion
    ListNode* tail = &dummy;        // points to the last node in merged list

    while (list1 != nullptr && list2 != nullptr) {
        if (list1->val <= list2->val) {
            tail->next = list1;
            list1 = list1->next;
        } else {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }

    // Attach the remaining non-empty list (if any)
    tail->next = (list1 != nullptr) ? list1 : list2;

    return dummy.next;              // first real node (or nullptr if both empty)
}
#include <cassert>

int main() {
    // Helper to build a list from a vector-like initializer list (for testing)
    ListNode* makeList(std::initializer_list<int> values) {
        ListNode* head = nullptr;
        ListNode* tail = nullptr;
        for (int v : values) {
            ListNode* node = new ListNode(v);
            if (head == nullptr) {
                head = node;
                tail = node;
            } else {
                tail->next = node;
                tail = node;
            }
        }
        return head;
    }

    // Helper to free list memory
    void deleteList(ListNode* head) {
        while (head != nullptr) {
            ListNode* next = head->next;
            delete head;
            head = next;
        }
    }

    // Helper to check if a list is sorted and equal in length to expected
    bool checkSortedList(ListNode* head, std::initializer_list<int> expected) {
        auto it = expected.begin();
        while (head != nullptr && it != expected.end()) {
            if (head->val != *it) return false;
            head = head->next;
            ++it;
        }
        return (head == nullptr && it == expected.end());
    }

    // Test 1: both empty
    ListNode* merged = mergeSortedLists(nullptr, nullptr);
    assert(merged == nullptr);

    // Test 2: one empty, other non-empty
    ListNode* l1 = makeList({1, 3, 5});
    merged = mergeSortedLists(l1, nullptr);
    assert(checkSortedList(merged, {1, 3, 5}));
    deleteList(merged);

    // Test 3: other empty, first non-empty
    l1 = makeList({2, 4});
    merged = mergeSortedLists(nullptr, l1);
    assert(checkSortedList(merged, {2, 4}));
    deleteList(merged);

    // Test 4: normal case with duplicates and different lengths
    ListNode* a = makeList({1, 2, 4});
    ListNode* b = makeList({1, 3, 4, 5});
    merged = mergeSortedLists(a, b);
    assert(checkSortedList(merged, {1, 1, 2, 3, 4, 4, 5}));
    deleteList(merged);

    // Test 5: all duplicates
    a = makeList({5, 5});
    b = makeList({5, 5, 5});
    merged = mergeSortedLists(a, b);
    assert(checkSortedList(merged, {5, 5, 5, 5, 5}));
    deleteList(merged);

    // Test 6: lists with negative numbers and zero
    a = makeList({-3, -1, 0});
    b = makeList({-2, 0, 2});
    merged = mergeSortedLists(a, b);
    assert(checkSortedList(merged, {-3, -2, -1, 0, 0, 2}));
    deleteList(merged);

    // Test 7: single node each
    a = makeList({7});
    b = makeList({3});
    merged = mergeSortedLists(a, b);
    assert(checkSortedList(merged, {3, 7}));
    deleteList(merged);

    return 0;
}
