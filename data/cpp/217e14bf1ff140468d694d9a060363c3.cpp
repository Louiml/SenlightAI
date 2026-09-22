Write a C++ function `ListNode* flattenLinkedList(ListNode* head)` that takes the head of a linked list where each node contains an integer value and a `next` pointer, and returns the head of a new linked list containing the same integer values sorted in non-decreasing order. The input linked list is null-terminated, may contain duplicate values, and can be empty. The function must not modify the original list; it should create a new sorted singly-linked list using the `ListNode` class defined below, with each node's value set accordingly. The original list structure must remain unchanged after the call.
The task requires producing a sorted linked list without mutating the input. The simplest robust approach is to traverse the original list, collect all values into a `std::vector<int>`, sort the vector in ascending order, then construct a new linked list from the sorted values. This respects the "do not modify the original" requirement because we only read from the original nodes. Edge cases include an empty list (return `NULL`), a single node (return a new single node with the same value), and duplicate values (they appear multiple times in the sorted output). Time complexity is O(n log n) due to sorting, where n is the number of nodes; auxiliary space is O(n) for the vector and the new linked list. The function does not need to handle cycles (input is assumed to be a valid linear list).
#include <vector>
#include <algorithm>

// Definition for singly-linked list.
class ListNode {
public:
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
};

// Return a new linked list containing the values of the input list sorted ascending.
// The input list is not modified.
ListNode* flattenLinkedList(ListNode* head) {
    if (head == nullptr) {
        return nullptr;
    }

    std::vector<int> values;
    for (ListNode* current = head; current != nullptr; current = current->next) {
        values.push_back(current->val);
    }

    std::sort(values.begin(), values.end());

    ListNode* newHead = new ListNode(values[0]);
    ListNode* current = newHead;
    for (std::size_t i = 1; i < values.size(); ++i) {
        current->next = new ListNode(values[i]);
        current = current->next;
    }
    return newHead;
}
#include <cassert>
#include <vector>

// ListNode and flattenLinkedList declarations assumed available from the solution above.
// Helper to convert vector to linked list (for testing).
ListNode* makeList(const std::vector<int>& v) {
    if (v.empty()) return nullptr;
    ListNode* head = new ListNode(v[0]);
    ListNode* cur = head;
    for (size_t i = 1; i < v.size(); ++i) {
        cur->next = new ListNode(v[i]);
        cur = cur->next;
    }
    return head;
}

// Helper to convert linked list to vector (for testing).
std::vector<int> listToVector(ListNode* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// Helper to free list memory (for testing).
void deleteList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: Normal case with unsorted values
    {
        ListNode* original = makeList({3, 1, 2});
        ListNode* sorted = flattenLinkedList(original);
        std::vector<int> result = listToVector(sorted);
        assert(result == std::vector<int>({1, 2, 3}));
        // Original remains unchanged
        std::vector<int> origVals = listToVector(original);
        assert(origVals == std::vector<int>({3, 1, 2}));
        deleteList(original);
        deleteList(sorted);
    }

    // Test 2: Already sorted
    {
        ListNode* original = makeList({1, 2, 3});
        ListNode* sorted = flattenLinkedList(original);
        std::vector<int> result = listToVector(sorted);
        assert(result == std::vector<int>({1, 2, 3}));
        deleteList(original);
        deleteList(sorted);
    }

    // Test 3: Reverse order
    {
        ListNode* original = makeList({5, 4, 3, 2, 1});
        ListNode* sorted = flattenLinkedList(original);
        std::vector<int> result = listToVector(sorted);
        assert(result == std::vector<int>({1, 2, 3, 4, 5}));
        deleteList(original);
        deleteList(sorted);
    }

    // Test 4: Duplicate values
    {
        ListNode* original = makeList({4, 2, 4, 1, 2});
        ListNode* sorted = flattenLinkedList(original);
        std::vector<int> result = listToVector(sorted);
        assert(result == std::vector<int>({1, 2, 2, 4, 4}));
        deleteList(original);
        deleteList(sorted);
    }

    // Test 5: Single node
    {
        ListNode* original = new ListNode(42);
        ListNode* sorted = flattenLinkedList(original);
        std::vector<int> result = listToVector(sorted);
        assert(result == std::vector<int>({42}));
        delete original;
        delete sorted;
    }

    // Test 6: Empty list (nullptr)
    {
        ListNode* original = nullptr;
        ListNode* sorted = flattenLinkedList(original);
        assert(sorted == nullptr);
        // No deletion needed
    }

    // Test 7: Two nodes unsorted
    {
        ListNode* original = makeList({10, -5});
        ListNode* sorted = flattenLinkedList(original);
        std::vector<int> result = listToVector(sorted);
        assert(result == std::vector<int>({-5, 10}));
        deleteList(original);
        deleteList(sorted);
    }

    // Test 8: Negative numbers and zeros
    {
        ListNode* original = makeList({0, -3, -1, 2, -3});
        ListNode* sorted = flattenLinkedList(original);
        std::vector<int> result = listToVector(sorted);
        assert(result == std::vector<int>({-3, -3, -1, 0, 2}));
        deleteList(original);
        deleteList(sorted);
    }

    return 0;
}
