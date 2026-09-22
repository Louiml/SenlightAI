Write a C++ function named `mergeSortedLists` that takes two pointers to the heads of two sorted singly-linked lists (each containing non-negative integers in non-decreasing order) and returns a pointer to the head of a new merged list that is also sorted in non-decreasing order. The function must not allocate new list nodes except for the dummy head used internally; it should reuse the existing nodes from the input lists. The input lists may be empty (nullptr), and the function must handle the case where one or both lists are empty. The resulting merged list must not contain any cycles and must preserve the relative order of equal-valued nodes (i.e., if a value appears in both lists, nodes from the first list should appear before nodes from the second list when values are equal). Provide a struct `ListNode` with members `int val; ListNode* next;` and appropriate constructors. The function should be safe to call with lists that have distinct or identical values, and it should not modify the input list heads' pointers beyond relinking them into the merged output.

The solution uses the standard iterative merge algorithm for two sorted linked lists. We create a dummy head node (with value 0) to simplify the logic and avoid special-casing the head insertion. We maintain a `tail` pointer that always points to the last node of the merged list we are building. We compare the current nodes of both input lists (`l1` and `l2`). If `l1`'s value is less than or equal to `l2`'s value, we attach `l1` to the tail, advance `l1`, and move the tail forward; otherwise we do the same with `l2`. The `<=` ensures stability when values are equal, preserving first-list order. After the main loop ends (because one list became null), we attach the remaining non-null list entirely to the tail. Finally, we return `dummy->next` (skipping the dummy). Edge cases: both lists empty returns nullptr; one list empty returns the other list directly. Time complexity is O(n + m), where n and m are the lengths of the two lists, because we visit each node once. Space complexity is O(1) auxiliary (only the dummy node), excluding the output list nodes which are reused.

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Merge two sorted linked lists into one sorted list, reusing existing nodes.
ListNode* mergeSortedLists(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);          // Dummy head to simplify edge cases.
    ListNode* tail = &dummy;    // Tail of the merged list.

    while (l1 != nullptr && l2 != nullptr) {
        if (l1->val <= l2->val) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }

    // Attach the remaining part of the non-empty list, if any.
    if (l1 != nullptr) {
        tail->next = l1;
    } else {
        tail->next = l2;
    }

    return dummy.next;
}

#include <cassert>
#include <vector>

// Helper to build a list from a vector.
ListNode* makeList(const std::vector<int>& values) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int v : values) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper to compare a list with a vector.
bool listEquals(ListNode* head, const std::vector<int>& expected) {
    ListNode* current = head;
    for (size_t i = 0; i < expected.size(); ++i) {
        if (current == nullptr || current->val != expected[i]) {
            return false;
        }
        current = current->next;
    }
    return current == nullptr;
}

int main() {
    // Both lists non-empty, typical case.
    ListNode* l1 = makeList({1, 3, 5});
    ListNode* l2 = makeList({2, 4, 6});
    ListNode* merged = mergeSortedLists(l1, l2);
    assert(listEquals(merged, {1, 2, 3, 4, 5, 6}));

    // One empty list.
    l1 = makeList({});
    l2 = makeList({1, 2});
    merged = mergeSortedLists(l1, l2);
    assert(listEquals(merged, {1, 2}));

    // Both empty.
    l1 = makeList({});
    l2 = makeList({});
    merged = mergeSortedLists(l1, l2);
    assert(merged == nullptr);

    // Equal values across lists, stability (first list first).
    l1 = makeList({1, 2, 2});
    l2 = makeList({2, 3});
    merged = mergeSortedLists(l1, l2);
    assert(listEquals(merged, {1, 2, 2, 2, 3}));

    // All values from second list first.
    l1 = makeList({0});
    l2 = makeList({-1, 1});
    merged = mergeSortedLists(l1, l2);
    assert(listEquals(merged, {-1, 0, 1}));

    // Large lists with duplicates.
    l1 = makeList({1, 1, 1});
    l2 = makeList({0, 1, 2});
    merged = mergeSortedLists(l1, l2);
    assert(listEquals(merged, {0, 1, 1, 1, 1, 2}));

    return 0;
}
