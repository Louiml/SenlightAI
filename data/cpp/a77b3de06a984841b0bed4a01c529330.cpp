// Write a C++ function that takes two non-empty singly linked lists representing non-negative integers in reverse order (each node contains a single digit 0-9), and returns a new linked list representing their sum, also in reverse order. For example, the number 342 is represented as 2→4→3. The lists may have different lengths, and the sum may have one more digit than either input (e.g., 9→9 + 1→9 = 0→9→1 for 99+91=190). The function must not modify the input lists, must handle the carry properly, and must return a newly allocated list. You may assume the input lists are valid and non-null (non-empty), but each digit is within 0–9, and there are no leading zeros except when the number itself is zero (in which case the list is a single node with value 0).

#include <cassert>

// Helper to build a list from a vector-like initializer list (using manual allocation).
ListNode* makeList(const std::initializer_list<int>& digits) {
    ListNode* dummy = new ListNode(0);
    ListNode* tail = dummy;
    for (int d : digits) {
        tail->next = new ListNode(d);
        tail = tail->next;
    }
    ListNode* head = dummy->next;
    delete dummy;
    return head;
}

// Helper to compare two lists.
bool listEqual(const ListNode* a, const ListNode* b) {
    while (a && b) {
        if (a->val != b->val) return false;
        a = a->next;
        b = b->next;
    }
    return a == nullptr && b == nullptr;
}

// Helper to delete a list.
void deleteList(ListNode* head) {
    while (head) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // 342 + 465 = 807 -> [7,0,8]
    ListNode* a1 = makeList({2,4,3});
    ListNode* b1 = makeList({5,6,4});
    ListNode* r1 = addTwoNumbers(a1, b1);
    assert(listEqual(r1, makeList({7,0,8})));
    deleteList(a1); deleteList(b1); deleteList(r1);

    // 0 + 0 = 0 -> [0]
    ListNode* a2 = makeList({0});
    ListNode* b2 = makeList({0});
    ListNode* r2 = addTwoNumbers(a2, b2);
    assert(listEqual(r2, makeList({0})));
    deleteList(a2); deleteList(b2); deleteList(r2);

    // 999 + 1 = 1000 -> [0,0,0,1]
    ListNode* a3 = makeList({9,9,9});
    ListNode* b3 = makeList({1});
    ListNode* r3 = addTwoNumbers(a3, b3);
    assert(listEqual(r3, makeList({0,0,0,1})));
    deleteList(a3); deleteList(b3); deleteList(r3);

    // 1 + 99 = 100 -> [0,0,1]
    ListNode* a4 = makeList({1});
    ListNode* b4 = makeList({9,9});
    ListNode* r4 = addTwoNumbers(a4, b4);
    assert(listEqual(r4, makeList({0,0,1})));
    deleteList(a4); deleteList(b4); deleteList(r4);

    // 8 + 7 = 15 -> [5,1]
    ListNode* a5 = makeList({8});
    ListNode* b5 = makeList({7});
    ListNode* r5 = addTwoNumbers(a5, b5);
    assert(listEqual(r5, makeList({5,1})));
    deleteList(a5); deleteList(b5); deleteList(r5);

    // 0 + 123 = 123 -> [3,2,1]
    ListNode* a6 = makeList({0});
    ListNode* b6 = makeList({3,2,1});
    ListNode* r6 = addTwoNumbers(a6, b6);
    assert(listEqual(r6, makeList({3,2,1})));
    deleteList(a6); deleteList(b6); deleteList(r6);

    // Large: 123456789 + 987654321 = 1111111110 -> [0,1,1,1,1,1,1,1,1,1]
    ListNode* a7 = makeList({9,8,7,6,5,4,3,2,1});
    ListNode* b7 = makeList({1,2,3,4,5,6,7,8,9});
    ListNode* r7 = addTwoNumbers(a7, b7);
    assert(listEqual(r7, makeList({0,1,1,1,1,1,1,1,1,1})));
    deleteList(a7); deleteList(b7); deleteList(r7);

    return 0;
}

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

// Add two numbers represented as reversed linked lists.
// Returns a new linked list representing their sum (also reversed).
ListNode* addTwoNumbers(const ListNode* A, const ListNode* B) {
    ListNode* dummy = new ListNode(0);
    ListNode* tail = dummy;
    int carry = 0;

    const ListNode* pA = A;
    const ListNode* pB = B;

    while (pA != nullptr || pB != nullptr || carry != 0) {
        int sum = carry;
        if (pA != nullptr) {
            sum += pA->val;
            pA = pA->next;
        }
        if (pB != nullptr) {
            sum += pB->val;
            pB = pB->next;
        }

        carry = sum / 10;
        tail->next = new ListNode(sum % 10);
        tail = tail->next;
    }

    ListNode* result = dummy->next;
    delete dummy; // clean up the dummy node
    return result;
}

// The solution uses a dummy head node to simplify building the result list. We iterate through both input lists simultaneously using two pointers, always checking whether either pointer is still non-null or whether there is a remaining carry. At each step, we compute the sum of the current digits (0 if a list is exhausted) plus the carry from the previous step. The new digit is `sum % 10`, and the new carry is `sum / 10`. We append a new node with that digit to the result list and advance the pointers. After the loop, we return `dummy->next` (skipping the dummy). Edge cases include: one list being longer than the other (we treat missing digits as 0), the final carry producing an extra highest-order digit (e.g., 9+1 → carry 1), and inputs like 0+0. Time complexity is O(max(m, n)) where m and n are the lengths of the input lists, because we traverse each node at most once. Space complexity is O(max(m, n)+1) for the result list, plus O(1) auxiliary space (excluding the input and output). The function is `const`-correct because it does not modify the input list nodes.
