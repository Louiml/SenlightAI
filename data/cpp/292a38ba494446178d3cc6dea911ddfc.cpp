// Write a C++ function `mergeSortedLists` that takes two sorted linked lists (represented by head pointers) as input, where the first list may be shorter than the second, and merges the first list into the second in sorted order, modifying the lists in place. The function should return the head of the resulting merged list (which is the second list after merging). If the first list is empty, return the second list unchanged. The lists contain integers, and duplicates are allowed. The function must not allocate new nodes; it can only rearrange existing nodes by changing their `next` pointers. After the operation, the first list is consumed, and the second list becomes the merged sorted list.

#include <cassert>
#include <vector>

// Helper to create a list from a vector
ListNode* makeList(const std::vector<int>& vals) {
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    for (int v : vals) {
        ListNode* node = new ListNode(v);
        if (head == nullptr) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

// Helper to convert list to vector
std::vector<int> listToVector(ListNode* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

int main() {
    // Test 1: basic merge, first shorter than second
    ListNode* first = makeList({4, 6, 8});
    ListNode* second = makeList({1, 2, 5, 9, 9});
    ListNode* merged = mergeSortedLists(first, second);
    assert(listToVector(merged) == std::vector<int>({1, 2, 4, 5, 6, 8, 9, 9}));

    // Test 2: first empty
    first = nullptr;
    second = makeList({1, 3, 5});
    merged = mergeSortedLists(first, second);
    assert(listToVector(merged) == std::vector<int>({1, 3, 5}));

    // Test 3: second has all smaller values than first
    first = makeList({10, 20});
    second = makeList({1, 2});
    merged = mergeSortedLists(first, second);
    assert(listToVector(merged) == std::vector<int>({1, 2, 10, 20}));

    // Test 4: first has all smaller values than second
    first = makeList({1, 2});
    second = makeList({10, 20});
    merged = mergeSortedLists(first, second);
    assert(listToVector(merged) == std::vector<int>({1, 2, 10, 20}));

    // Test 5: duplicate values across lists
    first = makeList({2, 2, 3});
    second = makeList({1, 2, 2, 4});
    merged = mergeSortedLists(first, second);
    assert(listToVector(merged) == std::vector<int>({1, 2, 2, 2, 2, 3, 4}));

    // Test 6: both lists have one element each
    first = makeList({5});
    second = makeList({3});
    merged = mergeSortedLists(first, second);
    assert(listToVector(merged) == std::vector<int>({3, 5}));

    // Test 7: first has more elements than second (still works)
    first = makeList({1, 4, 7, 9});
    second = makeList({2, 5});
    merged = mergeSortedLists(first, second);
    assert(listToVector(merged) == std::vector<int>({1, 2, 4, 5, 7, 9}));

    // Test 8: second empty (return first)
    first = makeList({1, 2});
    second = nullptr;
    merged = mergeSortedLists(first, second);
    assert(listToVector(merged) == std::vector<int>({1, 2}));

    return 0;
}

#include <iostream>

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Merges the sorted list 'first' into the sorted list 'second' in place.
// Returns the head of the merged list (which is the second list after merging).
ListNode* mergeSortedLists(ListNode* first, ListNode* second) {
    if (first == nullptr) {
        return second;
    }
    if (second == nullptr) {
        return first;
    }

    ListNode* dummy = new ListNode(0); // dummy to simplify head insertion
    dummy->next = second;
    ListNode* prev = dummy;          // last node of merged portion
    ListNode* curr = second;         // current node in second list
    ListNode* f = first;             // current node in first list

    while (curr != nullptr) {
        // Insert all nodes from first that are smaller than curr
        while (f != nullptr && f->val < curr->val) {
            ListNode* nextF = f->next;
            // Insert f before curr
            f->next = curr;
            prev->next = f;
            prev = f;
            f = nextF;
        }
        if (f == nullptr) {
            break;
        }
        // Move to next node in second list
        prev = curr;
        curr = curr->next;
    }

    // Append remaining nodes from first (if any) at the end
    if (f != nullptr) {
        // prev is the last node of the merged list so far
        prev->next = f;
        // Need to ensure the tail is correctly linked; but f is already sorted and its tail is nullptr
    }

    ListNode* result = dummy->next;
    delete dummy; // clean up dummy node
    return result;
}

// The key idea is to traverse the second list while maintaining a pointer `prev` to the last node of the merged portion and a pointer `current` to the node being examined in the second list. Simultaneously, we traverse the first list with a pointer `first`. For each node in the second list, we insert nodes from the first list that are smaller than the current second-list node. Since both lists are sorted, we can advance through the first list and insert each qualifying node before the current second-list node. If the first list still has remaining nodes after traversing all of the second list, we append them at the end of the second list. Edge cases: if the first list is empty, return the second list. If the first list has the smallest elements, they must be inserted at the head of the second list, so we need to update the head pointer. If the second list is empty, the merged list is just the first list (but the task assumes the second list has elements, though we handle it defensively). Time complexity is O(n + m) where n and m are the lengths of the first and second lists, since each node is visited once. Space complexity is O(1) because we only use a few pointers and do not allocate new nodes.
