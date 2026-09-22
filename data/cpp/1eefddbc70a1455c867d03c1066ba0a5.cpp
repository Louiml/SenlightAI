// Write a C++ function that takes a singly linked list whose nodes contain integer values sorted in non-decreasing order and returns a new linked list with all duplicate values removed so that each distinct value appears exactly once. The function should preserve the original relative order of the remaining nodes, and it must not modify the input list (i.e., it should create new nodes rather than reusing or deleting existing ones). The input list may be empty or contain any number of duplicate groups. You must implement the `ListNode` struct as provided and write a free function `ListNode* removeDuplicates(const ListNode* head)` that constructs and returns the deduplicated list. You may assume the input list is already sorted.

#include <cassert>
#include <cstddef>

// Provided struct and function declarations (for completeness, not repeated here).

// Helper to build a list from an initializer list-like array.
ListNode* buildList(const int* values, int size) {
    if (size == 0) return nullptr;
    ListNode* head = new ListNode(values[0]);
    ListNode* tail = head;
    for (int i = 1; i < size; ++i) {
        tail->next = new ListNode(values[i]);
        tail = tail->next;
    }
    return head;
}

// Helper to compare two lists and free memory.
bool listsEqual(ListNode* a, ListNode* b) {
    while (a != nullptr && b != nullptr) {
        if (a->val != b->val) return false;
        a = a->next;
        b = b->next;
    }
    return a == nullptr && b == nullptr;
}

void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* nxt = head->next;
        delete head;
        head = nxt;
    }
}

int main() {
    // Test empty list
    assert(removeDuplicates(nullptr) == nullptr);

    // Test single node
    int single[] = {42};
    ListNode* s = buildList(single, 1);
    ListNode* sRes = removeDuplicates(s);
    assert(sRes != nullptr && sRes->val == 42 && sRes->next == nullptr);
    deleteList(s);
    deleteList(sRes);

    // Test all duplicates
    int allDup[] = {5,5,5,5};
    ListNode* ad = buildList(allDup, 4);
    ListNode* adRes = removeDuplicates(ad);
    assert(adRes != nullptr && adRes->val == 5 && adRes->next == nullptr);
    deleteList(ad);
    deleteList(adRes);

    // Test no duplicates
    int noDup[] = {1,2,3,4};
    ListNode* nd = buildList(noDup, 4);
    ListNode* ndRes = removeDuplicates(nd);
    int expectedNo[] = {1,2,3,4};
    ListNode* expNo = buildList(expectedNo, 4);
    assert(listsEqual(ndRes, expNo));
    deleteList(nd);
    deleteList(ndRes);
    deleteList(expNo);

    // Test mixed duplicates
    int mixed[] = {1,1,2,3,3,3,4,4,5};
    ListNode* mx = buildList(mixed, 9);
    ListNode* mxRes = removeDuplicates(mx);
    int expectedMix[] = {1,2,3,4,5};
    ListNode* expMix = buildList(expectedMix, 5);
    assert(listsEqual(mxRes, expMix));
    deleteList(mx);
    deleteList(mxRes);
    deleteList(expMix);

    // Test negative duplicates
    int neg[] = {-2,-2,-1,0,0,1};
    ListNode* ng = buildList(neg, 6);
    ListNode* ngRes = removeDuplicates(ng);
    int expectedNeg[] = {-2,-1,0,1};
    ListNode* expNeg = buildList(expectedNeg, 4);
    assert(listsEqual(ngRes, expNeg));
    deleteList(ng);
    deleteList(ngRes);
    deleteList(expNeg);

    // Verify input list is not modified
    int original[] = {7,7,8,8,9};
    ListNode* orig = buildList(original, 5);
    ListNode* origCopy = buildList(original, 5); // copy for comparison
    ListNode* res = removeDuplicates(orig);
    assert(listsEqual(orig, origCopy));
    int expected[] = {7,8,9};
    ListNode* exp = buildList(expected, 3);
    assert(listsEqual(res, exp));
    deleteList(orig);
    deleteList(origCopy);
    deleteList(res);
    deleteList(exp);

    return 0;
}

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Remove duplicates from a sorted linked list by creating a new list.
// The input list is not modified.
ListNode* removeDuplicates(const ListNode* head) {
    if (head == nullptr) {
        return nullptr;
    }

    ListNode* resultHead = new ListNode(head->val);
    ListNode* resultTail = resultHead;
    int lastValue = head->val;

    for (const ListNode* current = head->next; current != nullptr; current = current->next) {
        if (current->val != lastValue) {
            resultTail->next = new ListNode(current->val);
            resultTail = resultTail->next;
            lastValue = current->val;
        }
    }

    return resultHead;
}

// The solution uses a single pass over the sorted input list. Since the list is sorted, all duplicates of a value appear consecutively. We iterate through the input list, and for each distinct value we create a new node with that value and append it to the result list. To detect distinct values, we track the last value we processed: if the current node's value differs from the last appended value, we create a new node. This avoids having to manually delete or skip duplicate nodes because we are building a separate result list. Edge cases include an empty input list (return `nullptr`), a single-node list (return a new node with the same value), and a list with all equal values (return a list with one node). The algorithm runs in \(O(n)\) time, where \(n\) is the number of nodes in the input, and uses \(O(n)\) auxiliary space for the newly created result list. It does not modify the input list, respecting const correctness.
