// Write a C++ function `addTwoNumbersReversed(ListNode* l1, ListNode* l2)` that takes two singly linked lists whose nodes store the digits of two non-negative integers in **reverse order** (i.e., the least significant digit is at the head), and returns a new singly linked list representing their sum, also in reverse order. Each node contains a single decimal digit (0-9). The result list must not have leading zeros unless the sum is exactly zero. You may assume the input lists are non-empty and contain valid digits. Do not modify the input lists. The function must use only O(1) extra space beyond the output list (i.e., you may not convert the numbers to integers or use arrays/stacks). The returned list must be dynamically allocated using `new ListNode(...)`.

// The classic addition algorithm processes digits from least significant to most significant, which matches the reverse-order storage directly. We traverse both lists simultaneously, maintaining a carry. For each pair of nodes (or when one list is exhausted), we compute `sum = carry + (digit from l1) + (digit from l2)`. The new digit is `sum % 10`, and the updated carry is `sum / 10`. We append the new digit to the tail of the result list (using a dummy head to simplify insertion). After both lists are exhausted, if carry remains (which can only be 1), we append a new node with value 1. Edge cases include: one list being longer than the other (handle by treating missing digits as 0), the final carry producing an extra most significant digit (e.g., 9+1=10 → result "0 1"), and the degenerate case where both inputs are zero (result should be a single node with value 0). The algorithm runs in O(n + m) time where n and m are the lengths of the input lists, and uses O(max(n,m)) space for the output list, plus O(1) auxiliary space for pointers and the carry.

#include <cstddef>   // for nullptr

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Adds two numbers represented as linked lists in reverse order.
// Returns a new list representing the sum in reverse order.
ListNode* addTwoNumbersReversed(const ListNode* l1, const ListNode* l2) {
    ListNode dummy(0);          // dummy head to simplify tail insertion
    ListNode* tail = &dummy;
    int carry = 0;

    const ListNode* p1 = l1;
    const ListNode* p2 = l2;

    while (p1 != nullptr || p2 != nullptr || carry != 0) {
        int sum = carry;
        if (p1 != nullptr) {
            sum += p1->val;
            p1 = p1->next;
        }
        if (p2 != nullptr) {
            sum += p2->val;
            p2 = p2->next;
        }

        carry = sum / 10;
        tail->next = new ListNode(sum % 10);
        tail = tail->next;
    }

    return dummy.next;
}

#include <cassert>

// Helper to build a list from an initializer list (for tests)
ListNode* makeList(std::initializer_list<int> digits) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int d : digits) {
        tail->next = new ListNode(d);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper to compare two lists
bool listsEqual(const ListNode* a, const ListNode* b) {
    while (a && b) {
        if (a->val != b->val) return false;
        a = a->next;
        b = b->next;
    }
    return (a == nullptr && b == nullptr);
}

// Helper to delete a list (avoid memory leaks in tests)
void deleteList(ListNode*& head) {
    while (head) {
        ListNode* tmp = head;
        head = head->next;
        delete tmp;
    }
}

int main() {
    // 342 + 465 = 807  -> lists: 2->4->3 and 5->6->4 => 7->0->8
    ListNode* l1 = makeList({2, 4, 3});
    ListNode* l2 = makeList({5, 6, 4});
    ListNode* result = addTwoNumbersReversed(l1, l2);
    assert(listsEqual(result, makeList({7, 0, 8})));
    deleteList(l1); deleteList(l2); deleteList(result);

    // 0 + 0 = 0
    l1 = makeList({0});
    l2 = makeList({0});
    result = addTwoNumbersReversed(l1, l2);
    assert(listsEqual(result, makeList({0})));
    deleteList(l1); deleteList(l2); deleteList(result);

    // 999 + 1 = 1000 -> 9->9->9 and 1 => 0->0->0->1
    l1 = makeList({9, 9, 9});
    l2 = makeList({1});
    result = addTwoNumbersReversed(l1, l2);
    assert(listsEqual(result, makeList({0, 0, 0, 1})));
    deleteList(l1); deleteList(l2); deleteList(result);

    // 5 + 5 = 10 -> 5 and 5 => 0->1
    l1 = makeList({5});
    l2 = makeList({5});
    result = addTwoNumbersReversed(l1, l2);
    assert(listsEqual(result, makeList({0, 1})));
    deleteList(l1); deleteList(l2); deleteList(result);

    // 123 + 4567 = 4690 -> 3->2->1 and 7->6->5->4 => 0->9->6->4
    l1 = makeList({3, 2, 1});
    l2 = makeList({7, 6, 5, 4});
    result = addTwoNumbersReversed(l1, l2);
    assert(listsEqual(result, makeList({0, 9, 6, 4})));
    deleteList(l1); deleteList(l2); deleteList(result);

    return 0;
}
