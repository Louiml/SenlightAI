// Write a C++ function `ListNode* mergeSortedLists(vector<ListNode*>& lists)` that accepts a vector of pointers to sorted singly-linked lists (each list is sorted in non-decreasing order) and returns a single sorted linked list containing all the elements from the input lists. The input `lists` may be empty, in which case the function should return `nullptr`. The function may modify the original lists' nodes (reusing them rather than creating copies) and may reorder the input vector's elements. Each node contains an integer value and a `next` pointer; the `ListNode` structure is defined as `struct ListNode { int val; ListNode *next; ListNode(int x) : val(x), next(nullptr) {} };`. The lists themselves may be of any length (including zero), and there may be duplicate values across or within lists. The function should handle up to thousands of lists and millions of total nodes efficiently.

// The core idea is to merge the lists pairwise using a helper that merges two sorted linked lists. Starting with the given vector, repeatedly take the first two lists, merge them into one sorted list, place the result at the end of the vector, and remove the two original lists. Continue this until only one list remains, which is the final merged result. For an empty input, immediately return `nullptr`. The merge of two lists uses the classic technique: create a dummy head, then compare the front nodes of the two lists, appending the smaller to the result and advancing that list. When one list becomes empty, attach the remaining portion of the other. This approach has a total time complexity of \(O(N \log K)\), where \(N\) is the total number of nodes and \(K\) is the number of lists, because each node participates in about \(\log_2 K\) merges. The auxiliary space is \(O(1)\) for the merging itself (excluding the temporary vectors used during pairwise combination), but the vector operations may require \(O(K)\) extra space. Edge cases include: empty input vector, a vector containing only null pointers, input lists with one element each, and lists with many duplicates.

#include <vector>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Merge two sorted linked lists into one sorted list, reusing nodes.
ListNode* mergeTwoSortedLists(ListNode* l1, ListNode* l2) {
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

// Merge a vector of sorted linked lists into one sorted linked list.
// Returns nullptr if the input vector is empty or contains only null pointers.
ListNode* mergeSortedLists(std::vector<ListNode*>& lists) {
    if (lists.empty()) {
        return nullptr;
    }
    while (lists.size() > 1) {
        ListNode* merged = mergeTwoSortedLists(lists[0], lists[1]);
        // Remove the first two lists and append the merged result at the end.
        lists.erase(lists.begin(), lists.begin() + 2);
        lists.push_back(merged);
    }
    return lists[0];
}

#include <cassert>
#include <vector>

// (The ListNode and mergeSortedLists declarations are assumed present from the solution.)

// Helper to build a list from an initializer list.
ListNode* buildList(std::initializer_list<int> vals) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int v : vals) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper to compare a list to a vector of expected values.
bool listEquals(ListNode* head, std::initializer_list<int> expected) {
    std::vector<int> actual;
    while (head) {
        actual.push_back(head->val);
        head = head->next;
    }
    std::vector<int> exp(expected);
    return actual == exp;
}

int main() {
    // Test 1: Typical case with multiple lists.
    std::vector<ListNode*> lists1;
    lists1.push_back(buildList({1, 4, 5}));
    lists1.push_back(buildList({1, 3, 4}));
    lists1.push_back(buildList({2, 6}));
    ListNode* result1 = mergeSortedLists(lists1);
    assert(listEquals(result1, {1, 1, 2, 3, 4, 4, 5, 6}));

    // Test 2: Empty input vector.
    std::vector<ListNode*> lists2;
    assert(mergeSortedLists(lists2) == nullptr);

    // Test 3: Vector with only null pointers.
    std::vector<ListNode*> lists3;
    lists3.push_back(nullptr);
    lists3.push_back(nullptr);
    ListNode* result3 = mergeSortedLists(lists3);
    assert(result3 == nullptr);

    // Test 4: Single list.
    std::vector<ListNode*> lists4;
    lists4.push_back(buildList({-2, 0, 3}));
    ListNode* result4 = mergeSortedLists(lists4);
    assert(listEquals(result4, {-2, 0, 3}));

    // Test 5: Lists with duplicate and negative values.
    std::vector<ListNode*> lists5;
    lists5.push_back(buildList({-5, -5, -1}));
    lists5.push_back(buildList({-10, 0, 0}));
    lists5.push_back(buildList({-3, -3, 7}));
    ListNode* result5 = mergeSortedLists(lists5);
    assert(listEquals(result5, {-10, -5, -5, -3, -3, -1, 0, 0, 7}));

    // Test 6: One empty list and one non-empty list.
    std::vector<ListNode*> lists6;
    lists6.push_back(nullptr);
    lists6.push_back(buildList({1, 2}));
    ListNode* result6 = mergeSortedLists(lists6);
    assert(listEquals(result6, {1, 2}));

    // Test 7: Many tiny lists (stress check with 5 lists each of one element).
    std::vector<ListNode*> lists7;
    lists7.push_back(new ListNode(5));
    lists7.push_back(new ListNode(1));
    lists7.push_back(new ListNode(9));
    lists7.push_back(new ListNode(0));
    lists7.push_back(new ListNode(7));
    ListNode* result7 = mergeSortedLists(lists7);
    assert(listEquals(result7, {0, 1, 5, 7, 9}));
}
