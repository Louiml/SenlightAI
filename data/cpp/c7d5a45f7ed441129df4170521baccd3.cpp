// Given two non-empty singly-linked lists representing non-negative integers in reverse digit order (e.g., the integer 342 is represented as 2 → 4 → 3), write a C++ function `ListNode* addTwoNumbers(ListNode* l1, ListNode* l2)` that returns the sum of the two numbers as a new linked list in the same reverse digit order. Each node holds a single digit (0-9), and the lists may have differing lengths. The most significant digit cannot be zero unless the number itself is zero. Handle cases where the sum produces an extra final carry digit (e.g., 5 + 5 = 10, resulting in 0 → 1). You must implement the solution without modifying the input lists and without converting the entire numbers to an integer type (they may be arbitrarily large). Provide a standalone function only (no `main`), but include the `ListNode` struct definition and all necessary headers.

#include <cassert>

// Helper to build a list from initializer list (for testing only)
ListNode* makeList(std::initializer_list<int> vals) {
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    for (int v : vals) {
        ListNode* n = new ListNode(v);
        if (!head) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
    }
    return head;
}

// Helper to convert list to vector for comparison, also frees memory
std::vector<int> listToVector(ListNode* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->val);
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
    return result;
}

int main() {
    // Example: 342 + 465 = 807 -> [7,0,8]
    ListNode* l1 = makeList({2,4,3});
    ListNode* l2 = makeList({5,6,4});
    ListNode* sum = addTwoNumbers(l1, l2);
    assert(listToVector(sum) == std::vector<int>({7,0,8}));

    // Unequal lengths: 999 + 1 = 1000 -> [0,0,0,1]
    l1 = makeList({9,9,9});
    l2 = makeList({1});
    sum = addTwoNumbers(l1, l2);
    assert(listToVector(sum) == std::vector<int>({0,0,0,1}));

    // Both single digits with carry: 5+5=10 -> [0,1]
    l1 = makeList({5});
    l2 = makeList({5});
    sum = addTwoNumbers(l1, l2);
    assert(listToVector(sum) == std::vector<int>({0,1}));

    // Zero + zero
    l1 = makeList({0});
    l2 = makeList({0});
    sum = addTwoNumbers(l1, l2);
    assert(listToVector(sum) == std::vector<int>({0}));

    // Very long lists (simulate large number) e.g., 100...0 (1 followed by 5 zeros) + 1 = 100001 -> [1,0,0,0,0,1]
    l1 = makeList({1,0,0,0,0,1}); // 100001 reversed
    l2 = makeList({0}); // 0
    sum = addTwoNumbers(l1, l2);
    assert(listToVector(sum) == std::vector<int>({1,0,0,0,0,1}));

    // Complex carry chain: 999 + 999 = 1998 -> [8,9,9,1]
    l1 = makeList({9,9,9});
    l2 = makeList({9,9,9});
    sum = addTwoNumbers(l1, l2);
    assert(listToVector(sum) == std::vector<int>({8,9,9,1}));

    // One list is empty (should not happen per spec, but test robustness)
    l1 = makeList({7,8});
    l2 = nullptr;
    sum = addTwoNumbers(l1, l2);
    assert(listToVector(sum) == std::vector<int>({7,8}));

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

// Add two numbers represented as reversed linked lists of digits.
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    // Helper to append a new node at the tail of the result list.
    auto insertAtTail = [](ListNode*& head, ListNode*& tail, int value) {
        ListNode* newNode = new ListNode(value);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    };

    int carry = 0;
    ListNode* head = nullptr;
    ListNode* tail = nullptr;

    while (l1 != nullptr || l2 != nullptr || carry != 0) {
        int digit1 = (l1 != nullptr) ? l1->val : 0;
        int digit2 = (l2 != nullptr) ? l2->val : 0;
        int sum = digit1 + digit2 + carry;
        int digit = sum % 10;
        carry = sum / 10;

        insertAtTail(head, tail, digit);

        if (l1 != nullptr) l1 = l1->next;
        if (l2 != nullptr) l2 = l2->next;
    }

    return head;
}

// The algorithm simulates manual addition from least significant digit to most significant digit, which matches the linked list order. Initialize a carry variable to 0. Traverse both lists simultaneously. At each step, extract the current digit value from each node (if a list is already exhausted, treat its digit as 0). Compute `sum = val1 + val2 + carry`. The new digit is `sum % 10`, and the new carry is `sum / 10`. Append the digit to the result list using a tail insertion helper to preserve order. After processing both lists, if the carry is nonzero, append it as the final digit. Edge cases include: lists of unequal lengths, one list being shorter, both lists empty (though the problem states non-empty inputs, the function should still handle gracefully), and a final carry that extends the result (e.g., 999 + 1 = 1000 → 0 → 0 → 0 → 1). Time complexity is O(max(m, n)) since we visit each node at most once. Space complexity is O(max(m, n)) for the result list, ignoring input and output list storage. The input lists remain unmodified, and the function returns a newly allocated list.
