/*
Write a standalone C++ function that takes two singly linked lists representing non-negative integers in **least-significant-digit-first** order (e.g., the number 123 is stored as 3 → 2 → 1) and returns a new linked list representing the sum of the two numbers in the same digit order. The input lists may be empty (representing zero), may have different lengths, and may contain any number of nodes. The function must handle the final carry‑over when the sum exceeds the length of both lists, and must not mutate the input lists. The result list must contain no leading zeros except for the single node `0` when the sum is zero. Provide the function in a header‑ready form (no `main`), and then provide a test harness that verifies correctness on multiple cases including empty lists, unequal lengths, and a large final carry.
*/
#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Adds two numbers represented as linked lists (least-significant digit first).
// Returns a new list representing the sum. Input lists are not modified.
// Returns a single node with value 0 if both inputs are empty or sum to zero.
ListNode* addTwoNumbers(const ListNode* l1, const ListNode* l2) {
    ListNode dummy(0);          // Dummy head to simplify construction.
    ListNode* tail = &dummy;
    int carry = 0;

    // Continue while either list has nodes or there's a remaining carry.
    while (l1 != nullptr || l2 != nullptr || carry != 0) {
        int sum = carry;

        if (l1 != nullptr) {
            sum += l1->val;
            l1 = l1->next;
        }
        if (l2 != nullptr) {
            sum += l2->val;
            l2 = l2->next;
        }

        tail->next = new ListNode(sum % 10);
        tail = tail->next;
        carry = sum / 10;
    }

    return dummy.next;
}
#include <cassert>
#include <iostream>

// Helper to build a list from a vector (used in tests).
ListNode* buildList(const std::vector<int>& vals) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int v : vals) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper to convert a list to a vector (for comparison).
std::vector<int> toVector(const ListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// Helper to delete a list to avoid memory leaks.
void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Case 1: 342 + 465 = 807 (digits: 2->4->3 + 5->6->4)
    ListNode* l1 = buildList({2,4,3});
    ListNode* l2 = buildList({5,6,4});
    ListNode* res = addTwoNumbers(l1, l2);
    assert(toVector(res) == std::vector<int>({7,0,8}));
    deleteList(l1); deleteList(l2); deleteList(res);

    // Case 2: 0 + 0 = 0 (empty lists)
    l1 = nullptr;
    l2 = nullptr;
    res = addTwoNumbers(l1, l2);
    assert(toVector(res) == std::vector<int>({0}));
    deleteList(res);

    // Case 3: 999 + 1 = 1000 (digits: 9->9->9 + 1)
    l1 = buildList({9,9,9});
    l2 = buildList({1});
    res = addTwoNumbers(l1, l2);
    assert(toVector(res) == std::vector<int>({0,0,0,1}));
    deleteList(l1); deleteList(l2); deleteList(res);

    // Case 4: 0 + 123 = 123 (one empty list)
    l1 = nullptr;
    l2 = buildList({3,2,1});
    res = addTwoNumbers(l1, l2);
    assert(toVector(res) == std::vector<int>({3,2,1}));
    deleteList(l2); deleteList(res);

    // Case 5: 5 + 5 = 10 (single digit carry)
    l1 = buildList({5});
    l2 = buildList({5});
    res = addTwoNumbers(l1, l2);
    assert(toVector(res) == std::vector<int>({0,1}));
    deleteList(l1); deleteList(l2); deleteList(res);

    // Case 6: Large numbers with different lengths and leading zeros in input
    l1 = buildList({0,0,1});  // represents 100
    l2 = buildList({0,9});    // represents 90
    res = addTwoNumbers(l1, l2);
    assert(toVector(res) == std::vector<int>({0,9,1})); // 190
    deleteList(l1); deleteList(l2); deleteList(res);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The core algorithm traverses both lists simultaneously while maintaining a `carry` variable. At each step, compute `sum = carry + (value from l1 if present) + (value from l2 if present)`. The next digit in the result is `sum % 10`, and the new carry is `sum / 10`. Continue until both lists are exhausted **and** the carry is zero. Handle empty lists by treating missing nodes as having value 0, which naturally unifies the loop condition. The final carry is handled inside the loop because the loop condition includes `carry != 0`. Since the problem guarantees non-negative integers with possibly leading zeros in the input (e.g., `0 -> 0` representing 0), no special handling is needed for trailing zeros in the result beyond the natural construction. Edge cases: both lists null (sum = 0), one list shorter, and a carry that propagates beyond the longer list (e.g., 9+1). Time complexity is O(max(len1, len2)) because we visit each node at most once. Space complexity is O(max(len1, len2)) for the result list, plus O(1) auxiliary space (excluding the result).
