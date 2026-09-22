// Write a C++ function that takes two singly-linked lists of non-negative digits (each node stores a single digit 0-9), where the digits are stored in reverse order so that the 1's place is at the head of each list. Return a new singly-linked list representing the sum of the two numbers, also stored in reverse order. The lists may have different lengths, and the sum may require an extra carry digit at the most significant position (i.e., appended at the tail of the result). The input lists should not be modified, and the function must handle empty lists (representing the number 0) gracefully.
The algorithm simulates manual digit-by-digit addition from the least significant digit to the most significant, which corresponds to traversing both lists from head to tail simultaneously. Maintain a `carry` variable initialized to 0. At each step, extract the current digit from each list if available (otherwise treat as 0), compute the sum `v1 + v2 + carry`, set the new carry to `sum / 10`, and create a new node for the digit `sum % 10` appended to the result list. Continue until both lists are exhausted. After the loop, if a carry remains, append an extra node for it. Edge cases include lists of unequal length (one list ends earlier), one list being empty (sum equals the other number), and a final carry creating an extra digit (e.g., 5 + 5 = 10). Time complexity is O(max(n, m)) where n and m are the lengths of the input lists, and space complexity is O(max(n, m) + 1) for the result list. The solution does not modify the inputs, so for better const-correctness, the function should accept `const ListNode*` parameters.
#include <cstddef>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Add two numbers represented as reversed digit lists, returning a new reversed digit list.
ListNode* addTwoNumbers(const ListNode* l1, const ListNode* l2) {
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    int carry = 0;

    while (l1 != nullptr || l2 != nullptr) {
        int digit1 = (l1 != nullptr) ? l1->val : 0;
        int digit2 = (l2 != nullptr) ? l2->val : 0;
        int sum = digit1 + digit2 + carry;
        carry = sum / 10;
        int digit = sum % 10;

        ListNode* newNode = new ListNode(digit);
        if (head == nullptr) {
            head = newNode;
        } else {
            tail->next = newNode;
        }
        tail = newNode;

        if (l1 != nullptr) l1 = l1->next;
        if (l2 != nullptr) l2 = l2->next;
    }

    if (carry > 0) {
        ListNode* newNode = new ListNode(carry);
        if (tail != nullptr) {
            tail->next = newNode;
        } else {
            head = newNode;
        }
    }

    return head;
}
#include <cassert>

// Helper to build a list from a vector of digits (in reverse order as per problem).
ListNode* buildList(const std::vector<int>& digits) {
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    for (int d : digits) {
        ListNode* node = new ListNode(d);
        if (head == nullptr) {
            head = node;
        } else {
            tail->next = node;
        }
        tail = node;
    }
    return head;
}

// Helper to convert a list to a vector (reading from head to tail).
std::vector<int> listToVector(const ListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// Helper to delete a list.
void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // 342 + 465 = 807 -> reversed: [7,0,8]
    ListNode* l1 = buildList({2,4,3});
    ListNode* l2 = buildList({5,6,4});
    ListNode* sum1 = addTwoNumbers(l1, l2);
    assert((listToVector(sum1) == std::vector<int>{7,0,8}));
    deleteList(l1); deleteList(l2); deleteList(sum1);

    // 0 + 0 = 0 -> reversed: [0]
    ListNode* l3 = buildList({0});
    ListNode* l4 = buildList({0});
    ListNode* sum2 = addTwoNumbers(l3, l4);
    assert((listToVector(sum2) == std::vector<int>{0}));
    deleteList(l3); deleteList(l4); deleteList(sum2);

    // 9999 + 1 = 10000 -> reversed: [0,0,0,0,1]
    ListNode* l5 = buildList({9,9,9,9});
    ListNode* l6 = buildList({1});
    ListNode* sum3 = addTwoNumbers(l5, l6);
    assert((listToVector(sum3) == std::vector<int>{0,0,0,0,1}));
    deleteList(l5); deleteList(l6); deleteList(sum3);

    // 5 + 5 = 10 -> reversed: [0,1]
    ListNode* l7 = buildList({5});
    ListNode* l8 = buildList({5});
    ListNode* sum4 = addTwoNumbers(l7, l8);
    assert((listToVector(sum4) == std::vector<int>{0,1}));
    deleteList(l7); deleteList(l8); deleteList(sum4);

    // 12 + 3 = 15 -> reversed: [2,1] + [3] = [5,1]
    ListNode* l9 = buildList({2,1});
    ListNode* l10 = buildList({3});
    ListNode* sum5 = addTwoNumbers(l9, l10);
    assert((listToVector(sum5) == std::vector<int>{5,1}));
    deleteList(l9); deleteList(l10); deleteList(sum5);

    // Empty list (representing 0) + [9,9] (99) = 99 -> reversed: [9,9]
    ListNode* l11 = nullptr; // represents 0
    ListNode* l12 = buildList({9,9});
    ListNode* sum6 = addTwoNumbers(l11, l12);
    assert((listToVector(sum6) == std::vector<int>{9,9}));
    deleteList(l12); deleteList(sum6);

    return 0;
}
