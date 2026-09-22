Write a C++ function named `mergeSortedLists` that takes a `std::vector<ListNode*>` where each `ListNode` is a singly-linked list (with `val` and `next` members) that is already sorted in non-decreasing order, and returns a pointer to the head of a new singly-linked list containing all elements from all input lists, in non-decreasing sorted order. The function must not modify the input lists; it must create new nodes for the result. You may assume `ListNode` is defined as: `struct ListNode { int val; ListNode *next; ListNode(int x) : val(x), next(nullptr) {} };` Handle the case where the vector is empty or contains null pointers (empty lists). The solution must be self-contained and include necessary headers.

The simplest and clearest approach is to iteratively merge two lists at a time, starting with an empty result. For each list in the input vector, merge it with the current result using a standard two-pointer merge algorithm. In each merge, we create a new dummy head node and a tail pointer. While both lists are non-null, compare their values, attach the smaller one (by creating a new node with that value) to the tail, and advance the corresponding input pointer. After one list is exhausted, append copies of all remaining nodes from the other list. The function returns `dummy->next` (skipping the dummy). Edge cases: an empty vector returns `nullptr`; null pointers in the vector are ignored by the merge (since the merge handles null inputs). Time complexity is O(N) where N is the total number of nodes across all lists, because each node is copied once during the sequential merging process. Auxiliary space is O(1) beyond the new nodes created for the result (which itself requires O(N) space). For a vector of k lists and total nodes N, the sequential merging approach is correct but not the most efficient (optimal is O(N log k) via heap/divide-and-conquer), but it is acceptable for this task.

#include <vector>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Merge two sorted linked lists into a new sorted list.
ListNode* mergeTwoSorted(const ListNode* l1, const ListNode* l2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (l1 && l2) {
        if (l1->val <= l2->val) {
            tail->next = new ListNode(l1->val);
            l1 = l1->next;
        } else {
            tail->next = new ListNode(l2->val);
            l2 = l2->next;
        }
        tail = tail->next;
    }
    while (l1) {
        tail->next = new ListNode(l1->val);
        tail = tail->next;
        l1 = l1->next;
    }
    while (l2) {
        tail->next = new ListNode(l2->val);
        tail = tail->next;
        l2 = l2->next;
    }
    return dummy.next;
}

// Merge all sorted lists in the vector into a single sorted list.
ListNode* mergeSortedLists(const std::vector<ListNode*>& lists) {
    ListNode* result = nullptr;
    for (const ListNode* list : lists) {
        result = mergeTwoSorted(result, list);
    }
    return result;
}

#include <cassert>
#include <vector>

// ListNode definition and mergeSortedLists (as in Solution) would be here.

int main() {
    // Helper to create a list from initializer list
    auto makeList = [](std::initializer_list<int> vals) {
        ListNode* head = nullptr;
        ListNode* tail = nullptr;
        for (int v : vals) {
            ListNode* n = new ListNode(v);
            if (!head) {
                head = n;
            } else {
                tail->next = n;
            }
            tail = n;
        }
        return head;
    };

    // Helper to convert list to vector for comparison
    auto listToVector = [](ListNode* head) {
        std::vector<int> out;
        while (head) {
            out.push_back(head->val);
            head = head->next;
        }
        return out;
    };

    // Test 1: Empty vector
    std::vector<ListNode*> lists1;
    assert(mergeSortedLists(lists1) == nullptr);

    // Test 2: Single empty list
    std::vector<ListNode*> lists2 = {nullptr};
    assert(mergeSortedLists(lists2) == nullptr);

    // Test 3: Single non-empty list
    ListNode* l3 = makeList({1,2,3});
    std::vector<ListNode*> lists3 = {l3};
    auto res3 = mergeSortedLists(lists3);
    assert(listToVector(res3) == (std::vector<int>{1,2,3}));

    // Test 4: Two lists
    ListNode* l4a = makeList({1,4,5});
    ListNode* l4b = makeList({1,3,4});
    std::vector<ListNode*> lists4 = {l4a, l4b};
    auto res4 = mergeSortedLists(lists4);
    assert(listToVector(res4) == (std::vector<int>{1,1,3,4,4,5}));

    // Test 5: Multiple lists with duplicates and nulls
    ListNode* l5a = makeList({});
    ListNode* l5b = makeList({2,6});
    ListNode* l5c = makeList({1,3});
    std::vector<ListNode*> lists5 = {l5a, l5b, l5c, nullptr};
    auto res5 = mergeSortedLists(lists5);
    assert(listToVector(res5) == (std::vector<int>{1,2,3,6}));

    // Test 6: Negative and zero values
    ListNode* l6a = makeList({-10, 0, 5});
    ListNode* l6b = makeList({-5, -1});
    std::vector<ListNode*> lists6 = {l6a, l6b};
    auto res6 = mergeSortedLists(lists6);
    assert(listToVector(res6) == (std::vector<int>{-10,-5,-1,0,5}));

    // Test 7: Many small lists (stress)
    std::vector<ListNode*> lists7;
    for (int i = 0; i < 100; ++i) {
        lists7.push_back(makeList({i, i+1}));
    }
    auto res7 = mergeSortedLists(lists7);
    auto v7 = listToVector(res7);
    assert(v7.size() == 200);
    for (size_t i = 1; i < v7.size(); ++i) {
        assert(v7[i-1] <= v7[i]);
    }
    assert(v7.front() == 0 && v7.back() == 100);

    // Cleanup (optional for testing, not required in final exercise)
    // Note: in a real test, you'd delete nodes to avoid leaks, but for brevity here we skip.

    return 0;
}
