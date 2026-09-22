// Write a C++ free function `addTwoNumbers(ListNode* l1, ListNode* l2)` that takes two singly linked lists representing non-negative integers in **reverse order** (each node stores a single digit 0–9, and the most significant digit is at the end; the lists do not contain leading zeros except when the number itself is zero). The function must return a new linked list representing the sum of the two numbers, also stored in reverse order. Use the provided `ListNode` struct definition (value + `next` pointer). The input lists must not be modified. Handle cases where the numbers have different lengths and where the final carry produces an extra digit. The returned list must be newly allocated; do not reuse nodes from the input lists. Also provide a helper to convert a vector of digits into a linked list and a helper to compare two linked lists for equality, but those helpers should be placed outside the solution function (in the test section).
// The problem is the classic “add two numbers” linked-list problem. The main algorithm iterates through both lists simultaneously, adding corresponding digits along with a carry. Since the lists are already in reverse order (least significant digit first), we process from the head to the tail naturally. At each step: compute `total = first->val + second->val + carry`, the new digit is `total % 10`, and the new carry is `total / 10`. We create a new node for that digit and append it to the result list. After one list ends, we continue with the remaining list using a helper that adds the remaining digits plus the carry. If after processing all nodes a carry remains (e.g., 9+9+1=19 → digit 9, carry 1), we append one extra node with that carry. Edge cases include: one list being null initially (though problem guarantees non-empty? treat as zero if null), lists of different lengths, and the final carry. We prepend a dummy head to simplify appending, then return `head->next`. Time complexity is O(max(n,m)) where n and m are the lengths of the two lists. Space complexity is O(max(n,m)) for the newly allocated result list (excluding the input lists). The function does not alter the input lists.
#include <cstddef> // for nullptr

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Adds the remaining nodes of `current` (which may be null) to `result`,
// propagating `carry`. Returns the new tail pointer.
ListNode* addRemaining(ListNode* result, ListNode* current, int carry) {
    while (current != nullptr) {
        int total = current->val + carry;
        int digit = total % 10;
        carry = total / 10;
        result->next = new ListNode(digit);
        result = result->next;
        current = current->next;
    }
    if (carry > 0) {
        result->next = new ListNode(carry);
        result = result->next; // optional, but consistent
    }
    return result;
}

// Adds two numbers represented as linked lists in reverse order.
// Returns a new linked list (reverse order) representing the sum.
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode dummyHead(0); // dummy head to simplify appending
    ListNode* tail = &dummyHead;
    int carry = 0;

    // Process while both lists have nodes
    while (l1 != nullptr && l2 != nullptr) {
        int total = l1->val + l2->val + carry;
        int digit = total % 10;
        carry = total / 10;
        tail->next = new ListNode(digit);
        tail = tail->next;
        l1 = l1->next;
        l2 = l2->next;
    }

    // Process the longer list (if any)
    ListNode* remaining = (l1 != nullptr) ? l1 : l2;
    tail = addRemaining(tail, remaining, carry);

    return dummyHead.next;
}
#include <cassert>
#include <vector>

// Helper: build a linked list from a vector of digits (reverse order representation).
ListNode* fromVector(const std::vector<int>& digits) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int d : digits) {
        tail->next = new ListNode(d);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper: convert linked list to vector for easy comparison.
std::vector<int> toVector(ListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// Helper: free a linked list (not required for correctness but clean).
void freeList(ListNode* head) {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: 342 + 465 = 807  (represented as 2->4->3 and 5->6->4)
    ListNode* a = fromVector({2, 4, 3});
    ListNode* b = fromVector({5, 6, 4});
    ListNode* sum = addTwoNumbers(a, b);
    std::vector<int> result = toVector(sum);
    assert(result == std::vector<int>({7, 0, 8}));
    freeList(a); freeList(b); freeList(sum);

    // Test 2: 0 + 0 = 0
    a = fromVector({0});
    b = fromVector({0});
    sum = addTwoNumbers(a, b);
    result = toVector(sum);
    assert(result == std::vector<int>({0}));
    freeList(a); freeList(b); freeList(sum);

    // Test 3: 999 + 1 = 1000  (represented as 9->9->9 and 1)
    a = fromVector({9, 9, 9});
    b = fromVector({1});
    sum = addTwoNumbers(a, b);
    result = toVector(sum);
    assert(result == std::vector<int>({0, 0, 0, 1}));
    freeList(a); freeList(b); freeList(sum);

    // Test 4: 123 + 456 = 579 (same lengths, no carry)
    a = fromVector({3, 2, 1});
    b = fromVector({6, 5, 4});
    sum = addTwoNumbers(a, b);
    result = toVector(sum);
    assert(result == std::vector<int>({9, 7, 5}));
    freeList(a); freeList(b); freeList(sum);

    // Test 5: 5 + 5 = 10 (single digit with carry)
    a = fromVector({5});
    b = fromVector({5});
    sum = addTwoNumbers(a, b);
    result = toVector(sum);
    assert(result == std::vector<int>({0, 1}));
    freeList(a); freeList(b); freeList(sum);

    // Test 6: 99 + 99 = 198 (two digits, carry propagation)
    a = fromVector({9, 9});
    b = fromVector({9, 9});
    sum = addTwoNumbers(a, b);
    result = toVector(sum);
    assert(result == std::vector<int>({8, 9, 1}));
    freeList(a); freeList(b); freeList(sum);

    // Test 7: 1 + 999 = 1000 (different lengths, final carry)
    a = fromVector({1});
    b = fromVector({9, 9, 9});
    sum = addTwoNumbers(a, b);
    result = toVector(sum);
    assert(result == std::vector<int>({0, 0, 0, 1}));
    freeList(a); freeList(b); freeList(sum);

    // Test 8: 0 + 567 = 567 (one list is zero)
    a = fromVector({0});
    b = fromVector({7, 6, 5});
    sum = addTwoNumbers(a, b);
    result = toVector(sum);
    assert(result == std::vector<int>({7, 6, 5}));
    freeList(a); freeList(b); freeList(sum);

    return 0;
}
