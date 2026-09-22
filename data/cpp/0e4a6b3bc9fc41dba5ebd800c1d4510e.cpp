Write a C++ function `ListNode* mergeSortedLinkedLists(std::vector<ListNode*>& lists)` that takes a vector of singly-linked list heads, where each list is sorted in non-decreasing order, and returns the head of a new singly-linked list containing all elements from all input lists in sorted non-decreasing order. The input lists may be empty (nullptr heads), the vector itself may be empty (in which case return nullptr), and there may be duplicate values across different lists. The function must not modify the input lists in a way that breaks their structure; however, you are free to reuse the existing nodes (i.e., rearrange pointers) rather than allocating new nodes. The ListNode structure is predefined as: `struct ListNode { int val; ListNode *next; ListNode() : val(0), next(nullptr) {} ListNode(int x) : val(x), next(nullptr) {} ListNode(int x, ListNode *next) : val(x), next(next) {} };`. Your implementation should handle up to 10^4 total nodes across all lists without stack overflow (so avoid deep recursion if possible). Provide the function as a free function (not part of a class), and ensure it is `const`-correct where appropriate (though the vector parameter is non-const because you may modify the heads if needed). The function should be self-contained, include the necessary `<bits/stdc++.h>` header or specific headers, and be ready to compile.

#include <bits/stdc++.h>
using namespace std;

// ListNode definition and solution functions as above (included here for completeness)

// Helper to convert vector<int> to linked list
ListNode* makeList(const vector<int>& vals) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int v : vals) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper to convert linked list to vector<int> for easy comparison
vector<int> listToVector(ListNode* head) {
    vector<int> result;
    while (head) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// Helper to delete linked list to avoid memory leaks
void deleteList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: Basic merge of two lists
    vector<ListNode*> lists1 = {makeList({1,4,5}), makeList({1,3,4}), makeList({2,6})};
    ListNode* merged1 = mergeSortedLinkedLists(lists1);
    assert(listToVector(merged1) == vector<int>({1,1,2,3,4,4,5,6}));
    deleteList(merged1);
    for (ListNode* h : lists1) deleteList(h);

    // Test 2: Empty vector
    vector<ListNode*> lists2;
    assert(mergeSortedLinkedLists(lists2) == nullptr);

    // Test 3: All empty lists
    vector<ListNode*> lists3 = {nullptr, nullptr, nullptr};
    assert(mergeSortedLinkedLists(lists3) == nullptr);
    // No deletion needed since all null

    // Test 4: Single list with one element
    vector<ListNode*> lists4 = {makeList({42})};
    ListNode* merged4 = mergeSortedLinkedLists(lists4);
    assert(listToVector(merged4) == vector<int>({42}));
    deleteList(merged4);
    for (ListNode* h : lists4) deleteList(h);

    // Test 5: Duplicates across many lists
    vector<ListNode*> lists5 = {makeList({1,1}), makeList({1}), makeList({1,1,1})};
    ListNode* merged5 = mergeSortedLinkedLists(lists5);
    assert(listToVector(merged5) == vector<int>({1,1,1,1,1,1}));
    deleteList(merged5);
    for (ListNode* h : lists5) deleteList(h);

    // Test 6: Negative numbers and uneven lengths
    vector<ListNode*> lists6 = {makeList({-3,5}), makeList({-10,-2,7}), makeList({4})};
    ListNode* merged6 = mergeSortedLinkedLists(lists6);
    assert(listToVector(merged6) == vector<int>({-10,-3,-2,4,5,7}));
    deleteList(merged6);
    for (ListNode* h : lists6) deleteList(h);

    // Test 7: Many empty lists mixed with non-empty
    vector<ListNode*> lists7 = {nullptr, makeList({2}), nullptr, makeList({1,3}), nullptr};
    ListNode* merged7 = mergeSortedLinkedLists(lists7);
    assert(listToVector(merged7) == vector<int>({1,2,3}));
    deleteList(merged7);
    for (ListNode* h : lists7) deleteList(h);

    return 0;
}

#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Merge two sorted linked lists iteratively (no recursion to avoid stack overflow).
ListNode* mergeTwoLists(ListNode* a, ListNode* b) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (a && b) {
        if (a->val <= b->val) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    tail->next = a ? a : b;
    return dummy.next;
}

// Merge all sorted linked lists in the vector into one sorted linked list.
// Returns nullptr if the vector is empty or all lists are empty.
ListNode* mergeSortedLinkedLists(vector<ListNode*>& lists) {
    ListNode* result = nullptr;
    for (ListNode* head : lists) {
        result = mergeTwoLists(result, head);
    }
    return result;
}

// The core problem is merging multiple sorted linked lists into one sorted list. The given code snippet uses a sequential merge: start with the first list as the result, then iteratively merge each subsequent list using a recursive two-list merge. The two-list merge recursively compares the heads of `a` and `b`: if `a->val <= b->val`, set `a->next = merge(a->next, b)` and return `a`; otherwise, set `b->next = merge(a, b->next)` and return `b`. This works because both input lists are sorted, and this recursive approach runs in O(m+n) time for two lists of lengths m and n, using O(m+n) stack space in the worst case (e.g., when one list is much longer than the other, recursion depth equals total nodes). However, for production quality and to avoid stack overflow with large inputs, we can implement the two-list merge iteratively with a dummy head node. Then for k lists with total n nodes, sequentially merging each list into the accumulating result takes O(k * n) time in the worst case (if lists are unevenly sized), but a more efficient approach uses a min-heap (priority queue) to merge all lists in O(n log k) time. For this task, the simpler sequential merge is acceptable, but to match the task's mention of "avoid stack overflow", I'll implement an iterative two-list merge and apply it sequentially. Edge cases: empty vector → nullptr; vector with some null heads → skip them during merge; all lists empty → nullptr; duplicates are handled naturally by the `<=` comparison. Time complexity: if there are k lists and total n nodes, sequential merge takes O(k * n) worst-case (when lists are balanced, it's O(n log k) but not guaranteed). Space complexity: O(1) auxiliary space (ignoring the recursion replaced by iteration). The iterative merge uses a dummy node to simplify linking.
