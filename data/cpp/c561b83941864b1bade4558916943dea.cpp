Write a C++ function `mergeSortedLists` that takes two sorted singly-linked lists (represented by `ListNode` structs with `val` and `next` fields) and returns a new singly-linked list containing all elements from both inputs in non-decreasing sorted order. The input lists must not be modified. The function should return `nullptr` when both inputs are empty, and it should correctly handle cases where one or both lists are empty. You are free to use `new` for dynamic allocation, but you must not use any standard library containers. Ensure your solution is clean, avoids memory leaks, and is `const`-correct for the input parameters.

The task can be solved using an iterative merge approach similar to the classic merge step in merge sort. Start with a dummy head node to simplify insertion, then repeatedly compare the heads of the two input lists. Append the smaller value to the result list and advance that list’s pointer. If the values are equal, you may choose either (commonly the second list’s value). After one list becomes empty, append the remaining nodes of the other list by copying their values. The input lists are not modified—only their values are read. Edge cases include both lists empty (return `nullptr`), one list empty (return a copy of the other), and lists of unequal lengths. Time complexity is `O(m + n)` where `m` and `n` are the lengths of the two lists, and space complexity is also `O(m + n)` because a new list is created (each node is allocated separately). The algorithm uses constant extra space beyond the output list.

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Merge two sorted linked lists into a new sorted list without modifying inputs.
ListNode* mergeSortedLists(const ListNode* l1, const ListNode* l2) {
    ListNode dummy(0);          // dummy head simplifies insertion
    ListNode* tail = &dummy;

    while (l1 != nullptr && l2 != nullptr) {
        if (l1->val <= l2->val) {
            tail->next = new ListNode(l1->val);
            l1 = l1->next;
        } else {
            tail->next = new ListNode(l2->val);
            l2 = l2->next;
        }
        tail = tail->next;
    }

    // Append remaining nodes from either list
    while (l1 != nullptr) {
        tail->next = new ListNode(l1->val);
        tail = tail->next;
        l1 = l1->next;
    }
    while (l2 != nullptr) {
        tail->next = new ListNode(l2->val);
        tail = tail->next;
        l2 = l2->next;
    }

    return dummy.next;  // first real node, or nullptr if both lists were empty
}

#include <cassert>

// Helper to build a list from an initializer list (for testing only)
ListNode* buildList(std::initializer_list<int> vals) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int v : vals) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper to compare two lists element-wise
bool listsEqual(const ListNode* a, const ListNode* b) {
    while (a != nullptr && b != nullptr) {
        if (a->val != b->val) return false;
        a = a->next;
        b = b->next;
    }
    return a == nullptr && b == nullptr;
}

// Helper to free a list
void freeList(ListNode* head) {
    while (head != nullptr) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: Both empty
    assert(mergeSortedLists(nullptr, nullptr) == nullptr);

    // Test 2: One empty, one non-empty
    ListNode* l1 = buildList({1, 3, 5});
    ListNode* merged = mergeSortedLists(l1, nullptr);
    assert(listsEqual(merged, buildList({1, 3, 5})));
    freeList(merged);

    // Test 3: Two non-empty, interleaved
    ListNode* l2 = buildList({2, 4, 6});
    merged = mergeSortedLists(l1, l2);
    assert(listsEqual(merged, buildList({1, 2, 3, 4, 5, 6})));
    freeList(merged);

    // Test 4: Duplicate values
    ListNode* l3 = buildList({1, 2, 2});
    ListNode* l4 = buildList({2, 3});
    merged = mergeSortedLists(l3, l4);
    assert(listsEqual(merged, buildList({1, 2, 2, 2, 3})));
    freeList(merged);

    // Test 5: All values from one list are smaller
    ListNode* l5 = buildList({1, 1});
    ListNode* l6 = buildList({2, 3, 4});
    merged = mergeSortedLists(l5, l6);
    assert(listsEqual(merged, buildList({1, 1, 2, 3, 4})));
    freeList(merged);

    // Cleanup original test lists
    freeList(l1);
    freeList(l2);
    freeList(l3);
    freeList(l4);
    freeList(l5);
    freeList(l6);

    return 0;
}
