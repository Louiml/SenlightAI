// Write a C++ function `ListNode* reverseLinkedList(ListNode* head)` that reverses a singly linked list in-place and returns the new head of the reversed list. The list nodes are defined by the struct `ListNode` with fields `int val` and `ListNode* next`. The input may be an empty list (nullptr) or a list of any positive length. The function must not allocate new nodes; it must only rearrange the existing `next` pointers. The reversal must be iterative (no recursion) and use only constant extra space beyond a few local pointers. The function should be `const`-correct where applicable (i.e., it does not modify the `val` fields, but it does modify the `next` pointers). Include necessary headers.

The iterative reversal uses three pointers: `prev`, `curr`, and `next`. Initially, `prev` is `nullptr`, `curr` is `head`. In each iteration, save `curr->next` into `next` (because we are about to overwrite it), then set `curr->next = prev` to reverse the link, then move `prev` to `curr` and `curr` to `next`. Continue until `curr` becomes `nullptr`. At that point, `prev` points to the new head (the last node of the original list). Edge cases: empty list (`head == nullptr`) and single-node list (`head->next == nullptr`) both return immediately without entering the loop, correctly returning `head`. Time complexity is O(n) where n is the number of nodes, because each node is visited exactly once. Space complexity is O(1) auxiliary, as only three pointers are used, regardless of list length.

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Reverse a singly linked list in-place, iteratively.
// Returns the new head of the reversed list.
ListNode* reverseLinkedList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    
    while (curr != nullptr) {
        ListNode* next = curr->next; // save the old next
        curr->next = prev;           // reverse the link
        prev = curr;                 // move prev forward
        curr = next;                 // move curr forward
    }
    
    return prev; // prev is the new head
}

#include <cassert>

int main() {
    // Helper to build a list from an initializer list for testing.
    ListNode* buildList(std::initializer_list<int> vals) {
        ListNode* dummy = new ListNode(0);
        ListNode* tail = dummy;
        for (int v : vals) {
            tail->next = new ListNode(v);
            tail = tail->next;
        }
        ListNode* head = dummy->next;
        delete dummy;
        return head;
    }

    // Helper to convert list to vector for comparison.
    std::vector<int> listToVector(ListNode* head) {
        std::vector<int> result;
        while (head) {
            result.push_back(head->val);
            head = head->next;
        }
        return result;
    }

    // Helper to delete list to avoid leaks.
    void deleteList(ListNode* head) {
        while (head) {
            ListNode* next = head->next;
            delete head;
            head = next;
        }
    }

    // Test 1: Empty list.
    assert(reverseLinkedList(nullptr) == nullptr);

    // Test 2: Single node.
    ListNode* single = new ListNode(5);
    ListNode* singleReversed = reverseLinkedList(single);
    assert(singleReversed == single);
    assert(singleReversed->val == 5);
    assert(singleReversed->next == nullptr);
    delete single;

    // Test 3: Two nodes.
    ListNode* two = buildList({1, 2});
    ListNode* twoRev = reverseLinkedList(two);
    assert((listToVector(twoRev) == std::vector<int>{2, 1}));
    deleteList(twoRev);

    // Test 4: Multiple nodes.
    ListNode* multi = buildList({1, 2, 3, 4, 5});
    ListNode* multiRev = reverseLinkedList(multi);
    assert((listToVector(multiRev) == std::vector<int>{5, 4, 3, 2, 1}));
    deleteList(multiRev);

    // Test 5: Already reversed (i.e., original ascending becomes descending).
    ListNode* desc = buildList({5, 4, 3, 2, 1});
    ListNode* descRev = reverseLinkedList(desc);
    assert((listToVector(descRev) == std::vector<int>{1, 2, 3, 4, 5}));
    deleteList(descRev);

    // Test 6: With duplicates.
    ListNode* dup = buildList({7, 7, 8, 7});
    ListNode* dupRev = reverseLinkedList(dup);
    assert((listToVector(dupRev) == std::vector<int>{7, 8, 7, 7}));
    deleteList(dupRev);

    return 0;
}
