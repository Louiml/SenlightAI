/*
Write a C++ function that takes two singly linked lists representing non-negative integers with exactly three digits each (least significant digit first) and returns a new singly linked list representing their sum, where each node holds a single decimal digit and the most significant digit is stored first. The input lists are non-empty, contain exactly 3 nodes, and the sum is guaranteed to fit in 4 digits. The function should not modify the input lists and should work for any valid three-digit inputs given least-significant-first order.
*/
#include <stdexcept>

// Definition for a singly-linked list node.
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Convert a least-significant-first linked list (exactly 3 digits) to an integer.
int listToValue(const ListNode* head) {
    int value = 0;
    int multiplier = 1;
    const ListNode* current = head;
    for (int i = 0; i < 3; ++i) {
        if (!current) {
            throw std::invalid_argument("List must have at least 3 nodes.");
        }
        value += current->val * multiplier;
        multiplier *= 10;
        current = current->next;
    }
    return value;
}

// Build a new linked list from an integer, most significant digit first.
ListNode* intToList(int value) {
    ListNode* result = nullptr;
    ListNode* tail = nullptr;
    do {
        int digit = value % 10;
        ListNode* node = new ListNode(digit);
        if (result == nullptr) {
            result = node;
            tail = node;
        } else {
            node->next = result;
            result = node;
        }
        value /= 10;
    } while (value > 0);
    return result;
}

// Add two three-digit numbers stored least-significant-first and return the sum as a list.
// The result list stores digits most significant first.
ListNode* addLists(const ListNode* a, const ListNode* b) {
    int sum = listToValue(a) + listToValue(b);
    return intToList(sum);
}
#include <cassert>

// Helper to create a list from an initializer list (least significant first).
ListNode* makeList(std::initializer_list<int> digits) {
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    for (int d : digits) {
        ListNode* node = new ListNode(d);
        if (!head) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

// Helper to compare two lists (most significant first).
bool listsEqual(ListNode* a, ListNode* b) {
    while (a && b) {
        if (a->val != b->val) return false;
        a = a->next;
        b = b->next;
    }
    return a == nullptr && b == nullptr;
}

// Helper to free a list.
void freeList(ListNode* head) {
    while (head) {
        ListNode* tmp = head->next;
        delete head;
        head = tmp;
    }
}

int main() {
    // 563 + 248 = 811 -> list: 8->1->1->X
    ListNode* a = makeList({3, 6, 5}); // 563 least significant first
    ListNode* b = makeList({8, 4, 2}); // 248 least significant first
    ListNode* result = addLists(a, b);
    assert(listsEqual(result, makeList({8, 1, 1})));
    freeList(a); freeList(b); freeList(result);

    // 100 + 200 = 300 -> list: 3->0->0->X
    a = makeList({0, 0, 1});
    b = makeList({0, 0, 2});
    result = addLists(a, b);
    assert(listsEqual(result, makeList({3, 0, 0})));
    freeList(a); freeList(b); freeList(result);

    // 999 + 1 = 1000 -> list: 1->0->0->0->X
    a = makeList({9, 9, 9});
    b = makeList({1, 0, 0});
    result = addLists(a, b);
    assert(listsEqual(result, makeList({1, 0, 0, 0})));
    freeList(a); freeList(b); freeList(result);

    // 000 + 000 = 0 -> list: 0->X
    a = makeList({0, 0, 0});
    b = makeList({0, 0, 0});
    result = addLists(a, b);
    assert(listsEqual(result, makeList({0})));
    freeList(a); freeList(b); freeList(result);

    // 500 + 500 = 1000
    a = makeList({0, 0, 5});
    b = makeList({0, 0, 5});
    result = addLists(a, b);
    assert(listsEqual(result, makeList({1, 0, 0, 0})));
    freeList(a); freeList(b); freeList(result);

    return 0;
}
// The solution must first reconstruct the integer value of each input list by interpreting the node order as least significant digit first: for a list with nodes `d0`, `d1`, `d2`, the value is `d0 + 10*d1 + 100*d2`. After computing the two numbers, add them to obtain the sum. Then convert the sum into a new linked list where the most significant digit appears first. This can be done by repeatedly extracting the last digit (mod 10) and prepending it to the result list, then dividing the sum by 10 until it becomes zero. Because the sum fits in 4 digits, the loop runs at most 4 times. Edge cases include sums with fewer than 4 digits (e.g., 100+200=300, which should produce a list `3->0->0->X`) and sums that exactly reach 4 digits (e.g., 999+1=1000). The input lists are traversed once each to compute the values (O(3) time each), and the result list construction takes O(4) time, so total time is O(1) in practice (constant number of nodes). Auxiliary space is O(4) for the result list, excluding input storage.
