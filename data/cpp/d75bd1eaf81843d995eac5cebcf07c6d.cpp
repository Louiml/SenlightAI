/*
Given a vector of singly-linked lists, where each list is already sorted in ascending order, write a C++ function named `mergeSortedLinkedLists` that merges all the given lists into a single sorted linked list and returns a pointer to the head of the merged list. The lists may contain duplicate values, may be empty, and the vector may be empty. The function should not allocate new nodes; instead, it must reuse the existing nodes by rearranging their `next` pointers. For consistency, define a simple `ListNode` struct with an integer `val` and a `ListNode* next` member, as well as a helper function (outside the solution function) to build a list from an initializer list and a helper to compare two lists for equality in tests. Implement the solution cleanly, handling all edge cases (empty vector, one list, multiple lists with varied lengths and duplicates).
*/

#include <vector>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Helper: merge two sorted linked lists into one sorted list.
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (l1 && l2) {
        if (l1->val <= l2->val) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }
    tail->next = (l1 != nullptr) ? l1 : l2;
    return dummy.next;
}

// Main solution: merge a vector of sorted linked lists into one sorted list.
ListNode* mergeSortedLinkedLists(std::vector<ListNode*>& lists) {
    if (lists.empty()) {
        return nullptr;
    }
    ListNode* merged = lists[0];
    for (size_t i = 1; i < lists.size(); ++i) {
        merged = mergeTwoLists(merged, lists[i]);
    }
    return merged;
}

// Helper to build a list from an initializer list (for testing, not part of solution).
ListNode* makeList(std::initializer_list<int> values) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int v : values) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper to compare two lists for equality (for testing).
bool listsEqual(ListNode* a, ListNode* b) {
    while (a && b) {
        if (a->val != b->val) return false;
        a = a->next;
        b = b->next;
    }
    return (a == nullptr && b == nullptr);
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Example from the problem statement
    ListNode* a = makeList({1, 4, 5});
    ListNode* b = makeList({1, 3, 4});
    ListNode* c = makeList({2, 6});
    std::vector<ListNode*> lists = {a, b, c};
    ListNode* merged = mergeSortedLinkedLists(lists);
    assert(listsEqual(merged, makeList({1, 1, 2, 3, 4, 4, 5, 6})));

    // Test 2: Empty vector
    std::vector<ListNode*> empty;
    assert(mergeSortedLinkedLists(empty) == nullptr);

    // Test 3: Single list
    ListNode* single = makeList({2, 3, 4});
    std::vector<ListNode*> singleList = {single};
    assert(listsEqual(mergeSortedLinkedLists(singleList), makeList({2, 3, 4})));

    // Test 4: Lists with empty inner lists, duplicates, and varied lengths
    ListNode* d = nullptr; // empty
    ListNode* e = makeList({1, 1});
    ListNode* f = makeList({0});
    ListNode* g = makeList({2});
    std::vector<ListNode*> lists2 = {d, e, f, g};
    assert(listsEqual(mergeSortedLinkedLists(lists2), makeList({0, 1, 1, 2})));

    // Test 5: All lists empty
    std::vector<ListNode*> allEmpty = {nullptr, nullptr, nullptr};
    assert(mergeSortedLinkedLists(allEmpty) == nullptr);

    // Test 6: negative numbers and one large list
    ListNode* h = makeList({-5, -1, 0});
    ListNode* i = makeList({-10, -2});
    std::vector<ListNode*> lists3 = {h, i};
    assert(listsEqual(mergeSortedLinkedLists(lists3), makeList({-10, -5, -2, -1, 0})));
}

// The most straightforward approach is to iteratively merge the lists one by one using a helper `mergeTwoLists` function that merges two sorted linked lists in linear time by comparing node values and rewiring pointers. Start with the first list as the accumulator, then for each subsequent list, merge it with the current accumulator. This approach correctly handles duplicate values because the merge logic uses `<=` (or `<` on one side and `>=` on the other) to preserve correct ordering. Edge cases include an empty input vector (return `nullptr`), a single list (return its head unchanged), lists of differing lengths, and empty inner lists (skip them naturally because merging with `nullptr` returns the other list). The time complexity is \(O(N \cdot k)\) where \(N\) is the total number of nodes across all lists and \(k\) is the number of lists, because each merge step processes nodes from the two lists being merged, and we perform \(k-1\) merges. The auxiliary space is \(O(1)\) beyond the input and output pointers, since we only use a few temporary pointers. A more efficient approach using a min-heap would achieve \(O(N \log k)\), but the sequential pairwise merge is simpler and sufficient for the typical task.
